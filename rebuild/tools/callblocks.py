"""callblocks.py [UNIT ...] [--write] [--recheck]: callee-first map of the call buckets (lane R44, review session
ordering 2026-11-19).

For every row of rebuild/manifest/tail_buckets.csv in the buckets inline_call, calls_differ and inline_body, the
unit's text (guards open, as tailmap.py compiles it; never the tree's file) is compiled twice: with its options,
and with /Ob0 appended (no inline expansion at all). The /Ob0 object's direct calls of the row's function are
every call its source makes. A callee is expanded

  by the original  when the source calls it and the original row has no direct call to its address;
  by us            when the /Ob0 object calls it and the /Ob2 object does not.

Each expanded callee is looked up among the rows of units.json (any unit) by its bound address:

  exact            the row is exact in its unit's last check (count-sensitive rows read as count_sensitive)
  parked           the row has text that is not exact (tail_buckets bucket given); main has its exact body
  no_draft         the row has no definition in its unit (no_draft, text_if0)
  outside          the address is bound but no rebuild unit has a row for it
  inline_only      the name binds to no address: the image has no out-of-line copy to measure it by

A caller with a callee the original expands that is parked, no_draft or count_sensitive is BLOCKED on it (only exact
rows release it); a callee in a unit or row a Windows worker holds is listed with the worker. --write writes
rebuild/manifest/caller_blocks.csv: one line per blocked caller (unit, entry, function, bytes, bucket,
blockers as 'unit:entry:symbol:status[:worker]' joined by ' | ', carry = blockers in non-worker units).

--recheck (after a landing; compiles nothing): re-reads caller_blocks.csv and the current last checks, and
prints the callers whose blockers are all exact now (released) and those still blocked. Nothing is written.

Results are cached per unit under build/r/callblocks/ by source, header and rows state."""
import collections
import contextlib
import csv
import hashlib
import io
import json
import shutil
import sys

import capstone

import rebuild as R
import tailmap as T
from coff import Coff
from verify_manifest import Audit

VERSION = 'v1'
OUT = R.MANIFEST / 'caller_blocks.csv'
CACHE = R.WORK / 'callblocks'
BUCKETS = ('inline_call', 'calls_differ', 'inline_body')
MD = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)


def compile_variant(unit, info, text, options, tag):
    path = CACHE / 'src' / ('%s.cpp' % unit)
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(text, encoding='latin-1')
    variant = dict(info, source=str(path.relative_to(R.ROOT)), options=options)
    obj, log, _, folder = R.compile_unit(unit + '-callblocks', variant, tag)
    data = obj.read_bytes() if obj is not None else None
    shutil.rmtree(folder, ignore_errors=True)
    return data


def direct_calls(coff, fname):
    sym, section, start, end = coff.function(fname)
    data = section['data']
    out = []
    for offset, index, kind in sorted(section['relocs']):
        if kind != 20 or not start <= offset < end or offset < 1 or data[offset - 1] != 0xE8:
            continue
        n = coff.symbols[index]['name']
        if not n.startswith('$'):
            out.append(n)
    return out


def row_index(units):
    """address -> (unit, row)."""
    out = {}
    for unit, info in units.items():
        for r in info['rows']:
            out.setdefault(int(r['entry'], 16), (unit, r))
    return out


def states(units):
    """(unit, entry) -> exact / count_sensitive / parked:<bucket> / no_draft."""
    buckets = {}
    with open(R.MANIFEST / 'tail_buckets.csv') as f:
        for r in csv.DictReader(f):
            buckets[(r['unit'], r['entry'])] = r['bucket']
    out = {}
    for unit, info in units.items():
        last = R.last_check(unit, info)
        status = {r['entry']: r['status'] for r in last['results']} if last else {}
        sens = info.get('count_sensitive') or {}
        for r in info['rows']:
            e = r['entry']
            if e in sens:
                out[(unit, e)] = 'count_sensitive'
            elif status.get(e) == 'exact':
                out[(unit, e)] = 'exact'
            else:
                b = buckets.get((unit, e), 'unchecked' if not last else 'parked')
                out[(unit, e)] = 'no_draft' if b in ('no_draft', 'text_if0') else 'parked:' + b
    return out


def workers():
    units, rows = {}, {}
    with open(R.MANIFEST / 'workers.csv') as f:
        for r in csv.DictReader(f):
            units[r['unit']] = r['worker']
    with open(R.MANIFEST / 'worker_rows.csv') as f:
        for r in csv.DictReader(f):
            rows[(r['unit'], r['entry'])] = r['worker']
    return units, rows


def map_unit(unit, info, targets, audit, hstate):
    text = (R.ROOT / info['source']).read_text(encoding='latin-1')
    key = hashlib.sha256((VERSION + text + hstate + json.dumps(info['rows'], sort_keys=True) +
                          json.dumps(sorted(targets))).encode('latin-1', 'replace')).hexdigest()
    cache = CACHE / (unit + '.json')
    cached = R.load_json(cache, None)
    if cached and cached.get('key') == key:
        return cached['rows']
    with contextlib.redirect_stdout(io.StringIO()):
        data2, closed = T.compile_open(unit, info, text)
    out = []
    if data2 is None:
        R.write_json(cache, dict(key=key, rows=out))
        return out
    opened = {g[0] for g in T.guards(text)} - set(closed)
    data0 = compile_variant(unit, info, T.open_text(text, opened), list(info['options']) + ['/Ob0'], 'z')
    if data0 is None:
        R.write_json(cache, dict(key=key, rows=[dict(entry=e, error='/Ob0 compile failed') for e in targets]))
        return []
    c2, c0 = Coff(data2), Coff(data0)
    resolve = R.resolver(unit, info)
    names2 = {s['name'] for s in c2._functions}
    names0 = {s['name'] for s in c0._functions}
    byaddr = {}
    for n in names2:
        a = resolve(n)[0]
        if a is not None:
            byaddr.setdefault(a, n)
    for r in info['rows']:
        if r['entry'] not in targets:
            continue
        entry = int(r['entry'], 16)
        fname = r['symbol'] if r['symbol'] in names2 else byaddr.get(entry)
        if fname is None or fname not in names0:
            out.append(dict(entry=r['entry'], error='function not in both objects'))
            continue
        calls0 = direct_calls(c0, fname)
        calls2 = collections.Counter(direct_calls(c2, fname))
        orig = collections.Counter(int(i.op_str, 16) for i in MD.disasm(audit.original(entry, r['body']), entry)
                                   if i.mnemonic == 'call' and i.op_str.startswith('0x'))
        seen = {}
        for n in calls0:
            a = resolve(n)[0]
            by_us = calls2[n] < calls0.count(n)
            by_orig = a is None or orig[a] < calls0.count(n)
            if by_us or by_orig:
                seen[n] = dict(symbol=n, address='%08x' % a if a is not None else '',
                               by_us=by_us, by_orig=by_orig if a is not None else None)
        out.append(dict(entry=r['entry'], function=fname, expanded=list(seen.values())))
    R.write_json(cache, dict(key=key, rows=out))
    return out


def judge(expanded, index, state, wunits, wrows, bysym):
    """[(symbol, unit, entry, status, worker)] of the expanded callees."""
    out = []
    for x in expanded:
        if not x['address']:
            out.append((x['symbol'], '', '', 'inline_only', '', x['by_orig']))
            continue
        a = int(x['address'], 16)
        if a not in index:
            out.append((x['symbol'], '', x['address'], 'outside', '', x['by_orig']))
            continue
        u, r = index[a]
        st = state.get((u, r['entry']), 'unchecked')
        # A select-any copy (template or inline member) is a row of several units under one name: its text is
        # exact when any copy is exact; exact_when_open is exact text kept under a guard for another row.
        if st != 'exact' and any(state.get(k) == 'exact' for k in bysym.get(x['symbol'], ())):
            st = 'exact'
        if st == 'parked:exact_when_open':
            st = 'exact'
        if a in VERIFIED and st != 'exact':
            st = 'exact (verified text)'
        out.append((x['symbol'], u, r['entry'], st, wrows.get((u, r['entry'])) or wunits.get(u, ''), x['by_orig']))
    return out


# Inline or template members whose image copy no unit emits (the row reads no_draft or is held), proven exact text
# by forcing the copy out of line in a scratch compile of the named unit (a member pointer, or a call under
# #pragma inline_depth(0)), compared with the full row: their text is right, so they block no caller.
VERIFIED = {
    0x00423EF0: 'anim: DirType::Direction(Coord const &, Coord const &), face.h (lane R44)',
    0x005940B0: 'missile: DirType::operator+, face.h (lane R44)',
    0x0056AA20: 'map: TPoint2D<short>::operator+=, point.h (lane R44)',
    0x0056BC00: 'map: HashTableClass<unsigned, SubzoneConnectionStruct>::Add_Object(int, ...) (lane R44)',
    0x004A9AF0: 'display: RGBClass::RGBClass(void), rgb.h (lane R44; dropship, codex1, does not compile)',
}

BLOCKING = ('parked', 'no_draft', 'count_sensitive', 'unchecked')


def recheck():
    units = R.load_json(R.UNITS, {})
    state = states(units)
    released, still = [], []
    with open(OUT) as f:
        for r in csv.DictReader(f):
            left = []
            for b in r['blockers'].split(' | '):
                u, e = b.split(':')[:2]
                if state.get((u, e)) not in ('exact', 'parked:exact_when_open') and int(e, 16) not in VERIFIED:
                    left.append(b)
            (still if left else released).append((r, left))
    for r, _ in released:
        print('released %-12s %s %6s %s' % (r['unit'], r['entry'], r['bytes'], r['function'][:80]))
    print('%d released (%d bytes); %d still blocked (%d bytes)' % (
        len(released), sum(int(r['bytes']) for r, _ in released),
        len(still), sum(int(r['bytes']) for r, _ in still)))


def exact_index():
    """build/r/callblocks/exact_expansions.json: for every inline callee with no out-of-line copy in the image, the
    exact rows (any unit) whose /Ob0 object calls it and whose /Ob2 object does not (they expand it and are
    exact, so its text is right in that context). A callee expanded only by parked rows is a step-2 suspect."""
    units = R.load_json(R.UNITS, {})
    out = collections.defaultdict(list)
    for unit, info in sorted(units.items()):
        last = R.last_check(unit, info)
        if not last or not (R.ROOT / info['source']).exists():
            continue
        exact = {r['entry'] for r in last['results'] if r['status'] == 'exact'}
        if not exact:
            continue
        cache = CACHE / 'exact' / (unit + '.json')
        text = (R.ROOT / info['source']).read_text(encoding='latin-1')
        key = hashlib.sha256((VERSION + text + T.header_state()).encode('latin-1', 'replace')).hexdigest()
        got = R.load_json(cache, None)
        if not got or got.get('key') != key:
            got = dict(key=key, rows={})
            data2 = compile_variant(unit, info, text, list(info['options']), 'x')
            data0 = compile_variant(unit, info, text, list(info['options']) + ['/Ob0'], 'y')
            if data2 is not None and data0 is not None:
                c2, c0 = Coff(data2), Coff(data0)
                resolve = R.resolver(unit, info)
                n2 = {s['name'] for s in c2._functions}
                n0 = {s['name'] for s in c0._functions}
                byaddr = {}
                for n in n2:
                    a = resolve(n)[0]
                    if a is not None:
                        byaddr.setdefault(a, n)
                for r in info['rows']:
                    if r['entry'] not in exact:
                        continue
                    f = r['symbol'] if r['symbol'] in n2 else byaddr.get(int(r['entry'], 16))
                    if f is None or f not in n0:
                        continue
                    k2 = collections.Counter(direct_calls(c2, f))
                    got['rows'][r['entry']] = sorted({n for n in direct_calls(c0, f)
                                                      if k2[n] == 0 and resolve(n)[0] is None})
            R.write_json(cache, got)
        for e, names in got['rows'].items():
            for n in names:
                out[n].append('%s:%s' % (unit, e))
        print(unit, len(got['rows']), flush=True)
    R.write_json(CACHE / 'exact_expansions.json', out)


def main():
    if '--recheck' in sys.argv:
        return recheck()
    if '--exact-index' in sys.argv:
        return exact_index()
    args = [a for a in sys.argv[1:] if not a.startswith('--')]
    units = R.load_json(R.UNITS, {})
    audit = Audit(R.ROOT, json.loads(R.CONFIG.read_text())['executable'])
    tail = collections.defaultdict(dict)
    with open(R.MANIFEST / 'tail_buckets.csv') as f:
        for r in csv.DictReader(f):
            if r['bucket'] in BUCKETS:
                tail[r['unit']][r['entry']] = r
    hstate = T.header_state()
    index = row_index(units)
    state = states(units)
    bysym = collections.defaultdict(list)
    for u, info in units.items():
        for r in info['rows']:
            bysym[r['symbol']].append((u, r['entry']))
    wunits, wrows = workers()
    lines = []
    for unit in (args or sorted(tail)):
        got = map_unit(unit, units[unit], set(tail[unit]), audit, hstate)
        for g in got:
            t = tail[unit][g['entry']]
            if 'error' in g:
                lines.append(dict(t, status='error', blockers=g['error'], carry='', expands=''))
                continue
            judged = judge(g['expanded'], index, state, wunits, wrows, bysym)
            # Only a callee the original expands puts its text inside the caller's bytes; one only we expand
            # (the original calls it) is the caller's own inline_call difference, listed under expands.
            blockers = [j for j in judged if j[3].split(':')[0] in BLOCKING and j[5] and j[0] != g['function']]
            lines.append(dict(t, status='blocked' if blockers else 'free',
                              blockers=' | '.join('%s:%s:%s:%s%s' % (j[1], j[2], j[0], j[3], (':' + j[4]) if j[4] else '')
                                                  for j in blockers),
                              carry=' | '.join('%s:%s' % (j[1], j[2]) for j in blockers if not j[4]),
                              expands=' | '.join('%s=%s%s' % (j[0], j[3], '' if j[5] else '(ours only)') for j in judged)))
        print('%-14s %s' % (unit, ' '.join('%s %s' % (l['entry'], l['status']) for l in lines if l['unit'] == unit)),
              flush=True)
    c = collections.defaultdict(lambda: [0, 0])
    for l in lines:
        c[l['status']][0] += 1; c[l['status']][1] += int(l['bytes'])
    for k, v in sorted(c.items()):
        print('%-8s %4d rows %7d bytes' % (k, v[0], v[1]))
    if '--write' in sys.argv:
        with open(OUT, 'w', newline='') as f:
            w = csv.DictWriter(f, ['unit', 'entry', 'function', 'bytes', 'bucket', 'status', 'blockers', 'carry'])
            w.writeheader()
            for l in sorted(lines, key=lambda l: (l['status'] != 'blocked', l['unit'], l['entry'])):
                if l['status'] == 'blocked':
                    w.writerow({k: l[k] for k in w.fieldnames})
    scratch = CACHE / 'all.json'
    R.write_json(scratch, lines)


if __name__ == '__main__':
    main()
