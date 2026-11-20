#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest yr-native-end-banner (lane N21; built with -DRA2_NATIVE_YR=ON): the end-of-game banner
("Game Over", "Mission Accomplished": GRFXTXT.SHP drawn over the middle of the tactical view by
Tactical::Draw_Screen_Text 0x006D4E20) at the original's brightness, on the Mac through the real menus:

  main menu -> Single Player -> Skirmish -> Start Game (the setup's defaults) -> at game frame 150 the
  player's house flagged to lose (RA2_NATIVE_FORCE_END=150,lose,flag: HouseClass::Flag_To_Lose, as the
  house AI flags a defeat: the "Game Over" banner and EVA) -> a window dump at frame 200, the banner up.

Check: the banner's middle (the 800x600 screen's 220-420 by 250-320, inside the picture) is lit as the
original draws it: its mean brightest channel at least 90 of 255. Init_Game's draft (glue/
init_game_native.inc, gamemd.exe 0x0052C233-0x0052C2AB) left GRFXTXT.PAL's 6-bit colours unscaled, so
the banner was drawn at a quarter of its brightness (mean about 27, drawn right about 120: the user's Duo report of
2026-11-20, "too dark"); gamemd.exe makes them 8-bit as every other palette (and Red Alert 2's
Init_Game does the same). Also checked: the run has no trap or fault and the flag was raised.
Skipped (77) without the installed game (gamemd.exe in --data-dir).

  yr_end_banner.py --binary <yr-native> --data-dir <game folder> --work <dir>
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

SCRIPT = """\
s1+1000 click 720 220
s2+1000 click 720 304
s3+2000 click 720 262
s3+3000 move 700 590
200 dump
"""
BANNER = (220, 250, 420, 320)    # x0, y0, x1, y1 of the 800x600 screen: inside the banner's picture


def banner_light(dump):
    """The mean brightest channel (0-255) over the banner's middle in a window dump."""
    width, height, rows = dump
    sx, sy = width / 800.0, height / 600.0
    x0, y0, x1, y1 = BANNER
    total = n = 0
    for y in range(y0, y1, 2):
        for x in range(x0, x1, 2):
            total += max(rows[min(height - 1, int(y * sy))][min(width - 1, int(x * sx))])
            n += 1
    return total / max(1, n)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--work', required=True)
    ap.add_argument('--seconds', type=int, default=70)
    a = ap.parse_args()
    if not a.data_dir or not os.path.exists(os.path.join(a.data_dir, 'gamemd.exe')):
        print('skipped: no Yuri\'s Revenge install (gamemd.exe) in --data-dir')
        return 77
    shutil.rmtree(a.work, ignore_errors=True)
    os.makedirs(os.path.join(a.work, 'w'))
    os.makedirs(os.path.join(a.work, 'shots'))
    script = os.path.join(a.work, 'banner.script')
    with open(script, 'w') as f:
        f.write(SCRIPT)
    env = dict(os.environ, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_DIALOG_LOG='1',
               RA2_NATIVE_SETTINGS=os.path.join(a.work, 'settings'), RA2_NATIVE_FORCE_END='150,lose,flag',
               RA2_NATIVE_WINDOW_DUMP=os.path.join(a.work, 'shots', 'w'), RA2_NATIVE_WINDOW_DUMP_MS='600000')
    env.pop('RA2_NATIVE_ENHANCEMENTS', None)
    cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=never', '--run-seconds=%d' % a.seconds,
           '--write-dir=' + os.path.join(a.work, 'w'), '--input-script=' + script, '--game-args=-noaudio -CD.']
    log_path = os.path.join(a.work, 'run.log')
    with open(log_path, 'w') as log:
        game_process.run(cmd, env=env, stdout=log, stderr=subprocess.STDOUT, timeout=a.seconds + 120)
    text = open(log_path, errors='replace').read()
    failures = []
    stop = [l for l in text.splitlines() if 'stopped:' in l]
    if not stop or 'time limit' not in stop[-1]:
        failures.append('the run stopped early: %s' % (stop[-1] if stop else 'no stop line'))
    if 'force-end: flagged loss at frame' not in text:
        failures.append('the player\'s house was not flagged to lose')
    dumps = re.findall(r'^host: window dump (\d+)', text, re.M)
    path = os.path.join(a.work, 'shots', 'w.%s.bmp' % dumps[-1]) if dumps else ''
    if not path or not os.path.exists(path):
        failures.append('no window dump with the banner up')
    else:
        light = banner_light(read_bmp(path))
        print('the banner\'s middle: mean brightest channel %.1f (%s)' % (light, path))
        if light < 90:
            failures.append('the banner is drawn too dark (%.1f of 255, drawn right about 120)' % light)
    for f in failures:
        print('FAIL: ' + f)
    print('%s (%s)' % ('FAIL' if failures else 'OK', a.work))
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
