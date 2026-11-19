import json, sys, capstone
import rebuild as R
from coff import Coff
u, name = sys.argv[1], sys.argv[2]
last = json.load(open(R.WORK/'last'/(u+'.json'))); c = Coff(open(last['object'],'rb').read())
sym, sec, a, b = c.function(name)
rel = {o: c.symbols[t]['name'] for o, t, k in sec['relocs']}
for i in capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32).disasm(bytes(sec['data'][a:b]), 0):
    r = [n for o, n in rel.items() if i.address <= o < i.address + i.size]
    print('%04x %s %s %s' % (i.address, i.mnemonic, i.op_str, r[0] if r else ''))
