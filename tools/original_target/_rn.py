import json, rebuild as R
from coff import Coff
units=R.load_json(R.UNITS,{})
land={}; held={}
for u,info in units.items():
    p=R.WORK/'last'/(u+'.json')
    if not p.exists(): continue
    last=json.load(open(p))
    try: c=Coff(open(last['object'],'rb').read())
    except Exception: continue
    ex={r['symbol'] for r in last['results'] if r['status']=='exact'}
    for s in c.sections:
        if s['name']!='.CRT$XCU': continue
        for o,x,k in s['relocs']:
            n=c.symbols[x]['name']
            sym,sec,a,b=c.function(n)
            t={c.symbols[tt]['name'] for oo,tt,kk in sec['relocs'] if a<=oo<b}
            if '_RECT_NONE' in t:
                (land if info.get('status')=='catalogued' else held)[u]=(n, n in ex)
print('landed', len(land), list(land.items())[:15])
print('held', held)
