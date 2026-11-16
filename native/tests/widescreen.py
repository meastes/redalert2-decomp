#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest native-widescreen (lane Y8, user report 2026-11-16): the Mac app's game mode follows its
window's aspect (link/native_widescreen.cpp, host.cpp ra2_host_widescreen_target), as the iOS app's
follows the screen. Three runs of ra2-native to the main menu and into a skirmish, the window set
after it opens (RA2_NATIVE_WINDOW_SIZE, as a player's resize leaves it):

  16:9  RA2_NATIVE_WIDESCREEN=1, window 1280x720 points: the mode changes in the menus to 1064x600
        (600 lines, the width by the aspect), the menus are laid out again, the skirmish plays at it;
  4:3   RA2_NATIVE_WIDESCREEN=1, window 1024x768: no change (the original's 800x600 frames);
  off   no RA2_NATIVE_WIDESCREEN (test runs and the iOS app): 1280x720 and no change.

Checks: each run reaches its time limit without a trap or a fault; the widescreen lines and the
surfaces made (the run's summary) are as above; the 16:9 run reached the skirmish's game frames.
Needs the installed game (as native-census): skipped (77) without --data-dir/--game-exe.

  widescreen.py --binary <ra2-native> --data-dir <game folder> --game-exe <game.exe> --work <dir>
"""
import argparse
import os
import re
import shutil
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import game_process  # noqa: E402

# Main menu -> Single Player -> Skirmish -> Start Game; the menus centred on the wider screen.
def script(width):
    mx = (width - 800) // 2
    return ('s1+2500 click %d 220\ns2+1500 click %d 304\ns3+2500 click %d 262\n' % (mx + 720, mx + 720, mx + 720))


def run(a, name, size, widescreen, width):
    work = os.path.join(a.work, name)
    shutil.rmtree(work, ignore_errors=True)
    os.makedirs(os.path.join(work, 'write'))
    # The player's mode: 800x600 (600 lines), whatever the installation's RA2.INI says.
    with open(os.path.join(work, 'write', 'RA2.INI'), 'w') as f:
        f.write('[Video]\r\nScreenWidth=800\r\nScreenHeight=600\r\n')
    path = os.path.join(work, 'input.script')
    with open(path, 'w') as f:
        f.write(script(width))
    env = dict(os.environ, RA2_GAME_EXE=a.game_exe, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_IGNORE_FOCUS='1',
               RA2_NATIVE_DIALOG_LOG='1', RA2_NATIVE_WINDOW_SIZE=size,
               RA2_NATIVE_WINDOW_DUMP=os.path.join(work, 'win'), RA2_NATIVE_WINDOW_DUMP_MS='5000')
    env.pop('RA2_NATIVE_ENHANCEMENTS', None)
    env.pop('RA2_NATIVE_WIDESCREEN', None)
    if widescreen:
        env['RA2_NATIVE_WIDESCREEN'] = '1'
    cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=never', '--run-seconds=%d' % a.seconds,
           '--write-dir=' + os.path.join(work, 'write'), '--game-args=-noaudio', '--input-script=' + path]
    with open(os.path.join(work, 'run.log'), 'w') as log:
        game_process.run(cmd, env=env, stdout=log, stderr=subprocess.STDOUT, timeout=a.seconds + 90)
    return open(os.path.join(work, 'run.log'), errors='replace').read()


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--game-exe', default='')
    ap.add_argument('--work', required=True)
    ap.add_argument('--seconds', type=int, default=60)
    a = ap.parse_args()
    if not a.data_dir or not a.game_exe or not os.path.exists(a.game_exe):
        print('skipped: no installed game (--data-dir/--game-exe)')
        return 77
    failures = []
    for name, size, wide, width, want in (('16x9', '1280x720', True, 1064, '1064x600'),
                                          ('4x3', '1024x768', True, 800, None),
                                          ('off', '1280x720', False, 800, None)):
        text = run(a, name, size, wide, width)
        stop = [l for l in text.splitlines() if 'stopped:' in l]
        changes = re.findall(r'host: widescreen: window (\d+x\d+) points, (\d+) lines: (\d+x\d+)', text)
        surfaces = re.findall(r'surfaces created: .*', text)
        frames = re.search(r'frames run (\d+)', text)
        print('%s: widescreen %s, %s, frames %s, %s' % (name, changes or 'none', surfaces[-1][:120] if surfaces else 'no surfaces line',
                                                    frames.group(1) if frames else '?', stop[-1] if stop else 'no stop line'))
        if not stop or 'time limit' not in stop[-1]:
            failures.append('%s: the run stopped early: %s' % (name, stop[-1] if stop else 'no stop line'))
        if want:
            if not changes or changes[-1][2] != want:
                failures.append('%s: the mode did not follow the window (%s)' % (name, changes))
            if not surfaces or (want + ' 16 bpp') not in surfaces[-1]:
                failures.append('%s: no %s surface made' % (name, want))
            if not frames or int(frames.group(1)) < 100:
                failures.append('%s: the skirmish did not play at the new mode' % name)
        elif changes:
            failures.append('%s: the mode changed (%s)' % (name, changes))
    for f in failures:
        print('FAIL: ' + f)
    print('%s (%s)' % ('FAIL' if failures else 'OK', a.work))
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
