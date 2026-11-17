#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""Campaign chain (ctest native-campaign-allied / native-campaign-soviet, lane C16): a new
campaign started through the menus (Single Player, New Campaign, the side's emblem) is won
twelve times by RA2_NATIVE_FORCE_END (one entry every 200 loop frames), each mission's score
screen is left by its Continue, so every mission of the side starts and ends, the last one
with the campaign's ending, back at the main menu. Fails unless all twelve missions are won
in order (force-end logs the scenario each entry ended), the main menu comes back after the
last, and no trap, fault or unrecovered call is logged.

  campaign_chain.py --side allied|soviet --binary <ra2-native> --data-dir <installed RA2>
                    --game-exe <game.exe> [--log <file>] [--wine-prefix <dir> [--wine <wine>]]

Without the installed game (RA2_NATIVE_CENSUS_DATA_DIR / RA2_NATIVE_CENSUS_GAME_EXE), or a
Windows build's run without Wine, it exits 77 (ctest: skipped). Writes go to a fresh
temporary folder."""
import argparse
import os
import re
import subprocess
import sys
import tempfile

import game_process
import wine as winehost

SKIP = 77
MISSIONS = 12
BAD = re.compile(r'unrecovered|trap|signal \d+|SIG(SEGV|BUS|ILL|ABRT|FPE)|crash:|abort', re.I)
# Game pixels at 1024x768: Single Player and New Campaign in the menus' right column, the
# side's emblem in the campaign dialog, the score screen's Continue.
SINGLE_PLAYER = (829, 302)
NEW_CAMPAIGN = (831, 305)
EMBLEM = {'allied': (433, 179), 'soviet': (429, 448)}
CONTINUE = (831, 640)
PREFIX = {'allied': 'ALL', 'soviet': 'SOV'}


def script(side):
    """Dialogs 1-3 are the menus; each score screen follows (4 .. 15); 16 is the main menu."""
    lines = ['s1+1500 click %d %d' % SINGLE_PLAYER, 's2+1500 click %d %d' % NEW_CAMPAIGN,
             's3+2000 click %d %d' % EMBLEM[side]]
    for k in range(MISSIONS):
        lines.append('d%d+3000 click %d %d' % (4 + k, CONTINUE[0], CONTINUE[1]))
    lines.append('d%d+3000 quit' % (4 + MISSIONS))
    return lines


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--side', required=True, choices=sorted(EMBLEM))
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default=os.environ.get('RA2_NATIVE_CENSUS_DATA_DIR', ''))
    ap.add_argument('--game-exe', default=os.environ.get('RA2_NATIVE_CENSUS_GAME_EXE', ''))
    ap.add_argument('--seconds', type=int, default=1200)
    ap.add_argument('--log', default='')
    ap.add_argument('--wine-prefix', default='', help='run the Windows .exe under Wine in this fresh prefix')
    ap.add_argument('--wine', default='')
    a = ap.parse_args()
    name = 'native-campaign-' + a.side
    if not a.data_dir or not os.path.isdir(a.data_dir) or not a.game_exe or not os.path.isfile(a.game_exe):
        print('%s: skipped (set RA2_NATIVE_CENSUS_DATA_DIR and RA2_NATIVE_CENSUS_GAME_EXE)' % name)
        return SKIP
    prefix = None
    if a.wine_prefix:
        wine = winehost.find_wine(a.wine)
        if not wine:
            print('%s: skipped (Windows build: no Wine; set RA2_NATIVE_WINE)' % name)
            return SKIP
        prefix = winehost.Prefix(wine, a.wine_prefix)
    ends = ','.join(str(100 + 200 * k) for k in range(MISSIONS))
    env = dict(os.environ, RA2_GAME_EXE=a.game_exe, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_CRASH_TRACE='1',
               RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_DIALOG_LOG='1', RA2_NATIVE_FORCE_END=ends)
    env.pop('RA2_NATIVE_TRAP_CONTINUE', None)   # a trap must end the run
    with tempfile.TemporaryDirectory(prefix='ra2-campaign-') as work:
        write_dir = os.path.join(work, 'write')
        os.makedirs(write_dir)
        path = os.path.join(work, 'chain.script')
        with open(path, 'w') as f:
            f.write('\n'.join(script(a.side)) + '\n')
        cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=seconds', '--run-seconds=%d' % a.seconds,
               '--write-dir=' + write_dir, '--game-args=-noaudio -CD.', '--input-script=' + path]
        if prefix:
            env = prefix.env(env)
        try:
            for attempt in range(winehost.HOST_RETRIES + 1):
                argv = prefix.command(cmd) if prefix else cmd
                try:
                    p = game_process.run(argv, env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True,
                                         errors='replace', timeout=a.seconds + 60)
                    out, code, timed_out = p.stdout, p.returncode, False
                except subprocess.TimeoutExpired as e:
                    out = e.stdout.decode('utf-8', 'replace') if isinstance(e.stdout, bytes) else (e.stdout or '')
                    code, timed_out = -1, True
                why = winehost.host_failure(out, timed_out, code) if prefix else ''
                if not why or attempt == winehost.HOST_RETRIES:
                    break
                print('%s: %s; launching again' % (name, why))
        finally:
            if prefix:
                prefix.close()
    if a.log:
        with open(a.log, 'w') as f:
            f.write(out)
    lines = out.splitlines()
    bad = [l for l in lines if BAD.search(l) and not l.startswith('input-script')]
    won = [m.group(1) for m in (re.match(r'force-end: win at frame \d+ in (\S+)', l) for l in lines) if m]
    numbers = []
    for scenario in won:
        m = re.match(PREFIX[a.side] + r'(\d\d)', scenario, re.I)
        numbers.append(int(m.group(1)) if m else -1)
    shown = [int(m.group(1)) for m in (re.match(r'dialog shown #(\d+)', l) for l in lines) if m]
    back = any(l.startswith('run-to: reached menu-return') for l in lines) and max(shown or [0]) >= 4 + MISSIONS
    print('%s: missions won %s' % (name, ' '.join(won) or 'none'))
    problems = []
    if numbers != list(range(1, MISSIONS + 1)):
        problems.append('missions won %s, not %s01-%02d in order' % (numbers, PREFIX[a.side], MISSIONS))
    if not back:
        problems.append('the main menu did not come back after the last mission (dialogs shown: %d)' % len(shown))
    if bad:
        problems.append('%d bad lines' % len(bad))
    if problems:
        print('%s: FAILED (exit %d): %s' % (name, code, '; '.join(problems)))
        for l in bad[:20]:
            print('  ' + l)
        print('\n'.join([l for l in lines if l.startswith('run-to:')][-6:]))
        return 1
    print('%s: %d missions won, the ending and the main menu: passed' % (name, MISSIONS))
    return 0


if __name__ == '__main__':
    sys.exit(main())
