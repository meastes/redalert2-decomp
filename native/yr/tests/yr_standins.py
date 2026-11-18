#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest yr-native-standins (lane Y16; built with -DRA2_NATIVE_YR=ON): the newly written stand-ins (NON-EXACT
drafts of lanes Y14-Y16) at work in one Yuri's Revenge skirmish on the Mac, through the real menus (Single
Player, Skirmish, Start Game; The Alamo against one easy opponent, as yr-native-combat). The harness
(glue/yr_harness.cpp, RA2_NATIVE_ORDERS) spawns the units next to the player's base and gives the orders:

  beams       a Magnetron (TELE: WaveClass::FUN_007602E0, wave type 3), a Prism Tank (SREF) and a Mirage Tank
              (MGTK) fire on two enemy Apocalypse tanks spawned beside them; a sonic wave (type 0,
              WaveClass::Draw_Sonic) and a magnetron wave are also made directly (`wave 0`, `wave 3`): waves
              are seen in flight (`waves`) and the targets take damage;
  spy         a Spy (SPY: BuildingClass::Spied_By) is sent into an enemy Allied power plant (GAPOWR): the spy
              leaves the map (it went in);
  superweapons the Psychic Dominator (HouseClass::Fire_PsyDom) and the Lightning Storm (LightningStorm::Start)
              are granted, charged and placed on the enemy's base.

Checks: each order found its units; waves in flight; the targets were destroyed; the both superweapons placed; no link-closure trap or fault for the whole run. Skipped (77) without
the installed game (gamemd.exe in --data-dir).

  yr_standins.py --binary <yr-native> --data-dir <game folder> --work <dir> [--seconds 170]
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
"""

SETTINGS = ('[Intro]\nPlay=no\n\n[Skirmish]\nGameMode=1\nScenIndex=0\nGameSpeed=1\nCredits=10000\nUnitCount=10\n'
            'ShortGame=yes\nSuperWeaponsAllowed=yes\nBuildOffAlly=yes\nMCVRepacks=yes\nCratesAppear=yes\n'
            + ''.join('Slot%02d=%d,-2,-2\n' % (i, 6 if i == 1 else 1) for i in range(1, 8)))

ORDERS = ['60 spawn TELE', '61 spawn SREF', '62 spawn MGTK', '63 spawn HTNK enemy', '64 spawn HTNK enemy',
          '67 spawn SPY', '68 spawn GAPOWR enemy', '70 owner HTNK',
          '80 fire TELE HTNK', '80 fire SREF HTNK', '80 fire MGTK HTNK', '82 enter SPY GAPOWR',
          '90 wave 0', '91 wave 3', '95 waves', '120 waves', '150 waves', '200 waves', '260 owner HTNK',
          '300 owner SPY', '320 super PsychicDominator enemy', '500 super LightningStorm enemy',
          '700 owner SPY', '900 owner HTNK']
BAD = re.compile(r'unrecovered|SIG(SEGV|BUS|ILL|ABRT|FPE)|crash:', re.I)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--work', required=True)
    ap.add_argument('--seconds', type=int, default=170)
    a = ap.parse_args()
    if not a.data_dir or not os.path.exists(os.path.join(a.data_dir, 'gamemd.exe')):
        print('skipped: no Yuri\'s Revenge install (gamemd.exe) in --data-dir')
        return 77
    shutil.rmtree(a.work, ignore_errors=True)
    os.makedirs(os.path.join(a.work, 'w'))
    with open(os.path.join(a.work, 'w', 'RA2MD.INI'), 'w') as f:
        f.write(SETTINGS)
    script = os.path.join(a.work, 'standins.script')
    with open(script, 'w') as f:
        f.write(SCRIPT)
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
    for unit in ('TELE', 'SREF', 'MGTK', 'SPY'):
        if not re.search(r'^harness-spawn %s h\d+ ' % unit, text, re.M):
            failures.append('%s not spawned' % unit)
    for order in ('fire TELE', 'fire SREF', 'fire MGTK', 'enter SPY'):
        if not re.search(r'^harness-%s ' % order, text, re.M):
            failures.append('no %s order' % order)
    waves = [int(n) for n in re.findall(r'^harness-waves (\d+) ', text, re.M)]
    print('standins beams: waves in flight %s' % waves)
    if not any(waves):
        failures.append('no wave in flight')

    def owners(frame, unit):
        block = text.split('harness-order %d owner %s' % (frame, unit))[0].rsplit('harness-order', 1)[-1]
        return re.findall(r'^harness-owner %s h(-?\d+) (p|e)( limbo)? &(\S+)' % unit, block, re.M)
    # the enemy Apocalypses: present at frame 70, fewer or gone by 900 (destroyed by the beams and the base)
    before, after = owners(70, 'HTNK'), owners(900, 'HTNK')
    enemy_before = [x for x in before if x[1] == 'e']
    enemy_after = [x for x in after if x[1] == 'e' and x[3] in {b[3] for b in enemy_before}]
    print('standins beams: enemy Apocalypses %d at frame 70, %d of them left at frame 900' % (len(enemy_before), len(enemy_after)))
    if len(enemy_before) < 2 or len(enemy_after) >= len(enemy_before):
        failures.append('the beams destroyed no Apocalypse (%d -> %d)' % (len(enemy_before), len(enemy_after)))
    spy = owners(700, 'SPY')
    print('standins spy: spies at frame 700 %s' % (spy or 'none'))
    if any(not x[2] for x in spy):
        failures.append('the spy is still on the map')
    for name in ('PsychicDominator', 'LightningStorm'):
        if not re.search(r'^harness-super %s\S* @' % name, text, re.M):
            failures.append('%s not placed' % name)
    bad = sorted(set(l for l in text.splitlines() if BAD.search(l)))
    if bad:
        failures.append('traps or faults: %s' % '; '.join(bad[:5]))
    for f in failures:
        print('FAIL: ' + f)
    print('%s: the stand-ins in a skirmish (%s)' % ('FAIL' if failures else 'OK', a.work))
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
