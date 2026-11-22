"""flex_menu.py [--write]: the end-of-week flex menu (owner, 2026-11-21): one row per parked function of the
rebuild with the flex that would land it. Rows only: no flex lands without the user's word.

Sources: rebuild/manifest/tail_buckets.csv (run `tailmap.py --write` first: every row of units.json that is not
exact, with its bucket), units.json `count_sensitive`, every RA2_NON_MATCHING guard in rebuild/src (its label
gives the score; guarded drafts with no row of their own are listed too), variation_candidates.csv,
no_draft_queue.csv and function_map.csv of main (sizes of rows the rebuild has not bounded).

Flex, first that applies (the others that also apply go to `also`):
  a  hand-made variation already known: on variation_candidates.csv, or the guard says the text is exact
     but some of its forms have no reference text (a lane's find)
  b  exact when checked on its own: tailmap bucket exact_when_open (exact with every guard of the unit open)
  c  kept unused declarations would hold it: a count-sensitive park (exact under a declaration count window)
  d  another named allowance (named in `flex_note`): unbound = binding of references this check has not
     bound (the bytes differ only at those references)
  e  none known: stays a flagged draft (or has no draft yet: `draft` = no)
Writes rebuild/manifest/flex_menu.csv; the totals per flex are rows whose unit is "(total)".

The gap between the function-level and the whole-unit counts (rebuild_census.function_level; owner,
2026-11-21) is itemised in the same file: each function counted at function level but not built (columns
gap_reason / allowance) and rows whose unit is "(gap)" with the totals per reason. A guard-open exact row is
sized with guardprobe.py (--probe runs it for the units concerned; the outputs are kept under
build/r/flex/<unit>.txt and reused while the unit's source is unchanged). Allowance = what would have to be
allowed for the function to be compiled into the build rather than only counted: nothing; a count-holding
declaration; a mask; an unverified reference; the parent landing (a row exact only inside one parked parent,
named in gap_reason; the parent's row carries `brings`, the rows and bytes its landing would add)."""
import csv
import collections
import re
import sys

import rebuild as R
from guard import demangle

MAN = R.MANIFEST
OUT = MAN / 'flex_menu.csv'
GUARD = re.compile(r'^#ifdef RA2_NON_MATCHING\b[^\n]*', re.M)
DEF = re.compile(r'^[A-Za-z_][^\n;{}()]*?\b((?:[A-Za-z_]\w*::)*~?[A-Za-z_]\w*|operator\s*[^\s(]+)\s*\(', re.M)


def guards(text):
    out = []
    for m in GUARD.finditer(text):
        depth = 0
        for line in re.finditer(r'^[ \t]*#[ \t]*(if|ifdef|ifndef|endif)\b[^\n]*', text[m.start():], re.M):
            if line.group(1).startswith('if'):
                depth += 1
            else:
                depth -= 1
                if depth == 0:
                    out.append((m.group(0), text[m.end():m.start() + line.start()]))
                    break
    return out


def score(label):
    m = re.search(r'(\d[\d,]*) of (\d[\d,]*) bytes differ', label) or re.search(r'(\d[\d,]*) bytes differ', label)
    if m:
        return m.group(1).replace(',', '')
    m = re.search(r'best ([\d.]+)', label)
    if m:
        return 'main best %s%%' % m.group(1)
    return ''


def probe(unit, entries):
    """{entry: (guard lines that make it exact alone, {guard line: (loses, added not exact)})} from guardprobe."""
    import hashlib, subprocess
    info = R.load_json(R.UNITS, {})[unit]
    key = hashlib.sha256((R.ROOT / info['source']).read_bytes() + ' '.join(sorted(entries)).encode()).hexdigest()[:16]
    out = R.WORK / 'flex' / ('%s.%s.txt' % (unit, key))
    if not out.exists():
        if '--probe' not in sys.argv:
            return None
        out.parent.mkdir(parents=True, exist_ok=True)
        r = subprocess.run([sys.executable, '-B', str(R.REBUILD / 'tools/guardprobe.py'), unit] + sorted(entries),
                           capture_output=True, text=True)
        out.write_text(r.stdout + r.stderr)
    text = out.read_text()
    gl = {}
    for m in re.finditer(r'^guard line (\d+) \((.*?)\): gains \d+ \(\d+ B\), loses (\d+), added not exact (\d+)'
                         r'(?:\n    loses:[^\n]*)?(?:\n    added, not exact rows: ([^\n]*))?', text, re.M):
        added = [(a.rsplit('@', 1)[0], a.rsplit('@', 1)[1]) for a in (m.group(5) or '').split() if '@' in a]
        gl[m.group(1)] = (int(m.group(3)), int(m.group(4)), added, m.group(2))
    rows = {}
    for m in re.finditer(r'^  ([0-9a-f]{8})\s+\d+ .*?(guards \[([\d, ]*)\]|none alone.*)$', text, re.M):
        rows[m.group(1)] = ([g.strip() for g in (m.group(3) or '').split(',') if g.strip()], gl)
    return rows


def holder_of(unit, added):
    """(entry, name) of the parked parent among a guard's added, not-exact functions (guardprobe): the one at an
    address inside the unit's range or among its rows (template or inline copies elsewhere are not parents),
    else ('', '')."""
    info = R.load_json(R.UNITS, {}).get(unit) or {}
    own = {x['entry'] for x in info.get('rows', [])}
    lo, hi = (info.get('range') or ['0', '0'])[:2]
    # A template instance (a copy itself) is a parent only when no other function qualifies.
    for sym, at in sorted(added, key=lambda a: '?$' in a[0]):
        if re.fullmatch(r'[0-9a-f]{8}', at) and (at in own or int(lo, 16) <= int(at, 16) < int(hi, 16)):
            return at, demangle(sym) or sym
    return '', ''


def gap_of(r, probes):
    """(reason, allowance) for a function counted at function level but not built, else ('', ''). A row exact
    only inside a parked parent names the parent (lane O46, 2026-11-22: the template copies each flex would
    bring along) and sets r['holder'] to the parent's entry."""
    if r['bucket'] == 'count_sensitive':
        return 'exact form parked count-sensitive', 'a count-holding declaration'
    if r['bucket'] != 'exact_when_open':
        return '', ''
    p = (probes.get(r['unit']) or {}).get(r['entry'])
    if p is None:
        return 'guard-open exact (not probed)', 'unknown until probed'
    gs, gl = p
    if not gs:
        return 'guard-open exact only with several guards open together', 'a mask'
    loses, added, functions, label = gl.get(gs[0], (0, 0, [], ''))
    if added == 0 and loses == 0 and 'not exact' in label and 'text is exact' not in label:
        # guardprobe counts rows only: the guard's own draft has no row (image-derived bound, no independent
        # bound) and is not exact by its label, so the guard cannot come off (lane O46: missile 0x593870)
        return 'exact only inside a parked parent (guard-open exact): the guard\'s draft, which has no row', \
            'the parent landing'
    if added == 0 and loses == 0:
        if r['flex'] == 'a':
            return 'guard-open exact, its own guard could come off, held as a hand-made variation', \
                'nothing (but its text is a hand-made variation: the user\'s word)'
        return 'guard-open exact, its own guard could come off as it stands', 'nothing'
    if added:
        entry, name = holder_of(r['unit'], functions)
        if entry:
            r['holder'] = entry
            return 'exact only inside a parked parent (guard-open exact): %s %s' % (name, entry), 'the parent landing'
        return 'exact only inside a parked parent (guard-open exact)', 'a mask'
    return 'guard-open exact, opening its guard loses %d rows' % loses, 'a count-holding declaration'


def main():
    units = R.load_json(R.UNITS, {})
    tail = list(csv.DictReader(open(MAN / 'tail_buckets.csv', encoding='utf-8', errors='replace')))
    variation = {r['entry'].lower(): r for r in csv.DictReader(open(MAN / 'variation_candidates.csv', encoding='utf-8', errors='replace'))}
    sizes = {}
    for r in csv.DictReader(open(R.ROOT / 'manifests/function_map.csv')):
        sizes[r['address'][2:].lower()] = (r['size'], r['symbol'])
    blocks = collections.defaultdict(list)       # unit -> [(label, body)]
    for u, info in units.items():
        p = R.ROOT / info['source']
        if p.exists():
            blocks[u] = guards(p.read_text(encoding='latin-1'))
    used = set()

    def guard_for(unit, function):
        q = demangle(function) or function
        name = q.split('::')[-1]
        pat = re.compile(r'(^|[\s*&])%s\s*\(' % re.escape(q if '::' in q else name), re.M)
        for i, (label, body) in enumerate(blocks.get(unit, [])):
            for m in pat.finditer(body):
                line = body[body.rfind('\n', 0, m.start()) + 1:body.find('\n', m.end())]
                if not line.rstrip().endswith(';') and not line.startswith(('\t', ' ')):
                    used.add((unit, i))
                    return label
        return ''

    rows = []
    # tail_buckets.csv is refreshed only by tailmap.py --write: leave out rows that have since left units.json or
    # that the unit's current check finds exact (lane O46: 12 stale rows, 1,031 B, counted in the gap).
    current = {}
    for u, info in units.items():
        last = R.last_check(u, info)
        status = {x['entry']: x['status'] for x in last['results']} if last else {}
        for x in info['rows']:
            current[(u, x['entry'])] = status.get(x['entry'])
    for r in tail:
        if (r['unit'], r['entry']) not in current or current[(r['unit'], r['entry'])] == 'exact':
            continue
        rows.append(dict(unit=r['unit'], entry=r['entry'], function=demangle(r['function']) or r['function'],
                         bytes=int(r['bytes']), bucket=r['bucket'], label=guard_for(r['unit'], r['function']),
                         draft='no' if r['bucket'] in ('no_draft', 'text_if0') else 'yes'))
    for u, info in sorted(units.items()):
        body = {x['entry']: x for x in info['rows']}
        for e, d in sorted((info.get('count_sensitive') or {}).items()):
            x = body.get(e, {})
            sym = x.get('symbol', '')
            rows.append(dict(unit=u, entry=e, function=demangle(sym) or sym, bytes=int(x.get('body', 0) or 0),
                             bucket='count_sensitive', label=guard_for(u, sym) or 'count-sensitive', draft='yes'))
    known = {(r['unit'], r['entry']) for r in rows}
    # Names a guard's draft may carry, per unit: the queue's functions with no draft and the image-derived
    # bounds' catalog symbols (short name -> entry), so a draft whose label gives no address is still placed.
    named = collections.defaultdict(dict)
    queue = list(csv.DictReader(open(MAN / 'no_draft_queue.csv', encoding='utf-8', errors='replace')))
    for r in queue:
        q = demangle(r['symbol']) or r['symbol']
        named[r['unit']].setdefault(q.split('::')[-1], r['entry'])
    bounds = R.MANIFEST / 'image_bounds.csv'
    image = {r['entry']: r for r in csv.DictReader(open(bounds))} if bounds.exists() else {}
    # guarded drafts with no row of their own (no independent bound / unplaced)
    for u in sorted(blocks):
        for i, (label, body) in enumerate(blocks[u]):
            if (u, i) in used or 'count-sensitive' in label:
                continue
            m = re.search(r'\(([0-9a-f]{8}) has no row\)', label) or \
                re.search(r'image-derived bound ([0-9a-f]{8})-', label)
            # The guard's function definitions (declarations and inline/static helpers set apart): the draft
            # is named after the one the label or the queue places, else the first that is not a helper
            # (lane O33's find: Unit_Draw_Voxel showed as its helper Is_Barrel_Ahead).
            defs = []
            for fm in DEF.finditer(body):
                line = body[fm.start():body.find('\n', fm.end()) if '\n' in body[fm.end():] else len(body)]
                if re.sub(r'//.*', '', line).rstrip().endswith(';'):
                    continue
                defs.append((fm.group(1), bool(re.match(r'(static\s+)?inline\b|static\b', line))))
            if not defs:
                fm = DEF.search(body)
                if not fm:
                    continue    # a guarded declaration or data block, not a function draft
                defs = [(fm.group(1), False)]   # a guarded object defined by a constructor call (RECT_NONE_2)
            e = m.group(1) if m else ''
            fname = ''
            for name, helper in defs:
                at = named[u].get(name.split('::')[-1])
                if at and (not e or at == e):
                    e, fname = at, name
                    break
            if not fname:
                fname = next((n for n, helper in defs if not helper), defs[0][0])
            if not e:
                a = re.search(r'RA2: 0x([0-9A-Fa-f]{6,8})', body[:400])
                e = a.group(1).lower().zfill(8) if a else ''
            if e and (u, e) in known:
                continue
            size = sizes.get(e, ('', ''))[0] if e else ''
            if not size and e in image:
                size = image[e]['body_bytes']
            if 'unplaced' in label or not e:
                bucket = 'unplaced'
            else:
                bucket = 'no_row'
            rows.append(dict(unit=u, entry=e, function=fname, bytes=int(size or 0), bucket=bucket, label=label,
                             draft='yes'))
            known.add((u, e or fname))
    # no draft in the unit yet (no_draft_queue.csv)
    for r in queue:
        if (r['unit'], r['entry']) in known:
            continue
        rows.append(dict(unit=r['unit'], entry=r['entry'], function=demangle(r['symbol']) or r['symbol'],
                         bytes=int(r['bytes'] or 0), bucket='no_draft (%s)' % r['group'], label='', draft='no'))
        known.add((r['unit'], r['entry']))

    for r in rows:
        flex = []
        if r['entry'].lower() in variation:
            flex.append(('a', 'variation_candidates.csv: ' + variation[r['entry'].lower()].get('main_variation', '')[:160]))
        if 'text is exact' in r['label']:
            flex.append(('a', "lane's find: the text is exact, some forms have no reference text (guard label)"))
        if r['bucket'] == 'exact_when_open':
            flex.append(('b', 'exact with every guard of the unit open'))
        if r['bucket'] == 'count_sensitive':
            flex.append(('c', 'exact under a declaration-count window (units.json count_sensitive)'))
        if r['bucket'] == 'unbound':
            flex.append(('d', 'binding allowance: differs only at references this check has not bound'))
        if not flex:
            if r['draft'] == 'no':
                note = 'no draft yet'
            elif r['bucket'] == 'no_row':
                note = 'no row of its own yet (needs an independent bound, then an exact draft); bytes = main\'s size if main has the row'
            elif r['bucket'] == 'unplaced':
                note = 'no row of the original matches the draft yet (unplaced; bytes unknown)'
            else:
                note = 'stays a flagged draft'
            flex.append(('e', note))
        seen = []
        for f, _ in flex:
            if f not in seen:
                seen.append(f)
        r.update(flex=flex[0][0], flex_note=flex[0][1], also=' '.join(seen[1:]), best=score(r['label']))

    want = collections.defaultdict(set)
    for r in rows:
        if r['bucket'] == 'exact_when_open':
            want[r['unit']].add(r['entry'])
    probes = {u: probe(u, e) for u, e in sorted(want.items())}
    extent = {}
    for u, info in units.items():
        for x in info['rows']:
            extent[x['entry']] = x['body'] + (x.get('padding') or 0)
    gap = collections.OrderedDict()
    for r in rows:
        r['gap_reason'], r['allowance'] = gap_of(r, probes)
        if r['gap_reason']:
            key = (r['gap_reason'].split(': ')[0] if r.get('holder') else r['gap_reason'], r['allowance'])
            g = gap.setdefault(key, [0, 0])
            g[0] += 1; g[1] += extent.get(r['entry'], r['bytes'])
    # What each parent's landing brings along: the gap rows it holds (lane O46, 2026-11-22).
    brings = collections.defaultdict(lambda: [0, 0])
    for r in rows:
        if r.get('holder'):
            brings[(r['unit'], r['holder'])][0] += 1
            brings[(r['unit'], r['holder'])][1] += extent.get(r['entry'], r['bytes'])
    for r in rows:
        n, b = brings.get((r['unit'], r['entry']), (0, 0))
        r['brings'] = '+%d rows / %d B (template or inline copies only it emits)' % (n, b) if n else ''
    unbound = [r for r in rows if r['bucket'] == 'unbound']

    rows.sort(key=lambda r: (r['flex'], r['unit'], r['entry'], r['function']))
    cols = ['unit', 'entry', 'function', 'bytes', 'bucket', 'best', 'flex', 'flex_note', 'also', 'draft',
            'gap_reason', 'allowance', 'brings']
    tot = collections.OrderedDict((f, [0, 0]) for f in 'abcde')
    for r in rows:
        tot[r['flex']][0] += 1
        tot[r['flex']][1] += r['bytes']
    nod = [r for r in rows if r['draft'] == 'no']
    if '--write' in sys.argv:
        with open(OUT, 'w', newline='', encoding='utf-8') as f:
            w = csv.DictWriter(f, cols, extrasaction='ignore', lineterminator='\n')
            w.writeheader()
            for r in rows:
                w.writerow(r)
            for k, (n, b) in tot.items():
                w.writerow(dict(unit='(total)', entry='', function='flex %s: %d functions' % (k, n), bytes=b,
                                bucket='', best='', flex=k, flex_note='', also='', draft=''))
            w.writerow(dict(unit='(total)', function='no draft yet: %d functions' % len(nod),
                            bytes=sum(r['bytes'] for r in nod), flex='e', draft='no'))
            for (reason, allow), (n, b) in gap.items():
                w.writerow(dict(unit='(gap)', function='%d functions' % n, bytes=b, gap_reason=reason, allowance=allow))
            w.writerow(dict(unit='(gap)', function='%d functions' % sum(v[0] for v in gap.values()),
                            bytes=sum(v[1] for v in gap.values()), gap_reason='total: function-level minus whole-unit'))
            w.writerow(dict(unit='(gap)', function='0 functions', bytes=0, gap_reason='exact but unbound / data not bound',
                            allowance='an unverified reference'))
            w.writerow(dict(unit='(not counted)', function='%d functions' % len(unbound), bytes=sum(r['bytes'] for r in unbound),
                            bucket='unbound', gap_reason='differ only at unbound references: counted at neither level',
                            allowance='an unverified reference'))
    for k, (n, b) in tot.items():
        print('flex %s: %4d functions %8d B' % (k, n, b))
    print('no draft yet (within e): %d functions %d B' % (len(nod), sum(r['bytes'] for r in nod)))
    print('rows %d' % len(rows))
    print('gap (function-level minus whole-unit; bytes with padding):')
    for (reason, allow), (n, b) in gap.items():
        print('  %4d functions %7d B  %s  -> allowance: %s' % (n, b, reason, allow))
    print('  %4d functions %7d B  total' % (sum(v[0] for v in gap.values()), sum(v[1] for v in gap.values())))
    print('  not counted at either level: unbound %d functions %d B -> an unverified reference' % (
        len(unbound), sum(r['bytes'] for r in unbound)))


if __name__ == '__main__':
    main()
