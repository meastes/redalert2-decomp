#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest yr-native-lan-lobby (lane YW): Yuri's Revenge's Network (LAN) screens with two games on this
machine over UDP, through the real menus with scripted input. FIRST STAGE: the route stops at the
join request (the host's answer, the guest's seat, the game and its sync are not reached yet:
native/yr/README.md "LAN").

  Alpha: main menu -> Network (the lobby) -> New (the host screen, "GUI:HostScreen").
  Bravo: main menu -> Network -> the Games list's second row (Alpha's game) -> Join.

Each instance has its own write folder with its player name in RA2MD.INI ([MultiPlayer] Handle) and
the pair its own UDP port (RA2_NATIVE_IPX_PORT, native/port/src/wsock32.cpp). Checks: neither log has
a link-closure trap or a fault and both reach the time limit; Alpha showed the host screen; Bravo
showed the lobby with the host's game listed (a second row in the Games list), selected it and
asked to join (the
Join button's command 0x6CD reached the lobby).

  yr_lan_lobby.py --binary <yr-native> --data-dir <game folder> --work <dir>
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
# 800x600: the main menu's Network (644,283 156x42), the lobby's New (644,199) and Join (644,241), the
# Games list (322,73 301x295; row 0 is "Lobby", row 1 the first game). Bravo waits for the host's
# game to be listed (Alpha forms it about 3 s after its lobby shows).
ALPHA = ['s1+1500 click 720 304', 's2+1500 click 720 220']
BRAVO = ['s1+1500 click 720 304', 's2+12000 move 400 100', 's2+12500 click 400 100', 's2+14000 click 720 262']


def handle(name):
    """RA2MD.INI's [MultiPlayer] Handle: the name's UTF-16 codes in hex, each followed by a comma"""
    return ''.join('%x,' % ord(c) for c in name)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--binary-b', default='')
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--work', required=True)
    ap.add_argument('--seconds', type=int, default=50)
    ap.add_argument('--wine-prefix-b', default='')
    ap.add_argument('--wine', default='')
    a = ap.parse_args()
    if not a.data_dir or not os.path.exists(os.path.join(a.data_dir, 'gamemd.exe')):
        print('skipped: no Yuri\'s Revenge install (gamemd.exe) in --data-dir')
        return 77
    prefix = None
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
        for name, script, binary in (('Alpha', ALPHA, a.binary), ('Bravo', BRAVO, a.binary_b or a.binary)):
            folder = os.path.join(work, name)
            os.makedirs(os.path.join(folder, 'w'))
            with open(os.path.join(folder, 'w', 'RA2MD.INI'), 'w') as f:
                f.write('[MultiPlayer]\nHandle=%s\n' % handle(name))
            with open(os.path.join(folder, 'input.script'), 'w') as f:
                f.write('\n'.join(script) + '\n')
            env = dict(os.environ, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_DIALOG_LOG='1',
                       RA2_NATIVE_MSG_LOG='1', RA2_NATIVE_IPX_PORT=port, RA2_NATIVE_SCRIPT_AS_USER='1')
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
        for name, p, log, folder in runs:
            try:
                p.wait(timeout=a.seconds + (240 if prefix else 90))
            except subprocess.TimeoutExpired:
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
        stop = [l for l in text[name].splitlines() if 'stopped:' in l]
        if not stop or 'time limit' not in stop[-1]:
            failures.append('%s stopped early: %s' % (name, stop[-1] if stop else 'no stop line'))
        failures += ['%s: %s' % (name, l) for l in text[name].splitlines() if BAD.search(l)][:5]
    if '"GUI:HostScreen"' not in text.get('Alpha', ''):
        failures.append('Alpha did not show the host screen')
    bravo = text.get('Bravo', '')
    if '"GUI:Lobby"' not in bravo:
        failures.append('Bravo did not show the lobby')
    # The Games list's rows (Net2DisplayGameList: LB_RESETCONTENT, then "Lobby" and each game by
    # WM_USER + 205): after the last refill there must be a second row, the host's game.
    lines = bravo.splitlines()
    resets = [i for i, l in enumerate(lines) if 'ListBox id 1353 tmpl 0: 0x184 ' in l]
    rows = sum(1 for l in lines[resets[-1]:] if 'ListBox id 1353 tmpl 0: 0x4cd ' in l) if resets else 0
    if rows < 2:
        failures.append('Bravo\'s Games list did not list the host\'s game (%d rows)' % rows)
    if not re.search(r'ListBox id 1353 tmpl 0: 0x186 1 0', bravo):
        failures.append('Bravo did not select the host\'s game (Games list row 1)')
    if not re.search(r': 0x111 6cd ', bravo):
        failures.append('Bravo\'s Join did not reach the lobby')
    for f in failures:
        print('FAIL: ' + f)
    print('%s: yr-native LAN lobby (%s)' % ('FAIL' if failures else 'OK', work))
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
