#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest yr-native-locked-input (lane N32; built with -DRA2_NATIVE_YR=ON): touch pan and pinch zoom wait for a
mission intro's input lock, as Red Alert 2's lane N31 (tests/command_bar_input.py --case lock).

A mission's intro locks the player's input (the LOCK_INPUT trigger action, gamemd.exe 0x006E2660 ->
Lock_Scenario_Input 0x00684240: IgnoreInput 0x00A8ED9C, which ra2-decomp also names UserInputLocked) while its
scripted camera moves, until the unlock. The original's own edge and drag scrolling refuse then
(ScrollClass::Scroll_AI 0x00692F30); natively the touch controls' pan (ra2_native_scroll_map, glue/yr_app_hooks.cpp)
and the zoom gestures (ra2_native_zoom_by, glue/yr_map_zoom.cpp) refuse too.

Allied mission 1 (ALL01UMD) from the menus (tests/mission_soak.py's route, the soak's fixed seed), Zoom gestures on
and the touch controls on. Its intro holds the input locked from the start to about frame 1490 while the scripted
camera moves, and rests the camera at frames ~450-740 (the test's locked window, so the camera's own moves are not
taken for the swipe's). A one-finger swipe over the map at frame LOCKED and a pinch step (input script `zoom`, the wheel's and the pinch's
path) 20 frames later must leave the view where the script has it (harness `view` every 10 frames: the view and
the zoom the same 9 frames before the swipe and 11 after the pinch, both refusals logged); the same swipe and zoom
at FREE and FREE + 40, after the unlock, must move and zoom the view (the input reaches the game at all).

  yr_locked_input.py --binary <yr-native> --data-dir <game folder> --work <dir>
Skipped (77) without the installed game (gamemd.exe in --data-dir) and for the Windows build.
"""
import argparse
import os
import re
import shutil
import subprocess
import sys

TESTS = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', 'tests')
sys.path.insert(0, TESTS)
import game_process  # noqa: E402
import mission_soak  # noqa: E402

BAD = re.compile(r'unrecovered|\btrap\b|signal \d+|SIG(SEGV|BUS|ILL|ABRT|FPE)|crash:', re.I)
VIEW = re.compile(r'^harness-view (-?\d+),(-?\d+) locked (\d) zoom ([\d.]+) frame (\d+)$', re.M)
LOCKED = 500    # the locked swipe: ALL01UMD's intro camera rests at frames ~450-740 (it moves at 0-440, 750-1460)
FREE = 1600     # the free swipe: the intro unlocks the input at about frame 1490


def swipe(frame, x=500, y=400, dx=-25, dy=-12, steps=12):
    lines = ['%d tdown %d %d' % (frame, x, y)]
    lines += ['%d tmove %d %d' % (frame + k, x + dx * k, y + dy * k) for k in range(1, steps + 1)]
    lines.append('%d tup %d %d' % (frame + steps + 1, x + dx * steps, y + dy * steps))
    return lines


def run(a):
    work = os.path.join(a.work, 'lock')
    shutil.rmtree(work, ignore_errors=True)
    os.makedirs(os.path.join(work, 'w'))

    class Soak:
        game, side, data_dir = 'yr', 'allied', a.data_dir
    with open(os.path.join(work, 'w', 'RA2MD.INI'), 'w') as f:
        f.write(mission_soak.settings(Soak))
    lines = (mission_soak.script('yr', 'allied', 1).strip().splitlines() + swipe(LOCKED)
             + ['%d zoom 1.5 300 300' % (LOCKED + 20)] + swipe(FREE) + ['%d zoom 1.5 300 300' % (FREE + 40)])
    script = os.path.join(work, 'input.script')
    with open(script, 'w') as f:
        f.write('\n'.join(lines) + '\n')
    env = dict(os.environ, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_MSG_LOG='0',
               RA2_NATIVE_CRASH_TRACE='1', RA2_NATIVE_ENHANCEMENTS='zoomgestures=on', RA2_NATIVE_TOUCH_UI='1',
               RA2_NATIVE_UNCAPPED='1', RA2_NATIVE_SOAK='250', RA2_NATIVE_SOAK_SEED='1', RA2_NATIVE_SOAK_SPEED='0',
               RA2_NATIVE_SOAK_END='%d@1' % (FREE + 100), RA2_NATIVE_ORDERS='+1/10 view',
               RA2_NATIVE_SETTINGS=os.path.join(work, 'settings'))
    for k in ('RA2_NATIVE_TRAP_CONTINUE', 'RA2_NATIVE_FORCE_END', 'RA2_NATIVE_SOAK_DUMP'):
        env.pop(k, None)
    cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=soak-end', '--run-seconds=%d' % a.seconds,
           '--write-dir=' + os.path.join(work, 'w'), '--input-script=' + script, '--game-args=-noaudio -CD.']
    with open(os.path.join(work, 'run.log'), 'w') as log:
        code = game_process.run(cmd, env=env, stdout=log, stderr=subprocess.STDOUT, timeout=a.seconds + 120).returncode
    out = open(os.path.join(work, 'run.log'), errors='replace').read()
    problems = []
    bad = [l for l in out.splitlines() if BAD.search(l) and not l.startswith('input-script')]
    if code != 0 or bad:
        problems.append('exit %d, bad lines: %s' % (code, '; '.join(bad[:3])))
    views = {int(m.group(5)): ((int(m.group(1)), int(m.group(2))), m.group(3) == '1', float(m.group(4)))
             for m in VIEW.finditer(out)}
    before, after = LOCKED - 9, LOCKED + 31
    want = (before, after, FREE + 1, FREE + 31, FREE + 61)
    for f in want:
        v = views.get(f)
        print('lock: frame %d: %s' % (f, 'view %d,%d, input %s, zoom %.3f' % (v[0] + ('locked' if v[1] else 'free', v[2]))
                                      if v else 'no view'))
    if any(f not in views for f in want):
        return problems + ['the run has no view at frames %s (did Allied mission 1 start and reach frame %d?)'
                           % ([f for f in want if f not in views], FREE + 61)]
    if not views[before][1] or not views[after][1]:
        problems.append('the input was not locked at frames %d-%d: the intro\'s lock is not where this test expects it'
                        % (before, after))
    if views[after][0] != views[before][0] or views[after][2] != views[before][2]:
        problems.append('while the input was locked the swipe or the pinch moved the view (%s zoom %.3f -> %s zoom %.3f)'
                        % (views[before][0], views[before][2], views[after][0], views[after][2]))
    for refusal in ('scroll: input locked', 'zoom: input locked'):
        if refusal not in out:
            problems.append('no "%s" line: the locked input did not reach its refusal' % refusal)
    if views[FREE + 1][1]:
        problems.append('the input was still locked at frame %d' % (FREE + 1))
    elif views[FREE + 31][0] == views[FREE + 1][0]:
        problems.append('the swipe at frame %d (input free) did not move the view' % FREE)
    if views[FREE + 61][2] == views[FREE + 31][2]:
        problems.append('the pinch step at frame %d (input free) did not zoom' % (FREE + 40))
    return ['lock: %s' % x for x in problems]


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--work', required=True)
    ap.add_argument('--seconds', type=int, default=300)
    a = ap.parse_args()
    if not a.data_dir or not os.path.exists(os.path.join(a.data_dir, 'gamemd.exe')):
        print('skipped: no Yuri\'s Revenge install (gamemd.exe) in --data-dir')
        return 77
    if a.binary.lower().endswith('.exe'):
        print('skipped: run on the Mac build (the Windows build has the same game code)')
        return 77
    os.makedirs(a.work, exist_ok=True)
    problems = run(a)
    for x in problems:
        print('FAIL: ' + x)
    print('yr-native-locked-input: %s: touch pan and pinch zoom wait for the intro\'s input lock (%s)'
          % ('FAIL' if problems else 'OK', a.work))
    return 1 if problems else 0


if __name__ == '__main__':
    sys.exit(main())
