"""nodraft_queue.py: write rebuild/manifest/no_draft_queue.csv, the workers' queue for functions with no text in
the rebuild tree (lane R43, 2026-11-19).

Two groups, per unit (address order inside a unit):

  main_exact   a row of units.json whose bucket in tail_buckets.csv is no_draft: main has an accepted exact
               body for it (main_source, main_symbol from main's catalog), so the body can be reshaped
               (provenance class b) into the unit's text
  no_main_row  a function mischa85's names.csv places inside the unit's range that is neither a row of the
               unit nor inside a row's extent: main has no accepted row either, so it has no independent
               bound here and cannot be accepted until main (or a frozen pass) bounds it

Columns: group, unit, held_by (the worker holding the unit in workers.csv, else empty), entry, bytes (body bytes
for main_exact; empty for no_main_row, whose extent is unknown), symbol, main_source, main_symbol, evidence.
Reads tail_buckets.csv as tailmap.py last wrote it; run tailmap.py --write first for a fresh queue."""
import csv

import rebuild as R

OUT = R.MANIFEST / 'no_draft_queue.csv'


def main():
    units = R.load_json(R.UNITS, {})
    workers = {r['unit']: r['worker'] for r in csv.DictReader(open(R.MANIFEST / 'workers.csv'))}
    nodraft = [r for r in csv.DictReader(open(R.MANIFEST / 'tail_buckets.csv')) if r['bucket'] == 'no_draft']
    out = []
    for r in nodraft:
        row = next(x for x in units[r['unit']]['rows'] if x['entry'] == r['entry'])
        out.append(dict(group='main_exact', unit=r['unit'], held_by=workers.get(r['unit'], ''), entry=r['entry'],
                        bytes=row['body'], symbol=row['symbol'], main_source=row.get('main_source', ''),
                        main_symbol=row.get('main_symbol', ''), evidence='main catalog row'))
    names = sorted((a, s) for s, a in R.mischa_names().items())
    for unit, info in sorted(units.items()):
        lo, hi = (int(x, 16) for x in info['range'])
        covered = [(int(x['entry'], 16), int(x['entry'], 16) + x['body'] + x.get('gap_padding', x['padding']))
                   for x in info['rows']]
        for address, symbol in names:
            if not lo <= address < hi or any(s <= address < e for s, e in covered):
                continue
            out.append(dict(group='no_main_row', unit=unit, held_by=workers.get(unit, ''), entry='%08x' % address,
                            bytes='', symbol=symbol, main_source='', main_symbol='',
                            evidence='mischa85 names.csv'))
    out.sort(key=lambda r: (r['group'] != 'main_exact', r['unit'], r['entry']))
    with open(OUT, 'w', newline='') as f:
        w = csv.DictWriter(f, list(out[0]))
        w.writeheader()
        w.writerows(out)
    for group in ('main_exact', 'no_main_row'):
        rs = [r for r in out if r['group'] == group]
        print('%-12s %4d rows, %6d bytes, %3d units (%d rows in worker-held units)' % (
            group, len(rs), sum(r['bytes'] or 0 for r in rs), len({r['unit'] for r in rs}),
            sum(bool(r['held_by']) for r in rs)))


if __name__ == '__main__':
    main()
