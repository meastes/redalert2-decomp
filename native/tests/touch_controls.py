#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest native-touch-controls (lane C10): which touch controls show where.

The app (--app, a fresh settings folder) with the touch controls on (RA2_NATIVE_TOUCH_UI=1, the
Mac's drawn controls; the iOS glass toolbar follows the same host state, host.cpp
show_controls) starts with its movies (no RA2_NATIVE_MOVIE_SKIP), then goes through the real
menus (Single Player > Skirmish > Start Game) into play. The input script taps the controls as
their buttons send them (`toolbar ITEM`: -2 Skip, -3 Keys, 3 A-Move, 6 the toolbar's Keys); a
hidden control's tap does nothing. Passes when, in order:

  movie  the Skip button alone is shown (toolbar and Keys button hidden) and its tap skips;
  menu   the Keys button alone is shown; the toolbar's A-Move tap does nothing; the Keys
         button's tap turns the keyboard on, a second off;
  play   the toolbar alone is shown; the Keys button's tap does nothing; Fire, Move, Add and
         A-Move are one either-or group (each of the six pairs: turning one on turns the lit
         one off; tapping the lit one turns it off); the toolbar's Keys works.

  touch_controls.py --binary <ra2-native> --data-dir <game folder> --game-exe <game.exe> --work <dir>

Skipped (77) without the installed game, and for the Windows build (no touch host there yet).
"""
import argparse
import os
import re
import shutil
import subprocess
import sys

# Skip taps every 2 s for the first 30 s (only the shown button acts), then the main menu's
# Single Player, Skirmish and Start Game at 1024x768 (enhancements_newgame.py's coordinates).
SCRIPT = ''.join('t%d toolbar -2\n' % ms for ms in range(2000, 30001, 2000)) + """d1+1500 toolbar 3
d1+2000 toolbar -3
d1+2500 toolbar -3
d1+3000 click 834 304
d2+2000 click 834 388
d3+2500 click 834 346
d3+20000 toolbar -3
d3+20300 toolbar 0
d3+20600 toolbar 1
d3+20900 toolbar 2
d3+21200 toolbar 3
d3+21500 toolbar 0
d3+21800 toolbar 2
d3+22100 toolbar 1
d3+22400 toolbar 3
d3+22700 toolbar 3
d3+23000 toolbar 6
d3+24000 quit
"""

STATE = re.compile(r'^host: touch controls: (\w+) \(toolbar (\w+), keys button (\w+), skip button (\w+)\)$')
WANT = {'movie': ('hidden', 'hidden', 'shown'), 'menu': ('hidden', 'shown', 'hidden'),
        'play': ('shown', 'hidden', 'hidden'), 'none': ('hidden', 'hidden', 'hidden')}
# What each phase must log while it is shown (in this order).
EXPECT = {
    'movie': ['host: skip button tapped', 'host: Escape pressed (touch control)'],
    'menu': ['host: toolbar item 3 tapped while hidden: nothing', 'host: keys button tapped', 'host: toggle KEYS on',
             'host: keys button tapped', 'host: toggle KEYS off'],
    'play': ['host: keys button tapped while hidden: nothing',
             'host: toggle FIRE on',
             'host: toggle FIRE off', 'host: toggle MOVE on',        # Fire -> Move
             'host: toggle MOVE off', 'host: toggle ADD on',         # Move -> Add
             'host: toggle ADD off', 'host: attack-move on',         # Add -> A-Move
             'host: attack-move off', 'host: toggle FIRE on',        # A-Move -> Fire
             'host: toggle FIRE off', 'host: toggle ADD on',         # Fire -> Add
             'host: toggle ADD off', 'host: toggle MOVE on',         # Add -> Move
             'host: toggle MOVE off', 'host: attack-move on',        # Move -> A-Move
             'host: attack-move off',                                # the lit one tapped
             'host: toggle KEYS on'],
}
# The modes' log lines in play, exactly (nothing else turns a mode on or off there).
MODE_LINES = re.compile(r'^host: (toggle (FIRE|MOVE|ADD) (on|off)|attack-move (on|off))')


def phases(lines):
    """[(phase, [host lines while it was shown])], checking each state line's controls"""
    out, problems = [], []
    for line in lines:
        m = STATE.match(line)
        if m:
            if (m.group(2), m.group(3), m.group(4)) != WANT[m.group(1)]:
                problems.append('wrong controls: ' + line)
            out.append((m.group(1), []))
        elif out and line.startswith('host: '):
            out[-1][1].append(line)
    return out, problems


def in_order(found, wanted):
    """the first wanted line not found after the previous one, or None"""
    i = 0
    for w in wanted:
        while i < len(found) and not found[i].startswith(w):
            i += 1
        if i == len(found):
            return w
        i += 1
    return None


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--game-exe', default='')
    ap.add_argument('--work', required=True)
    a = ap.parse_args()
    name = 'native-touch-controls'
    if not a.data_dir or not os.path.isdir(a.data_dir) or not a.game_exe or not os.path.isfile(a.game_exe):
        print('%s: skipped (no installed game given)' % name)
        return 77
    if a.binary.lower().endswith('.exe'):
        print('%s: skipped (the Windows build has no touch host yet)' % name)
        return 77
    work = os.path.abspath(a.work)
    shutil.rmtree(work, ignore_errors=True)
    os.makedirs(os.path.join(work, 'home'))
    script = os.path.join(work, 'controls.script')
    with open(script, 'w') as f:
        f.write(SCRIPT)
    env = dict(os.environ)
    for k in ('RA2_NATIVE_ENHANCEMENTS', 'RA2_NATIVE_SETUP', 'RA2_DATA_DIR', 'RA2_NATIVE_MOVIE_SKIP'):
        env.pop(k, None)
    env.update({'HOME': os.path.join(work, 'home'), 'RA2_GAME_EXE': a.game_exe, 'RA2_NATIVE_LOG_STDERR': '1',
                'RA2_NATIVE_IGNORE_FOCUS': '1', 'RA2_NATIVE_TOUCH_UI': '1'})
    cmd = [a.binary, '--app', '--data-dir', a.data_dir, '--input-script=' + script, '--game-args=-noaudio']
    p = subprocess.run(cmd, env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, errors='replace',
                       timeout=300)
    log = os.path.join(work, 'run.log')
    with open(log, 'w') as f:
        f.write(p.stdout)
    bad = []
    if p.returncode != 0:
        bad.append('exit %d' % p.returncode)
    if re.search(r'signal \d+|SIG(SEGV|BUS|ILL|ABRT)|crash|trap', p.stdout):
        bad.append('a fault in the log')
    found, problems = phases(p.stdout.splitlines())
    bad += problems
    order = [ph for ph, _ in found if ph != 'none']
    print('controls shown: ' + ' > '.join(ph for ph, _ in found))
    for ph in ('movie', 'menu', 'play'):
        if ph not in order:
            bad.append('the %s controls were never shown' % ph)
    if 'movie' in order and 'menu' in order and 'play' in order and not (
            order.index('movie') < order.index('menu') < order.index('play')):
        bad.append('not movie, then menu, then play: ' + ' > '.join(order))
    for ph, wanted in EXPECT.items():
        lines = [l for p_, ls in found if p_ == ph for l in ls]
        missing = in_order(lines, wanted)
        if missing:
            bad.append('%s: no "%s" (in order)' % (ph, missing))
    # Exclusivity: in play the modes change exactly as listed (no mode left on beside another),
    # and the same mode states hold after every tap.
    modes = [l for p_, ls in found if p_ == 'play' for l in ls if MODE_LINES.match(l)]
    wanted_modes = [w for w in EXPECT['play'] if MODE_LINES.match(w)]
    if [m[:len(w)] for m, w in zip(modes, wanted_modes)] != wanted_modes or len(modes) != len(wanted_modes):
        bad.append('play: the modes changed as %s, not %s' % (modes, wanted_modes))
    on = set()
    for m in modes:
        name = 'A-MOVE' if 'attack-move' in m else m.split()[2]
        (on.add if m.endswith(' on') or ' on (' in m else on.discard)(name)
        if len(on) > 1:
            bad.append('play: two modes on at once (%s) after "%s"' % (', '.join(sorted(on)), m))
            break
    for b in bad:
        print('%s: %s' % (name, b))
    print('%s: %s (see %s)' % (name, 'passed' if not bad else 'FAILED', log))
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main())
