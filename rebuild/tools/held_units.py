#!/usr/bin/env python3
"""held_units.py [--write]: rank the units held out of the catalog by the rows that would enter if their
blockers fell, and list every blocker per unit (lane R49, 2026-11-20).

The catalog (rebuild_catalog.write) stops at a unit's first blocker; this runs every one of its tests on
the unit's last check (build/r/last/<unit>.json, `rebuild.py check UNIT` first) without stopping:
own rows not exact (without partial_ok), initializers not exact rows, compiler-output rows (atexit
destructors) that differ, unguarded drafts, emitted
functions or data with no row or binding, external functions over the omitted budget, structure faults,
and a whole-file build refusal recorded for the current text. A stale or missing check is a blocker of
its own. Worker-held units (workers.csv) are listed, ranked last. Prints the table; --write writes
rebuild/manifest/held_units.csv.
"""
import csv
import json
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import rebuild as R  # noqa: E402
import rebuild_catalog as C  # noqa: E402


def blockers(unit, info):
    out = []
    last = R.load_json(R.WORK / 'last' / (unit + '.json'), None)
    source = R.ROOT / info['source']
    text = source.read_text(errors='replace')
    if last is None or last['source_sha256'] != C.sha(source.read_bytes()):
        return None, ['stale: no check of the current text']
    held = info.get('build_refused')
    if held and held.get('source_sha256') == last['source_sha256']:
        out.append('build: %s' % held['reason'][:120])
    results = {r['entry']: r for r in last['results']}
    exact = [r for r in info['rows'] if results.get(r['entry'], {}).get('status') == 'exact']
    core = info.get('core')
    inside = [r for r in info['rows'] if core[0] <= r['entry'] < core[1]] if core else info['rows']
    missed = [r for r in inside if results.get(r['entry'], {}).get('status') != 'exact']
    if missed and not info.get('partial_ok'):
        out.append('rows: %d of %d own rows not exact (%s)' % (
            len(missed), len(inside), ' '.join(results.get(r['entry'], {}).get('symbol') or r['symbol'] or r['entry']
                                               for r in missed)[:200]))
    from coff import Coff
    coff = Coff(Path(last['object']).read_bytes())
    exact_names = {r['symbol'] for r in exact}
    registered = {coff.symbols[x]['name'] for s in coff.sections if s['name'].startswith('.CRT$XC')
                  for o, x, k in s['relocs']}
    if registered - exact_names:
        out.append('initializers: %s' % ' '.join(sorted(registered - exact_names)))
    # Compiler-output rows (an atexit destructor is not in .CRT$XCU) that the object emits but that differ:
    # they cannot be guarded, and the whole-file build refuses their section as unaccounted code.
    emitted = [r for r in info['rows'] if results.get(r['entry'], {}).get('status') == 'differs'
               and r['symbol'].startswith('_$') and r['symbol'] not in registered]
    if emitted:
        out.append('compiler output: %s' % ' '.join('%s %s (%s B differ)' % (r['entry'], r['symbol'],
                   results[r['entry']].get('differing')) for r in emitted))
    resolve_u = R.resolver(unit, info)
    loose = [s['name'] for s in coff._functions if s['name'] not in exact_names and
             not s['name'].startswith(('??_G', '??_E')) and resolve_u(s['name'])[0] is None]
    unplaced = {n for n, k in (info.get('data_bound_by') or {}).items() if k in ('unplaced-unreferenced', 'layout', 'content')}
    for i, s in enumerate(coff.sections, 1):
        if s['flags'] & 0xc0 and not s['flags'] & 0x1020 and not s['name'].startswith(('.CRT', '.debug', '.drectve')):
            loose += [n['name'] for n in coff.symbols.values() if n['section'] == i and n['storage'] in (2, 3)
                      and not n['name'].startswith(('.', '$')) and resolve_u(n['name'])[0] is None
                      and n['name'] not in unplaced]
    loose += [n['name'] for n in coff.symbols.values() if n['name'].startswith('__real@')
              and n['section'] > 0 and resolve_u(n['name'])[0] is None and n['name'] not in
              {b['symbol'] for r in exact for b in results[r['entry']].get('relocations', [])}]
    import guard

    def defined_here(sym):
        name = guard.demangle(sym)
        return bool(name) and bool(re.search(r'^(?![ \t#/])[^\n;{}]*?(?<![\w:])%s\s*\(' % re.escape(name), text, re.M))
    own_rows = {int(r['entry'], 16) for r in info['rows']}
    exact_entries = {int(r['entry'], 16) for r in exact}
    drafts = [s['name'] for s in coff._functions if s['name'] not in exact_names and
              not s['name'].startswith(('_$', '??_')) and '?$' not in s['name'] and
              resolve_u(s['name'])[0] in own_rows - exact_entries and defined_here(s['name'])]
    if drafts:
        out.append('drafts: %s' % ' '.join(drafts))
    if loose:
        out.append('unbound: %s' % ' '.join(loose))
    budget = json.loads((R.MANIFEST / 'structure_budget.json').read_text())
    claimed = exact_names | {results[r['entry']].get('symbol') for r in exact}
    omitted = [n for n in C.external_omitted(coff, claimed, resolve_u) if resolve_u(n)[0] not in exact_entries]
    if len(omitted) > budget.get('external_omitted_by_unit', {}).get(unit, 0):
        out.append('omitted: %s' % ' '.join(omitted))
    import rebuild_census
    faults = rebuild_census.unit_faults(unit, text)
    if faults:
        out.append('structure: %s' % '; '.join(faults))
    if info.get('count_sensitive'):
        out.append('count-sensitive: %s' % json.dumps(info['count_sensitive'])[:120])
    return exact, out


def main(argv):
    units = R.load_json(R.UNITS, {})
    workers = {r['unit']: r['worker'] for r in csv.DictReader(open(R.MANIFEST / 'workers.csv'))}
    rows = []
    for unit, info in sorted(units.items()):
        if info['status'] != 'pending':
            continue
        exact, bl = blockers(unit, info)
        rows.append(dict(unit=unit, worker=info.get('holder') or workers.get(unit, ''), rows=len(info['rows']),
                         exact_rows=len(exact) if exact is not None else '',
                         exact_bytes=sum(r['body'] for r in exact) if exact else '',
                         blocker_count=len(bl), blockers=' | '.join(bl), held_reason=info.get('held_reason', '')))
    rows.sort(key=lambda r: (bool(r['worker']), r['blocker_count'], -(r['exact_rows'] or 0)))
    for r in rows:
        print('%-10s %-7s %3s/%-3s %7s B  %d: %s' % (r['unit'], r['worker'] or '-', r['exact_rows'], r['rows'],
                                                   r['exact_bytes'], r['blocker_count'], r['blockers'][:300]))
    if '--write' in argv:
        with open(R.MANIFEST / 'held_units.csv', 'w', newline='') as f:
            w = csv.DictWriter(f, fieldnames=list(rows[0]))
            w.writeheader()
            w.writerows(rows)
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
