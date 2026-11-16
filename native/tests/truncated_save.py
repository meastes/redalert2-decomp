#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest native-truncated-save (lane C15e; codex4 R684/R714): a damaged port save is refused
cleanly, never a fault.

In a fresh write folder under --work the test:
1. Makes a port save with the skirmish harness (RA2_NATIVE_SAVE_AT, GOOD.SAV at frame 40).
2. Writes TRUNC.SAV beside it: the same file without its last 4,096 bytes, as R684's
   tail_cut did.
3. Loads TRUNC.SAV through the menus (--load-save: Single Player > Load Saved Game, Load).
   It must be refused:
   - the storage open fails ("not a native save storage");
   - the game's "Error loading game" box is shown and its OK clicked;
   - the load list is shown again and its Back leads to the Single Player menu;
   - the script then quits with exit 0;
   - no fault, trap or swizzle failure, and TRUNC.SAV is unchanged.
4. Loads GOOD.SAV the same way and plays 30 frames (the control: the refusal is the
   truncation's).
The game data are read in place; the saves exist only under --work. Skipped (77) without
the installed game or, for the Windows build, without Wine.

  truncated_save.py --binary <ra2-native> --data-dir <game folder> --game-exe <game.exe> --work <dir>
                    [--wine-prefix <dir> [--wine <wine>]]
"""
import argparse
import hashlib
import os
import re
import shutil
import subprocess
import sys

import game_process
import wine as winehost

SKIP = 77
BAD = re.compile(r'unrecovered|trap|signal \d+|SIG(SEGV|BUS|ILL|ABRT|FPE)|crash:|swizzle: unmatched|abort', re.I)
# The error box's OK (control 1454, 597,505 126x25), the load list's Back (1670, 756,619
# 156x42), at 1024x768 (RA2_NATIVE_MSG_LOG). Dialogs: 1 main menu, 2 Single Player, 3-4 the
# load list made and hidden for the load, 5 the error box, 6 the list again, 7 Single Player.
REFUSE_SCRIPT = """s5+1000 click 660 517
s6+1000 click 834 640
s7+1000 quit
"""


def run(a, prefix, env, argv, seconds):
    """(output, exit code) of one launch; under Wine a launch the Mac host killed runs again."""
    out, code = '', -1
    for attempt in range(winehost.HOST_RETRIES + 1):
        cmd = prefix.command(argv) if prefix else argv
        e = prefix.env(env) if prefix else env
        try:
            p = game_process.run(cmd, env=e, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, errors='replace',
                                 timeout=seconds + 60)
            out, code, timed_out = p.stdout, p.returncode, False
        except subprocess.TimeoutExpired as x:
            out = x.stdout.decode('utf-8', 'replace') if isinstance(x.stdout, bytes) else (x.stdout or '')
            code, timed_out = -1, True
        why = winehost.host_failure(out, timed_out, code) if prefix else ''
        if not why or attempt == winehost.HOST_RETRIES:
            break
        print('truncated-save: %s; launching again' % why)
    return out, code


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--game-exe', default='')
    ap.add_argument('--work', required=True)
    ap.add_argument('--wine-prefix', default='')
    ap.add_argument('--wine', default='')
    a = ap.parse_args()
    if not a.data_dir or not os.path.isdir(a.data_dir) or not a.game_exe or not os.path.isfile(a.game_exe):
        print('truncated-save: skipped (set RA2_NATIVE_CENSUS_DATA_DIR and RA2_NATIVE_CENSUS_GAME_EXE)')
        return SKIP
    prefix = None
    if a.wine_prefix:
        wine = winehost.find_wine(a.wine)
        if not wine:
            print('truncated-save: skipped (Windows build: no Wine; set RA2_NATIVE_WINE)')
            return SKIP
        prefix = winehost.Prefix(wine, a.wine_prefix)
    work = os.path.abspath(a.work)
    shutil.rmtree(work, ignore_errors=True)
    write = os.path.join(work, 'write')
    os.makedirs(write)
    env = dict(os.environ, RA2_GAME_EXE=a.game_exe, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_CRASH_TRACE='1',
               RA2_NATIVE_IGNORE_FOCUS='1')
    for k in ('RA2_NATIVE_TRAP_CONTINUE', 'RA2_NATIVE_SAVE_AT', 'RA2_NATIVE_SAVE_LEAVE'):
        env.pop(k, None)
    common = [a.binary, '--data-dir', a.data_dir, '--write-dir=' + write]
    bad = []
    try:
        # 1. The port save.
        out, code = run(a, prefix, dict(env, RA2_NATIVE_SAVE_AT='40,GOOD.SAV'),
                        common + ['--run-to=frames', '--run-seconds=240', '--game-args=-noaudio -CD.', '--skirmish=#5',
                                  '--skirmish-frames=60'], 240)
        open(os.path.join(work, 'save.log'), 'w').write(out)
        good = os.path.join(write, 'GOOD.SAV')
        if code != 0 or not os.path.isfile(good):
            print('truncated-save: FAILED: the harness save was not made (exit %d; see %s)' % (code, os.path.join(work, 'save.log')))
            return 1
        # 2. The truncated copy.
        data = open(good, 'rb').read()
        trunc = os.path.join(write, 'TRUNC.SAV')
        open(trunc, 'wb').write(data[:-4096])
        before = hashlib.sha256(open(trunc, 'rb').read()).hexdigest()
        # 3. Refused.
        script = os.path.join(work, 'refuse.script')
        open(script, 'w').write(REFUSE_SCRIPT)
        out, code = run(a, prefix, dict(env, RA2_NATIVE_MSG_LOG='1', RA2_NATIVE_STG_LOG='1'),
                        common + ['--run-to=never', '--run-seconds=120', '--game-args=-noaudio -CD.',
                                  '--load-save=TRUNC.SAV', '--input-script=' + script], 120)
        open(os.path.join(work, 'refuse.log'), 'w').write(out)
        lines = out.splitlines()
        refused = next((i for i, l in enumerate(lines) if l.startswith('resume: could not load TRUNC.SAV')), None)
        after = lines[refused:] if refused is not None else []
        if code != 0:
            bad.append('refused load: exit %d (the script quits after Back; see refuse.log)' % code)
        if not any('TRUNC.SAV: not a native save storage' in l for l in lines):
            bad.append('the truncated save\'s storage was not refused ("not a native save storage")')
        if refused is None:
            bad.append('no "resume: could not load TRUNC.SAV"')
        if not any('"GUI:OK"' in l and l.strip().startswith('control 1454') for l in after):
            bad.append('no error box (its OK) after the refused load')
        ok_at = next((i for i, l in enumerate(after) if l.strip().startswith('control 1454') and '"GUI:OK"' in l), None)
        if ok_at is None or not any('"GUI:SinglePlayerMenu"' in l for l in after[ok_at:]):
            bad.append('the Single Player menu was not shown again after OK and Back')
        bad += ['refused load: ' + l for l in lines if BAD.search(l)][:10]
        if hashlib.sha256(open(trunc, 'rb').read()).hexdigest() != before:
            bad.append('TRUNC.SAV was changed')
        # 4. The control.
        out, code = run(a, prefix, env, common + ['--run-to=frames', '--frames=30', '--run-seconds=180',
                                                  '--game-args=-noaudio -CD.', '--load-save=GOOD.SAV'], 180)
        open(os.path.join(work, 'control.log'), 'w').write(out)
        if code != 0 or 'resume: loaded GOOD.SAV' not in out or \
                not re.search(r'run-to: target frames; stopped: target reached', out):
            bad.append('control: GOOD.SAV did not load and play 30 frames (exit %d; see control.log)' % code)
        bad += ['control: ' + l for l in out.splitlines() if BAD.search(l)][:10]
    finally:
        if prefix:
            prefix.close()
    for b in bad:
        print('truncated-save: ' + b)
    print('truncated-save: %s (see %s)' % ('ok' if not bad else 'FAILED', work))
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main())
