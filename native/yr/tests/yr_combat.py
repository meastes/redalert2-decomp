#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest yr-native-combat (lane Y12; built with -DRA2_NATIVE_YR=ON): forced combat in a Yuri's Revenge
skirmish on the Mac, so TechnoClass::Fire_At (glue/fire_at_native.inc) is proven to run: through the
real menus (Single Player, Skirmish, Start Game; The Alamo against one easy opponent) the harness
(glue/yr_harness.cpp, RA2_NATIVE_ORDERS "shoot *; watch") has the player's first vehicle attack a
second one of the same type, the order a force-fire click queues.

Checks: the order found two units; the target's strength drops below its start (damage dealt); the
target leaves the game ("harness-killed": a unit dies); no link-closure trap or fault. Skipped (77)
without the installed game (gamemd.exe in --data-dir).

  yr_combat.py --binary <yr-native> --data-dir <game folder> --work <dir> [--seconds 170]
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
    script = os.path.join(a.work, 'combat.script')
    with open(script, 'w') as f:
        f.write(SCRIPT)
    env = dict(os.environ, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_MSG_LOG='0',
               RA2_NATIVE_ORDERS='60 health; 80 shoot *; 80 watch')
    env.pop('RA2_NATIVE_FORCE_END', None)
    cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=never', '--run-seconds=%d' % a.seconds,
           '--write-dir=' + os.path.join(a.work, 'w'), '--input-script=' + script, '--game-args=-noaudio -CD.']
    with open(os.path.join(a.work, 'run.log'), 'w') as log:
        game_process.run(cmd, env=env, stdout=log, stderr=subprocess.STDOUT, timeout=a.seconds + 120)
    text = open(os.path.join(a.work, 'run.log'), errors='replace').read()
    failures = []
    shoot = re.search(r'^harness-shoot (\S+) &\S+ &\S+ s(\d+)$', text, re.M)
    if not shoot:
        failures.append('no shoot order (two units of one vehicle type not found)')
    else:
        start = int(shoot.group(2))
        seen = [int(s) for s in re.findall(r'^harness-watch s(-?\d+) ', text, re.M)]
        if not any(s < start for s in seen) and 'harness-killed' not in text:
            failures.append('no damage: the target stayed at %d' % start)
        if not re.search(r'^harness-killed ', text, re.M):
            failures.append('the target did not die (last strength %s)' % (seen[-1] if seen else '?'))
    bad = sorted(set(l for l in text.splitlines() if BAD.search(l)))
    if bad:
        failures.append('traps or faults: %s' % '; '.join(bad[:5]))
    for f in failures:
        print('FAIL: ' + f)
    killed = re.search(r'^harness-killed (\S+) at frame (\d+)', text, re.M)
    print('%s: %s%s (%s)' % ('FAIL' if failures else 'OK', 'a %s fired on another' % shoot.group(1) if shoot else 'no order',
                             ', killed at frame %s' % killed.group(2) if killed else '', a.work))
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
