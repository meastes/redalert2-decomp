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

A rescore sidecar (docs/MISSES_RESCORE.tsv by default, written by a rescore lane: entry, head, score, compiles,
draft, harness, note) overrides the note-derived numbers for the rows it names: `head` shows the score the named
draft reproduces at that commit ('-' when it no longer compiles), the rank uses it, and `stale` marks rows whose
recorded best is above the reproduced score or whose draft does not compile. Rows the sidecar does not name keep
the note numbers.

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

REPOSITORY = Path('/Users/ivalice/Code/redalert2-native')
SECTION = re.compile(r'^## +(\S+) +\((\d{4}-\d{2}-\d{2})?[^)]*\)')
ADDR = re.compile(r'\b0x([0-9A-Fa-f]{5,8})\b')
SCORE = re.compile(r'(?<![\d.,])(\d{1,2}\.\d{2}|100(?:\.00?)?)(?!\d|\.\d|,\d)')
ARROW = re.compile(r'\s*(?:->|→|to)\s*(\d{1,2}\.\d{2}|100(?:\.00?)?)(?!\d|\.\d)')
QUANTIFIED_BEFORE = re.compile(r'\b(?:all|both|each)\s*(?:at\s*)?$')
QUANTIFIED_AFTER = re.compile(r'\s*each\b')
SHAPE = re.compile(r'\b(?:shape|raw)\s*$')
DRAFT = re.compile(r'(?<![\w/.-])((?:build/class-work/|[A-Za-z0-9_.-]*(?:w|keep|best|hold|draft|rd|hm|h1|lab|v|t|d)/)[A-Za-z0-9_./-]*?\.(?:cpp|hpp|c|h))(?![\w])')
BARE = re.compile(r'(?<![\w/.-])([A-Za-z0-9_][A-Za-z0-9_-]*\.(?:cpp|hpp|c|h))(?![\w.])')
LANE = re.compile(r'\b((?:Scout|codex)[0-9][\w-]*)\s+$')
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


def depths(line):
    """Paren depth before each character."""
    out, d = [], 0
    for ch in line:
        if ch == ')':
            d -= 1
        out.append(d)
        if ch == '(':
            d += 1
    return out


def clause_end(line, depth, start, base, stops, commas):
    """End of the clause that starts at `start` at paren depth `base`: a ';' or a sentence '.' at that depth, a ')'
    closing the enclosing parenthesis, or (with `commas`) a ', ' list separator; `stops` cuts it at the next address."""
    for i in range(start, len(line)):
        if i in stops:
            return i
        ch = line[i]
        if depth[i] < base:
            return i
        if depth[i] == base and (ch == ';' or (ch == '.' and (i + 1 == len(line) or line[i + 1].isspace()))
                                 or (commas and ch == ',' and i + 1 < len(line) and line[i + 1] == ' ')):
            return i
    return len(line)


def first_score(text):
    """First score in `text` that is a match score (not a shape/raw score); an arrow chain gives its last value."""
    found = find_score(text)
    return found[0] if found else None


def find_score(text):
    for m in SCORE.finditer(text):
        if SHAPE.search(text[:m.start()]):
            continue
        value, pos = m.group(1), m.end()
        while True:
            a = ARROW.match(text, pos)
            if not a:
                break
            value, pos = a.group(1), a.end()
        return float(value), m.start(), pos
    return None


def drafts_in(text, lane, resolve):
    """Draft paths in `text`, qualified with the lane folder (an explicit lane name just before the path wins).
    A bare file name counts only when `resolve` finds it in that lane's folder."""
    found = []
    for m in DRAFT.finditer(text):
        named = LANE.search(text[:m.start()])
        path = m.group(1)
        if not path.startswith('build/'):
            path = 'build/class-work/%s/%s' % (named.group(1) if named else lane, path)
        found.append((m.start(), path))
    if resolve:
        for m in BARE.finditer(text):
            named = LANE.search(text[:m.start()])
            path = resolve(named.group(1) if named else lane, m.group(1))
            if path:
                found.append((m.start(), path))
    out = []
    for _, path in sorted(found):
        if path not in out:
            out.append(path)
    return out


def lane_resolver(root):
    """Find a bare draft name in build/class-work/<lane>/ or one folder below it."""
    def resolve(lane, name):
        folder = root / 'build/class-work' / lane
        if not folder.is_dir():
            return None
        if (folder / name).is_file():
            return 'build/class-work/%s/%s' % (lane, name)
        for sub in sorted(p for p in folder.iterdir() if p.is_dir()):
            if (sub / name).is_file():
                return 'build/class-work/%s/%s/%s' % (lane, sub.name, name)
        return None
    return resolve


def bind(line, lane, resolve=None):
    """Per address on the line: the score and drafts bound to it without ambiguity.

    A score binds when it is the first match score in the address's own list item (to the next ';', sentence '.',
    ', ' or closing parenthesis) with no other 0x address before it. A draft binds when it is in the address's clause
    (same, without the comma stop) or in the innermost parenthesis around the address when that parenthesis names
    no other address. A score shared by an address list counts for each address only when the note says so
    ("all 98.73", "98.73 each"). A score or draft in the same clause but behind another address is ambiguous: '?'."""
    depth = depths(line)
    addrs = list(ADDR.finditer(line))
    starts = {m.start() for m in addrs}
    out = []
    for m in addrs:
        base = depth[m.start()]
        stops = {s for s in starts if s > m.start()}
        item = line[m.end():clause_end(line, depth, m.end(), base, stops, True)]
        clause = line[m.end():clause_end(line, depth, m.end(), base, stops, False)]
        whole = line[m.end():clause_end(line, depth, m.end(), base, set(), False)]
        score = first_score(item)
        shared = find_score(whole) if score is None else None
        if shared and (QUANTIFIED_BEFORE.search(whole[:shared[1]]) or QUANTIFIED_AFTER.match(whole, shared[2])):
            score, shared = shared[0], None  # "0xA/0xB 98.73 each", "0xA (w/a.cpp), 0xB (w/b.cpp) all 98.73"
        score_q = shared is not None
        drafts = drafts_in(clause, lane, resolve)
        if base > 0:
            lo = max(i for i in range(m.start()) if line[i] == '(' and depth[i] == base - 1)
            hi = next((i for i in range(m.start(), len(line)) if depth[i] < base), len(line))
            if not any(lo < s < hi and s != m.start() for s in starts):
                for d in drafts_in(line[lo:hi], lane, resolve):
                    if d not in drafts:
                        drafts.append(d)
        draft_q = not drafts and bool(drafts_in(whole, lane, resolve))
        out.append((m, score, score_q, drafts, draft_q))
    return out


def load_rescore(path):
    """entry -> dict(head, score, compiles, draft, harness, note) from a rescore TSV; the latest line per entry wins."""
    out = {}
    if not path or not path.exists():
        return out
    lines = path.read_text(errors='replace').splitlines()
    head = [c.strip() for c in lines[0].split('\t')] if lines else []
    for line in lines[1:]:
        if not line.strip():
            continue
        cells = dict(zip(head, line.split('\t')))
        try:
            entry = int(cells.get('entry', ''), 16)
        except ValueError:
            continue
        score = cells.get('head_score') or cells.get('score') or '-'
        out[entry] = dict(head=cells.get('head', ''), score=float(score) if score not in ('', '-') else None,
                          compiles=cells.get('compiles', ''), draft=cells.get('draft', ''),
                          harness=cells.get('harness', ''), note=cells.get('note', ''))
    return out


def build(open_misses, treemap, resolve=None, rescore=None):
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
            for m, score, score_q, drafts, draft_q in bind(line, lane, resolve):
                e = int(m.group(1), 16)
                if e < 0x400000 or e > 0x800000 or (e, line) in seen:
                    continue
                seen.add((e, line))
                hist[e].append(dict(lane=lane, date=date, score=score, score_ambiguous=score_q,
                                    drafts=drafts, draft_ambiguous=draft_q,
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
        # the draft of the first note that records the highest score among notes naming a draft; else the latest draft
        scored = [x for x in h if x['drafts'] and x['score'] is not None and x['score'] <= 100]
        if scored:
            top = max(x['score'] for x in scored)
            draft = next(x for x in scored if x['score'] == top)['drafts'][0]
        else:
            draft = drafts[-1] if drafts else None
        r.update(best=best, best_ambiguous=best is None and any(x['score_ambiguous'] for x in h),
                 attempts=len(lanes), lanes=lanes, last_lane=lanes[-1], last_date=max(x['date'] for x in h),
                 pending=pending, causes=sorted(causes, key=lambda c: -causes[c])[:3], evidence=sorted(evidence),
                 drafts=drafts, draft=draft, draft_ambiguous=draft is None and any(x['draft_ambiguous'] for x in h),
                 rank=(r['bytes'] or 0) * (100 - (best if best is not None else 0)), history=h)
        rs = (rescore or {}).get(e)
        if rs:
            # The reproduced HEAD score ranks the row; a draft that no longer compiles ranks as if unscored.
            r.update(head=rs['head'], head_score=rs['score'], head_compiles=rs['compiles'], head_draft=rs['draft'],
                     head_harness=rs['harness'], head_note=rs['note'],
                     stale=rs['score'] is None or (best is not None and rs['score'] < best - 0.005),
                     rank=(r['bytes'] or 0) * (100 - (rs['score'] if rs['score'] is not None else 0)))
            if rs['draft']:
                r['draft'] = rs['draft'] if rs['draft'].startswith('build/') else 'build/class-work/' + rs['draft']
        out.append(r)
    out.sort(key=lambda r: -r['rank'])
    return out


def short_draft(path):
    return path[len('build/class-work/'):] if path.startswith('build/class-work/') else path


def render(out, markdown):
    head = ['entry', 'bytes', 'best', 'head', 'stale', 'tries', 'last', 'pending', 'causes', 'evidence', 'draft', 'rank']
    lines = []
    for r in out:
        best = ('%.2f' % r['best']) if r['best'] is not None else ('?' if r['best_ambiguous'] else '-')
        if 'head_score' in r:
            head_score = ('%.2f' % r['head_score']) if r['head_score'] is not None else 'broken'
        else:
            head_score = ''
        draft = short_draft(r['draft'])[-48:] if r['draft'] else ('?' if r['draft_ambiguous'] else '')
        lines.append(['0x%06X' % r['entry'], format(r['bytes'], ','), best, head_score, 'yes' if r.get('stale') else '',
                      str(r['attempts']), r['last_lane'], 'yes' if r['pending'] else '', ' '.join(r['causes']),
                      ' '.join(r['evidence']), draft, format(r['rank'] // 100, ',')])
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
    ap.add_argument('--lanes-root', type=Path, default=REPOSITORY,
                    help='checkout whose build/class-work/<lane>/ folders resolve bare draft names (default: this one)')
    ap.add_argument('--rescore', type=Path, default=REPOSITORY / 'docs/MISSES_RESCORE.tsv',
                    help='rescore sidecar TSV (entry, head, score, compiles, draft, harness, note); missing = note numbers only')
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
    false_path = REPOSITORY / 'manifests/original_target/false_rows.json'
    if treemap and false_path.exists():
        false_rows = {int(r['entry'], 16) for r in json.loads(false_path.read_text())['rows']}
        treemap = [r for r in treemap if r['entry'] not in false_rows]
    out = build(text, treemap, lane_resolver(args.lanes_root), load_rescore(args.rescore))
    if args.entry:
        e = int(args.entry, 16)
        for r in out:
            if r['entry'] == e:
                print('0x%06X %s %s bytes, best %s, %d lanes, pending=%s, causes=%s, evidence=%s, draft %s' % (
                    e, r['name'], format(r['bytes'], ','), r['best'] if r['best'] is not None else ('?' if r['best_ambiguous'] else '-'),
                    r['attempts'], r['pending'], r['causes'], r['evidence'], r['draft'] or ('?' if r['draft_ambiguous'] else '-')))
                if 'head_score' in r:
                    print('  rescored at %s: %s (%s), draft %s\n    %s' % (
                        r['head'], r['head_score'] if r['head_score'] is not None else 'does not compile',
                        r['head_note'][:200], r['head_draft'], r['head_harness'][:300]))
                for x in r['history']:
                    print('  %s %s %s  %s' % (x['date'], x['lane'], x['score'] if x['score'] is not None else
                                             ('?' if x['score_ambiguous'] else '-'), x['text'][:220]))
                return
        print('no notes mention 0x%06X' % e)
        return
    if args.json:
        args.json.write_text(json.dumps(out, indent=1))
    if args.markdown:
        args.markdown.write_text('# Per-row miss index (generated by misses_index.py; rank = bytes x (100 - best) / 100)\n\n'
                                 + render(out, True) + '\n')
    print(render(out[:args.top], False))
    print('%d unaccepted rows with notes; %d pending a ruling; %d with 3+ attempts; %d rescored, %d stale' % (
        len(out), sum(1 for r in out if r['pending']), sum(1 for r in out if r['attempts'] >= 3),
        sum(1 for r in out if 'head_score' in r), sum(1 for r in out if r.get('stale'))))


if __name__ == '__main__':
    main()
