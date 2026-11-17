#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest native-cursor-stress (lane C23): the software cursor's thread against the main thread's
drawing. Lane C22 saw an intermittent SIGSEGV in the blitter reached from Mouse_Thread ->
WWMouseClass::Low_Show_Mouse -> XSurface::Copy_From (2 of about 7 LAN runs with HD vehicles 2x):
the mouse thread locked the primary surface (DSurface::Lock 0x004AAC80) while the main thread held
it for its frame copy (GScreenClass::Blit 0x004E0C90), found it locked and so took BufferPtr as it
was, just as the main thread's last DSurface::Unlock set it to null; the cursor was then blitted
through a null-based pointer.

The run: a skirmish ('#5') with HD vehicles 2x (the 2x layer's copies hold the primary locked the
longest) and Map zoom 1.5x, the cursor moving over the battlefield every few frames and, through
RA2_NATIVE_TEST_CURSOR_JITTER, redrawn by the mouse thread on every one of its turns, which come every
millisecond instead of every 16;
the view scrolling, two short backgrounds. Passes when the
game plays its frames to the end without a fault (no "returns no buffer" diagnostic either, which
the instrumented build of the investigation printed). Before the fix runs of 2,400 frames crashed
in 3 of 6 (16 ms turns: 1 of 3; 1 ms turns: 2 of 3, after about 20 s); after it none did.

  cursor_stress.py --binary <ra2-native> --data-dir <installed RA2> --game-exe <game.exe>
                   [--frames 3600] [--hd 2] [--repeat 1] [--log <file>]

Without the data folder or game.exe it exits 77 (skipped)."""
import argparse
import os
import re
import shutil
import subprocess
import sys
import tempfile

import game_process

SKIP = 77
BAD = re.compile(r'unrecovered|trap|signal \d+|SIG(SEGV|BUS|ILL|ABRT|FPE)|crash:|abort|returns no buffer', re.I)


def script(frames):
    lines = []
    # The cursor over the battlefield: a loop of points every 3 frames.
    points = [(200, 150), (420, 160), (560, 300), (420, 450), (200, 440), (120, 300)]
    for k, f in enumerate(range(30, frames - 20, 3)):
        x, y = points[k % len(points)]
        lines.append('%d move %d %d' % (f, x + (k * 7) % 40, y + (k * 5) % 30))
    # The view scrolls.
    for f in range(400, frames - 100, 600):
        lines += ['%d keydown Right' % f, '%d keyup Right' % (f + 30), '%d keydown Down' % (f + 40), '%d keyup Down' % (f + 70)]
    lines = sorted(lines, key=lambda l: int(l.split()[0]))
    # Backgrounds (the iOS lifecycle as SDL reports it; a skirmish pauses there, its frames stop, so
    # these lines are timed from the script's start): 3 s away, twice.
    lines += ['t25000 background', 't28000 foreground', 't45000 background', 't48000 foreground']
    return lines


def run_once(a, k):
    env = dict(os.environ, RA2_GAME_EXE=a.game_exe, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_CRASH_TRACE='1',
               RA2_NATIVE_TEST_CURSOR_JITTER='1', RA2_NATIVE_HD_BUDGET_MS='0', RA2_NATIVE_IGNORE_FOCUS='1',
               RA2_NATIVE_ENHANCEMENTS='hdvehicles=%s,mapzoom=1.5' % a.hd)
    env.pop('RA2_NATIVE_TRAP_CONTINUE', None)
    work = tempfile.mkdtemp(prefix='ra2-cursor-')
    try:
        path = os.path.join(work, 'input.script')
        with open(path, 'w') as f:
            f.write('\n'.join(script(a.frames)) + '\n')
        write_dir = os.path.join(work, 'write')
        os.mkdir(write_dir)
        cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=frames', '--run-seconds=%d' % a.seconds,
               '--write-dir=' + write_dir, '--game-args=-noaudio -CD.', '--skirmish=#5', '--skirmish-order=both',
               '--skirmish-frames=%d' % a.frames, '--input-script=' + path]
        p = game_process.run(cmd, env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, errors='replace')
    finally:
        shutil.rmtree(work, ignore_errors=True)
    out = p.stdout
    if a.log:
        with open(a.log if a.repeat == 1 else '%s.%d' % (a.log, k), 'w') as f:
            f.write(out)
    problems = []
    bad = [l for l in out.splitlines() if BAD.search(l)]
    if p.returncode != 0 or bad:
        problems.append('exit %d, %d bad lines' % (p.returncode, len(bad)))
    if a.hd != '0' and 'hd-vehicles: %sx for this game' % a.hd not in out:
        problems.append('the game did not take HD vehicles %sx' % a.hd)
    if 'host: background' not in out or 'host: foreground' not in out:
        problems.append('no background and foreground')
    frames = [int(m.group(1)) for m in re.finditer(r'^run-to:\s+frames run (\d+)', out, re.M)]
    last = max(frames) if frames else 0
    if last < a.frames - 50:
        problems.append('%d frames played (wanted %d)' % (last, a.frames))
    for l in bad[:10]:
        print('  ' + l)
    return problems, last


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default=os.environ.get('RA2_NATIVE_CENSUS_DATA_DIR', ''))
    ap.add_argument('--game-exe', default=os.environ.get('RA2_NATIVE_CENSUS_GAME_EXE', ''))
    ap.add_argument('--frames', type=int, default=3600)
    ap.add_argument('--seconds', type=int, default=330)
    ap.add_argument('--hd', default='2', choices=['0', '2', '3'])
    ap.add_argument('--repeat', type=int, default=1, help='runs (all must pass)')
    ap.add_argument('--log', default='')
    a = ap.parse_args()
    if not a.data_dir or not os.path.isdir(a.data_dir) or not a.game_exe or not os.path.isfile(a.game_exe):
        print('native-cursor-stress: skipped (set RA2_NATIVE_CENSUS_DATA_DIR and RA2_NATIVE_CENSUS_GAME_EXE)')
        return SKIP
    failed = 0
    for k in range(a.repeat):
        problems, last = run_once(a, k)
        print('native-cursor-stress: run %d: %d frames%s' % (k + 1, last, (': ' + '; '.join(problems)) if problems else ', ok'))
        failed += bool(problems)
    if failed:
        print('native-cursor-stress: FAILED (%d of %d runs)' % (failed, a.repeat))
        return 1
    print('native-cursor-stress: passed')
    return 0


if __name__ == '__main__':
    sys.exit(main())
