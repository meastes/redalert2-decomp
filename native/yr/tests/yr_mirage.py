#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest yr-native-mirage (lane N4; built with -DRA2_NATIVE_YR=ON): a Mirage tank's camouflage draws as a tree in
the tree's colours (the user's Duo fault of 2026-11-19: the camouflage drew as corrupted graphics).

A Mirage tank (MGTK, DisguiseWhenStill) standing still takes a tree as its disguise; its owner sees the tree for
part of every blink cycle (TechnoClass::IsClearlyVisibleTo false), others while they do not sense it. The cause:
TechnoClass::Techno_Draw_Object (glue/techno_native.inc, a NON-EXACT draft from TibSun's text) drew the disguise's
shape through the Mirage's house colour scheme, so the tree came out as blue and white speckles. gamemd.exe
(0x00706061) draws a disguise as a tree or an overlay through its cell's lighting converter, and a disguise as
another house's unit through that house's colours; the draft now does the same.

Through the menus into a skirmish (yr_standins.py's route), harness orders (glue/yr_harness.cpp):
  frame 60       a Mirage tank spawned for the player near the base;
  frames 100-900 `camo MGTK 800`: on each frame the player is shown the disguise, the leaf-green and the blue/white
                 pixels in a box above the tank in the composed view are counted (the broken draw: about 85 green,
                 615 blue; the fixed one about 730 green, 5 blue);
  frame 900      the tank is still disguised as a tree.
Also `noise 800` (yr-native-graphics' watch for 16-bit pixels in the eight-bit compositing surface).

  yr_mirage.py --binary <yr-native> --data-dir <game folder> --work <dir>
Skipped (77) without the installed game (gamemd.exe in --data-dir) and for the Windows build.
"""
import argparse
import importlib
import os
import re
import shutil
import subprocess
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', 'tests'))
import game_process  # noqa: E402

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
standins = importlib.import_module('yr_standins')
ORDERS = '60 spawn MGTK 3 0; 100 camo MGTK 800; 100 noise 800; 900 disguise MGTK'
CAMO = re.compile(r'^harness-camo seen (\d+) green (\d+) blue (\d+)$', re.M)
DISGUISE = re.compile(r'^harness-disguise MGTK (\d) as (\S+) r(-?\d+) clear (\d)$', re.M)
NOISE = re.compile(r'^harness-noise-max (\d+) over', re.M)


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
    if a.binary.lower().endswith('.exe'):
        print('skipped: run on the Mac build (the Windows build has the same game code)')
        return 77
    shutil.rmtree(a.work, ignore_errors=True)
    os.makedirs(os.path.join(a.work, 'w'))
    with open(os.path.join(a.work, 'w', 'RA2MD.INI'), 'w') as f:
        f.write(standins.SETTINGS)
    script = os.path.join(a.work, 'mirage.script')
    with open(script, 'w') as f:
        f.write(standins.SCRIPT)
    env = dict(os.environ, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_MSG_LOG='0',
               RA2_NATIVE_ORDERS=ORDERS)
    env.pop('RA2_NATIVE_FORCE_END', None)
    env.pop('RA2_NATIVE_TRAP_CONTINUE', None)
    cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=never', '--run-seconds=%d' % a.seconds,
           '--write-dir=' + os.path.join(a.work, 'w'), '--input-script=' + script, '--game-args=-noaudio -CD.']
    with open(os.path.join(a.work, 'run.log'), 'w') as log:
        game_process.run(cmd, env=env, stdout=log, stderr=subprocess.STDOUT, timeout=a.seconds + 120)
    text = open(os.path.join(a.work, 'run.log'), errors='replace').read()
    failures = []
    camo = CAMO.search(text)
    disguise = DISGUISE.search(text)
    noise = NOISE.search(text)
    print('camo: %s; disguise at 900: %s; noise max: %s' % (camo and camo.groups(), disguise and disguise.groups(),
                                                            noise and noise.group(1)))
    if not camo:
        failures.append('the camouflage watch did not finish (no Mirage tank, or the game stopped early)')
    else:
        seen, green, blue = map(int, camo.groups())
        if seen < 40:
            failures.append('the disguise was shown on only %d frames' % seen)
        elif green < 200 or green < 3 * blue:
            failures.append('the camouflage is not drawn in a tree\'s colours: %d green, %d blue/white pixels' % (green, blue))
    if not disguise or disguise.group(1) != '1' or disguise.group(3) != '37':
        failures.append('the Mirage tank is not disguised as a tree at frame 900: %s' % (disguise and disguise.groups(),))
    if noise and int(noise.group(1)) > 60:
        failures.append('noise in the composed view: %s pixels' % noise.group(1))
    bad = sorted(set(l for l in text.splitlines() if standins.BAD.search(l)))
    if bad:
        failures.append('traps or faults: %s' % '; '.join(bad[:5]))
    for f in failures:
        print('FAIL: ' + f)
    print('%s: a Mirage tank\'s camouflage draws as a tree (%s)' % ('FAIL' if failures else 'OK', a.work))
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
