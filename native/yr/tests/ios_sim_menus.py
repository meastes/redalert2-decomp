#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""The one-game iPhone app's shell and a short game in each game, side by side (lane Y11, the YR iOS
polish pass): on the "RA2 Duo Validation" Simulator, one start per game (--game=yr, --game=ra2) with
the same settings: main menu -> Options -> Enhancements -> Cancel -> Main Menu -> Single Player ->
Skirmish -> Start Game -> the loading screen -> a short play with taps (select, move). Window dumps
(the game's own pictures; simctl's screenshots of the Metal layer are black) every second go to
--work/<game>/shots; the touch controls' and dialogs' lines are in each run.log. Checks: no trap or
fault in either game; on Options and Enhancements the backdrop region is drawn and is not noise
(menu_backdrop.py's measure); the touch Skip button is not left showing on the start-up loading screen
(lane Y14, the user's report: Yuri's Revenge keeps its intro movie open through the load, so the button
stayed until the main menu: in Yuri's Revenge a "movie" touch-controls state must give way before the
first "menu"; Red Alert 2 loads before its intro and goes from the movie straight to its menu); in
Yuri's Revenge the command bar is laid out under the tactical view and drawn on the screen (lane Y14, the
user's "missing bottom bar"; the harness's `rects` at frame 600). Lane N3 (the user's Duo reports of 2026-11-19): in both
games a tap on the player name field (control 1696) raises the on-screen keyboard and a tap away puts it
away; in Yuri's Revenge a tap on the command bar's TypeSelect button with one tank selected selects every
tank (it had crashed: an unrecovered function). Not a ctest (it needs the Simulator). Only "RA2 Duo Validation" is
used; booted when shut down and shut down again afterwards.

  ios_sim_menus.py --app build/ios-one-simulator/RA2YR.app --data-dir <RA2 + YR folder> --work <dir>
                   [--games yr,ra2] [--play-seconds 120] [--enhancements menufill=backdrop,...]
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
    # the player name field, control 1696 in both games (the on-screen keyboard; lane N3: by its id, as
    # Red Alert 2's field is further right than Yuri's Revenge's)
    lines += ['s7+1200 tdown @ctl=1696', 's7+1280 tup @ctl=1696']
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
    ap.add_argument('--play-seconds', type=int, default=120)
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
            lines = script(a.play_w)
            if game == 'yr':
                # Lane N3 (the user's Duo report: the command bar's TypeSelect button crashed the game): a
                # tank selected, the button tapped (@cmd=TypeSelect, glue/yr_app_hooks.cpp), every tank
                # selected after.
                env['RA2_NATIVE_ORDERS'] = '600 rects; 640 select MTNK 0; 700 groups MTNK'
                lines += ['660 tdown @cmd=TypeSelect', '662 tup @cmd=TypeSelect']
            text = one.launch(a, game, ['--game=' + game], [lines], a.play_seconds, env)
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
            states = re.findall(r'^host: touch controls: (\w+)', text, re.M)
            # Red Alert 2 loads before its intro and goes from the movie straight to the menu; Yuri's
            # Revenge loads after its intro, behind its loading screen.
            if game == 'yr' and 'menu' in states and 'movie' in states[:states.index('menu')]:
                before = states[:states.index('menu')]
                if before[-1] == 'movie':
                    failures.append('%s: the Skip button stayed on until the main menu (start-up loading screen)' % game)
            if game == 'yr':
                rects = re.search(r'^harness-rects tactical (-?\d+),(-?\d+) (\d+)x(\d+) visible (\d+)x(\d+) commandbar (\d+),(\d+) (\d+)x(\d+)', text, re.M)
                shown = re.search(r'^harness-rects commandbar on screen (-?\d+)% drawn', text, re.M)
                if not rects or not shown:
                    failures.append('yr: no command bar report (harness rects at frame 600)')
                else:
                    ty, th, vh, by, bh = (int(rects.group(i)) for i in (2, 4, 6, 8, 10))
                    print('yr: %s; %s' % (rects.group(0), shown.group(0)))
                    if bh != 32 or by != vh - 32 or ty + th != by:
                        failures.append('yr: the command bar is not under the tactical view')
                    if int(shown.group(1)) < 10:
                        failures.append('yr: the command bar is not drawn (%s%%)' % shown.group(1))
            # Lane N3 (the user's Duo report: no keyboard in Yuri's Revenge's skirmish name field): the tap
            # on the player name field raises the on-screen keyboard, the tap away puts it away.
            keys = re.findall(r'^host: on-screen keyboard (shown|hidden)', text, re.M)
            if keys[:2] != ['shown', 'hidden']:
                failures.append('%s: the player name field did not raise the keyboard (%s)' % (game, keys))
            if game == 'yr':
                groups = re.search(r'^harness-groups MTNK((?: -?\d+\*?)+)$', text, re.M)
                print('yr: after the TypeSelect tap: %s' % (groups.group(0) if groups else None))
                if not groups or not all(g.endswith('*') for g in groups.group(1).split()):
                    failures.append('yr: the TypeSelect button did not select every tank')
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
