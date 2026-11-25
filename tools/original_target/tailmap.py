"""tailmap.py [UNIT ...] [--write] [--notes]: map the parked tail by cause (lane R42, review session direction
2026-11-19).

For every unit with rows that are not exact (count-sensitive rows excepted, as in the census), a scratch copy
of the unit's text is compiled with every RA2_NON_MATCHING guard opened (never the tree's file). A guard whose
draft does not compile is closed again (the first error's line names it; otherwise a bisection over the
guards finds it) and its rows count as "does not compile". Every other row that is not exact is compared with
the original and bucketed by its first kind of difference:

  no_draft          no definition of the row's name in the unit's text
  text_if0          the definition is only inside #if 0 (adopted text not yet worked)
  not_emitted       an active definition the object does not emit under a name bound to the row
  no_compile        the row's draft is under a guard that does not compile
  calls_differ      more than four direct calls differ (the draft's text differs, not one expansion)
  inline_call       one to four direct calls differ: the original calls a callee out of line that we expand
                    (direction "orig_calls") or we call one the original expands ("ours_calls")
  unbound           the bytes differ only at relocations this check has not bound (data or callee)
  inline_body       same calls, different instruction count (a different expanded body or statement shape)
  stack_layout      same calls and count; the first difference is a stack-frame displacement
  register_order    same calls and count; registers, operand order or scheduling differ
  exact_when_open   exact once the guards are open (a guard kept for another row, or a stale label)

Per row: unit, entry, function, bytes, bucket, top_callee (the first differing callee, its template family
for inline_call; the first unbound name for unbound), direction, detail. Results are cached per unit under
build/r/tailmap/ by source and header state; --write writes manifests/tail_buckets.csv and prints the
summary (rows, bytes, top callee families per bucket). Nothing else in manifests is written.

Each bucket's summary line is followed by the docs/VC6_MATCHING_NOTES.md sections that address it, from
manifests/bucket_notes.csv (lane L1); print one with
`python3 tools/original_target/notes_index.py --section "<title>"`. --notes prints the summary of the
tail_buckets.csv already written, with those sections, without compiling anything."""
import collections
import csv
import hashlib
import io
import contextlib
import json
import re
import sys
from pathlib import Path

import capstone

import rebuild as R
from coff import Coff
from verify_manifest import Audit

VERSION = 'v2'
OUT = R.MANIFEST / 'tail_buckets.csv'
NOTES = R.MANIFEST / 'bucket_notes.csv'
CACHE = R.WORK / 'tailmap'
GUARD = re.compile(r'^#ifdef RA2_NON_MATCHING\b[^\n]*', re.M)
MD = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)


def family(name):
    """'Class::Member' of a mangled name, template arguments dropped (DynamicVectorClass::Add)."""
    if name is None:
        return '-'
    m = re.match(r'\?\?([0-9A-Z_])\?\$(\w+)@', name)
    if m:
        kind = {'0': m.group(2), '1': '~' + m.group(2), '4': 'operator=', 'A': 'operator[]'}.get(m.group(1), '??' + m.group(1))
        return '%s::%s' % (m.group(2), kind)
    m = re.match(r'\?\?([0-9A-Z_])(\w+)@', name)
    if m:
        kind = {'0': m.group(2), '1': '~' + m.group(2), '4': 'operator=', 'A': 'operator[]'}.get(m.group(1), '??' + m.group(1))
        return '%s::%s' % (m.group(2), kind)
    m = re.match(r'\?(\w+)@\?\$(\w+)@', name)
    if m:
        return '%s::%s' % (m.group(2), m.group(1))
    m = re.match(r'\?(\w+)@(\w+)@@', name)
    if m:
        return '%s::%s' % (m.group(2), m.group(1))
    m = re.match(r'\?(\w+)@@', name)
    if m:
        return m.group(1)
    return name.lstrip('_')


def guards(text):
    """[(start, end_line_index, label_line)] of the RA2_NON_MATCHING guards (start offset, matching #endif offset)."""
    out = []
    for m in GUARD.finditer(text):
        depth, i = 0, m.start()
        for line in re.finditer(r'^[ \t]*#[ \t]*(if|ifdef|ifndef|endif)\b[^\n]*', text[m.start():], re.M):
            if line.group(1).startswith('if'):
                depth += 1
            else:
                depth -= 1
                if depth == 0:
                    out.append((m.start(), m.start() + line.start(), m.group(0)))
                    break
    return out


def open_text(text, opened):
    """TEXT with the guards at the given start offsets opened (#if 1), offsets kept by padding."""
    parts, last = [], 0
    for start in sorted(opened):
        end = text.index('\n', start)
        parts.append(text[last:start]); parts.append('#if 1'.ljust(end - start)[:max(5, end - start)])
        last = end
    parts.append(text[last:])
    return ''.join(parts)


def compile_text(unit, info, text, tag):
    path = R.WORK / 'tailmap' / 'src' / ('%s.cpp' % unit)
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(text, encoding='latin-1')
    variant = dict(info, source=str(path.relative_to(R.ROOT)))
    obj, log, _, folder = R.compile_unit(unit + '-tailmap', variant, tag)
    data = obj.read_bytes() if obj is not None else None
    import shutil
    shutil.rmtree(folder, ignore_errors=True)
    return data, log


def first_error_line(log, unit):
    for line in log.splitlines():
        m = re.search(r'([^\\/]+)\((\d+)\)\s*:\s*(?:fatal )?error', line)
        if m:
            return int(m.group(2)) if m.group(1).lower() == ('%s.cpp' % unit).lower() else -1
    return None


def compile_open(unit, info, text):
    """(object bytes, offsets of guards that stayed closed because they do not compile)."""
    gs = guards(text)
    opened = {g[0] for g in gs}
    closed = []
    lines = [0] + [m.end() for m in re.finditer('\n', text)]
    for _ in range(len(gs) + 1):
        data, log = compile_text(unit, info, open_text(text, opened), 'o')
        if data is not None:
            return data, closed
        if not opened:
            return None, closed
        line = first_error_line(log, unit)
        culprit = None
        if line and line > 0:
            at = lines[line - 1] if line - 1 < len(lines) else len(text)
            inside = [g for g in gs if g[0] in opened and g[0] <= at <= g[1]]
            before = [g for g in gs if g[0] in opened and g[0] <= at]
            culprit = (inside or before[-1:] or [None])[0]
        if culprit is None:
            # bisection: the shortest prefix of the open guards that fails
            order = sorted(opened)
            lo, hi = 0, len(order)
            while lo < hi:
                mid = (lo + hi) // 2
                ok, _ = compile_text(unit, info, open_text(text, order[:mid + 1]), 'b')
                if ok is None:
                    hi = mid
                else:
                    lo = mid + 1
            if lo >= len(order):
                return None, closed + sorted(opened)
            culprit = next(g for g in gs if g[0] == order[lo])
        opened.discard(culprit[0]); closed.append(culprit[0])
    return None, closed


def classify(audit, coff, fname, row, resolve, names):
    entry, body = int(row['entry'], 16), row['body']
    status, differing, size, bindings, problems = R.compare(audit, coff, fname, entry, body,
                                                            row.get('gap_padding', row['padding']), resolve)
    if status == 'exact':
        return 'exact_when_open', '', '', 'exact with the guards open (the guard may protect another row)'
    actual = R.compare.actual
    original = audit.original(entry, body)
    # direct calls
    sym, section, start, end = coff.function(fname)
    data = section['data']
    ours = []
    for offset, index, kind in sorted(section['relocs']):
        if kind != 20 or not start <= offset < end or offset < 1 or data[offset - 1] != 0xE8:
            continue
        n = coff.symbols[index]['name']
        if n.startswith('$'):
            continue
        ours.append((n, resolve(n)[0]))
    orig = [int(i.op_str, 16) for i in MD.disasm(original, entry) if i.mnemonic == 'call' and i.op_str.startswith('0x')]
    oc = collections.Counter(orig)
    uc = collections.Counter(a for _, a in ours if a is not None)
    unresolved = [n for n, a in ours if a is None]
    orig_only = list((oc - uc).elements())
    ours_only = list((uc - oc).elements())
    if len(unresolved) == len(orig_only) and not ours_only and orig_only:
        return 'unbound', unresolved[0], '', 'unbound callee'
    if orig_only or ours_only or unresolved:
        if len(orig_only) + len(ours_only) + len(unresolved) > 4:
            first = min(orig_only, key=lambda a: orig.index(a)) if orig_only else None
            return 'calls_differ', family(names.get(first)) if first else '', '', \
                'orig-only %d, ours-only %d, unresolved %d' % (len(orig_only), len(ours_only), len(unresolved))
        if orig_only:
            first = min(orig_only, key=lambda a: orig.index(a))
            n = names.get(first, '%08x' % first)
            return 'inline_call', family(n), 'orig_calls', n[:120]
        n = unresolved[0] if unresolved else next(n for n, a in ours if a in ours_only)
        return 'inline_call', family(n), 'ours_calls', n[:120]
    # unbound relocations only
    unb = [p.rsplit(' (', 1)[0] for p in problems]
    if unb and len(actual) == len(original):
        fields = set()
        for offset, index, kind in section['relocs']:
            if start <= offset < end and resolve(coff.symbols[index]['name'])[0] is None:
                fields.update(range(offset - start, offset - start + 4))
        if all(i in fields for i, (a, b) in enumerate(zip(actual, original)) if a != b):
            return 'unbound', unb[0][:120], '', 'unbound data'
    a = list(MD.disasm(bytes(actual), entry))
    b = list(MD.disasm(original, entry))
    if len(a) != len(b):
        return 'inline_body', '', '', 'instructions %d vs original %d' % (len(a), len(b))
    for x, y in zip(a, b):
        if (x.mnemonic, x.op_str) != (y.mnemonic, y.op_str):
            if x.mnemonic.startswith('j') and y.mnemonic.startswith('j') and x.mnemonic == y.mnemonic:
                continue
            fx = re.sub(r'(esp|ebp)( [+-] 0x[0-9a-f]+)?', r'\1+D', x.op_str)
            fy = re.sub(r'(esp|ebp)( [+-] 0x[0-9a-f]+)?', r'\1+D', y.op_str)
            detail = '+%x %s %s | orig %s %s' % (y.address - entry, x.mnemonic, x.op_str, y.mnemonic, y.op_str)
            if x.mnemonic == y.mnemonic and fx == fy:
                return 'stack_layout', '', '', detail
            return 'register_order', '', '', detail
    return 'unbound', unb[0][:120] if unb else '', '', 'relocations only'


def dead_regions(text):
    """[(start, end)] of the #if 0 blocks (nested conditionals followed)."""
    out, stack = [], []
    for m in re.finditer(r'^[ \t]*#[ \t]*(if|ifdef|ifndef|else|elif|endif)\b([^\n]*)', text, re.M):
        kind = m.group(1)
        if kind.startswith('if'):
            stack.append((m.start(), kind == 'if' and m.group(2).strip().startswith('0')))
        elif kind == 'endif' and stack:
            start, dead = stack.pop()
            if dead:
                out.append((start, m.end()))
    return out


def where_defined(text, symbol):
    """no_draft (no definition of the row's name in the unit's text), text_if0 (only inside #if 0: text not
    yet worked), or not_emitted (an active definition the object does not emit under a name bound to the row:
    a static expanded into its caller, an unbound name, another signature)."""
    m = re.match(r'\?\?[01](\w+)@|\?(\w+)@|_(\w+)', symbol)
    word = m and (m.group(1) or m.group(2) or m.group(3))
    if not word:
        return 'no_draft'
    word = word.split('@')[0]
    dead = dead_regions(text)
    found = []
    for d in re.finditer(r'^(?![ \t]*[#/])[^\n;{}]*?(?<![\w~])%s\s*\(' % re.escape(word), text, re.M):
        brace = text.find('{', d.start())
        semi = text.find(';', d.start())
        if brace >= 0 and (semi < 0 or semi > brace):
            found.append(any(s <= d.start() < e for s, e in dead))
    if not found:
        return 'no_draft'
    return 'text_if0' if all(found) else 'not_emitted'


def header_state():
    h = hashlib.sha256()
    for p in sorted(q for q in (R.ROOT / 'include').rglob('*') if q.is_file()):
        h.update(p.name.encode()); h.update(p.read_bytes())
    return h.hexdigest()


def map_unit(unit, info, audit, names, hstate):
    source = R.ROOT / info['source']
    if not source.exists():
        return [dict(unit=unit, entry=r['entry'], function=r['symbol'], bytes=r['body'], bucket='no_draft',
                     top_callee='', direction='', detail='no source') for r in info['rows']]
    text = source.read_text(encoding='latin-1')
    key = hashlib.sha256((VERSION + text + hstate + json.dumps(info['rows'], sort_keys=True)).encode('latin-1', 'replace')).hexdigest()
    cache = CACHE / (unit + '.json')
    cached = R.load_json(cache, None)
    if cached and cached.get('key') == key:
        return cached['rows']
    last = R.last_check(unit, info)
    if last is None:
        with contextlib.redirect_stdout(io.StringIO()):
            R.check(unit)
        last = R.last_check(unit, info)
    status = {r['entry']: r['status'] for r in last['results']} if last else {}
    sensitive = info.get('count_sensitive') or {}
    todo = [r for r in info['rows'] if status.get(r['entry']) != 'exact' and r['entry'] not in sensitive]
    out = []
    if todo:
        data, closed = compile_open(unit, info, text)
        resolve = R.resolver(unit, info)
        coff = Coff(data) if data is not None else None
        byaddr, bynames = {}, set()
        if coff is not None:
            bynames = {s['name'] for s in coff._functions}
            for n in bynames:
                a = resolve(n)[0]
                if a is not None:
                    byaddr.setdefault(a, n)
        closed_ranges = [(g[0], g[1]) for g in guards(text) if g[0] in closed]
        for r in todo:
            entry = int(r['entry'], 16)
            fname = r['symbol'] if r['symbol'] in bynames else byaddr.get(entry)
            rec = dict(unit=unit, entry=r['entry'], function=r['symbol'], bytes=r['body'], top_callee='',
                       direction='', detail='')
            if coff is None:
                rec.update(bucket='no_compile', detail='unit does not compile with its guards open')
            elif fname is None:
                rec.update(bucket='no_compile' if closed_ranges else 'no_draft',
                           detail=('%d guard(s) do not compile' % len(closed_ranges)) if closed_ranges else '')
                if closed_ranges:
                    # only when the closed guard holds this function's name
                    hit = False
                    for s, e in closed_ranges:
                        piece = text[s:e]
                        m = re.match(r'\?\?[01](\w+)@|\?(\w+)@', r['symbol'])
                        word = m and (m.group(1) or m.group(2))
                        if word and re.search(r'\b%s\s*\(' % re.escape(word), piece):
                            hit = True
                    if not hit:
                        rec.update(bucket='no_draft', detail='')
                if rec['bucket'] == 'no_draft':
                    rec.update(bucket=where_defined(text, r['symbol']))
            else:
                try:
                    bucket, top, direction, detail = classify(audit, coff, fname, r, resolve, names)
                except Exception as e:     # noqa: BLE001
                    bucket, top, direction, detail = 'inline_body', '', '', 'compare failed: %s' % e
                rec.update(bucket=bucket, top_callee=top, direction=direction, detail=detail, function=fname)
            out.append(rec)
    R.write_json(cache, dict(key=key, rows=out))
    return out


def main():
    args = [a for a in sys.argv[1:] if not a.startswith('--')]
    if '--notes' in sys.argv and not args and '--write' not in sys.argv:
        rows = list(csv.DictReader(open(OUT)))
        for r in rows:
            r['bytes'] = int(r['bytes'])
        summary(rows)
        return
    units = R.load_json(R.UNITS, {})
    audit = Audit(R.ROOT, json.loads(R.CONFIG.read_text())['executable'])
    names = {}
    for s, a in R.mischa_names().items():
        names.setdefault(a, s)
    for s, a in R.extra_symbols().items():
        names.setdefault(a, s)
    for u in units.values():
        for r in u['rows']:
            names.setdefault(int(r['entry'], 16), r['symbol'])
    hstate = header_state()
    rows = []
    for unit in (args or sorted(units)):
        got = map_unit(unit, units[unit], audit, names, hstate)
        rows += got
        if got:
            c = collections.Counter(r['bucket'] for r in got)
            print('%-14s %s' % (unit, ' '.join('%s=%d' % kv for kv in sorted(c.items()))), flush=True)

    if '--write' in sys.argv:
        with open(OUT, 'w', newline='') as f:
            w = csv.DictWriter(f, ['unit', 'entry', 'function', 'bytes', 'bucket', 'top_callee', 'direction', 'detail'])
            w.writeheader()
            for r in sorted(rows, key=lambda r: (r['bucket'], r['unit'], r['entry'])):
                w.writerow(r)
    summary(rows)


def summary(rows):
    by = collections.defaultdict(list)
    for r in rows:
        by[r['bucket']].append(r)
    print('\n%-16s %6s %9s  top callees / details (rows, bytes)' % ('bucket', 'rows', 'bytes'))
    for bucket, rs in sorted(by.items(), key=lambda kv: -sum(r['bytes'] for r in kv[1])):
        tops = collections.defaultdict(lambda: [0, 0])
        for r in rs:
            k = (r['top_callee'] + (' [%s]' % r['direction'] if r['direction'] else '')) or '-'
            tops[k][0] += 1; tops[k][1] += r['bytes']
        best = sorted(tops.items(), key=lambda kv: -kv[1][1])[:8]
        print('%-16s %6d %9d  %s' % (bucket, len(rs), sum(r['bytes'] for r in rs),
                                     '; '.join('%s (%d, %d)' % (k, v[0], v[1]) for k, v in best if k != '-')))
    print('%-16s %6d %9d' % ('total', len(rows), sum(r['bytes'] for r in rows)))
    if NOTES.exists():
        notes = {r['bucket']: r for r in csv.DictReader(open(NOTES))}
        print('\nVC6_MATCHING_NOTES sections per bucket (tools/original_target/notes_index.py --section "<title>"):')
        for bucket, rs in sorted(by.items(), key=lambda kv: -sum(r['bytes'] for r in kv[1])):
            n = notes.get(bucket)
            if n:
                print('%-16s %s' % (bucket, n['sections']))


if __name__ == '__main__':
    main()
