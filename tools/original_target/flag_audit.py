#!/usr/bin/env python3
"""flag_audit.py [--fix] [--csv PATH]: judgment flags that disagree with a row's own provenance (lane O93, 2026-11-25).

An independent review of 40 accepted rows found rows whose provenance (text_source and note in
manifests/provenance.csv) says the text is a twin's in place of the archive's, or joins two references, or rests on
a declaration count, while the judgment column is empty or of another kind (owner ruling 2026-11-25). This lists
them by class:

  twin-over-archive   the twin's text replaced the archive's ("over the archive's", "in place of the archive's",
                      "replaces the adopted archive text", "the archive draft ... replaced by the twin's") and the
                      judgment carries no [iv-a] (or [joined]) part. With an empty judgment the fix is mechanical: --fix writes
                      "[iv-a] twin text where the archive differs ..." (the flag Set_Cursor_Pos and Reset_Subzone
                      carry), quoting the row's text_source.
  joined              the text names the twin's and the archive's parts side by side ("with the TibSun archive's",
                      "archive ... otherwise", "with ra2-decomp's twin's forms") and the judgment has no [joined]
                      or [iv-a] part: listed, for a reading (a joined flag needs the note on why neither side
                      stands whole).
  count               the note says the row's exact form rests on a declaration count (count window, count-sensitive,
                      declaration count) and the judgment is empty: listed (count-sensitive rows are tracked in
                      units.json; a row that only moves another row's window is not itself a judgment).

Only rows of units.json (accepted rows of the tree) are read. Output: one line per finding, then the counts."""
import argparse
import csv
import io
import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
PROVENANCE = ROOT / 'manifests/provenance.csv'
UNITS = ROOT / 'manifests/units.json'
RULING = 'flag_audit.py, owner ruling 2026-11-25 after the independent review, lane O93'

OVER_ARCHIVE = re.compile(
    r"\bover the (?:TibSun )?archive(?:'s)?\b|in place of the archive(?:'s)?\b|replaces the adopted archive"
    r"|\barchive(?:'s)? draft\b[^.;]*\breplaced by the twin", re.I)
JOINED = re.compile(
    r"\bwith (?:the )?(?:TibSun )?archive's\b|\barchive\b[^.;]*\botherwise\b|\bwith ra2-decomp's twin's\b"
    r"|\btwin\b[^.;]*\bwhere the image agrees\b", re.I)
COUNT = re.compile(r'count window|declaration[- ]count|count-sensitive|declaration window', re.I)
TWIN = re.compile(r'\btwin', re.I)
KIND = re.compile(r'\[([a-z0-9-]+)\]')


def kinds(judgment):
    """Every bracketed kind tag in a judgment note (the first is the row's kind)."""
    return KIND.findall(judgment or '')


def classify(row):
    """[(class, detail)] for one provenance row (a dict)."""
    text = '%s || %s' % (row.get('text_source', ''), row.get('note', ''))
    judgment = (row.get('judgment') or '').strip()
    tags = kinds(judgment)
    out = []
    if TWIN.search(text) and OVER_ARCHIVE.search(text) and not {'iv-a', 'joined'} & set(tags):
        out.append(('twin-over-archive', 'empty' if not judgment else 'kind ' + (tags[0] if tags else 'untagged')))
    elif TWIN.search(text) and JOINED.search(text) and not {'joined', 'iv-a'} & set(tags):
        out.append(('joined', 'empty' if not judgment else 'kind ' + (tags[0] if tags else 'untagged')))
    if COUNT.search(row.get('note', '')) and not judgment:
        out.append(('count', 'empty'))
    return out


def fix_note(row):
    return "[iv-a] twin text where the archive differs (%s): %s" % (RULING, row['text_source'].strip())


def rewrite(path, changes):
    """Write judgment cells {entry: note} into PATH line by line; every other line stays byte for byte."""
    lines = path.read_text().split('\n')
    header = next(csv.reader([lines[0]]))
    ji, ei = header.index('judgment'), header.index('entry')
    for i, line in enumerate(lines[1:], 1):
        if not line:
            continue
        fields = next(csv.reader([line]))
        if fields[ei] in changes:
            fields[ji] = changes[fields[ei]]
            buf = io.StringIO()
            csv.writer(buf, lineterminator='').writerow(fields)
            lines[i] = buf.getvalue()
    path.write_text('\n'.join(lines))


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('--fix', action='store_true', help='flag the mechanical class (twin-over-archive, empty)')
    parser.add_argument('--csv', type=Path, default=PROVENANCE)
    parser.add_argument('--units', type=Path, default=UNITS)
    args = parser.parse_args(argv)
    accepted = None
    if args.units.exists():
        accepted = {r['entry'] for info in json.loads(args.units.read_text()).values() for r in info['rows']}
    rows = list(csv.DictReader(open(args.csv, newline='')))
    findings, changes = [], {}
    for row in rows:
        if accepted is not None and row['entry'] not in accepted:
            continue
        for cls, detail in classify(row):
            mechanical = cls == 'twin-over-archive' and detail == 'empty'
            findings.append((cls, detail, row, mechanical))
            if mechanical:
                changes[row['entry']] = fix_note(row)
    for cls, detail, row, mechanical in findings:
        print('%-18s %-8s %s %08x %s%s' % (cls, row['unit'], detail.ljust(18), int(row['entry'], 16),
                                          row['symbol'][:70], '  (fixed)' if mechanical and args.fix else ''))
    counts = {}
    for cls, detail, _, mechanical in findings:
        counts.setdefault(cls, [0, 0])[0] += 1
        counts[cls][1] += mechanical
    print('flag_audit: %d findings (%s)%s' % (len(findings), ', '.join(
        '%s %d, %d mechanical' % (c, n, m) for c, (n, m) in sorted(counts.items())) or 'none',
        '; %d judgment notes written' % len(changes) if args.fix else ''))
    if args.fix and changes:
        rewrite(args.csv, changes)
    return 0


if __name__ == '__main__':
    sys.exit(main())
