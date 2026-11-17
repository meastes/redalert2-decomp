#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest one-game-saves / one-exe-saves (lane Y10; built with -DRA2_NATIVE_COMBINED=ON): both games of the
one-game app save in their original's format after the main menu's toggle, and each save loads back.

  run 1 (--game=ra2): the RA2 main menu -> toggle -> the YR main menu -> Skirmish: Yuri's Revenge saves
         at loop frame 300 (YRTOGGLE.SAV, gamemd.exe's format);
  run 2 (--game=yr): the YR main menu -> toggle -> the RA2 main menu -> Skirmish: Red Alert 2 saves at
         loop frame 300 (RA2TOGGLE.SAV, game.exe's format);
  run 3 (--game=yr): YRTOGGLE.SAV loaded from the YR menus (--load-save) and played 300 frames;
  run 4 (--game=ra2): RA2TOGGLE.SAV loaded from the RA2 menus and played 300 frames.

Checks: each save is a compound file with its game's internal version; each load reports it loaded and
runs 300 frames; no trap, fault, swizzle failure or type without a field map; no process of the run left.
Skipped (77) without the installed game (RA2 with Yuri's Revenge in --data-dir, game.exe given).

  one_game_saves.py --binary <ra2yr-native | ra2yr-one> --data-dir <game folder> --game-exe <game.exe> --work <dir>
"""
import argparse
import os
import re
import shutil
import struct
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(HERE, '..', '..', 'tests'))
sys.path.insert(0, os.path.join(HERE, '..', '..', 'tools'))
import game_process  # noqa: E402  (native/tests: the game's process group, lane C19)
import original_save_compare  # noqa: E402  (native/tools: the compound file's streams)

# As one_game_toggle.py: the toggle by its control id (0x7F5E; a push button above the bottom bar's left
# end since lane C23); Single Player, Skirmish, Start Game; then the mouse over the view's middle (a
# mouse left in a corner scrolls the view).
TOGGLE = 's1+1500 click @ctl=32606\n'
SKIRMISH = 's1+1500 click 720 220\ns2+1000 click 720 304\ns3+2500 click 720 262\ns3+3500 move 316 300\n'
CENTER = '1 move 316 300\n'
INI = '[Video]\nScreenWidth=800\nScreenHeight=600\n'
SKIRMISH_INI = ('[Skirmish]\nGameMode=1\nScenIndex=0\nGameSpeed=1\nCredits=10000\nUnitCount=10\nShortGame=yes\n'
                'Slot01=6,-2,-2\n' + ''.join('Slot%02d=1,-2,-2\n' % i for i in range(2, 8)))
VERSIONS = {'ra2': 166092, 'yr': 178328}      # SaveVersionInfo's internal version: game.exe, gamemd.exe 1.001


def internal_version(path):
    try:
        v = original_save_compare.streams(path).get('Internal Version')
    except (OSError, ValueError, struct.error):
        return None
    return struct.unpack('<I', v)[0] if v and len(v) == 4 else None


def run(a, name, game, scripts, args, env_extra, seconds):
    work = os.path.join(a.work, name)
    os.makedirs(os.path.join(work, 'shots'))
    paths = []
    for i, text in enumerate(scripts):
        p = os.path.join(work, 'e%d.script' % i)
        with open(p, 'w') as f:
            f.write(text)
        paths.append(p)
    env = dict(os.environ, RA2_NATIVE_SETTINGS=os.path.join(a.work, 'settings'), RA2_GAME_EXE=a.game_exe,
               RA2_NATIVE_WINDOW_DUMP=os.path.join(work, 'shots', 'w'), RA2_NATIVE_WINDOW_DUMP_MS='5000',
               RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_DIALOG_LOG='1', RA2_NATIVE_MOVIE_SKIP='1')
    for k in ('RA2_GAMEMD_EXE', 'RA2_NATIVE_INPUT_SCRIPT', 'RA2_NATIVE_SAVE_AT'):
        env.pop(k, None)
    env.update(env_extra)
    cmd = [a.binary, '--game=' + game, '--data-dir', a.data_dir, '--run-seconds=%d' % seconds,
           '--write-dir=' + a.writes, '--game-args=-noaudio'] + args
    if paths:
        cmd.append('--engine-scripts=' + ','.join(paths))
    log_path = os.path.join(work, 'run.log')
    with open(log_path, 'w') as log:
        game_process.run(cmd, env=env, stdout=log, stderr=subprocess.STDOUT, timeout=seconds + 120)
    text = open(log_path, errors='replace').read()
    ps = subprocess.run(['ps', '-axo', 'pid=,command='], capture_output=True, text=True).stdout
    left = [l for l in ps.splitlines() if a.writes in l and 'one_game_saves.py' not in l]
    return text, left


def problems(text, left, name):
    out = []
    lines = text.splitlines()
    traps = sorted(set(l.split('unrecovered ', 1)[1] for l in lines if 'unrecovered ' in l and ' reached' in l))
    stops = [l for l in lines if 'stopped:' in l]
    if traps or 'trap reached' in text or any(re.search(r'stopped: (signal|trap|fault)', l) for l in stops):
        out.append('%s: a trap or fault: %s %s' % (name, traps, stops))
    for pat, what in ((r'^swizzle: ', 'swizzle failures'), (r'original-save: (write: )?no layout for', 'types without a field map'),
                      (r'original-save: write: no original vtable', 'vtables not found')):
        hits = [l for l in lines if re.search(pat, l)]
        if hits:
            out.append('%s: %s: %s' % (name, what, hits[:3]))
    if left:
        out.append('%s: processes left: %s' % (name, left))
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--game-exe', default='')
    ap.add_argument('--work', required=True)
    a = ap.parse_args()
    if not a.data_dir or not os.path.exists(os.path.join(a.data_dir, 'gamemd.exe')):
        print('skipped: no Yuri\'s Revenge install (gamemd.exe) in --data-dir')
        return 77
    if not a.game_exe or not os.path.exists(a.game_exe):
        print('skipped: no game.exe (--game-exe)')
        return 77
    shutil.rmtree(a.work, ignore_errors=True)
    a.writes = os.path.join(a.work, 'w')
    os.makedirs(a.writes)
    with open(os.path.join(a.writes, 'RA2.INI'), 'w') as f:
        f.write(INI + '\n' + SKIRMISH_INI)
    with open(os.path.join(a.writes, 'RA2MD.INI'), 'w') as f:
        f.write('[Intro]\nPlay=no\n\n' + SKIRMISH_INI)
    failures = []
    saves = {'yr': 'YRTOGGLE.SAV', 'ra2': 'RA2TOGGLE.SAV'}

    # Runs 1 and 2: a toggle, then a skirmish in the other game, saved at frame 300.
    for name, first, other in (('1-ra2-to-yr', 'ra2', 'yr'), ('2-yr-to-ra2', 'yr', 'ra2')):
        save = saves[other]
        text, left = run(a, name, first, [TOGGLE, SKIRMISH], ['--run-to=never'], {'RA2_NATIVE_SAVE_AT': '300,' + save}, 75)
        failures.extend(problems(text, left, name))
        if 'engine: WinMain returned 0: swapping' not in text:
            failures.append('%s: no swap to the other game' % name)
        path = os.path.join(a.writes, save)
        if not re.search(r'frame 300: saved %s' % re.escape(save), text) or not os.path.exists(path):
            failures.append('%s: %s not saved at frame 300 (%s)' % (name, save, [l for l in text.splitlines() if 'harness-save' in l]))
        elif internal_version(path) != VERSIONS[other]:
            failures.append('%s: %s is not a compound file with %s\'s internal version (%s)' % (
                name, save, other, internal_version(path)))

    # Runs 3 and 4: each save loaded in its game, 300 frames.
    for name, game in (('3-yr-load', 'yr'), ('4-ra2-load', 'ra2')):
        save = saves[game]
        if not os.path.exists(os.path.join(a.writes, save)):
            continue
        text, left = run(a, name, game, [CENTER], ['--run-to=frames', '--frames=300', '--load-save=' + save], {}, 90)
        failures.extend(problems(text, left, name))
        if 'resume: loaded %s' % save not in text:
            failures.append('%s: %s was not loaded (%s)' % (name, save, [l for l in text.splitlines() if 'resume:' in l]))
        if 'run-to: reached frames' not in text:
            failures.append('%s: 300 frames after the load were not reached: %s' % (
                name, [l for l in text.splitlines() if 'stopped:' in l]))

    for f in failures:
        print('FAIL: ' + f)
    print('%s: both games saved after a toggle and loaded back (%s)' % ('FAIL' if failures else 'OK', a.work))
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
