#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest yr-native-graphics (lane Y14; built with -DRA2_NATIVE_YR=ON): the user's reports of noise in place of
a deploying MCV and of a Grizzly on its first move (2026-11-17, iPhone). A vehicle's voxels are drawn on the
eight-bit compositing surface; a picture not yet in the voxel cache (a new facing or frame) is rendered
directly (TechnoClass::Techno_Render_Voxel_Object, glue/techno_native.inc), and the draft drew it through
the house's 16-bit converter: 16-bit pixels in an 8-bit surface, noise until the cache held the picture.

Through the real menus (Single Player, Skirmish, Start Game; the setup of yr-native-combat) the harness
(glue/yr_harness.cpp) watches the composed tactical view for noise-coloured pixels for 900 frames
(`noise 900`) while the player's MCV deploys (`deploy AMCV`, its first turn to the deploy facing and the
Construction Yard's build-up) and a Grizzly makes its first move (`move MTNK 3 3`).

The command bar (the user's report "we're also missing the bottom bar in YR"): TabClass::Draw_It draws it on
the composite surface and sets Map's unknown_byte_5546 (gamemd.exe's DAT_00884D2E), which tells
Update_Visible_Surface to copy its 32 rows; natively the flag was a separate global, so the bar never reached
the screen. The harness's `rects` (frame 600) must show the tactical view ending where the bar starts and
the bar's rows on the screen (VisibleSurface) drawn: at least 10% not black (the bar is dark; 0% without it).

Checks: both orders found their unit; the Construction Yard (GACNST) exists afterwards; no frame had more
than 60 noise pixels (the broken draw gave 75-210 a frame, a clean scene 0-20); no link-closure trap or
fault. Skipped (77) without the installed game (gamemd.exe in --data-dir).

  yr_graphics.py --binary <yr-native> --data-dir <game folder> --work <dir> [--seconds 60]
"""
import argparse
import importlib
import os
import re
import shutil
import subprocess
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', 'tests'))
import game_process  # noqa: E402  (native/tests: the game's process group, lane C19)

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
combat = importlib.import_module('yr_combat')
SCRIPT, SETTINGS, BAD = combat.SCRIPT, combat.SETTINGS, combat.BAD
ORDERS = '60 noise 900; 80 deploy AMCV; 400 where GACNST; 420 move MTNK 3 3; 600 rects'


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
    shutil.rmtree(a.work, ignore_errors=True)
    os.makedirs(os.path.join(a.work, 'w'))
    with open(os.path.join(a.work, 'w', 'RA2MD.INI'), 'w') as f:
        f.write(SETTINGS)
    script = os.path.join(a.work, 'graphics.script')
    with open(script, 'w') as f:
        f.write(SCRIPT)
    env = dict(os.environ, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_MSG_LOG='0',
               RA2_NATIVE_ORDERS=ORDERS)
    env.pop('RA2_NATIVE_FORCE_END', None)
    cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=never', '--run-seconds=%d' % a.seconds,
           '--write-dir=' + os.path.join(a.work, 'w'), '--input-script=' + script, '--game-args=-noaudio -CD.']
    with open(os.path.join(a.work, 'run.log'), 'w') as log:
        game_process.run(cmd, env=env, stdout=log, stderr=subprocess.STDOUT, timeout=a.seconds + 120)
    text = open(os.path.join(a.work, 'run.log'), errors='replace').read()
    failures = []
    for verb, unit in (('deploy', 'AMCV'), ('move', 'MTNK')):
        if not re.search(r'^harness-%s %s &' % (verb, unit), text, re.M):
            failures.append('order "%s %s" found no unit' % (verb, unit))
    if not re.search(r'^harness-where GACNST px', text, re.M):
        failures.append('no Construction Yard on the view after the deploy')
    peak = re.search(r'^harness-noise-max (\d+) over (\d+) frames', text, re.M)
    noisy = re.findall(r'^harness-noise (\d+) at frame (\d+)', text, re.M)
    if not peak:
        failures.append('the noise watch did not finish (the game ended or stopped early)')
    if noisy:
        failures.append('%d frame(s) drew noise (first: %s pixels at frame %s)' % (len(noisy), noisy[0][0], noisy[0][1]))
    rects = re.search(r'^harness-rects tactical (-?\d+),(-?\d+) (\d+)x(\d+) visible (\d+)x(\d+) commandbar (\d+),(\d+) (\d+)x(\d+)', text, re.M)
    if not rects:
        failures.append('no layout report (harness rects)')
    else:
        ty, th, vw, vh, by, bh = (int(rects.group(i)) for i in (2, 4, 5, 6, 8, 10))
        if bh != 32 or by != vh - 32 or ty + th != by:
            failures.append('layout: tactical %s, command bar %s' % (rects.group(0).split(' visible')[0], rects.group(0).split('commandbar ')[1]))
    shown = re.search(r'^harness-rects commandbar on screen (-?\d+)% drawn', text, re.M)
    drawn = int(shown.group(1)) if shown else None
    if drawn is None:
        failures.append('no command bar measure (harness rects)')
    elif drawn < 10:
        failures.append('the command bar is not drawn (%d%% of its rows not black)' % drawn)
    bad = sorted(set(l for l in text.splitlines() if BAD.search(l)))
    if bad:
        failures.append('traps or faults: %s' % '; '.join(bad[:5]))
    for f in failures:
        print('FAIL: ' + f)
    print('%s: MCV deploy and first Grizzly move, noise peak %s over %s frames; command bar %s (%s)' % (
        'FAIL' if failures else 'OK', peak.group(1) if peak else '?', peak.group(2) if peak else '?',
        '%s%% drawn' % drawn, a.work))
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
