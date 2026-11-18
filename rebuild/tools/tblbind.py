"""tblbind.py UNIT: bind a unit's data tables whose bytes are all relocations (pointer tables to string
literals), which databind cannot place (the masked pattern matches everywhere). Each unbound string literal
COMDAT the table points to binds where its own bytes occur exactly once in the image's data (4-aligned);
then the table's bytes, every relocation filled with its now-bound target, must occur exactly once. Both
bindings go in the unit's own table; the whole-file audit compares every byte again. Run from rebuild/tools."""
import json, os, re, struct, sys
sys.path.insert(0, os.getcwd())
import rebuild as R
import pefile
from coff import Coff
unit = sys.argv[1]
units = R.load_json(R.UNITS, {})
info = units[unit]
last = json.load(open(R.WORK / 'last' / (unit + '.json')))
c = Coff(open(last['object'], 'rb').read())
pe = pefile.PE(json.loads(R.CONFIG.read_text())['executable'], fast_load=True)
images = [(s.VirtualAddress + pe.OPTIONAL_HEADER.ImageBase, s.get_data()) for s in pe.sections if not s.Characteristics & 0x20]
refs = info.setdefault('references', {})
resolve = R.resolver(unit, info)


def unique(blob, align=4):
    hits = [base + m.start() for base, img in images for m in re.finditer(re.escape(blob), img)
            if (base + m.start()) % align == 0]
    return hits[0] if len(hits) == 1 else None


changed = False
for i, s in enumerate(c.sections, 1):
    if s['flags'] & 0x1000 or not s['flags'] & 0x40 or s['flags'] & 0x20 or not s['size'] or not s['relocs'] \
            or s['name'].startswith(('.CRT', '.debug', '.drectve')):
        continue
    defined = sorted((n['value'], n['name']) for n in c.symbols.values() if n['section'] == i and n['storage'] in (2, 3) and not n['name'].startswith('.'))
    if not defined or all(resolve(n)[0] is not None for _, n in defined):
        continue
    data = bytearray(s['data'])
    ok = True
    for o, x, k in s['relocs']:
        t = c.symbols[x]
        if k != 6:
            ok = False; break
        addr = resolve(t['name'])[0]
        if addr is None and t['name'].startswith('??_C@') and t['section'] > 0:
            lit = bytes(c.sections[t['section'] - 1]['data'])
            addr = unique(lit, 1 if len(lit) < 4 else 4)
            if addr is not None and len(lit) >= 6:
                refs[t['name']] = '%08x' % addr; changed = True
                print('  %s = %08x (its %d bytes occur once)' % (t['name'], addr, len(lit)))
                resolve = R.resolver(unit, info)
            else:
                addr = None
        if addr is None:
            ok = False; break
        addend = struct.unpack_from('<I', data, o)[0]
        struct.pack_into('<I', data, o, (addr + addend) & 0xffffffff)
    if not ok:
        print('#%d %s: a relocation target stays unbound' % (i, defined[0][1])); continue
    at = unique(bytes(data))
    if at is None:
        print('#%d %s: resolved table not unique' % (i, defined[0][1])); continue
    for off, n in defined:
        if resolve(n)[0] is None:
            refs[n] = '%08x' % (at + off); changed = True
            print('  %s = %08x (section #%d resolved, unique)' % (n, at + off, i))
if changed:
    R.save_units(units, [unit])
