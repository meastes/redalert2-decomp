#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest native-lan-lobby (lane C20; the user's LAN reports of 2026-11-16): the LAN lobby's chat
and game list, with one side playing the iOS app's network, and the main menu's Internet button.

Two games on this machine (tests/lan_pair.py's Instance: own settings folders, own IPX port
range), Alpha hosting and Bravo joining, run twice: first Bravo, then Alpha, with
RA2_NATIVE_TEST_NO_BROADCAST=1 (every broadcast sendto fails with EHOSTUNREACH, as the iOS app's
do without Apple's multicast entitlement; port/src/wsock32.cpp). In each run:
  * Alpha clicks the main menu's Internet button first (it must be disabled: drawn so and a click
    does nothing; user 2026-11-16), then Network;
  * in the lobby each types a chat line in the chat edit and presses Return (the user's "chat
    sends nothing": NewEdit 0x005F22A0's Return in a multi-line edit, notification 0x501);
  * Alpha clicks New (hosts), Bravo picks Alpha's game in the Games list and clicks Join (the
    user's "iOS sees the players but no hosted game", in both directions).
Passes when, in both runs: Alpha's Internet button is listed disabled (WS_DISABLED in its
RA2_NATIVE_MSG_LOG control line) and no WM_COMMAND from it (0x684) reached the menu; each side's
lobby shows the other's chat line (the "list text" lines of RA2_NATIVE_LIST_LOG); Bravo reaches
the join screen (its third dialog); no fault. Skipped (77) without the installed game.

  lan_lobby.py --binary <ra2-native> --data-dir <installed RA2> --game-exe <game.exe> --work <dir>
"""
import argparse
import os
import re
import sys

import lan_pair

SKIP = 77
BAD = lan_pair.BAD

# The shell at 1024x768 (RA2_NATIVE_MSG_LOG): main menu Internet (756,325 156x42), Network
# (756,367); the lobby's chat edit (NewEdit list box 1344 at 208,563 441x19), New (756,283),
# Join (756,325), the Games list's second row (the first is "Lobby").
INTERNET, NETWORK, CHAT, NEW, JOIN, GAME_ROW = '834 346', '834 388', '420 572', '834 304', '834 346', '539 238'


def typed(anchor, start, word):
    return ['%s+%d key %s' % (anchor, start + 150 * k, c) for k, c in enumerate(word)] + \
           ['%s+%d key Return' % (anchor, start + 150 * len(word) + 300)]


def host_script():
    return (['s1+1500 click ' + INTERNET, 's1+5000 click ' + NETWORK, 's2+5000 click ' + CHAT]
            + typed('s2', 5500, 'alphachat') + ['s2+14000 click ' + NEW, 's3+30000 quit'])


def joiner_script():
    return (['s1+5000 click ' + NETWORK, 's2+6000 click ' + CHAT] + typed('s2', 6500, 'bravochat')
            + ['s2+24000 click ' + GAME_ROW, 's2+25000 click ' + JOIN, 's3+6000 quit'])


def check(tag, alpha, bravo):
    bad = []
    for inst, other in ((alpha, 'bravo'), (bravo, 'alpha')):
        m = BAD.search(inst.out)
        if m:
            bad.append('%s %s: %r' % (tag, inst.name, m.group(0)))
        if inst.code not in (0, 'stopped'):
            bad.append('%s %s: exit %s' % (tag, inst.name, inst.code))
        if not re.search(r'^list text id 1343: \[%s\] %schat' % (other.capitalize(), other), inst.out, re.M | re.I):
            bad.append('%s %s: the other side\'s chat line ("%schat") never shown' % (tag, inst.name, other))
    line = re.search(r'^  control 1668 Button style ([0-9a-f]+) ', alpha.out, re.M)
    if not line or not int(line.group(1), 16) & 0x08000000:
        bad.append('%s Alpha: main menu Internet button not disabled (%s)' % (tag, line.group(0) if line else 'not listed'))
    if re.search(r': 0x111 684 ', alpha.out):
        bad.append('%s Alpha: a click on the disabled Internet button reached the menu' % tag)
    if not re.search(r'^dialog shown #3 ', bravo.out, re.M):
        bad.append('%s Bravo: did not reach the join screen (Alpha\'s game not seen or not joined)' % tag)
    return bad


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--game-exe', default='')
    ap.add_argument('--work', required=True)
    a = ap.parse_args()
    if not a.data_dir or not os.path.isdir(a.data_dir) or not a.game_exe or not os.path.isfile(a.game_exe):
        print('lan-lobby: no installed game: skipped')
        return SKIP
    lock = lan_pair.pair_lock()
    bad = []
    try:
        for restricted in ('Bravo', 'Alpha'):
            tag = 'no-broadcast %s:' % restricted
            work = os.path.join(a.work, 'restricted-' + restricted.lower())
            alpha = lan_pair.Instance('Alpha', a.binary, work, a.data_dir, a.game_exe, host_script())
            bravo = lan_pair.Instance('Bravo', a.binary, work, a.data_dir, a.game_exe, joiner_script())
            for inst in (alpha, bravo):
                inst.env['RA2_NATIVE_MSG_LOG'] = '1'
                inst.env['RA2_NATIVE_NET_LOG'] = '1'
                inst.env['RA2_NATIVE_LIST_LOG'] = '1'
                # The session's UniqueID (SessionClass::Compute_Unique_ID, session.cpp: timeGetTime,
                # the free disk space capped at 2 GB, then PATH through Add_CRC, which keeps only a
                # few of the time's bits) came out equal for the two games about one run in two, and
                # each then ignores the other's chat announcements (as two original games would). A
                # PATH ending in each one's own name makes the IDs differ in their low bits.
                inst.env['PATH'] = inst.env.get('PATH', '') + ':/ra2-lan-' + inst.name
                inst.env.pop('RA2_NATIVE_TEST_NO_BROADCAST', None)
                if inst.name == restricted:
                    inst.env['RA2_NATIVE_TEST_NO_BROADCAST'] = '1'
            try:
                alpha.start()
                bravo.start()
                bravo.finish(240, stop=True)
                alpha.finish(120, stop=True)
            finally:
                alpha.end()
                bravo.end()
            found = check(tag, alpha, bravo)
            if restricted in (alpha.name, bravo.name):
                inst = alpha if restricted == 'Alpha' else bravo
                if 'broadcast FAILED' not in inst.out:
                    found.append('%s %s: the test hook did not refuse its broadcasts' % (tag, restricted))
            print('lan-lobby: %s %s' % (tag, 'ok' if not found else '%d problems' % len(found)))
            bad += found
    finally:
        lock.close()
    for b in bad:
        print('lan-lobby: FAIL ' + b)
    if not bad:
        print('lan-lobby: PASS (chat both ways, games seen and joined with either side unable to broadcast, Internet disabled)')
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main())
