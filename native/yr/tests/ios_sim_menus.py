#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""The one-game iPhone app's shell and a short game in each game, side by side (lane Y11, the YR iOS
polish pass): on the "RA2 Duo Validation" Simulator, one start per game (--game=yr, --game=ra2) with
the same settings: main menu -> Options -> Enhancements -> Cancel -> Main Menu -> Single Player ->
Skirmish -> Start Game -> the loading screen -> a short play with taps (select, move). Window dumps
(the game's own pictures; simctl's screenshots of the Metal layer are black) every second go to
--work/<game>/shots; the touch controls' and dialogs' lines are in each run.log. Checks: no trap or
fault in either game; on Options and Enhancements the backdrop region is drawn and is not noise
(menu_backdrop.py's measure). Not a ctest (it needs the Simulator). Only "RA2 Duo Validation" is
used; booted when shut down and shut down again afterwards.

  ios_sim_menus.py --app build/ios-one-simulator/RA2YR.app --data-dir <RA2 + YR folder> --work <dir>
                   [--games yr,ra2] [--play-seconds 40] [--enhancements menufill=backdrop,...]
"""
import argparse
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import ios_sim_one_game as one  # noqa: E402  (launch, simctl, device, the Simulator's names)
from menu_backdrop import read_bmp  # noqa: E402

REGION = (20, 400, 540, 520)      # the backdrop region in the 800x600 menu (menu_backdrop.py)


def tap(when, x, y):
    base, ms = when.split('+')
    return ['%s tdown %d %d' % (when, x, y), '%s+%d tup %d %d' % (base, int(ms) + 80, x, y)]


def script(play_w):
    mx = (play_w - 800) // 2
    lines = []
    lines += ['s1+1500 dump']
    lines += tap('s1+2000', mx + 720, 388)        # Options
    lines += ['s2+1500 dump']
    lines += tap('s2+2000', mx + 720, 304)        # Enhancements
    lines += ['s3+1500 dump']
    lines += tap('s3+2000', mx + 720, 556)        # Cancel (back on Options)
    lines += ['s4+1500 dump']
    lines += tap('s4+2000', mx + 720, 556)        # Main Menu
    lines += ['s5+1500 dump']
    lines += tap('s5+2000', mx + 720, 220)        # Single Player
    lines += tap('s6+1500', mx + 720, 304)        # Skirmish
    lines += ['s7+1000 dump']
    lines += tap('s7+1200', mx + 135, 68)         # the player name field (the on-screen keyboard)
    lines += ['s7+1700 dump']
    lines += tap('s7+2100', mx + 600, 300)        # away from it (the keyboard goes)
    lines += tap('s7+2500', mx + 720, 262)        # Start Game
    cx, cy = (play_w - 168) // 2, 300
    for t in range(500, 12000, 500):              # the loading screen and the first seconds of play
        lines += ['s7+%d dump' % (2500 + t)]
    # A tap on the start units, then on open ground (select, move), dumps between.
    lines += tap('s7+30000', cx, cy)
    lines += ['s7+30300 dump']
    lines += tap('s7+31000', cx - 120, cy - 80)
    for t in range(31200, 34000, 200):
        lines += ['s7+%d dump' % t]
    return lines


def region(path):
    width, height, rows = read_bmp(path)
    sx, sy = width / 800.0, height / 600.0
    x0, y0, x1, y1 = REGION
    lit = red = total = 0
    for y in range(y0, y1, 2):
        for x in range(x0, x1, 2):
            px = int((x + 0.5) * sx + (width - 800 * sx) / 2) if width / height > 800 / 600 else int((x + 0.5) * sx)
            r, g, b = rows[min(height - 1, int((y + 0.5) * sy))][min(width - 1, px)]
            total += 1
            if max(r, g, b) >= 40:
                lit += 1
                red += r > 1.4 * g and r > 1.4 * b
    return lit / max(1, total), red / max(1, lit)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--app', required=True)
    ap.add_argument('--data-dir', required=True)
    ap.add_argument('--work', required=True)
    ap.add_argument('--udid', default=one.VALIDATION)
    ap.add_argument('--games', default='yr,ra2')
    ap.add_argument('--play-seconds', type=int, default=60)
    ap.add_argument('--play-w', type=int, default=872)
    ap.add_argument('--shot-seconds', type=float, default=20)
    ap.add_argument('--enhancements', default='menufill=backdrop,vsync=on,scaling=smooth')
    ap.add_argument('--keep-booted', action='store_true')
    ap.add_argument('--movies', action='store_true', help='play the movies (the loading screen as a player sees it)')
    a = ap.parse_args()
    a.data_dir = os.path.abspath(a.data_dir)
    a.work = os.path.abspath(a.work)
    d = one.device(a.udid)
    if not d or d['name'] != one.VALIDATION_NAME:
        print('refused: %s is not the "%s" Simulator' % (a.udid, one.VALIDATION_NAME))
        return 2
    os.makedirs(a.work, exist_ok=True)
    booted_here = d['state'] != 'Booted'
    if booted_here:
        one.simctl('boot', a.udid)
        one.simctl('bootstatus', a.udid, '-b')
    failures = []
    try:
        one.simctl('install', a.udid, a.app)
        for game in a.games.split(','):
            shots = os.path.join(a.work, game + '-dumps')
            os.makedirs(shots, exist_ok=True)
            env = {'RA2_NATIVE_ENHANCEMENTS': a.enhancements, 'RA2_NATIVE_WINDOW_DUMP': os.path.join(shots, 'w'),
                   'RA2_NATIVE_MOVIE_SKIP': '0' if a.movies else '1'}
            text = one.launch(a, game, ['--game=' + game], [script(a.play_w)], a.play_seconds, env)
            if 'unrecovered ' in text or 'crash: signal' in text:
                failures.append('%s: a trap or fault: %s' % (game, [l for l in text.splitlines() if 'unrecovered' in l or 'crash:' in l][:2]))
            dumps = [int(n) for n in re.findall(r'^host: window dump (\d+)', text, re.M)]
            names = ['main menu', 'Options', 'Enhancements', 'Options again', 'main menu again']
            for step, n in zip(names, dumps):
                path = os.path.join(shots, 'w-e0.%d.bmp' % n)
                if not os.path.exists(path):
                    path = os.path.join(shots, 'w.%d.bmp' % n)
                if not os.path.exists(path):
                    failures.append('%s: no dump %d' % (game, n))
                    continue
                lit, red = region(path)
                print('%s: %s: backdrop region %.0f%% lit, %.0f%% of that red' % (game, step, lit * 100, red * 100))
                if step in ('Options', 'Enhancements', 'Options again'):
                    if lit < 0.03:
                        failures.append('%s: %s: no backdrop (black)' % (game, step))
                    elif red < 0.6:
                        failures.append('%s: %s: the backdrop is noise' % (game, step))
            for l in text.splitlines():
                if l.startswith('host: touch controls:') or l.startswith('host: on-screen keyboard'):
                    print('%s: %s' % (game, l))
    finally:
        if booted_here and not a.keep_booted:
            one.simctl('shutdown', a.udid, check=False)
    for f in failures:
        print('FAIL: ' + f)
    print('%s (%s)' % ('FAIL' if failures else 'OK', a.work))
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
