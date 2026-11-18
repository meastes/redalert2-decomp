"""rename_bound.py UNIT...: a row whose last check found no function under its name takes the name of the
object's function that the unit's own bindings (units.json references) already place at the row's entry, so
the next `check --learn` compares it and infers the data its bytes differ by (lane R6)."""
import json, sys
import rebuild as R
units = R.load_json(R.UNITS, {})
changed = []
for unit in sys.argv[1:]:
    info = units[unit]
    last = json.load(open(R.WORK / 'last' / (unit + '.json')))
    status = {r['entry']: r['status'] for r in last['results']}
    resolve = R.resolver(unit, info)
    by_entry = {}
    for name in last['others']:
        address, label = resolve(name)
        if address is not None and label == 'unit':
            by_entry.setdefault('%08x' % address, []).append(name)
    for row in info['rows']:
        names = by_entry.get(row['entry'], [])
        if status.get(row['entry']) == 'missing' and len(names) == 1:
            print('%s %s: %s -> %s' % (unit, row['entry'], row['symbol'], names[0]))
            row['symbol'] = names[0]
            changed.append(unit)
if changed:
    R.save_units(units, sorted(set(changed)))
