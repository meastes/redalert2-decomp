#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest yr-native-zoom-abort (lane N21; built with -DRA2_NATIVE_YR=ON): Abort Mission with Game
interface size 1.5x and Zoom gestures on (render scale 2; the user's Duo report of 2026-11-20), on the
Mac through the real menus with scripted input:

  main menu -> Single Player -> Skirmish -> Start Game -> about 12 s of play -> Escape (the in-game
  menu, laid out at the window's size) -> Abort Mission -> the question's Abort -> the skirmish setup.

Every picture the window shows (RA2_NATIVE_WINDOW_DUMP_MS=0, each numbered in the log:
RA2_NATIVE_WINDOW_DUMP_LOG) is checked as it is written: none written after the in-game menu ends ("zoom:
in-play dialogs end") may be the game's screen shown whole (the zoomed view in the screen's top left corner, the
sidebar column black: the user's report, "the map drawn small in the top-left corner, the rest
black"). Main_Loop returning true ends the play layout and clears a zoomed game's last frame
(glue/yr_app_hooks.cpp yr_native_main_loop -> glue/yr_map_zoom.cpp Yr_Map_Zoom_End, and native fixups
before Do_Abort and Do_Restart for the ends the in-game menu asks for directly); before lane N21 only
the win and loss cleared it, and the abort showed it whole through Do_Abort's wait for EVA. Also checked: the render scale was 2, the run reaches
the time limit without a trap or a fault, and the skirmish setup is shown again.
Skipped (77) without the installed game (gamemd.exe in --data-dir).

  yr_zoom_abort.py --binary <yr-native> --data-dir <game folder> --work <dir>
"""
import argparse
import os
import re
import shutil
import struct
import subprocess
import sys
import threading
import time

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', 'tests'))
import game_process  # noqa: E402  (native/tests: the game's process group, lane C19)

# The menus at 800x600 (yr_skirmish_smoke.py): Single Player, Skirmish, Start Game; in the game Escape,
# Abort Mission (the in-game menu's fifth button), the question's Abort (its first).
SCRIPT = """\
s1+1000 click 720 220
s2+1000 click 720 304
s3+2000 click 720 262
s3+3000 move 316 300
s3+15000 key Escape
s4+2000 click 715 339
s5+2000 click 715 239
"""


def regions(path):
    """The mean brightness (0-765) of a BMP picture's map part (its left 60% by top 60%) and of its
    sidebar column (its right 18%), sampled every 12 pixels; None for an unreadable file."""
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

    def mean(x0, x1, y0, y1):
        total = n = 0
        for y in range(y0, y1, 12):
            line = (rows - 1 - y) * row if h > 0 else y * row
            for x in range(x0, x1, 12):
                i = off + line + x * step
                total += b[i] + b[i + 1] + b[i + 2]
                n += 1
        return total // max(n, 1)
    return mean(0, int(w * 0.6), 0, int(rows * 0.6)), mean(int(w * 0.82), w, 0, rows)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--work', required=True)
    ap.add_argument('--seconds', type=int, default=70)
    ap.add_argument('--enhancements', default='uisize=1.5,zoomgestures=on')
    a = ap.parse_args()
    if not a.data_dir or not os.path.exists(os.path.join(a.data_dir, 'gamemd.exe')):
        print('skipped: no Yuri\'s Revenge install (gamemd.exe) in --data-dir')
        return 77
    shutil.rmtree(a.work, ignore_errors=True)
    dumps = os.path.join(a.work, 'shots')
    os.makedirs(os.path.join(a.work, 'w'))
    os.makedirs(dumps)
    script = os.path.join(a.work, 'abort.script')
    with open(script, 'w') as f:
        f.write(SCRIPT)
    env = dict(os.environ, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_DIALOG_LOG='1',
               RA2_NATIVE_SETTINGS=os.path.join(a.work, 'settings'), RA2_NATIVE_ENHANCEMENTS=a.enhancements,
               RA2_NATIVE_WINDOW_DUMP=os.path.join(dumps, 'w'), RA2_NATIVE_WINDOW_DUMP_MS='0',
               RA2_NATIVE_WINDOW_DUMP_LOG='1')
    cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=never', '--run-seconds=%d' % a.seconds,
           '--write-dir=' + os.path.join(a.work, 'w'), '--input-script=' + script, '--game-args=-noaudio -CD.']
    log_path = os.path.join(a.work, 'run.log')
    whole = []
    seen = 0
    with open(log_path, 'w') as log:
        proc = game_process.Popen(cmd, env=env, stdout=log, stderr=subprocess.STDOUT)
        deadline = time.monotonic() + a.seconds + 120
        try:
            while True:
                running = proc.poll() is None and time.monotonic() < deadline
                # Each picture once the next exists (so it is complete), or all once the game ended.
                names = sorted(os.listdir(dumps), key=lambda n: int(n.split('.')[1]))
                for n in names[:-1] if running else names:
                    path = os.path.join(dumps, n)
                    r = regions(path)
                    if r and r[0] > 60 and r[1] < 10:
                        whole.append((n, r))
                        if len(whole) <= 12:
                            shutil.copy(path, os.path.join(a.work, 'whole-' + n))
                    os.remove(path)
                    seen += 1
                if not running:
                    break
                time.sleep(0.05)
        finally:
            proc.end()
    text = open(log_path, errors='replace').read()
    failures = []
    # Only the pictures after the in-game menu ends count (its first frames, before the menu draws, show
    # the game's frame at the screen's top left too: a frame or two, not this report).
    end = text.rfind('zoom: in-play dialogs end')
    after = set(re.findall(r'^host: window dump (\d+)', text[end:], re.M)) if end >= 0 else set()
    if end < 0:
        failures.append('the in-game menu did not end ("zoom: in-play dialogs end")')
    whole = [(n, r) for n, r in whole if n.split('.')[1] in after]
    stop = [l for l in text.splitlines() if 'stopped:' in l]
    if not stop or 'time limit' not in stop[-1]:
        failures.append('the run stopped early: %s' % (stop[-1] if stop else 'no stop line'))
    if 'zoomgestures=on' in a.enhancements and "zoom: render scale 2: the game's screen" not in text:
        failures.append('the game\'s mode was not made at render scale 2')
    shown = [l for l in text.splitlines() if l.startswith('dialog shown #')]
    if len(shown) < 6:
        failures.append('the skirmish setup was not shown again after the abort (%d dialogs shown)' % len(shown))
    for n, r in whole[:5]:
        print('picture %s: map part %d, sidebar column %d (the game\'s screen shown whole)' % (n, r[0], r[1]))
    if whole:
        failures.append('%d pictures show the game\'s screen whole after play (the map in a corner, the sidebar '
                        'column black)' % len(whole))
    if seen < 100:
        failures.append('too few pictures checked (%d)' % seen)
    for f in failures:
        print('FAIL: ' + f)
    print('%s: %d pictures checked, %d dialogs shown (%s)' % ('FAIL' if failures else 'OK', seen, len(shown), a.work))
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
