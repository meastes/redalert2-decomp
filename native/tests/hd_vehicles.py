#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest native-hd-vehicles (lane C16): HD vehicles 2x in a skirmish ('#5') with Map zoom 1.5x (and
native-hd-vehicles-zoom2 with Map zoom 2x, where the scrolling view moves the most screen pixels),
checked by the 2x layer's own counts (link/native_hd_vehicles.cpp, RA2_NATIVE_HD_STATS=1: every
100 pictures "renders R blits B pixels P; shown S, mismatched M; 2x work T ms a picture", and
"by kind (renders, shown): vehicles R S aircraft R S buildings R S other R S; 3x fill F").

--scale 3 (lane C17; ctest native-hd-vehicles-3x) runs the same at 3x: the same checks, the time
limit scaled by the work (9/4), the frame budget off (RA2_NATIVE_HD_BUDGET_MS=0, so the game
stays at 3x), and the 3x gap fill must have filled sub-pixels.

--case air-base (lane C17; ctests native-hd-air-base and native-hd-air-base-3x) puts aircraft and a
voxel building part on screen: the MCV deploys and builds a power plant, a barracks and a Patriot
Missile (NASAM, a voxel turret; --skirmish-order=build, RA2_NATIVE_BUILD_LIST), and two paradrops
(super 5 home: the cargo plane, a voxel aircraft) fly over the base; passes when, beside the
checks above, aircraft and buildings each show at Nx on screen in some count (their own pixels in
the picture, counted by owner kind: techno.cpp native fixup).

The start units move (--skirmish-order=both: the MCV deploys, the tanks and infantry drive east)
and two tanks force-fire at the ground in four directions each (Ctrl with a click: the original's
force fire), so bodies and turrets take many facings; then the view scrolls (arrow keys). The game's voxel and shadow caches are off
while HD is on (techno.cpp fixups), so every facing is drawn afresh with its 2x image; a cached
facing would have none and show at 1x. Passes when, from the first count on:
  * units show at 2x in every count, at least 1,000 screen pixels in the median one (the start
    positions vary; under a load average above the CPUs, in the best one: the counted picture then
    comes late in the scroll, E1b's gate saw a median of 467);
  * no 2x pixel group disagrees with the 1x pixel it stands for (mismatched 0: the top-left 2x
    pixel is the 1x render's own, so a 2x image of another facing or frame would show here);
  * the 2x work stays under 4 ms a picture in the median count, checked only on a quiet Mac (a
    load average under 1; otherwise, and under Wine, only reported: it failed at 4.68 ms at load 3
    beside other lanes, owner ruling 2026-11-15);
  * no picture flickers back to 1x (drops 0: fewer than half the 2x unit pixels of the picture
    before, from at least 2,000; only reported under that load);
and the run ends without a fault.

  hd_vehicles.py --binary <ra2-native> --data-dir <installed RA2> --game-exe <game.exe> [--zoom 1.5|2] [--scale 2|3]
                 [--case units|air-base] [--log <file>]
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
KINDS = re.compile(r'^hd-vehicles: at (\d)x; by kind \(renders, shown\): vehicles (\d+) (\d+) aircraft (\d+) (\d+) buildings (\d+) (\d+)'
                   r' other (\d+) (\d+); 3x fill (\d+)')


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

# air-base: the base is built (power plant, barracks, Patriot), then two paradrops fly over it.
AIR_BASE_FRAMES = 2400
AIR_BASE_ENV = {'RA2_NATIVE_BUILD_LIST': 'GAPOWR,GAPILE,NASAM', 'RA2_NATIVE_ORDERS': '1500 super 5 home 3 3; 1900 super 5 home -3 4'}


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default=os.environ.get('RA2_NATIVE_CENSUS_DATA_DIR', ''))
    ap.add_argument('--game-exe', default=os.environ.get('RA2_NATIVE_CENSUS_GAME_EXE', ''))
    ap.add_argument('--seconds', type=int, default=300)
    ap.add_argument('--zoom', default='1.5', help='the Map zoom (1.5; ctest native-hd-vehicles-zoom2: 2)')
    ap.add_argument('--scale', default='2', choices=['2', '3'], help='HD vehicles 2x (default) or 3x')
    ap.add_argument('--case', default='units', choices=['units', 'air-base'])
    ap.add_argument('--budget-ms', default='0', help='the 3x frame budget (0: none; ctest native-hd-vehicles-3x-budget: 0.5, '
                    'so the game must drop to 2x and go on without a fault)')
    ap.add_argument('--log', default='')
    ap.add_argument('--dump', default='', help='a folder for window pictures (with and without the HD overlay) at --dump-frames')
    ap.add_argument('--dump-frames', default='', help='comma-separated Main_Loop frames for --dump')
    ap.add_argument('--wine-prefix', default='')
    ap.add_argument('--wine', default='')
    a = ap.parse_args()
    name = ('native-hd-air-base' if a.case == 'air-base' else 'native-hd-vehicles') \
        + ('' if a.zoom == '1.5' else '-zoom' + a.zoom.replace('.', '')) + ('-3x' if a.scale == '3' else '') \
        + ('-budget' if float(a.budget_ms) > 0 else '')
    budget = float(a.budget_ms) > 0
    scale = int(a.scale)
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
        prefix = winehost.Prefix(wine, a.wine_prefix + '-' + name)   # one per test
    env = dict(os.environ, RA2_GAME_EXE=a.game_exe, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_CRASH_TRACE='1',
               RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_HD_STATS='1', RA2_NATIVE_HD_BUDGET_MS=a.budget_ms,
               RA2_NATIVE_ENHANCEMENTS='hdvehicles=%d,mapzoom=%s' % (scale, a.zoom))
    if a.case == 'air-base':
        env.update(AIR_BASE_ENV)
    dumps = ['%s dump' % f for f in a.dump_frames.split(',') if f] if a.dump else []
    if dumps:
        os.makedirs(a.dump, exist_ok=True)
        env.update(RA2_NATIVE_WINDOW_DUMP=os.path.join(os.path.abspath(a.dump), 'w'), RA2_NATIVE_WINDOW_DUMP_MS='100000000',
                   RA2_NATIVE_HD_COMPARE='1')
    env.pop('RA2_NATIVE_TRAP_CONTINUE', None)
    work = tempfile.mkdtemp(prefix='ra2-hdvox-')
    try:
        script = os.path.join(work, 'input.script')
        with open(script, 'w') as f:
            lines = (SCRIPT if a.case == 'units' else []) + dumps
            f.write('\n'.join(sorted(lines, key=lambda l: int(l.split()[0]))) + '\n')
        write_dir = os.path.join(work, 'write')
        os.mkdir(write_dir)
        cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=frames', '--run-seconds=%d' % a.seconds,
               '--write-dir=' + write_dir, '--game-args=-noaudio -CD.', '--skirmish=#5',
               '--skirmish-order=' + ('both' if a.case == 'units' else 'build'),
               '--skirmish-frames=%d' % (FRAMES if a.case == 'units' else AIR_BASE_FRAMES)]
        if a.case == 'units' or dumps:
            cmd.append('--input-script=' + script)
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
    kinds = [tuple(int(g) for g in m.groups()) for m in map(KINDS.match, out.splitlines()) if m]
    problems = []
    if p.returncode != 0 or bad:
        problems.append('exit %d, %d bad lines' % (p.returncode, len(bad)))
    if 'hd-vehicles: %dx for this game' % scale not in out:
        problems.append('the game did not take HD vehicles %dx' % scale)
    if budget:
        # Over the budget: two windows of 60 pictures at 3x, then 2x for the rest of the game.
        if 'over the %.2f ms budget: 2x for the rest of this game' % float(a.budget_ms) not in out:
            problems.append('the game did not drop to 2x over the %s ms budget' % a.budget_ms)
        if not kinds or kinds[-1][0] != 2:
            problems.append('the last count is not at 2x after the drop')
    elif 'for the rest of this game' in out:
        problems.append('the game dropped to 2x (the frame budget is off in this test)')
    if len(kinds) != len(counts) or (not budget and any(k[0] != scale for k in kinds)):
        problems.append('the per-kind counts are missing or not at %dx' % scale)
    for k, c in enumerate(kinds):
        print('count %d by kind: vehicles %d/%d aircraft %d/%d buildings %d/%d other %d/%d (renders/shown); 3x fill %d'
              % ((k,) + c[1:]))
    if kinds and scale == 3 and not budget and kinds[-1][9] <= 0:
        problems.append('3x filled no sub-pixels (the 1x colour fill did not run)')
    if kinds and scale == 2 and kinds[-1][9] != 0:
        problems.append('2x filled %d sub-pixels (2x must stay as it was)' % kinds[-1][9])
    if a.case == 'air-base':
        for kind, at in (('aircraft', 4), ('buildings', 6)):
            best = max(c[at] for c in kinds) if kinds else 0
            print('%s: at most %d screen pixels at %dx in a count' % (kind, best, scale))
            if best <= 0:
                problems.append('no %s shown at %dx in any count' % (kind, scale))
    # A count every 100 pictures: under Wine fewer pictures are shown in the same 600 frames.
    if len(counts) < 2:
        problems.append('%d counts (wanted at least 2)' % len(counts))
    # Under load (a full ctest run beside other lanes: the load average above the CPUs) the measures
    # that follow the clock are only reported: the 2x work's time, and a picture shown with the units
    # at 1x after one at 2x (presents come late and pair differently). The units shown at 2x are the
    # median count's (the view scrolls and the start positions vary), at least 1,000 pixels, and
    # never none.
    try:
        load = os.getloadavg()[0]
    except OSError:
        load = 0.0
    loaded = load > (os.cpu_count() or 1)
    if loaded:
        print('%s: load %.0f over %d CPUs: time and flicker reported only' % (name, load, os.cpu_count() or 1))
    for k, c in enumerate(counts):
        print('count %d: renders %d blits %d pixels %d; shown %d, mismatched %d; %.2f ms a picture; drops %d' % ((k,) + c))
        if c[3] <= 0 and a.case == 'units':
            problems.append('count %d: no screen pixels at 2x' % k)
        if c[4] != 0:
            problems.append('count %d: %d 2x groups disagree with their 1x pixel' % (k, c[4]))
    if counts:
        which = 'best' if loaded else 'median'
        shown = max(c[3] for c in counts) if loaded else sorted(c[3] for c in counts)[len(counts) // 2]
        if shown < 1000 and a.case == 'units':
            problems.append('the %s count shows %d screen pixels at 2x (wanted 1,000 or more)' % (which, shown))
    if counts and counts[-1][6] != 0 and not loaded:
        problems.append('%d pictures showed the units at 1x after one at 2x (flicker)' % counts[-1][6])
    # The time is the Mac's measure: under Wine (x86_64 under Rosetta) it is not the game's.
    quiet = load < 1.0
    if counts and not prefix and not quiet:
        print('%s: load %.1f: the 2x work time reported only' % (name, load))
    if counts and not prefix and quiet:
        mean = sorted(c[5] for c in counts)[len(counts) // 2]
        limit = 4.0 * scale * scale / 4
        if mean > limit:
            problems.append('the %dx work took %.2f ms a picture (the median count; wanted under %g)' % (scale, mean, limit))
    for l in bad[:20]:
        print('  ' + l)
    if problems:
        print('%s: FAILED (%s)' % (name, '; '.join(problems)))
        return 1
    print('%s: passed' % name)
    return 0


if __name__ == '__main__':
    sys.exit(main())
