#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest yr-native-save-dialog (lane N3; built with -DRA2_NATIVE_YR=ON): a save made through the menus, as a
player makes it: a skirmish, Escape (the options menu), Save Game, a description typed into the field, Save.

The user's Mac report (2026-11-19): saving in Yuri's Revenge failed with an error. The Save dialog's new file name
(SAVExxxx.SAV, loaddlg.cpp) lived in an array declared inside a block and used after it; natively its stack slot
was reused, so the save went to a garbage name. fixups.json declares it in the enclosing block.
yr-native-save saves through the harness (Save_Game with a fixed name) and did not see it.

Checks: a file named SAVE????.SAV in the write folder, no trap or fault.

  yr_save_dialog.py --binary <yr-native> --data-dir <game folder> --work <dir>
Skipped (77) without the installed game (gamemd.exe in --data-dir) and for the Windows build.
"""
import argparse
import importlib
import os
import re
import shutil
import subprocess
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', 'tests'))
import game_process  # noqa: E402

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
standins = importlib.import_module('yr_standins')
# s4: the options menu (dialog 4), s5: the Save dialog; 1311 Save Game, 1318 the description, 1735 Save.
SCRIPT = standins.SCRIPT + ''.join(l + '\n' for l in (
    '300 key Escape', 's4+1500 click @ctl=1311', 's5+1500 click @ctl=1318', 's5+2000 key t', 's5+2200 key e',
    's5+2400 key s', 's5+3000 click @ctl=1735'))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--work', required=True)
    ap.add_argument('--seconds', type=int, default=70)
    a = ap.parse_args()
    if not a.data_dir or not os.path.exists(os.path.join(a.data_dir, 'gamemd.exe')):
        print('skipped: no Yuri\'s Revenge install (gamemd.exe) in --data-dir')
        return 77
    if a.binary.lower().endswith('.exe'):
        print('skipped: run on the Mac build (the Windows build has the same game code)')
        return 77
    shutil.rmtree(a.work, ignore_errors=True)
    writes = os.path.join(a.work, 'w')
    os.makedirs(writes)
    with open(os.path.join(writes, 'RA2MD.INI'), 'w') as f:
        f.write(standins.SETTINGS)
    script = os.path.join(a.work, 'save.script')
    with open(script, 'w') as f:
        f.write(SCRIPT)
    env = dict(os.environ, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_MSG_LOG='0',
               RA2_NATIVE_DIALOG_LOG='1', RA2_NATIVE_SCRIPT_AS_USER='1')
    env.pop('RA2_NATIVE_FORCE_END', None)
    env.pop('RA2_NATIVE_ORDERS', None)
    cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=never', '--run-seconds=%d' % a.seconds,
           '--write-dir=' + writes, '--input-script=' + script, '--game-args=-noaudio -CD.']
    with open(os.path.join(a.work, 'run.log'), 'w') as log:
        game_process.run(cmd, env=env, stdout=log, stderr=subprocess.STDOUT, timeout=a.seconds + 120)
    text = open(os.path.join(a.work, 'run.log'), errors='replace').read()
    failures = []
    files = sorted(os.listdir(writes))
    saves = [n for n in files if re.match(r'^SAVE[0-9A-F]{4}\.SAV$', n, re.I)]
    print('write folder: %s' % files)
    if not saves:
        failures.append('no SAVE????.SAV written through the Save dialog')
    others = [n for n in files if n not in saves and n not in ('RA2MD.INI', 'YRSERIAL.TXT')]
    if others:
        failures.append('unexpected files (a garbage save name?): %s' % others)
    bad = sorted(set(l for l in text.splitlines() if standins.BAD.search(l)))
    if bad:
        failures.append('traps or faults: %s' % '; '.join(bad[:5]))
    for f in failures:
        print('FAIL: ' + f)
    print('%s: a save through the menus (%s)' % ('FAIL' if failures else 'OK', a.work))
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
