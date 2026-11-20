#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest yr-native-map-zoom (lane N18; built with -DRA2_NATIVE_YR=ON): the Enhancements' Map zoom and
zoom row (Trackpad and wheel zoom on the Mac) in Yuri's Revenge (glue/yr_map_zoom.cpp, Red Alert 2's
link/native_map_zoom.cpp carried over), on the Mac through the real menus with scripted input:

  main menu -> Single Player -> Skirmish -> Start Game, with RA2_NATIVE_ENHANCEMENTS=
  "mapzoom=2,zoomgestures=on"; at game frames 200-420: a window mapping check, a window dump, the zoom
  gestures' steps (input script "zoom FACTOR X Y") to 2.6x, then down past 1x (kept at 1x), each with a
  mapping check and a dump.

Checks: the run reaches the time limit without a trap or a fault; the game's view is half the
tactical area at the start ("enhancements: map view ... (map zoom 2.0x)"); the steps log the zooms
asked for (2 -> 2.6, 2.6 -> 1); every mapping check passes (no pixel comes back as another); the
host's sidebar place in the window is the same at every zoom; and the sidebar column is drawn where the
screen has it (its region in each dump about as lit as in a run with the defaults, which is the
check that the game draws the sidebar at the tactical area's edge, not the zoomed view's). A run with
the defaults has no zoom lines. Skipped (77) without the installed game (gamemd.exe in --data-dir).

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
"""
ZOOM = re.compile(r'^zoom: ([\d.]+) -> ([\d.]+) at', re.M)
VIEW = re.compile(r'^enhancements: map view (\d+)x(\d+) at -?\d+,-?\d+ for (\d+)x(\d+) \(map zoom ([\d.]+)x\)', re.M)
CHECK = re.compile(r'^host: mapcheck at map zoom ([\d.]+): (\d+) pixels, largest error ([\d.]+), (\d+) come back as another', re.M)
LAYOUT = re.compile(r'^host: layout: map zoom ([\d.]+)(?: \(render scale \d+\))?, sidebar ([\d.,x ]+)$', re.M)
SIDEBAR = (640, 170, 795, 590)    # x0, y0, x1, y1 of the 800x600 screen: the sidebar's cameo column


def run(a, name, enhancements):
    work = os.path.join(a.work, name)
    shutil.rmtree(work, ignore_errors=True)
    os.makedirs(os.path.join(work, 'w'))
    os.makedirs(os.path.join(work, 'shots'))
    script = os.path.join(work, 'zoom.script')
    with open(script, 'w') as f:
        f.write(MENUS + PLAY)
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


def sidebar_lit(work, text):
    """The lit share of the sidebar column in each window dump, in dump order."""
    shares = []
    for n in re.findall(r'^host: window dump (\d+)', text, re.M):
        path = os.path.join(work, 'shots', 'w.%s.bmp' % n)
        if not os.path.exists(path):
            continue
        width, height, rows = read_bmp(path)
        sx, sy = width / 800.0, height / 600.0
        x0, y0, x1, y1 = SIDEBAR
        lit = total = 0
        for y in range(y0, y1, 3):
            for x in range(x0, x1, 3):
                total += 1
                if max(rows[min(height - 1, int(y * sy))][min(width - 1, int(x * sx))]) >= 40:
                    lit += 1
        shares.append(lit / max(1, total))
    return shares


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
    for name, enh in (('zoom', 'mapzoom=2,zoomgestures=on'), ('plain', '')):
        text, work = run(a, name, enh)
        stop = [l for l in text.splitlines() if 'stopped:' in l]
        if not stop or 'time limit' not in stop[-1]:
            failures.append('%s: the run stopped early: %s' % (name, stop[-1] if stop else 'no stop line'))
        zooms = [(float(z0), float(z1)) for z0, z1 in ZOOM.findall(text)]
        views = [(int(w), int(h), int(fw), int(fh), float(z)) for w, h, fw, fh, z in VIEW.findall(text)]
        checks = [(float(z), int(n), float(e), int(bad)) for z, n, e, bad in CHECK.findall(text)]
        layouts = sorted(set(s for _, s in LAYOUT.findall(text)))
        lit = sidebar_lit(work, text)
        results[name] = lit
        print('%s: zooms %s; views %s; mapchecks %s; sidebar places %s; sidebar lit %s; %s' % (
            name, zooms, views[-3:], checks, layouts, ['%.2f' % v for v in lit], stop[-1] if stop else 'no stop line'))
        if name == 'zoom':
            if not any(z >= 1.9 for *_, z in views):
                failures.append('zoom: the game\'s view was never half the tactical area (Map zoom 2x)')
            want = [(2.0, 2.6), (2.6, 1.0)]
            got = [(round(z0, 1), round(z1, 1)) for z0, z1 in zooms]
            if got != want:
                failures.append('zoom: the gestures\' steps were %s, %s expected' % (got, want))
            if len(checks) < 3 or any(n == 0 or bad for _, n, _, bad in checks):
                failures.append('zoom: a window mapping check failed or did not run (%s)' % checks)
            if len(layouts) > 1:
                failures.append('zoom: the sidebar moved in the window between zooms (%s)' % layouts)
        elif zooms or any(z > 1.01 for *_, z in views):
            failures.append('plain: zoom lines with the defaults')
    plain = results.get('plain') or []
    zoom = results.get('zoom') or []
    if len(zoom) < 3 or len(plain) < 3:
        failures.append('fewer than three window dumps in a run (zoom %d, plain %d)' % (len(zoom), len(plain)))
    else:
        for i, (z, p) in enumerate(zip(zoom, plain)):
            if p > 0.1 and z < 0.5 * p:
                failures.append('dump %d: the sidebar column is not drawn while zoomed (%.0f%% lit, %.0f%% unzoomed)'
                                % (i + 1, z * 100, p * 100))
    for f in failures:
        print('FAIL: ' + f)
    print('%s (%s)' % ('FAIL' if failures else 'OK', a.work))
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
