#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest yr-native-type-select (lane N3; built with -DRA2_NATIVE_YR=ON): the command bar's "select all
units of this type" button and the TypeSelect key.

The user's Duo report (2026-11-19): the bar's TypeSelect button crashed Yuri's Revenge. TabClass::AI
(gamemd.exe 0x006D0680) and the key's quick press (FUN_00732CC0) call FUN_00732950, which the YR tree
has not recovered: the link closure's trap. glue/uicmnds_native.inc has a NON-EXACT stand-in from
gamemd.exe's instructions.

Through the menus into a skirmish (yr_combat.py's route), with scripted clicks on the bar's button
(input script `click @cmd=TypeSelect`, glue/yr_app_hooks.cpp) and harness orders
(glue/yr_harness.cpp select/groups):
  frame 100  the first tank alone selected; 150 the button clicked; 200 groups: every tank selected;
  frame 250  nothing selected; 260 the button clicked (nothing to select: the message only);
  frame 300  groups: no tank selected;
  frame 320  the first tank alone selected; 350 the TypeSelect key (T) pressed and released;
  frame 400  groups: every tank selected.

  yr_type_select.py --binary <yr-native> --data-dir <game folder> --work <dir>
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
combat = importlib.import_module('yr_combat')
SCRIPT = combat.SCRIPT + ''.join(l + '\n' for l in (
    '150 click @cmd=TypeSelect', '260 click @cmd=TypeSelect', '350 key t'))
ORDERS = ('100 select MTNK 0; 120 groups MTNK; 200 groups MTNK; 250 select NONE 0; 300 groups MTNK; '
          '320 select MTNK 0; 400 groups MTNK')
GROUPS = re.compile(r'^harness-groups MTNK((?: -?\d+\*?)+)$', re.M)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--work', required=True)
    ap.add_argument('--seconds', type=int, default=60)
    a = ap.parse_args()
    if not a.data_dir or not os.path.exists(os.path.join(a.data_dir, 'gamemd.exe')):
        print('skipped: no Yuri\'s Revenge install (gamemd.exe) in --data-dir')
        return 77
    if a.binary.lower().endswith('.exe'):
        print('skipped: run on the Mac build (the Windows build has the same game code)')
        return 77
    shutil.rmtree(a.work, ignore_errors=True)
    os.makedirs(os.path.join(a.work, 'w'))
    with open(os.path.join(a.work, 'w', 'RA2MD.INI'), 'w') as f:
        f.write(combat.SETTINGS)
    script = os.path.join(a.work, 'type.script')
    with open(script, 'w') as f:
        f.write(SCRIPT)
    env = dict(os.environ, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_MSG_LOG='0',
               RA2_NATIVE_ORDERS=ORDERS)
    env.pop('RA2_NATIVE_FORCE_END', None)
    cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=never', '--run-seconds=%d' % a.seconds,
           '--write-dir=' + os.path.join(a.work, 'w'), '--input-script=' + script, '--game-args=-noaudio -CD.']
    with open(os.path.join(a.work, 'run.log'), 'w') as log:
        game_process.run(cmd, env=env, stdout=log, stderr=subprocess.STDOUT, timeout=a.seconds + 120)
    text = open(os.path.join(a.work, 'run.log'), errors='replace').read()
    groups = [m.group(1).split() for m in GROUPS.finditer(text)]
    failures = []
    if len(groups) < 4:
        failures.append('only %d of 4 "harness-groups MTNK" lines (the game did not reach frame 400)' % len(groups))
    else:
        first, after_button, after_empty, after_key = groups[:4]
        print('first tank alone: %s' % first)
        print('after the button: %s' % after_button)
        print('nothing selected, after the button: %s' % after_empty)
        print('after the key: %s' % after_key)
        if len(first) < 2 or not first[0].endswith('*') or any(g.endswith('*') for g in first[1:]):
            failures.append('the first tank was not selected alone: %s' % first)
        if not all(g.endswith('*') for g in after_button):
            failures.append('the button did not select every tank: %s' % after_button)
        if any(g.endswith('*') for g in after_empty):
            failures.append('the button selected tanks with nothing selected: %s' % after_empty)
        if not all(g.endswith('*') for g in after_key):
            failures.append('the TypeSelect key did not select every tank: %s' % after_key)
    if 'input-script: cannot' in text:
        failures.append('the input script was not read')
    bad = sorted(set(l for l in text.splitlines() if combat.BAD.search(l)))
    if bad:
        failures.append('traps or faults: %s' % '; '.join(bad[:5]))
    for f in failures:
        print('FAIL: ' + f)
    print('%s: the command bar\'s TypeSelect button and key (%s)' % ('FAIL' if failures else 'OK', a.work))
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
