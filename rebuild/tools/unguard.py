"""unguard.py UNIT [--small N]: try each draft guarded as unplaced ("no row of the original matches it yet")
- and with --small, each guarded with N or fewer differing bytes (scores taken before the unit's bindings
were learned) - without its guard, one at a time; keep the guard off when the unit gains exact rows and loses none (a row now
placed by a wider unit range, a renamed row, a binding learned since). Each trial is one check of the
whole unit (lane R4)."""
import json, re, subprocess, sys
import rebuild as R
from guard import body_end

unit = sys.argv[1]
info = R.load_json(R.UNITS, {})[unit]
src = R.ROOT / info['source']
P = sys.executable


def exact():
    subprocess.run([P, '-B', str(R.REBUILD / 'tools/rebuild.py'), 'check', unit, '--learn'], capture_output=True)
    last = json.load(open(R.WORK / 'last' / (unit + '.json')))
    if last['source_sha256'] != __import__('hashlib').sha256(src.read_bytes()).hexdigest():
        return None    # compile failed: the last check is an older text
    return {r['entry'] for r in last['results'] if r['status'] == 'exact'}


SMALL = int(sys.argv[sys.argv.index('--small') + 1]) if '--small' in sys.argv else -1


def wanted(label):
    if label.startswith('no row of the original matches it yet'):
        return True
    m = re.match(r'(\d+) (?:of \d+ )?bytes differ', label)
    return bool(m) and int(m.group(1)) <= SMALL


def guards(text):
    out = []
    for m in re.finditer(r'#ifdef RA2_NON_MATCHING // not exact: ([^\n]*)\n', text):
        if not wanted(m.group(1)):
            continue
        close = body_end(text, text.find('{', m.end()))
        nl = text.find('\n', close)
        if close > 0 and text.startswith('\n#endif', nl):
            out.append((m.start(), nl + len('\n#endif'), m.end(), nl))
    return out


base = exact()
if base is None:
    print('%s: does not compile' % unit); sys.exit(1)
kept = 0
i = 0
while True:
    text = src.read_text(encoding='latin-1')
    g = guards(text)
    if i >= len(g):
        break
    s, e, ins, ine = g[i]
    trial = text[:s] + text[ins:ine] + text[e:]
    src.write_text(trial, encoding='latin-1')
    now = exact()
    if now is not None and base <= now and len(now) > len(base):
        base = now; kept += 1
        print('  unguarded at line %d: %d rows exact' % (text[:s].count('\n') + 1, len(now)))
    else:
        src.write_text(text, encoding='latin-1')
        i += 1
exact()
print('%s: %d unplaced drafts unguarded, %d rows exact' % (unit, kept, len(base)))
