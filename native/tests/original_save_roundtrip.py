#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest native-original-save-roundtrip (lane C17, stage E): each of the user's original saves
(read in place, never copied) is loaded and at once saved again in the original's format
(RA2_NATIVE_SAVE_AFTER_LOAD, before any frame runs), with the typed reads and writes indexed
(RA2_NATIVE_SAVE_INDEX); tools/original_save_compare.py then compares the decoded streams
and reports the differing bytes as a count and a list by class and member (written to
--work/<save>.txt and .json, and summed in --work/summary.txt).

The test fails only on what the installed game cannot take: a vtable pointer that differs
from the original's, or indexes that part and never meet again (the writer and the reader
disagree about the stream). Member, pointer, padding and state differences are reported,
not judged (naming each is the deferred stage F). Skipped (77) without the installed game or
the member list, or for the Windows build without Wine.

  original_save_roundtrip.py --binary <ra2-native> --data-dir <game folder> --game-exe <game.exe>
                             --members <gen/original_layouts_members.json> --work <dir> --save <name> ...
                             [--wine-prefix <dir> [--wine <wine>]]
"""
import argparse
import json
import os
import shutil
import subprocess
import sys

import game_process
import wine as winehost

HERE = os.path.dirname(os.path.abspath(__file__))
COMPARE = os.path.join(HERE, '..', 'tools', 'original_save_compare.py')
SKIP = 77


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--game-exe', default='')
    ap.add_argument('--members', default='')
    ap.add_argument('--work', required=True)
    ap.add_argument('--save', action='append', default=[])
    ap.add_argument('--wine-prefix', default='')
    ap.add_argument('--wine', default='')
    a = ap.parse_args()
    if not a.data_dir or not os.path.isdir(a.data_dir) or not a.game_exe or not os.path.isfile(a.game_exe):
        print('original-save-roundtrip: skipped (set RA2_NATIVE_CENSUS_DATA_DIR and RA2_NATIVE_CENSUS_GAME_EXE)')
        return SKIP
    if not os.path.isfile(a.members):
        print('original-save-roundtrip: skipped (no %s)' % a.members)
        return SKIP
    prefix = None
    if a.wine_prefix:
        wine = winehost.find_wine(a.wine)
        if not wine:
            print('original-save-roundtrip: skipped (Windows build: no Wine; set RA2_NATIVE_WINE)')
            return SKIP
        prefix = winehost.Prefix(wine, a.wine_prefix)
    work = os.path.abspath(a.work)
    shutil.rmtree(work, ignore_errors=True)
    write = os.path.join(work, 'write')
    os.makedirs(write)
    bad, summary = [], []
    try:
        for source in a.save:
            if not os.path.isfile(os.path.join(a.data_dir, source)):
                summary.append('%s: not in the data folder, skipped' % source)
                continue
            stem = os.path.splitext(source)[0].upper()
            name, index = 'E_%s.SAV' % stem, os.path.join(work, stem + '.idx')
            env = dict(os.environ, RA2_GAME_EXE=a.game_exe, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_IGNORE_FOCUS='1',
                       RA2_NATIVE_SAVE_FORMAT='original', RA2_NATIVE_SAVE_AFTER_LOAD=name, RA2_NATIVE_SAVE_INDEX=index)
            argv = [a.binary, '--data-dir', a.data_dir, '--write-dir=' + write, '--game-args=-noaudio -CD.',
                    '--run-to=frames', '--frames=2', '--run-seconds=240', '--load-save=' + source]
            out, code = '', -1
            for attempt in range(winehost.HOST_RETRIES + 1):
                cmd, e = (prefix.command(argv), prefix.env(env)) if prefix else (argv, env)
                try:
                    p = game_process.run(cmd, env=e, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True,
                                         errors='replace', timeout=300)
                    out, code, timed_out = p.stdout, p.returncode, False
                except subprocess.TimeoutExpired as x:
                    out = x.stdout.decode('utf-8', 'replace') if isinstance(x.stdout, bytes) else (x.stdout or '')
                    code, timed_out = -1, True
                why = winehost.host_failure(out, timed_out) if prefix else ''
                if prefix and not why and code != 0 and 'run-to: reached main' not in out:
                    why = 'Wine ended the launch (exit %d) before the program started' % code
                if not why or attempt == winehost.HOST_RETRIES:
                    break
            open(os.path.join(work, stem + '.log'), 'w').write(out)
            if 'resume: saved %s after the load' % name not in out:
                bad.append('%s: not loaded and saved again (exit %d; see %s.log)' % (source, code, stem))
                continue
            report = os.path.join(work, stem + '.json')
            c = subprocess.run([sys.executable, '-B', COMPARE, '--source', os.path.join(a.data_dir, source),
                                '--written', os.path.join(write, name), '--index', index, '--members', a.members,
                                '--json', report, '--limit', '60'], stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                               text=True, errors='replace')
            open(os.path.join(work, stem + '.txt'), 'w').write(c.stdout)
            if c.returncode != 0:
                bad.append('%s: the comparison failed (see %s.txt)' % (source, stem))
                continue
            r = json.load(open(report))
            cls = r['by_class']
            summary.append('%s: %d entries paired; differing bytes: %s' % (
                source, r['paired'], ', '.join('%s %d' % (k, cls.get(k, 0)) for k in
                                               ('member', 'pointer', 'vptr', 'padding', 'untyped', 'state', 'summary'))))
            if cls.get('vptr', 0):
                bad.append('%s: %d vtable pointer bytes differ from the original\'s' % (source, cls['vptr']))
            for n in r['notes']:
                if 'do not meet again' in n:
                    bad.append('%s: %s' % (source, n))
    finally:
        if prefix:
            prefix.close()
    open(os.path.join(work, 'summary.txt'), 'w').write('\n'.join(summary) + '\n')
    for s in summary:
        print('original-save-roundtrip: ' + s)
    for b in bad:
        print('original-save-roundtrip: FAILED: ' + b)
    print('original-save-roundtrip: %s (see %s)' % ('ok' if not bad else 'FAILED', work))
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main())
