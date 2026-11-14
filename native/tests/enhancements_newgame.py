#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest native-enhancements-newgame (lane C7, codex4 R700): a Game interface size accepted on
the Enhancements page applies to the next new game in the same process, as the page says
("From the next game"), not only after a restart or a load.

The app (--app, a fresh settings folder: the interface at 1x) goes through the real menus:
Options > Enhancements, Game interface size dropdown set to 1.5x, Ok, Main Menu, Single Player >
Skirmish > Start Game, and plays for a while. Passes when the new game made its sidebar
surface for 1.5x (link/native_enhancements.cpp Native_Sidebar_New_Game: 512 of 768 rows)
and the window shown in play is wider than the menu's (the host's larger sidebar column),
with no fault. The Windows build runs under Wine (tests/wine.py; its settings land in the
prefix's AppData), skipped without one; skipped without the installed game.

  enhancements_newgame.py --binary <ra2-native> --data-dir <game folder> --game-exe <game.exe> --work <dir>
                          [--wine-prefix <dir> [--wine <wine>]]
"""
import argparse
import glob
import os
import re
import shutil
import struct
import subprocess
import sys

import wine as winehost

# The shell's controls at 1024x768 (RA2_NATIVE_MSG_LOG): main menu Options, the Options
# dialog's Enhancements, the page's Game interface size dropdown (its arrow: the game's
# dropdowns open on their rightmost pixels) and its 1.5x entry, the page's Ok (the column's
# top slot), the Options dialog's Main Menu, then Single Player, Skirmish, Start Game
# (build/routeA sk.script).
SCRIPT = """d1+1500 click 834 472
d2+2000 click 834 388
d3+1500 click 588 304
d3+2500 click 500 357
d3+3500 click 834 304
d3+5500 click 834 639
d4+2000 click 834 304
d5+2000 click 834 388
d6+2500 click 834 346
d6+%d quit
"""


def width(path):
    with open(path, 'rb') as f:
        head = f.read(26)
    return struct.unpack_from('<i', head, 18)[0]


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--game-exe', default='')
    ap.add_argument('--work', required=True)
    ap.add_argument('--play-ms', type=int, default=30000, help='time in the new game before the script quits')
    ap.add_argument('--wine-prefix', default='')
    ap.add_argument('--wine', default='')
    a = ap.parse_args()
    if not a.data_dir or not os.path.isdir(a.data_dir) or not a.game_exe or not os.path.isfile(a.game_exe):
        print('enhancements-newgame: no installed game given; skipped')
        return 77
    prefix = None
    if a.wine_prefix:
        wine = winehost.find_wine(a.wine)
        if not wine:
            print('enhancements-newgame: Windows build and no Wine (set RA2_NATIVE_WINE); skipped')
            return 77
        prefix = winehost.Prefix(wine, a.wine_prefix)
    work = os.path.abspath(a.work)
    shutil.rmtree(work, ignore_errors=True)
    os.makedirs(os.path.join(work, 'home'))
    script = os.path.join(work, 'newgame.script')
    with open(script, 'w') as f:
        f.write(SCRIPT % (2500 + a.play_ms))
    env = dict(os.environ)
    for k in ('RA2_NATIVE_ENHANCEMENTS', 'RA2_NATIVE_SETUP', 'RA2_DATA_DIR'):
        env.pop(k, None)
    # A fresh settings folder (macOS: under HOME; Windows: the fresh prefix's AppData).
    env.update({'HOME': os.path.join(work, 'home'), 'RA2_GAME_EXE': a.game_exe, 'RA2_NATIVE_LOG_STDERR': '1',
                'RA2_NATIVE_MOVIE_SKIP': '1', 'RA2_NATIVE_IGNORE_FOCUS': '1',
                'RA2_NATIVE_WINDOW_DUMP': os.path.join(work, 'w'), 'RA2_NATIVE_WINDOW_DUMP_MS': '2000'})
    cmd = [a.binary, '--app', '--data-dir', a.data_dir, '--input-script=' + script, '--game-args=-noaudio']
    if prefix:
        cmd, env = prefix.command(cmd), prefix.env(env)
    try:
        p = subprocess.run(cmd, env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, errors='replace',
                           timeout=300)
    finally:
        if prefix:
            prefix.close()
    log = os.path.join(work, 'run.log')
    with open(log, 'w') as f:
        f.write(p.stdout)
    bad = []
    if p.returncode != 0:
        bad.append('exit %d' % p.returncode)
    if re.search(r'signal \d+|SIG(SEGV|BUS|ILL|ABRT)|crash', p.stdout):
        bad.append('a fault in the log')
    if 'enhancements: accepted vsync off, scaling sharp, interface 1.5x' not in p.stdout:
        bad.append('the page did not accept interface 1.5x')
    if not re.search(r'enhancements: new game: sidebar surface \d+ rows, was \d+', p.stdout):
        bad.append('the new game kept the sidebar surface it found (no "new game: sidebar surface" line)')
    if 'harness-view' not in p.stdout:
        bad.append('the new game never showed a frame')
    dumps = sorted(glob.glob(os.path.join(work, 'w.*.bmp')), key=lambda s: int(s.rsplit('.', 2)[1]))
    if len(dumps) < 2:
        bad.append('%d window dumps' % len(dumps))
    elif not width(dumps[-1]) > width(dumps[0]):
        bad.append('the window in play (%d wide) is not wider than the menu (%d): no larger sidebar column'
                   % (width(dumps[-1]), width(dumps[0])))
    for b in bad:
        print('enhancements-newgame: ' + b)
    print('enhancements-newgame: %s (see %s)' % ('ok' if not bad else 'FAILED', log))
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main())
