#!/usr/bin/env python3
"""Write an objdiff-format progress report (version 2 JSON) for ByteMatch, and optionally upload it.

ByteMatch (https://bytematch.dev) reads objdiff reports. Our build needs the private target and the
VC6 toolchain, so it cannot run in GitHub Actions; this tool converts the landing numbers instead and
posts them through ByteMatch's upload API.

Input is build/treemap.json (treemap.py --json, last written at src-final; frozen) plus the tree's
catalog and units.json. The report holds only names, addresses, sizes and match percentages; no
bytes from the target. Scope matches STATUS "In-scope functions matched": game code below the C
runtime, handwritten-assembly ranges, evidenced inline-asm regions and known false rows excluded. Accepted handwritten assembly
procedures count as matched, as they do in the treemap.

One unit per unit of the tree (manifests/units.json, source src/<unit>.cpp); in-scope functions in no
unit go to unplaced/<treemap group>. Matched = catalogued in manifests/catalog; count-sensitive rows are not
matched. Library and handwritten assembly stay out of scope.

Fuzzy (lane O90, user 2026-11-25: "ByteMatch supports fuzzy match so we should report that number too"): each
function not exact carries fuzzy_match_percent = (body bytes - differing bytes) / body bytes, clamped to [0, 100],
from its best measured draft, the first that applies: the unit's last check of its current source
(build/r/last/<unit>.json); the measured "N of M bytes differ" of its RA2_NON_MATCHING guard label; units.json
parked_rows best_differing; a last check of an older source (marked so); else 0 (no draft, or no measurement).
Only counts measured against the function's image-derived bound (its units.json row, else image_bounds.csv) are
used; a draft with no bound scores 0. Units, categories and the whole report weight it by bytes, so the
project's figure is ByteMatch's "Fuzzy match" and STATUS's "fuzzy (best draft scores)" line (progress.csv
fuzzy_best_draft). Where each function's score came from is written beside the report
(<report>-fuzzy-sources.json, not uploaded).

The in-scope function set and byte denominator are src-final's, so the series stays comparable: the
frozen treemap (build/treemap.json, written by treemap.py at src-final) and the in-scope row of
docs/history/SRC_FINAL_STATUS.md. The earlier src/ series (version ra2) had its last upload at src-final;
since the layout move (lane O88, 2026-11-25) there is one tree and one series (version ra2-rebuild).

    python3 tools/original_target/bytematch_report.py                  # build/bytematch/rebuild-report.json
    python3 tools/original_target/bytematch_report.py --upload         # token from private/bytematch.token
"""
import argparse
import hashlib
from collections import Counter, defaultdict
import gzip
import json
import os
import re
from pathlib import Path
import subprocess
import sys
import urllib.request

sys.path.insert(0, str(Path(__file__).resolve().parent))
import catalog_io  # noqa: E402
import status  # noqa: E402

REPOSITORY = Path(__file__).resolve().parents[2]
PROJECT = 'meastes/redalert2-decomp'
VERSION = 'ra2-rebuild'                # the series name kept from the two-tree days
SRC_FINAL_STATUS = REPOSITORY / 'docs/history/SRC_FINAL_STATUS.md'
TOKEN_FILE = REPOSITORY / 'private/bytematch.token'  # ignored by git; never stage it
API = 'https://bytematch.dev/api/v1/projects/{project}/reports?version={version}&commit={commit}'

CATEGORIES = [
    ('classes', 'Classes'),
    ('free', 'Free functions'),
    ('statics', 'Header statics and templates'),
    ('other', 'Other units'),
]


def category(row):
    system = row.get('system', '')
    if system.startswith('header statics'):
        return 'statics'
    if system.startswith('free functions'):
        return 'free'
    if system.startswith('other units') or system.startswith('unattributed'):
        return 'other'
    return 'classes'


def empty():
    return dict(fuzzy_match_percent=0.0, total_code=0, matched_code=0, matched_code_percent=0.0,
                total_data=0, matched_data=0, matched_data_percent=0.0, total_functions=0,
                matched_functions=0, matched_functions_percent=0.0, complete_code=0,
                complete_code_percent=0.0, complete_data=0, complete_data_percent=0.0,
                total_units=0, complete_units=0)


def add(total, part):
    total['fuzzy_match_percent'] += part['fuzzy_match_percent'] * part['total_code']
    for key in ('total_code', 'matched_code', 'total_functions', 'matched_functions', 'complete_code',
                'total_units', 'complete_units'):
        total[key] += part[key]


def finish(m, weighted=True):
    if weighted:
        m['fuzzy_match_percent'] = m['fuzzy_match_percent'] / m['total_code'] if m['total_code'] else 100.0
    pct = lambda a, b: 100.0 * a / b if b else 100.0  # noqa: E731
    m['matched_code_percent'] = pct(m['matched_code'], m['total_code'])
    m['matched_functions_percent'] = pct(m['matched_functions'], m['total_functions'])
    m['complete_code_percent'] = pct(m['complete_code'], m['total_code'])
    m['matched_data_percent'] = m['complete_data_percent'] = 100.0
    return m


def wire(m):
    """objdiff's JSON: snake_case, 64-bit counts as strings, percents as numbers."""
    return {k: (v if isinstance(v, float) else str(v)) for k, v in m.items()}


def load_catalog():
    return catalog_io.load(catalog_io.locate(REPOSITORY), references=False)


def asm_ranges(catalog):
    return [(a, b) for a, b, _ in status.HANDWRITTEN] + [
        (r['entry'], r['entry'] + r['body_bytes']) for r in catalog['regions']
        if 'entry' in r and status.evidenced_inline_asm(r)]


def in_scope(rows, catalog):
    """STATUS's in-scope set: below the CRT, outside handwritten ranges and accepted evidenced
    inline-asm regions (status.ledger counts both as assembly), known false rows excluded."""
    asm = asm_ranges(catalog)
    false_path = REPOSITORY / 'manifests/original_target/false_rows.json'
    false_rows = {int(r['entry'], 16) for r in json.loads(false_path.read_text())['rows']} if false_path.exists() else set()
    return [r for r in rows if r['entry'] < status.CRT_START and r['entry'] not in false_rows
            and not status.inside(r['entry'], asm)]


def status_in_scope():
    """src-final's in-scope matched and total C/C++ bytes (its frozen STATUS table)."""
    for line in SRC_FINAL_STATUS.read_text().splitlines():
        if line.startswith('| In-scope matched C/C++ bytes'):
            found = re.search(r'\| ([\d,]+) of ([\d,]+) \(', line)
            return tuple(int(x.replace(',', '')) for x in found.groups())
    sys.exit('%s has no in-scope bytes row' % SRC_FINAL_STATUS)


def reconcile(units, matched=None):
    """In-scope bytes the treemap places in no function (non-contiguous or uninventoried bodies), as one
    unit with no functions, so the report's byte totals equal STATUS's in-scope C/C++ row. Rebuild mode
    passes its own in-scope matched body bytes; the total stays main's."""
    main_matched, total = status_in_scope()
    matched = main_matched if matched is None else matched
    m = empty()
    m['total_code'] = total - sum(u['m']['total_code'] for u in units)
    m['matched_code'] = matched - sum(u['m']['matched_code'] for u in units)
    if m['total_code'] < 0 or not 0 <= m['matched_code'] <= m['total_code']:
        sys.exit('treemap and STATUS disagree (%d/%d left over): regenerate both at the same landing'
                 % (m['matched_code'], m['total_code']))
    m['fuzzy_match_percent'] = 100.0 * m['matched_code'] / m['total_code'] if m['total_code'] else 100.0
    m['total_units'] = 1
    finish(m, weighted=False)
    unit = dict(name='unplaced/outside_inventoried_functions', measures=wire(m),
                sections=[dict(name='.text', size=str(m['total_code']), fuzzy_match_percent=m['fuzzy_match_percent'])],
                functions=[], metadata=dict(complete=False, progress_categories=['other'], auto_generated=True))
    return dict(unit=unit, m=m, cat='other')


def make_unit(name, rows, source, is_exact, score_of):
    """One objdiff unit from treemap rows: exact rows matched, others at score_of(row)."""
    m = empty()
    functions = []
    for r in rows:
        exact = is_exact(r)
        score = 100.0 if exact else score_of(r)
        m['total_code'] += r['bytes']
        m['total_functions'] += 1
        m['fuzzy_match_percent'] += score * r['bytes']
        if exact:
            m['matched_code'] += r['bytes']
            m['matched_functions'] += 1
        functions.append(dict(name=r['name'] or 'FUN_%08x' % r['entry'], size=str(r['bytes']),
                              fuzzy_match_percent=score,
                              metadata=dict(virtual_address=str(r['entry'])),
                              address=str(r['entry'])))
    complete = m['matched_functions'] == m['total_functions']
    m['total_units'] = 1
    m['complete_units'] = int(complete)
    m['complete_code'] = m['total_code'] if complete else 0
    finish(m)
    cat = category(rows[0])
    metadata = dict(complete=complete, progress_categories=[cat], auto_generated=False)
    if source:
        metadata['source_path'] = source
    return dict(unit=dict(name=name, measures=wire(m), sections=[dict(
        name='.text', size=str(m['total_code']), fuzzy_match_percent=m['fuzzy_match_percent'])],
        functions=functions, metadata=metadata), m=m, cat=cat)


def assemble(units):
    total = empty()
    per_cat = {cid: empty() for cid, _ in CATEGORIES}
    for u in units:
        add(total, u['m'])
        add(per_cat[u['cat']], u['m'])
    finish(total)
    categories = []
    for cid, label in CATEGORIES:
        if per_cat[cid]['total_units']:
            categories.append(dict(id=cid, name=label, measures=wire(finish(per_cat[cid]))))
    return dict(measures=wire(total), units=[u['unit'] for u in units], version=2, categories=categories), total


GUARD = re.compile(r'^#ifdef RA2_NON_MATCHING\b[^\n]*', re.M)
DEF = re.compile(r'^[A-Za-z_][^\n;{}()]*?\b((?:[A-Za-z_]\w*::)*~?[A-Za-z_]\w*|operator\s*[^\s(]+)\s*\(', re.M)
# Where a fuzzy score came from, best first (lane O90, user 2026-11-25: "ByteMatch supports fuzzy match so we should
# report that number too"). Every score is a measured count of differing bytes against the function's
# image-derived bound (its units.json row, or image_bounds.csv for a function with no row yet).
SOURCES = ('check', 'label', 'parked', 'check (older source)', 'none')


def fuzzy_percent(body, differing):
    """ByteMatch's fuzzy_match_percent of one function: (body bytes - differing bytes) / body bytes, clamped to
    [0, 100]. `differing` is rebuild.compare's count (bytes that differ by position plus the length difference);
    a function with no bound (body 0 or None) or no measurement (differing None) scores 0."""
    if not body or body <= 0 or differing is None:
        return 0.0
    return max(0.0, min(100.0, 100.0 * (body - differing) / body))


def label_differing(label):
    """(differing bytes, [bounds the label states]) of an RA2_NON_MATCHING guard label, or (None, []).
    A count "by position" (rebuild.compare's measure) wins over an instruction-aligned one in the same label."""
    found = re.search(r'\((\d[\d,]*) by position\b', label) or \
        re.search(r'(\d[\d,]*) (?:of \d[\d,]* )?bytes differ', label)
    if not found:
        return None, []
    stated = []
    for pattern in (r'(?:\d[\d,]*) of (\d[\d,]*) bytes differ', r'bound[^()]{0,30}\((\d[\d,]*) (?:body bytes|B)\b',
                    r'(\d[\d,]*) body bytes', r'(\d[\d,]*)-byte (?:original )?body', r', (\d[\d,]*)\+\d+, in image_bounds'):
        stated += [int(m.replace(',', '')) for m in re.findall(pattern, label)]
    return int(found.group(1).replace(',', '')), stated


def guards(text):
    """[(label, guarded text)] of every RA2_NON_MATCHING block of a source file (nested #if blocks kept inside)."""
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


def guard_entry(label, body, rows, bounded):
    """The entry a guarded draft is measured at: an address the label names as the bound ("bound 00535bf0",
    "differ at 005c3ff0", "(00445520 has no row)") when it is a row or an image-derived bound; else the first
    function defined in the block (helpers set apart) whose name is one of `rows` (the unit's rows and its queued
    functions with no row: dicts with 'entry' and 'symbol'); else a "RA2: 0x..." line near the top of the block.
    None when none applies. Other addresses in a label (callees, sibling rows) never place it."""
    from guard import demangle
    for found in re.finditer(r'bound (?:0x)?([0-9A-Fa-f]{8})\b|differ at (?:0x)?([0-9A-Fa-f]{8})\b|'
                             r'\(([0-9a-f]{8}) has no row\)', label):
        entry = int(next(g for g in found.groups() if g), 16)
        if entry in bounded:
            return entry
    names = {}
    for row in rows:
        q = demangle(row['symbol']) or row['symbol']
        names.setdefault(q, int(row['entry'], 16))
        names.setdefault(q.split('::')[-1], int(row['entry'], 16))
    defs = []
    for fm in list(DEF.finditer(body)) + list(re.finditer(r'^[ \t]*STDMETHOD_?\(\s*(?:\w+\s*,\s*)?(\w+)\)\s*\(', body, re.M)):
        line = body[fm.start():body.find('\n', fm.end()) if '\n' in body[fm.end():] else len(body)]
        if re.sub(r'//.*', '', line).rstrip().endswith(';'):
            continue
        defs.append((fm.group(1), bool(re.match(r'\s*((static\s+)?inline\b|static\b)', line))))
    for name, helper in sorted(defs, key=lambda d: d[1]):
        entry = names.get(name, names.get(name.split('::')[-1]))
        if entry is not None:
            return entry
    found = re.search(r'RA2: 0x([0-9A-Fa-f]{6,8})', body[:400])
    if found and int(found.group(1), 16) in bounded:
        return int(found.group(1), 16)
    return None


def guard_scores(worktree, units, bounds):
    """{entry: differing} from the guard labels of the tree's sources: a label's count is used only when the
    function has an image-derived bound and every bound the label states is that bound; a label that states no
    bound counts only for a function with a units.json row (the tree's checks compare against the row's bound)."""
    import csv
    queued = defaultdict(list)                    # functions with no row yet, by unit (named by nodraft_queue.py)
    queue = worktree / 'manifests/no_draft_queue.csv'
    for r in (csv.DictReader(open(queue, encoding='utf-8', errors='replace')) if queue.exists() else []):
        if r.get('entry') and r.get('symbol'):
            queued[r['unit']].append(dict(entry=r['entry'], symbol=r['symbol']))
    out = {}
    for unit, info in units.items():
        source = worktree / info['source']
        if not source.exists():
            continue
        row_entries = {int(r['entry'], 16) for r in info['rows']}
        for label, body in guards(source.read_text(encoding='latin-1')):
            differing, stated = label_differing(label)
            if differing is None:
                continue
            entry = guard_entry(label, body, info['rows'] + queued[unit], bounds)
            if entry is None or entry not in bounds:
                continue
            if stated and bounds[entry] not in stated:
                continue                  # measured against another extent (main's map, a candidate's size)
            if not stated and entry not in row_entries and not re.search(r'image.?(?:derived )?bound|image_bounds', label):
                continue
            out[entry] = min(differing, out.get(entry, differing))
    return out


def rebuild_scores(worktree):
    """(unit of each entry, {entry: (fuzzy percent, source)}, count-sensitive entries) from the tree's units.json,
    its last checks (build/r/last) and its guard labels. Per function, the first that applies (SOURCES): the
    unit's last check of its current source; the guard label's measured count; units.json parked_rows
    best_differing; a last check of an older source of the unit (marked so); else 0. Each only when measured
    against the function's image-derived bound (units.json row body, else image_bounds.csv)."""
    import csv
    units = json.loads((worktree / 'manifests/units.json').read_text())
    owner, sensitive, bounds = {}, set(), {}
    image = worktree / 'manifests/image_bounds.csv'
    for r in (csv.DictReader(open(image)) if image.exists() else []):
        if r['status'] == 'bound':
            bounds[int(r['entry'], 16)] = int(r['body_bytes'])
    for unit, info in units.items():
        for row in info['rows']:
            owner[int(row['entry'], 16)] = unit
            bounds[int(row['entry'], 16)] = row['body']
        sensitive |= {int(e, 16) for e in info.get('count_sensitive') or {}}
    current, older = {}, {}
    for unit, info in units.items():
        last_path = worktree / 'build/r/last' / (unit + '.json')
        source = worktree / info['source']
        if not last_path.exists() or not source.exists():
            continue
        last = json.loads(last_path.read_text())
        into = current if last.get('source_sha256') == hashlib.sha256(source.read_bytes()).hexdigest() else older
        for r in last['results']:
            entry = int(r['entry'], 16)
            if r.get('body') != bounds.get(entry):
                continue
            if r['status'] == 'exact':
                into[entry] = 0
            elif r['status'] == 'differs' and r.get('differing') is not None:
                into[entry] = r['differing']
    labels = guard_scores(worktree, units, bounds)
    parked = {}
    for unit, info in units.items():
        for row in info.get('parked_rows') or []:
            entry = int(row['entry'], 16)
            if row.get('best_differing') is not None and row.get('body') and row['body'] == bounds.get(entry):
                parked[entry] = row['best_differing']
    score = {}
    for entry in set(current) | set(labels) | set(parked) | set(older):
        for name, found in zip(SOURCES, (current, labels, parked, older)):
            if entry in found:
                score[entry] = (fuzzy_percent(bounds[entry], found[entry]), name)
                break
    return units, owner, score, sensitive


def build_rebuild(treemap, worktree):
    """The tree over src-final's in-scope function set and denominator (see the module docstring)."""
    catalog = load_catalog()
    rebuilt = catalog_io.load(worktree / 'manifests/catalog', references=False)
    accepted = {r['entry'] for r in rebuilt['regions']}
    units_info, owner, score, sensitive = rebuild_scores(worktree)
    rows = in_scope(treemap, catalog)
    groups = defaultdict(list)
    for r in rows:
        unit = owner.get(r['entry'])
        groups[('rebuild', unit) if unit else ('unplaced', r['group'])].append(r)
    is_exact = lambda r: r['entry'] in accepted and r['entry'] not in sensitive  # noqa: E731
    score_of = lambda r: score.get(r['entry'], (0.0, 'none'))[0]  # noqa: E731
    units = []
    for (kind, key), group_rows in sorted(groups.items()):
        group_rows.sort(key=lambda r: r['entry'])
        if kind == 'rebuild':
            units.append(make_unit(key, group_rows, units_info[key]['source'], is_exact, score_of))
        else:
            units.append(make_unit('unplaced/%s' % key, group_rows, None, is_exact, score_of))
    # Rebuild in-scope matched body bytes by main's ledger scope (CRT, handwritten ranges and evidenced
    # inline asm out; false rows are not catalogued bodies of real functions, so they are left out too).
    asm = asm_ranges(catalog)
    # Evidenced inline asm by main's flags, so the scope matches the in-scope rows above (the rebuild
    # flags more rows, e.g. through file-static helpers; those stay in scope here as main counts them).
    main_asm = {r['entry'] for r in catalog['regions'] if status.evidenced_inline_asm(r)}
    false_path = REPOSITORY / 'manifests/original_target/false_rows.json'
    false_rows = {int(r['entry'], 16) for r in json.loads(false_path.read_text())['rows']} if false_path.exists() else set()
    matched = sum(r['body_bytes'] for r in rebuilt['regions']
                  if r['entry'] < status.CRT_START and not status.inside(r['entry'], asm)
                  and r['entry'] not in false_rows and r['entry'] not in sensitive
                  and r['entry'] not in main_asm)
    units.append(reconcile(units, matched))
    # Where each in-scope function that is not exact took its fuzzy score from (not uploaded; the report's
    # sidecar and the print line).
    sources = []
    for r in rows:
        if not is_exact(r):
            pct, name = score.get(r['entry'], (0.0, 'none'))
            sources.append(dict(entry='%08x' % r['entry'], name=r['name'] or '', bytes=r['bytes'],
                                fuzzy=round(pct, 3), source=name))
    report, total = assemble(units)
    return report, total, sources


def fuzzy_figure(worktree=REPOSITORY, treemap=None):
    """The tree's fuzzy figure (ByteMatch's whole-project fuzzy_match_percent, best draft scores) for STATUS and
    progress.csv, or None when the frozen treemap is missing."""
    path = Path(treemap or Path(worktree) / 'build/treemap.json')
    if not path.exists():
        path = REPOSITORY / 'build/treemap.json'
    if not path.exists():
        return None
    return build_rebuild(json.loads(path.read_text()), Path(worktree))[1]['fuzzy_match_percent']


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--treemap', type=Path, default=REPOSITORY / 'build/treemap.json')
    ap.add_argument('--worktree', type=Path, default=REPOSITORY, help='checkout to report (default: this one)')
    ap.add_argument('--out', type=Path)
    ap.add_argument('--upload', action='store_true', help='POST to ByteMatch (token in $BYTEMATCH_TOKEN)')
    ap.add_argument('--commit', help='commit the report describes (default: the checkout\'s HEAD)')
    ap.add_argument('--version')
    args = ap.parse_args()
    args.out = args.out or REPOSITORY / 'build/bytematch/rebuild-report.json'
    args.version = args.version or VERSION

    treemap = json.loads(args.treemap.read_text())
    if not treemap:
        sys.exit('empty treemap: regenerate it with treemap.py --json build/treemap.json')
    report, total, sources = build_rebuild(treemap, args.worktree)
    args.out.parent.mkdir(parents=True, exist_ok=True)
    data = json.dumps(report, separators=(',', ':')).encode()
    args.out.write_bytes(data)
    args.out.with_name(args.out.stem + '-fuzzy-sources.json').write_text(json.dumps(sources, indent=1) + '\n')
    by = Counter()
    for s in sources:
        by[s['source'], 'n'] += 1
        by[s['source'], 'b'] += s['bytes']
    print('fuzzy sources of the %d functions not exact: %s' % (len(sources), '; '.join(
        '%s %d / %d B' % (name, by[name, 'n'], by[name, 'b']) for name in SOURCES if by[name, 'n'])))
    print('%s: %d units, %d/%d functions, %d/%d code bytes (%.2f%%), fuzzy %.2f%%' % (
        args.out, total['total_units'], total['matched_functions'], total['total_functions'],
        total['matched_code'], total['total_code'], total['matched_code_percent'], total['fuzzy_match_percent']))

    if args.upload:
        token = os.environ.get('BYTEMATCH_TOKEN')
        if not token and TOKEN_FILE.exists():
            token = TOKEN_FILE.read_text().strip()
        if not token:
            sys.exit('put the ByteMatch upload token in %s (or $BYTEMATCH_TOKEN)' % TOKEN_FILE)
        commit = args.commit or subprocess.check_output(
            ['git', 'rev-parse', 'HEAD'], cwd=args.worktree, text=True).strip()
        request = urllib.request.Request(
            API.format(project=PROJECT, version=args.version, commit=commit), data=gzip.compress(data),
            method='POST', headers={'Authorization': 'Bearer ' + token, 'Content-Type': 'application/json',
                                    'Content-Encoding': 'gzip', 'User-Agent': 'redalert2-decomp-report'})
        with urllib.request.urlopen(request) as response:
            print('uploaded for %s: HTTP %d %s' % (commit[:9], response.status, response.read().decode()[:300]))


if __name__ == '__main__':
    main()
