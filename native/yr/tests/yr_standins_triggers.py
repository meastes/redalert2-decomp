#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest yr-native-standins-triggers (lane N3; built with -DRA2_NATIVE_YR=ON): stand-ins and bindings for functions
the campaign missions' triggers and the keyboard reach, at work in one Yuri's Revenge skirmish on the Mac (the menus'
route of yr-native-standins). The harness (glue/yr_harness.cpp, RA2_NATIVE_ORDERS):

  radial      a Gap Generator (GAGAP) for the player, selected: its range ring is drawn
              (BuildingClass::Draw_Radial_Indicator -> XSurface::Draw_Ellipse, a NON-EXACT stand-in);
  create      an Allied power plant (GAPOWR) placed as the trigger action "create building at" places it
              (BuildingClass::ForceCreate, a NON-EXACT stand-in) on the cell of the player's first tank: the tanks
              there are moved aside, none is lost;
  storm       a lightning bolt as the trigger action "ion lightning strike" (IonStormClass::Lightning_Bolt, bound to
              the matched LightningStorm::Strike), a lightning storm started as "ion storm start" does with no
              deferment (LightningStorm::Start) and stopped as "ion storm stop" (Ion_Storm_End, bound to
              LightningStorm::RequestStop);
  beacon      the Beacon command's placement mode (the command bar's button in network games), then a click on
              the map: a beacon is placed
              (BeaconManagerClass::PlaceBeacon) and drawn (Drawing::Intersect), both NON-EXACT stand-ins;
  nav         the HealthNav and VeterancyNav keys (FUN_00733380, FUN_007336C0, NON-EXACT stand-ins), three presses
              each with nothing selected: the third selects the player's healthy (little experienced) units on the
              screen.

Checks: each order found its units; the building placed and no tank lost; the storm active when stopped; tanks
selected by both keys; no link-closure trap or fault. Skipped (77) without the installed game (gamemd.exe in
--data-dir) and for the Windows build.

  yr_standins_triggers.py --binary <yr-native> --data-dir <game folder> --work <dir> [--seconds 150]
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

ORDERS = ['60 spawn GAGAP', '62 select GAGAP 0', '100 owner MTNK', '110 create GAPOWR on MTNK', '200 owner MTNK',
          '210 select NONE 0', '220 cmd HealthNav', '222 cmd HealthNav', '224 cmd HealthNav', '230 groups MTNK',
          '240 select NONE 0', '250 cmd VeterancyNav', '252 cmd VeterancyNav', '254 cmd VeterancyNav',
          '260 groups MTNK', '270 bolt enemy', '280 stormstart', '300 stormend',
          '310 beacons', '320 beaconmode', '360 beacons']
# The Beacon command's placement mode (a skirmish's command bar has no Beacon button; a network game's has),
# then a click on the tactical view (DisplayClass::Mouse_Left_Release -> BeaconManagerClass::PlaceBeacon), as a
# player places one; drawn every frame after (Drawing::Intersect).
SCRIPT_EXTRA = '330 click 300 250\n'
GROUPS = re.compile(r'^harness-groups MTNK((?: -?\d+\*?)+)$', re.M)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--work', required=True)
    ap.add_argument('--seconds', type=int, default=150)
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
    script = os.path.join(a.work, 'triggers.script')
    with open(script, 'w') as f:
        f.write(standins.SCRIPT + SCRIPT_EXTRA)
    env = dict(os.environ, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_MSG_LOG='0',
               RA2_NATIVE_ORDERS='; '.join(ORDERS))
    env.pop('RA2_NATIVE_FORCE_END', None)
    env.pop('RA2_NATIVE_TRAP_CONTINUE', None)
    cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=never', '--run-seconds=%d' % a.seconds,
           '--write-dir=' + os.path.join(a.work, 'w'), '--input-script=' + script, '--game-args=-noaudio -CD.']
    with open(os.path.join(a.work, 'run.log'), 'w') as log:
        game_process.run(cmd, env=env, stdout=log, stderr=subprocess.STDOUT, timeout=a.seconds + 120)
    text = open(os.path.join(a.work, 'run.log'), errors='replace').read()
    failures = []

    def owners(frame):
        block = text.split('harness-order %d owner MTNK' % frame)[0].rsplit('harness-order', 1)[-1]
        return re.findall(r'^harness-owner MTNK h(-?\d+) (p|e)( limbo)? &(\S+)', block, re.M)
    if not re.search(r'^harness-spawn GAGAP ', text, re.M) or not re.search(r'^harness-select GAGAP 0 ', text, re.M):
        failures.append('no Gap Generator spawned and selected')
    created = re.search(r'^harness-create GAPOWR (ok|failed) @(\d+),(\d+)', text, re.M)
    before, after = owners(100), owners(200)
    print('create: %s; tanks (all houses) %d before, %d after' % (created.group(0) if created else None, len(before), len(after)))
    if not created or created.group(1) != 'ok':
        failures.append('the power plant was not placed with ForceCreate')
    before, after = [x for x in before if x[1] == 'p'], [x for x in after if x[1] == 'p']
    if len(before) < 2 or len(after) != len(before) or any(x[2] for x in after):
        failures.append('tanks lost or left off the map by ForceCreate (%d -> %d)' % (len(before), len(after)))
    print('storm: %s' % re.findall(r'^harness-(?:bolt|stormstart|stormend) .*$', text, re.M))
    if not re.search(r'^harness-bolt @', text, re.M):
        failures.append('no lightning bolt')
    if not re.search(r'^harness-stormstart @\d+,\d+ active$', text, re.M):
        failures.append('the storm did not start')
    if not re.search(r'^harness-stormend active$', text, re.M):
        failures.append('the storm was not active when stopped')
    beacons = [int(n) for n in re.findall(r'^harness-beacons (\d+)$', text, re.M)]
    print('beacons before and after the beacon mode and a map click: %s' % beacons)
    if len(beacons) < 2 or beacons[1] != beacons[0] + 1:
        failures.append('no beacon placed by the map click (%s)' % beacons)
    groups = [m.group(1).split() for m in GROUPS.finditer(text)]
    for i, key in enumerate(('HealthNav', 'VeterancyNav')):
        got = groups[i] if i < len(groups) else None
        print('%s x3: %s' % (key, got))
        if text.count('harness-cmd %s\n' % key) != 3:
            failures.append('%s did not run three times' % key)
        if not got or not any(g.endswith('*') for g in got):
            failures.append('%s selected no tank' % key)
    bad = sorted(set(l for l in text.splitlines() if standins.BAD.search(l)))
    if bad:
        failures.append('traps or faults: %s' % '; '.join(bad[:5]))
    for f in failures:
        print('FAIL: ' + f)
    print('%s: trigger and keyboard stand-ins (%s)' % ('FAIL' if failures else 'OK', a.work))
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
