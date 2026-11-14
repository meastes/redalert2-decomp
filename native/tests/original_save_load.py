#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""Original save load (ctest native-original-save, lane C15; README "Original saves"):
the built game loads one of the user's saves from the original game (a compound file
in the installation, read in place and never written) through the menus, as Resume at
start does (--load-save), and plays N Main_Loop frames of it. The run fails on a trap,
fault or swizzle failure, when the save is not opened as an original, when no object is
converted by the field maps, or when a type is read without one ("no layout").

  original_save_load.py --binary <ra2-native> --data-dir <installed RA2> --game-exe <game.exe>
                        [--save ALL01T.sav] [--frames 300] [--seconds 300] [--log <file>]
                        [--wine-prefix <dir> [--wine <wine>]]   the Windows build's .exe under Wine

Without a data folder or game.exe (RA2_NATIVE_CENSUS_DATA_DIR / RA2_NATIVE_CENSUS_GAME_EXE,
as native-census), without the save in the data folder, or a Windows build's run without
Wine, it exits 77 (ctest: skipped). The game's writes go to a fresh temporary folder."""
import argparse
import os
import re
import subprocess
import sys
import tempfile

import wine as winehost

SKIP = 77
BAD = re.compile(r'unrecovered|trap|signal \d+|SIG(SEGV|BUS|ILL|ABRT|FPE)|crash:|swizzle: unmatched|abort'
                 r'|original-save: no layout|resume: could not load', re.I)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default=os.environ.get('RA2_NATIVE_CENSUS_DATA_DIR', ''))
    ap.add_argument('--game-exe', default=os.environ.get('RA2_NATIVE_CENSUS_GAME_EXE', ''))
    ap.add_argument('--save', default='ALL01T.sav', help='an original save in the data folder (Allied 1)')
    ap.add_argument('--frames', type=int, default=300)
    ap.add_argument('--seconds', type=int, default=300)
    ap.add_argument('--log', default='')
    ap.add_argument('--wine-prefix', default='', help='run the Windows .exe under Wine in this fresh prefix')
    ap.add_argument('--wine', default='')
    a = ap.parse_args()
    if not a.data_dir or not os.path.isdir(a.data_dir) or not a.game_exe or not os.path.isfile(a.game_exe):
        print('native-original-save: skipped (set RA2_NATIVE_CENSUS_DATA_DIR and RA2_NATIVE_CENSUS_GAME_EXE)')
        return SKIP
    if not os.path.isfile(os.path.join(a.data_dir, a.save)):
        print('native-original-save: skipped (no %s in the data folder)' % a.save)
        return SKIP
    prefix = None
    if a.wine_prefix:
        wine = winehost.find_wine(a.wine)
        if not wine:
            print('native-original-save: skipped (Windows build: no Wine; set RA2_NATIVE_WINE)')
            return SKIP
        prefix = winehost.Prefix(wine, a.wine_prefix)
    env = dict(os.environ, RA2_GAME_EXE=a.game_exe, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_CRASH_TRACE='1',
               RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_STG_LOG='1', RA2_NATIVE_ORIGINAL_LOG='1')
    env.pop('RA2_NATIVE_TRAP_CONTINUE', None)   # a trap must end the run
    with tempfile.TemporaryDirectory(prefix='ra2-original-save-') as write_dir:
        cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=frames', '--run-seconds=%d' % a.seconds,
               '--write-dir=' + write_dir, '--game-args=-noaudio -CD.', '--load-save=' + a.save,
               '--frames=%d' % a.frames]
        if prefix:
            env = prefix.env(env)
        try:
            # Under Wine a launch the Mac host killed (wine.host_failure) runs again, at most
            # HOST_RETRIES times (in a prefix made again if it was left unusable); a failure of
            # the game itself is never retried.
            for attempt in range(winehost.HOST_RETRIES + 1):
                argv = prefix.command(cmd) if prefix else cmd
                try:
                    p = subprocess.run(argv, env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True,
                                       errors='replace', timeout=a.seconds + 60)
                    out, code, timed_out = p.stdout, p.returncode, False
                except subprocess.TimeoutExpired as e:
                    out = e.stdout.decode('utf-8', 'replace') if isinstance(e.stdout, bytes) else (e.stdout or '')
                    code, timed_out = -1, True
                why = winehost.host_failure(out, timed_out, code) if prefix else ''
                if not why or attempt == winehost.HOST_RETRIES:
                    break
                print('native-original-save: %s; launching again' % why)
        finally:
            if prefix:
                prefix.close()
    if a.log:
        with open(a.log, 'w') as f:
            f.write(out)
    lines = out.splitlines()
    bad = [l for l in lines if BAD.search(l)]
    opened = any('original save (compound file)' in l for l in lines)
    loaded = any(l.startswith('resume: loaded ') for l in lines)
    converted = sum(1 for l in lines if l.startswith('original-save: ') and ' i686 bytes to ' in l)
    reached = re.search(r'run-to: target frames; stopped: target reached', out) is not None
    print('\n'.join([l for l in lines if l.startswith('run-to:')][-3:]))
    print('native-original-save: %s opened as an original: %s, loaded: %s, %d objects converted'
          % (a.save, 'yes' if opened else 'no', 'yes' if loaded else 'no', converted))
    if code != 0 or not (reached and opened and loaded and converted) or bad:
        print('native-original-save: FAILED (exit %d, target %s, %d bad lines)'
              % (code, 'reached' if reached else 'not reached', len(bad)))
        for l in bad[:20]:
            print('  ' + l)
        return 1
    print('native-original-save: %s, %d frames: passed' % (a.save, a.frames))
    return 0


if __name__ == '__main__':
    sys.exit(main())
