#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest native-menu-movie (lane C11): a movie from the Movies menu at the iPhone Duo's width,
and the main menu after a submenu, as the user saw them on the device (2026-11-13).

The app (--app, a fresh settings folder) runs at 896x634, the Duo's 944x634 less the width a
1.25x interface gives back (RA2.INI [Video] written before the start, as the touch app does),
with Game interface size 1.25x and Map zoom 1.25x (the user's settings). Through the real
menus: Movies & Credits, Play Movies, the first entry (Intro Movie), Play Movie; while it
plays the window loses and regains the focus (a scripted blur and focus: on the Duo the
glass toolbar, a notification or the Skip button's scene do it); Escape; Back; Main Menu.
The window is dumped as drawn every 500 ms (RA2_NATIVE_WINDOW_DUMP).

Checks:
  - the movie is shown whole: in each dumped frame of the movie (the picture's margins
    black, the content at least 400 rows tall), the content's top and bottom rows are the same
    in every column. The split layout (the sidebar column shown larger) used to show the
    movie's right part lower down and larger, in the sidebar column;
  - the main menu's buttons are drawn when it is shown again after the submenu: its button
    column holds about as many lit-button (red) pixels as when the menu was first shown.
    A focus change in a submenu left the main menu without its buttons (the game's
    redraw after it copied the hidden surface over them).
Skipped (77) without the installed game.

  menu_movie.py --binary <ra2-native> --data-dir <game folder> --game-exe <game.exe> --work <dir>
  menu_movie.py --check-dumps <dir>        (the checks only, over a run's dumps and run.log)
"""
import argparse
import glob
import os
import shutil
import struct
import subprocess
import sys

WIDTH, HEIGHT = 896, 634
# The shell's controls at 896x634 (RA2_NATIVE_MSG_LOG): Movies & Credits, Play Movies, the
# list's first row, Play Movie, Back / Main Menu (the column's bottom slot).
SCRIPT = """d1+2500 click 770 363
d2+2500 click 770 279
d3+2000 click 236 168
d3+3000 click 770 237
d3+6000 blur
d3+6400 focus
d3+9000 key Escape
d3+12000 click 770 573
d3+16000 click 770 573
d3+22000 quit
"""


def load(path):
    """(width, height, rows of (r, g, b)) of an uncompressed BMP (SDL_SaveBMP)."""
    b = open(path, 'rb').read()
    off = struct.unpack_from('<I', b, 10)[0]
    w, h = struct.unpack_from('<ii', b, 18)
    bpp = struct.unpack_from('<H', b, 28)[0] // 8
    stride = (w * bpp + 3) & ~3
    rows = []
    for y in range(abs(h)):
        r = b[off + y * stride: off + y * stride + w * bpp]
        # SDL writes BGR(A).
        rows.append([(r[x * bpp + 2], r[x * bpp + 1], r[x * bpp]) for x in range(w)])
    if h > 0:
        rows.reverse()
    return w, abs(h), rows


def lit(p):
    return sum(p) > 60


def movie_frame(w, h, rows):
    """The content's bounding box when the frame looks like a movie (a black margin all round)."""
    xs, ys = [], []
    for y in range(0, h, 2):
        row = rows[y]
        for x in range(0, w, 2):
            if lit(row[x]):
                xs.append(x)
                ys.append(y)
    if not xs:
        return None
    x0, x1, y0, y1 = min(xs), max(xs), min(ys), max(ys)
    # (The bottom margin is not required: the split layout drew the movie's right part down
    # to the window's bottom edge.)
    if x0 < 8 or y0 < 8 or x1 > w - 9 or y1 - y0 < 400 * h // HEIGHT * 0.9:
        return None
    return x0, y0, x1, y1


def column_extents(rows, x, y0, y1):
    top = bottom = None
    for y in range(y0, y1 + 1):
        if lit(rows[y][x]):
            if top is None:
                top = y
            bottom = y
    return top, bottom


def check_movie(dumps):
    """Each movie frame: nothing drawn above or below the rows most columns of the movie span."""
    bad, seen = [], 0
    for path in dumps:
        w, h, rows = load(path)
        box = movie_frame(w, h, rows)
        if not box:
            continue
        seen += 1
        x0, y0, x1, y1 = box
        tops, bottoms = [], []
        for x in range(x0, x1 + 1, 2):
            t, b = column_extents(rows, x, y0, y1)
            if t is not None:
                tops.append(t)
                bottoms.append(b)
        tops.sort()
        bottoms.sort()
        # The movie's own rows: where most columns start and end (a dark scene's columns may
        # start lower or end higher, never outside them).
        mt, mb = tops[len(tops) // 2], bottoms[len(bottoms) * 3 // 4]
        outside = sum(1 for y in range(0, h, 2) if y < mt - 6 or y > mb + 6
                      for x in range(x0, x1 + 1, 2) if lit(rows[y][x]))
        if outside > 50:
            bad.append('%s: the movie is not whole: %d pixels drawn outside its rows %d-%d (the split layout?)'
                       % (os.path.basename(path), outside * 4, mt, mb))
    if seen == 0:
        bad.append('no frame of the movie was dumped')
    return bad, seen


def red_pixels(rows, w, h):
    """Lit-button pixels (the buttons' red) in the menu's button column, in window pixels."""
    sx, sy = w / float(WIDTH), h / float(HEIGHT)
    # The picture is centred in the window at its height's scale.
    scale = sy
    left = (w - WIDTH * scale) / 2
    count = 0
    for gy in range(216, 426, 2):
        for gx in range(692, 848, 2):
            r, g, b = rows[int(gy * scale)][int(left + gx * scale)]
            if r > 140 and g < 70 and b < 70:
                count += 1
    return count


def check_menu(dumps, log):
    """The first main-menu frame (before the first click) against the last one (after Main Menu)."""
    bad = []
    if len(dumps) < 6:
        return ['%d frames dumped' % len(dumps)], 0, 0
    first = None
    for path in dumps[:len(dumps) // 2]:
        w, h, rows = load(path)
        n = red_pixels(rows, w, h)
        if n > (first or 0):
            first = n
    w, h, rows = load(dumps[-1])
    last = red_pixels(rows, w, h)
    if not first:
        bad.append('the main menu never showed its buttons at the start')
    elif last < first // 2:
        bad.append('the main menu shown again has %d lit-button pixels, %d when first shown: its buttons are missing'
                   % (last, first))
    return bad, first or 0, last


def dumps_of(work):
    return sorted(glob.glob(os.path.join(work, 'w.*.bmp')), key=lambda s: int(s.rsplit('.', 2)[1]))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', default='')
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--game-exe', default='')
    ap.add_argument('--work', default='')
    ap.add_argument('--check-dumps', default='', help='only check the dumps of an earlier run in this folder')
    a = ap.parse_args()
    if a.check_dumps:
        work = a.check_dumps
    else:
        if not a.data_dir or not os.path.isdir(a.data_dir) or not a.game_exe or not os.path.isfile(a.game_exe):
            print('menu-movie: no installed game given; skipped')
            return 77
        work = os.path.abspath(a.work)
        shutil.rmtree(work, ignore_errors=True)
        game = os.path.join(work, 'home', 'Library', 'Application Support', 'RA2Native', 'Game')
        os.makedirs(game)
        with open(os.path.join(game, 'RA2.INI'), 'w') as f:
            f.write('[Intro]\nPlay=no\n\n[Video]\nScreenWidth=%d\nScreenHeight=%d\nAllowHiResModes=yes\nStretchMovies=no\n'
                    % (WIDTH, HEIGHT))
        script = os.path.join(work, 'movie.script')
        with open(script, 'w') as f:
            f.write(SCRIPT)
        env = dict(os.environ)
        for k in ('RA2_NATIVE_ENHANCEMENTS', 'RA2_NATIVE_SETUP', 'RA2_DATA_DIR', 'RA2_NATIVE_MOVIE_SKIP'):
            env.pop(k, None)
        env.update({'HOME': os.path.join(work, 'home'), 'RA2_GAME_EXE': a.game_exe, 'RA2_NATIVE_LOG_STDERR': '1',
                    'RA2_NATIVE_IGNORE_FOCUS': '1', 'RA2_NATIVE_ENHANCEMENTS': 'uisize=1.25,mapzoom=1.25',
                    'RA2_NATIVE_WINDOW_DUMP': os.path.join(work, 'w'), 'RA2_NATIVE_WINDOW_DUMP_MS': '500'})
        cmd = [a.binary, '--app', '--data-dir', a.data_dir, '--input-script=' + script, '--game-args=-noaudio']
        try:
            p = subprocess.run(cmd, env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, errors='replace',
                               timeout=240)
            out, code = p.stdout, p.returncode
        except subprocess.TimeoutExpired as e:
            out, code = (e.stdout or b'').decode('utf-8', 'replace') if isinstance(e.stdout, bytes) else (e.stdout or ''), -1
        with open(os.path.join(work, 'run.log'), 'w') as f:
            f.write(out)
        if code != 0:
            print('menu-movie: exit %d (see %s)' % (code, os.path.join(work, 'run.log')))
            return 1
    log = open(os.path.join(work, 'run.log'), errors='replace').read() if os.path.exists(os.path.join(work, 'run.log')) else ''
    dumps = dumps_of(work)
    bad = []
    if 'interface 1.25x, map zoom 1.25x' not in log and not a.check_dumps:
        bad.append('the settings line does not show interface 1.25x and map zoom 1.25x')
    # The movie played from the menu: the frames after the main menu was first shown (the
    # start-up logo movie before it is not checked).
    start = 0
    for i, path in enumerate(dumps):
        w, h, rows = load(path)
        if red_pixels(rows, w, h) > 0:
            start = i
            break
    movie_bad, seen = check_movie(dumps[start:])
    menu_bad, first, last = check_menu(dumps, log)
    bad += movie_bad + menu_bad
    for b in bad:
        print('menu-movie: ' + b)
    print('menu-movie: %s (%d movie frames; menu buttons %d then %d; see %s)'
          % ('ok' if not bad else 'FAILED', seen, first, last, work))
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main())
