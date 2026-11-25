"""expansions.py HEADER OLD NEW SYMBOL ADDRESS [UNIT ...]: how many sites of one inline function the exact rows
expand, against the image's calls to its out-of-line copy (lane R42, review direction 2026-11-19).

HEADER (relative to include) has the text OLD replaced by NEW (NEW declares the function without a
body, so every source site becomes a call) for the duration of the run only; the file is restored at the end
(also on failure). Each unit that names the function is compiled with it. For every row the unit's last check
finds exact, the calls to SYMBOL in our object (every source site, expanded or not) are set against the
original's direct calls to ADDRESS in the same row (the sites the image keeps out of line, which an exact row
reproduces). Their difference is the number of sites the exact row expands.

Prints per unit and in total: image call sites in exact rows, source sites in exact rows, expanded sites; and
the image's call sites in rows that are not exact. Nothing in rebuild/ is written."""
import collections
import hashlib
import json
import sys

import capstone

import rebuild as R
from coff import Coff
from verify_manifest import Audit


def main():
    header, old, new, symbol, address = sys.argv[1:6]
    address = int(address, 16)
    only = sys.argv[6:]
    path = R.ROOT / 'include' / header
    original_text = path.read_bytes()
    text = original_text.decode('latin-1')
    if text.count(old) != 1:
        sys.exit('OLD must occur exactly once in %s (found %d)' % (header, text.count(old)))
    audit = Audit(R.ROOT, json.loads(R.CONFIG.read_text())['executable'])
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    units = R.load_json(R.UNITS, {})
    totals = collections.Counter()
    try:
        path.write_text(text.replace(old, new), encoding='latin-1')
        for unit in (only or sorted(units)):
            info = units[unit]
            source = R.ROOT / info['source']
            last = R.load_json(R.WORK / 'last' / (unit + '.json'), None)
            if not source.exists() or not last:
                continue
            exact = {r['entry']: r['symbol'] for r in last['results'] if r['status'] == 'exact'}
            image_exact = image_other = 0
            image_rows = {}
            for r in info['rows']:
                entry = int(r['entry'], 16)
                n = sum(1 for i in md.disasm(audit.original(entry, r['body']), entry)
                        if i.mnemonic == 'call' and i.op_str == hex(address))
                image_rows[r['entry']] = n
                if r['entry'] in exact:
                    image_exact += n
                else:
                    image_other += n
            obj, log, _, folder = R.compile_unit(unit + '-expn', info)
            if obj is None:
                print('%-14s does not compile with the declaration' % unit, flush=True)
                totals['units_failed'] += 1
                continue
            coff = Coff(obj.read_bytes())
            target = next((i for i, s in coff.symbols.items() if s['name'] == symbol), None)
            source_sites = 0
            per_row = []
            names = {s['name'] for s in coff._functions}
            for entry, name in exact.items():
                if name not in names:
                    continue
                sym, section, start, end = coff.function(name)
                k = sum(1 for o, i, kind in section['relocs']
                        if kind == 20 and start <= o < end and i == target)
                source_sites += k
                if k != image_rows.get(entry, 0):
                    per_row.append('%s:%d/%d' % (entry, image_rows.get(entry, 0), k))
            import shutil
            shutil.rmtree(folder, ignore_errors=True)
            if source_sites or image_exact or image_other:
                print('%-14s image calls in exact rows %4d, source sites in exact rows %4d, expanded %4d; '
                      'image calls in other rows %4d  %s' % (unit, image_exact, source_sites, source_sites - image_exact,
                                                            image_other, ' '.join(per_row[:8])), flush=True)
            totals['image_exact'] += image_exact; totals['source_exact'] += source_sites
            totals['image_other'] += image_other
    finally:
        path.write_bytes(original_text)
    print('TOTAL image calls in exact rows %d, source sites in exact rows %d, expanded in exact rows %d; image calls '
          'in rows not exact %d; units failed %d' % (totals['image_exact'], totals['source_exact'],
                                                     totals['source_exact'] - totals['image_exact'],
                                                     totals['image_other'], totals['units_failed']))


if __name__ == '__main__':
    main()
