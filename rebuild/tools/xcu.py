"""xcu.py UNIT: name the unit's initializer rows by the .CRT$XCU table. The object's table lists its
`_$E` initializers in registration order; the original image holds the unit's table as consecutive
pointers to the unit's rows. When both have the same length, row i takes object entry i's name
(the table is the evidence; the audit later checks the whole table again) (lane R)."""
import json, struct, sys
import rebuild as R
from coff import Coff
import pefile
unit = sys.argv[1]
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
    for first in sorted(e for e, r in rows.items() if r['main_symbol'].startswith('_$E')):
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
    R.write_json(R.UNITS, units)
