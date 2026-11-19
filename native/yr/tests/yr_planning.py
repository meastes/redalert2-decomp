#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest yr-native-planning (lane N3; built with -DRA2_NATIVE_YR=ON): the command bar's PlanningMode button.

Planning mode's functions are unmatched in the YR tree and were link-closure traps: with units selected the
button stopped the game at its first draw (FUN_0063B550). glue/planmgr_planning_native.inc has NON-EXACT
stand-ins (the planned order, the commit, the branch filing, the outline and filled draws); a fixup empties
the drawn-nodes list each frame (it had kept deleted nodes).

Through the menus into a skirmish (yr_standins.py's route), with scripted clicks (`@cmd=PlanningMode`,
glue/yr_app_hooks.cpp) and harness orders (glue/yr_harness.cpp select/plan/plans/where):
  frame 100  every tank selected; 110 PlanningMode clicked (planning mode on);
  frame 130, 150  two orders as map clicks give them (`plan`, 4 cells east, then 4 east and 4 south of the first
             tank; Player_Assign_Mission, the click's path): two planned waypoints (nodes);
  frame 160  plans: planning mode on, two nodes;
  frame 170  PlanningMode clicked again: the plan is committed (planning mode off) and the tanks set out;
  frame 300  plans and where: planning mode off, the first tank moved from where it was at frame 100.

  yr_planning.py --binary <yr-native> --data-dir <game folder> --work <dir>
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
standins = importlib.import_module('yr_standins')
SCRIPT = standins.SCRIPT + ''.join(l + '\n' for l in (
    '110 click @cmd=PlanningMode', '170 click @cmd=PlanningMode'))
ORDERS = ('100 select MTNK all; 101 where MTNK; 105 plans; 130 plan 4 0; 150 plan 4 4; 160 plans; 300 plans; '
          '300 where MTNK')
PLANS = re.compile(r'^harness-plans mode (\d) tokens \d+ nodes (\d+)$', re.M)
WHERE = re.compile(r'^harness-where MTNK px(-?\d+),(-?\d+) ', re.M)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--work', required=True)
    ap.add_argument('--seconds', type=int, default=130)
    a = ap.parse_args()
    if not a.data_dir or not os.path.exists(os.path.join(a.data_dir, 'gamemd.exe')):
        print('skipped: no Yuri\'s Revenge install (gamemd.exe) in --data-dir')
        return 77
    if a.binary.lower().endswith('.exe'):
        print('skipped: run on the Mac build (the Windows build has the same game code)')
        return 77
    shutil.rmtree(a.work, ignore_errors=True)
    os.makedirs(os.path.join(a.work, 'w'))
    with open(os.path.join(a.work, 'w', 'RA2MD.INI'), 'w') as f:
        f.write(standins.SETTINGS)
    script = os.path.join(a.work, 'planning.script')
    with open(script, 'w') as f:
        f.write(SCRIPT)
    env = dict(os.environ, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_MSG_LOG='0',
               RA2_NATIVE_ORDERS=ORDERS)
    env.pop('RA2_NATIVE_FORCE_END', None)
    env.pop('RA2_NATIVE_TRAP_CONTINUE', None)
    cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=never', '--run-seconds=%d' % a.seconds,
           '--write-dir=' + os.path.join(a.work, 'w'), '--input-script=' + script, '--game-args=-noaudio -CD.']
    with open(os.path.join(a.work, 'run.log'), 'w') as log:
        game_process.run(cmd, env=env, stdout=log, stderr=subprocess.STDOUT, timeout=a.seconds + 120)
    text = open(os.path.join(a.work, 'run.log'), errors='replace').read()
    failures = []
    plans = [(int(m.group(1)), int(m.group(2))) for m in PLANS.finditer(text)]
    where = [(int(m.group(1)), int(m.group(2))) for m in WHERE.finditer(text)]
    print('plans (mode, nodes) at frames 105, 160, 300: %s' % plans)
    print('first tank on the view at frames 101 and 300: %s' % where)
    if len(plans) < 3:
        failures.append('only %d of 3 "harness-plans" lines (the game did not reach frame 300)' % len(plans))
    else:
        if plans[0][0] != 0:
            failures.append('planning mode on before the button')
        if plans[1] != (1, 2):
            failures.append('not two planned nodes in planning mode: %s' % (plans[1],))
        if plans[2][0] != 0 or plans[2][1] >= 2:
            failures.append('the plan was not committed and followed: %s' % (plans[2],))
    if len(where) < 2 or where[0] == where[1]:
        failures.append('the first tank did not move: %s' % where)
    if 'no command bar button' in text:
        failures.append('no PlanningMode button on the command bar')
    bad = sorted(set(l for l in text.splitlines() if standins.BAD.search(l)))
    if bad:
        failures.append('traps or faults: %s' % '; '.join(bad[:5]))
    for f in failures:
        print('FAIL: ' + f)
    print('%s: planning mode from the command bar (%s)' % ('FAIL' if failures else 'OK', a.work))
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
