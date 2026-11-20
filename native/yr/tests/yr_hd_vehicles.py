#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest yr-native-hd-vehicles (lane N19; built with -DRA2_NATIVE_YR=ON): the Enhancements' HD vehicles in
Yuri's Revenge (glue/yr_hd_vehicles.cpp, Red Alert 2's link/native_hd_vehicles.cpp carried over), on the Mac
through the real menus with scripted input: main menu -> Single Player -> Skirmish -> Start Game, the view
scrolled right and back in play, twice: HD vehicles 2x with Map zoom 1.5x, and 3x (the frame budget off,
RA2_NATIVE_HD_BUDGET_MS=0, so the game stays at 3x).

Checked by the Nx layer's own counts (RA2_NATIVE_HD_STATS=1: every 100 pictures "renders R blits B pixels
P; shown S, mismatched M; ..." and "at Nx; by kind (renders, shown): vehicles R S ..."), as Red Alert 2's
native-hd-vehicles: the game took the scale ("hd-vehicles: 2x for this game"); every count has renders,
blits and vehicles shown at Nx, at least 1,000 screen pixels in the median count; no Nx pixel group
disagrees with the 1x pixel it stands for (mismatched 0: the top-left Nx pixel is the 1x render's own, so
an image of another facing, frame or place would show here); and the run reaches its time limit without a
trap or a fault. Skipped (77) without the installed game (gamemd.exe in --data-dir).

  yr_hd_vehicles.py --binary <yr-native> --data-dir <game folder> --work <dir>
"""
import argparse
import os
import re
import shutil
import subprocess
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', 'tests'))
import game_process  # noqa: E402  (native/tests: the game's process group, lane C19)

SCRIPT = """\
s1+1000 click 720 220
s2+1000 click 720 304
s3+2000 click 720 262
s3+3000 move 316 300
300 keydown Right
360 keyup Right
420 keydown Left
480 keyup Left
"""
COUNTS = re.compile(r'^hd-vehicles: renders (\d+) blits (\d+) pixels (\d+); shown (\d+), mismatched (\d+);', re.M)
KINDS = re.compile(r'^hd-vehicles: at (\d)x; by kind \(renders, shown\): vehicles (\d+) (\d+)', re.M)


def run(a, name, enhancements):
    work = os.path.join(a.work, name)
    shutil.rmtree(work, ignore_errors=True)
    os.makedirs(os.path.join(work, 'w'))
    script = os.path.join(work, 'hd.script')
    with open(script, 'w') as f:
        f.write(SCRIPT)
    env = dict(os.environ, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_HD_STATS='1',
               RA2_NATIVE_HD_BUDGET_MS='0', RA2_NATIVE_SETTINGS=os.path.join(work, 'settings'),
               RA2_NATIVE_ENHANCEMENTS=enhancements)
    cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=never', '--run-seconds=%d' % a.seconds,
           '--write-dir=' + os.path.join(work, 'w'), '--input-script=' + script, '--game-args=-noaudio -CD.']
    with open(os.path.join(work, 'run.log'), 'w') as log:
        game_process.run(cmd, env=env, stdout=log, stderr=subprocess.STDOUT, timeout=a.seconds + 120)
    return open(os.path.join(work, 'run.log'), errors='replace').read()


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--work', required=True)
    ap.add_argument('--seconds', type=int, default=55)
    a = ap.parse_args()
    if not a.data_dir or not os.path.exists(os.path.join(a.data_dir, 'gamemd.exe')):
        print('skipped: no Yuri\'s Revenge install (gamemd.exe) in --data-dir')
        return 77
    failures = []
    for name, scale, enh in (('hd2', 2, 'hdvehicles=2,mapzoom=1.5'), ('hd3', 3, 'hdvehicles=3')):
        text = run(a, name, enh)
        stop = [l for l in text.splitlines() if 'stopped:' in l]
        counts = [tuple(int(v) for v in c) for c in COUNTS.findall(text)]
        kinds = [(int(s), int(r), int(v)) for s, r, v in KINDS.findall(text)]
        print('%s: %d counts, last %s; by kind (scale, vehicle renders, shown) %s; %s' % (
            name, len(counts), counts[-1:] if counts else None, kinds[-1:] if kinds else None, stop[-1] if stop else 'no stop line'))
        if not stop or 'time limit' not in stop[-1]:
            failures.append('%s: the run stopped early: %s' % (name, stop[-1] if stop else 'no stop line'))
        if 'hd-vehicles: %dx for this game' % scale not in text:
            failures.append('%s: the game did not take HD vehicles %dx' % (name, scale))
        if len(counts) < 2:
            failures.append('%s: fewer than two counts (%d)' % (name, len(counts)))
            continue
        if any(r == 0 or b == 0 for r, b, _, _, _ in counts):
            failures.append('%s: a count without voxel renders or Nx blits' % name)
        if any(m for *_, m in counts):
            failures.append('%s: Nx pixel groups disagree with their 1x pixels (%s)' % (name, [c[4] for c in counts]))
        shown = sorted(c[3] for c in counts)[len(counts) // 2]
        if shown < 1000:
            failures.append('%s: the median count shows %d screen pixels at Nx (wanted 1,000 or more)' % (name, shown))
        if not kinds or any(s != scale for s, _, _ in kinds) or not any(v for _, _, v in kinds):
            failures.append('%s: no vehicles shown at %dx by kind (%s)' % (name, scale, kinds[-1:]))
    for f in failures:
        print('FAIL: ' + f)
    print('%s (%s)' % ('FAIL' if failures else 'OK', a.work))
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
