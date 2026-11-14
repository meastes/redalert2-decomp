#!/usr/bin/env python3
"""Accept new exact rows into an existing Route A module population (owner ruling 2026-11-14).

    module_add_row.py UNIT ENTRY [ENTRY ...] [--draft ENTRY=FILE] [--symbol ENTRY=PREFIX]
                      [--name NAME] [--config CONFIG] [--output DIR] [--dry-run]

A module source (one original translation unit, manifests/original_target/modules/<UNIT>.json)
compiles as one population, so a function matched inside it cannot go through accept_sources.py
(which takes one file per region and refuses module sources). This adds rows to the module
without changing any acceptance rule:

* Text. A row with a non-exact draft (manifests/original_target/non_exact.json) loses its marker
  line and its `#ifdef RA2_NON_MATCHING` guard; `--draft ENTRY=FILE` replaces the guarded text
  with FILE. A row without a draft needs `--draft`; the text goes in at its address-order place
  (non_exact.insertion_line). The non_exact.json row is removed in the same step.
* Bound. Only from independent frozen boundaries: the first frozen inventory pass holding a
  contiguous row that starts at the entry (accept_sources.composite_pass, never the candidate's
  compiled output and never a stated body length). No such row refuses. The rows are copied into
  a new frozen composite pass `composite-addrow-<NAME>` held by a new row-less population, which
  each new row names as its `bound_population` (the same mechanism as an own-bound join, so the
  strict audit reads the bound, gap padding and provenance from that pass). Frozen padding is a
  whole independent gap of original NOPs (promote_sweep.gap_padding).
* Check. The whole module population is recompiled as its canonical units; every existing row
  must still reproduce its complete extent and every new row its complete bounded body, frozen
  padding and references; then the strict canonical audit (apply_rename.verify) and the module
  agreement check. Any miss restores every written file and leaves the catalog untouched.

Run ./build.command afterwards; it remains the final gate. The new population is also added to
the private compiler configs (promote_sweep.CONFIGS) so the build can read its pass.
"""
import argparse
import json
from pathlib import Path
import re
import shutil
import sys
import time

REPOSITORY = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(Path(__file__).resolve().parent))

import non_exact

GUARD = non_exact.GUARD


def module_item(unit, repository=REPOSITORY):
    """(source, population) of a Route A module map; refuses an unknown unit."""
    path = repository / 'manifests/original_target/modules' / (unit + '.json')
    if not path.exists():
        raise SystemExit('%s: no module map %s' % (unit, path.relative_to(repository)))
    item = json.loads(path.read_text())
    return item['source'], item['population']


def unguard(text, entry, replacement=None):
    """The text with the non-exact draft of `entry` taken out of its guard: the marker line, the
    `#ifdef RA2_NON_MATCHING` and its `#endif` go; the guarded lines stay (or `replacement`
    takes their place). Refuses when the marker or a well-formed block is missing."""
    lines = text.split('\n')
    markers = [n for n, line in enumerate(lines)
               if (lambda m: m and int(m.group('entry'), 16) == entry)(non_exact.MARKER.match(line))]
    if len(markers) != 1:
        raise SystemExit('%08x: %d non-exact markers in the source' % (entry, len(markers)))
    marker = markers[0]
    blocks, problems = non_exact.guarded_blocks(text)
    if problems:
        raise SystemExit('%08x: guard problems in the source: %s' % (entry, problems[:3]))
    block = next(((a, b) for a, b in blocks if a == marker + 1), None)
    if block is None:
        raise SystemExit('%08x: no #ifdef %s directly below its marker' % (entry, GUARD))
    first, last = block
    body = lines[first + 1:last] if replacement is None else replacement.rstrip('\n').split('\n')
    return '\n'.join(lines[:marker] + body + lines[last + 1:])


def insert(text, entry, draft, accepted_entries):
    """The text with `draft` placed at the address-order place of `entry` (before the comment
    block of the first accepted row above it), followed by one blank line."""
    lines = text.split('\n')
    at = non_exact.insertion_line(text, entry, accepted_entries)
    block = draft.rstrip('\n').split('\n') + ['']
    if at > 0 and lines[at - 1].strip():
        block = [''] + block
    return '\n'.join(lines[:at] + block + lines[at:])


def place(text, entry, draft, has_marker, accepted_entries):
    if has_marker:
        return unguard(text, entry, draft)
    if draft is None:
        raise SystemExit('%08x: no non-exact draft in the source; give --draft %08x=FILE' % (entry, entry))
    if re.search(r'\(0x0*%X[,;)]' % entry, text, re.I):
        raise SystemExit('%08x: the source already names this address in a header comment; edit it by hand '
                         'or remove the stale text' % entry)
    return insert(text, entry, draft, accepted_entries)


def overlapping(catalog, entry, length):
    """Accepted regions whose full extent overlaps [entry, entry + length)."""
    return [r for r in catalog['regions']
            if r['entry'] < entry + length and entry < r['entry'] + r['full_extent_bytes']]


def new_region(entry, body, padding_hex, *, population, bound_population, source, symbol_prefix, options,
               references, source_sha256, source_order):
    from canonical import sha
    pad = bytes.fromhex(padding_hex)
    return dict(entry=entry, method='%08x' % entry, symbol=symbol_prefix, symbol_prefix=symbol_prefix,
                options=list(options), body_bytes=len(body), body_sha256=sha(body), padding_bytes=len(pad),
                full_extent_bytes=len(body) + len(pad), full_extent_sha256=sha(body + pad),
                references=references, relocations=[], source_sha256=source_sha256, source=source,
                population=population, bound_population=bound_population, padding_sha256=sha(pad),
                boundary_confidence='Frozen independent analysis hypothesis; exact compiler extent, not certified function',
                source_quality_family='hand-written', source_order=source_order)


def pairs(values, what):
    out = {}
    for value in values or []:
        key, _, rest = value.partition('=')
        if not rest:
            raise SystemExit('%s takes ENTRY=VALUE: %s' % (what, value))
        out[int(key, 16)] = rest
    return out


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('unit')
    parser.add_argument('entries', nargs='+')
    parser.add_argument('--draft', action='append', help='ENTRY=FILE: the row text (replaces a guarded draft)')
    parser.add_argument('--symbol', action='append', help='ENTRY=PREFIX: the row symbol prefix (default: the '
                                                          'non_exact.json symbol)')
    parser.add_argument('--name', help='batch name (default UNIT-ENTRY...)')
    parser.add_argument('--config', type=Path, default=REPOSITORY / 'private/original-target.local.json')
    parser.add_argument('--output', type=Path)
    parser.add_argument('--dry-run', action='store_true')
    args = parser.parse_args()
    started = time.monotonic(); marks = [started]
    def phase(name):
        now = time.monotonic()
        print('phase %s: %.1fs' % (name, now - marks[-1]), file=sys.stderr, flush=True); marks.append(now)

    import catalog_io
    from accept_sources import composite_pass
    from apply_rename import bind_changed, restore, verify, write_files
    from canonical import sha, validate_modules
    from inventory import Inventory
    from promote_sweep import CONFIGS, gap_padding
    from verify_manifest import Audit

    entries = [int(e, 16) for e in args.entries]
    drafts = {e: (REPOSITORY / p).read_text() for e, p in pairs(args.draft, '--draft').items()}
    symbols = pairs(args.symbol, '--symbol')
    name = args.name or '%s-%s' % (args.unit, '-'.join('%08x' % e for e in entries))
    config = json.loads(args.config.read_bytes())
    catalog_path = catalog_io.locate(REPOSITORY)
    catalog = catalog_io.load(catalog_path)
    digest = catalog_io.digest(catalog_path)
    source, pid = module_item(args.unit)
    members = [r for r in catalog['regions'] if r['population'] == pid]
    if not any(r['source'] == source for r in members):
        raise SystemExit('%s: %s holds no row of %s' % (args.unit, pid, source))
    manifest = non_exact.load_manifest()
    drafted = {int(r['entry'], 16): r for r in manifest['rows']}
    for entry in entries:
        if any(r['entry'] == entry for r in catalog['regions']):
            raise SystemExit('%08x is already accepted' % entry)
        row = drafted.get(entry)
        if row and row['source'] != source:
            raise SystemExit('%08x: its non-exact draft lives in %s, not %s' % (entry, row['source'], source))
        if entry not in symbols and not (row and row.get('symbol')):
            raise SystemExit('%08x: no symbol; give --symbol %08x=PREFIX' % (entry, entry))

    # Text: unguard or insert each row, in address order.
    text = (REPOSITORY / source).read_text()
    accepted = {r['entry'] for r in members}
    for entry in sorted(entries):
        text = place(text, entry, drafts.get(entry), entry in drafted, accepted)
        accepted.add(entry)
    data = text.encode()
    left = [r for r in manifest['rows'] if int(r['entry'], 16) not in entries]
    manifest_text = json.dumps(dict(manifest, rows=left), indent=1) + '\n'

    # Bound: independent frozen passes only (no stated body length).
    audit = Audit(REPOSITORY, config['executable'])
    plan = dict(name='addrow-' + name, sources=[dict(entry='%08x' % e) for e in sorted(entries)])
    if args.dry_run:
        inventory = Inventory()
        from inventory import INVENTORY, PASSES
        for entry in entries:
            found = next((d for d in PASSES + sorted(INVENTORY.glob('composite-*'))
                          if (lambda r: r and r['contiguous'] and len(r['ranges']) == 1 and
                              int(r['ranges'][0]['start'], 16) == entry)(inventory.rows(d).get(entry))), None)
            print('%08x: bound from %s' % (entry, found and found.relative_to(REPOSITORY)))
        print(json.dumps(dict(name=name, unit=args.unit, population=pid, source=source,
                              entries=['%08x' % e for e in entries], unguarded=sorted('%08x' % e for e in entries
                                                                                 if e in drafted)), indent=1))
        return
    directory = composite_pass(plan, Inventory(), {}, catalog, audit)
    if directory is None:
        raise SystemExit('refused: no frozen inventory pass holds an independently bounded contiguous row '
                         'at every entry')
    rows = {int(r['entry'], 16): r for r in map(json.loads, (directory / 'functions.jsonl').read_text().splitlines())}
    gaps = {int(g['start'], 16): g for g in map(json.loads, (directory / 'unassigned-executable.jsonl').read_text().splitlines())}
    phase('bound')

    taken = [int(p['id'].split('-')[1]) for p in catalog['populations']]
    for config_name in CONFIGS:
        path = REPOSITORY / 'private' / config_name
        if path.exists():
            taken += [int(k.split('-')[1]) for k in json.loads(path.read_bytes()).get('populations', {})
                      if re.fullmatch(r'population-\d+', k)]
    bound_id = 'population-%02d' % (max(taken) + 1)
    inputs = dict(inventory=str((directory / 'functions.jsonl').relative_to(REPOSITORY)),
                  gaps=str((directory / 'unassigned-executable.jsonl').relative_to(REPOSITORY)))
    config['populations'][bound_id] = inputs
    bound_record = dict(id=bound_id, inventory_sha256=sha((directory / 'functions.jsonl').read_bytes()),
                        gaps_sha256=sha((directory / 'unassigned-executable.jsonl').read_bytes()))
    catalog['populations'].append(bound_record)

    table = members[0]['references']
    order = max(r['source_order'] for r in catalog['regions']) + 1
    added = []
    for entry in sorted(entries):
        bound = rows[entry]
        end = int(bound['ranges'][0]['end_inclusive'], 16) + 1
        body = audit.original(entry, end - entry)
        padding_hex, _ = gap_padding(audit.original, gaps, end)
        region = new_region(entry, body, padding_hex, population=pid, bound_population=bound_id, source=source,
                            symbol_prefix=symbols.get(entry) or drafted[entry]['symbol'], options=members[0]['options'],
                            references=table, source_sha256=sha(data), source_order=order)
        order += 1
        if overlapping(catalog, entry, region['full_extent_bytes']):
            raise SystemExit('%08x: its bound overlaps accepted regions' % entry)
        catalog['regions'].append(region); added.append(region)
    for row in members:
        if row['source'] == source:     # the population may compile other files before the module
            row['source_sha256'] = sha(data)

    output = (args.output or REPOSITORY / 'private/original-target-20261020' / ('addrow-' + name)).resolve()
    if output.exists():
        raise SystemExit('output exists: %s' % output)
    if not any(output.is_relative_to(REPOSITORY / n) for n in ('private', 'build')):
        raise SystemExit('output must be under ignored private/ or build/')
    output.mkdir(parents=True)
    backup = output / 'backup'
    files = {source: data, non_exact.MANIFEST: manifest_text.encode()}
    written = write_files(files, [], backup)
    try:
        bind_changed(catalog, {pid}, {source: data}, config)
        phase('write and bind')
        misses, fresh = verify(catalog, {pid}, config, output, phase)
        if misses:
            (output / 'misses.json').write_text(json.dumps(misses, indent=1) + '\n')
            raise SystemExit('refused, %d regions do not match (tree restored):\n%s' %
                             (len(misses), json.dumps(misses[:20], indent=1)))
        try:
            validate_modules(REPOSITORY, catalog)
        except ValueError as error:
            raise SystemExit('refused (tree restored): %s' % error)
        # Provenance as apply_rename.py records it.
        folder = output / pid; folder.mkdir()
        rows_now = [{k: v for k, v in r.items() if k != 'references'} for r in catalog['regions'] if r['population'] == pid]
        (folder / 'population.json').write_text(json.dumps(dict(add_row=name, references=table, regions=rows_now)))
        (folder / 'accepted.json').write_text(json.dumps(dict(accepted=[
            {k: v for k, v in r.items() if k != 'references'} for r in fresh['accepted'] if r['population'] == pid])))
        record = next(p for p in catalog['populations'] if p['id'] == pid)
        record['historical_population_sha256'] = sha((folder / 'population.json').read_bytes())
        record['historical_acceptance_sha256'] = sha((folder / 'accepted.json').read_bytes())
        bound_folder = output / bound_id; bound_folder.mkdir()
        (bound_folder / 'population.json').write_text(json.dumps(dict(
            add_row=name, role='frozen bound of rows compiled in %s' % pid, inventory=inputs,
            entries=['%08x' % r['entry'] for r in added])))
        bound_record['historical_population_sha256'] = sha((bound_folder / 'population.json').read_bytes())
        bound_record['historical_acceptance_sha256'] = record['historical_acceptance_sha256']
        for path, content in files.items():
            if (REPOSITORY / path).read_bytes() != content:
                raise SystemExit('refused: %s changed during verification (tree restored)' % path)
        if catalog_io.digest(catalog_path) != digest:
            raise SystemExit('refused: the catalog changed while the batch ran (tree restored)')
        catalog_io.dump(catalog, catalog_path)
    except BaseException:
        restore(backup, written, [])
        raise
    for config_name in CONFIGS:
        path = REPOSITORY / 'private' / config_name
        if path.exists():
            document = json.loads(path.read_bytes())
            document['populations'][bound_id] = inputs
            path.write_text(json.dumps(document, indent=2) + '\n')
    for path in [*(output / 'compile').glob('compile-*/accepted.*'), output / 'compile/fresh-accepted.json']:
        if path.exists():
            path.unlink()
    shutil.rmtree(backup, ignore_errors=True)
    receipt = dict(name=name, unit=args.unit, population=pid, bound_population=bound_id, bound_pass=inputs,
                   rows=[dict(entry='%08x' % r['entry'], symbol=r['symbol'], body_bytes=r['body_bytes'],
                              padding_bytes=r['padding_bytes']) for r in added],
                   regions_verified=fresh['matched_source_regions'], seconds=round(time.monotonic() - started, 1))
    (output / 'receipt.json').write_text(json.dumps(receipt, indent=1) + '\n')
    print(json.dumps(receipt, indent=1))


if __name__ == '__main__':
    main()
