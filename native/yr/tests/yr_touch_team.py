#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest yr-native-touch-team (lane Y15; built with -DRA2_NATIVE_YR=ON): a hold on a command-bar team
button sets the team (platform/sdl/touch.cpp; the RA2 game's check is native-touch-team-hold).

The bar's team buttons (TabClass::AI, gamemd.exe 0x006D0680): a click on an empty team gives it the
selection, a click on a team with members selects it, a right click disbands it; a held Ctrl is not
read. Touch has no right click on a button, so a team that has members could not be set again; a
hold there now sends the right click and then the click (the team becomes the selection).

Through the menus into a skirmish (yr_combat.py's route), with scripted fingers (input script
tdown/tup at `@team=1`, the host's point of the bar's Team 1) and harness orders
(glue/yr_harness.cpp select/groups):
  frame 100  the first tank selected; 150 Team 1 tapped: the empty team takes it (the original);
  frame 220  the second tank selected; 250 groups: the first tank in Team 1 (0), the second in none (-1);
  frame 300  Team 1 held 50 frames; 400 groups: the first tank in none, the second in Team 1;
  frame 420  the first tank selected; 480 Team 1 tapped; 560 groups: the second tank alone selected.

  yr_touch_team.py --binary <yr-native> --data-dir <game folder> --work <dir>
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
SCRIPT = combat.SCRIPT + ''.join(l + '\n' for l in (
    '150 tdown @team=1', '150 tup @team=1',
    '300 tdown @team=1', '350 tup @team=1',
    '480 tdown @team=1', '480 tup @team=1'))
ORDERS = '100 select MTNK 0; 220 select MTNK 1; 250 groups MTNK; 400 groups MTNK; 420 select MTNK 0; 560 groups MTNK'
GROUPS = re.compile(r'^harness-groups MTNK((?: -?\d+\*?)+)$', re.M)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--work', required=True)
    ap.add_argument('--seconds', type=int, default=60)
    a = ap.parse_args()
    if not a.data_dir or not os.path.exists(os.path.join(a.data_dir, 'gamemd.exe')):
        print('skipped: no Yuri\'s Revenge install (gamemd.exe) in --data-dir')
        return 77
    if a.binary.lower().endswith('.exe'):
        print('skipped: the Windows build has no touch host')
        return 77
    shutil.rmtree(a.work, ignore_errors=True)
    os.makedirs(os.path.join(a.work, 'w'))
    with open(os.path.join(a.work, 'w', 'RA2MD.INI'), 'w') as f:
        f.write(combat.SETTINGS)
    script = os.path.join(a.work, 'team.script')
    with open(script, 'w') as f:
        f.write(SCRIPT)
    env = dict(os.environ, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_MSG_LOG='0',
               RA2_NATIVE_TOUCH_UI='1', RA2_NATIVE_ORDERS=ORDERS)
    env.pop('RA2_NATIVE_FORCE_END', None)
    cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=never', '--run-seconds=%d' % a.seconds,
           '--write-dir=' + os.path.join(a.work, 'w'), '--input-script=' + script, '--game-args=-noaudio -CD.']
    with open(os.path.join(a.work, 'run.log'), 'w') as log:
        game_process.run(cmd, env=env, stdout=log, stderr=subprocess.STDOUT, timeout=a.seconds + 120)
    text = open(os.path.join(a.work, 'run.log'), errors='replace').read()
    groups = [m.group(1).split() for m in GROUPS.finditer(text)]
    failures = []
    want = [(['0', '-1*'], 'Team 1 tapped while empty: the first tank in it'),
            (['-1', '0*'], 'Team 1 held: the second tank in it instead'),
            (['-1', '0*'], 'Team 1 tapped: the second tank alone selected')]
    for i, (prefix, what) in enumerate(want):
        got = groups[i][:2] if i < len(groups) else None
        print('%s: %s' % (what, got))
        if got != prefix:
            failures.append('%s: got %s' % (what, got))
    if 'touch: hold on team button 1' not in text:
        failures.append('no "touch: hold on team button 1" (the hold was not over Team 1)')
    bad = sorted(set(l for l in text.splitlines() if combat.BAD.search(l)))
    if bad:
        failures.append('traps or faults: %s' % '; '.join(bad[:5]))
    for f in failures:
        print('FAIL: ' + f)
    print('%s: team set by a hold, selected by a tap (%s)' % ('FAIL' if failures else 'OK', a.work))
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
