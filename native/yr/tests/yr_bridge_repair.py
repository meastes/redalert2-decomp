#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest yr-native-bridge-repair (lane N4; built with -DRA2_NATIVE_YR=ON): an engineer sent into a bridge repair hut
repairs the bridge beside it, and a hut blown with C4 destroys it again (PORTS_REMAINING item 2).

The skirmish map at ScenIndex 3 (XDEADMAN.MAP) starts with its high bridges broken and a bridge repair hut (CABHUT)
at each end. Found by this test: InfantryClass::Per_Cell_Process (glue/infantry_native.inc, TibSun's text) asked the
navigation target Considered_Vehicle(), TibSun's name for what the YR tree calls IsStrange (vtable 0x80; a building
that counts as a vehicle); the YR tree's Considered_Vehicle is another slot (Height > -20), true for every building.
So an engineer that reached a hut "captured" it as a vehicle and vanished, and the bridge was never repaired. The
same wrong call stood in the bullet, particle and bounce drafts (every building let shots and debris through) and in
the sale code (the player's sell sound never played); all now call IsStrange, as gamemd.exe does.

Through the menus into a skirmish (yr_standins.py's route, ScenIndex 3), harness orders (glue/yr_harness.cpp):
  frame 40   `scenario` XDEADMAN.MAP; `bridge` 1 (the first hut's bridge needs repair: MapClass::Can_Repair_Bridge);
  frame 41   an engineer placed beside the hut and sent into it (`repairhut ENGINEER`, an engineer's click);
  frame 200  `bridge` 0 (repaired), the engineer gone into the hut;
  frame 300  the hut blown as C4 blows it (`blowhut`); 400 `bridge` 1 (destroyed again);
  frame 410  a second engineer; 600 `bridge` 0 (repaired again).

  yr_bridge_repair.py --binary <yr-native> --data-dir <game folder> --work <dir>
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
ORDERS = ('40 scenario; 40 bridge; 41 repairhut ENGINEER; 200 bridge; 201 owner ENGINEER; 300 blowhut; 400 bridge; '
          '410 repairhut ENGINEER; 600 bridge')
BRIDGE = re.compile(r'^harness-bridge (\d)$', re.M)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--work', required=True)
    ap.add_argument('--seconds', type=int, default=75)
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
        f.write(standins.SETTINGS.replace('ScenIndex=0', 'ScenIndex=3'))
    script = os.path.join(a.work, 'bridge.script')
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
    scen = re.search(r'^harness-scenario (\S+)$', text, re.M)
    states = [int(s) for s in BRIDGE.findall(text)]
    print('scenario %s; bridge needs repair at 40, 200, 400, 600: %s' % (scen and scen.group(1), states))
    if not scen or scen.group(1).upper() != 'XDEADMAN.MAP':
        failures.append('not the expected map (XDEADMAN.MAP): %s' % (scen and scen.group(1)))
    if len(re.findall(r'^harness-repairhut ENGINEER ', text, re.M)) < 2:
        failures.append('an engineer could not be placed beside the hut')
    if states != [1, 0, 1, 0]:
        failures.append('the bridge readings are %s, expected [1, 0, 1, 0] (broken, repaired, blown, repaired)' % states)
    bad = sorted(set(l for l in text.splitlines() if standins.BAD.search(l)))
    if bad:
        failures.append('traps or faults: %s' % '; '.join(bad[:5]))
    for f in failures:
        print('FAIL: ' + f)
    print('%s: an engineer repairs a bridge (%s)' % ('FAIL' if failures else 'OK', a.work))
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
