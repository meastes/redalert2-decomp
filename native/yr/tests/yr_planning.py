#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest yr-native-planning (lane N3; built with -DRA2_NATIVE_YR=ON): the command bar's PlanningMode button.

Planning mode's functions are unmatched in the YR tree and were link-closure traps: with units selected the
button stopped the game at its first draw (FUN_0063B550). glue/planmgr_planning_native.inc has NON-EXACT
stand-ins (the planned order, the commit, the branch filing, the outline and filled draws); a fixup empties
the drawn-nodes list each frame (it had kept deleted nodes).

Through the menus into a skirmish (yr_standins.py's route), with scripted clicks (`@cmd=PlanningMode`,
glue/yr_app_hooks.cpp) and harness orders (glue/yr_harness.cpp select/plan/plans/where):
  frame 100  every tank selected; 102 the view centred on them (`centermap`: the planned nodes on the view);
             110 PlanningMode clicked (planning mode on);
  frame 130, 140, 150  three orders as map clicks give them (`plan`, the cells -3,0, -3,3 and 0,3 from the first
             tank's; Player_Assign_Mission, the click's path): three planned waypoints (nodes);
  frame 160  plans: planning mode on, three nodes;
  frame 170-178  (lane N23) the mouse on the second node and a left click there (`@node=1`): the next to last node
             of the path cannot close a loop, so planning mode refuses the press (FUN_0063A5A0's InvalidNodeX;
             the refusal flag is cleared each frame, so only its effect is checked: planloop at 181, the path
             still has three nodes; the old build's map click on a planned node planned nothing either);
  frame 182-192  the mouse on the first node (it becomes the hovered node; FUN_0063AB60 keeps the normal cursor),
             the left button down (FUN_0063A5A0 picks the node up; planloop at 188: picked 0) and up
             (FUN_0063A8E0 connects the paths to it: a closed loop; planloop at 192-196: loop start 0 over 3
             nodes, still three nodes);
  frame 200  PlanningMode clicked again: the plan is committed (planning mode off) and the tanks set out;
  frame 300  plans and where: planning mode off, the first tank moved from where it was at frame 100.
On the build before lane N23 (FUN_0063AB60 / FUN_0063A5A0 returning false) the first node is hovered but not
picked up and no loop is closed: the test fails there.

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
    '110 click @cmd=PlanningMode',
    '170 move @node=1', '174 ldown @node=1', '176 lup @node=1',
    '182 move @node=0', '186 ldown @node=0', '190 lup @node=0',
    '200 click @cmd=PlanningMode'))
ORDERS = ('100 select MTNK all; 101 where MTNK; 102 centermap; 105 plans; 130 plan -3 0; 140 plan -3 3; 150 plan 0 3; '
          '160 plans; 165 planloop; 180 plans; 181 planloop; 184 planloop; 188 planloop; 192 planloop; 194 planloop; '
          '196 planloop; 198 plans; 300 plans; 300 where MTNK')
PLANS = re.compile(r'^harness-plans mode (\d) tokens \d+ nodes (\d+)$', re.M)
LOOP = re.compile(r'^harness-planloop picked (-?\d+) hover (-?\d+) refused \d loop (-?\d+) (-?\d+) path (-?\d+) ', re.M)
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
    loops = [tuple(int(g) for g in m.groups()) for m in LOOP.finditer(text)]
    print('plans (mode, nodes) at frames 105, 160, 180, 198, 300: %s' % plans)
    print('planloop (picked, hover, loop start, loop nodes, path nodes) at frames 165, 181, 184, 188, 192, 194, 196: %s'
          % loops)
    print('first tank on the view at frames 101 and 300: %s' % where)
    if len(plans) < 5:
        failures.append('only %d of 5 "harness-plans" lines (the game did not reach frame 300)' % len(plans))
    else:
        if plans[0][0] != 0:
            failures.append('planning mode on before the button')
        if plans[1] != (1, 3):
            failures.append('not three planned nodes in planning mode: %s' % (plans[1],))
        if plans[2] != (1, 3):
            failures.append('a node was added at the press on the second node: %s' % (plans[2],))
        if plans[3] != (1, 3):
            failures.append('the press on the first node planned a waypoint: %s' % (plans[3],))
        if plans[4][0] != 0:
            failures.append('the plan was not committed: %s' % (plans[4],))
    if len(loops) < 7:
        failures.append('only %d of 7 "harness-planloop" lines' % len(loops))
    else:
        if loops[0][4] != 3:
            failures.append('the path is not the three planned nodes: %s' % (loops[0],))
        if loops[1][4] != 3:
            failures.append('the press on the second node changed the path: %s' % (loops[1],))
        if loops[2][1] != 0:
            failures.append('the first node is not the hovered node with the mouse on it: %s' % (loops[2],))
        if loops[3][0] != 0:
            failures.append('the left press did not pick up the first node: %s' % (loops[3],))
        if loops[-1][0] != -1 or loops[-1][2] != 0 or loops[-1][3] != 3:
            failures.append('the release on the first node did not close the path into a loop: %s' % (loops[-1],))
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
