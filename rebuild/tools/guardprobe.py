"""guardprobe.py UNIT [ENTRY ...]: what holds a row that is exact once the unit's guards are open (tailmap's
exact_when_open bucket; lane R43, 2026-11-19).

The unit's text is compiled in scratch copies (never the tree's file): once as it stands, then with each
RA2_NON_MATCHING guard opened alone. Every row is compared each time. Per guard: the rows it gains, the rows
it loses, and whether the guard's own functions are all exact when it is open (then the guard can come off as
it stands). Per wanted row (the given entries, else every row not exact now): the guards that make it exact
alone, or "none alone" (it needs several guards together, a binding, or a bound). Count-sensitive guards are
listed but never opened."""
import collections
import json
import re
import sys

import rebuild as R
from coff import Coff
from verify_manifest import Audit
from tailmap import guards, open_text, compile_text


def statuses(unit, info, audit, text, opened, tag):
    data, log = compile_text(unit, info, open_text(text, opened), tag)
    if data is None:
        return None, log
    coff = Coff(data)
    resolve = R.resolver(unit, info)
    names = {s['name'] for s in coff._functions}
    byaddr = {}
    for n in names:
        a = resolve(n)[0]
        if a is not None:
            byaddr.setdefault(a, n)
    out, owner = {}, {}
    unbound = [n for n in names if resolve(n)[0] is None]
    for r in info['rows']:
        f = r['symbol'] if r['symbol'] in names else byaddr.get(int(r['entry'], 16))
        tries = [f] if f else ([n for n in unbound if n not in owner.values()] if r['body'] >= 8 else [])
        st = 'missing'
        for n in tries:
            try:
                st = R.compare(audit, coff, n, int(r['entry'], 16), r['body'], r.get('gap_padding', r['padding']), resolve)[0]
            except Exception:      # noqa: BLE001
                st = 'error'
            if st == 'exact' or f:
                owner[r['entry']] = n
                break
        out[r['entry']] = st if (f or st == 'exact') else 'missing'
    statuses.owner = owner
    return out, names


def main():
    unit = sys.argv[1]
    want = set(a.lower() for a in sys.argv[2:])
    info = R.load_json(R.UNITS, {})[unit]
    audit = Audit(R.ROOT, json.loads(R.CONFIG.read_text())['executable'])
    text = (R.ROOT / info['source']).read_text(encoding='latin-1')
    base, base_names = statuses(unit, info, audit, text, set(), 'p0')
    if base is None:
        print('%s: does not compile as it stands' % unit); return
    if not want:
        want = {e for e, s in base.items() if s != 'exact' and e not in (info.get('count_sensitive') or {})}
    sym = {r['entry']: r['symbol'] for r in info['rows']}
    body = {r['entry']: r['body'] for r in info['rows']}
    held = collections.defaultdict(list)
    for start, end, label in guards(text):
        line = text[:start].count('\n') + 1
        if 'count-sensitive' in label:
            print('guard line %d: count-sensitive, not opened' % line); continue
        now, names = statuses(unit, info, audit, text, {start}, 'p1')
        if now is None:
            print('guard line %d: does not compile when open (%s)' % (line, label[:80])); continue
        gain = sorted(e for e in now if now[e] == 'exact' and base[e] != 'exact')
        lose = sorted(e for e in now if now[e] != 'exact' and base[e] == 'exact')
        # Functions the open guard adds to the object that are not exact rows: a draft that is not exact,
        # or a function with no row (no independent bound) - either keeps the guard on.
        owned = set(statuses.owner[e] for e in gain if e in statuses.owner) | \
            {sym[e] for e in gain}
        resolve = R.resolver(unit, info)
        exact_entries = {int(e, 16) for e in now if now[e] == 'exact'}
        own = {int(e, 16) for e in now}
        still = sorted(n for n in names - base_names - owned
                       if not n.startswith(('_$', '??_G', '??_E')) and resolve(n)[0] not in exact_entries
                       and not ('?$' in n and resolve(n)[0] is not None and resolve(n)[0] not in own))
        print('guard line %d (%s): gains %d (%d B), loses %d, added not exact %d%s' % (
            line, label[18:90], len(gain), sum(body[e] for e in gain), len(lose), len(still),
            '  -> can come off' if gain and not lose and not still else ''))
        for e in gain:
            held[e].append(line)
        if lose:
            print('    loses: ' + ' '.join(lose))
        if still and gain:
            print('    added, not exact rows: ' + ' '.join('%s@%s' % (n[:60], '%08x' % resolve(n)[0] if resolve(n)[0] else 'unbound') for n in still))
    print('\nrows:')
    for e in sorted(want):
        print('  %s %5d %-70s %s' % (e, body.get(e, 0), sym.get(e, '?')[:70],
                                     ('guards %s' % held[e]) if held[e] else 'none alone (base %s)' % base.get(e)))


if __name__ == '__main__':
    main()
