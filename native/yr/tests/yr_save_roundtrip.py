#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest yr-native-save (lane Y10; built with -DRA2_NATIVE_YR=ON): Yuri's Revenge saves in gamemd.exe's
format (native/yr/README.md "Saves in gamemd.exe's format"), on the Mac:

  1. a skirmish from the menus (scripted input, one easy opponent) saved at loop frame --save-frame
     (RA2_NATIVE_SAVE_AT, glue/yr_app_hooks.cpp) as YRSAVE1.SAV;
  2. YRSAVE1.SAV loaded from the menus (--load-save: Single Player, Load Saved Game) and played 300
     frames, saved again at frame 50 as YRSAVE2.SAV (a save written after a load keeps the ids);
  3. YRSAVE2.SAV loaded and played 300 frames;
  4. a save the user's installed Yuri's Revenge made, if the data folder holds one (a compound file
     whose Internal Version is YR's, read in place, never copied): loaded and played 300 frames;
     skipped (reported) when there is none.

Checks: each save is a compound file with YR's internal version; each load reports it loaded and
reaches 300 frames; no trap or fault; no swizzle failure; no type read or written without a field
map; no vtable left unwritten. Skipped (77) without the installed game (gamemd.exe in --data-dir).

  yr_save_roundtrip.py --binary <yr-native> --data-dir <game folder> --work <dir> [--save-frame 500]
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

# 800x600: Single Player, Skirmish, Start Game (as yr_skirmish_long.py).
SCRIPT = 's1+1000 click 720 220\ns2+1000 click 720 304\ns3+2000 click 720 262\ns3+3000 move 316 300\n'
SETTINGS = ('[Intro]\nPlay=no\n\n[Skirmish]\nGameMode=1\nScenIndex=0\nGameSpeed=1\nCredits=10000\nUnitCount=10\n'
            'ShortGame=yes\nSuperWeaponsAllowed=yes\nBuildOffAlly=yes\nMCVRepacks=yes\nCratesAppear=yes\n'
            'Slot01=6,-2,-2\n' + ''.join('Slot%02d=1,-2,-2\n' % i for i in range(2, 8)))
YR_VERSION = 178328     # SaveVersionInfo's internal version of gamemd.exe 1.001 (0x2B898)


def internal_version(path):
    try:
        s = original_save_compare.streams(path)
    except (OSError, ValueError, struct.error):
        return None
    v = s.get('Internal Version')
    return struct.unpack('<I', v)[0] if v and len(v) == 4 else None


def run(a, name, args, env_extra, seconds):
    work = os.path.join(a.work, name)
    os.makedirs(os.path.join(a.work, 'shots'), exist_ok=True)
    env = dict(os.environ, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_DIALOG_LOG='1',
               RA2_NATIVE_WINDOW_DUMP=os.path.join(a.work, 'shots', name), RA2_NATIVE_WINDOW_DUMP_MS='5000')
    env.pop('RA2_NATIVE_SAVE_AT', None)
    env.update(env_extra)
    cmd = [a.binary, '--data-dir', a.data_dir, '--run-seconds=%d' % seconds, '--write-dir=' + a.writes,
           '--game-args=-noaudio -CD.'] + args
    log = os.path.join(a.work, name + '.log')
    with open(log, 'w') as f:
        game_process.run(cmd, env=env, stdout=f, stderr=subprocess.STDOUT, timeout=seconds + 120)
    return open(log, errors='replace').read()


def problems(text, name):
    out = []
    lines = text.splitlines()
    stop = [l for l in lines if 'stopped:' in l]
    traps = sorted(set(l.split('unrecovered ', 1)[1] for l in lines if 'unrecovered ' in l and ' reached' in l))
    if traps or any(re.search(r'stopped: (signal|trap|fault)', l) for l in stop):
        out.append('%s: a trap or fault: %s %s' % (name, traps, stop[-1] if stop else ''))
    for pat, what in ((r'^swizzle: ', 'swizzle failures'), (r'original-save: (write: )?no layout for', 'types without a field map'),
                      (r'original-save: write: no original vtable', 'vtables not found in gamemd.exe')):
        hits = [l for l in lines if re.search(pat, l)]
        if hits:
            out.append('%s: %s: %s' % (name, what, hits[:3]))
    return out


def load_check(text, name, save, failures):
    failures.extend(problems(text, name))
    if 'resume: loaded %s' % save not in text:
        failures.append('%s: %s was not loaded (%s)' % (name, save, [l for l in text.splitlines() if 'resume:' in l]))
    if 'run-to: reached frames' not in text:
        failures.append('%s: 300 frames after the load were not reached: %s' % (
            name, [l for l in text.splitlines() if 'stopped:' in l]))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--work', required=True)
    ap.add_argument('--save-frame', type=int, default=500)
    a = ap.parse_args()
    if not a.data_dir or not os.path.exists(os.path.join(a.data_dir, 'gamemd.exe')):
        print('skipped: no Yuri\'s Revenge install (gamemd.exe) in --data-dir')
        return 77
    shutil.rmtree(a.work, ignore_errors=True)
    a.writes = os.path.join(a.work, 'w')
    os.makedirs(a.writes)
    with open(os.path.join(a.writes, 'RA2MD.INI'), 'w') as f:
        f.write(SETTINGS)
    script = os.path.join(a.work, 'skirmish.script')
    with open(script, 'w') as f:
        f.write(SCRIPT)
    # A loaded game: the mouse over the middle of the view at its first frame (left at 0,0 the view
    # scrolls to the north-west edge, as a player's mouse in the corner does).
    center = os.path.join(a.work, 'center.script')
    with open(center, 'w') as f:
        f.write('1 move 316 300\n')
    load = ['--run-to=frames', '--frames=300', '--input-script=' + center]
    failures = []

    # 1. A skirmish saved at frame N.
    text = run(a, '1-skirmish', ['--run-to=harness-save', '--input-script=' + script],
               {'RA2_NATIVE_SAVE_AT': '%d,YRSAVE1.SAV' % a.save_frame}, 150)
    failures.extend(problems(text, 'skirmish'))
    first = os.path.join(a.writes, 'YRSAVE1.SAV')
    if 'harness-save: frame %d: saved YRSAVE1.SAV' % a.save_frame not in text or not os.path.exists(first):
        failures.append('skirmish: not saved at frame %d' % a.save_frame)
    elif internal_version(first) != YR_VERSION:
        failures.append('skirmish: YRSAVE1.SAV is not a compound file with YR\'s internal version (%s)' % internal_version(first))

    # 2. Loaded, 300 frames, saved again at frame 50.
    if os.path.exists(first):
        text = run(a, '2-load', load + ['--load-save=YRSAVE1.SAV'],
                   {'RA2_NATIVE_SAVE_AT': '50,YRSAVE2.SAV'}, 120)
        load_check(text, 'load 1', 'YRSAVE1.SAV', failures)
        if 'harness-save: frame 50: saved YRSAVE2.SAV' not in text:
            failures.append('load 1: not saved again at frame 50')

    # 3. The save written after a load, loaded, 300 frames.
    second = os.path.join(a.writes, 'YRSAVE2.SAV')
    if os.path.exists(second):
        text = run(a, '3-load-again', load + ['--load-save=YRSAVE2.SAV'], {}, 120)
        load_check(text, 'load 2', 'YRSAVE2.SAV', failures)

    # 4. A save of the user's installed Yuri's Revenge, if there is one (read in place).
    originals = sorted(f for f in os.listdir(a.data_dir)
                       if f.lower().endswith('.sav') and not f.startswith('._') and internal_version(os.path.join(a.data_dir, f)) == YR_VERSION)
    if originals:
        save = originals[0]
        text = run(a, '4-original', load + ['--load-save=' + save], {}, 120)
        load_check(text, 'installed game\'s save', save, failures)
        original = 'the installed game\'s %s loaded and played' % save
    else:
        original = 'no save of the installed Yuri\'s Revenge in the data folder: its read was skipped'

    for f in failures:
        print('FAIL: ' + f)
    print('%s: saved at frame %d, loaded, saved after the load, loaded again; %s (%s)' % (
        'FAIL' if failures else 'OK', a.save_frame, original, a.work))
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
