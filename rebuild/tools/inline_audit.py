"""inline_audit.py [--family TEXT] [--top N]: the image's evidence on which inline members it keeps out of line
(lane R42, review session direction 2026-11-19, item 2a).

Reads every row of main's catalog, disassembles it, and indexes its direct calls. For every callee family
(Class::Member, template arguments dropped) named in rebuild/manifest/tail_buckets.csv as an inline_call
difference, reports:

  image   direct call sites in the image to any out-of-line copy of the family, split by the calling row's state
          in this tree (exact / parked / outside the rebuild's units)
  copies  the out-of-line copies' addresses
  tail    parked rows where the original calls it and we expand it (orig_calls) and the reverse (ours_calls),
          with their bytes

A family the image calls out of line from exact rows while parked rows expand it is consistent (a budget
case); one the image never calls out of line but parked rows call (ours_calls), or one the image always calls
out of line at depth one, points at the header's definition. Nothing is written."""
import collections
import csv
import hashlib
import json
import sys

import capstone

import rebuild as R
from tailmap import family
from verify_manifest import Audit


def main():
    want = sys.argv[sys.argv.index('--family') + 1] if '--family' in sys.argv else None
    top = int(sys.argv[sys.argv.index('--top') + 1]) if '--top' in sys.argv else 40
    audit = Audit(R.ROOT, json.loads(R.CONFIG.read_text())['executable'])
    names = {}
    for s, a in R.mischa_names().items():
        names.setdefault(a, s)
    for s, a in R.extra_symbols().items():
        names.setdefault(a, s)
    units = R.load_json(R.UNITS, {})
    state = {}
    for unit, info in units.items():
        for r in info['rows']:
            names.setdefault(int(r['entry'], 16), r['symbol'])
        last = R.load_json(R.WORK / 'last' / (unit + '.json'), None)
        st = {r['entry']: r['status'] for r in last['results']} if last else {}
        for r in info['rows']:
            state[int(r['entry'], 16)] = 'exact' if st.get(r['entry']) == 'exact' else 'parked'
    catalog = R.main_catalog()
    for r in catalog['regions']:
        if r.get('symbol'):
            names.setdefault(r['entry'], r['symbol'])
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    sites = collections.defaultdict(collections.Counter)     # target -> Counter(state)
    callers = collections.defaultdict(list)
    for r in catalog['regions']:
        entry, body = r['entry'], r.get('body_bytes') or 0
        if not body:
            continue
        for i in md.disasm(audit.original(entry, body), entry):
            if i.mnemonic == 'call' and i.op_str.startswith('0x'):
                t = int(i.op_str, 16)
                s = state.get(entry, 'outside')
                sites[t][s] += 1
                callers[t].append(entry)
    fam_copies = collections.defaultdict(list)
    for t in sites:
        fam_copies[family(names.get(t))].append(t)
    tail = collections.defaultdict(lambda: collections.Counter())
    tailbytes = collections.defaultdict(lambda: collections.Counter())
    for row in csv.DictReader(open(R.MANIFEST / 'tail_buckets.csv')):
        if row['bucket'] == 'inline_call':
            tail[row['top_callee']][row['direction']] += 1
            tailbytes[row['top_callee']][row['direction']] += int(row['bytes'])
    fams = [f for f in tail if not want or want in f]
    fams.sort(key=lambda f: -sum(tailbytes[f].values()))
    print('%-48s %22s %8s | %-16s %-16s' % ('family', 'image sites ex/pk/out', 'copies', 'orig_calls r/B', 'ours_calls r/B'))
    for f in fams[:top]:
        c = collections.Counter()
        for t in fam_copies.get(f, []):
            c.update(sites[t])
        print('%-48s %22s %8d | %-16s %-16s' % (
            f[:48], '%d/%d/%d' % (c['exact'], c['parked'], c['outside']), len(fam_copies.get(f, [])),
            '%d/%d' % (tail[f]['orig_calls'], tailbytes[f]['orig_calls']),
            '%d/%d' % (tail[f]['ours_calls'], tailbytes[f]['ours_calls'])))
        if want:
            for t in sorted(fam_copies.get(f, [])):
                print('    copy %08x %-70s sites %s' % (t, names.get(t, '')[:70], dict(sites[t])))


if __name__ == '__main__':
    main()
