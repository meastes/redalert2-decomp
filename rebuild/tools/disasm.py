"""dis.py ENTRY [BYTES]: the original instructions of a row, absolute targets annotated with the name
the rebuild tree's tables give that address (mischa85 names.csv, rebuild symbols.csv, units.json) -
names only, never another tree's body (lane R, the (c) sample)."""
import json, sys, csv
import capstone
import rebuild as R
from verify_manifest import Audit
audit = Audit(R.ROOT, json.loads(R.CONFIG.read_text())['executable'])
names = {}
for s, a in R.mischa_names().items(): names.setdefault(a, s)
for s, a in R.extra_symbols().items(): names.setdefault(a, s)
rows = {}
for u in R.load_json(R.UNITS, {}).values():
    for r in u['rows']: rows[int(r['entry'], 16)] = r
entry = int(sys.argv[1], 16)
size = int(sys.argv[2]) if len(sys.argv) > 2 else rows[entry]['body']
md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
import re
for i in md.disasm(audit.original(entry, size), entry):
    text = '%s %s' % (i.mnemonic, i.op_str)
    notes = [names[int(h, 16)] for h in re.findall(r'0x[0-9a-f]{6,8}', i.op_str) if int(h, 16) in names]
    print('%08x  %-44s %s' % (i.address, text, ' '.join(n[:70] for n in notes)))
