import json, sys, capstone, rebuild as R
from coff import Coff
from verify_manifest import Audit
u=sys.argv[1]; info=R.load_json(R.UNITS,{})[u]
last=json.load(open(R.WORK/'last'/(u+'.json'))); c=Coff(open(last['object'],'rb').read())
ours=[c.symbols[x]['name'] for s in c.sections if s['name']=='.CRT$XCU' for o,x,k in sorted(s['relocs'])]
def targets(name):
    sym,sec,a,b=c.function(name)
    return sorted({c.symbols[t]['name'] for o,t,k in sec['relocs'] if a<=o<b})[:3], b-a
print('object:')
for n in ours: print('  ',n,*targets(n))
audit=Audit(R.ROOT, json.loads(R.CONFIG.read_text())['executable'])
t=R.initializer_table(); rows=[int(r['entry'],16) for r in info['rows']]
idx=[t.index(a) for a in rows if a in t]
md=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32); md.detail=False
names={a:s for s,a in R.mischa_names().items()}
print('original:', len(idx))
if idx:
  for a in t[min(idx):max(idx)+1]:
    ops=[]
    for i in md.disasm(audit.original(a,64),a):
        import re
        for m in re.findall(r'0x[0-9a-f]{6,}', i.op_str): ops.append(m)
        if i.mnemonic=='ret': break
    print('   %08x'%a, ops[:4])
