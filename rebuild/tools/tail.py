"""tail.py UNIT...: extend a unit's rows over the compiler-emitted copies the linker placed after its
functions when the object itself names them (lane R3). A function the object emits that binds to no
row is taken as the unit's copy at address A when (a) one of the object's vtables names it in a slot
and the original vtable holds A there, or (b) main's row at A carries the same Class::Name; and A lies
after the unit's range, before the next unit mischa85 places by evidence, and outside every other
unit's range. The unit's range then ends after the last such row, and every main row in between joins
the unit (rows outside mischa85's module range need not be exact; learning names the ones that are)."""
import json, struct, sys
import rebuild as R
import guard
from coff import Coff
from verify_manifest import Audit

audit = Audit(R.ROOT, json.loads(R.CONFIG.read_text())['executable'])
catalog = R.main_catalog()
main = {r['entry']: r for r in catalog['regions']}
ordered = sorted(catalog['regions'], key=lambda r: r['entry'])
modules = R.mischa_modules()
known_starts = sorted(int(m['start'], 16) for m in modules.values() if m['category'] != 'unknown')
units = R.load_json(R.UNITS, {})
names = {a: s for s, a in R.mischa_names().items()}
for unit in sys.argv[1:]:
    info = units[unit]
    last = json.load(open(R.WORK / 'last' / (unit + '.json')))
    c = Coff(open(last['object'], 'rb').read())
    resolve = R.resolver(unit, info)
    exact = {r['symbol'] for r in last['results'] if r['status'] == 'exact'}
    loose = {f['name'] for f in c._functions if f['name'] not in exact and resolve(f['name'])[0] is None}
    lo, hi = int(info['range'][0], 16), int(info['range'][1], 16)
    nxt = min([s for s in known_starts if s >= hi] or [hi])
    others = [(int(i['range'][0], 16), int(i['range'][1], 16)) for u, i in units.items() if u != unit]
    def free(a):
        return hi <= a < nxt and not any(s <= a < e for s, e in others)
    targets = {}
    for i, s in enumerate(c.sections, 1):
        vt = [n for n in c.symbols.values() if n['section'] == i and n['name'].startswith('??_7')]
        if not vt: continue
        va = resolve(vt[0]['name'])[0]
        if va is None: continue
        for o, x, k in s['relocs']:
            n = c.symbols[x]['name']
            if n in loose:
                t = struct.unpack('<I', audit.original(va - vt[0]['value'] + o, 4))[0]
                if t in main and free(t):
                    targets[n] = t
    by_name = {}
    for r in ordered:
        if free(r['entry']):
            d = guard.demangle(r['symbol'])
            if d: by_name.setdefault(d, []).append(r['entry'])
    for n in loose - set(targets):
        d = guard.demangle(n)
        if d and len(by_name.get(d, [])) == 1:
            targets[n] = by_name[d][0]
    if not targets:
        print('%s: no trailing copies' % unit); continue
    new_end = max(t + main[t]['full_extent_bytes'] for t in targets.values())
    have = {r['entry'] for r in info['rows']}
    added = [r for r in ordered if hi <= r['entry'] < new_end and '%08x' % r['entry'] not in have]
    for r in added:
        info['rows'].append(dict(entry='%08x' % r['entry'], body=r['body_bytes'], padding=r['padding_bytes'],
                                 symbol=names.get(r['entry'], r['symbol']), main_symbol=r['symbol'],
                                 main_source=r['source']))
    info['rows'].sort(key=lambda r: r['entry'])
    info['range'][1] = '%08x' % new_end
    print('%s: range end %08x -> %08x, %d rows added (%s)' % (unit, hi, new_end, len(added),
          ' '.join(sorted(n[:40] for n in targets))[:200]))
R.save_units(units, sys.argv[1:])
