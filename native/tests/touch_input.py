#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""Touch input regressions (ctest native-touch-two-finger, native-touch-attack-move; lane C8).
A skirmish ('#5') with the touch controls on (RA2_NATIVE_TOUCH_UI=1) played by scripted fingers
(input script tdown/tup, the touch gestures' own path: platform/sdl/touch.cpp, host.cpp), checked
by game state in the harness-input log (link/harness_input.cpp):

  two-finger   a tank tapped (selected 1), then a two-finger tap on the ground: selected 0
               (the right click deselects: DisplayClass::Mouse_Right_Release 0x0049AF90).
  attack-move  a tank tapped, the toolbar's A-Move (item 3, as its button sends it) turned on, then
               a tap on the ground: the tank has the attack-move order pending (FootClass +0x4B4 =
               0x1D, ':p29') and is moving (frame 250); it arrives (frame 400: guarding, no order).
               A-Move stays on (lane C10: a toggle like Fire, Move and Add), so a second tap on
               another spot is a second attack-move (frame 450: ':p29' again, moving). A-Move
               tapped again turns it off, and the next tap is a plain move (frame 650: moving, no
               pending order). Each order gets 200 frames (they arrive in about 100; the skirmish's
               start positions vary between runs).

Both failed when a touch's press reached the game before its mouse thread had read the pointer's
new position (host.cpp kPointerSeenMs).

  touch_input.py --case two-finger|attack-move --binary <ra2-native> --data-dir <installed RA2>
                 --game-exe <game.exe> [--log <file>]

Without the data folder or game.exe it exits 77 (skipped), as native-census; so does the Windows
build (no touch host there yet)."""
import argparse
import os
import re
import subprocess
import sys
import tempfile

SKIP = 77
BAD = re.compile(r'unrecovered|trap|signal \d+|SIG(SEGV|BUS|ILL|ABRT|FPE)|crash:|abort', re.I)
STATE = re.compile(r'^run-to: reached (harness-input(?:-(\d+))?) \+\d+ ms\s+selected (\d+);(.*)$')

# Frame-numbered script lines; @MTNK is the player's first tank (harness_input.cpp).
SCRIPTS = {
    'two-finger': ['100 tdown @MTNK', '100 tup @MTNK',
                   '200 tdown 300 300 1', '200 tdown 360 320 2',
                   '201 tup 300 300 1', '201 tup 360 320 2'],
    'attack-move': ['100 tdown @MTNK', '100 tup @MTNK',
                    '200 toolbar 3',
                    '205 tdown @MTNK+6,2', '205 tup @MTNK+6,2',
                    '405 tdown @MTNK-4,4', '405 tup @MTNK-4,4',
                    '605 toolbar 3',
                    '610 tdown @MTNK+4,-3', '610 tup @MTNK+4,-3'],
}
# The run's last frame per case (each tank order takes under 100 frames to arrive).
FRAMES = {'two-finger': 240, 'attack-move': 660}
TANK = re.compile(r' MTNK\*:m(\d+)@([-\d]+),([-\d]+)(?::p(-?\d+))?')


def tank(state):
    """(mission, cell, pending order or None) of the selected tank, or None"""
    m = TANK.search(state[1]) if state else None
    return (int(m.group(1)), (int(m.group(2)), int(m.group(3))), m.group(4)) if m else None


def attack_move_problems(st):
    """The attack-move case's checks (MISSION_MOVE 2, MISSION_GUARD 5; order 29 attack-move)."""
    want = [(250, 'first attack-move', lambda t: t[2] == '29' and t[0] == 2),
            (400, 'first attack-move arrived', lambda t: t[2] is None and t[0] == 5),
            (450, 'second attack-move', lambda t: t[2] == '29' and t[0] == 2),
            (650, 'plain move after A-Move off', lambda t: t[2] is None and t[0] == 2)]
    problems = []
    for frame, what, ok in want:
        t = tank(st.get(frame))
        print('frame %d (%s): %s' % (frame, what, t))
        if t is None:
            problems.append('no selected tank at frame %d' % frame)
        elif not ok(t):
            problems.append('frame %d: not the %s (mission %d, order %s)' % (frame, what, t[0], t[2]))
    return problems


def states(out):
    """frame (None for the last) -> (selected count, objects text)"""
    found = {}
    for line in out.splitlines():
        m = STATE.match(line)
        if m:
            found[int(m.group(2)) if m.group(2) else None] = (int(m.group(3)), m.group(4))
    return found


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--case', required=True, choices=sorted(SCRIPTS))
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default=os.environ.get('RA2_NATIVE_CENSUS_DATA_DIR', ''))
    ap.add_argument('--game-exe', default=os.environ.get('RA2_NATIVE_CENSUS_GAME_EXE', ''))
    ap.add_argument('--frames', type=int, default=0)   # 0: the case's own (FRAMES)
    ap.add_argument('--seconds', type=int, default=300)
    ap.add_argument('--log', default='')
    a = ap.parse_args()
    name = 'native-touch-' + a.case
    if not a.data_dir or not os.path.isdir(a.data_dir) or not a.game_exe or not os.path.isfile(a.game_exe):
        print('%s: skipped (set RA2_NATIVE_CENSUS_DATA_DIR and RA2_NATIVE_CENSUS_GAME_EXE)' % name)
        return SKIP
    if a.binary.lower().endswith('.exe'):
        print('%s: skipped (the Windows build has no touch host yet)' % name)
        return SKIP
    env = dict(os.environ, RA2_GAME_EXE=a.game_exe, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_CRASH_TRACE='1',
               RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_TOUCH_UI='1', RA2_NATIVE_INPUT_LOG='50')
    env.pop('RA2_NATIVE_TRAP_CONTINUE', None)
    with tempfile.TemporaryDirectory(prefix='ra2-touch-') as work:
        script = os.path.join(work, 'input.script')
        with open(script, 'w') as f:
            f.write('\n'.join(SCRIPTS[a.case]) + '\n')
        write_dir = os.path.join(work, 'write')
        os.mkdir(write_dir)
        cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=frames', '--run-seconds=%d' % a.seconds,
               '--write-dir=' + write_dir, '--game-args=-noaudio -CD.', '--skirmish=#5',
               '--skirmish-frames=%d' % (a.frames or FRAMES[a.case]), '--input-script=' + script]
        p = subprocess.run(cmd, env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, errors='replace')
    out = p.stdout
    if a.log:
        with open(a.log, 'w') as f:
            f.write(out)
    bad = [l for l in out.splitlines() if BAD.search(l)]
    st = states(out)
    before, last = st.get(200), st.get(None)
    problems = []
    if p.returncode != 0 or bad:
        problems.append('exit %d, %d bad lines' % (p.returncode, len(bad)))
    if before is None or last is None:
        problems.append('no harness-input state at frame 200 and at the end')
    else:
        print('frame 200: selected %d;%s' % before)
        print('end:       selected %d;%s' % last)
        if before[0] != 1:
            problems.append('the tank tap selected %d objects, not 1' % before[0])
        elif a.case == 'two-finger':
            if last[0] != 0:
                problems.append('after the two-finger tap %d objects are still selected' % last[0])
        else:
            problems += attack_move_problems(st)
    for l in bad[:20]:
        print('  ' + l)
    if problems:
        print('%s: FAILED (%s)' % (name, '; '.join(problems)))
        return 1
    print('%s: passed' % name)
    return 0


if __name__ == '__main__':
    sys.exit(main())
