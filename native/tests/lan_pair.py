#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest native-lan-pair (lane C17w, user 2026-11-15: LAN on by default): two games on this
machine play a Network (LAN) game against each other over UDP, through the real menus.

Each instance (--app, its own fresh settings folder, no RA2_NATIVE_LAN in the environment: the
Network button must open the lobby by default) gets a player name in RA2.INI ([MultiPlayer]
Handle, the game's own hex form). Alpha: Network, New, Start Game. Bravo: Network, the game in the
Games list, Join, Accept. Then they play (the start units, both sides giving orders, control
groups made and reused: the user's "crashed after a few minutes" was Extract_Compressed_Events
on reused group orders, Scout1231 r21) for --frames frames on the host (about 60 a second: five minutes).
Meanwhile:
  * Alpha holds the mouse on each of the screen's edges for 100 frames (edge scrolling), and the
    view's fastest movement per game frame is measured
    (RA2_NATIVE_INPUT_LOG: harness-view lines) against the same in a one-player skirmish (the
    user's "edge scrolling way too fast" in a network game: Wait_For_Players scrolled on each
    pass of its wait loop, Scout1223 r19 / Scout1231 r21);
  * Bravo opens Escape > Game Controls > Sound, waits there, then Back, Back, Resume Mission
    (the user's Sound-menu crash in a network game: ThemeClass's view, Scout1236 r22), twice;
then Alpha leaves (its frame count reached) and Bravo, told the host left, quits by script.

Passes when: both reach the game; neither log has a fault; the per-frame game CRCs both logged
(RA2_NATIVE_CRC_LOG with RA2_NATIVE_DEBUG_LOG) agree on every frame both logged, and there are at
least --frames / 2 of them; Bravo's Sound dialog opened (its controls logged) both times and its
game went on after; the network edge-scroll rate is at most 1.5 times the skirmish's (the fastest edge of each; neither 0).

  lan_pair.py --binary <ra2-native> --data-dir <installed RA2> --game-exe <game.exe> --work <dir>
              [--frames 18000] [--binary-b <second build> [--wine-prefix-b <dir> [--wine <wine>]]]

--binary-b runs Bravo from another build (a Windows build under Wine: Mac to Windows over UDP
on one machine). Without the installed game it exits 77 (skipped)."""
import argparse
import os
import re
import shutil
import subprocess
import sys
import time

import wine as winehost

SKIP = 77
BAD = re.compile(r'unrecovered|\btrap\b|signal \d+|SIG(SEGV|BUS|ILL|ABRT|FPE)|crash:|\babort\b', re.I)
CRC = re.compile(r'^crc (\d+) ([0-9a-f]{8})$', re.M)
# The Sound dialog's own control (RA2_NATIVE_MSG_LOG's control list when it is shown).
SOUND = re.compile(r'^  control \d+ \S+ style \w+ shown at [\d,]+ \d+x\d+ "GUI:SoundVolume"', re.M)
VIEW = re.compile(r'^harness-view (\d+) @(-?\d+),(-?\d+)', re.M)


def handle(name):
    """RA2.INI's [MultiPlayer] Handle: the name's UTF-16 codes in hex, each followed by a comma
    (INIClass::Put_Unicode 0x0050ECF0)"""
    return ''.join('%x,' % ord(c) for c in name)


# The shell at 1024x768 (RA2_NATIVE_MSG_LOG): main menu Network (756,367 156x42), the lobby's New
# (756,283) and Join (756,325), the Games list's second row (the first is "Lobby"), the host
# screen's Start Game and the join screen's Accept (both 756,325). "s<k>"/"d<k>": the k-th dialog
# settled or shown (host.cpp). Start Game is clicked twice (the second lands on the map if the
# first started the game: a deselect).
# The four edges in turn, 100 frames each (a start position at the map's border cannot scroll
# past it: the fastest edge is the rate compared).
EDGE = [(200, 'move 0 300'), (300, 'move 500 300'), (320, 'move 1023 300'), (420, 'move 500 300'),
        (440, 'move 400 0'), (540, 'move 500 300'), (560, 'move 400 767'), (660, 'move 500 300')]
EDGE_WINDOWS = [(220, 300, 'x'), (340, 420, 'x'), (460, 540, 'y'), (580, 660, 'y')]


def orders(start, end, every):
    """the start tanks selected by type, grouped (Ctrl+1), and the group called back and sent
    somewhere each round: reused group orders"""
    lines = []
    for k, f in enumerate(range(start, end, every)):
        dx = (5, 0, -5, 0)[k % 4]
        dy = (0, 5, 0, -5)[k % 4]
        lines += ['%d click @MTNK' % f, '%d keydown Left_Ctrl' % (f + 10), '%d key 1' % (f + 12), '%d keyup Left_Ctrl' % (f + 16),
                  '%d click @MTNK+%d,%d' % (f + 60, dx, dy), '%d key 1' % (f + 200), '%d click @MTNK+%d,%d' % (f + 210, -dx, -dy)]
    return lines


def host_script(frames):
    lines = ['s1+1000 click 834 388', 's2+1500 click 834 304', 's3+20000 click 834 346', 's3+24000 click 834 346']
    lines += ['%d %s' % e for e in EDGE]
    lines += orders(800, frames - 400, 600)
    lines += ['%d quit' % frames]
    return lines


def joiner_script(frames):
    lines = ['s1+1000 click 834 388', 's2+5000 click 539 238', 's2+6000 click 834 346', 's3+3000 click 834 346']
    # Escape > Game Controls (877,227) > Sound (877,227); Back (877,702) twice, Resume Mission
    # (877,702); each click 5 s after its dialog is shown (the column's slide-in done: a click
    # during the slide lands on the dialog, not the button; the "settled" anchor comes late in a
    # network game, whose frames the menu's callback runs). Dialogs: 1-3 the shell, 4 Game Options, 5 Game Controls, 6 Sound, 7 Game Controls,
    # 8 Game Options; the second round 9-13.
    for n, f in ((4, 900), (9, 2400)):
        lines += ['%d key Escape' % f, 'd%d+5000 click 939 239' % n, 'd%d+5000 click 939 239' % (n + 1),
                  'd%d+8000 click 939 714' % (n + 2), 'd%d+5000 click 939 714' % (n + 3), 'd%d+5000 click 939 714' % (n + 4)]
    lines += orders(1000, frames - 400, 600)
    return lines


def skirmish_script():
    return ['%d %s' % e for e in EDGE] + ['700 quit']


class Instance:
    def __init__(self, name, binary, work, data_dir, game_exe, script, prefix=None, skirmish=False):
        self.name, self.prefix = name, prefix
        folder = os.path.join(work, name)
        shutil.rmtree(folder, ignore_errors=True)
        game = os.path.join(folder, 'home', 'Library', 'Application Support', 'RA2Native', 'Game')
        os.makedirs(game)
        ini = '[MultiPlayer]\nHandle=%s\n\n[Video]\nScreenWidth=1024\nScreenHeight=768\n' % handle(name)
        with open(os.path.join(game, 'RA2.INI'), 'w') as f:
            f.write(ini)
        if prefix:      # a Windows build's settings folder: the prefix's AppData\Local\RA2Native
            import glob
            for user in glob.glob(os.path.join(prefix.folder, 'drive_c', 'users', '*', 'AppData', 'Local')):
                if os.path.basename(os.path.dirname(os.path.dirname(user))) == 'Public':
                    continue
                os.makedirs(os.path.join(user, 'RA2Native', 'Game'), exist_ok=True)
                with open(os.path.join(user, 'RA2Native', 'Game', 'RA2.INI'), 'w') as f:
                    f.write(ini)
        path = os.path.join(folder, 'input.script')
        self.cmd_script = path
        with open(path, 'w') as f:
            f.write('\n'.join(script) + '\n')
        env = dict(os.environ)
        for k in ('RA2_NATIVE_LAN', 'RA2_NATIVE_ENHANCEMENTS', 'RA2_NATIVE_SETUP', 'RA2_DATA_DIR', 'RA2_NATIVE_TRAP_CONTINUE'):
            env.pop(k, None)
        env.update({'HOME': os.path.join(folder, 'home'), 'RA2_GAME_EXE': game_exe, 'RA2_NATIVE_LOG_STDERR': '1',
                    'RA2_NATIVE_MOVIE_SKIP': '1', 'RA2_NATIVE_IGNORE_FOCUS': '1', 'RA2_NATIVE_CRASH_TRACE': '1',
                    'RA2_NATIVE_SCRIPT_AS_USER': '1', 'RA2_NATIVE_DIALOG_LOG': '1', 'RA2_NATIVE_CRC_LOG': '1',
                    'RA2_NATIVE_DEBUG_LOG': '1', 'RA2_NATIVE_INPUT_LOG': '20', 'RA2_NATIVE_PROGRESS_LOG': '1'})
        if name == 'Bravo':
            env['RA2_NATIVE_MSG_LOG'] = '1'      # the Sound dialog's controls
        if skirmish:
            write = os.path.join(folder, 'write')
            os.makedirs(write)
            cmd = [binary, '--data-dir', data_dir, '--run-to=frames', '--run-seconds=240', '--write-dir=' + write,
                   '--game-args=-noaudio -CD.', '--skirmish=#5', '--skirmish-frames=700', '--input-script=' + path]
        else:
            # Bravo keeps its sound: Game Controls enables Sound only with an audio device
            # (Game_Controls_Proc 0x004D0030, AudioDeviceAvailable).
            cmd = [binary, '--app', '--data-dir', data_dir, '--input-script=' + path] + ([] if name == 'Bravo' else ['--game-args=-noaudio'])
        if prefix:
            cmd, env = prefix.command(cmd), prefix.env(env)
        self.cmd, self.env, self.out, self.code = cmd, env, '', None

    def start(self):
        self.log = open(os.path.join(os.path.dirname(self.cmd_script), self.name + '.out'), 'w+', errors='replace')
        self.p = subprocess.Popen(self.cmd, env=self.env, stdout=self.log, stderr=subprocess.STDOUT)

    def finish(self, timeout, stop=False):
        """wait for the game (stop: end it, as a player closing the window would, after timeout)"""
        try:
            self.code = self.p.wait(timeout=timeout)
        except subprocess.TimeoutExpired:
            self.p.terminate()
            try:
                self.p.wait(timeout=20)
            except subprocess.TimeoutExpired:
                self.p.kill()
                self.p.wait()
            self.code = 'stopped' if stop else 'timeout'
        self.log.seek(0)
        self.out = self.log.read()
        self.log.close()

    def run(self, timeout):
        self.start()
        self.finish(timeout)


def scroll_rate(out):
    """the view's movement per game frame while the mouse rests on an edge: the largest of the
    windows' rates (pixels a frame), and the windows' readings"""
    views = {int(m.group(1)): (int(m.group(2)), int(m.group(3))) for m in VIEW.finditer(out)}
    rates = []
    for lo, hi, axis in EDGE_WINDOWS:
        if lo in views and hi in views:
            k = 0 if axis == 'x' else 1
            rates.append(abs(views[hi][k] - views[lo][k]) / float(hi - lo))
    return rates


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--binary-b', default='')
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--game-exe', default='')
    ap.add_argument('--work', required=True)
    ap.add_argument('--frames', type=int, default=18000, help='the host\'s game frames (about 60 a second: 5 minutes)')
    ap.add_argument('--wine-prefix-b', default='')
    ap.add_argument('--wine', default='')
    a = ap.parse_args()
    if not a.data_dir or not os.path.isdir(a.data_dir) or not a.game_exe or not os.path.isfile(a.game_exe):
        print('lan-pair: no installed game given; skipped')
        return SKIP
    prefix = None
    if a.binary_b.lower().endswith('.exe'):
        wine = winehost.find_wine(a.wine)
        if not wine or not a.wine_prefix_b:
            print('lan-pair: a Windows Bravo needs Wine and --wine-prefix-b; skipped')
            return SKIP
        prefix = winehost.Prefix(wine, a.wine_prefix_b)
    work = os.path.abspath(a.work)
    os.makedirs(work, exist_ok=True)
    problems = []

    # The one-player reference: the same edge holds in a skirmish.
    solo = Instance('Solo', a.binary, work, a.data_dir, a.game_exe, skirmish_script(), skirmish=True)
    solo.run(300)
    solo_rates = scroll_rate(solo.out)

    alpha = Instance('Alpha', a.binary, work, a.data_dir, a.game_exe, host_script(a.frames))
    bravo = Instance('Bravo', a.binary_b or a.binary, work, a.data_dir, a.game_exe, joiner_script(a.frames), prefix=prefix)
    limit = 180 + a.frames // 20
    # Alpha leaves at its frame count; Bravo, told the host has gone, is then closed (20 s on).
    alpha.start()
    time.sleep(4)
    bravo.start()
    try:
        alpha.finish(limit)
        bravo.finish(20, stop=True)
    finally:
        if prefix:
            prefix.close()
    for inst in (solo, alpha, bravo):
        with open(os.path.join(work, inst.name + '.log'), 'w') as f:
            f.write(inst.out)

    for inst in (alpha, bravo):
        bad = [l for l in inst.out.splitlines() if BAD.search(l) and not l.startswith('net:')]
        if inst.code not in (0, 'stopped') or bad:
            problems.append('%s: exit %s, %d bad lines%s' % (inst.name, inst.code, len(bad), (': ' + bad[0][:120]) if bad else ''))
        if 'Starting scnenario' not in inst.out:
            problems.append('%s never reached the game' % inst.name)
    if 'Multiplayer is not available' in alpha.out + bravo.out or 'not available in this build' in alpha.out + bravo.out:
        problems.append('the Network button showed the not-available box')
    ca = {int(f): v for f, v in CRC.findall(alpha.out)}
    cb = {int(f): v for f, v in CRC.findall(bravo.out)}
    both = sorted(set(ca) & set(cb))
    differ = [f for f in both if ca[f] != cb[f]]
    print('lan-pair: %d frames with both CRCs (Alpha %d, Bravo %d), %d differ' % (len(both), len(ca), len(cb), len(differ)))
    if differ:
        problems.append('out of sync from frame %d' % differ[0])
    if len(both) < a.frames // 2:
        problems.append('%d frames played together (wanted at least %d)' % (len(both), a.frames // 2))
    sounds = len(re.findall(SOUND, bravo.out))
    print('lan-pair: Bravo opened the Sound dialog %d times' % sounds)
    if sounds < 2:
        problems.append('the Sound dialog opened %d times in the network game (wanted 2)' % sounds)
    rates = scroll_rate(alpha.out)
    print('lan-pair: edge scroll pixels a frame: network %s, skirmish %s' % (rates, solo_rates))
    net, one = max(rates or [0]), max(solo_rates or [0])
    if net <= 0 or one <= 0:
        problems.append('no edge scroll measured (network %.2f, skirmish %.2f pixels a frame)' % (net, one))
    elif net > 1.5 * one:
        problems.append('edge scroll %.2f pixels a frame in the network game, %.2f in a skirmish' % (net, one))
    if problems:
        print('lan-pair: FAILED (%s)' % '; '.join(problems))
        return 1
    print('lan-pair: passed')
    return 0


if __name__ == '__main__':
    sys.exit(main())
