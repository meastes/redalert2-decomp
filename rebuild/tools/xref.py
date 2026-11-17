"""xref.py ENTRY...: the names main's accepted row at ENTRY references, each with its address and the
name mischa85's names.csv and the rebuild tree (symbols.csv) give that address. A translation table
for carrying an old body into the rebuild tree's names (lane R)."""
import sys
import rebuild as R
cat = R.main_catalog()
by = {r['entry']: r for r in cat['regions']}
names = {}
for s, a in R.mischa_names().items(): names.setdefault(a, []).append(s)
extra = {}
for s, a in R.extra_symbols().items(): extra.setdefault(a, []).append(s)
for arg in sys.argv[1:]:
    row = by[int(arg, 16)]
    seen = {}
    for b in row['relocations']:
        if b['symbol'] != '$local': seen[b['symbol']] = b['target']
    print('%s %s' % (arg, row['symbol']))
    for s, a in sorted(seen.items(), key=lambda kv: kv[1]):
        print('  %08x %-60s m85: %s %s' % (a, s[:60], ' '.join(names.get(a, [])[:2])[:90], ' '.join(extra.get(a, []))[:60]))
