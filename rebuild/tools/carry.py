"""carry.py ENTRY...: main's accepted body for each row (class b work): its source file, symbol and the
function text cut from main's source, followed by the names its references take in the rebuild tree
(xref). Read-only (lane R2)."""
import re, sys, subprocess
import rebuild as R
import guard

cat = R.main_catalog()
by = {r['entry']: r for r in cat['regions']}
for arg in sys.argv[1:]:
    row = by[int(arg, 16)]
    path = R.ROOT / row['source']
    text = path.read_text(encoding='latin-1')
    name = guard.demangle(row['symbol']) or ''
    print('== %s %s (%s, %d bytes)' % (arg, row['symbol'], row['source'], row['body_bytes']))
    hits = [m for m in re.finditer(r'^(?![ \t#/])[^\n;{}]*?(?<![\w:])%s\s*\(' % re.escape(name), text, re.M)
            if text.find(';', m.start(), text.find('{', m.start())) < 0] if name else []
    for m in hits[:2]:
        end = guard.body_end(text, text.find('{', m.start()))
        print(text[m.start():end])
    if not hits:
        print('(definition of %s not found by name)' % name)
subprocess.run([sys.executable, '-B', str(R.REBUILD / 'tools/xref.py')] + sys.argv[1:])
