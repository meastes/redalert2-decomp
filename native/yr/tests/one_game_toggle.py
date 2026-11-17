#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest one-game-toggle (lane Y5; off by default, built with -DRA2_NATIVE_COMBINED=ON): the combined
app (ra2yr-native, native/yr/README.md "One game") swaps Red Alert 2 and Yuri's Revenge in one
process through the main menu's toggle, with scripted input (one script per engine start).

  run 1 (--game=ra2): the RA2 main menu -> toggle -> the YR main menu (no start-up movie) -> toggle
         -> the RA2 main menu (no start-up movie) -> Single Player -> Skirmish -> Start Game: a
         skirmish runs in RA2 after two swaps;
  run 2 (no --game: the game chosen last, RA2): the RA2 main menu -> toggle -> the YR main menu ->
         Single Player -> Skirmish -> Start Game: a skirmish runs in YR after a swap;
  run 3 (no --game: YR, chosen last in run 2): YR starts, with its start-up movies, to its menu.

Checks: each run's engines start in that order, on one window, and each swap's engine skips the
start-up movies and reaches its menu sooner than a first start; the toggle is placed in both main
menus as a red menu button that shows its press (window dumps, lane C23); the skirmishes run game frames; the settings file holds the game chosen last; no trap, no
fault; no process of the run is left (native/tests/game_process.py ends the group; checked).
Skipped (77) without the installed game (RA2 with Yuri's Revenge in --data-dir, game.exe given).

  one_game_toggle.py --binary <ra2yr-native> --data-dir <game folder> --game-exe <game.exe> --work <dir>
"""
import argparse
import glob
import os
import re
import shutil
import subprocess
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', 'tests'))
import game_process  # noqa: E402  (native/tests: the game's process group, lane C19)

# Both games at 800x600 (RA2: the write folder's RA2.INI; YR: its installed RA2MD.INI, as the
# yr-native tests run it): Single Player, Skirmish (the single-player menu's third button) and Start
# Game (the setup's second slot) in the right-hand column. Each menu click waits for its dialog to
# settle ("s<k>", host.cpp; k counts that engine's dialogs). The toggle (lane C23: a red menu button
# above the bottom bar's left end) is clicked by its control id (0x7F5E, "@ctl="), with a window dump
# before the press and one while it is held (the screenshot check, toggle_look).
TOGGLE_ID = 0x7F5E
TOGGLE = ('s1+1500 dump\ns1+1700 ldown @ctl=%d\ns1+1900 dump\ns1+2300 lup @ctl=%d\n' % (TOGGLE_ID, TOGGLE_ID))
SKIRMISH = 's1+1500 click 720 220\ns2+1000 click 720 304\ns3+2500 click 720 262\n'
INI = '[Video]\nScreenWidth=800\nScreenHeight=600\n'


def run(a, name, scripts, game, seconds, run_to='never'):
    work = os.path.join(a.work, name)
    os.makedirs(os.path.join(work, 'shots'))
    paths = []
    for i, text in enumerate(scripts):
        p = os.path.join(work, 'e%d.script' % i)
        with open(p, 'w') as f:
            f.write(text)
        paths.append(p)
    env = dict(os.environ, RA2_NATIVE_SETTINGS=os.path.join(a.work, 'settings'), RA2_GAME_EXE=a.game_exe,
               RA2_NATIVE_WINDOW_DUMP=os.path.join(work, 'shots', 'w'), RA2_NATIVE_WINDOW_DUMP_MS='1000',
               RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_DIALOG_LOG='1')
    env.pop('RA2_GAMEMD_EXE', None)
    env.pop('RA2_NATIVE_INPUT_SCRIPT', None)
    cmd = [a.binary] + (['--game=' + game] if game else []) + [
        '--data-dir', a.data_dir, '--run-to=' + run_to, '--run-seconds=%d' % seconds,
        '--write-dir=' + a.writes, '--game-args=-noaudio']
    if paths:
        cmd.append('--engine-scripts=' + ','.join(paths))
    log_path = os.path.join(work, 'run.log')
    with open(log_path, 'w') as log:
        p = game_process.run(cmd, env=env, stdout=log, stderr=subprocess.STDOUT, timeout=seconds + 120)
    text = open(log_path, errors='replace').read()
    # No process of this run is left: nothing whose command line names this run's write folder.
    ps = subprocess.run(['ps', '-axo', 'pid=,command='], capture_output=True, text=True).stdout
    left = [l for l in ps.splitlines() if a.writes in l and 'one_game_toggle.py' not in l]
    return text, p.returncode, left, work


def read_bmp(path):
    """(width, height, rows of (r, g, b)) of an uncompressed 24/32-bit BMP (SDL_SaveBMP), top row first."""
    data = open(path, 'rb').read()
    offset, width, height, bpp = (int.from_bytes(data[10:14], 'little'), int.from_bytes(data[18:22], 'little', signed=True),
                                  int.from_bytes(data[22:26], 'little', signed=True), int.from_bytes(data[28:30], 'little'))
    step = bpp // 8
    stride = (width * step + 3) & ~3
    rows = []
    for y in range(abs(height)):
        base = offset + (abs(height) - 1 - y if height > 0 else y) * stride
        rows.append([(data[base + x * step + 2], data[base + x * step + 1], data[base + x * step]) for x in range(width)])
    return width, abs(height), rows


def toggle_look(text, work, name, failures):
    """The screenshot check: in each engine's main menu the toggle is a red menu button (red art over
    most of its rectangle, as the menu buttons' SDBTNANM frames are; the version-line clone it
    replaced had almost none) and looks different while pressed (the pressed frame)."""
    engine = None
    shots = {}
    placed = {}
    for line in text.splitlines():
        m = re.match(r'^one-game: engine (\d+):', line)
        if m:
            engine = int(m.group(1))
        m = re.match(r'^port: game toggle "[^"]+" at (-?\d+),(-?\d+) (\d+)x(\d+) in the main menu', line)
        if m and engine is not None and engine not in placed:
            placed[engine] = tuple(int(g) for g in m.groups())
        m = re.match(r'^host: window dump (\d+)', line)
        if m and engine is not None:
            shots.setdefault(engine, []).append(int(m.group(1)))
    for e in (0, 1):
        if e not in placed or len(shots.get(e, [])) < 2:
            failures.append('%s: engine %d: no toggle placement or no dumps around its press (%s, %s)'
                            % (name, e, placed.get(e), shots.get(e)))
            continue
        x, y, w, h = placed[e]
        looks = []
        for n in shots[e][:2]:
            path = os.path.join(work, 'shots', 'w-e%d.%d.bmp' % (e, n))
            if not os.path.exists(path):
                failures.append('%s: engine %d: no dump %s' % (name, e, path))
                break
            width, height, rows = read_bmp(path)
            sx, sy = width / 800.0, height / 600.0
            box = [rows[int((y + j + 0.5) * sy)][int((x + i + 0.5) * sx)] for j in range(h) for i in range(w)
                   if 0 <= int((y + j + 0.5) * sy) < height and 0 <= int((x + i + 0.5) * sx) < width]
            red = sum(1 for r, g, b in box if r >= 80 and r > 1.5 * g and r > 1.5 * b)
            looks.append((box, red / max(1, len(box))))
        if len(looks) < 2:
            continue
        share = looks[0][1]
        changed = sum(1 for p, q in zip(looks[0][0], looks[1][0]) if p != q) / max(1, len(looks[0][0]))
        print('%s: engine %d: toggle at %d,%d %dx%d: %.0f%% red, %.0f%% changed while pressed'
              % (name, e, x, y, w, h, share * 100, changed * 100))
        if share < 0.15:   # the button 27-28%, its frame and text the rest; the version-line clone 0%
            failures.append('%s: engine %d: the toggle is not a red menu button (%.0f%% red in its rectangle)' % (name, e, share * 100))
        if changed < 0.05:
            failures.append('%s: engine %d: the toggle shows no press (%.0f%% of it changed)' % (name, e, changed * 100))


def engines(text):
    """[(serial, game, after_swap)] from the thin host's lines."""
    return [(int(m.group(1)), m.group(2), bool(m.group(3)))
            for m in re.finditer(r'^one-game: engine (\d+): (Red Alert 2|Yuri\'s Revenge)( \(after a swap)?', text, re.M)]


def menu_times(text):
    return [int(x) for x in re.findall(r'^run-to: reached menu \+(\d+) ms', text, re.M)]


def common(text, left, name, failures):
    if 'trap reached' in text or re.search(r'stopped: (signal|trap|fault)', text):
        failures.append('%s: a trap or fault: %s' % (name, [l for l in text.splitlines() if 'stopped:' in l]))
    if left:
        failures.append('%s: processes left: %s' % (name, left))


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
        f.write(INI)
    failures = []

    # Run 1: RA2 -> YR -> RA2, then a skirmish in RA2.
    text, code, left, work = run(a, 'run1', [TOGGLE, TOGGLE, SKIRMISH], 'ra2', 110)
    common(text, left, 'run 1', failures)
    e = engines(text)
    want = [(0, 'Red Alert 2', False), (1, "Yuri's Revenge", True), (2, 'Red Alert 2', True)]
    if e != want:
        failures.append('run 1: engines %s, wanted %s' % (e, want))
    if text.count('port: start-up movies skipped until the first dialog') != 2:
        failures.append('run 1: the two swapped engines did not both skip the start-up movies')
    toggles = re.findall(r'^port: game toggle "(Play [^"]+)" at', text, re.M)
    if toggles[:3] != ["Play Yuri's Revenge", 'Play Red Alert 2', "Play Yuri's Revenge"]:
        failures.append('run 1: the toggle was not placed in each main menu: %s' % toggles)
    toggle_look(text, work, 'run 1', failures)
    t = menu_times(text)
    if len(t) < 3 or not (t[1] < t[0] and t[2] < t[0]):
        failures.append('run 1: the swapped engines did not reach their menus sooner than the first start: %s ms' % t)
    if text.count('engine: WinMain returned 0: swapping') != 2 or text.count('host: released for the next engine') != 2:
        failures.append('run 1: the two swaps did not both end their game through WinMain')
    frames = re.findall(r'^run-to:   frames run (\d+)', text, re.M)
    if not frames or int(frames[-1]) < 100:
        failures.append('run 1: the RA2 skirmish after the swaps ran %s frames' % (frames[-1] if frames else 'no'))
    if 'stopped: time limit' not in text:
        failures.append('run 1: did not run to its time limit')

    # Run 2: the game chosen last (RA2) -> YR, then a skirmish in YR.
    text, code, left, work = run(a, 'run2', [TOGGLE, SKIRMISH], None, 62)
    common(text, left, 'run 2', failures)
    if 'one-game: starting Red Alert 2 (the game chosen last)' not in text:
        failures.append('run 2: did not start the game chosen last (Red Alert 2)')
    e = engines(text)
    want = [(0, 'Red Alert 2', False), (1, "Yuri's Revenge", True)]
    if e != want:
        failures.append('run 2: engines %s, wanted %s' % (e, want))
    # YR's loop reports no frame count: the skirmish setup shown (its third dialog) and the window
    # presented through the game (dumps each second) after the Start click, as yr-native's smoke test.
    if len(re.findall(r'^dialog shown #3 ', text, re.M)) < 1:
        failures.append('run 2: YR\'s skirmish setup was not reached after the swap')
    dumps = len(glob.glob(os.path.join(work, 'shots', 'w-e1.*.bmp')))
    if dumps < 30:
        failures.append('run 2: too few YR window presents (%d dumps): the skirmish did not run' % dumps)
    if 'stopped: time limit' not in text:
        failures.append('run 2: did not run to its time limit')
    settings = open(os.path.join(a.work, 'settings')).read()
    if 'game=yr' not in settings:
        failures.append('run 2: the settings do not hold Yuri\'s Revenge as the game chosen last: %r' % settings)

    # Run 3: YR, chosen last, starts with its start-up movies, to its menu.
    text, code, left, work = run(a, 'run3', [], None, 60, run_to='menu')
    common(text, left, 'run 3', failures)
    if 'one-game: starting Yuri\'s Revenge (the game chosen last)' not in text:
        failures.append('run 3: did not start Yuri\'s Revenge, the game chosen last')
    if 'start-up movies skipped' in text:
        failures.append('run 3: a first start skipped its start-up movies')
    if 'stopped: target reached' not in text:
        failures.append('run 3: the YR menu was not reached')

    for f in failures:
        print('FAIL: ' + f)
    print('%s: one game, three runs (%s)' % ('FAIL' if failures else 'OK', a.work))
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
