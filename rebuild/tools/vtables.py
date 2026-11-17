"""vtables.py UNIT: the object's vtable sections, with each slot's binding in the rebuild catalog's
reference table (UNBOUND when none) - the work list before a whole-file build (lane R)."""
import json, sys
import rebuild as R, catalog_io
from coff import Coff
unit = sys.argv[1]
last = json.load(open(R.WORK / 'last' / (unit + '.json')))
c = Coff(open(last['object'], 'rb').read())
refs = catalog_io.load(R.MANIFEST / 'catalog')['population_references'].get('rebuild-' + unit.replace('_', '-'), {})
for i, s in enumerate(c.sections, 1):
    names = [n['name'] for n in c.symbols.values() if n['section'] == i and n['name'].startswith('??_7')]
    if not names: continue
    missing = [(o, c.symbols[x]['name']) for o, x, k in sorted(s['relocs']) if c.symbols[x]['name'] not in refs]
    if missing:
        print('#%d %s %s' % (i, names[0][:80], 'vtable bound %08x' % refs[names[0]] if names[0] in refs else 'vtable UNBOUND'))
        for o, n in missing: print('   +%02x %s' % (o, n[:110]))

if '--learn' in sys.argv:
    # An unbound slot of a vtable whose own binding is known takes the address the original vtable
    # holds there, when main has an accepted function at that address (its spelling differs: const
    # template arguments, mischa85's placeholder names). Recorded in symbols.csv with the evidence.
    import csv, struct
    from verify_manifest import Audit
    audit = Audit(R.ROOT, json.loads(R.CONFIG.read_text())['executable'])
    main = {r['entry']: r['symbol'] for r in R.main_catalog()['regions']}
    rows = list(csv.DictReader(open(R.SYMBOLS))) if R.SYMBOLS.exists() else []
    known = {r['symbol'] for r in rows}
    for i, s in enumerate(c.sections, 1):
        vt = [n for n in c.symbols.values() if n['section'] == i and n['name'].startswith('??_7')]
        names = [n['name'] for n in vt]
        if not names or names[0] not in refs: continue
        base = refs[names[0]] - vt[0]['value']     # a /GR vtable starts with its locator pointer
        for o, x, k in sorted(s['relocs']):
            n = c.symbols[x]['name']
            if n in known: continue
            target = struct.unpack('<I', audit.original(base + o, 4))[0]
            if refs.get(n) == target: continue
            if target in main:
                rows.append(dict(symbol=n, address='0x%08X' % target,
                                 evidence='slot +%02x of %s at %08x; main row %s' % (o, names[0], base, main[target])))
                known.add(n); print('  bound %s = %08x (main %s)' % (n[:70], target, main[target][:60]))
            else:
                print('  no main row for %s at %08x' % (n[:70], target))
    with open(R.SYMBOLS, 'w', newline='') as f:
        w = csv.DictWriter(f, fieldnames=['symbol', 'address', 'evidence'])
        w.writeheader(); w.writerows(sorted(rows, key=lambda r: r['symbol']))
