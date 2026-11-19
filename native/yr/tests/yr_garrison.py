#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest yr-native-garrison (lane N4; built with -DRA2_NATIVE_YR=ON): a GI sent into a civilian building garrisons
it, and leaves it again when the building is told to unload (the user's Duo fault of 2026-11-19: GIs sent into a
building disappeared).

The cause: InfantryClass::Per_Cell_Process (glue/infantry_native.inc, a NON-EXACT draft from TibSun's text) had no
garrisons. A soldier that reached the cell of the building it was sent to capture fell through to TibSun's
capture code, which deletes a unit that is neither an engineer nor a spy. gamemd.exe (0x00519675) first lets an
occupier (or an assaulter) garrison the building (InfantryClass::FUN_00522910: Limbo, into Occupants); the draft
now does the same.

Through the menus into a skirmish (yr_standins.py's route), harness orders (glue/yr_harness.cpp):
  frame 60   a civilian house (CAHSE07, CanBeOccupied) spawned near the player's base;
  frame 81   the player's first GI (E1) clicked onto it (What_Action, Active_Click_With: action Capture,
             MISSION_CAPTURE with the house as the destination, a mouse click's order);
  frame 700  the house is the player's with one occupant; the GI is in limbo (inside), not deleted;
  frame 710  the house unloads (MISSION_UNLOAD, the deploy order); 800 no occupants, the GI is back on the map.

  yr_garrison.py --binary <yr-native> --data-dir <game folder> --work <dir>
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
ORDERS = ('60 spawn CAHSE07 6 0; 81 click E1 CAHSE07; 82 owner E1; 700 occupants CAHSE07; 700 owner E1; '
          '710 deploy CAHSE07; 800 occupants CAHSE07; 800 owner E1')
OCCUPANTS = re.compile(r'^harness-occupants CAHSE07 h(-?\d+) (\d+)(p?)$', re.M)
OWNER = re.compile(r'^harness-owner E1 h(-?\d+) ([pe])( limbo)? &(\S+)(?: @-?\d+,-?\d+ m-?\d+)?$', re.M)   # lane N6: the cell and mission after
ORDER_DONE = re.compile(r'^harness-order (\d+) owner E1: ', re.M)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--work', required=True)
    ap.add_argument('--seconds', type=int, default=85)
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
    script = os.path.join(a.work, 'garrison.script')
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
    if not re.search(r'^harness-click E1 action \d+ -> CAHSE07 ok$', text, re.M):
        failures.append('no house or no GI to click (the order failed)')
    occupants = [(int(m.group(2)), m.group(3) == 'p') for m in OCCUPANTS.finditer(text)]
    # the owner reports, split by order (82, 700, 800): the clicked GI is the player's first E1 at frame 82
    blocks = {}
    for line in text.splitlines():
        m = OWNER.match(line)
        if m and m.group(2) == 'p':
            blocks.setdefault('cur', []).append((m.group(4), bool(m.group(3))))
        d = ORDER_DONE.match(line)
        if d:
            blocks[int(d.group(1))] = blocks.pop('cur', [])
    print('occupants at 700, 800: %s' % occupants)
    gi = blocks.get(82, [(None, False)])[0][0]
    state = {f: [s for (addr, s) in blocks.get(f, []) if addr == gi] for f in (700, 800)}
    print('the clicked GI %s: at 700 %s, at 800 %s (True: in limbo)' % (gi, state[700], state[800]))
    if len(occupants) < 2:
        failures.append('the game did not reach frame 800')
    else:
        if occupants[0] != (1, True):
            failures.append('frame 700: the house is not the player\'s with one occupant: %s' % (occupants[0],))
        if occupants[1][0] != 0:
            failures.append('frame 800: the house did not unload its occupant')
    if gi is None:
        failures.append('no GI of the player\'s at frame 82')
    else:
        if state[700] != [True]:
            failures.append('frame 700: the GI is %s (expected inside the house, in limbo)'
                            % ('gone' if not state[700] else 'still on the map'))
        if state[800] != [False]:
            failures.append('frame 800: the GI is not back on the map: %s' % state[800])
    bad = sorted(set(l for l in text.splitlines() if standins.BAD.search(l)))
    if bad:
        failures.append('traps or faults: %s' % '; '.join(bad[:5]))
    for f in failures:
        print('FAIL: ' + f)
    print('%s: a GI garrisons a civilian house and leaves it (%s)' % ('FAIL' if failures else 'OK', a.work))
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
