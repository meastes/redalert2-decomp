#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""Control enhancements played by scripted clicks in a skirmish ('#5'), checked by game state
in the harness-input log (link/harness_input.cpp, link/harness_sidebar.cpp); lane C16.

  queue-five      Queue five with Shift on (RA2_NATIVE_ENHANCEMENTS=queuefive=on). The harness
                  builds a power plant and a barracks (--skirmish-order=build, RA2_NATIVE_BUILD_LIST);
                  the infantry tab is clicked, then the GI's build icon with Shift held: five GIs
                  in the barracks' factory (inf=5); a plain click: one more (6); a right click puts
                  the one in production on hold (6); a right click with Shift: five fewer (1).
  queue-five-off  The same clicks with the setting off (the default): Shift changes nothing; one
                  GI per click (1, then 2), the right click holds (2), the Shift right click
                  removes one (1).
  modern          Controls: Modern, with the start units: a left click selects a tank, a left click
                  on the ground deselects it and orders nothing, a right click on a GI neither
                  selects it nor drops the tank, a right click on the ground moves the tank (still
                  selected), Shift-click adds a tank and removes it again, a double click selects
                  every tank, a box selects, the same box with Shift removes what it holds (all
                  selected) and adds it back, a right click on the selected MCV deploys it.
  classic         The same script with Classic (the default): the original's clicks (the ground
                  click moves the tank, a right click deselects, no type selection by double click,
                  the Shift box adds, a right click on the MCV deselects it).

  controls_input.py --case <case> --binary <ra2-native> --data-dir <installed RA2> --game-exe <game.exe>
                    [--log <file>]

Without the data folder or game.exe it exits 77 (skipped), as native-census. A Windows build's
.exe runs under Wine (tests/wine.py) when --wine-prefix is given, else it is skipped."""
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
STATE = re.compile(r'^harness-state (\d+) selected (\d+);(.*)$')

# The barracks stands from about frame 850 (harness-build-7 "placed GAPILE"; 846 in a run on
# 2026-11-14); the clicks start at 900. A GI takes longer than 100 frames to train, and the
# hold at 960 stops it, so the counts below are the queue's alone.
QUEUE_SCRIPT = ['900 click @tab=2',
                '910 keydown Left_Shift', '911 click @cameo=E1', '916 keyup Left_Shift',
                '935 click @cameo=E1',
                '960 rclick @cameo=E1',
                '970 keydown Left_Shift', '971 rclick @cameo=E1', '976 keyup Left_Shift']
BUILD_ENV = {'RA2_NATIVE_BUILD_LIST': 'GAPOWR,GAPILE'}
BUILD_ARGS = ['--skirmish-order=build']

def count(key, want):
    """the harness-input value key=N (inf, veh) is want"""
    def check(state, st):
        got = value(state, key)
        return got == want, '%s=%s' % (key, got)
    return check


def objects(state):
    """[(type ID, selected, mission, (x, y))] in the harness-input order (Technos)"""
    return [(m.group(1), m.group(2) == '*', int(m.group(3)), (int(m.group(4)), int(m.group(5))))
            for m in re.finditer(r' ([A-Z0-9]+)(\*?):m(-?\d+)@(-?\d+),(-?\d+)', state[1])] if state else []


def tanks(state):
    return [o for o in objects(state) if o[0] == 'MTNK']


def selected(state):
    return [o[0] for o in objects(state) if o[1]]


def described(state):
    return 'selected %s' % (','.join(selected(state)) or '-') if state else 'no state'


def selection(ids):
    """exactly these type IDs are selected (in the harness-input order)"""
    def check(state, st):
        return state is not None and selected(state) == ids, described(state)
    return check


def first_tank(mission, chosen):
    """the first tank (the one '@MTNK' names) has this mission (5 guard, 2 move) and is or is
    not selected"""
    def check(state, st):
        t = tanks(state)
        ok = bool(t) and t[0][2] == mission and t[0][1] == chosen
        return ok, ('first tank m%d%s; ' % (t[0][2], '*' if t[0][1] else '') if t else 'no tank; ') + described(state)
    return check


def all_tanks_only():
    """a double click's type selection: every tank (all on the screen at the start), nothing else"""
    def check(state, st):
        t = tanks(state)
        return state is not None and len(t) >= 2 and selected(state) == ['MTNK'] * len(t), described(state)
    return check


def same_as(frame):
    """the selection is the one at that frame (a box's objects)"""
    def check(state, st):
        ref = st.get(frame)
        ok = state is not None and ref is not None and selected(ref) and selected(state) == selected(ref)
        return ok, described(state) + ' (at %d: %s)' % (frame, described(ref))
    return check


def deployed():
    """the MCV deployed: a construction yard stands, no MCV"""
    def check(state, st):
        ids = [o[0] for o in objects(state)]
        return 'GACNST' in ids and 'AMCV' not in ids, ' '.join(sorted(set(ids)))
    return check


def box(frame):
    """a selection band around the third tank: the press, a move held over several frames (the
    band starts once the held pointer is more than 4 pixels from the press), the release"""
    return ['%d move @MTNK:2+0,-2' % frame, '%d ldown @MTNK:2+0,-2' % (frame + 1), '%d move @MTNK:2+0,0' % (frame + 3),
            '%d move @MTNK:2+0,2' % (frame + 5), '%d lup @MTNK:2+0,2' % (frame + 8)]


# Modern controls (Controls: Modern) against the original's clicks (Classic), the same script.
# '@MTNK' is the player's first tank, '@MTNK:1' the second; ground two or three cells off a tank.
# The box (frames 503-548) is drawn around the third tank: whatever it holds is the band's.
CONTROLS_SCRIPT = ['100 click @AMCV', '120 rclick @AMCV',             # right click on the selected MCV
                   '200 click @MTNK',                                   # select
                   '230 click @MTNK+2,2',                               # left click on the ground
                   '310 click @MTNK',
                   '340 rclick @E1',                                    # right click on another unit
                   '370 rclick @MTNK+3,3',                              # right click on the ground
                   '420 keydown Left_Shift', '421 click @MTNK:1', '425 keyup Left_Shift',
                   '445 keydown Left_Shift', '446 click @MTNK:1', '450 keyup Left_Shift',
                   '480 click @MTNK:2', '481 click @MTNK:2',            # a double click
] + box(503) + ['518 keydown Left_Shift'] + box(520) + ['533 keyup Left_Shift', '538 keydown Left_Shift'] + box(540) + [
                   '553 keyup Left_Shift']
CASES = {
    'queue-five': dict(script=QUEUE_SCRIPT, frames=1000, env=dict(BUILD_ENV, RA2_NATIVE_ENHANCEMENTS='queuefive=on'),
                       args=BUILD_ARGS, want=[(930, 'Shift-click queues five', count('inf', 5)),
                                              (955, 'a plain click queues one more', count('inf', 6)),
                                              (965, 'a right click holds', count('inf', 6)),
                                              (1000, 'Shift-right-click removes five', count('inf', 1))]),
    'queue-five-off': dict(script=QUEUE_SCRIPT, frames=1000, env=dict(BUILD_ENV, RA2_NATIVE_ENHANCEMENTS='queuefive=off'),
                           args=BUILD_ARGS, want=[(930, 'Shift-click queues one (setting off)', count('inf', 1)),
                                                  (955, 'a plain click queues one more', count('inf', 2)),
                                                  (965, 'a right click holds', count('inf', 2)),
                                                  (1000, 'Shift-right-click removes one (setting off)', count('inf', 1))]),
    'modern': dict(script=CONTROLS_SCRIPT, frames=560, env={'RA2_NATIVE_ENHANCEMENTS': 'controls=modern'}, args=[],
                   want=[(195, 'right click on the selected MCV deploys it', deployed()),
                         (225, 'left click selects', first_tank(5, True)),
                         (260, 'left click on the ground deselects', selection([])),
                         (300, 'and orders nothing', first_tank(5, False)),
                         (360, 'right click on a unit never selects', selection(['MTNK'])),
                         (400, 'right click on the ground orders the move', first_tank(2, True)),
                         (400, 'and keeps the selection', selection(['MTNK'])),
                         (440, 'Shift-click adds', selection(['MTNK', 'MTNK'])),
                         (465, 'Shift-click on a selected unit removes it', selection(['MTNK'])),
                         (500, 'double click selects the type on the screen', all_tanks_only()),
                         (515, 'a box selects', lambda state, st: (bool(selected(state)), described(state))),
                         (535, 'Shift-box over selected objects removes them', selection([])),
                         (555, 'Shift-box again adds them', same_as(515)),
                         (560, 'the MCV stays deployed', deployed())]),
    'classic': dict(script=CONTROLS_SCRIPT, frames=560, env={'RA2_NATIVE_ENHANCEMENTS': 'controls=classic'}, args=[],
                    want=[(140, 'right click on the selected MCV deselects it', selection([])),
                          (225, 'left click selects', first_tank(5, True)),
                          (260, 'left click on the ground orders the move', first_tank(2, True)),
                          (360, 'right click deselects', selection([])),
                          (440, 'Shift-click selects (nothing was selected)', selection(['MTNK'])),
                          (500, 'a double click selects the one tank', selection(['MTNK'])),
                          (535, 'Shift-box adds (all were selected: they stay)', same_as(515)),
                          (560, 'the MCV stays undeployed', lambda state, st: ('AMCV' in [o[0] for o in objects(state)], described(state)))]),
}


def states(out):
    """frame -> (selected count, objects text)"""
    found = {}
    for line in out.splitlines():
        m = STATE.match(line)
        if m:
            found[int(m.group(1))] = (int(m.group(2)), m.group(3))
    return found


def value(state, key):
    m = re.search(r' %s=(-?\d+)' % re.escape(key), state[1]) if state else None
    return int(m.group(1)) if m else None


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--case', required=True, choices=sorted(CASES))
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default=os.environ.get('RA2_NATIVE_CENSUS_DATA_DIR', ''))
    ap.add_argument('--game-exe', default=os.environ.get('RA2_NATIVE_CENSUS_GAME_EXE', ''))
    ap.add_argument('--seconds', type=int, default=300)
    ap.add_argument('--log', default='')
    ap.add_argument('--wine-prefix', default='')
    ap.add_argument('--wine', default='')
    a = ap.parse_args()
    case = CASES[a.case]
    name = 'native-controls-' + a.case
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
        prefix = winehost.Prefix(wine, a.wine_prefix + '-' + a.case)   # one prefix per case (ctest runs them in parallel)
    env = dict(os.environ, RA2_GAME_EXE=a.game_exe, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_CRASH_TRACE='1',
               RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_INPUT_LOG='5')
    env.pop('RA2_NATIVE_TRAP_CONTINUE', None)
    env.update(case['env'])
    work = tempfile.mkdtemp(prefix='ra2-controls-')
    try:
        script = os.path.join(work, 'input.script')
        with open(script, 'w') as f:
            f.write('\n'.join(case['script']) + '\n')
        write_dir = os.path.join(work, 'write')
        os.mkdir(write_dir)
        cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=frames', '--run-seconds=%d' % a.seconds,
               '--write-dir=' + write_dir, '--game-args=-noaudio -CD.', '--skirmish=#5',
               '--skirmish-frames=%d' % case['frames'], '--input-script=' + script] + case['args']
        # Under Wine a launch the Mac host killed (wine.host_failure) runs again, at most
        # HOST_RETRIES times; a failure of the game itself is never retried.
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
    st = states(out)
    problems = []
    if p.returncode != 0 or bad:
        problems.append('exit %d, %d bad lines' % (p.returncode, len(bad)))
    for frame, what, check in case['want']:
        ok, got = check(st.get(frame), st)
        print('frame %d (%s): %s%s' % (frame, what, got, '' if ok else '  <- FAILED'))
        if not ok:
            problems.append('frame %d: not %s (%s)' % (frame, what, got))
    for l in bad[:20]:
        print('  ' + l)
    if problems:
        print('%s: FAILED (%s)' % (name, '; '.join(problems)))
        return 1
    print('%s: passed' % name)
    return 0


if __name__ == '__main__':
    sys.exit(main())
