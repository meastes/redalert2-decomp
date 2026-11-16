#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest native-windows-gui (lane C15d; README "Windows build"): the players' RA2Native.exe.

Static checks of the two executables' PE headers: RA2Native.exe is linked for the GUI
subsystem (2: no console window), ra2-native.exe stays the console build (3) the test runs
use, and both carry our icon (an RT_GROUP_ICON resource; packaging/make_icon.py --ico).
Then RA2Native.exe runs under Wine in a fresh prefix with no --app: started without a
console it is the app by itself (host_win.cpp launched_as_app), reaches the main menu and
quits by script. Its output goes to the log file, not to the process's output:
%LOCALAPPDATA%\\RA2Native\\Logs\\ra2-native.log in the prefix must hold the session line, the
game folder, the main menu and the window icon read from the user's game.exe at run time.
Skipped (77) without the installed game or without Wine (the static checks still run then
and fail the test when wrong).

  windows_gui.py --gui <RA2Native.exe> --console <ra2-native.exe> --data-dir <game folder>
                 --game-exe <game.exe> --work <dir> [--wine-prefix <dir> [--wine <wine>]]
"""
import argparse
import glob
import os
import shutil
import struct
import subprocess
import sys

import game_process
import wine as winehost


def pe_info(path):
    """(subsystem, RT_GROUP_ICON count) of a PE32+ file."""
    b = open(path, 'rb').read()
    pe = struct.unpack_from('<I', b, 0x3C)[0]
    if b[pe:pe + 4] != b'PE\0\0':
        raise ValueError('%s: not a PE file' % path)
    opt = pe + 24
    subsystem = struct.unpack_from('<H', b, opt + 68)[0]
    count = struct.unpack_from('<H', b, pe + 6)[0]
    size = struct.unpack_from('<H', b, pe + 20)[0]
    rsrc = struct.unpack_from('<I', b, opt + 112 + 2 * 8)[0]     # PE32+: data directories at +112
    sections = [struct.unpack_from('<IIII', b, opt + size + 40 * i + 8) for i in range(count)]

    def offset(rva):
        for vsize, va, rsize, raw in sections:
            if va <= rva < va + max(vsize, rsize):
                return rva - va + raw
        return None
    groups = 0
    root = offset(rsrc) if rsrc else None
    if root is not None:
        named, ids = struct.unpack_from('<HH', b, root + 12)
        for i in range(named + ids):
            name, sub = struct.unpack_from('<II', b, root + 16 + 8 * i)
            if name == 14 and sub & 0x80000000:
                n1, n2 = struct.unpack_from('<HH', b, root + (sub & 0x7FFFFFFF) + 12)
                groups = n1 + n2
    return subsystem, groups


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--gui', required=True)
    ap.add_argument('--console', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--game-exe', default='')
    ap.add_argument('--work', required=True)
    ap.add_argument('--wine-prefix', default='')
    ap.add_argument('--wine', default='')
    a = ap.parse_args()
    bad = []
    for path, want, kind in ((a.gui, 2, 'GUI'), (a.console, 3, 'console')):
        subsystem, groups = pe_info(path)
        print('windows-gui: %s: subsystem %d, %d icon group(s)' % (os.path.basename(path), subsystem, groups))
        if subsystem != want:
            bad.append('%s: subsystem %d, want %d (%s)' % (os.path.basename(path), subsystem, want, kind))
        if groups < 1:
            bad.append('%s: no icon resource' % os.path.basename(path))
    if bad:
        for b in bad:
            print('windows-gui: ' + b)
        print('windows-gui: FAILED')
        return 1
    if not a.data_dir or not os.path.isdir(a.data_dir) or not a.game_exe or not os.path.isfile(a.game_exe):
        print('windows-gui: static checks passed; run skipped (set RA2_NATIVE_CENSUS_DATA_DIR and RA2_NATIVE_CENSUS_GAME_EXE)')
        return 77
    wine = winehost.find_wine(a.wine)
    if not wine or not a.wine_prefix:
        print('windows-gui: static checks passed; run skipped (no Wine; set RA2_NATIVE_WINE)')
        return 77
    work = os.path.abspath(a.work)
    shutil.rmtree(work, ignore_errors=True)
    os.makedirs(work)
    script = os.path.join(work, 'menu.script')
    with open(script, 'w') as f:
        f.write('s1+2000 quit\n')
    env = dict(os.environ)
    for k in ('RA2_NATIVE_ENHANCEMENTS', 'RA2_NATIVE_SETUP', 'RA2_DATA_DIR', 'RA2_NATIVE_LOG_STDERR'):
        env.pop(k, None)
    env.update({'RA2_GAME_EXE': a.game_exe, 'RA2_NATIVE_MOVIE_SKIP': '1', 'RA2_NATIVE_IGNORE_FOCUS': '1',
                'RA2_NATIVE_PROGRESS_LOG': '1'})
    prefix = winehost.Prefix(wine, a.wine_prefix)
    cmd = [a.gui, '--data-dir', a.data_dir, '--input-script=' + script, '--game-args=-noaudio']
    out, code, why = '', -1, ''
    try:
        for attempt in range(winehost.HOST_RETRIES + 1):
            argv = prefix.command(cmd)
            for old in glob.glob(os.path.join(prefix.folder, 'drive_c', 'users', '*', 'AppData', 'Local', 'RA2Native',
                                              'Logs', '*.log')):
                os.remove(old)
            try:
                p = game_process.run(argv, env=prefix.env(env), stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                                     text=True, errors='replace', timeout=240)
                out, code, timed_out = p.stdout, p.returncode, False
            except subprocess.TimeoutExpired as e:
                out = e.stdout.decode('utf-8', 'replace') if isinstance(e.stdout, bytes) else (e.stdout or '')
                code, timed_out = -1, True
            logs = glob.glob(os.path.join(prefix.folder, 'drive_c', 'users', '*', 'AppData', 'Local', 'RA2Native',
                                          'Logs', 'ra2-native.log'))
            log = open(logs[0], errors='replace').read() if logs else ''
            # The log, not the process output, shows how far the game got: a hang before
            # "progress: main" is the host's (wine.host_failure reads the run-to spelling).
            why = winehost.host_failure(out + log.replace('progress: main', 'run-to: reached main'), timed_out)
            if not why or attempt == winehost.HOST_RETRIES:
                break
            print('windows-gui: %s; launching again' % why)
    finally:
        prefix.close()
    with open(os.path.join(work, 'process-output.txt'), 'w') as f:
        f.write(out)
    with open(os.path.join(work, 'ra2-native.log'), 'w') as f:
        f.write(log)
    if code != 0:
        bad.append('exit %d%s' % (code, ' (%s)' % why if why else ''))
    if not log:
        bad.append('no log file in the prefix\'s AppData\\Local\\RA2Native\\Logs')
    for want, what in (('ra2-native: session started', 'the session line'),
                       ('app: game folder', 'the app\'s game folder line (app mode without --app)'),
                       ('progress: menu', 'the main menu'),
                       ('host: window icon 32x32 from game.exe', 'the window icon from game.exe')):
        if log and want not in log:
            bad.append('the log has no %s (%r)' % (what, want))
    if 'progress: menu' in out:
        bad.append('the game wrote to the process output, not only to the log')
    for b in bad:
        print('windows-gui: ' + b)
    print('windows-gui: %s (see %s)' % ('ok' if not bad else 'FAILED', work))
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main())
