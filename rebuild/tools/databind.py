"""databind.py UNIT: bind the data a unit defines (non-COMDAT .data/.rdata sections) by locating each
section's bytes in the original image: relocation fields masked, the match must be unique. Each
defined symbol then takes its section offset from that match; recorded in the unit's own table
(units.json references) with the section as evidence. The whole-file audit still compares every
byte with its relocations resolved (lane R)."""
import json, re, sys
import rebuild as R
from coff import Coff
import pefile
unit = sys.argv[1]
units = R.load_json(R.UNITS, {})
info = units[unit]
last = json.load(open(R.WORK / 'last' / (unit + '.json')))
c = Coff(open(last['object'], 'rb').read())
pe = pefile.PE(json.loads(R.CONFIG.read_text())['executable'], fast_load=True)
images = [(s.VirtualAddress + pe.OPTIONAL_HEADER.ImageBase, s.get_data()) for s in pe.sections if not s.Characteristics & 0x20]
resolve = R.resolver(unit, info)
for i, s in enumerate(c.sections, 1):
    if s['flags'] & 0x1000 or not s['flags'] & 0x40 or s['flags'] & 0x20 or not s['size'] or s['name'].startswith(('.CRT', '.debug', '.drectve')):
        continue
    defined = [n for n in c.symbols.values() if n['section'] == i and n['storage'] in (2, 3) and not n['name'].startswith('.')]
    if not defined or all(resolve(n['name'])[0] is not None for n in defined):
        continue
    data = bytes(s['data'])
    mask = set()
    for o, x, k in s['relocs']: mask.update(range(o, o + 4))
    pattern = b''.join(re.escape(bytes([b])) if j not in mask else b'.' for j, b in enumerate(data))
    hits = [(base + m.start()) for base, img in images for m in re.finditer(pattern, img, flags=re.S)]
    if len(hits) != 1:
        print('#%d %s: %d matches, not bound as a whole' % (i, [n['name'] for n in defined][:3], len(hits)))
        # Each symbol on its own (its bytes up to the next symbol): the linker keeps a section's
        # symbols in order, but the original object may order its tables differently; a symbol
        # binds when its own bytes occur once, at a 4-byte boundary (lane R5).
        cuts = sorted({n['value'] for n in defined} | {len(data)})
        for n in sorted(defined, key=lambda n: n['value']):
            if resolve(n['name'])[0] is not None:
                continue
            lo, hi = n['value'], cuts[cuts.index(n['value']) + 1]
            if hi - lo < 8:
                print('  %s: %d bytes, too short to place' % (n['name'][:60], hi - lo)); continue
            piece = b''.join(re.escape(bytes([data[j]])) if j not in mask else b'.' for j in range(lo, hi))
            found = [(base + m.start()) for base, img in images for m in re.finditer(piece, img, flags=re.S)
                     if (base + m.start()) % 4 == 0]
            if len(found) == 1:
                info.setdefault('references', {})[n['name']] = '%08x' % found[0]
                print('  %s = %08x (its own %d bytes, unique match)' % (n['name'][:70], found[0], hi - lo))
            else:
                print('  %s: %d matches, not bound' % (n['name'][:60], len(found)))
        continue
    for n in defined:
        if resolve(n['name'])[0] is None:
            info.setdefault('references', {})[n['name']] = '%08x' % (hits[0] + n['value'])
            print('  %s = %08x (section #%d, %d bytes, unique match)' % (n['name'][:70], hits[0] + n['value'], i, len(data)))
R.save_units(units, [unit])

# Pointers in a bound data section whose targets are unbound (string literal COMDATs): the target is
# the original pointer at that offset, accepted only when the literal's own bytes are found there.
import struct
from verify_manifest import Audit
audit = Audit(R.ROOT, json.loads(R.CONFIG.read_text())['executable'])
resolve = R.resolver(unit, info)
for i, s in enumerate(c.sections, 1):
    if s['flags'] & 0x1000 or not s['flags'] & 0x40 or s['flags'] & 0x20 or not s['relocs']:
        continue
    defined = sorted((n['value'], n['name']) for n in c.symbols.values() if n['section'] == i and n['storage'] in (2, 3) and not n['name'].startswith('.'))
    if not defined: continue
    for o, x, k in sorted(s['relocs']):
        target = c.symbols[x]
        if k != 6 or resolve(target['name'])[0] is not None: continue
        owner = [d for d in defined if d[0] <= o][-1]
        base, _ = resolve(owner[1])
        if base is None: continue
        address = struct.unpack('<I', audit.original(base + o - owner[0], 4))[0]
        literal = c.sections[target['section'] - 1]['data'] if target['section'] > 0 else None
        try:
            equal = literal is not None and audit.original(address, len(literal)) == bytes(literal)
        except ValueError:          # the pointer leads outside the image: no evidence (lane R5)
            equal = False
        if equal:
            info['references'][target['name']] = '%08x' % address
            print('  %s = %08x (pointer at %s+%d, literal bytes equal)' % (target['name'][:60], address, owner[1][:30], o - owner[0]))
R.save_units(units, [unit])
