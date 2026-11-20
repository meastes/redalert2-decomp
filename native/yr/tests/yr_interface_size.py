#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest yr-native-interface-size (lane N19; built with -DRA2_NATIVE_YR=ON): the Enhancements' Game
interface size in Yuri's Revenge (glue/yr_interface_size.cpp, Red Alert 2's link/native_enhancements.cpp
carried over), on the Mac through the real menus with scripted input, as the player sets it:

  main menu -> Options -> Enhancements -> Game interface size 1.5x -> Ok -> Main Menu -> Single Player
  -> Skirmish -> Start Game (a fresh settings folder: the app starts at 1x, so the new game in the same
  process makes its sidebar surface again: Start_Scenario's mode check, Yr_Sidebar_Remake); in play a
  window mapping check and a window dump, then Escape (the in-game menu) and a dump of it.

Checks: the run reaches the time limit without a trap or a fault; the new game made its sidebar surface
400 of the 600 rows ("enhancements: new game: sidebar surface 400 rows, was 600"); the host laid play out
split with the sidebar column 1.5x as wide (252 window points) and the window widened by it (884); the
mapping check passes; in play the sidebar column is drawn down to the window's bottom and the command bar
along the map's bottom (lit as in a run with the defaults); the in-game menu is shown whole (the window's
800x600 picture: its Resume Mission button at the column's bottom lit). A run with the defaults has no
split and keeps the sidebar surface at the screen's height. Skipped (77) without the installed game.

  yr_interface_size.py --binary <yr-native> --data-dir <game folder> --work <dir>
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

# Dialogs: 1 main menu, 2 Options, 3 the Enhancements page, 4 Options again, 5 the main menu, 6 Single
# Player, 7 the skirmish setup, 8 the in-game menu. The page's third row is Game interface size (its
# dropdown at 225,117 181x22; the game's dropdowns open on their rightmost pixels), its 1.5x entry three
# entries down; Ok is the column's top slot.
PAGE = """\
s1+1500 click 720 388
s2+2500 click 720 304
s3+2500 click 404 128
s3+3500 click 300 182
s3+5000 click 720 220
s4+1500 click 720 556
s5+1000 click 720 220
s6+1000 click 720 304
s7+2000 click 720 262
s7+3000 move 316 300
"""
MENUS = """\
s1+1000 click 720 220
s2+1000 click 720 304
s3+2000 click 720 262
s3+3000 move 316 300
"""
PLAY = """\
200 mapcheck
220 dump
250 key Escape
@MENU+1500 dump
"""
NEW_GAME = re.compile(r'^enhancements: new game: sidebar surface (\d+) rows, was (\d+)', re.M)
ROWS = re.compile(r'^enhancements: sidebar (\d+) of (\d+) screen rows', re.M)
LAYOUT = re.compile(r'^host: layout: map zoom [\d.]+, sidebar ([\d.]+),([\d.]+) ([\d.]+)x([\d.]+)$', re.M)
CHECK = re.compile(r'^host: mapcheck at map zoom [\d.]+: (\d+) pixels, largest error [\d.]+, (\d+) come back as another', re.M)


def run(a, name, script_text, menu_dialog):
    work = os.path.join(a.work, name)
    shutil.rmtree(work, ignore_errors=True)
    os.makedirs(os.path.join(work, 'w'))
    os.makedirs(os.path.join(work, 'shots'))
    script = os.path.join(work, 'ui.script')
    with open(script, 'w') as f:
        f.write(script_text + PLAY.replace('@MENU', 's%d' % menu_dialog))
    env = dict(os.environ, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_DIALOG_LOG='1',
               RA2_NATIVE_SETTINGS=os.path.join(work, 'settings'), RA2_NATIVE_WINDOW_DUMP=os.path.join(work, 'shots', 'w'))
    env.pop('RA2_NATIVE_ENHANCEMENTS', None)
    cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=never', '--run-seconds=%d' % a.seconds,
           '--write-dir=' + os.path.join(work, 'w'), '--input-script=' + script, '--game-args=-noaudio -CD.']
    with open(os.path.join(work, 'run.log'), 'w') as log:
        game_process.run(cmd, env=env, stdout=log, stderr=subprocess.STDOUT, timeout=a.seconds + 120)
    return open(os.path.join(work, 'run.log'), errors='replace').read(), work


def lit(rows, width, height, box):
    """The lit share of a box (x0, y0, x1, y1 in window pixels) of a dump."""
    x0, y0, x1, y1 = box
    on = total = 0
    for y in range(max(0, y0), min(height, y1), 3):
        for x in range(max(0, x0), min(width, x1), 3):
            total += 1
            if max(rows[y][x]) >= 40:
                on += 1
    return on / max(1, total)


def dumps(work, text):
    out = []
    for n in re.findall(r'^host: window dump (\d+)', text, re.M):
        path = os.path.join(work, 'shots', 'w.%s.bmp' % n)
        if os.path.exists(path):
            out.append(read_bmp(path))
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--work', required=True)
    ap.add_argument('--seconds', type=int, default=75)
    a = ap.parse_args()
    if not a.data_dir or not os.path.exists(os.path.join(a.data_dir, 'gamemd.exe')):
        print('skipped: no Yuri\'s Revenge install (gamemd.exe) in --data-dir')
        return 77
    failures = []
    shots = {}
    for name, script, menu in (('ui', PAGE, 8), ('plain', MENUS, 4)):
        text, work = run(a, name, script, menu)
        stop = [l for l in text.splitlines() if 'stopped:' in l]
        if not stop or 'time limit' not in stop[-1]:
            failures.append('%s: the run stopped early: %s' % (name, stop[-1] if stop else 'no stop line'))
        new_game = NEW_GAME.findall(text)
        rows = ROWS.findall(text)
        layouts = sorted(set(LAYOUT.findall(text)))
        checks = [(int(n), int(bad)) for n, bad in CHECK.findall(text)]
        shots[name] = dumps(work, text)
        print('%s: new game %s; sidebar rows %s; layouts %s; mapchecks %s; dumps %s; %s' % (
            name, new_game, rows[-2:], layouts, checks, [(w, h) for w, h, _ in shots[name]], stop[-1] if stop else 'no stop line'))
        if name == 'ui':
            if ('400', '600') not in new_game:
                failures.append('ui: the new game did not make its sidebar surface 400 of 600 rows (Game interface size 1.5x)')
            if not any(float(w) == 252 and float(h) == 600 for x, y, w, h in layouts):
                failures.append('ui: play was not laid out split with a 252-point sidebar column (%s)' % layouts)
            if not checks or any(n == 0 or bad for n, bad in checks):
                failures.append('ui: the window mapping check failed or did not run (%s)' % checks)
        else:
            if new_game or layouts or any(r != t for r, t in rows):
                failures.append('plain: a shorter sidebar or a split layout with the defaults')
    ui, plain = shots.get('ui') or [], shots.get('plain') or []
    if len(ui) < 2 or len(plain) < 2:
        failures.append('fewer than two window dumps in a run (ui %d, plain %d)' % (len(ui), len(plain)))
    else:
        (uw, uh, urows), (pw, ph, prows) = ui[-2], plain[-2]
        if uw != 884 or pw != 800:
            failures.append('the play window was %dx%d (%dx%d with the defaults), 884 wide expected' % (uw, uh, pw, ph))
        side_ui = lit(urows, uw, uh, (uw - 240, 380, uw - 12, uh - 4))     # the column's lower part, to the bottom
        side_plain = lit(prows, pw, ph, (pw - 160, 380, pw - 8, ph - 4))
        bar_ui = lit(urows, uw, uh, (10, uh - 28, 600, uh - 4))            # the command bar under the map
        bar_plain = lit(prows, pw, ph, (10, ph - 28, 600, ph - 4))
        print('lit: sidebar column %.2f (defaults %.2f), command bar %.2f (defaults %.2f)' % (side_ui, side_plain, bar_ui, bar_plain))
        if side_ui < 0.5 * side_plain:
            failures.append('the sidebar column is not drawn to the window\'s bottom in play')
        if bar_ui < 0.5 * bar_plain:
            failures.append('the command bar is not drawn along the map\'s bottom in play')
        mw, mh, mrows = ui[-1]
        resume = lit(mrows, mw, mh, (42 + 660, 505, 42 + 790, 525))       # the picture centred in the window
        print('lit: the in-game menu\'s Resume Mission button %.2f (%dx%d)' % (resume, mw, mh))
        if resume < 0.3:
            failures.append('the in-game menu was not shown whole (its Resume Mission button not at the column\'s bottom)')
    for f in failures:
        print('FAIL: ' + f)
    print('%s (%s)' % ('FAIL' if failures else 'OK', a.work))
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
