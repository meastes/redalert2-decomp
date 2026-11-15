#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest native-hd-vehicles (lane C16): HD vehicles 2x in a skirmish ('#5') with Map zoom 1.5x (and
native-hd-vehicles-zoom2 with Map zoom 2x, where the scrolling view moves the most screen pixels),
checked by the 2x layer's own counts (link/native_hd_vehicles.cpp, RA2_NATIVE_HD_STATS=1: every
100 pictures "renders R blits B pixels P; shown S, mismatched M; 2x work T ms a picture").

The start units move (--skirmish-order=both: the MCV deploys, the tanks and infantry drive east)
and two tanks force-fire at the ground in four directions each (Ctrl with a click: the original's
force fire), so bodies and turrets take many facings; then the view scrolls (arrow keys). The game's voxel and shadow caches are off
while HD is on (techno.cpp fixups), so every facing is drawn afresh with its 2x image; a cached
facing would have none and show at 1x. Passes when, from the first count on:
  * every picture shows units at 2x (shown at least 1,000 screen pixels; the start positions vary);
  * no 2x pixel group disagrees with the 1x pixel it stands for (mismatched 0: the top-left 2x
    pixel is the 1x render's own, so a 2x image of another facing or frame would show here);
  * the 2x work stays under 4 ms a picture on average (not checked under Wine);
  * no picture flickers back to 1x (drops 0: fewer than half the 2x unit pixels of the picture
    before, from at least 2,000);
and the run ends without a fault.

  hd_vehicles.py --binary <ra2-native> --data-dir <installed RA2> --game-exe <game.exe> [--zoom 1.5|2] [--log <file>]
                 [--wine-prefix <dir> [--wine <wine>]]

Without the data folder or game.exe it exits 77 (skipped), as native-census."""
import argparse
import os
import re
import shutil
import subprocess
import sys
import tempfile

import wine as winehost

SKIP = 77
BAD = re.compile(r'unrecovered|trap|signal \d+|SIG(SEGV|BUS|ILL|ABRT|FPE)|crash:|abort', re.I)
COUNTS = re.compile(r'^hd-vehicles: renders (\d+) blits (\d+) pixels (\d+); shown (\d+), mismatched (\d+); 2x work ([\d.]+) ms'
                    r' a picture; drops (\d+)')


def fire(frame, tank, offsets):
    """select the tank, then force-fire (Ctrl with a click) at cells around it"""
    lines = ['%d click @MTNK:%d' % (frame, tank)]
    for k, (dx, dy) in enumerate(offsets):
        f = frame + 20 + 40 * k
        lines += ['%d keydown Left_Ctrl' % f, '%d click @MTNK:%d%+d,%d' % (f + 1, tank, dx, dy), '%d keyup Left_Ctrl' % (f + 5)]
    return lines


# The view scrolls too (the arrow keys, frames 500-580): the battlefield pans under the units.
SCRIPT = (fire(120, 1, [(5, 0), (0, 5), (-5, 0), (0, -5)]) + fire(320, 2, [(4, 4), (-4, 4), (-4, -4), (4, -4)])
          + ['500 keydown Right', '530 keyup Right', '540 keydown Down', '580 keyup Down'])
FRAMES = 600


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default=os.environ.get('RA2_NATIVE_CENSUS_DATA_DIR', ''))
    ap.add_argument('--game-exe', default=os.environ.get('RA2_NATIVE_CENSUS_GAME_EXE', ''))
    ap.add_argument('--seconds', type=int, default=300)
    ap.add_argument('--zoom', default='1.5', help='the Map zoom (1.5; ctest native-hd-vehicles-zoom2: 2)')
    ap.add_argument('--log', default='')
    ap.add_argument('--wine-prefix', default='')
    ap.add_argument('--wine', default='')
    a = ap.parse_args()
    name = 'native-hd-vehicles' + ('' if a.zoom == '1.5' else '-zoom' + a.zoom.replace('.', ''))
    if not a.data_dir or not os.path.isdir(a.data_dir) or not a.game_exe or not os.path.isfile(a.game_exe):
        print('%s: skipped (set RA2_NATIVE_CENSUS_DATA_DIR and RA2_NATIVE_CENSUS_GAME_EXE)' % name)
        return SKIP
    prefix = None
    if a.binary.lower().endswith('.exe'):
        if not a.wine_prefix:
            print('%s: skipped (a Windows build without a Wine prefix)' % name)
            return SKIP
        wine = winehost.find_wine(a.wine)
        if not wine:
            print('%s: Windows build and no Wine (set RA2_NATIVE_WINE); skipped' % name)
            return SKIP
        prefix = winehost.Prefix(wine, a.wine_prefix + ('' if a.zoom == '1.5' else '-zoom' + a.zoom))   # one per test
    env = dict(os.environ, RA2_GAME_EXE=a.game_exe, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_CRASH_TRACE='1',
               RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_HD_STATS='1', RA2_NATIVE_ENHANCEMENTS='hdvehicles=2,mapzoom=' + a.zoom)
    env.pop('RA2_NATIVE_TRAP_CONTINUE', None)
    work = tempfile.mkdtemp(prefix='ra2-hdvox-')
    try:
        script = os.path.join(work, 'input.script')
        with open(script, 'w') as f:
            f.write('\n'.join(SCRIPT) + '\n')
        write_dir = os.path.join(work, 'write')
        os.mkdir(write_dir)
        cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=frames', '--run-seconds=%d' % a.seconds,
               '--write-dir=' + write_dir, '--game-args=-noaudio -CD.', '--skirmish=#5', '--skirmish-order=both',
               '--skirmish-frames=%d' % FRAMES, '--input-script=' + script]
        for attempt in range(winehost.HOST_RETRIES + 1):
            run_env, run_cmd = env, cmd
            if prefix:
                run_cmd, run_env = prefix.command(cmd), prefix.env(env)
            p = subprocess.run(run_cmd, env=run_env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True,
                               errors='replace')
            why = winehost.host_failure(p.stdout) if prefix else ''
            if not why or attempt == winehost.HOST_RETRIES:
                break
            print('%s: %s; running again' % (name, why))
    finally:
        if prefix:
            prefix.close()
        shutil.rmtree(work, ignore_errors=True)
    out = p.stdout
    if a.log:
        with open(a.log, 'w') as f:
            f.write(out)
    bad = [l for l in out.splitlines() if BAD.search(l)]
    counts = [tuple(float(g) for g in m.groups()) for m in map(COUNTS.match, out.splitlines()) if m]
    problems = []
    if p.returncode != 0 or bad:
        problems.append('exit %d, %d bad lines' % (p.returncode, len(bad)))
    if 'hd-vehicles: 2x for this game' not in out:
        problems.append('the game did not take HD vehicles')
    # A count every 100 pictures: under Wine fewer pictures are shown in the same 600 frames.
    if len(counts) < 2:
        problems.append('%d counts (wanted at least 2)' % len(counts))
    for k, c in enumerate(counts):
        print('count %d: renders %d blits %d pixels %d; shown %d, mismatched %d; %.2f ms a picture; drops %d' % ((k,) + c))
        if c[3] < 1000:
            problems.append('count %d: %d screen pixels at 2x (wanted 1,000 or more)' % (k, c[3]))
        if c[4] != 0:
            problems.append('count %d: %d 2x groups disagree with their 1x pixel' % (k, c[4]))
    if counts and counts[-1][6] != 0:
        problems.append('%d pictures showed the units at 1x after one at 2x (flicker)' % counts[-1][6])
    # The time is the Mac's measure: under Wine (x86_64 under Rosetta) it is not the game's.
    if counts and not prefix:
        mean = sum(c[5] for c in counts) / len(counts)
        if mean > 4.0:
            problems.append('the 2x work took %.2f ms a picture on average (wanted under 4)' % mean)
    for l in bad[:20]:
        print('  ' + l)
    if problems:
        print('%s: FAILED (%s)' % (name, '; '.join(problems)))
        return 1
    print('%s: passed' % name)
    return 0


if __name__ == '__main__':
    sys.exit(main())
