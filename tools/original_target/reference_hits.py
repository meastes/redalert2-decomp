"""reference_hits.py: functions a reference decompilation reports matched that we have not accepted.

Reads mischa85/RedAlert2's config/ra2-1.006/functions.csv (same game.exe; address,size,evidence) and lists rows
whose evidence says "matched" and whose address is not the start of, or inside, one of our accepted catalog
regions, library-ledger regions or assembly regions. Hypothesis feed only (AGENTS.md External references):
a hit is routed to a lane and accepted only by the strict audit and the exact whole-file build.

    python3 tools/original_target/reference_hits.py [--repo private/reference/mischa85-RedAlert2] [--json]
"""
import argparse, bisect, csv, json, sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'tools/original_target'))
import catalog_io  # noqa: E402


def covered_spans(root=ROOT):
    """Sorted (start, end) spans of everything we already reproduce."""
    spans = []
    cat = catalog_io.load(catalog_io.locate(root))
    for r in cat['regions']:
        spans.append((r['entry'], r['entry'] + max(r.get('full_extent_bytes') or 0, r.get('body_bytes') or 0, 1)))
    for name in ('library.json', 'assembly.json'):
        path = root / 'manifests/original_target' / name
        if not path.exists():
            continue
        for r in json.loads(path.read_text()).get('regions', []):
            start = r.get('start', r.get('entry', r.get('address')))
            start = int(start, 16) if isinstance(start, str) else start
            if start is None:
                continue
            spans.append((start, start + max(int(r.get('bytes', r.get('size', 1)) or 1), 1)))
    spans.sort()
    return spans


def is_covered(addr, spans, starts):
    i = bisect.bisect_right(starts, addr) - 1
    return i >= 0 and spans[i][0] <= addr < spans[i][1]


def hits(repo, root=ROOT):
    spans = covered_spans(root)
    starts = [s for s, _ in spans]
    out = []
    with open(Path(repo) / 'config/ra2-1.006/functions.csv', newline='') as f:
        for row in csv.DictReader(f):
            evidence = row['evidence']
            if evidence.startswith('inferred') or not (evidence.startswith('matched') or ', matched' in evidence):
                continue
            addr = int(row['address'], 16)
            if not is_covered(addr, spans, starts):
                out.append(dict(address='0x%08X' % addr, size=int(row['size'] or 0), evidence=row['evidence']))
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--repo', default=str(ROOT / 'private/reference/mischa85-RedAlert2'))
    ap.add_argument('--json', action='store_true')
    a = ap.parse_args()
    found = hits(a.repo)
    if a.json:
        print(json.dumps(found, indent=1))
    else:
        for h in found:
            print('%s %6d  %s' % (h['address'], h['size'], h['evidence']))
        print('%d matched there, not reproduced here (%d bytes)' % (len(found), sum(h['size'] for h in found)))


if __name__ == '__main__':
    main()
