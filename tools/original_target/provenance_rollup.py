#!/usr/bin/env python3
"""Where the accepted text came from, by source (lane O88, 2026-11-25; review session request).

    provenance_rollup.py            print the table (functions, body bytes) over the catalog's rows
    provenance_rollup.py --csv      the same as CSV

manifests/provenance.csv's text_source is free text (hundreds of distinct values). This maps each accepted
row of the catalog (manifests/catalog: the rows the whole-file build accepts) to one source:

  windows-workers     a row a Windows worker delivered (workers.csv / worker_rows.csv, as progress.py counts them)
  reshaped-from-main  class b: a body from this project's earlier tree (src-final), reshaped
  fresh               class c: written fresh from the Ghidra seed and the class layout
  mischa85, ra2-decomp, tibsun, robin, renegade, generals
                      class a whose text_source names exactly one of these references, or starts "adopted from" one
                      (a class a row whose text_source names several references or none, e.g. "inherited
                      adopted unit", takes the adopted origin of its unit file in manifests/adopted.csv; the
                      table shows how many rows were placed that way)
  mixed               the rest: class a naming several references or none in a unit file not in adopted.csv,
                      or a row with no provenance line

Body bytes are the catalog rows' body_bytes (no padding), the measure of "C/C++ bodies matched".
"""
import argparse
import csv
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
MANIFEST = ROOT / 'manifests'
LABELS = {'windows-workers': "windows-workers (this project's own matching, done on its Windows machines)",
          'mixed': 'mixed (template copies and header scans in files not adopted from one source)',
          'reshaped-from-main': "reshaped-from-main (this project's earlier tree)",
          'fresh': 'fresh (written here from the seed and the class layout)'}
CAVEAT = ("The sources build on one another (mischa85's on the TibSun reconstruction; the Yuri's Revenge and 1.001 "
          "projects on each other and on this one), so this records where each function's text was taken from, not "
          "who first wrote it.")
SOURCES = ('mischa85', 'ra2-decomp', 'tibsun', 'robin', 'renegade', 'generals', 'reshaped-from-main', 'fresh',
           'windows-workers', 'mixed')
REFERENCES = (('robin', r'robin'), ('renegade', r'renegade'), ('generals', r'generals|gametext'),
              ('mischa85', r'mischa85|mischa'), ('ra2-decomp', r'ra2-decomp|ra2decomp|gibbo101|yuri'),
              ('tibsun', r'tibsun|tiberian sun|opents'))


def normalise(cls, text_source):
    """One of SOURCES for a provenance line (class and text_source); workers are decided by the caller."""
    if cls == 'b':
        return 'reshaped-from-main'
    if cls == 'c':
        return 'fresh'
    text = (text_source or '').lower()
    lead = re.match(r'\s*(?:adopted\s+)?from\s+(\S+)', text)       # "adopted from robin-ra2-1001-decomp ...": that one
    if lead:
        named = [name for name, pattern in REFERENCES if re.search(pattern, lead.group(1))]
        if len(named) == 1:
            return named[0]
    found = [name for name, pattern in REFERENCES if re.search(pattern, text)]
    return found[0] if len(found) == 1 else 'mixed'


def worker_lookup(manifest=MANIFEST):
    """worker_of(unit, entry) as progress.py counts worker rows (a unit in worker_rows.csv counts only its rows)."""
    def read(name):
        path = manifest / name
        return list(csv.DictReader(open(path))) if path.exists() else []
    units = {r['unit']: r['worker'] for r in read('workers.csv')}
    rows = {}
    for r in read('worker_rows.csv'):
        rows.setdefault(r['unit'], {})[r['entry']] = r['worker']
    return lambda unit, entry: rows[unit].get(entry) if unit in rows else units.get(unit)


def rollup(regions, manifest=MANIFEST):
    """{source: [functions, body bytes, functions placed by their unit file's adopted origin]} over catalog
    REGIONS, in SOURCES order."""
    prov = {r['entry']: r for r in csv.DictReader(open(manifest / 'provenance.csv'))}
    origin = {r['file']: r['origin'] for r in csv.DictReader(open(manifest / 'adopted.csv'))}
    worker_of = worker_lookup(manifest)
    out = {s: [0, 0, 0] for s in SOURCES}
    for row in regions:
        entry = '%08x' % row['entry']
        unit = row['source'].split('/')[-1].rsplit('.', 1)[0]
        if worker_of(unit, entry):
            source = 'windows-workers'
        elif entry in prov:
            source = normalise(prov[entry]['class'], prov[entry]['text_source'])
        else:
            source = 'mixed'
        if source == 'mixed' and entry in prov and prov[entry]['class'] == 'a' and origin.get(row['source']) in SOURCES:
            source = origin[row['source']]
            out[source][2] += 1
        out[source][0] += 1
        out[source][1] += row['body_bytes']
    return out


def table_lines(split):
    total_f, total_b = sum(v[0] for v in split.values()), sum(v[1] for v in split.values())
    lines = ['| Source of the text | Functions | Body bytes | Share of body bytes | Of the functions: placed by the unit '
             'file\'s adopted origin |', '| --- | ---: | ---: | ---: | ---: |']
    for source in SOURCES:
        f, b, by_file = split[source]
        lines.append('| {} | {:,} | {:,} | {:.1f}% | {} |'.format(LABELS.get(source, source), f, b, 100.0 * b / total_b if total_b else 0,
                                                                 '{:,}'.format(by_file) if by_file else ''))
    lines.append('| total | {:,} | {:,} | 100.0% | {:,} |'.format(total_f, total_b, sum(v[2] for v in split.values())))
    return lines


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--csv', action='store_true')
    args = ap.parse_args()
    sys.path.insert(0, str(Path(__file__).resolve().parent))
    import catalog_io
    split = rollup(catalog_io.load(MANIFEST / 'catalog', references=False)['regions'])
    if args.csv:
        w = csv.writer(sys.stdout)
        w.writerow(['source', 'functions', 'body_bytes', 'by_unit_file_origin'])
        for source in SOURCES:
            w.writerow([source] + split[source])
    else:
        print('\n'.join(table_lines(split) + ['', CAVEAT]))


if __name__ == '__main__':
    main()
