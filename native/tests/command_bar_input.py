#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""The command bar and the player's camera input (lane N31; the user's Duo reports of 2026-11-22), checked by the
game's own state through harness orders:

  thumb   (ctest native-command-bar-thumb) a skirmish ('#5') with the bar's thumbs clicked through the window
          (RA2_NATIVE_SCRIPT_AS_USER: a click lands where the window shows that game pixel). The bar's close thumb
          (TabClass::ThumbOpenButton, 0xF0, at the left end of its buttons) hides the buttons, and its open thumb
          (ThumbClosedButton, 0xF1, at the bar's right end) brings them back (TabClass::AI 0x006A0F30). With Zoom
          gestures on, the game's screen is twice the window's in its map column and the host shows only the bar's
          left part, so the open thumb laid out at the larger screen's right end lay off the picture: a bar closed
          could not be opened again (link/native_map_zoom.cpp Native_Command_Bar_Right lays it out from the shown
          part's right end). Before that, the in-game menu is opened and resumed: closing an in-play dialog made the
          sidebar art's layout again for the sidebar's shorter height, and with it the bar's rectangles, so
          TabClass::Draw_It drew the bar hundreds of rows up the map whenever its buttons asked for a redraw (the
          bar flashing higher up when its icons were tapped; link/native_enhancements.cpp Native_Command_Bar_Top).
          Harness `thumb` lines at frames 100 (open), 200 (after the menu: the bar still at the screen's bottom),
          270 (the close thumb clicked at 230: closed, the open thumb a button inside the shown bar) and 330 (the
          open thumb clicked at 290: open). Three settings: Zoom gestures with Game interface size 1.25x (the Duo's),
          interface size 1.25x alone, and the defaults (the original's layout: the open thumb at x 800 of 1024).
  lock    (ctest native-locked-input) Allied mission 1 (ALL01T) from the menus, Zoom gestures on and the touch
          controls on. Its intro locks the player's input (TAction_LOCK_INPUT 0x006B1AE0 -> Pause_Game: IgnoreInput)
          while the scripted camera moves, until about frame 830. A one-finger swipe over the map (the touch
          controls' pan, ra2_native_scroll_map) at frame 300 and a pinch step ("zoom", the wheel's and the pinch's
          path, ra2_native_zoom_by) at 320 must leave the view where the script has it (harness `view`: the view
          at frames 291 and 331 the same, the zoom unchanged, both refusals logged); the same swipe and zoom at
          1000 and 1040, after the unlock, must move and zoom the view (the input reaches the game at all).

  command_bar_input.py --case thumb|lock --binary <ra2-native> --data-dir <installed RA2> --game-exe <game.exe>
                       [--work <dir>]

Without the data folder or game.exe it exits 77 (skipped); the Windows build too.
"""
import argparse
import os
import re
import shutil
import subprocess
import sys
import tempfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import game_process  # noqa: E402
import mission_soak  # noqa: E402

SKIP = 77
BAD = re.compile(r'unrecovered|\btrap\b|signal \d+|SIG(SEGV|BUS|ILL|ABRT|FPE)|crash:', re.I)
THUMB = re.compile(r'^harness-thumb (open|closed) close (-?\d+),(-?\d+) (\d+)x(\d+)( listed)? '
                   r'open (-?\d+),(-?\d+) (\d+)x(\d+)( listed)? bar (-?\d+),(-?\d+) (\d+)x(\d+) end (-?\d+),(-?\d+)$', re.M)
SCREEN = re.compile(r"^zoom: render scale (\d+): the game's screen (\d+)x(\d+) for (\d+)x(\d+)", re.M)
VIEW = re.compile(r'^harness-view (-?\d+),(-?\d+) locked (\d) zoom ([\d.]+) frame (\d+)$', re.M)

# The in-game menu (Escape) and its Resume Mission button (the sidebar column's bottom button at 1024x768, where
# the host shows it with and without Zoom gestures: tests/zoom_input.py's menu case), then the thumbs.
THUMB_SCRIPT = ['120 key Escape', 'd1+1500 click 940 714', '230 click @thumb', '290 click @thumb']
THUMB_ORDERS = '100 thumb; 200 thumb; 270 thumb; 330 thumb'


def env_for(a, enhancements):
    env = dict(os.environ, RA2_GAME_EXE=a.game_exe, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_CRASH_TRACE='1',
               RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_MSG_LOG='0')
    for k in ('RA2_NATIVE_TRAP_CONTINUE', 'RA2_NATIVE_FORCE_END', 'RA2_NATIVE_ENHANCEMENTS', 'RA2_NATIVE_ORDERS'):
        env.pop(k, None)
    if enhancements:
        env['RA2_NATIVE_ENHANCEMENTS'] = enhancements
    return env


def run_game(cmd, env, log_path, timeout):
    p = game_process.run(cmd, env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, errors='replace',
                         timeout=timeout)
    with open(log_path, 'w') as f:
        f.write(p.stdout)
    return p


def thumb_run(a, work, name, enhancements):
    d = os.path.join(work, name)
    shutil.rmtree(d, ignore_errors=True)
    os.makedirs(os.path.join(d, 'w'))
    script = os.path.join(d, 'input.script')
    with open(script, 'w') as f:
        f.write('\n'.join(THUMB_SCRIPT) + '\n')
    env = env_for(a, enhancements)
    env.update(RA2_NATIVE_SCRIPT_AS_USER='1', RA2_NATIVE_ORDERS=THUMB_ORDERS)
    cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=frames', '--run-seconds=240', '--write-dir=' + os.path.join(d, 'w'),
           '--game-args=-noaudio -CD.', '--skirmish=#5', '--skirmish-frames=360', '--input-script=' + script]
    p = run_game(cmd, env, os.path.join(d, 'run.log'), 360)
    out = p.stdout
    problems = []
    bad = [l for l in out.splitlines() if BAD.search(l)]
    if p.returncode != 0 or bad:
        problems.append('exit %d, bad lines: %s' % (p.returncode, '; '.join(bad[:3])))
    m = SCREEN.search(out)
    screen_h, shown_w = (int(m.group(3)), int(m.group(4)) - 168) if m else (768, 1024 - 168)
    states = THUMB.findall(out)
    for label, s in zip(('start', 'after the menu', 'closed', 'opened'), states):
        print('%s %s: %s; close thumb %s,%s%s; open thumb %s,%s%s; bar at y %s, end piece x %s' % (
            name, label, s[0], s[1], s[2], s[5], s[6], s[7], s[10], s[12], s[15]))
    if len(states) != 4:
        return problems + ['%d of 4 "harness-thumb" lines (the game did not reach frame 330)' % len(states)]
    start, menu, closed, opened = states
    for label, s in (('at the start', start), ('after the in-game menu', menu), ('closed', closed), ('opened', opened)):
        if int(s[12]) != screen_h - 32:
            problems.append('%s the bar is laid out at y %s, not the screen\'s bottom (%d): Draw_It draws it there'
                            % (label, s[12], screen_h - 32))
    if start[0] != 'open' or not start[5]:
        problems.append('the bar was not open with its close thumb a button at first')
    if menu[0] != 'open':
        problems.append('the bar was not open after the in-game menu')
    if closed[0] != 'closed':
        problems.append('the close thumb did not close the bar')
    elif not closed[10]:
        problems.append('the closed bar has no open thumb button')
    open_x, open_w = int(closed[6]), int(closed[8])
    if open_x < 0 or open_x + open_w > shown_w:
        problems.append('the open thumb at x %d (%d wide) lies outside the shown bar (0-%d): the closed bar cannot be '
                        'opened' % (open_x, open_w, shown_w))
    if opened[0] != 'open':
        problems.append('the open thumb did not open the bar again')
    if not enhancements and open_x != 800:
        problems.append('the defaults moved the open thumb from the original\'s x 800 to %d' % open_x)
    if 'input-script: cannot' in out or 'is not a button now' in out or 'no point for' in out:
        problems.append('a scripted click found no thumb')
    return ['%s: %s' % (name, x) for x in problems]


def swipe(frame, x=500, y=400, dx=-25, dy=-12, steps=12):
    lines = ['%d tdown %d %d' % (frame, x, y)]
    lines += ['%d tmove %d %d' % (frame + k, x + dx * k, y + dy * k) for k in range(1, steps + 1)]
    lines.append('%d tup %d %d' % (frame + steps + 1, x + dx * steps, y + dy * steps))
    return lines


def lock_run(a, work):
    d = os.path.join(work, 'lock')
    shutil.rmtree(d, ignore_errors=True)
    os.makedirs(os.path.join(d, 'w'))

    class Soak:
        game, side, data_dir = 'ra2', 'allied', a.data_dir
    with open(os.path.join(d, 'w', 'RA2.INI'), 'w') as f:
        f.write(mission_soak.settings(Soak))
    # The menus' points (the installation's 1024x768 screen) moved to where the render-scaled screen has them: its
    # 800x600 menu at the centre of the 1880x1504 screen, (1880 - 1024) / 2 and (1504 - 768) / 2 further.
    menus = re.sub(r'click (\d+) (\d+)', lambda m: 'click %d %d' % (int(m.group(1)) + 428, int(m.group(2)) + 368),
                   mission_soak.script('ra2', 'allied', 1)).strip().splitlines()
    lines = menus + swipe(300) + ['320 zoom 1.5 300 300'] + swipe(1000) + ['1040 zoom 1.5 300 300']
    script = os.path.join(d, 'input.script')
    with open(script, 'w') as f:
        f.write('\n'.join(lines) + '\n')
    env = env_for(a, 'zoomgestures=on')
    env.update(RA2_NATIVE_TOUCH_UI='1', RA2_NATIVE_UNCAPPED='1', RA2_NATIVE_SOAK='250', RA2_NATIVE_SOAK_SEED='1',
               RA2_NATIVE_SOAK_SPEED='0', RA2_NATIVE_SOAK_END='1100@1', RA2_NATIVE_ORDERS='+1/10 view')
    cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=soak-end', '--run-seconds=300', '--write-dir=' + os.path.join(d, 'w'),
           '--input-script=' + script, '--game-args=-noaudio -CD.']
    p = run_game(cmd, env, os.path.join(d, 'run.log'), 400)
    out = p.stdout
    problems = []
    bad = [l for l in out.splitlines() if BAD.search(l)]
    if p.returncode != 0 or bad:
        problems.append('exit %d, bad lines: %s' % (p.returncode, '; '.join(bad[:3])))
    views = {int(m.group(5)): ((int(m.group(1)), int(m.group(2))), m.group(3) == '1', float(m.group(4)))
             for m in VIEW.finditer(out)}
    want = (291, 331, 1001, 1031, 1061)
    for f in want:
        print('lock: frame %d: %s' % (f, ('view %d,%d, input %s, zoom %.3f' % (views[f][0] + ('locked' if views[f][1] else 'free',
                                                                                             views[f][2]))) if f in views else 'no view'))
    if any(f not in views for f in want):
        return problems + ['the run has no view at frames %s (did Allied mission 1 start and reach frame 1061?)'
                           % [f for f in want if f not in views]]
    if not views[291][1] or not views[331][1]:
        problems.append('the input was not locked at frames 291-331: the intro\'s lock is not where this test expects it')
    if views[331][0] != views[291][0] or views[331][2] != views[291][2]:
        problems.append('while the input was locked the swipe or the pinch moved the view (%s zoom %.3f -> %s zoom %.3f)'
                        % (views[291][0], views[291][2], views[331][0], views[331][2]))
    for refusal in ('scroll: input locked', 'zoom: input locked'):
        if refusal not in out:
            problems.append('no "%s" line: the locked input did not reach its refusal' % refusal)
    if views[1001][1]:
        problems.append('the input was still locked at frame 1001')
    elif views[1031][0] == views[1001][0]:
        problems.append('the swipe at frame 1000 (input free) did not move the view')
    if views[1061][2] == views[1031][2]:
        problems.append('the pinch step at frame 1040 (input free) did not zoom')
    return ['lock: %s' % x for x in problems]


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--case', required=True, choices=['thumb', 'lock'])
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default=os.environ.get('RA2_NATIVE_CENSUS_DATA_DIR', ''))
    ap.add_argument('--game-exe', default=os.environ.get('RA2_NATIVE_CENSUS_GAME_EXE', ''))
    ap.add_argument('--work', default='')
    a = ap.parse_args()
    name = 'native-command-bar-thumb' if a.case == 'thumb' else 'native-locked-input'
    if not a.data_dir or not os.path.isdir(a.data_dir) or not a.game_exe or not os.path.isfile(a.game_exe):
        print('%s: skipped (set RA2_NATIVE_CENSUS_DATA_DIR and RA2_NATIVE_CENSUS_GAME_EXE)' % name)
        return SKIP
    if a.binary.lower().endswith('.exe'):
        print('%s: skipped (the Windows build)' % name)
        return SKIP
    work = a.work or tempfile.mkdtemp(prefix='ra2-' + a.case + '-')
    os.makedirs(work, exist_ok=True)
    if a.case == 'thumb':
        problems = (thumb_run(a, work, 'zoom', 'uisize=1.25,zoomgestures=on') + thumb_run(a, work, 'interface', 'uisize=1.25')
                    + thumb_run(a, work, 'defaults', ''))
        what = 'the command bar stays at the screen\'s bottom and its thumbs close and open it'
    else:
        problems = lock_run(a, work)
        what = 'touch pan and pinch zoom wait for the intro\'s input lock'
    for x in problems:
        print('FAIL: ' + x)
    print('%s: %s: %s (%s)' % (name, 'FAIL' if problems else 'OK', what, work))
    return 1 if problems else 0


if __name__ == '__main__':
    sys.exit(main())
