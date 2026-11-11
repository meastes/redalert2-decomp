#!/usr/bin/env python3
"""Merge accepted populations into one translation unit (one new population).

    merge_populations.py POPULATION POPULATION... --name NAME [--drafts DIR] [--run] [--output DIR]

A module conversion needs every row of an original translation unit in one
population (apply_rename.py `modules`), but acceptance history sometimes split
one unit: ccfile.cpp's CCFileClass::Error (0x00469000) was accepted alone in
population-140, the other rows in population-4353. This tool writes the
accept_sources.py plan that re-accepts every row of the listed populations as
one unit, unchanged:

- each row keeps its source text (copied to a draft under DIR, default
  build/merge-NAME/drafts), source path, symbol prefix and semantic name;
- ledger flags (count_calibrated, split_declaration, inline_asm) and their
  notes are carried;
- the unit order is the rows' canonical source_order, so the populations'
  units keep their relative order;
- the plan is all-or-nothing (a merged unit installs whole or not at all).

It refuses when the populations record different option sets (one unit
compiles under one set) or when a population holds a source-form module
(several rows in one file; apply_rename.py owns those).

accept_sources.py does the rest: a composite frozen inventory, one compile of
the merged unit, the strict audit, a new population replacing the old rows
(the old populations are left without rows) and the private config entry. A
row whose code changes when the other population's declarations join its unit
refuses the whole plan; that is the measurement the merge exists to make.
With --run the plan is passed to accept_sources.py; run ./build.command after.
"""
import argparse
import json
from pathlib import Path
import subprocess
import sys

REPOSITORY = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(Path(__file__).resolve().parent))

LEDGER_KEYS = ('inline_asm', 'inline_asm_citation', 'count_calibrated', 'count_calibrated_note',
               'split_declaration', 'split_declaration_note')
DEFAULT_OPTIONS = ['/O2', '/Ob2', '/G5', '/Gy']


def build_plan(catalog, populations, name, draft_dir, read):
    """The accept_sources.py plan merging `populations`; `read(path)` returns a source's bytes.
    Returns (plan, {draft path: bytes})."""
    if len(set(populations)) < 2:
        raise SystemExit('name at least two populations')
    rows = [r for r in catalog['regions'] if r['population'] in populations]
    for pid in populations:
        if not any(r['population'] == pid for r in rows):
            raise SystemExit('%s: no accepted rows' % pid)
    options = {tuple(r['options']) for r in rows}
    if len(options) != 1:
        raise SystemExit('the populations record different option sets: %s' % sorted(options))
    sources = {}
    for row in rows:
        sources.setdefault(row['source'], []).append(row['entry'])
    shared = sorted(s for s, entries in sources.items() if len(entries) > 1)
    if shared:
        raise SystemExit('a population holds a source-form module (several rows in one file): %s' % shared)
    rows.sort(key=lambda r: (r['source_order'], r['entry']))
    items, drafts = [], {}
    for row in rows:
        draft = (Path(draft_dir) / Path(row['source']).name).as_posix()
        if draft in drafts:
            raise SystemExit('two rows share the file name %s' % Path(row['source']).name)
        drafts[draft] = read(row['source'])
        item = dict(entry='%08x' % row['entry'], draft=draft, source=row['source'],
                    symbol_prefix=row['symbol_prefix'])
        if row.get('semantic_name'):
            item['semantic_name'] = row['semantic_name']
        item.update({k: row[k] for k in LEDGER_KEYS if k in row})
        items.append(item)
    plan = dict(name=name, sources=items, all_or_nothing=True,
                merged_populations=sorted(populations))
    option_set = list(options.pop())
    if option_set != DEFAULT_OPTIONS:
        plan['options'] = [option_set]
    return plan, drafts


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('populations', nargs='+')
    parser.add_argument('--name', required=True)
    parser.add_argument('--drafts', type=Path, help='draft directory (default build/merge-NAME/drafts)')
    parser.add_argument('--run', action='store_true', help='pass the plan to accept_sources.py')
    parser.add_argument('--output', type=Path, help='accept_sources.py --output')
    args = parser.parse_args()
    import catalog_io
    catalog = catalog_io.load(catalog_io.locate(REPOSITORY))
    draft_dir = args.drafts or Path('build') / ('merge-' + args.name) / 'drafts'
    if draft_dir.is_absolute():
        draft_dir = draft_dir.resolve().relative_to(REPOSITORY.resolve())
    if draft_dir.parts[0] not in ('build', 'private'):
        raise SystemExit('drafts must be under the ignored build/ or private/')
    plan, drafts = build_plan(catalog, args.populations, args.name, draft_dir,
                              lambda path: (REPOSITORY / path).read_bytes())
    for path, data in drafts.items():
        target = REPOSITORY / path
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(data)
    plan_path = REPOSITORY / draft_dir.parent / 'plan.json'
    plan_path.write_text(json.dumps(plan, indent=1) + '\n')
    print('plan: %s (%d rows from %s)' % (plan_path.relative_to(REPOSITORY), len(plan['sources']),
                                          ', '.join(plan['merged_populations'])))
    if args.run:
        command = [sys.executable, '-B', str(REPOSITORY / 'tools/original_target/accept_sources.py'), str(plan_path)]
        if args.output:
            command += ['--output', str(args.output)]
        raise SystemExit(subprocess.call(command))


if __name__ == '__main__':
    main()
