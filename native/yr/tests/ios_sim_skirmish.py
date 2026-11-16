#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""Yuri's Revenge's iOS app (YR.app, lane Y7) on the "RA2 Duo Validation" Simulator, through the
touch layer with scripted input (not a ctest: it needs the Simulator; native/yr/README.md
"yr-native on iOS"):

  import the game files from the installation (RA2_NATIVE_IMPORT, the app's own import filter and
  checks; the first run only) -> main menu -> Single Player -> Skirmish -> Start Game (taps) -> a
  few minutes of play: a swipe over the map (the touch layer's pan, ra2_native_scroll_map), a
  hold-and-drag selection box, the toolbar's Deploy, taps on the sidebar and the map;
  then (--score) a second start: a lost short game (the construction yard sold) -> the Skirmish
  Score screen -> Continue -> Back -> Back -> the main menu.

Checks per run: the time limit ends it (no trap, no fault); the dialogs expected were shown; game
frames ran (window dumps). Screenshots of the Simulator (simctl io) go to --work.

Only the "RA2 Duo Validation" Simulator is used (its UDID by default; another name is refused).
It is booted when shut down and shut down again afterwards; the app stays installed.

  ios_sim_skirmish.py --app build/ios-yr-simulator/YR.app --data-dir <RA2 + YR folder> --work <dir>
                      [--play-seconds 180] [--score]
"""
import argparse
import json
import os
import re
import shutil
import subprocess
import sys
import threading
import time

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', 'tests'))
import game_process  # noqa: E402  (native/tests: the launcher's process group, lane C19)

VALIDATION = '3F7CDD1E-A992-4FF8-A898-10CDF3A28920'
VALIDATION_NAME = 'RA2 Duo Validation'
BUNDLE = 'io.github.ra2native.YR'


def simctl(*args, check=True, **kw):
    return subprocess.run(['xcrun', 'simctl'] + list(args), check=check, capture_output=True, text=True, **kw)


def device(udid):
    out = json.loads(simctl('list', 'devices', '-j').stdout)
    for runtime in out['devices'].values():
        for d in runtime:
            if d['udid'] == udid:
                return d
    return None


# Menu taps at the main menu's / Single Player's / the skirmish setup's buttons (YR's 800x600 menu
# coordinates, x offset by the play width's margin: the menus are centred on the wider surface).
# Each line waits for its dialog to settle ("s<k>", host.cpp); the in-game lines count from the
# setup's settling. "tap" is tdown + tup (a touch, touch.cpp), "tswipe" a pan over the map.
def tap(when, x, y):
    return ['%s tdown %d %d' % (when, x, y), '%s tup %d %d' % (when.split('+')[0] + '+' + str(int(when.split('+')[1]) + 80), x, y)]


def at(base, ms):
    return '%s+%d' % (base, ms)


def play_script(play_w, play_ms):
    """Menus -> skirmish -> play_ms of play with the touch gestures."""
    mx = (play_w - 800) // 2       # the menus' left edge on the wider surface
    lines = []
    lines += tap('s1+1500', mx + 720, 220)          # Single Player
    lines += tap('s2+1500', mx + 720, 304)          # Skirmish
    lines += tap('s3+2500', mx + 720, 262)          # Start Game
    g = 's3'
    cx, cy = (play_w - 168) // 2, 300
    # A swipe over the map: the view pans (touch.cpp -> ra2_native_scroll_map).
    t = 20000
    lines += ['%s tdown %d %d' % (at(g, t), cx, cy)]
    for k in range(1, 9):
        lines += ['%s tmove %d %d' % (at(g, t + 40 * k), cx - 20 * k, cy - 12 * k)]
    lines += ['%s tup %d %d' % (at(g, t + 400), cx - 160, cy - 96)]
    # Back home (H), then a hold-and-drag selection box over the start units (400 ms hold).
    lines += ['%s key H' % at(g, 23000)]
    t = 25000
    lines += ['%s tdown 20 20' % at(g, t)]
    lines += ['%s tmove 22 22' % at(g, t + 500)]
    for k in range(1, 9):
        lines += ['%s tmove %d %d' % (at(g, t + 500 + 60 * k), 20 + (play_w - 220) * k // 8, 20 + 540 * k // 8)]
    lines += ['%s tup %d 560' % (at(g, t + 1200), play_w - 200)]
    # The toolbar's Deploy (the MCV deploys), Guard, A-Move on and off.
    lines += ['%s toolbar 4' % at(g, 28000)]
    lines += ['%s toolbar 5' % at(g, 36000)]
    lines += ['%s toolbar 3' % at(g, 38000), '%s toolbar 3' % at(g, 40000)]
    # Then build: every 60 s H, a tap on the sidebar's first icon (production starts: a power
    # plant once the construction yard stands), 55 s later a tap on it again (the finished
    # building is picked up for placement) and taps beside the yard (placed).
    t = 45000
    while t < play_ms - 60000:
        lines += ['%s key H' % at(g, t)]
        lines += tap(at(g, t + 1000), play_w - 120, 260)
        lines += ['%s key H' % at(g, t + 54000)]
        lines += tap(at(g, t + 55000), play_w - 120, 260)
        for k, (dx, dy) in enumerate(((-90, 60), (90, 60), (0, 100), (-120, -20))):
            lines += tap(at(g, t + 56500 + 700 * k), cx + dx, cy + dy)
        t += 60000
    return lines


def score_script(play_w):
    """Menus -> a short game lost by selling the construction yard -> score -> back to the menu."""
    mx = (play_w - 800) // 2
    lines = []
    lines += tap('s1+1500', mx + 720, 220)
    lines += tap('s2+1500', mx + 720, 304)
    lines += tap('s3+2500', mx + 720, 262)
    g = 's3'
    t = 18000
    lines += ['%s tdown 15 15' % at(g, t), '%s tmove 17 17' % at(g, t + 500)]
    for k in range(1, 9):
        lines += ['%s tmove %d %d' % (at(g, t + 500 + 60 * k), 15 + (play_w - 215) * k // 8, 15 + 525 * k // 8)]
    lines += ['%s tup %d 540' % (at(g, t + 1200), play_w - 200)]
    lines += ['%s toolbar 4' % at(g, 20500)]                     # Deploy
    lines += ['%s key H' % at(g, 28000)]
    lines += tap(at(g, 29000), play_w - 64, 180)                   # the sidebar's Sell
    lines += tap(at(g, 30000), (play_w - 168) // 2, 284)            # the construction yard
    lines += tap('s4+3000', mx + 720, 556)                          # the score screen's Continue
    lines += tap('s5+2000', mx + 720, 556)                          # the setup's Back
    lines += tap('s6+2000', mx + 720, 556)                          # Single Player's Back
    return lines


def dialogs(text):
    titles = []
    for l in text.splitlines():
        if l.startswith('dialog shown #'):
            titles.append(None)
        elif titles and titles[-1] is None:
            m = re.search(r'"GUI:(MainMenu|SinglePlayerMenu|SkirmishGame|MultiplayerScore)"', l)
            if m and ' 1684 ' in l:
                titles[-1] = m.group(1)
    return [t for t in titles if t]


def shots(udid, folder, stop, period):
    n = 0
    while not stop.wait(period):
        n += 1
        # The Duo Simulator has two displays: the inner (unfolded) one is display 3.
        simctl('io', udid, 'screenshot', '--display=3', os.path.join(folder, 'sim-%03d.png' % n), check=False)


def run(a, name, script_lines, seconds, extra_env):
    work = os.path.join(a.work, name)
    shutil.rmtree(work, ignore_errors=True)
    os.makedirs(os.path.join(work, 'shots'))
    script = os.path.join(work, name + '.script')
    with open(script, 'w') as f:
        f.write('\n'.join(script_lines) + '\n')
    env = dict(os.environ)
    child = dict(RA2_NATIVE_IMPORT=a.data_dir, RA2_NATIVE_LOG_STDERR='1', RA2_NATIVE_NO_CRASH_DIALOG='1',
                 RA2_NATIVE_NO_RELAUNCH='1', RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_DIALOG_LOG='1', RA2_NATIVE_MSG_LOG='0',
                 RA2_NATIVE_WINDOW_DUMP=os.path.join(work, 'shots', 'win'), RA2_NATIVE_WINDOW_DUMP_MS='5000',
                 RA2_NATIVE_INPUT_SCRIPT=script, RA2_NATIVE_IGNORE_FOCUS='1', **extra_env)
    for k, v in child.items():
        env['SIMCTL_CHILD_' + k] = v
    # The app as a player starts it (app mode: the import, the screen's resolution, the touch
    # controls); the input script plays through the host. The run ends after `seconds` from the
    # game's start (the "app: game folder" line, after the first run's import of about 2 GB).
    cmd = ['xcrun', 'simctl', 'launch', '--console', '--terminate-running-process', a.udid, BUNDLE]
    stop = threading.Event()
    shooter = threading.Thread(target=shots, args=(a.udid, work, stop, a.shot_seconds), daemon=True)
    shooter.start()
    log_path = os.path.join(work, 'run.log')
    alive = False
    try:
        with open(log_path, 'w') as log, game_process.Popen(cmd, env=env, stdout=log, stderr=subprocess.STDOUT) as g:
            deadline = time.monotonic() + 900
            started = None
            while time.monotonic() < deadline and g.poll() is None:
                text = open(log_path, errors='replace').read()
                if started is None and 'app: game folder' in text:
                    started = time.monotonic()
                    deadline = started + seconds
                if 'unrecovered ' in text or 'crash: signal' in text:
                    break
                time.sleep(1)
            text = open(log_path, errors='replace').read()
            alive = g.poll() is None and started is not None and 'unrecovered ' not in text and 'crash: signal' not in text
    finally:
        stop.set()
        shooter.join()
        simctl('terminate', a.udid, BUNDLE, check=False)
    with open(log_path, 'a') as log:
        log.write('harness: stopped: %s\n' % ('time limit' if alive else 'the app ended or failed first'))
    return open(os.path.join(work, 'run.log'), errors='replace').read(), work


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--app', required=True)
    ap.add_argument('--data-dir', required=True)
    ap.add_argument('--work', required=True)
    ap.add_argument('--udid', default=VALIDATION)
    ap.add_argument('--play-seconds', type=int, default=180)
    ap.add_argument('--shot-seconds', type=float, default=30)
    ap.add_argument('--play-w', type=int, default=872, help='the play width the first run assumes (the Duo Simulator: 872)')
    ap.add_argument('--score', action='store_true')
    ap.add_argument('--keep-booted', action='store_true')
    a = ap.parse_args()
    a.data_dir = os.path.abspath(a.data_dir)
    a.work = os.path.abspath(a.work)
    if not os.path.exists(os.path.join(a.data_dir, 'gamemd.exe')):
        print('skipped: no Yuri\'s Revenge install (gamemd.exe) in --data-dir')
        return 77
    d = device(a.udid)
    if not d or d['name'] != VALIDATION_NAME:
        print('refused: %s is not the "%s" Simulator' % (a.udid, VALIDATION_NAME))
        return 2
    os.makedirs(a.work, exist_ok=True)
    booted_here = d['state'] != 'Booted'
    if booted_here:
        simctl('boot', a.udid)
        simctl('bootstatus', a.udid, '-b')
    failures = []
    try:
        simctl('install', a.udid, a.app)
        play_ms = a.play_seconds * 1000
        # The play width the app picks from the screen (main.cpp, app::game_resolution) is in the
        # log; the first run's menu taps assume it from the Duo Simulator's points (872).
        text, work = run(a, 'play', play_script(a.play_w, play_ms), a.play_seconds, {})
        m = re.search(r'play (\d+)x(\d+)', text)
        width = int(m.group(1)) if m else a.play_w
        stop = [l for l in text.splitlines() if 'stopped:' in l]
        order = dialogs(text)
        dumps = len([f for f in os.listdir(os.path.join(work, 'shots')) if f.endswith('.bmp')])
        print('play: resolution %s, dialogs %s, %d window dumps, %s' % (m.group(0) if m else '?', ' -> '.join(order), dumps,
                                                                       stop[-1] if stop else 'no stop line'))
        if not stop or 'time limit' not in stop[-1]:
            failures.append('play: the run stopped early: %s' % (stop[-1] if stop else 'no stop line'))
        if order[:3] != ['MainMenu', 'SinglePlayerMenu', 'SkirmishGame']:
            failures.append('play: menus %s' % ' -> '.join(order))
        if dumps < a.play_seconds // 5 // 2:
            failures.append('play: too few window presents (%d)' % dumps)
        for needle, what in (('touch: pan from', 'the swipe pan'), ('touch: left down', 'a tap')):
            if needle not in text:
                failures.append('play: no %s in the log' % what)
        if a.score:
            text, work = run(a, 'score', score_script(width), 120, {})
            stop = [l for l in text.splitlines() if 'stopped:' in l]
            order = dialogs(text)
            print('score: dialogs %s, %s' % (' -> '.join(order), stop[-1] if stop else 'no stop line'))
            if not stop or 'time limit' not in stop[-1]:
                failures.append('score: the run stopped early: %s' % (stop[-1] if stop else 'no stop line'))
            if 'MultiplayerScore' not in order:
                failures.append('score: the score screen was not shown (%s)' % ' -> '.join(order))
            elif order[order.index('MultiplayerScore') + 1:][:3] != ['SkirmishGame', 'SinglePlayerMenu', 'MainMenu']:
                failures.append('score: after the score screen: %s' % ' -> '.join(order[order.index('MultiplayerScore') + 1:]))
    finally:
        if booted_here and not a.keep_booted:
            simctl('shutdown', a.udid, check=False)
    for f in failures:
        print('FAIL: ' + f)
    print('%s (%s)' % ('FAIL' if failures else 'OK', a.work))
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
