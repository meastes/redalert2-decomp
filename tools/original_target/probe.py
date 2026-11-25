"""probe.py UNIT START END [OPTIONS-INDEX] [--asm ENTRY]: check src/UNIT.cpp against main's rows in
[START, END) without touching units.json (for drafting a unit while a batch owns the manifest)."""
import json, sys
import rebuild as R
from coff import Coff
from verify_manifest import Audit
unit, start, end = sys.argv[1], int(sys.argv[2], 16), int(sys.argv[3], 16)
opt = int(sys.argv[4]) if len(sys.argv) > 4 and not sys.argv[4].startswith('--') else 4
asm = [sys.argv[i + 1] for i, a in enumerate(sys.argv) if a == '--asm']
rows = sorted((r for r in R.main_catalog()['regions'] if start <= r['entry'] < end), key=lambda r: r['entry'])
names = {a: s for s, a in R.mischa_names().items()}
info = dict(source='src/%s.cpp' % unit, options=[['/O2', '/Ob2', '/G5', '/Gy', '/Gr'], ['/O2', '/Ob2', '/G5', '/Gy', '/Gr', '/GR']][opt == 6 and 1 or 0],
            range=['%08x' % start, '%08x' % end], references={}, rows=[])
obj, log, sec, folder = R.compile_unit(unit + '-probe', info)
if obj is None:
    print('\n'.join(l for l in log.splitlines() if 'error' in l)[:3000]); sys.exit(2)
audit = Audit(R.ROOT, json.loads(R.CONFIG.read_text())['executable'])
coff = Coff(obj.read_bytes()); resolve = R.resolver(unit, info)
funcs = {s['name'] for s in coff._functions}
for r in rows:
    name = names.get(r['entry'], r['symbol'])
    if name not in funcs:
        cands = [f for f in funcs if f.split('@')[0] == name.split('@')[0]]
        name = cands[0] if len(cands) == 1 else None
    if name is None:
        print('%08x missing %s' % (r['entry'], r['symbol'][:70])); continue
    st, diff, size, b, probs = R.compare(audit, coff, name, r['entry'], r['body_bytes'], r['padding_bytes'], resolve)
    print('%08x %-7s %4s %s %s' % (r['entry'], st, diff if st != 'exact' else '', name[:70], ' '.join(probs)[:120]))
    if '%08x' % r['entry'] in asm:
        print(R.disasm_diff(audit, R.compare.actual, r['entry'], r['body_bytes']))
