#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest yr-native-skirmish-score (lane Y4; off by default, built with -DRA2_NATIVE_YR=ON): a lost
Yuri's Revenge skirmish on the Mac, through the real menus with scripted input:

  main menu -> Single Player -> Skirmish -> Start Game (the setup's defaults: Short Game on) ->
  a band selection over the start view and D (the MCV deploys into a construction yard) ->
  H (the view centres on the base) -> the sidebar's Sell button -> a click on the construction
  yard (sold: no buildings left, a short game is lost) -> the Skirmish Score screen ->
  Continue -> the skirmish setup -> Back -> Single Player -> Back -> the main menu.

The start position is random; the route only uses screen-relative positions (the start view is
centred on the player's units, H centres the construction yard). Checks: the run reaches the time
limit without a link-closure trap or a fault; the score screen ("GUI:MultiplayerScore") is shown;
the skirmish setup, Single Player and the main menu are shown again after it.
Skipped (77) without the installed game (RA2 with Yuri's Revenge: gamemd.exe in --data-dir).

  yr_skirmish_score.py --binary <yr-native> --data-dir <game folder> --work <dir>
"""
import argparse
import os
import re
import shutil
import subprocess
import sys

# 800x600 (RA2_NATIVE_DIALOG_LOG): Single Player (the main menu's first button), Skirmish (Single
# Player's third), Start Game (the setup's first); in the game the band selection is held over a
# few frames (ldown, moves, lup), the Sell button is the sidebar's $ (736,180), Continue and Back
# are the menus' bottom button (720,556). Each menu click waits for its dialog to settle ("s<k>",
# host.cpp); the in-game lines count from the setup's settling.
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
s3+27000 key H
s3+28000 click 736 180
s3+29000 click 316 284
s4+3000 click 720 556
s5+2000 click 720 556
s6+2000 click 720 556
"""


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--work', required=True)
    ap.add_argument('--seconds', type=int, default=110)
    a = ap.parse_args()
    if not a.data_dir or not os.path.exists(os.path.join(a.data_dir, 'gamemd.exe')):
        print('skipped: no Yuri\'s Revenge install (gamemd.exe) in --data-dir')
        return 77
    shutil.rmtree(a.work, ignore_errors=True)
    os.makedirs(os.path.join(a.work, 'w'))
    os.makedirs(os.path.join(a.work, 'shots'))
    script = os.path.join(a.work, 'score.script')
    with open(script, 'w') as f:
        f.write(SCRIPT)
    env = dict(os.environ, RA2_NATIVE_WINDOW_DUMP=os.path.join(a.work, 'shots', 'win'), RA2_NATIVE_WINDOW_DUMP_MS='1000',
               RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_DIALOG_LOG='1',
               RA2_NATIVE_MSG_LOG='0')   # set (0: no message trace): the shown dialogs' controls are listed
    cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=never', '--run-seconds=%d' % a.seconds,
           '--write-dir=' + os.path.join(a.work, 'w'), '--input-script=' + script, '--game-args=-noaudio -CD.']
    with open(os.path.join(a.work, 'run.log'), 'w') as log:
        subprocess.run(cmd, env=env, stdout=log, stderr=subprocess.STDOUT, timeout=a.seconds + 120)
    text = open(os.path.join(a.work, 'run.log'), errors='replace').read()
    lines = text.splitlines()
    failures = []
    stop = [l for l in lines if 'stopped:' in l]
    if not stop or 'time limit' not in stop[-1]:
        failures.append('the run stopped early: %s' % (stop[-1] if stop else 'no stop line'))
    # Each "dialog shown #n" block lists its controls; name each dialog by its title static.
    titles = []
    for l in lines:
        if l.startswith('dialog shown #'):
            titles.append(None)
        elif titles and titles[-1] is None:
            m = re.search(r'"GUI:(MainMenu|SinglePlayerMenu|SkirmishGame|MultiplayerScore)"', l)
            if m and ' 1684 ' in l:
                titles[-1] = m.group(1)
    order = [t for t in titles if t]
    if 'MultiplayerScore' not in order:
        failures.append('the score screen was not shown (dialogs: %s)' % ', '.join(order))
    else:
        after = order[order.index('MultiplayerScore') + 1:]
        if after[:3] != ['SkirmishGame', 'SinglePlayerMenu', 'MainMenu']:
            failures.append('after the score screen: %s (expected the skirmish setup, Single Player, the main menu)' % ', '.join(after))
    for f in failures:
        print('FAIL: ' + f)
    print('%s: dialogs %s (%s)' % ('FAIL' if failures else 'OK', ' -> '.join(order), a.work))
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
