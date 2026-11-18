#!/usr/bin/env python3
"""held.py CATALOG_LOG [--write] [--note UNIT=KIND:TEXT ...]: refresh held_reason and held_note on
every pending unit of units.json from a catalog run's "left out:" lines, and print the breakdown.

Kinds: worker (a Windows worker holds the unit), header (a header emits initializers or statics the
original lacks), tooling (a tool cannot yet audit the unit), mismatch (own rows not exact), binding
(an emitted function, initializer or data section is not bound to a row), owner (anything else).
Worker and the catalog-derived kinds are recomputed every run; a header or tooling note is written by
hand (--note) and kept until the unit lands or a new note replaces it, because the catalog line alone
does not say why a header is wrong. Units that are no longer pending lose both fields.
"""
import collections
import csv
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import rebuild as R  # noqa: E402

MANUAL = {'header', 'tooling'}
# codex2's backlog units that a landing helper is moving into the tree by source; workers.csv does not
# list them yet. Drop a name once its unit lands.
IN_TRANSIT = {u: 'codex2' for u in ('tab scroll strtable mouse ion ebolt mainopt options ldprgmgr session '
                                    'netshare scenario saveload revent startup beacon').split()}


def classify(line):
    if 'unguarded drafts' in line or 'Unaccounted emitted code' in line:
        return 'tooling'
    if 'own rows not exact' in line or 'no exact row' in line:
        return 'mismatch'
    if any(s in line for s in ('initializers not exact', 'Unsupported allocatable data',
                               'no row or binding', 'closed compiler-output')):
        return 'binding'
    return 'owner'


def main(argv):
    if not argv or argv[0].startswith('-'):
        print(__doc__.strip())
        return 2
    write = '--write' in argv
    notes = {}
    for i, a in enumerate(argv):
        if a == '--note':
            unit, rest = argv[i + 1].split('=', 1)
            kind, text = rest.split(':', 1)
            notes[unit] = (kind, text.strip())
    reasons = collections.defaultdict(list)
    for line in open(argv[0]):
        m = re.match(r'\s*left out: (\w+): (.*)', line)
        if m:
            reasons[m.group(1)].append(m.group(2).strip())
    units = R.load_json(R.UNITS, {})
    workers = {r['unit']: r['worker'] for r in csv.DictReader(open(R.MANIFEST / 'workers.csv'))}
    out = collections.defaultdict(lambda: [0, 0, []])
    for u, info in units.items():
        if info['status'] != 'pending':
            info.pop('held_reason', None)
            info.pop('held_note', None)
            continue
        r0 = (reasons.get(u) or ['no catalog line'])[0]
        holder = info.get('holder') or workers.get(u) or IN_TRANSIT.get(u)
        if u in notes:
            kind, note = notes[u]
        elif holder:
            kind, note = 'worker', '%s; catalog: %s' % (holder, r0[:120])
        elif info.get('held_reason') in MANUAL and info.get('held_note'):
            kind, note = info['held_reason'], info['held_note']
        else:
            kind, note = classify(r0), r0[:160]
        info['held_reason'] = kind
        info['held_note'] = note
        core = info.get('core')
        b = sum(r['body'] for r in info['rows'] if not core or core[0] <= r['entry'] < core[1])
        o = out[kind]
        o[0] += 1
        o[1] += b
        o[2].append(u)
    if write:
        R.write_json(R.UNITS, units)
    for k, (n, b, us) in sorted(out.items()):
        print('%-9s %3d units %9d bytes: %s' % (k, n, b, ' '.join(sorted(us))))
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
