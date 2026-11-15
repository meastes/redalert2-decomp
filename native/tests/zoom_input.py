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
  pinch   two scripted fingers (the touch controls on, RA2_NATIVE_TOUCH_UI) move apart from 100 to
          220 points: a pinch (no click, no selection, no pan), the zoom about 2.2x; then they move
          together: back down.

  zoom_input.py --case steps|menu|pinch --binary <ra2-native> --data-dir <installed RA2> --game-exe <game.exe>
                [--log <file>]

Without the data folder or game.exe it exits 77 (skipped); the Windows build too (no window
mapping under Wine to check here)."""
import argparse
import os
import re
import subprocess
import sys
import tempfile

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
    ap.add_argument('--case', required=True, choices=['steps', 'menu', 'pinch'])
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
    elif a.case == 'menu':
        env['RA2_NATIVE_SCRIPT_AS_USER'] = '1'
        lines, frames = MENU, 250
    else:
        env['RA2_NATIVE_TOUCH_UI'] = '1'
        lines, frames = pinch_script(), 140
    with tempfile.TemporaryDirectory(prefix='ra2-zoom-') as work:
        script = os.path.join(work, 'input.script')
        with open(script, 'w') as f:
            f.write('\n'.join(lines) + '\n')
        write_dir = os.path.join(work, 'write')
        os.mkdir(write_dir)
        cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=frames', '--run-seconds=240', '--write-dir=' + write_dir,
               '--game-args=-noaudio -CD.', '--skirmish=#5', '--skirmish-frames=%d' % frames, '--input-script=' + script]
        p = subprocess.run(cmd, env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, errors='replace')
    out = p.stdout
    if a.log:
        with open(a.log, 'w') as f:
            f.write(out)
    problems = []
    bad = [l for l in out.splitlines() if BAD.search(l)]
    if p.returncode != 0 or bad:
        problems.append('exit %d, %d bad lines' % (p.returncode, len(bad)))
    full, scale = FULL, 1
    m = SCREEN.search(out)
    if m:
        scale = int(m.group(1))
        full = (int(m.group(2)) - 168, int(m.group(3)) - 32)
    print('render scale %d: tactical area %dx%d' % (scale, full[0], full[1]))
    if scale != 2:
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
    elif a.case == 'menu':
        want = 'zoom: in-play dialogs at %dx%d' % (full[0] // scale + 168, full[1] // scale + 32)
        print('%s: %s' % (want, want in out))
        if want not in out:
            problems.append('the dialogs were not laid out at the window\'s size ("%s")' % want)
        if 'host: layout: in-play dialogs at' not in out:
            problems.append('the host did not show the dialogs\' part of the screen')
        if 'host: layout: dialog in play' in out:
            problems.append('a dialog was shown with the larger screen\'s layout')
        last = out.rfind('host: layout: in-play dialogs at')
        if last < 0 or 'host: layout: map zoom' not in out[last:]:
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
