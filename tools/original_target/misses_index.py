#!/usr/bin/env python3
"""Per-row ledger of unaccepted functions, generated from docs/OPEN_MISSES.md.

OPEN_MISSES.md is written per lane; this reads every lane section and turns it
inside out: one row per unaccepted function with its best recorded score, the
lanes that attempted it (count and the latest), the drafts named beside it, the
cause words the notes use for its last difference, whether a ruling is pending
and whether archive/twin evidence is mentioned. Bytes and status come from the
treemap table (tools/original_target/treemap.py --json) so the ranking column
is bytes x (100 - best score), the pass order the owner uses.

Nothing here is acceptance evidence; it is an index of the notes.

    python3 tools/original_target/misses_index.py                      # table to stdout
    python3 tools/original_target/misses_index.py --json build/misses-index.json --markdown build/misses-index.md
    python3 tools/original_target/misses_index.py --entry 0x4370E0      # one row's full history
"""
import argparse
import json
import re
import sys
from collections import OrderedDict, defaultdict
from pathlib import Path

REPOSITORY = Path(__file__).resolve().parents[2]
SECTION = re.compile(r'^## +(\S+) +\((\d{4}-\d{2}-\d{2})?[^)]*\)')
ADDR = re.compile(r'\b0x([0-9A-Fa-f]{5,8})\b')
SCORE = re.compile(r'(?<![\d.])(\d{1,2}\.\d{2}|100(?:\.00?)?)(?![\d.])')
ARROW = re.compile(r'(\d{1,2}\.\d{2})\s*(?:->|→|to)\s*(\d{1,2}\.\d{2}|100)')
DRAFT = re.compile(r'\b((?:build/class-work/\S+?|[A-Za-z0-9_./-]*(?:w|keep|best|hold|draft|rd|hm|h1|lab|v|t|d)/[A-Za-z0-9_./-]+\.(?:cpp|hpp|c|h)))\b')
CAUSES = OrderedDict([
    ('ruling', r'PENDING|ruling|OWNER DECISION'),
    ('layout', r'\blayout\b|\bheader\b|\bstruct\b|\bmember\b|\boffset\b'),
    ('frame', r'\bframe\b|\bslot\b|\bspill\b'),
    ('register', r'\bregister\b|\btie\b|\be[abcd]x\b|\be[sd]i\b|\bebp\b|\bswap\b'),
    ('inline', r'\binline\b|\bbudget\b|\bout of line\b|\bout-of-line\b'),
    ('order', r'\border\b|\bschedul'),
    ('bound', r'\bbound\b|\bextent\b'),
    ('asm', r'__asm|\basm\b'),
    ('switch', r'\bswitch\b|jump table|jump-table'),
    ('float', r'\bfld\b|\bfstp\b|\bdouble\b|\bfloat\b'),
])
EVIDENCE = OrderedDict([('twin', r'\btwin\b|ra2-decomp'), ('tibsun', r'TibSun|archive')])
STATE = OrderedDict([('parked', r'\bparked\b|\bpark\b'), ('held', r'\bheld\b|\bHELD\b|\bhold\b'),
                     ('exact', r'\bexact\b|\blanded\b|\bqueued\b')])


def sections(text):
    lane, date, buf = None, None, []
    for line in text.splitlines():
        m = SECTION.match(line)
        if m:
            if lane:
                yield lane, date, buf
            lane, date, buf = m.group(1), m.group(2) or '', [line]
        elif lane:
            buf.append(line)
    if lane:
        yield lane, date, buf


def scores_near(line, pos):
    window = line[pos:pos + 90]
    m = ARROW.search(window)
    if m:
        return float(m.group(2))
    m = SCORE.search(window)
    return float(m.group(1)) if m else None


def build(open_misses, treemap):
    rows = {}
    if treemap:
        for r in treemap:
            if r['status'] != 'exact':
                rows[r['entry']] = dict(entry=r['entry'], bytes=r['bytes'], status=r['status'],
                                        name=r.get('name') or '', group=r.get('group') or '')
    hist = defaultdict(list)
    for lane, date, buf in sections(open_misses):
        seen = set()
        for line in buf:
            for m in ADDR.finditer(line):
                e = int(m.group(1), 16)
                if e < 0x400000 or e > 0x800000 or (e, line) in seen:
                    continue
                seen.add((e, line))
                hist[e].append(dict(lane=lane, date=date, score=scores_near(line, m.end()),
                                    drafts=sorted(set(DRAFT.findall(line))),
                                    causes=[k for k, rx in CAUSES.items() if re.search(rx, line)],
                                    evidence=[k for k, rx in EVIDENCE.items() if re.search(rx, line)],
                                    state=[k for k, rx in STATE.items() if re.search(rx, line)],
                                    text=line.strip()[:400]))
    out = []
    for e, h in hist.items():
        if treemap and e not in rows:
            continue                       # accepted, or not a function entry
        r = dict(rows.get(e) or dict(entry=e, bytes=0, status='?', name='', group=''))
        scores = [x['score'] for x in h if x['score'] is not None and x['score'] <= 100]
        lanes = []
        for x in h:
            if x['lane'] not in lanes:
                lanes.append(x['lane'])
        causes = defaultdict(int); evidence = set(); drafts = []; pending = False
        for x in h:
            for c in x['causes']:
                causes[c] += 1
            evidence.update(x['evidence'])
            for d in x['drafts']:
                if d not in drafts:
                    drafts.append(d)
            if 'PENDING' in x['text']:
                pending = True
        best = max(scores) if scores else None
        r.update(best=best, attempts=len(lanes), lanes=lanes, last_lane=lanes[-1], last_date=max(x['date'] for x in h),
                 pending=pending, causes=sorted(causes, key=lambda c: -causes[c])[:3], evidence=sorted(evidence),
                 drafts=drafts[-3:], rank=(r['bytes'] or 0) * (100 - (best if best is not None else 0)),
                 history=h)
        out.append(r)
    out.sort(key=lambda r: -r['rank'])
    return out


def render(out, markdown):
    head = ['entry', 'bytes', 'best', 'tries', 'last', 'pending', 'causes', 'evidence', 'draft', 'rank']
    lines = []
    for r in out:
        lines.append(['0x%06X' % r['entry'], format(r['bytes'], ','), ('%.2f' % r['best']) if r['best'] is not None else '-',
                      str(r['attempts']), r['last_lane'], 'yes' if r['pending'] else '', ' '.join(r['causes']),
                      ' '.join(r['evidence']), (r['drafts'][-1] if r['drafts'] else '')[-48:], format(r['rank'] // 100, ',')])
    if markdown:
        s = ['| ' + ' | '.join(head) + ' |', '|' + '---|' * len(head)]
        s += ['| ' + ' | '.join(c.replace('|', '/') for c in cells) + ' |' for cells in lines]
        return '\n'.join(s)
    widths = [max(len(c) for c in col) for col in zip(head, *lines)] if lines else [len(h) for h in head]
    fmt = '  '.join('%%-%ds' % w for w in widths)
    return '\n'.join([fmt % tuple(head)] + [fmt % tuple(c) for c in lines])


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--treemap', type=Path, default=REPOSITORY / 'build/treemap.json', help='treemap.py --json output (bytes, status)')
    ap.add_argument('--json', type=Path, help='write the full index (with per-lane history) here')
    ap.add_argument('--markdown', type=Path, help='write the ranked table as markdown here')
    ap.add_argument('--entry', help='print one row with its full history')
    ap.add_argument('--top', type=int, default=60, help='rows to print on stdout')
    args = ap.parse_args()
    text = (REPOSITORY / 'docs/OPEN_MISSES.md').read_text(errors='replace')
    treemap = None
    if args.treemap.exists():
        d = json.loads(args.treemap.read_text())
        treemap = d if isinstance(d, list) else d.get('functions', d)
    else:
        print('no treemap table at %s: bytes and status unknown, accepted rows not filtered' % args.treemap, file=sys.stderr)
    out = build(text, treemap)
    if args.entry:
        e = int(args.entry, 16)
        for r in out:
            if r['entry'] == e:
                print('0x%06X %s %s bytes, best %s, %d lanes, pending=%s, causes=%s, evidence=%s' % (
                    e, r['name'], format(r['bytes'], ','), r['best'], r['attempts'], r['pending'], r['causes'], r['evidence']))
                for x in r['history']:
                    print('  %s %s %s  %s' % (x['date'], x['lane'], x['score'] if x['score'] is not None else '-', x['text'][:220]))
                return
        print('no notes mention 0x%06X' % e)
        return
    if args.json:
        args.json.write_text(json.dumps(out, indent=1))
    if args.markdown:
        args.markdown.write_text('# Per-row miss index (generated by misses_index.py; rank = bytes x (100 - best) / 100)\n\n'
                                 + render(out, True) + '\n')
    print(render(out[:args.top], False))
    print('%d unaccepted rows with notes; %d pending a ruling; %d with 3+ attempts' % (
        len(out), sum(1 for r in out if r['pending']), sum(1 for r in out if r['attempts'] >= 3)))


if __name__ == '__main__':
    main()
