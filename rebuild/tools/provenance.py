"""provenance.py UNIT [ENTRY=CLASS:FIRST:MINUTES:NOTE ...]: (re)write the unit's lines of
rebuild/manifest/provenance.csv from its last check. Every exact row is listed once.

CLASS  a  text adopted (TibSun archive / mischa85 / ra2-decomp), including compiler-emitted
          initializers and template copies of adopted headers
       b  body taken from the old tree (main's src/recovered) as the answer key, then reshaped
       c  written without either, from the Ghidra seed and the class layout alone
FIRST  yes: exact the first time the row was checked with its final text source (tooling and
          reference bindings aside); edit: exact after small edits (minutes); rematch: needed
          real re-matching work
Rows not given take the unit's default (--default CLASS:FIRST:SOURCE)."""
import csv, json, sys
import rebuild as R
PATH = R.MANIFEST / 'provenance.csv'
FIELDS = ['unit', 'entry', 'symbol', 'body_bytes', 'class', 'text_source', 'first_try', 'minutes', 'note']
unit = sys.argv[1]
default = ('a', 'yes', 'mischa85')
given = {}
for arg in sys.argv[2:]:
    if arg.startswith('--default='):
        default = tuple(arg.split('=', 1)[1].split(':')); continue
    entry, spec = arg.split('=', 1)
    parts = spec.split(':', 4) + [''] * 5
    given[entry] = parts[:5]
last = json.load(open(R.WORK / 'last' / (unit + '.json')))
rows = [r for r in csv.DictReader(open(PATH))] if PATH.exists() else []
rows = [r for r in rows if r['unit'] != unit]
for r in last['results']:
    if r['status'] != 'exact': continue
    cls, first, minutes, note, source = (given.get(r['entry']) or [default[0], default[1], '', '', default[2]])[:5] \
        if r['entry'] not in given else (given[r['entry']][0], given[r['entry']][1], given[r['entry']][2],
                                          given[r['entry']][3], {'a': default[2], 'b': 'old tree', 'c': 'seed'}[given[r['entry']][0]])
    rows.append(dict(unit=unit, entry=r['entry'], symbol=r['symbol'], body_bytes=r['body'], **{'class': cls},
                     text_source=source, first_try=first, minutes=minutes, note=note))
with open(PATH, 'w', newline='') as f:
    w = csv.DictWriter(f, fieldnames=FIELDS); w.writeheader()
    w.writerows(sorted(rows, key=lambda r: (r['unit'], r['entry'])))
print('%s: %d rows' % (unit, sum(1 for r in rows if r['unit'] == unit)))
