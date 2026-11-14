#!/usr/bin/env python3
"""The ORIGINAL preset draws what the app drew before the enhancements (C3).

Every enhancement (platform/sdl/enhancements.cpp) is off by default, and the ORIGINAL
preset sets each one back to that default. This test runs the game to the main menu
three times with the window at 0.75x the picture (RA2_NATIVE_WINDOW_ZOOM, so the picture
is scaled and the scaling mode shows) and compares the window as drawn
(RA2_NATIVE_WINDOW_DUMP) once the menu has settled: the frames compared are taken after
the port's `menu-settled` milestone (USER32 saw the menu idle for 250 ms once it was
shown; the last button's tail still slides in for about half a second after it, timed
by WM_TIMER, so the frames compared start 2 s after it), not after a wall-clock delay
from start-up, and the system's focus reports
are ignored (RA2_NATIVE_IGNORE_FOCUS=1): on a busy machine another game window taking the
focus made the menu redraw and slide its buttons in again, and slowed menus were dumped
half drawn (C5).

The settled menu still animates (the button column's lights) on a wall clock, so two
runs never dump the same animation frame at the same moment. The animated pixels are
measured, not assumed: the 16x16 tiles (and their neighbours) holding a pixel that differs
between any two settled frames of the default run itself. Outside them:
  - the ORIGINAL run (RA2_NATIVE_ENHANCEMENTS=preset=original) must equal the default
    run (no settings) byte for byte, in every settled frame;
  - the ENHANCED run must differ (so the comparison can see the scaling at all).
The animated area must stay small (under 3% of the window), or the test fails rather
than compare little. It also checks each run's settings line. Needs the installed
game (as native-census): skipped (77) without --data-dir/--game-exe.

  enhancements_frames.py --binary <ra2-native> --data-dir <game folder> --game-exe <game.exe> --work <dir>
                         [--wine-prefix <dir> [--wine <wine>]]   the Windows build under Wine (tests/wine.py;
                                                                skipped without Wine)
"""
import argparse
import glob
import os
import shutil
import struct
import subprocess
import sys

import wine as winehost


def run(a, tag, enhancements, prefix=None):
    work = os.path.join(a.work, tag)
    shutil.rmtree(work, ignore_errors=True)
    os.makedirs(os.path.join(work, 'write'))
    script = os.path.join(work, 'menu.script')
    with open(script, 'w') as f:
        f.write('s1+%d quit\n' % a.settle_ms)
    env = dict(os.environ)
    env.update({'RA2_GAME_EXE': a.game_exe, 'RA2_NATIVE_MOVIE_SKIP': '1', 'RA2_NATIVE_WINDOW_ZOOM': '0.75',
                'RA2_NATIVE_WINDOW_DUMP': os.path.join(work, 'w'), 'RA2_NATIVE_WINDOW_DUMP_MS': '500',
                'RA2_NATIVE_IGNORE_FOCUS': '1'})
    env.pop('RA2_NATIVE_ENHANCEMENTS', None)
    if enhancements:
        env['RA2_NATIVE_ENHANCEMENTS'] = enhancements
    cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=never', '--run-seconds=%d' % a.seconds,
           '--write-dir=' + os.path.join(work, 'write'), '--game-args=-noaudio', '--input-script=' + script]
    if prefix:
        cmd, env = prefix.command(cmd), prefix.env(env)
    p = subprocess.run(cmd, env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, errors='replace')
    with open(os.path.join(work, 'run.log'), 'w') as f:
        f.write(p.stdout)
    dumps = sorted(glob.glob(os.path.join(work, 'w.*.bmp')), key=lambda s: int(s.rsplit('.', 2)[1]))
    settings = [l for l in p.stdout.splitlines() if l.startswith('enhancements: vsync')]
    settled = any(l.startswith('run-to: reached menu-settled') for l in p.stdout.splitlines())
    return dumps, (settings[0] if settings else ''), settled


def pixels(path):
    """(width, height, rows of bytes) of an uncompressed BMP (SDL_SaveBMP)."""
    b = open(path, 'rb').read()
    off = struct.unpack_from('<I', b, 10)[0]
    w, h = struct.unpack_from('<ii', b, 18)
    bpp = struct.unpack_from('<H', b, 28)[0] // 8
    stride = (w * bpp + 3) & ~3
    return w, abs(h), bpp, [b[off + y * stride: off + y * stride + w * bpp] for y in range(abs(h))]


def differing(f, g, bpp):
    """The set of (x, y) where two frames of one size differ."""
    out = set()
    for y, (r, s) in enumerate(zip(f, g)):
        if r == s:
            continue
        for x in range(len(r) // bpp):
            if r[x * bpp:(x + 1) * bpp] != s[x * bpp:(x + 1) * bpp]:
                out.add((x, y))
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--game-exe', default='')
    ap.add_argument('--work', required=True)
    ap.add_argument('--settle-ms', type=int, default=5000, help='time on the main menu after it settled (the last frames are compared)')
    ap.add_argument('--compare', type=int, default=6, help='settled frames compared from each run (the last ones)')
    ap.add_argument('--seconds', type=int, default=120)
    ap.add_argument('--wine-prefix', default='', help='run the Windows .exe under Wine in this fresh prefix')
    ap.add_argument('--wine', default='')
    a = ap.parse_args()
    if not a.data_dir or not os.path.isdir(a.data_dir) or not a.game_exe or not os.path.isfile(a.game_exe):
        print('enhancements-frames: no installed game given; skipped')
        return 77
    prefix = None
    if a.wine_prefix:
        wine = winehost.find_wine(a.wine)
        if not wine:
            print('enhancements-frames: Windows build and no Wine (set RA2_NATIVE_WINE); skipped')
            return 77
        prefix = winehost.Prefix(wine, a.wine_prefix)
    os.makedirs(a.work, exist_ok=True)
    results = {}
    try:
        for tag, enh in (('default', ''), ('original', 'preset=original'), ('enhanced', 'preset=enhanced')):
            results[tag] = run(a, tag, enh, prefix)
    finally:
        if prefix:
            prefix.close()
    bad = []
    for tag, want in (('default', 'vsync off, scaling sharp'), ('original', 'vsync off, scaling sharp'),
                      ('enhanced', 'vsync on, scaling smooth')):
        dumps, settings, settled = results[tag]
        if not settled:
            bad.append('%s: the menu never settled (no menu-settled milestone; see %s)' % (tag, os.path.join(a.work, tag, 'run.log')))
        if len(dumps) < a.compare:
            bad.append('%s: %d frames dumped, %d needed (see %s)' % (tag, len(dumps), a.compare, os.path.join(a.work, tag, 'run.log')))
        if want not in settings:
            bad.append('%s: settings line %r, want %r' % (tag, settings, want))
    animated = set()
    if not bad:
        frames = {tag: [pixels(p) for p in results[tag][0][-a.compare:]] for tag in results}
        w, h, bpp, _ = frames['default'][0]
        if any(f[:3] != (w, h, bpp) for tag in frames for f in frames[tag]):
            bad.append('the runs dumped windows of different sizes')
        else:
            d = frames['default']
            for i in range(len(d)):
                for j in range(i + 1, len(d)):
                    animated |= differing(d[i][3], d[j][3], bpp)
            # Each animation runs through more states than a few frames show: the 16x16 tiles
            # holding an animated pixel, and their neighbours, are left out as a whole.
            tiles = {(x // 16 + dx, y // 16 + dy) for x, y in animated for dx in (-1, 0, 1) for dy in (-1, 0, 1)}
            animated = {(x, y) for x in range(w) for y in range(h) if (x // 16, y // 16) in tiles}
            if len(animated) > 0.03 * w * h:
                bad.append('%d animated pixels (over 3%% of %dx%d): too little left to compare' % (len(animated), w, h))
            for k, f in enumerate(frames['original']):
                extra = differing(d[k][3], f[3], bpp) - animated
                if extra:
                    xs = [p[0] for p in extra]; ys = [p[1] for p in extra]
                    bad.append('ORIGINAL preset frame %d differs from the defaults in %d still pixels (x %d-%d, y %d-%d)' %
                               (k, len(extra), min(xs), max(xs), min(ys), max(ys)))
                    break
            if not any(differing(d[k][3], f[3], bpp) - animated for k, f in enumerate(frames['enhanced'])):
                bad.append('ENHANCED preset equals the defaults outside the animated pixels: the comparison cannot see the scaling')
    for b in bad:
        print('enhancements-frames: ' + b)
    print('enhancements-frames: %s (%d settled frames per run, %d animated pixels left out)' % (
        'ok' if not bad else 'FAILED', a.compare, len(animated)))
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main())
