#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest yr-native-command-bar-thumb (lane N27; built with -DRA2_NATIVE_YR=ON): the command bar's
thumbs, closing the bar and opening it again.

The user's Duo report (2026-11-21): "the bottom bar blanked out. I think I tapped somewhere on the very
left side." The original's bar has a close thumb at the left end of its buttons (ButtonA, 0xF0) that
hides them, and an open thumb at its right end beside the sidebar (ButtonB, 0xF1) that brings them back
(TabClass::AI, gamemd.exe 0x006D0680). With Zoom gestures on the game's screen is made twice as large in
its map column (the render scale) and the host shows only the bar's left part, the window's map column;
the interface art layout (FUN_0072FC60) laid the bar out from the larger screen's sidebar x, so the open
thumb lay off the picture and the closed bar could not be opened. glue/yr_map_zoom.cpp
Yr_Command_Bar_Right lays it out from the shown part's right end.

Through the menus into a skirmish (yr_combat.py's route), at 800x600 with scripted clicks on the thumb
shown now (input script `click @thumb`) and harness `thumb` orders (glue/yr_harness.cpp):
  frame 100 the bar open; 150 the close thumb clicked; 200 the bar closed, the open thumb a button
  inside the window's map column (x + width <= 632); 250 the open thumb clicked; 300 the bar open.
Two runs: Zoom gestures on with Game interface size 1.25x (the Duo's settings), and the defaults (the
original's layout: the open thumb at 576).

  yr_command_bar_thumb.py --binary <yr-native> --data-dir <game folder> --work <dir>
Skipped (77) without the installed game (gamemd.exe in --data-dir) and for the Windows build.
"""
import argparse
import importlib
import os
import re
import shutil
import subprocess
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', 'tests'))
import game_process  # noqa: E402

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
combat = importlib.import_module('yr_combat')
SCRIPT = combat.SCRIPT + '150 click @thumb\n250 click @thumb\n'
ORDERS = '100 thumb; 200 thumb; 300 thumb'
THUMB = re.compile(r'^harness-thumb (open|closed) close (-?\d+),(-?\d+) (\d+)x(\d+)( listed)? '
                   r'open (-?\d+),(-?\d+) (\d+)x(\d+)( listed)?', re.M)
SHOWN_BAR = 800 - 168   # the window's map column at 800x600: the part of the bar the host shows


def run(a, name, enhancements):
    work = os.path.join(a.work, name)
    shutil.rmtree(work, ignore_errors=True)
    os.makedirs(os.path.join(work, 'w'))
    with open(os.path.join(work, 'w', 'RA2MD.INI'), 'w') as f:
        f.write(combat.SETTINGS)
    script = os.path.join(work, 'thumb.script')
    with open(script, 'w') as f:
        f.write(SCRIPT)
    env = dict(os.environ, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_MSG_LOG='0',
               RA2_NATIVE_ORDERS=ORDERS, RA2_NATIVE_SETTINGS=os.path.join(work, 'settings'))
    env.pop('RA2_NATIVE_FORCE_END', None)
    env.pop('RA2_NATIVE_ENHANCEMENTS', None)
    if enhancements:
        env['RA2_NATIVE_ENHANCEMENTS'] = enhancements
    cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=never', '--run-seconds=%d' % a.seconds,
           '--write-dir=' + os.path.join(work, 'w'), '--input-script=' + script, '--game-args=-noaudio -CD.']
    with open(os.path.join(work, 'run.log'), 'w') as log:
        game_process.run(cmd, env=env, stdout=log, stderr=subprocess.STDOUT, timeout=a.seconds + 120)
    text = open(os.path.join(work, 'run.log'), errors='replace').read()
    states = THUMB.findall(text)
    failures = []
    if len(states) < 3:
        failures.append('only %d of 3 "harness-thumb" lines (the game did not reach frame 300)' % len(states))
    else:
        before, closed, after = states[:3]
        for label, s in (('before', before), ('closed', closed), ('after', after)):
            print('%s %s: %s; close thumb %s,%s%s; open thumb %s,%s %sx%s%s' % (
                name, label, s[0], s[1], s[2], s[5], s[6], s[7], s[8], s[9], s[10]))
        if before[0] != 'open' or not before[5]:
            failures.append('the bar was not open with its close thumb a button at first')
        if closed[0] != 'closed':
            failures.append('the close thumb did not close the bar')
        elif not closed[10]:
            failures.append('the closed bar has no open thumb button')
        open_x, open_w = int(closed[6]), int(closed[8])
        if open_x < 0 or open_x + open_w > SHOWN_BAR:
            failures.append('the open thumb at x %d (%d wide) lies outside the shown bar (0-%d): the closed bar '
                            'cannot be opened' % (open_x, open_w, SHOWN_BAR))
        if after[0] != 'open':
            failures.append('the open thumb did not open the bar again')
        if not enhancements and open_x != 576:
            failures.append('the defaults moved the open thumb from the original\'s 576 to %d' % open_x)
    if 'input-script: cannot' in text or 'is not a button now' in text:
        failures.append('a scripted click found no thumb')
    bad = sorted(set(l for l in text.splitlines() if combat.BAD.search(l)))
    if bad:
        failures.append('traps or faults: %s' % '; '.join(bad[:5]))
    return ['%s: %s' % (name, f) for f in failures]


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--work', required=True)
    ap.add_argument('--seconds', type=int, default=40)
    a = ap.parse_args()
    if not a.data_dir or not os.path.exists(os.path.join(a.data_dir, 'gamemd.exe')):
        print('skipped: no Yuri\'s Revenge install (gamemd.exe) in --data-dir')
        return 77
    if a.binary.lower().endswith('.exe'):
        print('skipped: run on the Mac build (the Windows build has the same game code)')
        return 77
    failures = run(a, 'zoom', 'uisize=1.25,zoomgestures=on') + run(a, 'defaults', '')
    for f in failures:
        print('FAIL: ' + f)
    print('%s: the command bar\'s thumbs close and open it (%s)' % ('FAIL' if failures else 'OK', a.work))
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
