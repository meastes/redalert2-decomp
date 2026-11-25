"""ra2drafts.py [--units]: how many parked functions have an RA2-correct draft (lane B2, 2026-11-20; the
checkpoint's completeness measure). Reads manifests/tail_buckets.csv (tailmap.py --write), compiles nothing.

The parked rows are tailmap's rows: every row the census counts as parked (`rebuild_census.parked_tail`, every
row of units.json not exact, count-sensitive rows apart). A parked row has an RA2-correct draft when its text,
compiled with the guards open, emits the row's function with the original's direct calls, at most four call
sites apart (one callee expanded on one side and called on the other): tailmap's buckets exact_when_open,
inline_call, unbound, inline_body, stack_layout and register_order. Such a draft does what RA2 does (main's
accepted form, an evidenced twin, or adopted text that already follows RA2's calls); what is left is compiler
shape. It is not RA2-correct when there is no text (no_draft), only text under #if 0 (text_if0), the draft
does not compile (no_compile), the object does not emit it under the row's name (not_emitted), or more than
four calls differ (calls_differ: TibSun or YR text standing in, or a draft still missing RA2's code).

Prints rows and body bytes for both sides and the split by bucket; --units adds the units with the most
parked rows that have no RA2-correct draft."""
import collections
import csv
import sys
from pathlib import Path

CORRECT = ('exact_when_open', 'inline_call', 'unbound', 'inline_body', 'stack_layout', 'register_order')
OUT = Path(__file__).resolve().parents[2] / 'manifests' / 'tail_buckets.csv'


def measure(path=OUT):
    """(correct rows, correct bytes, parked rows, parked bytes, Counter of (bucket) -> [rows, bytes], rows)."""
    rows = list(csv.DictReader(open(path)))
    by = collections.defaultdict(lambda: [0, 0])
    good = good_bytes = total = 0
    for r in rows:
        n = int(r['bytes'])
        by[r['bucket']][0] += 1; by[r['bucket']][1] += n
        total += n
        if r['bucket'] in CORRECT:
            good += 1; good_bytes += n
    return good, good_bytes, len(rows), total, by, rows


def main():
    good, good_bytes, count, total, by, rows = measure()
    print('Parked functions with RA2-correct drafts: %d / %d (%.1f%%), %d / %d body bytes (%.1f%%)'
          % (good, count, 100.0 * good / max(count, 1), good_bytes, total, 100.0 * good_bytes / max(total, 1)))
    for bucket, (n, b) in sorted(by.items(), key=lambda kv: -kv[1][1]):
        print('  %-16s %-12s %5d rows %8d bytes' % (bucket, 'RA2-correct' if bucket in CORRECT else 'not', n, b))
    if '--units' in sys.argv:
        units = collections.Counter(r['unit'] for r in rows if r['bucket'] not in CORRECT)
        print('units with the most parked rows without an RA2-correct draft:')
        for unit, n in units.most_common(25):
            print('  %-16s %d' % (unit, n))


if __name__ == '__main__':
    main()
