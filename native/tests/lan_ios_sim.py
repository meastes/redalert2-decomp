#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""A LAN game between this Mac and the iOS app in a Simulator (lane C22, the user's LAN game of
2026-11-16 between a Mac and the iPhone Duo). Not a ctest: it needs a Simulator device and the
iOS build; run by hand, e.g. on the "RA2 Duo Validation" device:

  lan_ios_sim.py --binary build/native/ra2-native --app build/ios-sim-b/RA2.app
                 --device 3F7CDD1E-A992-4FF8-A898-10CDF3A28920 --data-dir <installed RA2>
                 --game-exe <game.exe> --work <dir> [--frames 9000] [--hd 0|2|3] [--no-backgrounds]

Alpha (this Mac, tests/lan_pair.py's host: Network, New, Start Game, orders) hosts; Bravo (the iOS
app in the Simulator, RA2_NATIVE_TEST_NO_BROADCAST=1 as on a device without the multicast
entitlement) joins by its menus' control ids (input-script "@ctl=<id>": Network 1400, the Games
list 1353's second row, Join 1741, Accept 1439). Both fire a tank on one of their own
(RA2_NATIVE_ORDERS `shoot`). During the game Bravo really goes to the background twice (another
app launched in the Simulator: Settings): once for a few seconds (it must run on and come back
in step) and once for good (its background time runs out: it must leave the game and Alpha play
on without the reconnect dialog). The device is booted if it was not, and shut down again after.

Reports: frames played together with equal CRCs, each side's frame rate (host stats lines),
Bravo's background lines, any reconnect (template 234) or out-of-sync ("Reconnection Error!",
TXT_OUT_OF_SYNC) on either side. Exit 0 when all hold, 1 otherwise, 77 without the inputs."""
import argparse
import os
import re
import shutil
import subprocess
import sys
import time

import game_process
import lan_pair

SKIP = 77
BUNDLE = 'io.github.ra2native.RA2'
CRC = lan_pair.CRC
STATS = re.compile(r'^host: stats: ([\d.]+) frames/s', re.M)
RECONNECT = re.compile(r'^dialog shown #\d+ \(template 234\)', re.M)
OUT_OF_SYNC = re.compile(r'out of sync|Print_CRCs|SYNC\d\.TXT', re.I)
ORDERS = '2000 health; 2010 shoot MTNK; 2010 shoot HTNK; 2400 health'


def simctl(*args, check=True, capture=True):
    r = subprocess.run(['xcrun', 'simctl'] + list(args), capture_output=capture, text=True)
    if check and r.returncode != 0:
        raise RuntimeError('simctl %s: %s' % (' '.join(args), (r.stderr or '').strip()))
    return r


def device_state(udid):
    out = simctl('list', 'devices', check=False).stdout
    m = re.search(re.escape(udid) + r'\) \((\w+)\)', out)
    return m.group(1) if m else None


def joiner_script():
    """Bravo's menus by control id (any screen size), then nothing: the backgrounds come from
    the driver (another app launched in the Simulator)"""
    return ['s1+3000 click @ctl=1400', 's2+8000 click @ctl=1353/0,-70', 's2+9000 click @ctl=1741',
            's3+4000 click @ctl=1439']


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--app', required=True)
    ap.add_argument('--device', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--game-exe', default='')
    ap.add_argument('--work', required=True)
    ap.add_argument('--frames', type=int, default=9000)
    ap.add_argument('--hd', default='3', help='HD vehicles chosen on both sides: 0, 2 or 3')
    ap.add_argument('--no-backgrounds', action='store_true')
    a = ap.parse_args()
    if not (a.data_dir and os.path.isdir(a.data_dir) and a.game_exe and os.path.isfile(a.game_exe) and os.path.isdir(a.app)):
        print('lan-ios-sim: inputs missing; skipped')
        return SKIP
    state = device_state(a.device)
    if state is None:
        print('lan-ios-sim: no device %s; skipped' % a.device)
        return SKIP
    work = os.path.abspath(a.work)
    os.makedirs(work, exist_ok=True)
    booted_here = state != 'Booted'
    lock = lan_pair.pair_lock()
    problems = []
    alpha = None
    bravo_proc = None
    bravo_log = None
    try:
        if booted_here:
            simctl('boot', a.device)
        simctl('bootstatus', a.device, '-b')
        simctl('install', a.device, os.path.abspath(a.app))
        # Bravo's player name in the app's RA2.INI (Documents/Game: the app's write folder).
        container = simctl('get_app_container', a.device, BUNDLE, 'data').stdout.strip()
        game = os.path.join(container, 'Documents', 'Game')
        os.makedirs(game, exist_ok=True)
        with open(os.path.join(game, 'RA2.INI'), 'w') as f:
            f.write('[MultiPlayer]\nHandle=%s\n' % lan_pair.handle('Bravo'))
        script = os.path.join(work, 'bravo.script')
        with open(script, 'w') as f:
            f.write('\n'.join(joiner_script()) + '\n')

        # The host starts the game a minute after its host screen (the Simulator app starts slower).
        host = [l.replace('s3+20000 ', 's3+60000 ').replace('s3+24000 ', 's3+64000 ') for l in lan_pair.host_script(a.frames)]
        alpha = lan_pair.Instance('Alpha', a.binary, work, a.data_dir, a.game_exe, host)
        alpha.env['RA2_NATIVE_ENHANCEMENTS'] = 'hdvehicles=%s' % a.hd
        alpha.env['RA2_NATIVE_ORDERS'] = ORDERS
        env = dict(os.environ)
        child = {'RA2_NATIVE_LOG_STDERR': '1', 'RA2_NATIVE_IPX_PORT': lan_pair.IPX_PORT, 'RA2_NATIVE_TEST_NO_BROADCAST': '1',
                 'RA2_NATIVE_DIALOG_LOG': '1', 'RA2_NATIVE_CRC_LOG': '1', 'RA2_NATIVE_DEBUG_LOG': '1',
                 'RA2_NATIVE_PROGRESS_LOG': '1', 'RA2_NATIVE_MOVIE_SKIP': '1', 'RA2_GAME_EXE': a.game_exe,
                 # The Duo's settings from the user's log (render scale 2 comes with Zoom gestures).
                 'RA2_NATIVE_ENHANCEMENTS': 'hdvehicles=%s,zoomgestures=on,vsync=on,scaling=smooth,uisize=1.25' % a.hd,
                 'RA2_NATIVE_ORDERS': ORDERS,
                 'RA2_NATIVE_FPS_LOG': '1', 'RA2_NATIVE_NO_CRASH_DIALOG': '1'}
        for k, v in child.items():
            env['SIMCTL_CHILD_' + k] = v
        alpha.start()
        time.sleep(4)
        bravo_log = open(os.path.join(work, 'Bravo.out'), 'w+', errors='replace')
        bravo_proc = game_process.Popen(['xcrun', 'simctl', 'launch', '--console-pty', '--terminate-running-process', a.device,
                                         BUNDLE, '--data-dir', a.data_dir, '--input-script=' + script],
                                        env=env, stdout=bravo_log, stderr=subprocess.STDOUT)

        def bravo_frame():
            bravo_log.flush()
            with open(bravo_log.name, errors='replace') as f:
                found = lan_pair.FRAME.findall(f.read())
            return max(int(x) for x in found) if found else 0

        # The backgrounds, by Bravo's game frames: a short one, then one for good.
        plan = [] if a.no_backgrounds else [(2600, 'short'), (5000, 'final')]
        start = time.time()
        while plan and time.time() - start < 1500 and alpha.p.poll() is None:
            f = bravo_frame()
            if f >= plan[0][0]:
                kind = plan.pop(0)[1]
                print('lan-ios-sim: Bravo at frame %d: background (%s)' % (f, kind))
                simctl('launch', a.device, 'com.apple.Preferences', check=False)
                if kind == 'short':
                    time.sleep(5)
                    simctl('launch', a.device, BUNDLE, check=False)      # back to the foreground
            time.sleep(2)
        alpha.finish_paced(a.frames, cap=2400)
    finally:
        if alpha:
            alpha.end()
        if bravo_proc:
            bravo_proc.end()
        simctl('terminate', a.device, BUNDLE, check=False)
        simctl('terminate', a.device, 'com.apple.Preferences', check=False)
        if booted_here:
            simctl('shutdown', a.device, check=False)
            print('lan-ios-sim: device shut down again')
    bravo_log.seek(0)
    bravo_out = bravo_log.read()
    bravo_log.close()
    with open(os.path.join(work, 'Alpha.log'), 'w') as f:
        f.write(alpha.out)

    ca = {int(f): v for f, v in CRC.findall(alpha.out)}
    cb = {int(f): v for f, v in CRC.findall(bravo_out)}
    both = sorted(set(ca) & set(cb))
    differ = [f for f in both if ca[f] != cb[f]]
    print('lan-ios-sim: %d frames with both CRCs (Alpha %d, Bravo %d), %d differ' % (len(both), len(ca), len(cb), len(differ)))
    if differ:
        problems.append('out of sync from frame %d' % differ[0])
    if len(both) < 2000:
        problems.append('only %d frames played together' % len(both))
    for name, out in (('Alpha', alpha.out), ('Bravo', bravo_out)):
        rates = [float(x) for x in STATS.findall(out)]
        print('lan-ios-sim: %s frame rates %s' % (name, rates))
        if RECONNECT.search(out):
            problems.append('%s showed the reconnect dialog' % name)
        if OUT_OF_SYNC.search(out):
            problems.append('%s reported out of sync' % name)
        hd = re.findall(r'^hd-vehicles: [^\n]*game[^\n]*$', out, re.M)
        print('lan-ios-sim: %s %s' % (name, hd))
    for line in re.findall(r'^host: (?:background|foreground|inactive|active again|pause|game held)[^\n]*$|^host: background time[^\n]*$',
                           bravo_out, re.M):
        print('lan-ios-sim: Bravo %s' % line)
    if not a.no_backgrounds:
        for want in ('host: background (network game: the game runs on', 'host: foreground (network game ran on)'):
            if want not in bravo_out:
                problems.append('Bravo: no "%s"' % want)
        # The final background: Bravo leaves when its background time runs low, or (the Simulator
        # gives an app launched from simctl unlimited background time: no estimate is logged) plays
        # on in the background to Alpha's end, in step.
        if 'left the network game, holding' not in bravo_out and len(both) < a.frames - 100:
            problems.append('Bravo neither left the game nor played on to its end in the background')
    hits = re.findall(r'^harness-shoot #\d+ #(\d+)$', alpha.out + '\n' + bravo_out, re.M)
    print('lan-ios-sim: shots ordered at %s' % hits)
    if problems:
        print('lan-ios-sim: FAILED (%s)' % '; '.join(problems))
        return 1
    print('lan-ios-sim: passed')
    return 0


if __name__ == '__main__':
    sys.exit(main())
