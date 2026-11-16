#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest yr-native-controls (lane Y8; built with -DRA2_NATIVE_YR=ON): the Enhancements' Controls
(Classic / Modern) and Queue five with Shift in Yuri's Revenge (glue/yr_enhancements.cpp), on the
Mac through the real menus with scripted input, once with RA2_NATIVE_ENHANCEMENTS=
"controls=modern,queuefive=on" and once with the defaults (Classic, Queue five off):

  main menu -> Single Player -> Skirmish -> Start Game -> H -> a double click on a tank (Modern:
  the type select) -> a click on the MCV and D (it deploys) -> a band over the tanks -> a right
  click on open ground (Modern: a move order; Classic: the selection cleared) -> Shift held and a click on the
  sidebar's first build icon once the construction yard stands (Queue five: the PRODUCE event
  five times).

A third run opens the shell Options dialog (glue/mainopt_native.inc), its Enhancements button, the
Enhanced preset and Ok, then Options' Main Menu: the page accepts the preset (queue five on) and the
main menu comes back.

Checks: both runs reach the time limit without a trap or a fault; with Modern the right click gave
an order ("modern-controls: right click: action 1", a move), the double click selected the tanks
on the screen and the Shift click queued five
("queue-five: ... added 5 times"); with the defaults neither line appears. Skipped (77) without
the installed game (gamemd.exe in --data-dir).

  yr_controls.py --binary <yr-native> --data-dir <game folder> --work <dir>
"""
import argparse
import os
import re
import shutil
import subprocess
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', 'tests'))
import game_process  # noqa: E402  (native/tests: the game's process group, lane C19)

SCRIPT = """\
s1+1000 click 720 220
s2+1000 click 720 304
s3+2000 click 720 262
s3+3000 move 316 300
s3+20000 key H
s3+21000 click 313 158
s3+21150 click 313 158
s3+23000 click 316 270
s3+23600 key D
s3+25000 drag 15 15 600 225
s3+26000 rclick 470 460
s3+38000 keydown Left_Shift
s3+38300 click 680 260
s3+39000 keyup Left_Shift
"""


# The shell Options dialog (glue/mainopt_native.inc) and its Enhancements page: Options ->
# Enhancements -> Enhanced (the preset) -> Ok -> Options' Main Menu.
PAGE_SCRIPT = """\
s1+1500 click 720 388
s2+2500 click 720 304
s3+2500 click 720 304
s3+3500 click 720 220
s3+5500 click 720 556
"""


def run(a, name, enhancements, script_text=SCRIPT):
    work = os.path.join(a.work, name)
    shutil.rmtree(work, ignore_errors=True)
    os.makedirs(os.path.join(work, 'w'))
    script = os.path.join(work, 'controls.script')
    with open(script, 'w') as f:
        f.write(script_text)
    env = dict(os.environ, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_DIALOG_LOG='1')
    env.pop('RA2_NATIVE_ENHANCEMENTS', None)
    env['RA2_NATIVE_SETTINGS'] = os.path.join(work, 'settings')   # never the player's own settings file
    if enhancements:
        env['RA2_NATIVE_ENHANCEMENTS'] = enhancements
    cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=never', '--run-seconds=%d' % a.seconds,
           '--write-dir=' + os.path.join(work, 'w'), '--input-script=' + script, '--game-args=-noaudio -CD.']
    with open(os.path.join(work, 'run.log'), 'w') as log:
        game_process.run(cmd, env=env, stdout=log, stderr=subprocess.STDOUT, timeout=a.seconds + 120)
    return open(os.path.join(work, 'run.log'), errors='replace').read()


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--work', required=True)
    ap.add_argument('--seconds', type=int, default=80)
    a = ap.parse_args()
    if not a.data_dir or not os.path.exists(os.path.join(a.data_dir, 'gamemd.exe')):
        print('skipped: no Yuri\'s Revenge install (gamemd.exe) in --data-dir')
        return 77
    failures = []
    for name, enh in (('modern', 'controls=modern,queuefive=on'), ('classic', '')):
        text = run(a, name, enh)
        stop = [l for l in text.splitlines() if 'stopped:' in l]
        if not stop or 'time limit' not in stop[-1]:
            failures.append('%s: the run stopped early: %s' % (name, stop[-1] if stop else 'no stop line'))
        orders = re.findall(r'modern-controls: right click: action (\d+)', text)
        five = re.findall(r'queue-five: event \d+ added (\d+) times', text)
        double = re.findall(r'modern-controls: double click: (\d+) selected', text)
        print('%s: right-click orders %s, queue five %s, double click %s, %s' % (
            name, orders or 'none', five or 'none', double or 'none', stop[-1] if stop else 'no stop line'))
        if name == 'modern':
            if '1' not in orders:
                failures.append('modern: the right click on open ground gave no move order')
            if '5' not in five:
                failures.append('modern: the Shift click on the build icon did not queue five')
            if not double or int(double[0]) < 2:
                failures.append('modern: the double click on a tank did not select the tanks on the screen')
        else:
            if orders or five or double:
                failures.append('classic: Modern or Queue five lines with the defaults')
    # The page's run: RA2_NATIVE_ENHANCEMENTS keeps the settings unsaved (one run).
    text = run(a, 'page', 'controls=classic', PAGE_SCRIPT)
    stop = [l for l in text.splitlines() if 'stopped:' in l]
    accepted = [l for l in text.splitlines() if l.startswith('enhancements: accepted')]
    print('page: %s; %s' % (accepted[-1] if accepted else 'not accepted', stop[-1] if stop else 'no stop line'))
    if not stop or 'time limit' not in stop[-1]:
        failures.append('page: the run stopped early: %s' % (stop[-1] if stop else 'no stop line'))
    if not accepted or 'queue five on' not in accepted[-1]:
        failures.append('page: the Enhanced preset was not accepted through the page')
    if 'enhancements: page closed (accept)' not in text:
        failures.append('page: the page did not close with Ok')
    if len([l for l in text.splitlines() if l.startswith('dialog shown #')]) < 4:
        failures.append('page: the main menu did not come back after Options')
    for f in failures:
        print('FAIL: ' + f)
    print('%s (%s)' % ('FAIL' if failures else 'OK', a.work))
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
