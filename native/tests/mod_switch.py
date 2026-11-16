#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest native-mod-switch (lane C16, mod support level 2): the Enhancements page's Mod row.

Two empty mod folders (Another, TestMod) and, for level 3, ZipMod.zip (a zipped ZipMod folder with a
mod.ini naming it "Zipped Mod") in Mods in a fresh settings folder (macOS: under HOME;
Windows: the fresh Wine prefix's AppData\\Roaming\\RA2Native). Three app runs through the real
menus (Options > Enhancements):
  1. the .zip is unpacked (Apple platforms) and the Mod row lists None, Another, TestMod, Zipped Mod;
     TestMod chosen and Ok: the app saves the choice
     ("mod=TestMod" in its settings file) and ends to start again (RA2_NATIVE_NO_RELAUNCH=1: the
     test starts the next run itself);
  2. the app starts with TestMod (no --mod): "mod: TestMod from", its writes in Game/Mods/TestMod;
     None chosen and Ok: it ends to start again;
  3. the app starts with no mod, its writes in Game again.
The Windows build runs under Wine (tests/wine.py), skipped without one; skipped without the
installed game.

  mod_switch.py --binary <ra2-native> --data-dir <game folder> --game-exe <game.exe> --work <dir>
                [--wine-prefix <dir> [--wine <wine>]]
"""
import argparse
import glob
import os
import re
import shutil
import subprocess
import sys

import game_process
import wine as winehost

# The shell's controls at 1024x768: main menu Options, the Options dialog's Enhancements, the
# page's Mod dropdown (its arrow; the eleventh row) and its entries (None, Another, TestMod: 18
# pixels apart under the dropdown), the page's Ok. "s<k>": the k-th dialog shown and settled.
def choose(entry):
    return """s1+1000 click 834 472
s2+1000 click 834 388
s3+1000 click 588 547
s3+2000 click 500 %d
s3+3000 click 834 304
s4+4000 quit
""" % (565 + 18 * entry)


QUIT = "s1+2000 quit\n"


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--game-exe', default='')
    ap.add_argument('--work', required=True)
    ap.add_argument('--wine-prefix', default='')
    ap.add_argument('--wine', default='')
    a = ap.parse_args()
    if not a.data_dir or not os.path.isdir(a.data_dir) or not a.game_exe or not os.path.isfile(a.game_exe):
        print('mod-switch: no installed game given; skipped')
        return 77
    prefix = None
    if a.wine_prefix:
        wine = winehost.find_wine(a.wine)
        if not wine:
            print('mod-switch: Windows build and no Wine (set RA2_NATIVE_WINE); skipped')
            return 77
        prefix = winehost.Prefix(wine, a.wine_prefix)
    work = os.path.abspath(a.work)
    shutil.rmtree(work, ignore_errors=True)
    home = os.path.join(work, 'home')
    os.makedirs(home)
    env = dict(os.environ)
    for k in ('RA2_NATIVE_ENHANCEMENTS', 'RA2_NATIVE_SETUP', 'RA2_DATA_DIR', 'RA2_NATIVE_MOD'):
        env.pop(k, None)
    env.update({'HOME': home, 'RA2_GAME_EXE': a.game_exe, 'RA2_NATIVE_LOG_STDERR': '1', 'RA2_NATIVE_MOVIE_SKIP': '1',
                'RA2_NATIVE_IGNORE_FOCUS': '1', 'RA2_NATIVE_NO_RELAUNCH': '1'})
    if prefix:
        prefix.make()
        roaming = [d for d in glob.glob(os.path.join(prefix.folder, 'drive_c', 'users', '*', 'AppData', 'Roaming'))
                   if os.path.basename(os.path.dirname(os.path.dirname(d))) != 'Public']
        if not roaming:
            prefix.close()
            print('mod-switch: no AppData\\Roaming in the Wine prefix; FAILED')
            return 1
        settings_dir = os.path.join(roaming[0], 'RA2Native')
    else:
        settings_dir = os.path.join(home, 'Library', 'Application Support', 'RA2Native')
    for name in ('TestMod', 'Another'):
        os.makedirs(os.path.join(settings_dir, 'Mods', name))
    # Level 3: a mod as a .zip of its folder with a mod.ini (unpacked at the app's start on Apple
    # platforms; its name on the row, sorted by its folder name ZipMod after TestMod).
    import zipfile
    with zipfile.ZipFile(os.path.join(settings_dir, 'Mods', 'ZipMod.zip'), 'w', zipfile.ZIP_DEFLATED) as z:
        z.writestr('ZipMod/mod.ini', '[Mod]\nName=Zipped Mod\nDescription=From a zip.\n')
        z.writestr('ZipMod/readme.txt', 'a test mod\n')
    outs = []
    bad = []
    try:
        for k, text in enumerate([choose(2), choose(0), QUIT]):
            script = os.path.join(work, 'run%d.script' % k)
            with open(script, 'w') as f:
                f.write(text)
            cmd = [a.binary, '--app', '--data-dir', a.data_dir, '--input-script=' + script, '--game-args=-noaudio']
            run_env = env
            if prefix:
                cmd, run_env = prefix.command(cmd), prefix.env(env)
            try:
                p = game_process.run(cmd, env=run_env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True,
                                     errors='replace', timeout=300)
                out, code = p.stdout, p.returncode
            except subprocess.TimeoutExpired as e:
                out, code = (e.stdout or b'').decode('utf-8', 'replace') if isinstance(e.stdout, bytes) else (e.stdout or ''), -1
            with open(os.path.join(work, 'run%d.log' % k), 'w') as f:
                f.write(out)
            outs.append(out)
            if code != 0:
                bad.append('run %d: exit %d' % (k + 1, code))
            if re.search(r'signal \d+|SIG(SEGV|BUS|ILL|ABRT)|crash', out):
                bad.append('run %d: a fault in the log' % (k + 1))
    finally:
        if prefix:
            prefix.close()
    writes = re.compile(r'^app: writes go to (.*)$', re.M)
    one, two, three = (outs + ['', '', ''])[:3]
    if not prefix:
        if 'mods: ZipMod.zip unpacked into Mods/ZipMod' not in one:
            bad.append('run 1: the .zip was not unpacked')
        if 'enhancements: Mod row: None, Another, TestMod, Zipped Mod' not in one:
            bad.append('run 1: the Mod row did not list None, Another, TestMod and the zip\'s mod by its mod.ini name')
    if 'enhancements: mod TestMod chosen (running none)' not in one:
        bad.append('run 1: TestMod was not chosen on the Mod row')
    if 'not started (RA2_NATIVE_NO_RELAUNCH)' not in one:
        bad.append('run 1: the app did not end to start again')
    settings = os.path.join(settings_dir, 'settings')
    if not os.path.isfile(settings) or 'mod=' not in open(settings).read():
        bad.append('the choice was not saved in the settings file')
    if not re.search(r'^mod: TestMod from .*Mods[/\\]TestMod$', two, re.M):
        bad.append('run 2: the app did not start with TestMod')
    w = writes.findall(two)
    if not w or not re.search(r'Mods[/\\]TestMod$', w[-1]):
        bad.append('run 2: the writes did not go to Game/Mods/TestMod (%s)' % (w[-1] if w else 'none'))
    if 'enhancements: mod none chosen (running TestMod)' not in two:
        bad.append('run 2: None was not chosen on the Mod row')
    if re.search(r'^mod: ', three, re.M):
        bad.append('run 3: a mod was used after None was chosen')
    w = writes.findall(three)
    if not w or re.search(r'Mods[/\\]', w[-1]):
        bad.append('run 3: the writes did not go back to Game (%s)' % (w[-1] if w else 'none'))
    for b in bad:
        print('mod-switch: ' + b)
    print('mod-switch: %s (see %s)' % ('ok' if not bad else 'FAILED', work))
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main())
