"""xcu.py UNIT: name the unit's initializer rows by the .CRT$XCU table. The object's table lists its
`_$E` initializers in registration order; the original image holds the unit's table as consecutive
pointers to the unit's rows. When both have the same length, row i takes object entry i's name
(the table is the evidence; the audit later checks the whole table again) (lane R)."""
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
    R.save_units(units, [unit])
