#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest yr-native-lan-pair (lane YW2): two Yuri's Revenge games on this machine play a Network (LAN)
game against each other over UDP, through the real menus with scripted input (as the RA2 game's
native-lan-pair, native/tests/lan_pair.py).

  Alpha: main menu -> Network -> New (the host screen); types a chat line; Start Game once Bravo
         has accepted.
  Bravo: main menu -> Network -> the Games list's second row (Alpha's game) -> Join (the guest
         screen); types a chat line; Accept.
  Both then play: the game's frames run in step until Alpha's frame count (--frames) is reached and
  Alpha quits; Bravo, told the host left, is closed.

Each instance has its own write folder with its player name in RA2MD.INI ([MultiPlayer] Handle) and
the pair its own UDP port (RA2_NATIVE_IPX_PORT). Checks: no link-closure trap or fault in either log;
the host seated Bravo (Bravo's guest screen shown; Alpha's join answer); each saw the other's chat
line (RA2_NATIVE_LIST_LOG: the chat list's "[Name] text" line); both reached the game; the per-frame
game CRCs both logged (RA2_NATIVE_CRC_LOG: "crc <frame> <value>") agree on every frame both logged,
and there are at least --frames / 2 of them.

  yr_lan_pair.py --binary <yr-native> --data-dir <game folder> --work <dir> [--frames 3000]
                 [--binary-b <second build> [--wine-prefix-b <dir> [--wine <wine>]]]

--binary-b runs Bravo from another build (the Windows build under Wine: Mac to Windows over UDP on
one machine; its Wine prefix is deleted afterwards). Skipped (77) without the installed game.
"""
import argparse
import os
import random
import re
import shutil
import subprocess
import sys
import time

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', 'tests'))
import game_process  # noqa: E402  (native/tests: the game's process group, lane C19)
import wine as winehost  # noqa: E402

BAD = re.compile(r'unrecovered|\btrap\b|signal \d+|SIG(SEGV|BUS|ILL|ABRT|FPE)|crash:', re.I)
CRC = re.compile(r'^crc (\d+) ([0-9a-f]{8})$', re.M)
# 800x600 (RA2_NATIVE_MSG_LOG): the main menu's Network (644,283 156x42); the lobby's New (644,199),
# Join (644,241) and Games list (322,73 301x295; row 0 is "Lobby", row 1 the first game); the setup
# screens' chat edit (NewEdit list box 1344 at 11,540 608x19), the host's Start Game and the
# guest's Accept (both 644,241). "s<k>": the k-th dialog shown (host.cpp).
NETWORK, NEW, JOIN, GAME_ROW, CHAT, START = '720 304', '720 220', '720 262', '400 100', '315 549', '722 262'


def typed(anchor, start, word):
    return ['%s+%d key %s' % (anchor, start + 150 * k, c) for k, c in enumerate(word)] + \
           ['%s+%d key Return' % (anchor, start + 150 * len(word) + 300)]


def host_script(frames):
    return (['s1+1500 click ' + NETWORK, 's2+1500 click ' + NEW, 's3+26000 click ' + CHAT] + typed('s3', 26500, 'alphachat')
            + ['s3+40000 click ' + START, '%d quit' % frames])


def guest_script():
    return (['s1+1500 click ' + NETWORK, 's2+12000 move ' + GAME_ROW, 's2+12500 click ' + GAME_ROW, 's2+14000 click ' + JOIN,
             's3+8000 click ' + CHAT] + typed('s3', 8500, 'bravochat') + ['s3+20000 click ' + START])


def handle(name):
    """RA2MD.INI's [MultiPlayer] Handle: the name's UTF-16 codes in hex, each followed by a comma"""
    return ''.join('%x,' % ord(c) for c in name)


def frames_logged(path):
    try:
        with open(path, errors='replace') as f:
            found = CRC.findall(f.read())
    except OSError:
        return 0
    return int(found[-1][0]) if found else 0


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--binary-b', default='')
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--work', required=True)
    ap.add_argument('--frames', type=int, default=3000, help='Alpha\'s scripted frames before it quits')
    ap.add_argument('--seconds', type=int, default=900, help='the run\'s time limit')
    ap.add_argument('--wine-prefix-b', default='')
    ap.add_argument('--wine', default='')
    a = ap.parse_args()
    if not a.data_dir or not os.path.exists(os.path.join(a.data_dir, 'gamemd.exe')):
        print('skipped: no Yuri\'s Revenge install (gamemd.exe) in --data-dir')
        return 77
    prefix = None
    if a.wine_prefix_b and not (a.binary_b and os.path.isfile(a.binary_b)):
        print('skipped: no Windows build given (--binary-b)')
        return 77
    if a.wine_prefix_b:
        wine = winehost.find_wine(a.wine)
        if not wine:
            print('skipped: no Wine for the Windows build')
            return 77
        prefix = winehost.Prefix(wine, a.wine_prefix_b)
    work = os.path.abspath(a.work)
    shutil.rmtree(work, ignore_errors=True)
    port = str(random.SystemRandom().randrange(27000, 60000, 8))
    runs = []
    try:
        for name, script, binary in (('Alpha', host_script(a.frames), a.binary), ('Bravo', guest_script(), a.binary_b or a.binary)):
            folder = os.path.join(work, name)
            os.makedirs(os.path.join(folder, 'w'))
            with open(os.path.join(folder, 'w', 'RA2MD.INI'), 'w') as f:
                f.write('[MultiPlayer]\nHandle=%s\n' % handle(name))
            with open(os.path.join(folder, 'input.script'), 'w') as f:
                f.write('\n'.join(script) + '\n')
            env = dict(os.environ, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_DIALOG_LOG='1',
                       RA2_NATIVE_MSG_LOG='1', RA2_NATIVE_LIST_LOG='1', RA2_NATIVE_CRC_LOG='1', RA2_NATIVE_IPX_PORT=port,
                       RA2_NATIVE_SCRIPT_AS_USER='1')
            for k in ('RA2_NATIVE_TRAP_CONTINUE', 'RA2_NATIVE_FORCE_END'):
                env.pop(k, None)
            cmd = [binary, '--data-dir', a.data_dir, '--run-to=never', '--run-seconds=%d' % a.seconds,
                   '--write-dir=' + os.path.join(folder, 'w'), '--input-script=' + os.path.join(folder, 'input.script'),
                   '--game-args=-noaudio -CD.']
            if prefix and name == 'Bravo':
                cmd, env = prefix.command(cmd), prefix.env(env)
            log = open(os.path.join(folder, 'run.log'), 'w')
            runs.append((name, game_process.Popen(cmd, env=env, stdout=log, stderr=subprocess.STDOUT), log, folder))
            time.sleep(2)
        alpha, bravo = runs
        # Alpha quits itself at its frame count; then Bravo is closed (20 s on). A run whose frames
        # stop advancing for 180 s (after its first frame) is ended.
        start, last, last_at = time.time(), -1, time.time()
        while alpha[1].poll() is None and time.time() - start < a.seconds + 60:
            time.sleep(5)
            f = frames_logged(os.path.join(alpha[3], 'run.log'))
            if f != last:
                last, last_at = f, time.time()
            elif last > 0 and time.time() - last_at > 180:
                print('yr-lan-pair: Alpha: no new frame for 180 s (at frame %d)' % last)
                break
        if alpha[1].poll() is None:
            alpha[1].end()
        time.sleep(20)
        for name, p, log, folder in runs:
            if p.poll() is None:
                p.end()
            log.close()
    finally:
        for name, p, log, folder in runs:
            if p.poll() is None:
                p.end()
        if prefix:
            prefix.close()
            shutil.rmtree(prefix.folder, ignore_errors=True)   # Wine prefixes are large
    failures = []
    text = {}
    for name, p, log, folder in runs:
        text[name] = open(os.path.join(folder, 'run.log'), errors='replace').read()
        failures += ['%s: %s' % (name, l) for l in text[name].splitlines() if BAD.search(l) and not l.startswith('net:')][:5]
    for name, other in (('Alpha', 'Bravo'), ('Bravo', 'Alpha')):
        if not re.search(r'^list text id 1343: \[%s\] %schat' % (other, other.lower()), text[name], re.M | re.I):
            failures.append('%s did not show %s\'s chat line' % (name, other))
        if 'Starting scnenario' not in text[name] and 'progress: frame' not in text[name] and not CRC.search(text[name]):
            failures.append('%s never reached the game' % name)
    crcs = [dict((int(f), v) for f, v in CRC.findall(text[n])) for n in ('Alpha', 'Bravo')]
    both = sorted(set(crcs[0]) & set(crcs[1]))
    differ = [f for f in both if crcs[0][f] != crcs[1][f]]
    print('yr-lan-pair: %d frames with both CRCs logged (Alpha %d, Bravo %d), %d differ%s' % (
        len(both), len(crcs[0]), len(crcs[1]), len(differ), (' (first at frame %d)' % differ[0]) if differ else ''))
    if differ:
        failures.append('the games\' CRCs differ from frame %d' % differ[0])
    if len(both) < a.frames // 2:
        failures.append('only %d frames with both CRCs (at least %d wanted)' % (len(both), a.frames // 2))
    for f in failures:
        print('FAIL: ' + f)
    print('%s: yr-native LAN pair (%s)' % ('FAIL' if failures else 'OK', work))
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
