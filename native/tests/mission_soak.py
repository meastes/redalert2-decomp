#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""Mission soak (lane N25; ctests mission-soak-ra2-allied/-soviet, mission-soak-yr-allied/-soviet, label `soak`,
not in the default gate): every campaign mission of a side is played for several thousand game frames in the
native build, headless, with the harness's scripted orders, so its triggers, scripted teams, reinforcements,
timers, briefings and EVA events run as they would in play.

Mission n is reached as a player would reach it: Single Player, New Campaign, the side's emblem; missions 1 to
n-1 are won by RA2_NATIVE_FORCE_END (one entry every 200 loop frames, as tests/campaign_chain.py) and left by
their score screen's Continue. Mission n is then played (no forced end) until it has run --frames game frames,
or is won or lost (RA2_NATIVE_SOAK_END=<frames>@<n>: the run ends at milestone soak-end). The orders, relative to
each scenario's start (RA2_NATIVE_ORDERS "+<n>[/<p>] <order>"): the MCV deploys at frame 30, and from frame 1,200
every 1,200 frames the player's combat units hunt (the AI attack teams' order), so the enemy's defences, the
mission's reinforcements and its scripted teams meet the player's forces. The game runs at game speed 0, as fast as
the machine draws (RA2_NATIVE_SOAK_SPEED=0; a campaign normally forces speed 2, and the speed sets normalized
animations' rates, so runs are compared at one speed only) with a fixed seed (RA2_NATIVE_SOAK_SEED), and with --twice each mission is
played twice: the two runs' digests (every techno's type, house, place, strength and mission, the houses'
money and the scenario's random generator, every 250 frames) and trigger lines must be the same.

Recorded per mission (the table on stdout and <work>/soak.json): frames reached, how it ended (played, won,
lost), triggers seen to fire (a volatile trigger leaving the list; tripped-event changes; new ones), the mission
timer, houses defeated, faults (a trap, an unrecovered call, a signal, a crash, an assert or a "not written"
stand-in path), the first digest that differs between the two runs, and the run's wall time.

  mission_soak.py --game ra2|yr --side allied|soviet --binary <ra2-native|yr-native> --data-dir <game folder>
                  [--game-exe <game.exe>] --work <dir> [--frames 4000] [--missions 1-12] [--jobs 3] [--twice]

Fails on a fault, a mission that did not start, or (--twice) a desync. A mission lost early is recorded, not a
failure (scripted orders are not tactics). Skipped (77) without the installed game, and for a Windows build.
"""
import argparse
import concurrent.futures
import json
import os
import re
import shutil
import subprocess
import sys
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import game_process  # noqa: E402

SKIP = 77
BAD = re.compile(r'unrecovered|\btrap\b|signal \d+|SIG(SEGV|BUS|ILL|ABRT|FPE)|crash:|assert|not written', re.I)
GAMES = {
    # missions per side, the side's emblem and the menus' points, the settings file, the orders
    'ra2': dict(missions=12, single=(829, 302), new=(831, 305), emblem={'allied': (433, 179), 'soviet': (429, 448)},
                cont=(831, 640), settings='RA2.INI', orders='+30 deploy *; +1200/1200 hunt'),
    'yr': dict(missions=7, single=(720, 220), new=(720, 220), emblem={'allied': (315, 125), 'soviet': (315, 430)},
               cont=(720, 560), settings='RA2MD.INI', orders='+30 deploy AMCV; +30 deploy SMCV; +1200/1200 hunt'),
}
PREFIX = {'allied': 'ALL', 'soviet': 'SOV'}
SEED = 20261121


def script(game, side, n):
    g = GAMES[game]
    lines = ['s1+1500 click %d %d' % g['single'], 's2+1500 click %d %d' % g['new'],
             's3+%d click %d %d' % (2000 if game == 'ra2' else 2500, g['emblem'][side][0], g['emblem'][side][1])]
    for k in range(n - 1):
        lines.append('d%d+3000 click %d %d' % (4 + k, g['cont'][0], g['cont'][1]))
    return '\n'.join(lines) + '\n'


def settings(a):
    """The settings file in the write folder: RA2's the installation's own (its 1024x768 screen, which the menu
    points assume); Yuri's Revenge's the tests' usual one (no intro)."""
    if a.game == 'ra2':
        src = os.path.join(a.data_dir, 'RA2.INI')
        return open(src, errors='replace').read() if os.path.exists(src) else '[Video]\nScreenWidth=1024\nScreenHeight=768\n'
    return '[Intro]\nPlay=no\n'


def run_one(a, n, attempt):
    work = os.path.join(a.work, '%s-%s-%02d-%s' % (a.game, a.side, n, attempt))
    shutil.rmtree(work, ignore_errors=True)
    os.makedirs(os.path.join(work, 'w'))
    with open(os.path.join(work, 'w', GAMES[a.game]['settings']), 'w') as f:
        f.write(settings(a))
    path = os.path.join(work, 'soak.script')
    with open(path, 'w') as f:
        f.write(script(a.game, a.side, n))
    env = dict(os.environ, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_MSG_LOG='0',
               RA2_NATIVE_CRASH_TRACE='1', RA2_NATIVE_UNCAPPED='1', RA2_NATIVE_SOAK='250',
               RA2_NATIVE_SOAK_SEED=str(SEED), RA2_NATIVE_SOAK_SPEED='0',
               RA2_NATIVE_SOAK_END='%d@%d' % (a.frames, n), RA2_NATIVE_ORDERS=a.orders or GAMES[a.game]['orders'])
    if a.game == 'ra2':
        env['RA2_GAME_EXE'] = a.game_exe
    for k in ('RA2_NATIVE_TRAP_CONTINUE', 'RA2_NATIVE_FORCE_END', 'RA2_NATIVE_SOAK_DUMP'):
        env.pop(k, None)
    if n > 1:
        env['RA2_NATIVE_FORCE_END'] = ','.join(str(100 + 200 * k) for k in range(n - 1))
    seconds = 120 + 15 * n + a.frames // 40
    cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=soak-end', '--run-seconds=%d' % seconds,
           '--write-dir=' + os.path.join(work, 'w'), '--input-script=' + path, '--game-args=-noaudio -CD.']
    start = time.time()
    with open(os.path.join(work, 'run.log'), 'w') as log:
        try:
            code = game_process.run(cmd, env=env, stdout=log, stderr=subprocess.STDOUT, timeout=seconds + 60).returncode
        except subprocess.TimeoutExpired:
            code = -1
    wall = time.time() - start
    text = open(os.path.join(work, 'run.log'), errors='replace').read()
    return parse(a, n, text, code, wall, work)


def parse(a, n, text, code, wall, work):
    lines = text.splitlines()
    starts = [i for i, l in enumerate(lines) if l.startswith('soak-mission ')]
    r = dict(mission=n, scenario=None, code=code, wall=round(wall, 1), end=None, frames=0, gone=0, new=0, tripped=0,
             timer=[], defeated=[], faults=[], digests=[], triggers=[], log=os.path.join(work, 'run.log'))
    r['faults'] = sorted(set(l.strip() for l in lines if BAD.search(l) and not l.startswith('input-script')))[:8]
    if len(starts) < n:
        r['end'] = 'mission %d did not start (%d scenarios started)' % (n, len(starts))
        return r
    body = lines[starts[n - 1]:]
    r['scenario'] = body[0].split()[1]
    for l in body:
        m = re.match(r'soak-trigger (\d+) (gone|new|tripped) (\S+)(.*)$', l)
        if m:
            r[m.group(2)] += 1
            r['triggers'].append(l)
            r['frames'] = max(r['frames'], int(m.group(1)))
            continue
        m = re.match(r'soak-digest (\d+) ', l)
        if m:
            r['digests'].append(l)
            r['frames'] = max(r['frames'], int(m.group(1)))
            continue
        m = re.match(r'soak-timer (\d+) (\S+) (-?\d+)', l)
        if m:
            r['timer'].append('%s at %s (%s left)' % (m.group(2), m.group(1), m.group(3)))
            continue
        m = re.match(r'soak-house (\d+) \d+:(\S+) defeated', l)
        if m:
            r['defeated'].append('%s@%s' % (m.group(2), m.group(1)))
            continue
        m = re.match(r'soak-end \S+ (\S+) at frame (\d+)', l)
        if m:
            r['end'] = m.group(1)
            r['frames'] = max(r['frames'], int(m.group(2)))
    if r['end'] is None:
        stop = [l for l in lines if l.startswith('run-to: target')]
        r['end'] = 'stopped: ' + (stop[-1].split('stopped: ', 1)[-1] if stop else 'no summary (exit %s)' % code)
    return r


def compare(x, y):
    """The first digest or trigger line where two runs of a mission part (None: the same)."""
    for kind in ('digests', 'triggers'):
        for i, (p, q) in enumerate(zip(x[kind], y[kind])):
            if p != q:
                return '%s: %s | %s' % (kind, p, q)
        if len(x[kind]) != len(y[kind]) and x['end'] == y['end'] == 'played':
            return '%s: %d lines against %d' % (kind, len(x[kind]), len(y[kind]))
    return None


def missions(spec, count):
    if not spec:
        return list(range(1, count + 1))
    out = []
    for part in spec.split(','):
        lo, _, hi = part.partition('-')
        out.extend(range(int(lo), int(hi or lo) + 1))
    return [m for m in out if 1 <= m <= count]


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--game', required=True, choices=sorted(GAMES))
    ap.add_argument('--side', required=True, choices=sorted(PREFIX))
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--game-exe', default='')
    ap.add_argument('--work', required=True)
    ap.add_argument('--frames', type=int, default=4000)
    ap.add_argument('--missions', default='')
    ap.add_argument('--jobs', type=int, default=3)
    ap.add_argument('--twice', action='store_true', help='play each mission twice and compare (desync)')
    ap.add_argument('--orders', default='', help='RA2_NATIVE_ORDERS instead of the default soak orders')
    a = ap.parse_args()
    name = 'mission-soak-%s-%s' % (a.game, a.side)
    need = 'game.exe' if a.game == 'ra2' else 'gamemd.exe'
    if not a.data_dir or not os.path.exists(os.path.join(a.data_dir, need)):
        print('%s: skipped (no %s in --data-dir)' % (name, need))
        return SKIP
    if a.game == 'ra2' and (not a.game_exe or not os.path.isfile(a.game_exe)):
        print('%s: skipped (no --game-exe)' % name)
        return SKIP
    if a.binary.lower().endswith('.exe'):
        print('%s: skipped (run on the Mac build; the Windows build has the same game code)' % name)
        return SKIP
    shutil.rmtree(a.work, ignore_errors=True)
    os.makedirs(a.work)
    wanted = missions(a.missions, GAMES[a.game]['missions'])
    jobs = [(n, t) for n in wanted for t in (('a', 'b') if a.twice else ('a',))]
    results = {}
    with concurrent.futures.ThreadPoolExecutor(max_workers=max(1, a.jobs)) as pool:
        futures = {pool.submit(run_one, a, n, t): (n, t) for n, t in jobs}
        for f in concurrent.futures.as_completed(futures):
            results[futures[f]] = f.result()
    failures = []
    rows = []
    print('%s: %d frames a mission, seed %d%s' % (name, a.frames, SEED, ', each twice' if a.twice else ''))
    print('%-3s %-14s %-8s %6s %5s %5s %5s  %-28s %s' % ('#', 'scenario', 'end', 'frames', 'gone', 'trip', 'new',
                                                       'defeated', 'timer / faults / desync'))
    for n in wanted:
        r = results[(n, 'a')]
        notes = []
        if r['timer']:
            notes.append('timer ' + ', '.join(r['timer'][:3]))
        expect = '%s%02d' % (PREFIX[a.side], n)
        if not r['scenario'] or not r['scenario'].upper().startswith(expect):
            failures.append('mission %d: %s (scenario %s, expected %s*)' % (n, r['end'], r['scenario'], expect))
        if r['end'] not in ('played', 'won', 'lost'):
            failures.append('mission %d (%s): %s' % (n, r['scenario'], r['end']))
        if r['faults']:
            failures.append('mission %d (%s): faults: %s' % (n, r['scenario'], '; '.join(r['faults'][:3])))
            notes.append('FAULT ' + r['faults'][0][:80])
        if a.twice:
            other = results[(n, 'b')]
            if other['faults']:
                failures.append('mission %d (%s), second run: faults: %s' % (n, r['scenario'], '; '.join(other['faults'][:3])))
            why = compare(r, other)
            r['desync'] = why
            if why:
                failures.append('mission %d (%s): the two runs differ: %s' % (n, r['scenario'], why))
                notes.append('DESYNC ' + why[:90])
            else:
                notes.append('same twice')
        print('%-3d %-14s %-8s %6d %5d %5d %5d  %-28s %s' % (n, r['scenario'] or '-', (r['end'] or '?')[:8], r['frames'],
                                                           r['gone'], r['tripped'], r['new'],
                                                           ','.join(r['defeated'])[:28], '; '.join(notes)))
        rows.append(r)
    with open(os.path.join(a.work, 'soak.json'), 'w') as f:
        json.dump([dict((k, v) for k, v in r.items() if k not in ('digests', 'triggers')) for r in rows], f, indent=1)
    for f in failures:
        print('FAIL: ' + f)
    print('%s: %s (%d missions; %s)' % (name, 'FAIL' if failures else 'OK', len(rows), a.work))
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
