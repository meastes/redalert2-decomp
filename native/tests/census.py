#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""Native census (ctest native-census, Scout1209): runs the built game to a real
game frame and fails on any trap, fault or unrecovered call. --smoke-test only
opens the window; this reaches Main_Loop frames of a skirmish, so a landing that
breaks the first frame (a dropped draft, a cut pointer) fails here.

  census.py --binary <ra2-native> --data-dir <installed RA2> --game-exe <game.exe>
            [--map '#5'] [--frames 600] [--seconds 600] [--log <file>]
            [--wine-prefix <dir> [--wine <wine>]]   the Windows build's .exe under Wine (tests/wine.py)

Without a data folder or game.exe (the RA2_NATIVE_CENSUS_DATA_DIR /
RA2_NATIVE_CENSUS_GAME_EXE cache variables, or the same environment variables)
it exits 77, which ctest reports as skipped; so does a Windows build's run without Wine. The game data are read only; the
game's writes go to a fresh temporary folder."""
import argparse
import os
import re
import subprocess
import sys
import tempfile

import wine as winehost

SKIP = 77
# Lines the port prints for a run that must not pass: a trap stub for still
# unrecovered code, a fault (signal), an abort, a swizzle failure on load.
BAD = re.compile(r'unrecovered|trap|signal \d+|SIG(SEGV|BUS|ILL|ABRT|FPE)|crash:|swizzle: unmatched|abort', re.I)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default=os.environ.get('RA2_NATIVE_CENSUS_DATA_DIR', ''))
    ap.add_argument('--game-exe', default=os.environ.get('RA2_NATIVE_CENSUS_GAME_EXE', ''))
    ap.add_argument('--map', default='#5')
    ap.add_argument('--frames', type=int, default=600)
    ap.add_argument('--seconds', type=int, default=600)
    ap.add_argument('--log', default='')
    ap.add_argument('--wine-prefix', default='', help='run the Windows .exe under Wine in this fresh prefix')
    ap.add_argument('--wine', default='')
    a = ap.parse_args()
    if not a.data_dir or not os.path.isdir(a.data_dir) or not a.game_exe or not os.path.isfile(a.game_exe):
        print('native-census: skipped (set RA2_NATIVE_CENSUS_DATA_DIR and RA2_NATIVE_CENSUS_GAME_EXE)')
        return SKIP
    prefix = None
    if a.wine_prefix:
        wine = winehost.find_wine(a.wine)
        if not wine:
            print('native-census: skipped (Windows build: no Wine; set RA2_NATIVE_WINE)')
            return SKIP
        prefix = winehost.Prefix(wine, a.wine_prefix)
    # The system's focus reports are ignored (C5): an inactive game stops (MSEngine::Check_Focus
    # 0x005B3670), and on a busy machine another game window taking the focus held this run until
    # its time limit (the 900 s timeout under load).
    env = dict(os.environ, RA2_GAME_EXE=a.game_exe, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_CRASH_TRACE='1',
               RA2_NATIVE_IGNORE_FOCUS='1')
    env.pop('RA2_NATIVE_TRAP_CONTINUE', None)   # a trap must end the run
    with tempfile.TemporaryDirectory(prefix='ra2-census-') as write_dir:
        cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=frames', '--run-seconds=%d' % a.seconds,
               '--write-dir=' + write_dir, '--game-args=-noaudio -CD.', '--skirmish=' + a.map,
               '--skirmish-frames=%d' % a.frames, '--skirmish-order=all']
        if prefix:
            cmd, env = prefix.command(cmd), prefix.env(env)
        try:
            p = subprocess.run(cmd, env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, errors='replace')
        finally:
            if prefix:
                prefix.close()
    out = p.stdout
    if a.log:
        with open(a.log, 'w') as f:
            f.write(out)
    bad = [l for l in out.splitlines() if BAD.search(l)]
    reached = re.search(r'run-to: target frames; stopped: target reached', out) is not None
    last = [l for l in out.splitlines() if l.startswith('run-to:')][-3:]
    print('\n'.join(last))
    if p.returncode != 0 or not reached or bad:
        print('native-census: FAILED (exit %d, target %s, %d bad lines)' % (p.returncode, 'reached' if reached else 'not reached', len(bad)))
        for l in bad[:20]:
            print('  ' + l)
        return 1
    print('native-census: %s, %d frames: passed' % (a.map, a.frames))
    return 0


if __name__ == '__main__':
    sys.exit(main())
