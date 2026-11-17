#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest yr-native-skirmish-smoke (lane Y3; off by default, built with -DRA2_NATIVE_YR=ON): Yuri's
Revenge's first-playable route on the Mac, through the real menus with scripted input:

  main menu -> Single Player -> Skirmish -> Start Game (the setup's defaults) -> about 20 s of the
  skirmish (the game loop, the tactical view, the sidebar) -> H (the player's base), a drag
  selection and D (deploy) -> Escape (the in-game options) -> Abort Mission -> the question's
  Abort -> back at the skirmish setup.

Checks: the run reaches the time limit without a link-closure trap or a fault; the skirmish setup
is shown a second time (after the abort); the window was presented during the game (window dumps
continue after the setup's Start click). YR's abort in a skirmish returns to the setup without a
score screen; the score screen needs a won or lost game (not scripted yet).
Skipped (77) without the installed game (RA2 with Yuri's Revenge: gamemd.exe in --data-dir).

  yr_skirmish_smoke.py --binary <yr-native> --data-dir <game folder> --work <dir>
                       [--wine-prefix <dir> [--wine <wine>]]

The Windows build (lane Y13): --wine-prefix runs yr-native.exe under Wine in that fresh prefix (Wine
is a local test host, native/tests/wine.py); skipped (77) when no Wine is found.
"""
import argparse
import glob
import os
import shutil
import subprocess
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', 'tests'))
import game_process  # noqa: E402  (native/tests: the game's process group, lane C19)
import wine as winehost  # noqa: E402

# The menus at 800x600 (RA2_NATIVE_MSG_LOG): Single Player (the main menu's first button), Skirmish
# (Single Player's third), Start Game (the setup's first); in the game Escape opens Game Options,
# Abort Mission (653,327 125x25), the question's Abort (653,227 125x25). Each menu click waits for
# its dialog to settle ("s<k>", host.cpp); the in-game lines count from the setup's settling.
SCRIPT = """\
s1+1000 click 720 220
s2+1000 click 720 304
s3+2000 click 720 262
s3+3000 move 316 300
s3+20000 key H
s3+21000 drag 15 15 600 540
s3+21600 move 316 300
s3+22000 key D
s3+26000 key Escape
s4+2000 click 715 339
s5+2000 click 715 239
"""


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--work', required=True)
    ap.add_argument('--seconds', type=int, default=75)
    ap.add_argument('--wine-prefix', default='', help='run the Windows .exe under Wine in this fresh prefix')
    ap.add_argument('--wine', default='')
    a = ap.parse_args()
    if not a.data_dir or not os.path.exists(os.path.join(a.data_dir, 'gamemd.exe')):
        print('skipped: no Yuri\'s Revenge install (gamemd.exe) in --data-dir')
        return 77
    shutil.rmtree(a.work, ignore_errors=True)
    os.makedirs(os.path.join(a.work, 'w'))
    os.makedirs(os.path.join(a.work, 'shots'))
    script = os.path.join(a.work, 'smoke.script')
    with open(script, 'w') as f:
        f.write(SCRIPT)
    env = dict(os.environ, RA2_NATIVE_WINDOW_DUMP=os.path.join(a.work, 'shots', 'win'), RA2_NATIVE_WINDOW_DUMP_MS='1000',
               RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_DIALOG_LOG='1')
    cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=never', '--run-seconds=%d' % a.seconds,
           '--write-dir=' + os.path.join(a.work, 'w'), '--input-script=' + script, '--game-args=-noaudio -CD.']
    prefix = None
    if a.wine_prefix:
        wine = winehost.find_wine(a.wine)
        if not wine:
            print('skipped: no Wine for the Windows build')
            return 77
        prefix = winehost.Prefix(wine, a.wine_prefix)
        cmd, env = prefix.command(cmd), prefix.env(env)
    try:
        with open(os.path.join(a.work, 'run.log'), 'w') as log:
            game_process.run(cmd, env=env, stdout=log, stderr=subprocess.STDOUT, timeout=a.seconds + 180)
    finally:
        if prefix:
            prefix.close()
    text = open(os.path.join(a.work, 'run.log'), errors='replace').read()
    failures = []
    stop = [l for l in text.splitlines() if 'stopped:' in l]
    if not stop or 'time limit' not in stop[-1]:
        failures.append('the run stopped early: %s' % (stop[-1] if stop else 'no stop line'))
    shown = [l for l in text.splitlines() if l.startswith('dialog shown #')]
    if len(shown) < 6:
        failures.append('the skirmish setup was not shown again after the abort (%d dialogs shown)' % len(shown))
    dumps = len(glob.glob(os.path.join(a.work, 'shots', 'win.*.bmp')))
    if dumps < 25:
        failures.append('too few window presents during the game (%d dumps)' % dumps)
    for f in failures:
        print('FAIL: ' + f)
    print('%s: %d dialogs shown, %d window dumps (%s)' % ('FAIL' if failures else 'OK', len(shown), dumps, a.work))
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
