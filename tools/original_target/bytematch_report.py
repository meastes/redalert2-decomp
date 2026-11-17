#!/usr/bin/env python3
"""Write an objdiff-format progress report (version 2 JSON) for ByteMatch, and optionally upload it.

ByteMatch (https://bytematch.dev) reads objdiff reports. Our build needs the private target and the
VC6 toolchain, so it cannot run in GitHub Actions; this tool converts the landing numbers instead and
posts them through ByteMatch's upload API.

Input is build/treemap.json (treemap.py --json, regenerated at each landing) plus the canonical
manifest for source paths. The report holds only names, addresses, sizes and match percentages; no
bytes from the target. Scope matches STATUS "In-scope functions matched": game code below the C
runtime, handwritten-assembly ranges, evidenced inline-asm regions and known false rows excluded. Accepted handwritten assembly
procedures count as matched, as they do in the treemap.

One unit per treemap group (class, recovered directory or original source file). A function counts as
matched only when accepted exact; others carry their best recorded score as fuzzy_match_percent.

    python3 tools/original_target/bytematch_report.py                    # build/bytematch/report.json
    python3 tools/original_target/bytematch_report.py --upload          # token from private/bytematch.token
"""
import argparse
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
VERSION = 'ra2'
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


def sources(catalog):
    return {r['entry']: r['source'] for r in catalog['regions'] if r.get('source')}


def in_scope(rows, catalog):
    """STATUS's in-scope set: below the CRT, outside handwritten ranges and accepted evidenced
    inline-asm regions (status.ledger counts both as assembly), known false rows excluded."""
    asm = [(a, b) for a, b, _ in status.HANDWRITTEN] + [
        (r['entry'], r['entry'] + r['body_bytes']) for r in catalog['regions']
        if 'entry' in r and status.evidenced_inline_asm(r)]
    false_path = REPOSITORY / 'manifests/original_target/false_rows.json'
    false_rows = {int(r['entry'], 16) for r in json.loads(false_path.read_text())['rows']} if false_path.exists() else set()
    return [r for r in rows if r['entry'] < status.CRT_START and r['entry'] not in false_rows
            and not status.inside(r['entry'], asm)]


def unit_name(group, source):
    if source:
        folder = str(Path(source).parent)
        for prefix in ('src/recovered/', 'src/'):
            if folder.startswith(prefix):
                folder = folder[len(prefix):]
        return folder if Path(folder).name == group else '%s/%s' % (folder, group)
    return 'unplaced/%s' % group


def status_in_scope():
    """In-scope matched and total C/C++ bytes from the generated STATUS table (status.py --write)."""
    for line in (REPOSITORY / 'docs/STATUS.md').read_text().splitlines():
        if line.startswith('| In-scope matched C/C++ bytes'):
            found = re.search(r'\| ([\d,]+) of ([\d,]+) \(', line)
            return tuple(int(x.replace(',', '')) for x in found.groups())
    sys.exit('docs/STATUS.md has no in-scope bytes row: run status.py --write')


def reconcile(units):
    """In-scope bytes the treemap places in no function (non-contiguous or uninventoried bodies), as one
    unit with no functions, so the report's byte totals equal STATUS's in-scope C/C++ row."""
    matched, total = status_in_scope()
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


def build(treemap):
    best = status.best_scores(REPOSITORY / 'docs/MISSES_INDEX.md', REPOSITORY / 'build/sweep-6b/results.jsonl')
    catalog = load_catalog()
    src = sources(catalog)
    groups = defaultdict(list)
    for r in in_scope(treemap, catalog):
        groups[r['group']].append(r)

    units, names = [], Counter()
    for group, rows in sorted(groups.items()):
        rows.sort(key=lambda r: r['entry'])
        paths = Counter(src[r['entry']] for r in rows if r['entry'] in src)
        source = paths.most_common(1)[0][0] if paths else None
        name = unit_name(group, source)
        names[name] += 1
        if names[name] > 1:
            name = '%s~%d' % (name, names[name])
        m = empty()
        functions = []
        for r in rows:
            exact = r['status'] == 'exact'
            score = 100.0 if exact else float(r.get('score') or best.get(r['entry'], 0.0))
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
        units.append(dict(unit=dict(name=name, measures=wire(m), sections=[dict(
            name='.text', size=str(m['total_code']), fuzzy_match_percent=m['fuzzy_match_percent'])],
            functions=functions, metadata=metadata), m=m, cat=cat))

    units.append(reconcile(units))
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


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--treemap', type=Path, default=REPOSITORY / 'build/treemap.json')
    ap.add_argument('--out', type=Path, default=REPOSITORY / 'build/bytematch/report.json')
    ap.add_argument('--upload', action='store_true', help='POST to ByteMatch (token in $BYTEMATCH_TOKEN)')
    ap.add_argument('--commit', help='commit the report describes (default: HEAD)')
    ap.add_argument('--version', default=VERSION)
    args = ap.parse_args()

    treemap = json.loads(args.treemap.read_text())
    if not treemap:
        sys.exit('empty treemap: regenerate it with treemap.py --json build/treemap.json')
    report, total = build(treemap)
    args.out.parent.mkdir(parents=True, exist_ok=True)
    data = json.dumps(report, separators=(',', ':')).encode()
    args.out.write_bytes(data)
    print('%s: %d units, %d/%d functions, %d/%d code bytes (%.2f%%), fuzzy %.2f%%' % (
        args.out, total['total_units'], total['matched_functions'], total['total_functions'],
        total['matched_code'], total['total_code'], total['matched_code_percent'], total['fuzzy_match_percent']))

    if args.upload:
        token = os.environ.get('BYTEMATCH_TOKEN')
        if not token and TOKEN_FILE.exists():
            token = TOKEN_FILE.read_text().strip()
        if not token:
            sys.exit('put the ByteMatch upload token in %s (or $BYTEMATCH_TOKEN)' % TOKEN_FILE)
        commit = args.commit or subprocess.check_output(['git', 'rev-parse', 'HEAD'], cwd=REPOSITORY, text=True).strip()
        request = urllib.request.Request(
            API.format(project=PROJECT, version=args.version, commit=commit), data=gzip.compress(data),
            method='POST', headers={'Authorization': 'Bearer ' + token, 'Content-Type': 'application/json',
                                    'Content-Encoding': 'gzip', 'User-Agent': 'redalert2-decomp-report'})
        with urllib.request.urlopen(request) as response:
            print('uploaded for %s: HTTP %d %s' % (commit[:9], response.status, response.read().decode()[:300]))


if __name__ == '__main__':
    main()
