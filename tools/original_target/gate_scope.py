#!/usr/bin/env python3
"""Which gates a change needs (lane P1, 2026-11-15; one tree since the layout move, lane O88, 2026-11-25).

    gate_scope.py [--base REV]      print {"scope": ..., "paths": N, "exact_paths": [...]} for REV..HEAD
                                    plus the working tree (default REV: main)

* "exact": the change touches what the gate reads (src/, include/, asm/, manifests/, tools/, tests/,
  build.command, test.command, the toolchain pins): the tree's gate (catalog, structure gate,
  whole-file exact build; private/tools/land_rebuild_lane.sh).
* "none": documentation only.
* "frozen": the change touches manifests/original_target/, the earlier tree's catalog, kept read-only
  as the evidence the catalog rows cite (bounds, hashes, bound_population; the tree itself is the tag
  src-final). Refused: nothing lands there.

The native ports moved to github.com/meastes/redalert2-modern (user decision 2026-11-24/25), so there
is no native scope or native test tier here any more.
"""
import argparse
import json
from pathlib import Path
import subprocess

REPOSITORY = Path(__file__).resolve().parents[2]

DOC_PREFIXES = ('docs/', 'README', 'THIRD_PARTY.md', 'AGENTS.md', 'CLAUDE.md', '.gitignore', 'LICENSE')
# The earlier tree's catalog, frozen at the tag src-final: read-only evidence.
FROZEN_PREFIXES = ('manifests/original_target/',)


def classify(path):
    """'frozen', 'doc' or 'exact' for one repository path."""
    if path.startswith(FROZEN_PREFIXES):
        return 'frozen'
    if path.startswith(DOC_PREFIXES) or (path.endswith('.md') and '/' not in path):
        return 'doc'
    return 'exact'


def scope(paths):
    kinds = {classify(p) for p in paths}
    if 'frozen' in kinds:
        return 'frozen'
    if 'exact' in kinds:
        return 'exact'
    return 'none'


def rebuild_gate(paths):
    """Whether the change needs the gate (kept for the landing scripts: true for any non-doc change)."""
    return scope(paths) == 'exact'


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
    out = dict(scope=scope(paths), rebuild_gate=rebuild_gate(paths), paths=len(paths),
               exact_paths=[p for p in paths if classify(p) == 'exact'][:20],
               frozen_paths=[p for p in paths if classify(p) == 'frozen'][:20])
    print(json.dumps(out, indent=1))


if __name__ == '__main__':
    import sys
    sys.path.insert(0, str(Path(__file__).resolve().parent))
    main()
