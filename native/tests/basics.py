#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest native-basics (lane P2, 2026-11-16): the game's basic mechanics in one skirmish, in the
fast tier of every native gate. One launch, frames uncapped (RA2_NATIVE_UNCAPPED=1, game speed 0:
nothing here measures time), the human player's orders from RA2_NATIVE_ORDERS and the harness
build order (link/harness_orders.cpp, harness_build.cpp):

  damage     one of the player's Grizzlies fires on another (`shoot MTNK`, the order a force-fire
             click queues): the target's strength falls from 300 (native-tank-damage's check;
             the 2026-11-16 RadiusCount regression stopped every shell with a CellSpread);
  death      the Grizzly keeps firing until the target dies: the player has one Grizzly fewer
             and the target is gone from Technos (removed, not left at 0 strength);
  shroud     three GIs are sent 12 cells away (`scout`: each in another direction, the least mapped
             first, only to a cell in the GI's own movement zone: lane Y16 found the old flake was the
             random start position giving three unreachable destinations, which the path finder refuses): one has moved at least 6 cells and mapped at least 20 of the 7x7 cells around
             it that were not mapped when it was sent (CellClass::Unshroud; the RadiusCount
             regression also revealed the wrong cells, Sight_From);
  produce    the build order places a power plant, a refinery and a barracks; `train E1` (the
             barracks' cameo click) then gives the player more GIs;
  harvest    the refinery's free harvester delivers: the player's money rises while nothing is
             bought (the samples after the last purchase).

  basics.py --binary <ra2-native> --data-dir <installed RA2> --game-exe <game.exe> [--log <file>]

Each check prints its own `basics <check>: passed|FAILED (...)` line. Without the installed game
it exits 77 (skipped)."""
import argparse
import os
import re
import subprocess
import sys
import tempfile

import game_process
import wine as winehost

SKIP = 77
FRAMES = 6000
MONEY_FRAMES = list(range(3000, FRAMES + 1, 250))
TRAIN_FRAMES = list(range(2400, 4001, 200))
DAMAGE_FRAMES = list(range(100, 1101, 100))
ORDERS = ['20 health', '25 scout E1 12 0', '30 shoot MTNK'] + ['%d health' % f for f in DAMAGE_FRAMES] + ['1200 health', '1200 mapped',
          '2300 health'] + ['%d train E1' % f for f in TRAIN_FRAMES] + ['%d money' % f for f in MONEY_FRAMES] \
    + ['%d health' % (FRAMES - 10)]
HEALTH = re.compile(r'^harness-health h(\d+) (\S+) k\d+ s(-?\d+) @\S+ #\d+ &(\S+)$')
NOTE = re.compile(r'^harness-order (\d+) (\w+)')
SHOOT = re.compile(r'^harness-shoot #(\d+) #(\d+) &\S+ &(\S+)$', re.M)
MAPPED = re.compile(r'^harness-mapped s(\d+) moved (\d+) now (\d+)/(\d+) before (\d+)$')
MONEY = re.compile(r'^harness-money (-?\d+)$')
BUILD = re.compile(r'harness-build-\d+ \+\d+ ms\s+frame (\d+): (placed|produce) (\w+)')
BAD = re.compile(r'unrecovered|\btrap\b|signal \d+|SIG(SEGV|BUS|ILL|ABRT|FPE)|crash:', re.I)


def blocks(out):
    """[(frame, order word, [lines])]: each order's own lines come before its harness-order note."""
    found, pending = [], []
    for line in out.splitlines():
        m = NOTE.match(line)
        if m:
            found.append((int(m.group(1)), m.group(2), pending))
            pending = []
        elif line.startswith(('harness-health', 'harness-mapped', 'harness-money', 'harness-shoot', 'harness-scout')):
            pending.append(line)
    return found


def units(lines, house=None):
    """[(house, ID, strength, address)] of `health` lines (an object keeps its address; its
    Technos index moves as objects come and go)"""
    out = []
    for line in lines:
        m = HEALTH.match(line)
        if m:
            out.append((int(m.group(1)), m.group(2), int(m.group(3)), m.group(4)))
    return out


def checks(out):
    """{check: problem or None}"""
    by = {}
    for frame, word, lines in blocks(out):
        by.setdefault((frame, word), []).extend(lines)
    result = {}
    shot = SHOOT.search(out)
    start = units(by.get((20, 'health'), []))
    player = shot and next((h for h, i, s, n in start if n == shot.group(3)), None)
    # damage
    if not shot:
        result['damage'] = 'no shoot order (two Grizzlies of the player not found)'
    else:
        target = shot.group(3)
        before = next((s for h, i, s, n in start if n == target), None)
        seen = []
        for f in DAMAGE_FRAMES:
            now = [s for h, i, s, n in units(by.get((f, 'health'), [])) if n == target]
            seen.append((f, now[0] if now else None))
        hit = [(f, s) for f, s in seen if s is not None and 0 < s < 300]
        print('basics damage: Grizzly #%s fired on #%s: strength %s at frame 20; %s'
              % (shot.group(1), shot.group(2), before, ', '.join('%s@%d' % (s, f) for f, s in seen)))
        if before != 300:
            result['damage'] = 'the target started at %s, not 300' % before
        elif not hit:
            result['damage'] = 'the target never took damage'
        else:
            result['damage'] = None
    # death
    if shot and player is not None:
        def grizzlies(frame):
            return [n for h, i, s, n in units(by.get((frame, 'health'), [])) if h == player and i == 'MTNK']
        late = units(by.get((1200, 'health'), []))
        n0, n1 = len(grizzlies(20)), len(grizzlies(1200))
        zero = [x for x in late if x[2] <= 0]
        print('basics death: the player\'s Grizzlies %d at frame 20, %d at frame 1200' % (n0, n1))
        gone = all(n != shot.group(3) for h, i, s, n in late)
        result['death'] = None if n1 == n0 - 1 and gone and not zero else \
            'Grizzlies %d -> %d, target %s%s' % (n0, n1, 'gone' if gone else 'still listed',
                                                 ', units left at strength 0: %s' % zero if zero else '')
    else:
        result['death'] = 'no target'
    # shroud: a scout that moved at least 6 cells has mapped at least 20 of the 49 cells around it
    # that were not mapped when it was sent (a GI's sight does not reach every corner of 7x7)
    later = [m for m in (MAPPED.match(l) for l in by.get((1200, 'mapped'), [])) if m]
    good = [m for m in later if int(m.group(2)) >= 6 and int(m.group(3)) - int(m.group(5)) >= 20]
    print('basics shroud: %s' % ('; '.join('scout %s moved %s cells, %s/%s cells around it mapped (%s before)'
                                          % m.groups() for m in later) or 'no scouts'))
    result['shroud'] = None if good else 'no scout moved 6 cells and mapped 20 new cells around it (%d scouts)' % len(later)
    # produce
    def gis(frame):
        return len([1 for h, i, s, n in units(by.get((frame, 'health'), [])) if h == player and i == 'E1'])
    placed = [(int(f), t) for f, kind, t in BUILD.findall(out) if kind == 'placed']
    g0, g1 = gis(2300), gis(FRAMES - 10)
    print('basics produce: placed %s; the player\'s GIs %d at frame 2300, %d at frame %d' % (placed, g0, g1, FRAMES - 10))
    result['produce'] = None if any(t == 'GAPILE' for f, t in placed) and g1 > g0 else \
        'barracks %s, GIs %d -> %d' % ('placed' if any(t == 'GAPILE' for f, t in placed) else 'not placed', g0, g1)
    # harvest
    money = [(f, int(MONEY.match(l).group(1))) for (f, w), lines in sorted(by.items()) if w == 'money'
             for l in lines if MONEY.match(l)]
    rises = [(a, b) for a, b in zip(money, money[1:]) if b[1] > a[1]]
    print('basics harvest: money %s' % ', '.join('%d@%d' % (v, f) for f, v in money))
    result['harvest'] = None if rises else 'the money never rose (%d samples)' % len(money)
    return result


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--game-exe', default='')
    ap.add_argument('--log', default='')
    ap.add_argument('--wine-prefix', default='')
    ap.add_argument('--wine', default='')
    a = ap.parse_args()
    if not a.data_dir or not os.path.isdir(a.data_dir) or not a.game_exe or not os.path.isfile(a.game_exe):
        print('basics: no installed game given; skipped')
        return SKIP
    prefix = None
    if a.binary.lower().endswith('.exe'):
        # The Windows build runs under Wine, in a prefix of its own (tests/wine.py).
        if not a.wine_prefix:
            print('basics: skipped (a Windows build without a Wine prefix)')
            return SKIP
        wine = winehost.find_wine(a.wine)
        if not wine:
            print('basics: Windows build and no Wine (set RA2_NATIVE_WINE); skipped')
            return SKIP
        prefix = winehost.Prefix(wine, a.wine_prefix)
    with tempfile.TemporaryDirectory(prefix='ra2-basics-') as work:
        os.makedirs(os.path.join(work, 'home'))
        os.makedirs(os.path.join(work, 'write'))
        env = game_process.game_env()
        for k in ('RA2_NATIVE_ENHANCEMENTS', 'RA2_NATIVE_ORDERS', 'RA2_DATA_DIR', 'RA2_NATIVE_TRAP_CONTINUE'):
            env.pop(k, None)
        env.update({'HOME': os.path.join(work, 'home'), 'RA2_GAME_EXE': a.game_exe, 'RA2_NATIVE_LOG_STDERR': '1',
                    'RA2_NATIVE_IGNORE_FOCUS': '1', 'RA2_NATIVE_MOVIE_SKIP': '1', 'RA2_NATIVE_UNCAPPED': '1', 'RA2_NATIVE_SKIRMISH_SPEED': '0',
                    'RA2_NATIVE_BUILD_LIST': 'GAPOWR,GAREFN,GAPILE', 'RA2_NATIVE_ORDERS': '; '.join(ORDERS)})
        cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=frames', '--run-seconds=120',
               '--write-dir=' + os.path.join(work, 'write'), '--game-args=-noaudio -CD.', '--skirmish=#5',
               '--skirmish-frames=%d' % FRAMES, '--skirmish-order=build']
        try:
            for attempt in range(winehost.HOST_RETRIES + 1):
                run_env, run_cmd = env, cmd
                if prefix:
                    run_cmd, run_env = prefix.command(cmd), prefix.env(env)
                try:
                    p = game_process.run(run_cmd, env=run_env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                                         text=True, errors='replace', timeout=180 * (3 if prefix else 1))
                    out, code = p.stdout, p.returncode
                except subprocess.TimeoutExpired as e:
                    out, code = (e.output or ''), 'timeout'
                    if isinstance(out, bytes):
                        out = out.decode(errors='replace')
                why = winehost.host_failure(out, code == 'timeout', code if isinstance(code, int) else 0) if prefix else ''
                if not why or attempt == winehost.HOST_RETRIES:
                    break
                print('basics: %s; running again' % why)
        finally:
            if prefix:
                prefix.close()
    if a.log:
        with open(a.log, 'w') as f:
            f.write(out)
    bad = [l for l in out.splitlines() if BAD.search(l)]
    failed = []
    if code != 0 or bad:
        print('basics run: FAILED (exit %s, %d bad lines%s)' % (code, len(bad), (': ' + bad[0][:120]) if bad else ''))
        failed.append('run')
    for name, problem in checks(out).items():
        print('basics %s: %s' % (name, 'passed' if problem is None else 'FAILED (%s)' % problem))
        if problem is not None:
            failed.append(name)
    if failed:
        print('basics: FAILED (%s)' % ', '.join(failed))
        return 1
    print('basics: passed')
    return 0


if __name__ == '__main__':
    sys.exit(main())
