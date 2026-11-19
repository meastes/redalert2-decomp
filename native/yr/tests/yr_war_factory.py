#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest yr-native-war-factory (lane N3; built with -DRA2_NATIVE_YR=ON): a finished vehicle leaves an Allied War
Factory (GAWEAP) and a finished soldier leaves an Allied Barracks (GAPILE), through the harness.

Finished production leaves its factory through BuildingClass::Exit_Object (glue/building_native.inc, a NON-EXACT
draft): a war factory takes the vehicle in and switches to MISSION_UNLOAD, which BuildingClass::Do_MISSION_UNLOAD
(NON-EXACT, lane Y15) runs to drive it out; a barracks puts the soldier at its door with a move order. Found by
this test (lane N3): the drafts gave those missions with the YR tree's ObjectClass::Assign_Mission, an empty
virtual, so the vehicle stayed inside the factory and every mission they gave was dropped; they now call
QueueMission(mission, false) as gamemd.exe does.

Through the menus into a skirmish (yr_standins.py's route), harness orders (glue/yr_harness.cpp
spawn/produce/produced):
  frame 60   a War Factory spawned for the player; 160 a Grizzly tank (MTNK) produced in it;
  frames 161-300  the factory unloads (mission 16) and returns to guard (5); the tank is outside its foundation;
  frame 310  a Barracks spawned; 400 a GI (E1) produced in it; 520 the GI has walked from the door.

  yr_war_factory.py --binary <yr-native> --data-dir <game folder> --work <dir>
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
ORDERS = ('60 spawn GAWEAP; 160 produce GAWEAP MTNK; 161 produced; 190 produced; 220 produced; 260 produced; '
          '300 produced; 310 spawn GAPILE; 400 produce GAPILE E1; 401 produced; 520 produced')
PRODUCED = re.compile(r'^harness-produced (\S+) cell (-?\d+),(-?\d+) factory (-?\d+),(-?\d+) limbo (\d) mission (-?\d+) '
                      r'factory-mission (-?\d+)$', re.M)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--work', required=True)
    ap.add_argument('--seconds', type=int, default=190)
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
    script = os.path.join(a.work, 'factory.script')
    with open(script, 'w') as f:
        f.write(standins.SCRIPT)
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
    rows = [(m.group(1), (int(m.group(2)), int(m.group(3))), (int(m.group(4)), int(m.group(5))), int(m.group(6)),
             int(m.group(8))) for m in PRODUCED.finditer(text)]
    tank = [r for r in rows if r[0] == 'MTNK']
    gi = [r for r in rows if r[0] == 'E1']
    print('tank: %s' % [(r[1], r[4]) for r in tank])
    print('GI: %s' % [(r[1], r[4]) for r in gi])
    if len(tank) < 5:
        failures.append('only %d of 5 reports on the tank (no factory, or the game did not reach frame 300)' % len(tank))
    else:
        if 16 not in [r[4] for r in tank]:
            failures.append('the war factory never unloaded (mission 16)')
        if tank[-1][4] == 16:
            failures.append('the war factory is still unloading at frame 300')
        if tank[-1][3] or tank[-1][1] == tank[0][1]:
            failures.append('the tank did not drive out: %s -> %s' % (tank[0][1], tank[-1][1]))
    if len(gi) < 2:
        failures.append('only %d of 2 reports on the GI' % len(gi))
    elif gi[-1][3] or gi[-1][1] == gi[0][1]:
        failures.append('the GI did not walk from the barracks door: %s -> %s' % (gi[0][1], gi[-1][1]))
    bad = sorted(set(l for l in text.splitlines() if standins.BAD.search(l)))
    if bad:
        failures.append('traps or faults: %s' % '; '.join(bad[:5]))
    for f in failures:
        print('FAIL: ' + f)
    print('%s: production leaves its factory (%s)' % ('FAIL' if failures else 'OK', a.work))
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
