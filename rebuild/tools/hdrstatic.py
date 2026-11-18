"""hdrstatic.py UNIT: bind the per-unit copies of header statics (lane R5). A header constant with a
dynamic initializer (`const double CELL_SLOPE_ANGLE = ...;`) has internal linkage: every unit that
includes the header holds its own copy, so a name-wide binding (symbols.csv, main) points at another
unit's copy. For each initializer row of UNIT (`_$E<n>`) that is not exact but whose bytes equal the
original's outside its absolute (dir32) relocations, every data symbol it refers to is bound in the
unit's own table to the original operand at that offset. The audit compares every byte again. Run
`rebuild.py check UNIT --learn` afterwards."""
import json, struct, sys
import rebuild as R
from coff import Coff
from verify_manifest import Audit

unit = sys.argv[1]
units = R.load_json(R.UNITS, {})
info = units[unit]
last = json.load(open(R.WORK / 'last' / (unit + '.json')))
coff = Coff(open(last['object'], 'rb').read())
audit = Audit(R.ROOT, json.loads(R.CONFIG.read_text())['executable'])
functions = {s['name'] for s in coff._functions}
refs = info.setdefault('references', {})
found = {}
for r in last['results']:
    if r['status'] == 'exact' or not r['symbol'].startswith('_$E') or r['symbol'] not in functions:
        continue
    symbol, section, start, end = coff.function(r['symbol'])
    body = r['body']
    if end - start < body:
        continue
    actual = bytes(section['data'][start:start + body])
    original = audit.original(int(r['entry'], 16), body)
    relocs = [(o - start, x, k) for o, x, k in section['relocs'] if start <= o < start + body]
    masked = set()
    for o, x, k in relocs:
        masked.update(range(o, o + 4))
    if any(actual[i] != original[i] for i in range(body) if i not in masked):
        continue
    for o, x, k in relocs:
        # internal linkage: a static, or a const namespace-scope variable (decorated ...B)
        target = coff.symbols[x]
        if k != 6 or target['section'] <= 0 or not (target['storage'] == 3 or (target['name'].startswith('?') and target['name'].endswith('B'))) or target['name'].startswith(('_$E', '__real@', '??_C')):
            continue
        if coff.sections[target['section'] - 1]['flags'] & 0x20:
            continue                        # code, not a header static
        addend = struct.unpack_from('<I', actual, o)[0]
        value = struct.unpack_from('<I', original, o)[0] - addend
        found.setdefault(target['name'], set()).add(value)
bound = []
for name, values in sorted(found.items()):
    if len(values) == 1:
        value = '%08x' % values.pop()
        if refs.get(name) != value:
            refs[name] = value
            bound.append('%s = %s' % (name, value))
    else:
        print('  %s: the rows disagree, not bound' % name)
R.save_units(units, [unit])
print('%s: %d header statics bound' % (unit, len(bound)))
for b in bound:
    print('   ', b)
