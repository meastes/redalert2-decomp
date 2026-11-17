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

  modern       Controls: Modern (lane C16): a tap selects a tank, a two-finger tap on the ground
               moves it (still selected), a tap on the ground deselects, A-Move's tap still orders.

  second-classic  Controls: Classic (the original's clicks; C18, Duo report: a tap on another of
                  the player's units moved the selected one): a tap selects the first tank, a tap
                  on the second tank selects it instead (the first tank unselected and not
                  ordered: still guarding), a tap on the first tank selects it again.
  second-modern   The same taps with Controls: Modern.
  cross-classic   Allied mission 2 from the original's own save at its start (ALL02S.sav in the
                  data folder; skipped without it), Controls: Classic: Tanya there is the Germans',
                  a house of her own under the player's control (C18, the user's Duo report: "it
                  keeps moving the current unit instead"). A tap selects a GI; a tap on Tanya
                  selects her (the original's click moves the GI there); a tap on the second GI
                  selects it (the original moves Tanya); a tap on Tanya selects her again. No GI
                  is ordered.
  cross-modern    The same taps with Controls: Modern.
  cross-mouse     The same as mouse clicks with Classic: the original's clicks (the GI selected
                  moves to Tanya and stays selected): the touch rule leaves a mouse alone.

  touch_input.py --case two-finger|attack-move|modern|second-classic|second-modern|cross-classic|cross-modern|cross-mouse --binary <ra2-native> --data-dir <installed RA2>
                 --game-exe <game.exe> [--log <file>]

Without the data folder or game.exe it exits 77 (skipped), as native-census; so does the Windows
build (no touch host there yet)."""
import argparse
import os
import re
import shutil
import subprocess
import sys
import tempfile

import game_process

SKIP = 77
BAD = re.compile(r'unrecovered|trap|signal \d+|SIG(SEGV|BUS|ILL|ABRT|FPE)|crash:|abort', re.I)
STATE = re.compile(r'^run-to: reached (harness-input(?:-(\d+))?) \+\d+ ms\s+selected (\d+)([^;]*);(.*)$')

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
    # Controls: Modern (lane C16; RA2_NATIVE_ENHANCEMENTS=controls=modern): a tap selects, a
    # two-finger tap (a right click) on the ground orders the move and keeps the selection, a tap
    # on the ground deselects without an order; A-Move (Ctrl and Shift held) keeps its taps as
    # orders.
    'modern': ['100 tdown @MTNK', '100 tup @MTNK',
               '205 tdown @MTNK+6,2 1', '205 tdown @MTNK+6,3 2', '206 tup @MTNK+6,2 1', '206 tup @MTNK+6,3 2',
               '380 tdown @MTNK-3,3', '380 tup @MTNK-3,3',
               '455 tdown @MTNK', '455 tup @MTNK',
               '505 toolbar 3',
               '510 tdown @MTNK-4,4', '510 tup @MTNK-4,4',
               '600 toolbar 3'],
    # A tap on a second tank with the first selected (C18): selects it (Classic: the original's
    # select action on another of the player's units; Modern: a tap always selects).
    'second-classic': ['100 tdown @MTNK', '100 tup @MTNK',
                       '200 tdown @MTNK:1', '200 tup @MTNK:1',
                       '300 tdown @MTNK', '300 tup @MTNK'],
}
SCRIPTS['second-modern'] = SCRIPTS['second-classic']
# Allied mission 2's start (C18): '@E1' is the player's first GI, '@TANY' Tanya (the Germans').
SCRIPTS['cross-classic'] = ['20 tdown @E1', '20 tup @E1', '60 tdown @TANY', '60 tup @TANY',
                            '100 tdown @E1:1', '100 tup @E1:1', '140 tdown @TANY', '140 tup @TANY']
SCRIPTS['cross-modern'] = SCRIPTS['cross-classic']
SCRIPTS['cross-mouse'] = ['20 click @E1', '60 click @TANY']
# Cases that load the original's own save of a mission instead of a skirmish (C18).
SAVES = {'cross-classic': 'ALL02S.sav', 'cross-modern': 'ALL02S.sav', 'cross-mouse': 'ALL02S.sav'}
# The run's last frame per case (each tank order takes under 100 frames to arrive).
FRAMES = {'two-finger': 240, 'attack-move': 660, 'modern': 620, 'second-classic': 380, 'second-modern': 380,
          'cross-classic': 180, 'cross-modern': 180, 'cross-mouse': 120}
# Extra environment per case.
ENV = {'modern': {'RA2_NATIVE_ENHANCEMENTS': 'controls=modern'},
       'second-classic': {'RA2_NATIVE_ENHANCEMENTS': 'controls=classic'},
       'second-modern': {'RA2_NATIVE_ENHANCEMENTS': 'controls=modern'},
       'cross-classic': {'RA2_NATIVE_ENHANCEMENTS': 'controls=classic'},
       'cross-modern': {'RA2_NATIVE_ENHANCEMENTS': 'controls=modern'},
       'cross-mouse': {'RA2_NATIVE_ENHANCEMENTS': 'controls=classic'}}
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


def modern_problems(st):
    """The Modern case's checks (MISSION_MOVE 2, MISSION_GUARD 5; order 29 attack-move)."""
    want = [(250, 'two-finger tap: move, still selected', lambda s, t: s == 1 and t is not None and t[0] == 2),
            (450, 'tap on the ground: nothing selected', lambda s, t: s == 0),
            (500, 'tap on the tank selects it again', lambda s, t: s == 1 and t is not None),
            (550, 'A-Move tap: an attack-move', lambda s, t: s == 1 and t is not None and t[2] == '29' and t[0] == 2)]
    problems = []
    for frame, what, ok in want:
        state = st.get(frame)
        t = tank(state)
        print('frame %d (%s): selected %s, tank %s' % (frame, what, state[0] if state else None, t))
        if state is None or not ok(state[0], t):
            problems.append('frame %d: not %s' % (frame, what))
    return problems


def tanks(state):
    """[(selected, mission)] of the player's tanks in the harness-input order (@MTNK first)"""
    return [(m.group(1) == '*', int(m.group(2))) for m in re.finditer(r' MTNK(\*?):m(-?\d+)@', state[1])] if state else []


def second_problems(st):
    """The second-unit cases' checks: which tank is selected, and that the taps ordered nothing
    (MISSION_GUARD 5 for both; a move would be MISSION_MOVE 2). Each step's window allows for
    the tap's click arriving a few frames late (the tap waits for a second finger)."""
    want = [((120, 195), 'the first tank selected', [True, False]),
            ((220, 295), 'the second tank selected instead', [False, True]),
            ((320, 380), 'the first tank selected again', [True, False])]
    problems = []
    for (lo, hi), what, chosen in want:
        frames = [f for f in sorted(k for k in st if k is not None) if lo <= f <= hi]
        got = None
        for f in frames:
            t = tanks(st[f])
            got = (f, st[f][0], t)
            if st[f][0] == 1 and len(t) >= 2 and [x[0] for x in t[:2]] == chosen:
                break
        else:
            problems.append('frames %d-%d: not %s (%s)' % (lo, hi, what, got))
        print('frames %d-%d (%s): %s' % (lo, hi, what, got))
    for f in sorted(k for k in st if k is not None):
        t = tanks(st[f])
        if 120 <= f and len(t) >= 2 and (t[0][1] == 2 or t[1][1] == 2):
            problems.append('frame %d: a tank is moving (%s): a tap ordered a move' % (f, t[:2]))
            break
    return problems


def gis(state):
    """[(selected, mission)] of the player's GIs (E1) in the harness-input order"""
    return [(m.group(1) == '*', int(m.group(2))) for m in re.finditer(r' E1(\*?):m(-?\d+)@', state[1])] if state else []


def tanya_selected(state):
    return bool(state) and ' other=TANY/' in state[2]


def cross_problems(st, case):
    """The cross-house cases' checks (the harness logs every 20 frames; a step's result by the
    next step's frame)."""
    def ok_gi(n):
        return lambda state: state[0] == 1 and not tanya_selected(state) and len(gis(state)) > n and gis(state)[n][0]
    tanya = lambda state: state[0] == 1 and tanya_selected(state)
    if case == 'cross-mouse':
        want = [((40, 60), 'the first GI selected', ok_gi(0)),
                ((80, 120), "the original's click: the GI moves to Tanya, still selected",
                 lambda state: ok_gi(0)(state) and gis(state)[0][1] == 2)]
    else:
        want = [((40, 60), 'the first GI selected', ok_gi(0)),
                ((80, 100), 'Tanya selected instead', tanya),
                ((120, 140), 'the second GI selected instead', ok_gi(1)),
                ((160, 180), 'Tanya selected again', tanya)]
    problems = []
    for (lo, hi), what, ok in want:
        frames = [f for f in sorted(k for k in st if k is not None) if lo <= f <= hi]
        good = [f for f in frames if ok(st[f])]
        shown = st[good[0]] if good else (st[frames[-1]] if frames else None)
        print('frames %d-%d (%s): %s' % (lo, hi, what, ('ok at %d' % good[0]) if good else ('no: %s' % (shown[1][:120] if shown else None))))
        if not good:
            problems.append('frames %d-%d: not %s' % (lo, hi, what))
    if case != 'cross-mouse':
        moved = [f for f in sorted(k for k in st if k is not None) if any(g[1] == 2 for g in gis(st[f]))]
        if moved:
            problems.append('frame %d: a GI is moving: a tap ordered a move' % moved[0])
    return problems


def states(out):
    """frame (None for the last) -> (selected count, objects text)"""
    found = {}
    for line in out.splitlines():
        m = STATE.match(line)
        if m:
            # (selected count, the player's objects, the selected objects of other houses)
            found[int(m.group(2)) if m.group(2) else None] = (int(m.group(3)), m.group(5), m.group(4))
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
    ap.add_argument('--enhancements', default='')   # more settings, e.g. uisize=1.25,zoomgestures=on
    a = ap.parse_args()
    name = 'native-touch-' + a.case
    if not a.data_dir or not os.path.isdir(a.data_dir) or not a.game_exe or not os.path.isfile(a.game_exe):
        print('%s: skipped (set RA2_NATIVE_CENSUS_DATA_DIR and RA2_NATIVE_CENSUS_GAME_EXE)' % name)
        return SKIP
    if a.binary.lower().endswith('.exe'):
        print('%s: skipped (the Windows build has no touch host yet)' % name)
        return SKIP
    save = SAVES.get(a.case)
    if save and not os.path.isfile(os.path.join(a.data_dir, save)):
        print('%s: skipped (no %s in the data folder)' % (name, save))
        return SKIP
    env = dict(os.environ, RA2_GAME_EXE=a.game_exe, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_CRASH_TRACE='1',
               RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_TOUCH_UI='1', RA2_NATIVE_INPUT_LOG='20' if save else '50')
    env.pop('RA2_NATIVE_TRAP_CONTINUE', None)
    env.update(ENV.get(a.case, {}))
    if a.enhancements:
        env['RA2_NATIVE_ENHANCEMENTS'] = ','.join(x for x in (env.get('RA2_NATIVE_ENHANCEMENTS', ''), a.enhancements) if x)
    with tempfile.TemporaryDirectory(prefix='ra2-touch-') as work:
        script = os.path.join(work, 'input.script')
        with open(script, 'w') as f:
            f.write('\n'.join(SCRIPTS[a.case]) + '\n')
        write_dir = os.path.join(work, 'write')
        os.mkdir(write_dir)
        cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=frames', '--run-seconds=%d' % a.seconds,
               '--write-dir=' + write_dir, '--game-args=-noaudio -CD.', '--input-script=' + script]
        if save:
            # The mission from the original's own save (copied: the game reads saves from its write folder).
            shutil.copyfile(os.path.join(a.data_dir, save), os.path.join(write_dir, save))
            cmd += ['--load-save=' + save, '--frames=%d' % (a.frames or FRAMES[a.case])]
        else:
            cmd += ['--skirmish=#5', '--skirmish-frames=%d' % (a.frames or FRAMES[a.case])]
        p = game_process.run(cmd, env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, errors='replace')
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
    if save:
        problems += cross_problems(st, a.case)
    elif before is None or last is None:
        problems.append('no harness-input state at frame 200 and at the end')
    else:
        print('frame 200: selected %d;%s' % before[:2])
        print('end:       selected %d;%s' % last[:2])
        if before[0] != 1:
            problems.append('the tank tap selected %d objects, not 1' % before[0])
        elif a.case == 'modern':
            problems += modern_problems(st)
        elif a.case.startswith('second-'):
            problems += second_problems(st)
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
