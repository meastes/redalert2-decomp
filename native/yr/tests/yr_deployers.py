#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest yr-native-deployers (lane N13; built with -DRA2_NATIVE_YR=ON): soldiers that deploy (Guardian GI, GI,
Desolator) deploy and undeploy on command, the Siege Chopper deploys, and the first Allied mission's Guardian GIs
deploy by themselves (the user's Duo fault of 2026-11-19: they stood and died).

The cause: infantry.h's `MasterDoControls[DO_COUNT]` is TibSun's 21 infantry actions; Yuri's Revenge has 42
(gamemd.exe 0x007EAF7C). yr_data.py sized and loaded the table from the declaration, so the deploy action (0x1B)
and every later one read the next global's bytes: rate 0, the deploy animation never advanced and the soldier
stayed in its deploy frame (`do27`) for ever. fixups.json declares the original's 42.

Two runs:
  a skirmish (yr_standins.py's route), harness orders (glue/yr_harness.cpp):
    frame 60   a Guardian GI, a GI and a Siege Chopper spawned for the player beside the base, a Desolator
               well away from them (its deployed radiation kills soldiers);
    frame 90   each ordered to deploy (Queue_Mission MISSION_UNLOAD, the D key's order): the spawned one (lane N16:
               `deploy E1` once took the player's first GI, a starting soldier that sometimes stood in the
               Desolator's radiation and died, and `doing` then reported another, unordered GI);
    frame 300  `doing`: each soldier in its deployed idle animation (0x1C, `do28`), the chopper deployed;
    frame 320  the Guardian GI, the GI and the Desolator ordered to deploy again (to undeploy);
    frame 500  each soldier out of its deploy animations again (standing, `do0`);
  the first Allied mission (ALL01UMD, New Campaign through the menus, no forced end):
    frame 300  every Guardian GI of the mission deployed (`deployed GGI 10 of 10`; before the fix 0 of 10,
               all of them stuck deploying).

  yr_deployers.py --binary <yr-native> --data-dir <game folder> --work <dir>
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

SOLDIERS = ('GGI', 'E1', 'DESO')
SKIRMISH_ORDERS = ('60 spawn GGI 3 0; 60 spawn E1 4 1; 60 spawn SCHP 6 0; 60 spawn DESO -8 -8; '
                   '90 deploy GGI; 90 deploy E1; 90 deploy SCHP; 90 deploy DESO; '
                   '300 doing GGI; 300 doing E1; 300 doing DESO; 300 doing SCHP; '
                   '320 deploy GGI; 320 deploy E1; 320 deploy DESO; '
                   '500 doing GGI; 500 doing E1; 500 doing DESO')
MISSION_ORDERS = '40 scenario; 300 deployed GGI; 900 deployed GGI'
# 800x600 shell: Single Player, New Campaign, the Allied emblem (yr_campaign_chain.py)
MISSION_SCRIPT = 's1+1500 click 720 220\ns2+1500 click 720 220\ns3+2500 click 315 125\n'
DOING = re.compile(r'^harness-doing (\S+) do(-?\d+) m(-?\d+) deployer (\d) deployed (\d) s(-?\d+) ', re.M)
DEPLOYED = re.compile(r'^harness-deployed GGI (\d+) of (\d+) \((\d+) deploying\) dead (\d+)$', re.M)
SCENARIO = re.compile(r'^harness-scenario (\S+)$', re.M)


def run(a, name, orders, script, settings, seconds):
    work = os.path.join(a.work, name)
    os.makedirs(os.path.join(work, 'w'))
    with open(os.path.join(work, 'w', 'RA2MD.INI'), 'w') as f:
        f.write(settings)
    path = os.path.join(work, name + '.script')
    with open(path, 'w') as f:
        f.write(script)
    env = dict(os.environ, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_MSG_LOG='0',
               RA2_NATIVE_ORDERS=orders)
    env.pop('RA2_NATIVE_FORCE_END', None)
    env.pop('RA2_NATIVE_TRAP_CONTINUE', None)
    cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=never', '--run-seconds=%d' % seconds,
           '--write-dir=' + os.path.join(work, 'w'), '--input-script=' + path, '--game-args=-noaudio -CD.']
    with open(os.path.join(work, 'run.log'), 'w') as log:
        try:
            game_process.run(cmd, env=env, stdout=log, stderr=subprocess.STDOUT, timeout=seconds + 120)
        except subprocess.TimeoutExpired:
            pass
    return open(os.path.join(work, 'run.log'), errors='replace').read()


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--work', required=True)
    ap.add_argument('--seconds', type=int, default=45)
    a = ap.parse_args()
    if not a.data_dir or not os.path.exists(os.path.join(a.data_dir, 'gamemd.exe')):
        print('skipped: no Yuri\'s Revenge install (gamemd.exe) in --data-dir')
        return 77
    if a.binary.lower().endswith('.exe'):
        print('skipped: run on the Mac build (the Windows build has the same game code)')
        return 77
    shutil.rmtree(a.work, ignore_errors=True)
    failures = []

    text = run(a, 'skirmish', SKIRMISH_ORDERS, standins.SCRIPT, standins.SETTINGS, a.seconds)
    # the `doing` lines in order: the first of each type at frame 300, the second at frame 500
    seen = {}
    for m in DOING.finditer(text):
        seen.setdefault(m.group(1), []).append(m.groups()[1:])
    print('skirmish: %s' % '; '.join('%s %s' % (k, ' -> '.join('do%s deployed %s' % (v[0], v[3]) for v in vs))
                                     for k, vs in sorted(seen.items())))
    gone = set(re.findall(r'^harness-doing (\S+) gone$', text, re.M))
    for soldier in SOLDIERS:
        states = seen.get(soldier, [])
        if soldier in gone:
            failures.append('%s: the one spawned and ordered to deploy left the map (killed?) before frame 500' % soldier)
            continue
        if len(states) < 2:
            failures.append('%s: no state at frames 300 and 500 (not spawned, or it died): %s' % (soldier, states))
            continue
        if states[0][0] != '28' or states[0][3] != '1':
            failures.append('%s did not deploy: animation %s at frame 300 (28: deployed; 27: stuck deploying)'
                            % (soldier, states[0][0]))
        if states[1][3] != '0':
            failures.append('%s did not undeploy: animation %s, deployed %s at frame 500' % (soldier, states[1][0], states[1][3]))
    chopper = seen.get('SCHP', [])
    if not chopper or chopper[0][3] != '1':
        failures.append('the Siege Chopper did not deploy: %s' % chopper)
    bad = sorted(set(l for l in text.splitlines() if standins.BAD.search(l)))
    if bad:
        failures.append('skirmish: traps or faults: %s' % '; '.join(bad[:5]))

    text = run(a, 'mission', MISSION_ORDERS, MISSION_SCRIPT, '[Intro]\nPlay=no\n', a.seconds + 15)
    scenario = SCENARIO.search(text)
    deployed = DEPLOYED.findall(text)
    print('mission %s: Guardian GIs deployed at frames 300, 900: %s' % (
        scenario and scenario.group(1), ', '.join('%s of %s (%s deploying, %s dead)' % d for d in deployed)))
    if not scenario or scenario.group(1).upper() != 'ALL01UMD.MAP':
        failures.append('the first Allied mission did not start: %s' % (scenario and scenario.group(1)))
    elif not deployed:
        failures.append('the mission ran no deploy check (the game stopped early?)')
    else:
        done, total = int(deployed[0][0]), int(deployed[0][1])
        if total < 1 or done != total:
            failures.append('the mission\'s Guardian GIs did not deploy: %d of %d at frame 300 (%s deploying)'
                            % (done, total, deployed[0][2]))
    bad = sorted(set(l for l in text.splitlines() if standins.BAD.search(l) and not l.startswith('input-script')))
    if bad:
        failures.append('mission: traps or faults: %s' % '; '.join(bad[:5]))

    for f in failures:
        print('FAIL: ' + f)
    print('%s: soldiers and the Siege Chopper deploy on command, the mission\'s Guardian GIs deploy (%s)' % (
        'FAIL' if failures else 'OK', a.work))
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
