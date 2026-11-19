#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest yr-native-keys-load (lane N5; built with -DRA2_NATIVE_YR=ON): keys and a load from inside a game that reached
functions not recovered yet (PORTS_REMAINING: the functions still missing at the Mac link).

Through the menus into a skirmish (yr_standins.py's route), saved at frame 300 (RA2_NATIVE_SAVE_AT), then:
  frame 400  Tab: Keyboard_Process's plain Tab (FUN_006ABC40 -> SpecialDialog 8) opens the Diplomacy dialog, whose fill
             called RadarClass::FUN_00658330 (now a stand-in, glue/radar_native.inc); the dialog's OK (0x423) closes it;
  frame 700  Delete: the Delete command (FUN_00731A10) with no planned node removes the player's beacon
             (BeaconManagerClass::DeleteBeacon; the tree called it Remove_Beacon, now bound by name);
  frame 800  two combat lights made (`lights add`: a SpotLightClass, as a bright warhead's explosion makes one);
  then Escape, Load Game (1310), the save's row, Load: Delete_All_Objects deletes the living lights (the tree's
             SpotlightClass destructor, now bound to SpotLightClass's) and YRKEYS.SAV loads; 300 more frames played.

Checks: no trap or fault; the Diplomacy dialog was shown and closed; two lights were alive before the load and none
after; the save was read back (the storage log's loaded objects) and the game went on.

  yr_keys_load.py --binary <yr-native> --data-dir <game folder> --work <dir>
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
    '400 key Tab', 's4+1500 dump', 's4+2000 click @ctl=1059',
    '700 key Delete',
    '820 key Escape', 's5+1500 click @ctl=1310', 's6+2000 click 300 137', 's6+2600 click @ctl=1039', 's7+3000 move 316 300'))
ORDERS = '790 lights; 800 lights add; 801 lights add; 802 lights; 1300 lights; 1301 scenario'
LIGHTS = re.compile(r'^harness-lights (\d+)$', re.M)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--work', required=True)
    ap.add_argument('--seconds', type=int, default=90)
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
    script = os.path.join(a.work, 'keys.script')
    with open(script, 'w') as f:
        f.write(SCRIPT)
    env = dict(os.environ, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_MSG_LOG='0',
               RA2_NATIVE_DIALOG_LOG='1', RA2_NATIVE_STG_LOG='1', RA2_NATIVE_ORDERS=ORDERS,
               RA2_NATIVE_SAVE_AT='300,YRKEYS.SAV', RA2_NATIVE_SCRIPT_AS_USER='1',
               RA2_NATIVE_WINDOW_DUMP=os.path.join(a.work, 'shot'))
    for k in ('RA2_NATIVE_FORCE_END', 'RA2_NATIVE_TRAP_CONTINUE'):
        env.pop(k, None)
    cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=never', '--run-seconds=%d' % a.seconds,
           '--write-dir=' + os.path.join(a.work, 'w'), '--input-script=' + script, '--game-args=-noaudio -CD.']
    with open(os.path.join(a.work, 'run.log'), 'w') as log:
        game_process.run(cmd, env=env, stdout=log, stderr=subprocess.STDOUT, timeout=a.seconds + 120)
    text = open(os.path.join(a.work, 'run.log'), errors='replace').read()
    failures = []
    shown = re.findall(r'^dialog shown #(\d+)', text, re.M)
    lights = [int(n) for n in LIGHTS.findall(text)]
    saved = 'harness-save: frame 300: saved YRKEYS.SAV' in text
    loaded = len(re.findall(r'load object \d+ class', text))
    print('dialogs shown %s; lights %s; saved %s; objects loaded %d; scenario after the load %s' % (
        shown, lights, saved, loaded, re.findall(r'^harness-scenario (\S+)$', text, re.M)))
    if len(shown) < 6:
        failures.append('fewer dialogs than the route needs (the Diplomacy dialog, the options menu, the load dialog): %s' % shown)
    if not saved:
        failures.append('not saved at frame 300')
    if lights != [0, 1, 2, 2, 0]:
        failures.append('combat lights %s, expected [0, 1, 2, 2, 0] (none, one, two, still two, none after the load)' % lights)
    diplomacy = re.search(r'^dialog shown #4 .*\n(?:dialog 0x.*\n)?(?:  control .*\n)*', text, re.M)
    if not diplomacy or 'control 1041 ' not in diplomacy.group(0):
        failures.append('the 4th dialog is not the Diplomacy dialog (no player name control 1041)')
    if loaded == 0:
        failures.append('YRKEYS.SAV was not loaded from the load dialog')
    if not re.search(r'^harness-scenario ', text, re.M):
        failures.append('the loaded game did not reach frame 1300')
    bad = sorted(set(l for l in text.splitlines() if standins.BAD.search(l)))
    if bad:
        failures.append('traps or faults: %s' % '; '.join(bad[:5]))
    for f in failures:
        print('FAIL: ' + f)
    print('%s: Tab (Diplomacy), Delete, a load with combat lights alive (%s)' % ('FAIL' if failures else 'OK', a.work))
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
