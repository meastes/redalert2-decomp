#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest native-exit-route (lane C15f; codex4 R710/R719/R722): the game ends cleanly by the
menus after a loaded game was abandoned.

codex4's route on real Windows: Load a saved game, play, Escape, Abort Mission, Quit, the
skirmish setup's Back, Single Player's Back, the main menu's Exit Game, OK. On older drops it
ended once with STATUS_HEAP_CORRUPTION (0xC0000374, R710, 7cc1eb111) and once with a SIGSEGV in
the teardown (ConvertClass::Delete_Blitters from the ColorScheme destructors, R719,
becbf2fcb). In a fresh write folder under --work the test makes a port save with the
skirmish harness at Game interface size 1.5 (the failing runs' setting), then takes the route
through the menus in a new process at the same size and requires:
- the save loaded and the main menu shown again after the game (menu-return);
- WinMain returning 0 and the process (its exit-time teardown: Prog_End, the static
  destructors) exiting 0;
- no fault, trap or swizzle failure, and no blit reaching outside its surface.
Under Wine a heap corruption the Windows heap detects ends the process nonzero, so the test
fails on it. Skipped (77) without the installed game or, for the Windows build, without Wine.

  exit_route.py --binary <ra2-native> --data-dir <game folder> --game-exe <game.exe> --work <dir>
                [--wine-prefix <dir> [--wine <wine>]]
"""
import argparse
import os
import re
import shutil
import subprocess
import sys

import wine as winehost

SKIP = 77
BAD = re.compile(r'unrecovered|trap|signal \d+|SIG(SEGV|BUS|ILL|ABRT|FPE)|crash:|swizzle: unmatched|reaches outside', re.I)
# At 1024x768 (RA2_NATIVE_MSG_LOG): in game frame 100 Escape; the options menu's Abort Mission
# (1314, 877,327 125x25); the question's Quit (1737, 877,227); the skirmish setup's Back (1472,
# 756,619 156x42); Single Player's Main Menu (1670, the same slot); the main menu's Exit Game
# (1006, the same slot); the box's OK (1454, 597,472 126x26). Dialogs: 1 main menu, 2 Single
# Player, 3-4 the load list (resume), 5 the options menu, 6 the question, 7 the skirmish setup,
# 8 Single Player, 9 the main menu, 10 the box. The fallback quit fires only if the route stalls.
SCRIPT = """100 key Escape
s5+1000 click 939 339
s6+1000 click 939 239
s7+1000 click 834 640
s8+1000 click 834 640
s9+1000 click 834 640
s10+1000 click 660 485
s10+30000 quit
"""


def run(prefix, env, argv, seconds):
    out, code = '', -1
    for attempt in range(winehost.HOST_RETRIES + 1):
        cmd = prefix.command(argv) if prefix else argv
        e = prefix.env(env) if prefix else env
        try:
            p = subprocess.run(cmd, env=e, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, errors='replace',
                               timeout=seconds + 60)
            out, code, timed_out = p.stdout, p.returncode, False
        except subprocess.TimeoutExpired as x:
            out = x.stdout.decode('utf-8', 'replace') if isinstance(x.stdout, bytes) else (x.stdout or '')
            code, timed_out = -1, True
        why = ''
        if prefix:
            why = winehost.host_failure(out, timed_out)
            # A launch Wine ended before the game's first milestone (its output cut off).
            if not why and code != 0 and 'run-to: reached main' not in out:
                why = 'Wine ended the launch (exit %d) before the program started' % code
        if not why or attempt == winehost.HOST_RETRIES:
            break
        print('exit-route: %s; launching again' % why)
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
        print('exit-route: skipped (set RA2_NATIVE_CENSUS_DATA_DIR and RA2_NATIVE_CENSUS_GAME_EXE)')
        return SKIP
    prefix = None
    if a.wine_prefix:
        wine = winehost.find_wine(a.wine)
        if not wine:
            print('exit-route: skipped (Windows build: no Wine; set RA2_NATIVE_WINE)')
            return SKIP
        prefix = winehost.Prefix(wine, a.wine_prefix)
    work = os.path.abspath(a.work)
    shutil.rmtree(work, ignore_errors=True)
    write = os.path.join(work, 'write')
    os.makedirs(write)
    env = dict(os.environ, RA2_GAME_EXE=a.game_exe, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_CRASH_TRACE='1',
               RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_ENHANCEMENTS='uisize=1.5')
    for k in ('RA2_NATIVE_TRAP_CONTINUE', 'RA2_NATIVE_SAVE_AT', 'RA2_NATIVE_SAVE_LEAVE'):
        env.pop(k, None)
    common = [a.binary, '--data-dir', a.data_dir, '--write-dir=' + write, '--game-args=-noaudio -CD.']
    bad = []
    try:
        out, code = run(prefix, dict(env, RA2_NATIVE_SAVE_AT='40,ROUTE.SAV'),
                        common + ['--run-to=frames', '--run-seconds=240', '--skirmish=#5', '--skirmish-frames=60'], 240)
        open(os.path.join(work, 'save.log'), 'w').write(out)
        if code != 0 or not os.path.isfile(os.path.join(write, 'ROUTE.SAV')):
            print('exit-route: FAILED: the harness save was not made (exit %d; see %s)' % (code, os.path.join(work, 'save.log')))
            return 1
        script = os.path.join(work, 'route.script')
        open(script, 'w').write(SCRIPT)
        out, code = run(prefix, env, common + ['--run-to=never', '--run-seconds=240', '--load-save=ROUTE.SAV',
                                               '--input-script=' + script], 240)
        open(os.path.join(work, 'route.log'), 'w').write(out)
        lines = out.splitlines()
        if code != 0:
            bad.append('exit %d (0x%08X)' % (code, code & 0xFFFFFFFF))
        if 'resume: loaded ROUTE.SAV' not in out:
            bad.append('the save did not load')
        if not any('reached menu-return' in l for l in lines):
            bad.append('the main menu was not shown again after the game (no menu-return)')
        if not re.search(r'stopped: WinMain returned 0', out):
            bad.append('WinMain did not return 0 (the route stalled or the game faulted; see route.log)')
        if any(l.startswith('input-script:') and 'event 256' in l for l in lines):
            bad.append('the fallback quit fired: the route did not end by Exit Game')
        bad += [l for l in lines if BAD.search(l)][:10]
    finally:
        if prefix:
            prefix.close()
    for b in bad:
        print('exit-route: ' + b)
    print('exit-route: %s (see %s)' % ('ok' if not bad else 'FAILED', work))
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main())
