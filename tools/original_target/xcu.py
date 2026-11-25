"""xcu.py UNIT: name the unit's initializer rows by the .CRT$XCU table. The object's table lists its
`_$E` initializers in registration order; the original image holds the unit's table as consecutive
pointers to the unit's rows. When both have the same length, row i takes object entry i's name
(the table is the evidence; the audit later checks the whole table again) (lane R).
Initializers the table does not list (a constructor an XCU entry calls, a destructor it registers with
atexit) are then named through the references: where a named initializer row's object function refers to an
`_$E` symbol, the original bytes at the same offset address the row that symbol names (c2land 2026-11-19;
the compiler's `_$E` counter renumbers them whenever an earlier function changes)."""
import json, struct, sys
import rebuild as R
from coff import Coff
import pefile
unit = sys.argv[1]
if '--score' in sys.argv:
    # Exact rows less the initializers the object registers that are not exact rows: the measure the
    # prologue variants are ranked by (an extra header static costs as much as a missing one).
    last = json.load(open(R.WORK / 'last' / (unit + '.json')))
    c = Coff(open(last['object'], 'rb').read())
    exact = {r['symbol'] for r in last['results'] if r['status'] == 'exact'}
    ours = [c.symbols[x]['name'] for s in c.sections if s['name'] == '.CRT$XCU' for o, x, k in s['relocs']]
    print(len(exact) - sum(n not in exact for n in ours)); sys.exit(0)
units = R.load_json(R.UNITS, {})
info = units[unit]
last = json.load(open(R.WORK / 'last' / (unit + '.json')))
c = Coff(open(last['object'], 'rb').read())
ours = [c.symbols[x]['name'] for s in c.sections if s['name'] == '.CRT$XCU' for o, x, k in sorted(s['relocs'])]
rows = {int(r['entry'], 16): r for r in info['rows']}
pe = pefile.PE(json.loads(R.CONFIG.read_text())['executable'], fast_load=True)
found = None
for sec in pe.sections:
    if sec.Characteristics & 0x20: continue
    data = sec.get_data()
    xcu = set(R.initializer_table())
    for first in sorted(e for e, r in rows.items() if r['main_symbol'].startswith('_$E') or e in xcu):
        i = data.find(struct.pack('<I', first))
        while i >= 0 and i % 4 == 0 and i >= 4 and struct.unpack_from('<I', data, i - 4)[0] in rows:
            i -= 4
        if i < 0: continue
        table = []
        while i + 4 <= len(data) and struct.unpack_from('<I', data, i)[0] in rows:
            table.append(struct.unpack_from('<I', data, i)[0]); i += 4
        found = table; break
    if found: break
print('object table %d entries, original table %s' % (len(ours), len(found) if found else None))
if found and len(found) == len(ours):
    for address, name in zip(found, ours):
        rows[address]['symbol'] = name
        print('  %08x = %s' % (address, name))

by_name = {}
for s in c.symbols.values():
    by_name.setdefault(s['name'], s)
image = pe.OPTIONAL_HEADER.ImageBase
def original(address, size):
    return pe.get_data(address - image, size)
# Start from rows whose name is evidenced: the table's (when it matched) and those the last check found exact.
exact = {r['entry'] for r in last['results'] if r['status'] == 'exact'}
named = set(found) if found and len(found) == len(ours) else set()
todo = [r for e, r in rows.items() if r['symbol'].startswith('_$E') and (e in named or r['entry'] in exact)]
seen = set()
while todo:
    r = todo.pop()
    entry = int(r['entry'], 16)
    if entry in seen: continue
    seen.add(entry)
    try:
        symbol, section, start, end = c.function(r['symbol'])
    except ValueError:
        continue
    if symbol['name'] != r['symbol'] or end - start < r['body']: continue
    body = original(entry, r['body'])
    for offset, index, kind in section['relocs']:
        if not start <= offset < start + r['body']: continue
        name = c.symbols[index]['name']
        if not name.startswith('_$E'): continue
        at = offset - start
        value = struct.unpack_from('<I', body, at)[0]
        target = value if kind == 6 else (entry + at + 4 + value) & 0xffffffff if kind == 20 else None
        row = rows.get(target)
        if row is None or not row['main_symbol'].startswith('_$E') or row['symbol'] == name: continue
        print('  %08x = %s (referenced by %s at %08x)' % (target, name, r['symbol'], entry + at))
        row['symbol'] = name
        todo.append(row); seen.discard(target)
R.save_units(units, [unit])
