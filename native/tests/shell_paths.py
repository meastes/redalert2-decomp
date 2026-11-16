#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest native-shell-paths (lane C21): menu and in-game paths whose behaviour rewrites the
earlier tests never reached (native/behaviour_rewrites.json; found with coverage_runs.py).

  --case name-filter   Main menu > Network: the lobby's name edit (the NewEdit list 1345, procedure
                       0x005F22A0) is given ',' as its character filter (message 0x4EB) and the
                       narrow-only flag (0x4D1) by the lobby (netdlg2.cpp, message 0x497). Typing
                       ",x,y" (RA2_NATIVE_SCRIPT_AS_USER: each key also types its character) must
                       deliver four WM_CHAR and change the text twice (two EN_CHANGE/EN_UPDATE
                       pairs to the dialog): the commas refused.
  --case options       Main menu > Options (Main_Options_Proc 0x00542590): Tooltips, Target
                       lines and Show hidden toggled, the detail and scroll-rate sliders clicked,
                       Main Menu. RA2.INI's [Options] ToolTips, UnitActionLines and ShowHidden
                       must differ from a run that opens and closes the page untouched.
  --case ingame-keys   A skirmish (--skirmish '#5') with the in-game commands: a band selection,
                       Ctrl+1 and 1 twice, T, G, Ctrl+Shift click, right button, waypoint mode,
                       chat, cheer, beacon, the diplomacy screen (Tab: "GUI:DiplomacyMenu") and its
                       Return, a cameo built, held and cancelled. The game must reach its frame
                       target after the diplomacy screen.
Every case fails on a trap, fault or unrecovered call. Skipped (77) without the installed game.

  shell_paths.py --case <case> --binary <ra2-native> --data-dir <game folder> --game-exe <game.exe> --work <dir>
"""
import argparse
import os
import re
import shutil
import subprocess
import sys

import game_process

SKIP = 77
BAD = re.compile(r'unrecovered|trap|signal \d+|SIG(SEGV|BUS|ILL|ABRT|FPE)|crash:|abort', re.I)

# The shell at 1024x768 (RA2_NATIVE_MSG_LOG): main menu Network (756,367 156x42) and Options
# (756,451); the lobby's name edit (list 1345 at 328,161 135x19); the Options page's Target lines
# (229,429), Tooltips (229,405) and Show hidden (229,453) check boxes, its detail (229,217 181x22)
# and scroll rate (447,429 181x22) sliders and Main Menu (756,619).
NAME_FILTER = ['s1+1000 click 834 388', 's2+3000 click 400 170', 's2+3500 key End', 's2+3700 key ,',
               's2+3900 key X', 's2+4100 key ,', 's2+4300 key Y']
OPTIONS_UNTOUCHED = ['s1+1000 click 834 472', 's2+1500 click 834 640']
OPTIONS_TOGGLED = ['s1+1000 click 834 472', 's2+800 click 235 437', 's2+1200 click 235 413', 's2+1600 click 235 461',
                   's2+2000 click 380 228', 's2+2400 click 600 440', 's2+2800 click 834 640']
# The diplomacy screen's Return (877,702 125x25); a cameo of the sidebar's first column (908,252).
INGAME_KEYS = [
    '60 drag 150 150 750 650', '70 keydown Left_Ctrl', '71 key 1', '72 keyup Left_Ctrl', '80 key 1', '85 key 1',
    '90 key T', '95 key T', '100 key G',
    '110 drag 150 150 750 650', '120 keydown Left_Ctrl', '121 keydown Left_Shift', '123 click 500 420',
    '127 keyup Left_Shift', '128 keyup Left_Ctrl',
    '135 rdown 500 400', '141 rup 500 400', '145 rclick 450 380',
    '150 key Z', '155 click 300 300', '160 click 350 320', '165 click 400 300', '170 key Z', '172 key Return',
    '180 keydown Left_Ctrl', '181 key C', '182 keyup Left_Ctrl', '185 key C', '190 key B', '193 click 420 420',
    '200 key Tab', 'd1+2500 click 940 714',
    '300 click 908 252', '330 rclick 908 252', '360 click 908 252', '390 rclick 908 252', '392 rclick 908 252',
]


def launch(a, work, name, script, args, env_extra, seconds):
    folder = os.path.join(work, name)
    os.makedirs(os.path.join(folder, 'w'), exist_ok=True)
    path = os.path.join(folder, 'input.script')
    with open(path, 'w') as f:
        f.write('\n'.join(script) + '\n')
    env = dict(os.environ, RA2_GAME_EXE=a.game_exe, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_IGNORE_FOCUS='1',
               RA2_NATIVE_CRASH_TRACE='1')
    for k in ('RA2_NATIVE_TRAP_CONTINUE', 'RA2_NATIVE_SAVE_AT', 'RA2_NATIVE_FORCE_END', 'RA2_NATIVE_ORDERS'):
        env.pop(k, None)
    env.update(env_extra)
    cmd = [a.binary, '--data-dir', a.data_dir, '--write-dir=' + os.path.join(folder, 'w'),
           '--input-script=' + path] + args
    try:
        p = game_process.run(cmd, env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, errors='replace',
                             timeout=seconds + 60)
        out, code = p.stdout, p.returncode
    except subprocess.TimeoutExpired as x:
        out = x.stdout.decode('utf-8', 'replace') if isinstance(x.stdout, bytes) else (x.stdout or '')
        code = -1
    with open(os.path.join(folder, 'run.log'), 'w') as f:
        f.write(out)
    return out, code, folder


def ini_options(folder):
    values, section = {}, ''
    path = os.path.join(folder, 'w', 'RA2.INI')
    if not os.path.isfile(path):
        return values
    for line in open(path, errors='replace'):
        line = line.strip()
        if line.startswith('['):
            section = line
        elif '=' in line and section == '[Options]':
            k, v = line.split('=', 1)
            values[k.strip()] = v.strip()
    return values


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--case', required=True, choices=('name-filter', 'options', 'ingame-keys'))
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default=os.environ.get('RA2_NATIVE_CENSUS_DATA_DIR', ''))
    ap.add_argument('--game-exe', default=os.environ.get('RA2_NATIVE_CENSUS_GAME_EXE', ''))
    ap.add_argument('--work', required=True)
    a = ap.parse_args()
    tag = 'shell-paths %s' % a.case
    if not a.data_dir or not os.path.isdir(a.data_dir) or not a.game_exe or not os.path.isfile(a.game_exe):
        print('%s: skipped (set RA2_NATIVE_CENSUS_DATA_DIR and RA2_NATIVE_CENSUS_GAME_EXE)' % tag)
        return SKIP
    work = os.path.abspath(a.work)
    shutil.rmtree(work, ignore_errors=True)
    os.makedirs(work)
    menu = ['--run-to=seconds', '--game-args=-noaudio -CD.']
    bad, logs = [], []
    if a.case == 'name-filter':
        out, code, folder = launch(a, work, 'lobby', NAME_FILTER, menu + ['--run-seconds=22'],
                                   dict(RA2_NATIVE_MSG_LOG='1', RA2_NATIVE_SCRIPT_AS_USER='1'), 22)
        logs.append(folder)
        chars = re.findall(r'id 1345 tmpl \d+: 0x102 (\w+) ', out)
        changes = len(re.findall(r': 0x111 3000541 ', out))
        updates = len(re.findall(r': 0x111 4000541 ', out))
        if not re.search(r'id 1345 tmpl \d+: 0x4eb ', out):
            bad.append('the lobby never gave the name edit its filter (message 0x4EB)')
        if chars != ['2c', '78', '2c', '79']:
            bad.append('the name edit got WM_CHAR %s, not , x , y' % chars)
        if changes != 2 or updates != 2:
            bad.append('%d EN_CHANGE / %d EN_UPDATE for ",x,y" (2 each: the commas refused)' % (changes, updates))
        bad += [l for l in out.splitlines() if BAD.search(l)][:10]
    elif a.case == 'options':
        base_out, base_code, base = launch(a, work, 'untouched', OPTIONS_UNTOUCHED, menu + ['--run-seconds=18'], {}, 18)
        out, code, folder = launch(a, work, 'toggled', OPTIONS_TOGGLED, menu + ['--run-seconds=20'], {}, 20)
        logs += [base, folder]
        before, after = ini_options(base), ini_options(folder)
        for key in ('ToolTips', 'UnitActionLines', 'ShowHidden'):
            if key not in before or key not in after:
                bad.append('RA2.INI [Options] %s missing (untouched %r, toggled %r)' % (key, before.get(key), after.get(key)))
            elif before[key] == after[key]:
                bad.append('RA2.INI [Options] %s=%s unchanged by its check box' % (key, after[key]))
        bad += [l for l in (base_out + out).splitlines() if BAD.search(l)][:10]
    else:
        frames = 900
        out, code, folder = launch(a, work, 'skirmish', INGAME_KEYS,
                                   ['--skirmish=#5', '--skirmish-frames=%d' % frames, '--skirmish-order=both',
                                    '--run-to=frames', '--run-seconds=240', '--game-args=-noaudio -CD.'],
                                   dict(RA2_NATIVE_MSG_LOG='1'), 240)
        logs.append(folder)
        if '"GUI:DiplomacyMenu"' not in out:
            bad.append('the diplomacy screen (Tab) was not shown')
        if 'stopped: target reached' not in out:
            bad.append('the skirmish did not reach its %d frames (exit %d)' % (frames, code))
        bad += [l for l in out.splitlines() if BAD.search(l)][:10]
    if bad:
        print('%s: FAILED (logs in %s)' % (tag, ', '.join(logs)))
        for b in bad:
            print('  ' + b)
        return 1
    print('%s: passed' % tag)
    return 0


if __name__ == '__main__':
    sys.exit(main())
