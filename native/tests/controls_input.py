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

CASES = {
    'queue-five': dict(script=QUEUE_SCRIPT, frames=1000, env=dict(BUILD_ENV, RA2_NATIVE_ENHANCEMENTS='queuefive=on'),
                       args=BUILD_ARGS, want=[(930, 'inf', 5, 'Shift-click queues five'),
                                              (955, 'inf', 6, 'a plain click queues one more'),
                                              (965, 'inf', 6, 'a right click holds'),
                                              (1000, 'inf', 1, 'Shift-right-click removes five')]),
    'queue-five-off': dict(script=QUEUE_SCRIPT, frames=1000, env=dict(BUILD_ENV, RA2_NATIVE_ENHANCEMENTS='queuefive=off'),
                           args=BUILD_ARGS, want=[(930, 'inf', 1, 'Shift-click queues one (setting off)'),
                                                  (955, 'inf', 2, 'a plain click queues one more'),
                                                  (965, 'inf', 2, 'a right click holds'),
                                                  (1000, 'inf', 1, 'Shift-right-click removes one (setting off)')]),
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
    for frame, key, want, what in case['want']:
        got = value(st.get(frame), key)
        print('frame %d (%s): %s=%s' % (frame, what, key, got))
        if got != want:
            problems.append('frame %d: %s %s=%s, not %d' % (frame, what, key, got, want))
    for l in bad[:20]:
        print('  ' + l)
    if problems:
        print('%s: FAILED (%s)' % (name, '; '.join(problems)))
        return 1
    print('%s: passed' % name)
    return 0


if __name__ == '__main__':
    sys.exit(main())
