#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest native-mod-rules (lane C16, mod support level 1): a tiny mod made at test time changes a
vehicle's cost, and with no mod the game is unchanged.

  1. No mod (None): a skirmish's first frames; the harness writes the game's RULES.INI as the game
     reads it (RA2_NATIVE_HARNESS_DUMP_RULES, link/harness_mod.cpp) and logs the Grizzly's (MTNK)
     and the Rhino's (HTNK) cost as loaded (RA2_NATIVE_HARNESS_COST).
  2. The mod: <work>/Mods/CostTest/rules.ini, that RULES.INI with the Grizzly's Cost set to 1234
     (lower-case file name: the mod's names match in any case).
  3. --mod=<that folder>: the Grizzly costs 1234, the Rhino what it cost without the mod.

  mod_rules.py --binary <ra2-native> --data-dir <installed RA2> --game-exe <game.exe> --work <dir>
               [--wine-prefix <dir> [--wine <wine>]]

Without the data folder or game.exe it exits 77 (skipped), as native-census."""
import argparse
import os
import re
import shutil
import subprocess
import sys

import game_process
import wine as winehost

SKIP = 77
BAD = re.compile(r'unrecovered|trap|signal \d+|SIG(SEGV|BUS|ILL|ABRT|FPE)|crash:|abort', re.I)
COST = re.compile(r'^harness-cost (\w+) (-?\d+)$', re.M)
NEW_COST = 1234


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default=os.environ.get('RA2_NATIVE_CENSUS_DATA_DIR', ''))
    ap.add_argument('--game-exe', default=os.environ.get('RA2_NATIVE_CENSUS_GAME_EXE', ''))
    ap.add_argument('--work', required=True)
    ap.add_argument('--wine-prefix', default='')
    ap.add_argument('--wine', default='')
    a = ap.parse_args()
    name = 'native-mod-rules'
    if not a.data_dir or not os.path.isdir(a.data_dir) or not a.game_exe or not os.path.isfile(a.game_exe):
        print('%s: skipped (set RA2_NATIVE_CENSUS_DATA_DIR and RA2_NATIVE_CENSUS_GAME_EXE)' % name)
        return SKIP
    prefix = None
    if a.binary.lower().endswith('.exe'):
        if not a.wine_prefix:
            print('%s: skipped (a Windows build without a Wine prefix)' % name)
            return SKIP
        wine = winehost.find_wine(a.wine)
        if not wine:
            print('%s: Windows build and no Wine (set RA2_NATIVE_WINE); skipped' % name)
            return SKIP
        prefix = winehost.Prefix(wine, a.wine_prefix)
    work = os.path.abspath(a.work)
    shutil.rmtree(work, ignore_errors=True)
    os.makedirs(work)
    rules = os.path.join(work, 'rules-dump.ini')
    mod = os.path.join(work, 'Mods', 'CostTest')

    def run(tag, extra_env, extra_args):
        env = dict(os.environ, RA2_GAME_EXE=a.game_exe, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_CRASH_TRACE='1',
                   RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_HARNESS_COST='MTNK,HTNK')
        for k in ('RA2_NATIVE_TRAP_CONTINUE', 'RA2_NATIVE_MOD'):
            env.pop(k, None)
        env.update(extra_env)
        write_dir = os.path.join(work, 'write-' + tag)
        os.makedirs(write_dir, exist_ok=True)
        cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=frames', '--run-seconds=240', '--write-dir=' + write_dir,
               '--game-args=-noaudio -CD.', '--skirmish=#5', '--skirmish-frames=5'] + extra_args
        for attempt in range(winehost.HOST_RETRIES + 1):
            run_env, run_cmd = env, cmd
            if prefix:
                run_cmd, run_env = prefix.command(cmd), prefix.env(env)
            p = game_process.run(run_cmd, env=run_env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True,
                                 errors='replace')
            why = winehost.host_failure(p.stdout) if prefix else ''
            if not why or attempt == winehost.HOST_RETRIES:
                break
            print('%s: %s; running again' % (name, why))
        with open(os.path.join(work, tag + '.log'), 'w') as f:
            f.write(p.stdout)
        bad = [l for l in p.stdout.splitlines() if BAD.search(l)]
        costs = {m.group(1).upper(): int(m.group(2)) for m in COST.finditer(p.stdout)}
        return p, bad, costs

    problems = []
    try:
        p, bad, plain = run('none', {'RA2_NATIVE_HARNESS_DUMP_RULES': rules}, [])
        print('no mod: %s' % plain)
        if p.returncode != 0 or bad:
            problems.append('no mod: exit %d, %d bad lines' % (p.returncode, len(bad)))
        text = open(rules, 'rb').read().decode('latin-1') if os.path.isfile(rules) else ''
        m = re.search(r'(?im)^\[MTNK\][^\[]*?^Cost=[ \t]*(\d+)', text)
        if not m or 'MTNK' not in plain or 'HTNK' not in plain:
            problems.append('no RULES.INI dump or no costs from the game without a mod')
        else:
            if int(m.group(1)) != plain['MTNK']:
                problems.append('the dumped rules say MTNK costs %s, the game %d' % (m.group(1), plain['MTNK']))
            os.makedirs(mod)
            with open(os.path.join(mod, 'rules.ini'), 'wb') as f:
                f.write((text[:m.start(1)] + str(NEW_COST) + text[m.end(1):]).encode('latin-1'))
            p, bad, modded = run('mod', {}, ['--mod=' + mod])
            print('mod: %s' % modded)
            if p.returncode != 0 or bad:
                problems.append('mod: exit %d, %d bad lines' % (p.returncode, len(bad)))
            if 'mod: ' + mod not in p.stdout:
                problems.append('the game did not take the mod')
            if modded.get('MTNK') != NEW_COST:
                problems.append('with the mod MTNK costs %s, not %d' % (modded.get('MTNK'), NEW_COST))
            if modded.get('HTNK') != plain['HTNK']:
                problems.append('with the mod HTNK costs %s, not %d as without' % (modded.get('HTNK'), plain['HTNK']))
    finally:
        if prefix:
            prefix.close()
    if problems:
        print('%s: FAILED (%s)' % (name, '; '.join(problems)))
        return 1
    print('%s: passed' % name)
    return 0


if __name__ == '__main__':
    sys.exit(main())
