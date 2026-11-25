import json, sys, rebuild as R
from coff import Coff
u=sys.argv[1]; last=json.load(open(R.WORK/'last'/(u+'.json'))); c=Coff(open(last['object'],'rb').read())
for i,s in enumerate(c.sections,1):
    if s['flags'] & 0xc0 and not s['name'].startswith(('.debug','.drectve')):
        syms=[x['name'] for x in c.symbols.values() if x['section']==i][:6]
        print(i, s['name'], hex(s['flags']), s['size'], len(s['relocs']), syms, [c.symbols[t]['name'] for o,t,k in s['relocs']][:4])
