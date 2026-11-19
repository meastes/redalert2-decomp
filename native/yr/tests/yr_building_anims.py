#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest yr-native-building-anims (lane N4; built with -DRA2_NATIVE_YR=ON): an oil derrick draws its pump and its
flame, and building animations in general are drawn (the user's Duo fault of 2026-11-19: oil derricks were
missing some of their graphics).

The cause was data, not drawing code: the layer names (LayerName, gamemd.exe 0x0081DA78: "Underground", "Surface",
"Ground", "Air", "Top") point at strings just after Powerups::Anims (0x0081DAD8). native/yr/tools/yr_data.py read
Powerups::Anims[19]'s declaration from builtype.h's `AnimControlType Anims[...]` (members of `namespace Powerups`
were not indexed as Powerups::), so its storage was not loaded and was sized to the next known global (11,768
bytes): the layer names' pointers resolved into that zeroed storage, every name read as "", and every animation
type with a Layer= key (all building animations: art says Layer=ground) got LAYER_NONE. Such an animation is put
in no display layer and never drawn. yr_data.py now indexes a namespace's members under its name.

Through the menus into a skirmish (yr_standins.py's route), harness orders (glue/yr_harness.cpp):
  frame 60   ART(MD).INI's Layer= of CAOILD_A read as a layer name (`artkey`): 2, ground;
             an oil derrick (CAOILD) and a power plant (GAPOWR) spawned for the player;
  frame 300  `banims`: the derrick's pump (CAOILD_A) and flame (CAOILD_F) and the power plant's animation are
             playing and each is in the ground display layer (in-layer 2; the broken build: -1);
  The pump's and flame's pictures on the screen were checked by hand from screen dumps (RA2_NATIVE_PRESENT_DUMP):
  the pumpjack moves and the flare burns, where the broken build drew the derrick's body alone.

  yr_building_anims.py --binary <yr-native> --data-dir <game folder> --work <dir>
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
ORDERS = '60 artkey CAOILD_A Layer; 60 spawn CAOILD 4 -2; 61 spawn GAPOWR 0 4; 300 banims CAOILD; 300 banims GAPOWR'
LAYER_OF = re.compile(r'^harness-layer-of (\S+) = (-?\d+)$', re.M)
BANIM = re.compile(r'^harness-banim (\S+) (\d+) (\S+) live (\d) .* in-layer (-?\d+) ', re.M)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--work', required=True)
    ap.add_argument('--seconds', type=int, default=40)
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
    script = os.path.join(a.work, 'anims.script')
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
    layer = LAYER_OF.search(text)
    anims = {(m.group(1), m.group(3)): (int(m.group(4)), int(m.group(5))) for m in BANIM.finditer(text)}
    print('Layer=%s read as %s; animations (live, in-layer): %s' % (layer and layer.group(1), layer and layer.group(2), anims))
    if not layer or layer.group(1).lower() != 'ground' or layer.group(2) != '2':
        failures.append('CAOILD_A\'s Layer= did not read as the ground layer: %s' % (layer and layer.groups(),))
    for key in (('CAOILD', 'CAOILD_A'), ('CAOILD', 'CAOILD_F'), ('GAPOWR', 'GAPOWR_A')):
        if key not in anims:
            failures.append('%s has no %s animation (the building is missing, or the game stopped early)' % key)
        elif anims[key] != (1, 2):
            failures.append('%s\'s %s is not playing in the ground layer (live, in-layer: %s)' % (key[0], key[1], anims[key]))
    bad = sorted(set(l for l in text.splitlines() if standins.BAD.search(l)))
    if bad:
        failures.append('traps or faults: %s' % '; '.join(bad[:5]))
    for f in failures:
        print('FAIL: ' + f)
    print('%s: building animations are drawn (%s)' % ('FAIL' if failures else 'OK', a.work))
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
