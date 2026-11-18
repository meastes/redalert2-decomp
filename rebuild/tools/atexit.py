"""atexit.py UNIT: bind a unit's static-destructor rows (lane R5). A global with a destructor gets two
compiler-emitted functions: the initializer (in the .CRT$XCU table, named by xcu.py) and the
destructor thunk it registers with atexit. The initializer's check binds the thunk's name to the
original address it pushes (a per-unit reference), but the row of that address keeps the stale
name the unit's earlier text gave it. Step 1 renames such a row to the bound name and drops the
reference. Step 2 binds the `_$S<n>` guard bytes the thunks test: each is read from the original
row at the offset where the object's thunk refers to it, and bound only when every reference of the
name in the unit's rows agrees (the audit later compares every byte again). Run `rebuild.py check
UNIT --learn` afterwards."""
import json, struct, sys
import rebuild as R
from coff import Coff

unit = sys.argv[1]
units = R.load_json(R.UNITS, {})
info = units[unit]
last = json.load(open(R.WORK / 'last' / (unit + '.json')))
status = {r['entry']: r['status'] for r in last['results']}
refs = info.setdefault('references', {})
coff = Coff(open(last['object'], 'rb').read())
present = {m['name'] for m in coff.symbols.values()}
for name in [n for n in refs if n.startswith(('_$E', '_$S')) and n not in present]:
    del refs[name]          # a per-unit name of an earlier compile (the numbering moves with the text)
renamed = []
for row in info['rows']:
    if status.get(row['entry']) == 'exact' or not (status.get(row['entry']) == 'missing' or row['symbol'].startswith('_$E')):
        continue
    bound = [n for n, a in refs.items() if n.startswith('_$E') and a == row['entry']]
    if len(bound) == 1:
        renamed.append('%s %s -> %s' % (row['entry'], row['symbol'], bound[0]))
        row['symbol'] = bound[0]
        del refs[bound[0]]

config = json.loads(R.CONFIG.read_text())
import pefile
pe = pefile.PE(config['executable'], fast_load=True)
base = pe.OPTIONAL_HEADER.ImageBase
rows = {r['symbol']: int(r['entry'], 16) for r in info['rows']}
seen = {}
for i, s in enumerate(coff.sections, 1):
    if s['name'] != '.text':
        continue
    owner = [m for m in coff.symbols.values() if m['section'] == i and m['storage'] in (2, 3)
             and not m['name'].startswith('.')]
    if len(owner) != 1 or owner[0]['name'] not in rows:
        continue
    entry = rows[owner[0]['name']] + owner[0]['value']
    for offset, index, kind in s['relocs']:
        name = coff.symbols[index]['name']
        if not name.startswith('_$S') or name in refs:
            continue
        addend = struct.unpack_from('<I', bytes(s['data']), offset)[0]
        target = struct.unpack('<I', pe.get_data(entry + offset - base, 4))[0] - addend
        seen.setdefault(name, set()).add(target)
bound = []
for name, targets in sorted(seen.items()):
    if len(targets) == 1:
        refs[name] = '%08x' % targets.pop()
        bound.append('%s = %s' % (name, refs[name]))
    else:
        print('  %s: references disagree (%s), not bound' % (name, ' '.join('%08x' % t for t in sorted(targets))))
R.save_units(units, [unit])
print('%s: %d rows renamed, %d guard names bound' % (unit, len(renamed), len(bound)))
for x in renamed + bound:
    print('   ', x)
