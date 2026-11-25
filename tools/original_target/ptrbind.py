"""ptrbind.py UNIT: an unbound data symbol (.bss/.data) the unit defines, pointed to by a dir32 field of a
bound data section of the same object, takes the original pointer's value at that field (every field
that points to it must agree). The whole-file audit compares that pointer again. Run from tools/original_target."""
import json, os, struct, sys
sys.path.insert(0, os.getcwd())
import rebuild as R
from coff import Coff
from verify_manifest import Audit
unit = sys.argv[1]
units = R.load_json(R.UNITS, {})
info = units[unit]
last = json.load(open(R.WORK / 'last' / (unit + '.json')))
c = Coff(open(last['object'], 'rb').read())
audit = Audit(R.ROOT, json.loads(R.CONFIG.read_text())['executable'])
resolve = R.resolver(unit, info)
found = {}
for i, s in enumerate(c.sections, 1):
    if not s['flags'] & 0x40 or s['flags'] & 0x20 or not s['relocs'] or s['name'].startswith(('.CRT', '.debug', '.drectve')):
        continue
    defined = sorted((n['value'], n['name']) for n in c.symbols.values() if n['section'] == i and n['storage'] in (2, 3) and not n['name'].startswith('.'))
    if not defined:
        continue
    for o, x, k in sorted(s['relocs']):
        t = c.symbols[x]
        if k != 6 or t['name'].startswith('.') or t['section'] <= 0 or resolve(t['name'])[0] is not None:
            continue
        ts = c.sections[t['section'] - 1]
        if ts['flags'] & 0x20:      # code
            continue
        owners = [d for d in defined if d[0] <= o]
        if not owners:      # a field before the section's first defined symbol has no owner to read
            continue
        owner = owners[-1]
        base, _ = resolve(owner[1])
        if base is None:
            continue
        addend = struct.unpack_from('<I', bytes(s['data']), o)[0] if s['data'] else 0
        value = struct.unpack('<I', audit.original(base + o - owner[0], 4))[0] - addend
        found.setdefault(t['name'], set()).add(value)
for name, values in found.items():
    if len(values) == 1:
        info.setdefault('references', {})[name] = '%08x' % values.pop()
        print('  %s = %s (pointer in a bound data section)' % (name, info['references'][name]))
    else:
        print('  %s: pointers disagree %s' % (name, values))
R.save_units(units, [unit])
