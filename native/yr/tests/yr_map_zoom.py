#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest yr-native-map-zoom (lanes N18, N20; built with -DRA2_NATIVE_YR=ON): the Enhancements' Map zoom
and zoom row (Trackpad and wheel zoom on the Mac) in Yuri's Revenge (glue/yr_map_zoom.cpp, Red Alert 2's
link/native_map_zoom.cpp carried over), on the Mac through the real menus with scripted input:

  main menu -> Single Player -> Skirmish -> Start Game, with RA2_NATIVE_ENHANCEMENTS=
  "mapzoom=2,zoomgestures=on"; at game frames 200-330: a window mapping check, a window dump, the zoom
  gestures' steps (input script "zoom FACTOR X Y") to 2.6x, then down to the least, 0.5x, each with a
  mapping check and a dump; then Escape (the in-game menu) and a dump of it, its Resume Mission
  clicked through the window where the original at 800x600 has it, and play again (a mapping check and
  a dump at 0.5x).

Checks: the run reaches the time limit without a trap or a fault; with Zoom gestures on the game's own
mode is twice the window's in its map column and above the command bar (lane N20, the render scale:
"zoom: render scale 2: the game's screen 1432x1168 for 800x600", Yr_Game_Mode_Size) while the menus
keep their 800x600; the game's view is half the tactical area of the window's size at the start (the
game's zoom 4: Map zoom 2x times the render scale); the steps log the zooms asked for, as the player
sees them (the logged zoom over the render scale: 2 -> 2.6, 2.6 -> 0.5); at 0.5x the view is the whole
1264x1136 tactical area (the window's map shows twice its pixels at the art's own size); every mapping
check passes (no pixel comes back as another); the host's sidebar place in the window is the same at
every zoom; the sidebar column is drawn where the screen has it (its region in each dump about as lit
as in a run with the defaults, which is the check that the game draws the sidebar at the tactical
area's edge, not the zoomed view's); the in-game menu is laid out at the window's size ("zoom: in-play
dialogs at 800x600", Yr_Dialog_Screen) and its dump matches the defaults' (the same picture, about);
Resume Mission ends it and the play layout is back. A run with the defaults has no zoom or render scale
lines. Skipped (77) without the installed game (gamemd.exe in --data-dir).

  yr_map_zoom.py --binary <yr-native> --data-dir <game folder> --work <dir>
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

MENUS = """\
s1+1000 click 720 220
s2+1000 click 720 304
s3+2000 click 720 262
s3+3000 move 316 300
"""
PLAY = """\
200 mapcheck
210 dump
240 zoom 1.3 300 250
260 mapcheck
270 dump
300 zoom 0.1 300 250
320 mapcheck
330 dump
350 key Escape
s4+1500 dump
s4+2000 click 720 515
450 mapcheck
460 dump
"""
PLAIN = """\
200 mapcheck
210 dump
240 mapcheck
270 dump
320 mapcheck
330 dump
350 key Escape
s4+1500 dump
s4+2000 click 720 515
450 mapcheck
460 dump
"""
ZOOM = re.compile(r'^zoom: ([\d.]+) -> ([\d.]+) at', re.M)
VIEW = re.compile(r'^enhancements: map view (\d+)x(\d+) at -?\d+,-?\d+ for (\d+)x(\d+) \(map zoom ([\d.]+)x\)', re.M)
CHECK = re.compile(r'^host: mapcheck at map zoom ([\d.]+): (\d+) pixels, largest error ([\d.]+), (\d+) come back as another', re.M)
LAYOUT = re.compile(r'^host: layout: map zoom ([\d.]+)(?: \(render scale \d+\))?, sidebar ([\d.,x ]+)$', re.M)
SCALE = re.compile(r"^zoom: render scale (\d+): the game's screen (\d+)x(\d+) for (\d+)x(\d+)", re.M)
DIALOGS = re.compile(r'^zoom: in-play dialogs at (\d+)x(\d+) \(the screen (\d+)x(\d+)\)', re.M)
SIDEBAR = (640, 170, 795, 590)    # x0, y0, x1, y1 of the 800x600 screen: the sidebar's cameo column


def run(a, name, enhancements, play):
    work = os.path.join(a.work, name)
    shutil.rmtree(work, ignore_errors=True)
    os.makedirs(os.path.join(work, 'w'))
    os.makedirs(os.path.join(work, 'shots'))
    script = os.path.join(work, 'zoom.script')
    with open(script, 'w') as f:
        f.write(MENUS + play)
    env = dict(os.environ, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_DIALOG_LOG='1',
               RA2_NATIVE_SETTINGS=os.path.join(work, 'settings'), RA2_NATIVE_WINDOW_DUMP=os.path.join(work, 'shots', 'w'))
    env.pop('RA2_NATIVE_ENHANCEMENTS', None)
    if enhancements:
        env['RA2_NATIVE_ENHANCEMENTS'] = enhancements
    cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=never', '--run-seconds=%d' % a.seconds,
           '--write-dir=' + os.path.join(work, 'w'), '--input-script=' + script, '--game-args=-noaudio -CD.']
    with open(os.path.join(work, 'run.log'), 'w') as log:
        game_process.run(cmd, env=env, stdout=log, stderr=subprocess.STDOUT, timeout=a.seconds + 120)
    return open(os.path.join(work, 'run.log'), errors='replace').read(), work


def window_dumps(work, text):
    """The window dumps (width, height, rows), in dump order."""
    out = []
    for n in re.findall(r'^host: window dump (\d+)', text, re.M):
        path = os.path.join(work, 'shots', 'w.%s.bmp' % n)
        if os.path.exists(path):
            out.append(read_bmp(path))
    return out


def sidebar_lit(dump):
    """The lit share of the sidebar column in a window dump."""
    width, height, rows = dump
    sx, sy = width / 800.0, height / 600.0
    x0, y0, x1, y1 = SIDEBAR
    lit = total = 0
    for y in range(y0, y1, 3):
        for x in range(x0, x1, 3):
            total += 1
            if max(rows[min(height - 1, int(y * sy))][min(width - 1, int(x * sx))]) >= 40:
                lit += 1
    return lit / max(1, total)


def difference(one, two):
    """The mean difference (0-255) of two dumps of one size, sampled every 4 pixels; None if the sizes differ."""
    (w1, h1, r1), (w2, h2, r2) = one, two
    if (w1, h1) != (w2, h2):
        return None
    total = n = 0
    for y in range(0, h1, 4):
        for x in range(0, w1, 4):
            total += sum(abs(a - b) for a, b in zip(r1[y][x], r2[y][x])) / 3.0
            n += 1
    return total / max(1, n)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--work', required=True)
    ap.add_argument('--seconds', type=int, default=60)
    a = ap.parse_args()
    if not a.data_dir or not os.path.exists(os.path.join(a.data_dir, 'gamemd.exe')):
        print('skipped: no Yuri\'s Revenge install (gamemd.exe) in --data-dir')
        return 77
    failures = []
    results = {}
    for name, enh, play in (('zoom', 'mapzoom=2,zoomgestures=on', PLAY), ('plain', '', PLAIN)):
        text, work = run(a, name, enh, play)
        stop = [l for l in text.splitlines() if 'stopped:' in l]
        if not stop or 'time limit' not in stop[-1]:
            failures.append('%s: the run stopped early: %s' % (name, stop[-1] if stop else 'no stop line'))
        scales = [tuple(int(v) for v in m) for m in SCALE.findall(text)]
        scale = max([s for s, *_ in scales] or [1])
        zooms = [(float(z0) / scale, float(z1) / scale) for z0, z1 in ZOOM.findall(text)]
        views = [(int(w), int(h), int(fw), int(fh), float(z)) for w, h, fw, fh, z in VIEW.findall(text)]
        checks = [(float(z), int(n), float(e), int(bad)) for z, n, e, bad in CHECK.findall(text)]
        layouts = sorted(set(s for _, s in LAYOUT.findall(text)))
        dialogs = [tuple(int(v) for v in m) for m in DIALOGS.findall(text)]
        shots = window_dumps(work, text)
        lit = [sidebar_lit(d) for d in shots]
        results[name] = (lit, shots)
        print('%s: render scales %s; zooms %s; views %s; mapchecks %s; sidebar places %s; in-play dialogs %s; '
              'dumps %s; sidebar lit %s; %s' % (
                  name, scales, zooms, views[-3:], checks, layouts, dialogs, [(w, h) for w, h, _ in shots],
                  ['%.2f' % v for v in lit], stop[-1] if stop else 'no stop line'))
        if name == 'zoom':
            if (2, 1432, 1168, 800, 600) not in scales:
                failures.append('zoom: the game\'s mode was not made twice the window\'s (render scale 2, 1432x1168)')
            if not any(z >= 3.9 and fw == 1264 for w, h, fw, fh, z in views):
                failures.append('zoom: the game\'s view was never half the window\'s tactical area (Map zoom 2x)')
            want = [(2.0, 2.6), (2.6, 0.5)]
            got = [(round(z0, 1), round(z1, 1)) for z0, z1 in zooms]
            if got != want:
                failures.append('zoom: the gestures\' steps were %s, %s expected' % (got, want))
            if not any(w == fw == 1264 and h == fh == 1136 for w, h, fw, fh, z in views):
                failures.append('zoom: at 0.5x the view was not the whole 1264x1136 tactical area')
            if len(checks) < 4 or any(n == 0 or bad for _, n, _, bad in checks):
                failures.append('zoom: a window mapping check failed or did not run (%s)' % checks)
            if len(layouts) > 1:
                failures.append('zoom: the sidebar moved in the window between zooms (%s)' % layouts)
            if (800, 600, 1432, 1168) not in dialogs:
                failures.append('zoom: the in-game menu was not laid out at the window\'s size (Yr_Dialog_Screen)')
            tail = text[text.rfind('zoom: in-play dialogs'):] if dialogs else ''
            if 'host: layout: map zoom' not in tail:
                failures.append('zoom: the play layout did not come back after the in-game menu')
        elif zooms or scales != [] and scale > 1 or dialogs or any(z > 1.01 for *_, z in views):
            failures.append('plain: zoom or render scale lines with the defaults')
    plain_lit, plain = results.get('plain') or ([], [])
    zoom_lit, zoom = results.get('zoom') or ([], [])
    if len(zoom) < 5 or len(plain) < 5:
        failures.append('fewer than five window dumps in a run (zoom %d, plain %d)' % (len(zoom), len(plain)))
    else:
        for i in (0, 1, 2, 4):
            z, p = zoom_lit[i], plain_lit[i]
            if p > 0.1 and z < 0.5 * p:
                failures.append('dump %d: the sidebar column is not drawn while zoomed (%.0f%% lit, %.0f%% unzoomed)'
                                % (i + 1, z * 100, p * 100))
        menu = difference(zoom[3], plain[3])
        print('the in-game menu: mean difference from the defaults\' %s' % ('%.1f' % menu if menu is not None else 'n/a'))
        if menu is None or menu > 12:
            failures.append('the in-game menu at the render scale is not the picture the defaults show (%s)' % menu)
    for f in failures:
        print('FAIL: ' + f)
    print('%s (%s)' % ('FAIL' if failures else 'OK', a.work))
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
