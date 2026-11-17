#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest native-tank-damage (lane C22, the user's report of 2026-11-16: in a LAN game tanks never
damaged each other while GIs did): a skirmish where one of the player's Grizzlies (105mm, warhead
AP, CellSpread 0.3) fires on another (RA2_NATIVE_ORDERS `shoot MTNK`, the order a force-fire
click queues) and the target's strength must drop.

The cause was the native view of the RadiusCount table 0x007A5908: the rename of 0d65d9378
(MapClass_RadiusCount) slipped past native_units.py's whole-table rule, so Explosion_Damage's ring
loop (MapClass_RadiusCount[rings] cells) read 0 for every warhead with a CellSpread, and a shell
landing on its target found nothing to damage; warheads without a spread (the GIs' SA) take the
centre cell through RadiusCount[0] and still hit.

  tank_damage.py --binary <ra2-native> --data-dir <installed RA2> --game-exe <game.exe> [--log <file>]

Passes when the target's strength (`health` lines, harness_orders.cpp) is the full 300 before the
order and below it 170 frames after. Without the installed game it exits 77 (skipped)."""
import argparse
import os
import re
import subprocess
import sys
import tempfile

import game_process

SKIP = 77
HEALTH = re.compile(r'^harness-health h(\d+) (\S+) k\d+ s(-?\d+) @\S+ #\d+ &(\S+)$', re.M)
SHOOT = re.compile(r'^harness-shoot #(\d+) #(\d+) &\S+ &(\S+)$', re.M)
BAD = re.compile(r'unrecovered|\btrap\b|signal \d+|SIG(SEGV|BUS|ILL|ABRT|FPE)|crash:', re.I)


def strengths(out, frame_marks):
    """{frame: {index: strength}} from the `health` lines, each block after its order's note"""
    blocks = {}
    for frame in frame_marks:
        m = re.search(r'^harness-order %d health: units \d+$' % frame, out, re.M)
        if not m:
            continue
        # the order's lines come before its harness-order note: take the block just before it
        start = out.rfind('harness-order', 0, m.start())
        chunk = out[start if start >= 0 else 0:m.start()]
        blocks[frame] = {i: int(s) for _, _, s, i in HEALTH.findall(chunk)}   # by the object's address
    return blocks


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--game-exe', default='')
    ap.add_argument('--log', default='')
    a = ap.parse_args()
    if not a.data_dir or not os.path.isdir(a.data_dir) or not a.game_exe or not os.path.isfile(a.game_exe):
        print('tank-damage: no installed game given; skipped')
        return SKIP
    with tempfile.TemporaryDirectory(prefix='ra2-tankdmg-') as work:
        os.makedirs(os.path.join(work, 'home'))
        os.makedirs(os.path.join(work, 'write'))
        env = game_process.game_env()
        for k in ('RA2_NATIVE_ENHANCEMENTS', 'RA2_NATIVE_ORDERS', 'RA2_DATA_DIR'):
            env.pop(k, None)
        env.update({'HOME': os.path.join(work, 'home'), 'RA2_GAME_EXE': a.game_exe, 'RA2_NATIVE_LOG_STDERR': '1',
                    'RA2_NATIVE_IGNORE_FOCUS': '1', 'RA2_NATIVE_MOVIE_SKIP': '1', 'RA2_NATIVE_ORDERS': '20 health; 30 shoot MTNK; 200 health'})
        cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=frames', '--run-seconds=240',
               '--write-dir=' + os.path.join(work, 'write'), '--game-args=-noaudio -CD.', '--skirmish=#5',
               '--skirmish-frames=220']
        try:
            p = game_process.run(cmd, env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True,
                                 errors='replace', timeout=300)
            out, code = p.stdout, p.returncode
        except subprocess.TimeoutExpired as e:
            out, code = (e.output or ''), 'timeout'
            if isinstance(out, bytes):
                out = out.decode(errors='replace')
    if a.log:
        with open(a.log, 'w') as f:
            f.write(out)
    problems = []
    bad = [l for l in out.splitlines() if BAD.search(l)]
    if code != 0 or bad:
        problems.append('exit %s, %d bad lines%s' % (code, len(bad), (': ' + bad[0][:120]) if bad else ''))
    shot = SHOOT.search(out)
    blocks = strengths(out, (20, 200))
    if not shot:
        problems.append('no shoot order (two Grizzlies of the player not found)')
    elif 20 not in blocks or 200 not in blocks:
        problems.append('health lines missing (frames %s)' % sorted(blocks))
    else:
        # the target by its address (Technos indexes move as objects come and go)
        target = shot.group(3)
        before, after = blocks[20].get(target), blocks[200].get(target)
        print('tank-damage: Grizzly #%s fired on #%s (%s): strength %s at frame 20, %s at frame 200'
              % (shot.group(1), shot.group(2), target, before, after))
        if before != 300:
            problems.append('the target started at %s, not 300' % before)
        elif after is not None and after >= before:
            problems.append('the target took no damage (%s -> %s)' % (before, after))
    if problems:
        print('tank-damage: FAILED (%s)' % '; '.join(problems))
        return 1
    print('tank-damage: passed')
    return 0


if __name__ == '__main__':
    sys.exit(main())
