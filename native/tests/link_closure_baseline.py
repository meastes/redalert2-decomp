#!/usr/bin/env python3
"""Link-closure baseline guard (C3, after the 2026-11-12 seed-free regression).

The link closure (tools/link_closure.py, gen/link_closure.json) lists every symbol
the ra2-native inputs reference that nothing defines; each one becomes a trap stub
or a zero placeholder. A source change that respells a declaration (the ghidra_types
replacement turned 74 seed frees into traps: 290 -> 364) passes the build and the
other tests silently. This check fails when the undefined set grows past the
recorded baseline (native/data/link_closure_baseline.json: the total, the per-kind
counts and the symbols), naming every new symbol.

  link_closure_baseline.py --build <native build dir>          check (ctest native-link-closure)
  link_closure_baseline.py --build <native build dir> --write  record the current set as the baseline
  link_closure_baseline.py --self-test                         the check fails on a grown set and passes otherwise
  link_closure_baseline.py --build <win build dir> --target windows   the Windows cross build against the same baseline

The Windows cross build (README "Windows build") names its symbols in the Mach-O spelling,
but a parameter the sources declare as intptr_t/uintptr_t (an LPARAM, a WPARAM) mangles as
long long there (Win64 is LLP64) and as long on the Mac: --target windows compares the
demangled names with "long long" read as "long", so the same set passes on both.

A shrinking set passes and says so; record it with --write in the same commit that
removes the symbols, so the baseline only ever moves down.
"""
import argparse
import json
import sys
import tempfile
from pathlib import Path

BASELINE = Path(__file__).resolve().parent.parent / 'data' / 'link_closure_baseline.json'


def load_report(build):
    path = Path(build) / 'gen' / 'link_closure.json'
    if not path.exists():
        print('link-closure baseline: %s missing (configure with RA2_NATIVE_LINK_CLOSURE=ON and build)' % path)
        sys.exit(77)
    return json.loads(path.read_text())


def symbol_key(s):
    return s['symbol']


def windows_key(s):
    """A Windows symbol and its Mac namesake: Win64's long long is the Mac's 64-bit long."""
    return s['name'].replace('long long', 'long')


def compare(report, baseline, key=symbol_key):
    """Returns (ok, lines)."""
    now = {key(s): s for s in report['symbols']}
    was = {key(s): s for s in baseline['symbols']}
    new = sorted(set(now) - set(was))
    gone = sorted(set(was) - set(now))
    lines = []
    ok = True
    if report['total'] > baseline['total']:
        ok = False
        lines.append('undefined total %d > baseline %d' % (report['total'], baseline['total']))
    for kind in sorted(set(report['by_class']) | set(baseline['by_class'])):
        a, b = report['by_class'].get(kind, 0), baseline['by_class'].get(kind, 0)
        if a > b:
            ok = False
            lines.append('%s: %d > baseline %d' % (kind, a, b))
    if new:
        ok = False
        lines.append('%d new undefined symbol(s) (each is a trap or a zero placeholder now):' % len(new))
        for s in new:
            lines.append('  + %s  [%s]  %s' % (now[s]['name'], now[s]['cls'], now[s]['symbol']))
    if gone:
        lines.append('%d symbol(s) no longer undefined; lower the baseline with --write:' % len(gone))
        for s in gone[:40]:
            lines.append('  - %s' % was[s]['name'])
        if len(gone) > 40:
            lines.append('  ...')
    return ok, lines


def baseline_of(report):
    return {
        'about': 'Undefined symbols of the Mac ra2-native link closure (RA2_NATIVE_SUBSYSTEM=all, drafts and link '
                 'closure on, WIN64_PARAMS on) that tests/link_closure_baseline.py allows. Names only (no bytes). '
                 'Regenerate with tests/link_closure_baseline.py --build <dir> --write when the set shrinks.',
        'total': report['total'],
        'by_class': dict(sorted(report['by_class'].items())),
        'symbols': sorted(({'symbol': s['symbol'], 'name': s['name'], 'cls': s['cls']} for s in report['symbols']),
                          key=lambda s: s['symbol']),
    }


def self_test():
    base = {'total': 2, 'by_class': {'c-function': 1, 'cxx-function': 1},
            'symbols': [{'symbol': '_a', 'name': 'a', 'cls': 'c-function'},
                        {'symbol': '__Z1bv', 'name': 'b()', 'cls': 'cxx-function'}]}
    same = dict(base)
    grown = {'total': 3, 'by_class': {'c-function': 1, 'cxx-function': 2},
             'symbols': base['symbols'] + [{'symbol': '__Z1cj', 'name': 'c(unsigned int)', 'cls': 'cxx-function'}]}
    swapped = {'total': 2, 'by_class': {'c-function': 1, 'cxx-function': 1},
               'symbols': [base['symbols'][0], {'symbol': '__Z1cj', 'name': 'c(unsigned int)', 'cls': 'cxx-function'}]}
    shrunk = {'total': 1, 'by_class': {'c-function': 1}, 'symbols': base['symbols'][:1]}
    # The Windows spelling of b(long) (Win64: long long) is the same symbol; c() is new.
    base_l = {'total': 1, 'by_class': {'cxx-function': 1},
              'symbols': [{'symbol': '__Z1bl', 'name': 'b(long)', 'cls': 'cxx-function'}]}
    win_same = {'total': 1, 'by_class': {'cxx-function': 1},
                'symbols': [{'symbol': '__Z1bx', 'name': 'b(long long)', 'cls': 'cxx-function'}]}
    win_swapped = {'total': 1, 'by_class': {'cxx-function': 1},
                   'symbols': [{'symbol': '__Z1cx', 'name': 'c(unsigned int)', 'cls': 'cxx-function'}]}
    checks = [('same', same, True), ('grown', grown, False), ('swapped', swapped, False), ('shrunk', shrunk, True)]
    bad = 0
    for name, report, want, b, key in [c + (base, symbol_key) for c in checks] + [
            ('windows same', win_same, True, base_l, windows_key),
            ('windows swapped', win_swapped, False, base_l, windows_key),
            ('windows spelling without --target', win_same, False, base_l, symbol_key)]:
        ok, lines = compare(report, b, key)
        named = name == 'windows spelling without --target' or any('c(unsigned int)' in l for l in lines)
        if ok != want or (not want and not named):
            bad += 1
            print('self-test %s: ok=%s want %s; %s' % (name, ok, want, lines))
    with tempfile.TemporaryDirectory() as tmp:
        (Path(tmp) / 'gen').mkdir()
        (Path(tmp) / 'gen' / 'link_closure.json').write_text(json.dumps(grown))
        if compare(load_report(tmp), base)[0]:
            bad += 1
            print('self-test file round trip: grown set passed')
    print('link-closure baseline self-test: %s' % ('ok' if not bad else '%d failure(s)' % bad))
    return 1 if bad else 0


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--build', help='native build directory (holds gen/link_closure.json)')
    ap.add_argument('--baseline', default=str(BASELINE))
    ap.add_argument('--write', action='store_true', help='record the current set as the baseline')
    ap.add_argument('--self-test', action='store_true')
    ap.add_argument('--target', choices=('macos', 'windows'), default='macos',
                    help='windows: the cross build\'s set, long long read as long (see above)')
    a = ap.parse_args()
    if a.self_test:
        return self_test()
    if not a.build:
        ap.error('--build is required')
    report = load_report(a.build)
    if a.write:
        Path(a.baseline).write_text(json.dumps(baseline_of(report), indent=1, ensure_ascii=False) + '\n')
        print('link-closure baseline written: %d undefined %s' % (report['total'], report['by_class']))
        return 0
    baseline = json.loads(Path(a.baseline).read_text())
    ok, lines = compare(report, baseline, windows_key if a.target == 'windows' else symbol_key)
    print('link-closure baseline: %d undefined (baseline %d) %s' % (report['total'], baseline['total'],
                                                                   'ok' if ok else 'GREW'))
    for l in lines:
        print(l)
    return 0 if ok else 1


if __name__ == '__main__':
    sys.exit(main())
