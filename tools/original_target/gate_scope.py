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
"""
import argparse
import json
from pathlib import Path
import subprocess

REPOSITORY = Path(__file__).resolve().parents[2]

# Paths the native tree alone reads; anything else that is not documentation counts as exact.
NATIVE_PREFIXES = ('native/', 'historical/native-reconstruction/', 'tests/test_native', 'build-ios.command',
                   'play-native.command')
DOC_PREFIXES = ('docs/', 'README', 'THIRD_PARTY.md', 'AGENTS.md', 'CLAUDE.md', '.gitignore')


def classify(path):
    """'native', 'doc' or 'exact' for one repository path."""
    if path.startswith(NATIVE_PREFIXES):
        return 'native'
    if path.startswith(DOC_PREFIXES) or (path.endswith('.md') and '/' not in path):
        return 'doc'
    return 'exact'


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
    args = parser.parse_args()
    paths = changed_paths(args.base)
    print(json.dumps(dict(scope=scope(paths), paths=len(paths),
                          exact_paths=[p for p in paths if classify(p) == 'exact'][:20]), indent=1))


if __name__ == '__main__':
    import sys
    sys.path.insert(0, str(Path(__file__).resolve().parent))
    main()
