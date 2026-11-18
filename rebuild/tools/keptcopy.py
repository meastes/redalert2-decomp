"""keptcopy.py UNIT [--write]: rule on each select-any COMDAT the unit's object emits that binds to no
row or address (review ruling 2026-11-18, the second half of the unemitted-functions row in
docs/DECISIONS.md), one section at a time:

(1) the image holds a copy: main names the same symbol (the resolver already binds it), else exactly one
    place in the image's code equals our bytes modulo relocations, and every relocation operand there
    holds the address our target symbol is bound to (where it is bound). Ours is then the copy the
    linker discarded: --write binds the name to that copy in symbols.csv, with the kept copy's main unit
    as evidence (the audit omits the section by that binding and compares the bytes again);
(2) no copy: the original did not instantiate it here or inlined it - a source question, reported;
(3) a no-duplicates COMDAT (a non-inline external function under /Gy) is never bound this way.
Lane R7."""
import json, struct, sys
import rebuild as R
from coff import Coff
import pefile

unit = sys.argv[1]
write = '--write' in sys.argv
units = R.load_json(R.UNITS, {})
info = units[unit]
last = json.load(open(R.WORK / 'last' / (unit + '.json')))
coff = Coff(open(last['object'], 'rb').read())
resolve = R.resolver(unit, info)
exact = {r['symbol'] for r in last['results'] if r['status'] == 'exact'}
pe = pefile.PE(json.loads(R.CONFIG.read_text())['executable'], fast_load=True)
base = pe.OPTIONAL_HEADER.ImageBase
text = [s for s in pe.sections if s.Characteristics & 0x20]
main_units = R.load_json(R.ROOT / 'manifests/original_target/unit_map.json', {}).get('units', {})
unit_of = {int(a, 16): u for u, entries in main_units.items() if isinstance(entries, list) for a in entries}


def selection(index):
    records = [s for s in coff.symbols.values() if s['section'] == index and s['storage'] == 3
               and s['type'] == 0 and len(s['auxiliary']) == 18]
    return records[0]['auxiliary'][14] if len(records) == 1 else None


def masked(data, relocs):
    data = bytearray(data)
    for o in relocs:
        data[o:o + 4] = bytes(4)
    return bytes(data)


rows = []
for index, section in enumerate(coff.sections, 1):
    if not section['flags'] & 0x20 or not section['flags'] & 0x1000 or not section['size']:
        continue
    names = [s['name'] for s in coff.symbols.values() if s['section'] == index and s['storage'] == 2 and s['value'] == 0]
    if len(names) != 1 or names[0] in exact or names[0].startswith(('??_G', '??_E')):
        continue
    name = names[0]
    if resolve(name)[0] is not None:
        continue
    if selection(index) != 2:
        print('%s: (3) no-duplicates COMDAT (an external function), never bound as a kept copy' % name)
        continue
    relocs = [(o, coff.symbols[t]['name'], k) for o, t, k in section['relocs']]
    pattern = masked(section['data'], [o for o, _, _ in relocs])
    hits = []
    for s in text:
        data = s.get_data()
        at = 0
        while True:
            # Search on the longest relocation-free run, then compare the whole masked body.
            at = data.find(pattern[:max(1, min([o for o, _, _ in relocs] or [len(pattern)]))], at)
            if at < 0:
                break
            if masked(data[at:at + len(pattern)], [o for o, _, _ in relocs]) == pattern:
                hits.append(base + s.VirtualAddress + at)
            at += 1
    hits = [h for h in hits if h % 16 == 0]

    def disagreements(address):
        bad = []
        for o, target, kind in relocs:
            bound = resolve(target)[0]
            value = struct.unpack_from('<I', pe.get_data(address - base + o, 4))[0]
            if kind == 20:
                value = (value + address + o + 4) & 0xffffffff
            if bound is not None and bound != value:
                bad.append('%s at +%x is %08x, bound %08x' % (target, o, value, bound))
        return bad
    # A copy whose relocation operands name other targets (another instance with the same code) is not ours.
    agreeing = [h for h in hits if not disagreements(h)]
    if len(agreeing) != 1:
        print('%s: (2) %s' % (name, 'no copy in the image: a source question' if not hits else
                                  'copies at %s, %d agreeing at the relocations: not bound' % (
                                      ' '.join('%08x' % h for h in hits), len(agreeing))))
        continue
    address = agreeing[0]
    if not any(resolve(t)[0] is not None for _, t, _ in relocs) and len(hits) > 1:
        print('%s: (2) copies at %s and no bound relocation tells them apart: not bound' % (
            name, ' '.join('%08x' % h for h in hits)))
        continue
    owner = unit_of.get(address, '?')
    if owner == unit:
        evidence = ('kept copy: byte-equal modulo relocations to the image at %08x, relocation operands agree; '
                    'main files it in this unit (%s), past the rebuild range: the copy the linker kept is this '
                    "object's own, left as original bytes until the unit's rows reach it (lane R7)" % (address, owner))
    else:
        evidence = ('kept copy: byte-equal modulo relocations to the image at %08x (main unit %s), relocation '
                    'operands agree; ours is the linker-discarded duplicate (lane R7)' % (address, owner))
    print('%s: (1) %s' % (name, evidence))
    rows.append(dict(symbol=name, address='0x%08X' % address, evidence=evidence))
if write and rows:
    R.save_symbols(rows)
