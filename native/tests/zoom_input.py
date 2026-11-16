#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""Zoom gestures in a skirmish ('#5'; lane C16; Enhancements "Zoom gestures" on), checked by the
game's state and the host's own measures:

  steps   the zoom gestures' steps (input script "zoom FACTOR X Y", the wheel's and a pinch's path)
          to 1.37x, 2.6x and out to 0.5x (the least: the game's screen is twice the window's in its
          map column, link/native_map_zoom.cpp Native_Render_Size). At each zoom: the logged view (link/native_map_zoom.cpp
          "zoom:") has the zoom asked for, a view no larger than the tactical area and a shown part of
          the tactical area divided by the zoom; the window mapping checked both ways over the shown
          map ("mapcheck": no pixel comes back as another); a click through the window (as a
          player's, RA2_NATIVE_SCRIPT_AS_USER) on a given tank selects that tank; and the sidebar's
          place in the window ("host: layout") is the same at every zoom.
  menu    the in-game menu at the render scale (Escape), its Save screen, Back, Abort Mission and the
          Abort screen's Resume Mission, each clicked through the window (as a player's) where the
          original at 1024x768 has them: the game lays the dialogs out at the window's size
          (link/native_map_zoom.cpp Native_Dialog_Screen: "in-play dialogs at 1024x768") and the host
          shows that part (the column's bottom button, Back and Resume Mission, is where the
          original has it only when the interface art was laid out again); then the game plays on
          to its last frame with the play layout back.
  score   a skirmish won at frame 100 (RA2_NATIVE_FORCE_END): the score screen (a menu-style dialog
          centred on the screen) is shown as the menus are, the screen's centre at the window's scale,
          not through the in-play dialog layout; its Continue button clicked through the window (where
          the original at 1024x768 has it, moved to the screen's centre) ends the game (WinMain returns).
  abort   the in-game menu's Abort Mission and the Abort screen's Quit (interface 1.25x, render scale 2):
          every picture the window shows (RA2_NATIVE_WINDOW_DUMP_MS=0) is checked as it is written; none
          may be the zoomed game's screen shown whole (the tactical part in the top left corner, the
          bottom half black: the user's Duo report after Abort Mission), and the game ends.
  pinch   two scripted fingers (the touch controls on, RA2_NATIVE_TOUCH_UI) move apart from 100 to
          220 points: a pinch (no click, no selection, no pan), the zoom about 2.2x; then they move
          together: back down.

  zoom_input.py --case steps|menu|score|abort|pinch --binary <ra2-native> --data-dir <installed RA2> --game-exe <game.exe>
                [--log <file>]

Without the data folder or game.exe it exits 77 (skipped); the Windows build too (no window
mapping under Wine to check here)."""
import argparse
import os
import re
import struct
import subprocess
import sys
import tempfile
import threading
import time

SKIP = 77
BAD = re.compile(r'unrecovered|trap|signal \d+|SIG(SEGV|BUS|ILL|ABRT|FPE)|crash:|abort', re.I)
ZOOM = re.compile(r'^zoom: ([\d.]+) -> ([\d.]+) at [-\d.]+,[-\d.]+: view (\d+)x(\d+), shown ([\d.]+)x([\d.]+) from', re.M)
CHECK = re.compile(r'^host: mapcheck at map zoom ([\d.]+): (\d+) pixels, largest error ([\d.]+), (\d+) come back as another', re.M)
LAYOUT = re.compile(r'^host: layout: map zoom ([\d.]+)(?: \(render scale \d+\))?, sidebar ([\d.,x ]+)$', re.M)
SCREEN = re.compile(r"^zoom: render scale (\d+): the game's screen (\d+)x(\d+) for", re.M)
STATE = re.compile(r'^harness-state (\d+) selected (\d+);(.*)$', re.M)
FULL = (856, 736)    # the tactical area at 1024x768 (the skirmish harness's screen), at render scale 1

# Each zoom is anchored at the tank clicked next, so that tank stays on the screen (the start
# positions vary).
STEPS = ['90 mapcheck', '100 zoom 1.37 @MTNK:1', '110 mapcheck', '120 click @MTNK:1',
         '150 zoom 1.9 @MTNK:2', '160 mapcheck', '170 click @MTNK:2',
         '200 zoom 0.1 400 300', '210 mapcheck', '220 click @MTNK:3']
WANT_ZOOMS = [1.369, 2.602, 0.5]     # as the player sees them (the logged zoom over the render scale)
CLICKS = [(140, 1), (195, 2), (240, 3)]     # frame, the tank (n-th MTNK) selected then

# The in-game menu's buttons in game pixels, where the original at 1024x768 has them: Save Game and
# Abort Mission in the sidebar column's list, Back and Resume Mission at its bottom ("dN": the N-th
# dialog shown: 1 the menu, 2 Save, 3 the menu, 4 Abort).
MENU = ['150 key Escape', 'd1+1500 click 940 289', 'd2+1500 click 940 714', 'd3+1500 click 940 339',
        'd4+1500 click 940 714']
# Abort Mission, then the Abort screen's Quit (the column's first button).
ABORT = ['30 key Escape', 'd1+1500 click 940 339', 'd2+2000 click 940 239']
# The score screen's Continue (game pixels: 831,640 at 1024x768, the screen's centre 428,368 further).
SCORE = ['d1+1500 click 1259 1008']


def quadrants(path):
    """The mean brightness (0-765) of a BMP picture's four quadrants (top left, top right, bottom
    left, bottom right), sampled every 24 pixels; None for an unreadable file."""
    try:
        b = open(path, 'rb').read()
        off = struct.unpack_from('<I', b, 10)[0]
        w, h = struct.unpack_from('<ii', b, 18)
        step = struct.unpack_from('<H', b, 28)[0] // 8
    except (OSError, struct.error):
        return None
    rows, row = abs(h), ((w * step + 3) // 4) * 4
    if step < 3 or len(b) < off + rows * row:
        return None
    q = []
    for x0, y0 in ((0, 0), (w // 2, 0), (0, rows // 2), (w // 2, rows // 2)):
        total = n = 0
        for y in range(y0, y0 + rows // 2, 24):
            line = (rows - 1 - y) * row if h > 0 else y * row
            for x in range(x0, x0 + w // 2, 24):
                i = off + line + x * step
                total += b[i] + b[i + 1] + b[i + 2]
                n += 1
        q.append(total // max(n, 1))
    return q


def pinch_script():
    lines = ['100 tdown 350 300 1', '100 tdown 450 300 2']
    for k in range(1, 11):
        lines += ['%d tmove %d 300 1' % (101 + k, 350 - 6 * k), '%d tmove %d 300 2' % (101 + k, 450 + 6 * k)]
    for k in range(1, 6):
        lines += ['%d tmove %d 300 1' % (111 + k, 290 + 4 * k), '%d tmove %d 300 2' % (111 + k, 510 - 4 * k)]
    lines += ['118 tup 310 300 1', '118 tup 490 300 2']
    return lines


def tanks_selected(state):
    return [i for i, m in enumerate(re.finditer(r' MTNK(\*?):', state)) if m.group(1)]


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--case', required=True, choices=['steps', 'menu', 'menu-ui', 'score', 'abort', 'pinch'])
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default=os.environ.get('RA2_NATIVE_CENSUS_DATA_DIR', ''))
    ap.add_argument('--game-exe', default=os.environ.get('RA2_NATIVE_CENSUS_GAME_EXE', ''))
    ap.add_argument('--log', default='')
    a = ap.parse_args()
    name = 'native-zoom-' + a.case
    if not a.data_dir or not os.path.isdir(a.data_dir) or not a.game_exe or not os.path.isfile(a.game_exe):
        print('%s: skipped (set RA2_NATIVE_CENSUS_DATA_DIR and RA2_NATIVE_CENSUS_GAME_EXE)' % name)
        return SKIP
    if a.binary.lower().endswith('.exe'):
        print('%s: skipped (the Windows build)' % name)
        return SKIP
    env = dict(os.environ, RA2_GAME_EXE=a.game_exe, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_CRASH_TRACE='1',
               RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_INPUT_LOG='5', RA2_NATIVE_ENHANCEMENTS='zoomgestures=on')
    env.pop('RA2_NATIVE_TRAP_CONTINUE', None)
    if a.case == 'steps':
        env['RA2_NATIVE_SCRIPT_AS_USER'] = '1'
        lines, frames = STEPS, 250
    elif a.case in ('menu', 'menu-ui'):
        env['RA2_NATIVE_SCRIPT_AS_USER'] = '1'
        # Game interface size 1.25x (the Duo's setting): play lays the sidebar art out shorter than
        # the screen; the dialogs need it at the screen's whole height (the user's Duo report: the
        # command bar across the menu, the column ending above Resume Mission). 'menu-ui' is the
        # same without Zoom gestures (render scale 1).
        env['RA2_NATIVE_ENHANCEMENTS'] = 'uisize=1.25' + (',zoomgestures=on' if a.case == 'menu' else '')
        lines, frames = MENU, 250
    elif a.case == 'score':
        env['RA2_NATIVE_SCRIPT_AS_USER'] = '1'
        env['RA2_NATIVE_FORCE_END'] = '100'
        lines, frames = SCORE, 300
    elif a.case == 'abort':
        env['RA2_NATIVE_SCRIPT_AS_USER'] = '1'
        env['RA2_NATIVE_ENHANCEMENTS'] = 'uisize=1.25,zoomgestures=on'
        lines, frames = ABORT, 100000
    else:
        env['RA2_NATIVE_TOUCH_UI'] = '1'
        lines, frames = pinch_script(), 140
    with tempfile.TemporaryDirectory(prefix='ra2-zoom-') as work:
        script = os.path.join(work, 'input.script')
        with open(script, 'w') as f:
            f.write('\n'.join(lines) + '\n')
        write_dir = os.path.join(work, 'write')
        os.mkdir(write_dir)
        seconds = 40 if a.case == 'abort' else 240
        cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=seconds' if a.case in ('score', 'abort') else '--run-to=frames',
               '--run-seconds=%d' % seconds, '--write-dir=' + write_dir,
               '--game-args=-noaudio -CD.', '--skirmish=#5', '--skirmish-frames=%d' % frames, '--input-script=' + script]
        corners = []
        if a.case == 'abort':
            dumps = os.path.join(work, 'dumps')
            os.mkdir(dumps)
            env['RA2_NATIVE_WINDOW_DUMP'] = os.path.join(dumps, 'w')
            env['RA2_NATIVE_WINDOW_DUMP_MS'] = '0'
            proc = subprocess.Popen(cmd, env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, errors='replace')
            out_parts = []
            reader = threading.Thread(target=lambda: out_parts.append(proc.stdout.read()))
            reader.start()
            seen = 0
            while True:
                running = proc.poll() is None
                # Each picture once the next exists (so it is complete), or all once the game ended.
                names = sorted(os.listdir(dumps), key=lambda n: int(n.split('.')[1]))
                for n in names[:-1] if running else names:
                    path = os.path.join(dumps, n)
                    q = quadrants(path)
                    if q and q[0] > 60 and q[2] < 5 and q[3] < 5:
                        corners.append((n, q))
                    os.remove(path)
                    seen += 1
                if not running:
                    break
                time.sleep(0.05)
            reader.join()
            p = subprocess.CompletedProcess(cmd, proc.returncode, ''.join(out_parts))
            print('abort: %d pictures checked' % seen)
        else:
            p = subprocess.run(cmd, env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, errors='replace')
    out = p.stdout
    if a.log:
        with open(a.log, 'w') as f:
            f.write(out)
    problems = []
    bad = [l for l in out.splitlines() if BAD.search(l) and 'abort at frame' not in l]
    if (p.returncode != 0 and not (a.case == 'abort' and p.returncode == 2)) or bad:
        problems.append('exit %d, %d bad lines' % (p.returncode, len(bad)))
    full, scale = FULL, 1
    m = SCREEN.search(out)
    if m:
        scale = int(m.group(1))
        full = (int(m.group(2)) - 168, int(m.group(3)) - 32)
    print('render scale %d: tactical area %dx%d' % (scale, full[0], full[1]))
    if scale != (1 if a.case == 'menu-ui' else 2):
        problems.append('render scale %d, wanted 2 (Zoom gestures on)' % scale)
    zooms = [(float(m.group(1)), float(m.group(2)), int(m.group(3)), int(m.group(4)), float(m.group(5)), float(m.group(6)))
             for m in ZOOM.finditer(out)]
    for z0, z1, vw, vh, sw, sh in zooms:
        print('zoom %.3f -> %.3f: view %dx%d, shown %.2fx%.2f' % (z0, z1, vw, vh, sw, sh))
        if vw > full[0] or vh > full[1] or sw > vw or sh > vh:
            problems.append('zoom %.3f: view %dx%d or shown %.1fx%.1f too large' % (z1, vw, vh, sw, sh))
        # (the log's zoom has three decimals: compared as tactical area over shown part)
        if abs(full[0] / sw - z1) > 0.002 or abs(full[1] / sh - z1) > 0.002:
            problems.append('zoom %.3f: shown %.2fx%.2f, not the tactical area divided by the zoom' % (z1, sw, sh))
    states = {int(m.group(1)): (int(m.group(2)), m.group(3)) for m in STATE.finditer(out)}
    if a.case == 'steps':
        got = [round(z[1] / scale, 3) for z in zooms]
        if [abs(g - w) < 0.002 for g, w in zip(got, WANT_ZOOMS)] != [True] * len(WANT_ZOOMS) or len(got) != len(WANT_ZOOMS):
            problems.append('zooms %s, wanted %s' % (got, WANT_ZOOMS))
        checks = [(float(m.group(1)), int(m.group(2)), float(m.group(3)), int(m.group(4))) for m in CHECK.finditer(out)]
        for z, n, err, other in checks:
            print('mapcheck at %.3f: %d pixels, largest error %.5f, %d another' % (z, n, err, other))
            if other or err > 0.01 or n < 1000:
                problems.append('mapcheck at %.3f: %d of %d pixels come back as another (error %.5f)' % (z, other, n, err))
        if len(checks) != 4:
            problems.append('%d mapchecks, wanted 4' % len(checks))
        sidebars = set(m.group(2) for m in LAYOUT.finditer(out))
        print('sidebar places: %s' % sorted(sidebars))
        if len(sidebars) != 1:
            problems.append('the sidebar moved with the zoom: %s' % sorted(sidebars))
        for frame, tank in CLICKS:
            st = states.get(frame)
            sel = tanks_selected(st[1]) if st else None
            print('frame %d: tanks selected %s (wanted [%d])' % (frame, sel, tank))
            if sel != [tank]:
                problems.append('frame %d: tanks selected %s, not [%d]' % (frame, sel, tank))
    elif a.case == 'score':
        for want, what in (('player wins at frame 100', 'the game was not won at frame 100'),
                           ('dialog #0 shown (50 controls)', 'the score screen was not shown'),
                           ('stopped: WinMain returned 0', 'Continue did not end the game (WinMain did not return)')):
            print('%s: %s' % (want, want in out))
            if want not in out:
                problems.append(what)
        if 'host: layout: dialog in play' in out:
            problems.append('the score screen was shown through the in-play dialog layout')
    elif a.case == 'abort':
        if 'abort at frame' not in out:
            problems.append('the game was not aborted')
        for n, q in corners[:5]:
            print('corner picture %s: quadrant brightness %s' % (n, q))
        if corners:
            problems.append('%d pictures show the zoomed screen whole (the tactical part in a corner)' % len(corners))
    elif a.case in ('menu', 'menu-ui'):
        want = 'zoom: in-play dialogs at %dx%d' % (full[0] // scale + 168, full[1] // scale + 32)
        print('%s: %s' % (want, want in out))
        if want not in out:
            problems.append('the dialogs were not laid out at the window\'s size ("%s")' % want)
        if a.case == 'menu' and 'host: layout: in-play dialogs at' not in out:
            problems.append('the host did not show the dialogs\' part of the screen')
        if 'host: layout: dialog in play' in out:
            problems.append('a dialog was shown with the larger screen\'s layout')
        last = out.rfind('host: layout: in-play dialogs at')
        if a.case == 'menu' and (last < 0 or 'host: layout: map zoom' not in out[last:]):
            problems.append('the game was not shown again after Resume Mission')
        if 'stopped: target reached' not in out:
            problems.append('the game did not play on to its last frame')
    else:
        if 'touch: pinch (' not in out:
            problems.append('no pinch')
        for word in ('touch: tap', 'two-finger tap right click', 'touch: two fingers moved', 'touch: left', 'touch: right'):
            if word in out:
                problems.append('the pinch also made "%s"' % word)
        peak = max([z[1] / scale for z in zooms] or [0])
        last = zooms[-1][1] / scale if zooms else 0
        print('pinch: highest zoom %.3f, last %.3f' % (peak, last))
        if not 2.1 <= peak <= 2.3:
            problems.append('the pinch to 220 of 100 points zoomed to %.3f, not about 2.2' % peak)
        if not last < peak:
            problems.append('the fingers moving together did not zoom out')
        st = states.get(140)
        if not st or st[0] != 0:
            problems.append('the pinch selected something (%s)' % (st[0] if st else 'no state'))
    for l in bad[:20]:
        print('  ' + l)
    if problems:
        print('%s: FAILED (%s)' % (name, '; '.join(problems)))
        return 1
    print('%s: passed' % name)
    return 0


if __name__ == '__main__':
    sys.exit(main())
