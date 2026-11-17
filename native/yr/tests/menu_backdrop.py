#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest one-game-menu-backdrop (lane Y11; built with -DRA2_NATIVE_COMBINED=ON): the shell's
Options and Enhancements pages draw their backdrop in both games of the one-game app.

User report 2026-11-17 (the Duo, Yuri's Revenge): Options drew plain black behind its controls and
Enhancements coloured noise; on the Mac both were noise. The cause was the dialogs' backdrop art
(a converter and two shapes) kept as truncated 32-bit ints (native/yr/fixups.json, lane Y11).

A played path per game (--game=yr, --game=ra2), with the menu-fill, vertical-sync and smooth-scaling
settings the user had: main menu -> Options -> Enhancements -> Cancel (back on Options) -> Main Menu,
a window dump after each step settles. Checks on the backdrop region (the left part below the
controls, where only the backdrop shows): on Options and Enhancements it is drawn (not black: some
of it lit) and it is the backdrop's dark-red art, not noise (lit pixels mostly red); the main menu's
region looks the same before and after. Then, at a 1024x768 game resolution, each game's shell
menu is shown as its original shows it (Yuri's Revenge's filling the screen, Red Alert 2's centred),
and both fill the window with Menu fill = Scale to fit (see main). No trap, no fault.

  menu_backdrop.py --binary <ra2yr-native> --data-dir <game folder> --game-exe <game.exe> --work <dir>
"""
import argparse
import os
import re
import shutil
import subprocess
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', 'tests'))
import game_process  # noqa: E402  (native/tests: the game's process group, lane C19)
from one_game_toggle import read_bmp  # noqa: E402  (the dumps' reader)

# 800x600 in both games: Options is the main menu's fifth button, Enhancements the Options page's
# third, Cancel and Main Menu the bottom button of their pages (right-hand column).
SCRIPT = ('s1+1500 dump\n'
          's1+2000 click 720 388\n'
          's2+1500 dump\n'
          's2+2000 click 720 304\n'
          's3+1500 dump\n'
          's3+2000 click 720 556\n'
          's4+1500 dump\n'
          's4+2000 click 720 556\n'
          's5+1500 dump\n')
STEPS = ['main menu', 'Options', 'Enhancements', 'Options again', 'main menu again']
INI = '[Video]\nScreenWidth=800\nScreenHeight=600\n'
REGION = (20, 400, 540, 520)        # x0, y0, x1, y1 in 800x600: below every control of both pages
ENHANCEMENTS = 'menufill=backdrop,vsync=on,scaling=smooth'


def region_stats(path):
    width, height, rows = read_bmp(path)
    sx, sy = width / 800.0, height / 600.0
    x0, y0, x1, y1 = REGION
    lit = red = 0
    total = 0
    for y in range(y0, y1, 2):
        for x in range(x0, x1, 2):
            r, g, b = rows[min(height - 1, int((y + 0.5) * sy))][min(width - 1, int((x + 0.5) * sx))]
            total += 1
            if max(r, g, b) >= 40:
                lit += 1
                if r > 1.4 * g and r > 1.4 * b:
                    red += 1
    return lit / max(1, total), red / max(1, lit)


def lit_box(path):
    """(left, top, right, bottom) of the dump's lit pixels as fractions of its size."""
    width, height, rows = read_bmp(path)
    xs, ys = [], []
    for y in range(0, height, 2):
        for x in range(0, width, 2):
            if max(rows[y][x]) > 24:
                xs.append(x)
                ys.append(y)
    if not xs:
        return None
    return (min(xs) / width, min(ys) / height, (max(xs) + 2) / width, (max(ys) + 2) / height)


def run(a, game, tag=None, ini=INI, script_text=SCRIPT, seconds=45, enhancements=ENHANCEMENTS):
    work = os.path.join(a.work, tag or game)
    os.makedirs(os.path.join(work, 'shots'))
    writes = os.path.join(work, 'w')
    os.makedirs(writes)
    with open(os.path.join(writes, 'RA2.INI'), 'w') as f:
        f.write(ini)
    with open(os.path.join(writes, 'RA2MD.INI'), 'w') as f:     # Yuri's Revenge: no start-up movie
        f.write('[Intro]\nPlay=no\n\n' + ini)
    script = os.path.join(work, 'e0.script')
    with open(script, 'w') as f:
        f.write(script_text)
    env = dict(os.environ, RA2_NATIVE_SETTINGS=os.path.join(work, 'settings'), RA2_GAME_EXE=a.game_exe,
               RA2_NATIVE_ENHANCEMENTS=enhancements, RA2_NATIVE_WINDOW_DUMP=os.path.join(work, 'shots', 'w'),
               RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_DIALOG_LOG='1', RA2_NATIVE_MOVIE_SKIP='1')
    env.pop('RA2_GAMEMD_EXE', None)
    env.pop('RA2_NATIVE_INPUT_SCRIPT', None)
    cmd = [a.binary, '--game=' + game, '--data-dir', a.data_dir, '--run-to=never', '--run-seconds=%d' % seconds,
           '--write-dir=' + writes, '--game-args=-noaudio', '--engine-scripts=' + script]
    with open(os.path.join(work, 'run.log'), 'w') as log:
        game_process.run(cmd, env=env, stdout=log, stderr=subprocess.STDOUT, timeout=165)
    return open(os.path.join(work, 'run.log'), errors='replace').read(), work


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--game-exe', default='')
    ap.add_argument('--work', required=True)
    a = ap.parse_args()
    if not a.data_dir or not os.path.exists(os.path.join(a.data_dir, 'gamemd.exe')):
        print('skipped: no Yuri\'s Revenge install (gamemd.exe) in --data-dir')
        return 77
    if not a.game_exe or not os.path.exists(a.game_exe):
        print('skipped: no game.exe (--game-exe)')
        return 77
    shutil.rmtree(a.work, ignore_errors=True)
    failures = []
    for game in ('yr', 'ra2'):
        text, work = run(a, game)
        stop = [l for l in text.splitlines() if 'stopped:' in l]
        if 'trap reached' in text or ' reached\n' in text and 'unrecovered' in text or re.search(r'stopped: (signal|trap|fault)', text):
            failures.append('%s: a trap or fault: %s' % (game, stop[-1] if stop else 'no stop line'))
        dumps = [int(n) for n in re.findall(r'^host: window dump (\d+)', text, re.M)]
        if len(dumps) < len(STEPS):
            failures.append('%s: %d window dumps, %d steps' % (game, len(dumps), len(STEPS)))
            continue
        stats = []
        for step, n in zip(STEPS, dumps):
            path = os.path.join(work, 'shots', 'w-e0.%d.bmp' % n)
            if not os.path.exists(path):
                path = os.path.join(work, 'shots', 'w.%d.bmp' % n)
            lit, red = region_stats(path)
            stats.append((lit, red))
            print('%s: %s: backdrop region %.0f%% lit, %.0f%% of that red (%s)' % (game, step, lit * 100, red * 100, os.path.basename(path)))
        for step, (lit, red) in zip(STEPS, stats):
            if step.startswith('main menu'):
                continue
            if lit < 0.03:
                failures.append('%s: %s: no backdrop (the region is black: %.1f%% lit)' % (game, step, lit * 100))
            elif red < 0.6:
                failures.append('%s: %s: the backdrop is noise (only %.0f%% of the lit region red)' % (game, step, red * 100))
        if abs(stats[0][0] - stats[4][0]) > 0.05:
            failures.append('%s: the main menu\'s region changed after Options (%.0f%% lit, then %.0f%%)'
                            % (game, stats[0][0] * 100, stats[4][0] * 100))
    # The menus' size at a larger game resolution (1024x768 here; the phone's play size in the app; lane
    # Y11b, user rule 2026-11-17: the original by default, filling as an enhancement). By default each
    # game as its original: Red Alert 2 runs its menus at the game resolution with the 800x600 menu
    # centred (game.exe 0x005115F0); Yuri's Revenge runs them in a separate 800x600 display mode,
    # GameOptionsClass::ShellWidth x ShellHeight (gamemd.exe WinMain 0x006BDB7E-0x006BDB90), so its menu
    # fills the screen. Menu fill = Scale to fit fills the window in both games. Measured: the menu's
    # rectangle in the window from the host's layout line, as fractions of the window.
    big = '[Video]\nScreenWidth=1024\nScreenHeight=768\nAllowHiResModes=yes\n'
    boxes = {}
    for fill in ('off', 'fit'):
        for game in ('yr', 'ra2'):
            text, work = run(a, game, '%s-1024-%s' % (game, fill), big, 's1+1500 dump\n', 25, 'menufill=' + fill)
            # The host's layout line for the main menu: the game's picture's place in the window, and the
            # 800x600 menu centred in it.
            lines = re.findall(r'^host: layout: menu (\d+)x(\d+) at the scale of \S+: picture (-?[\d.]+),(-?[\d.]+) ([\d.]+)x([\d.]+) in ([\d.]+)x([\d.]+)', text, re.M)
            if not lines:
                failures.append('%s 1024x768 menu fill %s: no layout line' % (game, fill))
                continue
            pw, ph, x, y, w, h, ww, wh = (float(v) for v in lines[-1])
            sx, sy = w / pw, h / ph
            mx, my = x + (pw - 800) / 2 * sx, y + (ph - 600) / 2 * sy
            boxes[(game, fill)] = (mx / ww, my / wh, (mx + 800 * sx) / ww, (my + 600 * sy) / wh)
            print('%s 1024x768 menu fill %s: the menu covers %s of the window' % (
                game, fill, ', '.join('%.2f' % v for v in boxes[(game, fill)])))
    def near(p, q, tol=0.03):
        return p is not None and q is not None and all(abs(x - y) <= tol for x, y in zip(p, q))
    off_yr, off_ra2 = boxes.get(('yr', 'off')), boxes.get(('ra2', 'off'))
    fit_yr, fit_ra2 = boxes.get(('yr', 'fit')), boxes.get(('ra2', 'fit'))
    if off_yr is None or max(off_yr[2] - off_yr[0], off_yr[3] - off_yr[1]) < 0.95:
        failures.append('yr: the original look (its own 800x600 shell mode) does not fill the window (%s)' % (off_yr,))
    if not near(off_ra2, (112 / 1024.0, 84 / 768.0, 912 / 1024.0, 684 / 768.0)):
        failures.append('ra2: the original look is not the 800x600 menu centred at 1024x768 (%s)' % (off_ra2,))
    for game, box in (('yr', fit_yr), ('ra2', fit_ra2)):
        if box is None or max(box[2] - box[0], box[3] - box[1]) < 0.95:
            failures.append('%s: Scale to fit does not fill the window (%s)' % (game, box))
    for f in failures:
        print('FAIL: ' + f)
    print('%s (%s)' % ('FAIL' if failures else 'OK', a.work))
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
