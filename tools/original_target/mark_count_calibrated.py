#!/usr/bin/env python3
"""Set or clear the count-calibration flag of accepted canonical rows (no source or byte change).

    mark_count_calibrated.py MARKS.json

MARKS.json maps an entry ("0x6CFEA0") to its count_calibrated_note (user decision 2026-11-09:
the chosen forms and the evidence checked first), or to null to clear the flag. Every entry must
be an accepted region. Rewrites only the catalog shards that change (catalog_io.dump), so it can be
re-run on any later HEAD; the next full build's strict audit checks the result. A plan item can set
the same keys at acceptance instead (accept_sources carries them).
"""
import argparse
import json
from pathlib import Path
import sys

sys.path.insert(0, str(Path(__file__).resolve().parent))
import catalog_io
from canonical import COUNT_CALIBRATED_KEYS, validate_count_calibrated

REPOSITORY = Path(__file__).resolve().parents[2]


def apply_marks(catalog, marks):
    """marks: {entry int: note str or None}. Returns the entries whose row changed."""
    regions = {r['entry']: r for r in catalog['regions']}
    missing = sorted(set(marks) - set(regions))
    if missing:
        raise SystemExit('not accepted regions: ' + ', '.join('%08x' % e for e in missing))
    changed = []
    for entry, note in sorted(marks.items()):
        region = regions[entry]
        before = {k: region.get(k) for k in COUNT_CALIBRATED_KEYS}
        for key in COUNT_CALIBRATED_KEYS:
            region.pop(key, None)
        if note is not None:
            region.update(count_calibrated=True, count_calibrated_note=note)
        validate_count_calibrated(region)
        if {k: region.get(k) for k in COUNT_CALIBRATED_KEYS} != before:
            changed.append(entry)
    return changed


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('marks', type=Path)
    args = parser.parse_args(argv)
    marks = {int(k, 16): v for k, v in json.loads(args.marks.read_text()).items()}
    path = catalog_io.locate(REPOSITORY)
    catalog = catalog_io.load(path)
    changed = apply_marks(catalog, marks)
    written = catalog_io.dump(catalog, path) if changed else []
    print('changed %d rows: %s; wrote %s' % (len(changed), ', '.join('%08x' % e for e in changed),
                                            ', '.join(written) or 'nothing'))


if __name__ == '__main__':
    main()
