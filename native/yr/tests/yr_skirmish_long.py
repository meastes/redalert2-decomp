#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest yr-native-skirmish-long (lane Y6; off by default, built with -DRA2_NATIVE_YR=ON): a long Yuri's
Revenge skirmish on the Mac against computer opponents, through the real menus with scripted input:

  the write folder's RA2MD.INI sets the skirmish setup (the opponents in slots 1..N at the chosen
  difficulty, the rest closed; the setup's other defaults) -> main menu -> Single Player ->
  Skirmish -> Start Game -> a band selection and D (the MCV deploys) -> the opponents play.

Checks: the run reaches its time limit without a link-closure trap or a fault (about --seconds of
play after the load); the window is presented throughout (window dumps up to the end). A score
screen (the player lost) is reported, not a failure. Skipped (77) without the installed game
(RA2 with Yuri's Revenge: gamemd.exe in --data-dir).

  yr_skirmish_long.py --binary <yr-native> --data-dir <game folder> --work <dir>
                      [--seconds 600] [--opponents 3] [--difficulty hard|normal|easy]
"""
import argparse
import glob
import os
import re
import shutil
import subprocess
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', 'tests'))
import game_process  # noqa: E402  (native/tests: the game's process group, lane C19)

# The skirmish setup's slot types (skirmish.cpp Skirmish_Slot_Type): 4 hard, 5 normal, 6 easy, 1 closed.
SLOT_TYPE = {'hard': 4, 'normal': 5, 'easy': 6}

# 800x600: Single Player, Skirmish, Start Game (as yr_skirmish_score.py); then a band selection over
# the start view and D (the MCV deploys into a construction yard).
SCRIPT = """\
s1+1000 click 720 220
s2+1000 click 720 304
s3+2000 click 720 262
s3+3000 move 316 300
s3+18000 move 15 15
s3+18100 ldown 15 15
s3+18300 move 300 280
s3+18500 move 600 540
s3+18900 lup 600 540
s3+19400 move 250 340
s3+19600 key D
"""

LOAD_SECONDS = 40      # menus and the skirmish load before play starts


# The skirmish map list's index (sorted by player count): 0 is The Alamo, a two-player map, on which
# the setup closes every slot past the first opponent (lane Y8: the Y6 runs had one opponent); 30 is
# a four-player map (the setup shows three opponents).
FOUR_PLAYER_SCEN = 30


def settings(opponents, difficulty, countries=()):
    def country(i):
        return countries[i - 1] if i <= len(countries) else -2
    slots = ''.join('Slot%02d=%d,%d,-2\n' % (i, SLOT_TYPE[difficulty] if i <= opponents else 1, country(i)) for i in range(1, 8))
    scen = FOUR_PLAYER_SCEN if opponents > 1 else 0
    return ('[Intro]\nPlay=no\n\n[Skirmish]\nGameMode=1\nScenIndex=%d\nGameSpeed=1\nCredits=10000\nUnitCount=10\n' % scen +
            'ShortGame=yes\nSuperWeaponsAllowed=yes\nBuildOffAlly=yes\nMCVRepacks=yes\nCratesAppear=yes\n' + slots)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--work', required=True)
    ap.add_argument('--seconds', type=int, default=600, help='seconds of play after the load')
    ap.add_argument('--opponents', type=int, default=3, choices=range(1, 8))
    ap.add_argument('--difficulty', default='hard', choices=sorted(SLOT_TYPE))
    ap.add_argument('--dump-ms', type=int, default=10000)
    ap.add_argument('--countries', default='', help="the opponents' countries in slot order, comma-separated "
                    "(the setup's indexes: 0 America .. 8 Russia, 9 Yuri; default random), e.g. 8,8,9 for rockets and Yuri's weapons")
    a = ap.parse_args()
    if not a.data_dir or not os.path.exists(os.path.join(a.data_dir, 'gamemd.exe')):
        print('skipped: no Yuri\'s Revenge install (gamemd.exe) in --data-dir')
        return 77
    shutil.rmtree(a.work, ignore_errors=True)
    os.makedirs(os.path.join(a.work, 'w'))
    os.makedirs(os.path.join(a.work, 'shots'))
    with open(os.path.join(a.work, 'w', 'RA2MD.INI'), 'w') as f:
        f.write(settings(a.opponents, a.difficulty, [int(c) for c in a.countries.split(",") if c.strip()]))
    script = os.path.join(a.work, 'long.script')
    with open(script, 'w') as f:
        f.write(SCRIPT)
    total = a.seconds + LOAD_SECONDS
    env = dict(os.environ, RA2_NATIVE_WINDOW_DUMP=os.path.join(a.work, 'shots', 'win'), RA2_NATIVE_WINDOW_DUMP_MS=str(a.dump_ms),
               RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_DIALOG_LOG='1', RA2_NATIVE_MSG_LOG='0')
    cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=never', '--run-seconds=%d' % total,
           '--write-dir=' + os.path.join(a.work, 'w'), '--input-script=' + script, '--game-args=-noaudio -CD.']
    with open(os.path.join(a.work, 'run.log'), 'w') as log:
        game_process.run(cmd, env=env, stdout=log, stderr=subprocess.STDOUT, timeout=total + 120)
    text = open(os.path.join(a.work, 'run.log'), errors='replace').read()
    lines = text.splitlines()
    failures = []
    stop = [l for l in lines if 'stopped:' in l]
    if not stop or 'time limit' not in stop[-1]:
        failures.append('the run stopped early: %s' % (stop[-1] if stop else 'no stop line'))
    traps = sorted(set(l.split('unrecovered ', 1)[1] for l in lines if 'unrecovered ' in l and ' reached' in l))
    if traps:
        failures.append('traps: %s' % '; '.join(traps))
    dumps = len(glob.glob(os.path.join(a.work, 'shots', 'win.*.bmp')))
    want = total * 1000 // a.dump_ms - 3
    if dumps < want:
        failures.append('too few window presents (%d dumps, %d expected)' % (dumps, want))
    score = any(re.search(r'"GUI:MultiplayerScore"', l) for l in lines)
    for f in failures:
        print('FAIL: ' + f)
    print('%s: %d s of play against %d %s opponent(s), %d window dumps%s (%s)' % (
        'FAIL' if failures else 'OK', a.seconds, a.opponents, a.difficulty, dumps,
        ', the player lost (score screen)' if score else '', a.work))
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
