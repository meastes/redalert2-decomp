#!/usr/bin/env python3
"""Flat function map of the exact build: one CSV row per catalog region and per non-exact draft.

    export_function_map.py [--output PATH]      (default: standard output)

Columns: address, size, source, symbol, population, options, flags.

  address      the function's original entry, 0x%08X
  size         body bytes (accepted rows: the exact body the compiler reproduces; drafts: the
               original function's bytes as non_exact.json records them)
  source       the tracked file that holds the function's text
  symbol       the decorated (linker) name the row binds
  population   the compile group that verifies the row (empty for a draft)
  options      the row's recorded compiler options, space-separated (empty for a draft)
  flags        semicolon-separated: `exact` or `non_exact score=N`, then every judgment-call or
               evidence field the row carries (count_calibrated, split_declaration,
               declarations_only, inline_asm=evidenced, unit_evidence=..., data_bound_by=...,
               bound_population=...)

Read from the tracked manifests only (manifests/original_target/ and non_exact.json): no
original bytes and no hashes are written, so the file is byte-free. Rows are ordered by
address; a draft whose address an accepted row also holds is listed after it.
"""
import argparse
import csv
import io
import json
from pathlib import Path
import sys

sys.path.insert(0, str(Path(__file__).resolve().parent))
import catalog_io

REPOSITORY = Path(__file__).resolve().parents[2]
COLUMNS = ('address', 'size', 'source', 'symbol', 'population', 'options', 'flags')
# Row fields reported as flags, in this order: a bare name for a true value, name=value otherwise.
FLAG_FIELDS = ('count_calibrated', 'split_declaration', 'declarations_only', 'inline_asm', 'unit_evidence',
               'data_bound_by', 'bound_population')


def region_flags(region):
    flags = ['exact']
    for field in FLAG_FIELDS:
        value = region.get(field)
        if value is None or value is False:
            continue
        flags.append(field if value is True else '%s=%s' % (field, value))
    return ';'.join(flags)


def rows(catalog, drafts):
    """The map's rows as tuples in COLUMNS order, sorted by address."""
    out = []
    for region in catalog['regions']:
        out.append((region['entry'], 0, ('0x%08X' % region['entry'], region['body_bytes'], region['source'],
                                          region['symbol'], region['population'], ' '.join(region['options']),
                                          region_flags(region))))
    for draft in drafts:
        entry = int(draft['entry'], 16)
        out.append((entry, 1, ('0x%08X' % entry, int(draft['bytes']), draft['source'], draft.get('symbol', ''),
                               '', '', 'non_exact score=%s' % draft['non_exact']['score'])))
    return [row for _, _, row in sorted(out, key=lambda item: (item[0], item[1]))]


def render(table):
    buffer = io.StringIO()
    writer = csv.writer(buffer, lineterminator='\n')
    writer.writerow(COLUMNS)
    writer.writerows(table)
    return buffer.getvalue()


def load(repository=REPOSITORY):
    repository = Path(repository)
    catalog = catalog_io.load(catalog_io.locate(repository), references=False)
    path = repository / 'manifests/original_target/non_exact.json'
    drafts = json.loads(path.read_text()).get('rows', []) if path.exists() else []
    return catalog, drafts


def main():
    parser = argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    parser.add_argument('--repository', type=Path, default=REPOSITORY)
    parser.add_argument('--output', type=Path, help='write here instead of standard output')
    args = parser.parse_args()
    text = render(rows(*load(args.repository)))
    if args.output:
        args.output.write_text(text)
        print('%s: %d rows, %d bytes' % (args.output, text.count('\n') - 1, len(text.encode())), file=sys.stderr)
    else:
        sys.stdout.write(text)


if __name__ == '__main__':
    main()
