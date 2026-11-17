#!/usr/bin/env python3
"""Which gates a change needs (lane P1, 2026-11-15).

    gate_scope.py [--base REV]      print {"scope": ..., "paths": N, "exact_paths": [...]} for REV..HEAD
                                    plus the working tree (default REV: main)

* "exact": the change touches what the whole-file exact build reads (src/, manifests/, the
  build tools, build.command, the toolchain pins): the whole-file exact gate, then native
  configure, compile and ctest (the native tree is generated from src/).
* "native": every changed path is native-only (native/, native tests, docs): native configure,
  compile and ctest only. It takes neither the full-build lock nor the whole-file gate; it rests
  on the last exact whole-file receipt for an ancestor of HEAD, which the native gate records.
* "none": documentation only.

Which native tests (lane P2, 2026-11-16; the ctest labels of native/CMakeLists.txt):

* tier "fast": the static checks and the fast played checks, plus each slow test whose area the
  change touches (AREAS: its paths, and its own test script);
* tier "full": every test. Chosen when asked (--tier full: once per landing batch, and before any
  build for the user's devices), and by "auto" when the native gate history
  (native_gate.HISTORY) holds no full pass from the last FULL_TIER_HOURS hours.

    gate_scope.py [--base REV] --tests      also print the tier and the slow tests it adds
"""
import argparse
import json
from pathlib import Path
import subprocess

REPOSITORY = Path(__file__).resolve().parents[2]

# Paths the native tree alone reads; anything else that is not documentation counts as exact.
NATIVE_PREFIXES = ('native/', 'tests/test_native', 'build-ios.command', 'play-native.command')
DOC_PREFIXES = ('docs/', 'README', 'THIRD_PARTY.md', 'AGENTS.md', 'CLAUDE.md', '.gitignore')


def classify(path):
    """'native', 'doc' or 'exact' for one repository path."""
    if path.startswith(NATIVE_PREFIXES):
        return 'native'
    if path.startswith(DOC_PREFIXES) or (path.endswith('.md') and '/' not in path):
        return 'doc'
    return 'exact'


# Slow-test areas: a changed path matching one of these (case-insensitive, anywhere in the path)
# adds that area's slow tests to a fast run. Kept broad: a false match costs one slow test.
AREAS = {
    'network': r'(?:\blan|lan_|_lan|network|netw|ipx|udp|wsock|winsock|wol|session|lobby|nullmodem|connect|packet|/event\.)',
    'campaign': r'(?:campaign|scenario|mission|score|briefing|movie|vqa|ending)',
    'saves': r'(?:save|load|swizzle|stream|persist|pipe|straw)',
    'hd-aircraft': r'(?:hd_vehicles|hd-vehicles|hdvox|voxel|vox|aircraft|air_base|paradrop|plane)',
    'widescreen': r'(?:widescreen|aspect|resolution|native_map_zoom|window|display)',
}
# Each slow test's own script (a change to it runs it).
SLOW_SCRIPTS = {
    'native/tests/lan_pair.py': 'network', 'native/tests/lan_lobby.py': 'network',
    'native/tests/campaign_chain.py': 'campaign', 'native/tests/original_save_write.py': 'saves',
    'native/tests/hd_vehicles.py': 'hd-aircraft', 'native/tests/widescreen.py': 'widescreen',
}
FULL_TIER_HOURS = 12.0


def areas(paths):
    """The slow-test areas the changed paths touch."""
    import re
    found = set()
    for p in paths:
        if p in SLOW_SCRIPTS:
            found.add(SLOW_SCRIPTS[p])
        if classify(p) == 'doc':
            continue
        for area, pattern in AREAS.items():
            if re.search(pattern, p, re.I):
                found.add(area)
    return sorted(found)


def tier(paths, requested='auto', history=None, now=None):
    """('fast' | 'full', [areas], reason)."""
    import time
    if requested == 'full':
        return 'full', [], 'asked'
    touched = areas(paths)
    if requested == 'auto' and history is not None:
        now = now or time.time()
        fulls = [row for row in history if row.get('tier') == 'full' and row.get('ok')]
        if not fulls or now - max(r.get('time', 0) for r in fulls) > FULL_TIER_HOURS * 3600:
            return 'full', touched, 'no full pass in the last %g hours' % FULL_TIER_HOURS
    return 'fast', touched, 'fast tier' + (' with areas ' + ', '.join(touched) if touched else '')


def scope(paths):
    kinds = {classify(p) for p in paths}
    if 'exact' in kinds:
        return 'exact'
    if 'native' in kinds:
        return 'native'
    return 'none'


def changed_paths(base, repository=REPOSITORY):
    def lines(*args):
        out = subprocess.run(['git', '-C', str(repository), *args], capture_output=True, text=True, check=True).stdout
        return [l for l in out.splitlines() if l]
    paths = set(lines('diff', '--name-only', '%s...HEAD' % base))
    paths.update(lines('diff', '--name-only', 'HEAD'))
    paths.update(lines('ls-files', '--others', '--exclude-standard'))
    return sorted(paths)


def resting_receipt(repository=REPOSITORY):
    """The newest exact, clean whole-file receipt whose commit is an ancestor of HEAD (or None)."""
    import whole_file_receipt
    try:
        _, _, main = whole_file_receipt.checkout_state(repository)
        text = (Path(main) / whole_file_receipt.HISTORY).read_text()
    except Exception:
        return None
    rows = []
    for line in text.splitlines():
        try:
            rows.append(json.loads(line))
        except ValueError:
            continue
    for row in reversed(rows):
        if not row.get('whole_file_exact') or row.get('dirty') or not row.get('commit'):
            continue
        if subprocess.run(['git', '-C', str(repository), 'merge-base', '--is-ancestor', row['commit'], 'HEAD'],
                          capture_output=True).returncode == 0:
            return dict(commit=row['commit'], time_utc=row.get('time_utc'), gate=row.get('gate'))
    return None


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('--base', default='main')
    parser.add_argument('--tests', action='store_true', help='also print the native test tier and areas')
    args = parser.parse_args()
    paths = changed_paths(args.base)
    out = dict(scope=scope(paths), paths=len(paths), exact_paths=[p for p in paths if classify(p) == 'exact'][:20])
    if args.tests:
        import native_gate
        chosen, touched, reason = tier(paths, 'auto', native_gate.read_history())
        out.update(tier=chosen, areas=touched, reason=reason)
    print(json.dumps(out, indent=1))


if __name__ == '__main__':
    import sys
    sys.path.insert(0, str(Path(__file__).resolve().parent))
    main()
