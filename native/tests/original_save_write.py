#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest native-original-save-write (lane C17; README "Original saves"): saves the port writes
(always in the original game's format since lane C20: a compound file with the i686 object
images) load back in the port and play.

In a fresh write folder under --work:
1. a skirmish saved at frame 40 (FRESH.SAV), then loaded in a new process (--load-save) and
   played 300 frames;
2. for each of the user's saves named with --save (in the installed game, read in place,
   never copied): loaded, saved at frame 50 (RT_<name>.SAV), then that file loaded in a new
   process and played 300 frames.
Each written file must be a compound file the port marks as its own, and no run may report
a type written or read without a map, a vtable pointer without the original's vtable, an
unmatched swizzle id, a fault or a trap. Skipped (77) without the installed game or, for the
Windows build, without Wine.

--port-transition (ctest native-port-save-transition, lane C20; TRANSITION: remove in
release N+1 with the port container reader) runs one case instead: a skirmish saved at
frame 40 in the port's old container (PORT.SAV, under the test hook
RA2_NATIVE_TEST_PORT_SAVE=1, made at test time), checked to be one ("RA2NSTG2"); loaded in
a new process and saved at frame 50 (RT_PORT.SAV), which must be in the original's format;
that file loaded in a new process and played 300 frames.

  original_save_write.py --binary <ra2-native> --data-dir <game folder> --game-exe <game.exe> --work <dir>
                         [--save ALL01T.sav ... | --port-transition] [--wine-prefix <dir> [--wine <wine>]]
"""
import argparse
import os
import re
import shutil
import struct
import subprocess
import sys

import game_process
import wine as winehost

SKIP = 77
BAD = re.compile(r'unrecovered|trap|signal \d+|SIG(SEGV|BUS|ILL|ABRT|FPE)|crash:|swizzle: unmatched|abort'
                 r'|no layout|no original vtable|resume: could not load', re.I)
COMPOUND = bytes([0xD0, 0xCF, 0x11, 0xE0, 0xA1, 0xB1, 0x1A, 0xE1])
PORT_MARK = 0x504E3252   # the root entry's state bits in a compound file the port wrote


def run(prefix, env, argv, seconds):
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
        why = ''
        if prefix:
            why = winehost.host_failure(out, timed_out)
            if not why and code != 0 and 'run-to: reached main' not in out:
                why = 'Wine ended the launch (exit %d) before the program started' % code
        if not why or attempt == winehost.HOST_RETRIES:
            break
        print('original-save-write: %s; launching again' % why)
    return out, code


def port_compound(path):
    """Whether path is a compound file whose root entry carries the port's mark."""
    try:
        b = open(path, 'rb').read()
    except OSError:
        return False
    if len(b) < 1024 or b[:8] != COMPOUND:
        return False
    sector = 1 << struct.unpack_from('<H', b, 30)[0]
    root = 512 + struct.unpack_from('<I', b, 48)[0] * sector
    return root + 128 <= len(b) and b[root + 66] == 5 and struct.unpack_from('<I', b, root + 96)[0] == PORT_MARK


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--game-exe', default='')
    ap.add_argument('--work', required=True)
    ap.add_argument('--save', action='append', default=[])
    ap.add_argument('--port-transition', action='store_true')
    ap.add_argument('--wine-prefix', default='')
    ap.add_argument('--wine', default='')
    a = ap.parse_args()
    if not a.data_dir or not os.path.isdir(a.data_dir) or not a.game_exe or not os.path.isfile(a.game_exe):
        print('original-save-write: skipped (set RA2_NATIVE_CENSUS_DATA_DIR and RA2_NATIVE_CENSUS_GAME_EXE)')
        return SKIP
    prefix = None
    if a.wine_prefix:
        wine = winehost.find_wine(a.wine)
        if not wine:
            print('original-save-write: skipped (Windows build: no Wine; set RA2_NATIVE_WINE)')
            return SKIP
        prefix = winehost.Prefix(wine, a.wine_prefix)
    work = os.path.abspath(a.work)
    shutil.rmtree(work, ignore_errors=True)
    write = os.path.join(work, 'write')
    os.makedirs(write)
    env = dict(os.environ, RA2_GAME_EXE=a.game_exe, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_CRASH_TRACE='1',
               RA2_NATIVE_IGNORE_FOCUS='1')
    for k in ('RA2_NATIVE_TRAP_CONTINUE', 'RA2_NATIVE_SAVE_AT', 'RA2_NATIVE_SAVE_LEAVE', 'RA2_NATIVE_TEST_PORT_SAVE'):
        env.pop(k, None)
    common = [a.binary, '--data-dir', a.data_dir, '--write-dir=' + write, '--game-args=-noaudio -CD.', '--run-seconds=240']
    bad = []

    def check(tag, out, code, reached=True):
        open(os.path.join(work, tag + '.log'), 'w').write(out)
        if code != 0:
            bad.append('%s: exit %d' % (tag, code))
        if reached and not re.search(r'run-to: target frames; stopped: target reached', out):
            bad.append('%s: the frames target was not reached' % tag)
        bad.extend('%s: %s' % (tag, l) for l in out.splitlines() if BAD.search(l))

    cases = [('FRESH.SAV', None)] + [('RT_%s.SAV' % os.path.splitext(s)[0].upper(), s) for s in a.save
                                     if os.path.isfile(os.path.join(a.data_dir, s))]
    skipped = [s for s in a.save if not os.path.isfile(os.path.join(a.data_dir, s))]
    try:
        if a.port_transition:
            # TRANSITION (remove in release N+1): a port save made now with the old writer.
            out, code = run(prefix, dict(env, RA2_NATIVE_TEST_PORT_SAVE='1', RA2_NATIVE_SAVE_AT='40,PORT.SAV'),
                            common + ['--run-to=frames', '--skirmish=#5', '--skirmish-frames=60'], 240)
            check('write-PORT.SAV', out, code)
            try:
                head = open(os.path.join(write, 'PORT.SAV'), 'rb').read(8)
            except OSError:
                head = b''
            if head != b'RA2NSTG2':
                bad.append('PORT.SAV: not written in the port container (test hook RA2_NATIVE_TEST_PORT_SAVE)')
                cases = []
            else:
                cases = [('RT_PORT.SAV', 'PORT.SAV')]
        for name, source in cases:
            if source is None:
                out, code = run(prefix, dict(env, RA2_NATIVE_SAVE_AT='40,' + name),
                                common + ['--run-to=frames', '--skirmish=#5', '--skirmish-frames=60'], 240)
            else:
                # --load-save reads the data folder or the write folder (PORT.SAV is in the latter).
                out, code = run(prefix, dict(env, RA2_NATIVE_SAVE_AT='50,' + name),
                                common + ['--run-to=frames', '--frames=100', '--load-save=' + source], 240)
            check('write-' + name, out, code)
            if source and 'resume: loaded ' + source not in out:
                bad.append('%s: did not load' % source)
            path = os.path.join(write, name)
            if not port_compound(path):
                bad.append('%s: not written as a compound file marked as the port\'s' % name)
                continue
            out, code = run(prefix, env, common + ['--run-to=frames', '--frames=300', '--load-save=' + name], 240)
            check('load-' + name, out, code)
            if 'resume: loaded ' + name not in out:
                bad.append('%s: did not load' % name)
    finally:
        if prefix:
            prefix.close()
    for b in bad[:40]:
        print('original-save-write: ' + b)
    print('original-save-write: %s: %d saves written and loaded%s (see %s)'
          % ('ok' if not bad else 'FAILED', len(cases), ('; not in the data folder: ' + ', '.join(skipped)) if skipped else '',
             work))
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main())
