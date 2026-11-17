#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest yr-native-campaign-allied / -soviet (lane Y12; built with -DRA2_NATIVE_YR=ON): Yuri's Revenge's
campaign chain on the Mac, as RA2's native-campaign-allied/-soviet (tests/campaign_chain.py): a new
campaign started through the menus (Single Player, New Campaign, the side's emblem) is won seven times
by RA2_NATIVE_FORCE_END (glue/yr_harness.cpp; one entry every 200 loop frames), each mission's score
screen is left by its Continue, so every mission of the side starts and ends, the last one with the
campaign's final movie and the credits (Show_Who_Was_Responsible, run to their end), back at the main
menu.

Checks: all seven missions won in order (force-end logs the scenario each entry ended); the credits
ran; the main menu came back; no link-closure trap or fault. Skipped (77) without the installed game
(gamemd.exe in --data-dir).

  yr_campaign_chain.py --side allied|soviet --binary <yr-native> --data-dir <game folder> --work <dir>
"""
import argparse
import os
import re
import shutil
import subprocess
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', 'tests'))
import game_process  # noqa: E402  (native/tests: the game's process group, lane C19)

MISSIONS = 7
BAD = re.compile(r'unrecovered|SIG(SEGV|BUS|ILL|ABRT|FPE)|crash:', re.I)
# 800x600 shell: Single Player and New Campaign in the right column, the side's emblem in the
# campaign dialog, the score screen's Continue.
SINGLE_PLAYER = (720, 220)
NEW_CAMPAIGN = (720, 220)
EMBLEM = {'allied': (315, 125), 'soviet': (315, 430)}
CONTINUE = (720, 560)


def script(side):
    lines = ['s1+1500 click %d %d' % SINGLE_PLAYER, 's2+1500 click %d %d' % NEW_CAMPAIGN,
             's3+2500 click %d %d' % EMBLEM[side]]
    for k in range(MISSIONS):
        lines.append('d%d+3000 click %d %d' % (4 + k, CONTINUE[0], CONTINUE[1]))
    return lines


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--side', required=True, choices=sorted(EMBLEM))
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--work', required=True)
    ap.add_argument('--seconds', type=int, default=900)
    ap.add_argument('--dump-ms', type=int, default=0, help='window dumps every N ms (research)')
    a = ap.parse_args()
    if not a.data_dir or not os.path.exists(os.path.join(a.data_dir, 'gamemd.exe')):
        print('skipped: no Yuri\'s Revenge install (gamemd.exe) in --data-dir')
        return 77
    shutil.rmtree(a.work, ignore_errors=True)
    os.makedirs(os.path.join(a.work, 'w'))
    with open(os.path.join(a.work, 'w', 'RA2MD.INI'), 'w') as f:
        f.write('[Intro]\nPlay=no\n')
    path = os.path.join(a.work, 'chain.script')
    with open(path, 'w') as f:
        f.write('\n'.join(script(a.side)) + '\n')
    ends = ','.join(str(100 + 200 * k) for k in range(MISSIONS))
    env = dict(os.environ, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_DIALOG_LOG='1',
               RA2_NATIVE_MSG_LOG='0', RA2_NATIVE_FORCE_END=ends)
    env.pop('RA2_NATIVE_TRAP_CONTINUE', None)
    env.pop('RA2_NATIVE_ORDERS', None)
    if a.dump_ms:
        os.makedirs(os.path.join(a.work, 'shots'))
        env.update(RA2_NATIVE_WINDOW_DUMP=os.path.join(a.work, 'shots', 'win'), RA2_NATIVE_WINDOW_DUMP_MS=str(a.dump_ms))
    cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=menu-return', '--run-seconds=%d' % a.seconds,
           '--write-dir=' + os.path.join(a.work, 'w'), '--input-script=' + path, '--game-args=-noaudio -CD.']
    with open(os.path.join(a.work, 'run.log'), 'w') as log:
        try:
            game_process.run(cmd, env=env, stdout=log, stderr=subprocess.STDOUT, timeout=a.seconds + 120)
        except subprocess.TimeoutExpired:
            pass
    lines = open(os.path.join(a.work, 'run.log'), errors='replace').read().splitlines()
    won = [m.group(1) for m in (re.match(r'force-end: win at frame \d+ in (\S+)', l) for l in lines) if m]
    print('yr-native-campaign-%s: missions won %s' % (a.side, ' '.join(won) or 'none'))
    problems = []
    if len(won) != MISSIONS or len(set(w.lower() for w in won)) != MISSIONS:
        problems.append('%d distinct missions won, not %d' % (len(set(w.lower() for w in won)), MISSIONS))
    if not any('credits:' in l for l in lines):
        problems.append('the credits did not run')
    if not any(l.startswith('run-to: reached menu-return') for l in lines):
        problems.append('the main menu did not come back: %s' % ([l for l in lines if l.startswith('run-to:')][-1:] or '?'))
    bad = sorted(set(l for l in lines if BAD.search(l) and not l.startswith('input-script')))
    if bad:
        problems.append('traps or faults: %s' % '; '.join(bad[:5]))
    if problems:
        print('FAIL: ' + '; '.join(problems) + ' (%s)' % a.work)
        return 1
    print('OK: %d missions won, the ending, the credits and the main menu (%s)' % (MISSIONS, a.work))
    return 0


if __name__ == '__main__':
    sys.exit(main())
