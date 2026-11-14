#!/usr/bin/env python3
"""Apply a signature/name rename batch to accepted source in one pass.

    apply_rename.py SPEC [--config CONFIG] [--output DIR] [--dry-run] [--header-users]

A rename (a parameter retyped, a function, field or global renamed) changes
source text and manglings across many accepted populations without changing
a single original byte. Instead of one whole-population plan per affected
population through accept_sources.py, this applies the batch once and
re-verifies exactly the affected translation units:

  {"name": "screen26-ren2",
   "renames": {"OLD MANGLING": "NEW MANGLING", ...},   (or a path to such a JSON file)
   "mangling_rewrite": [{"pattern": "REGEX", "replace": "TEXT"}, ...],
   "rewrite": [{"pattern": "REGEX", "replace": "TEXT"}, ...],
   "headers": {"src/shared/techno.hpp": "build/class-work/W/shared/techno.hpp"},
   "header_base": "COMMIT the header copies were made from",
   "sources": {"src/recovered/.../X.cpp": "build/class-work/W/d/X.cpp"},
   "moves": {"src/recovered/.../Old_0076e730.cpp": "src/recovered/.../New_0076e730.cpp"},
   "modules": {"src/recovered/version.cpp": {"draft": "build/class-work/W/version.cpp",
                                              "regions": ["00712b20", "00712b50", ...]}},
   "options": {"population-3705": ["/O2", "/Ob2", "/G5", "/Gy", "/GR"]},
   "flags": {"src/recovered/ownrdraw.cpp": {"declarations_only": true, "declarations_only_note": "..."}}}

`modules` gathers accepted regions of one population into one source file
(the original translation unit's form): the draft becomes the module source,
every listed region's row points at it (its old file is removed, its
semantic name dropped), and the population recompiles as one unit like any
other. `options` sets a population's recorded option set (one of
canonical.RECORDED_OPTIONS), e.g. /GR for a unit whose emitted RTTI and
vtable are then compared with the original's.
A module item may list `joins`: accepted regions of the same original unit that live
in other populations (acceptance history split the unit). A joining row moves into the
module's population only as it is: same option set, the same frozen bound and padding
provenance in the module population's inventory pass (so no private input changes), and
no name its accepted relocations bind at another address than the module population's
table does (the unit's own table has precedence; a joining row adds only names the table
lacks, never a compiler-numbered one). Both populations recompile and every region of
both must still reproduce its complete extent (refile_unit.py drives this).
With `"joins_bound": "own"` (docs/DECISIONS.md "Route A split rows", option (ii)) a
joining row whose bound the module population's pass does not hold, or holds differently,
still joins the module's translation unit: it keeps the population that froze it as
`bound_population`, and the audit reads its bound, gap padding and provenance from that
population's pass (canonical.bound_population). Nothing else is relaxed: same option
set, the unit's table has precedence, neither population has a closed compiler-output
policy, and the row must reproduce its complete extent in the module's unit.
`"joins_options": "unit_gr"` lets a row frozen without /GR take the unit's /GR;
`"joins_options": "unit"` lets a row frozen under any other recorded option set take the
unit's (docs/DECISIONS.md "Stand-ins frozen under another optimisation option than their
unit"; refile_unit.py join-own --adopt-options checks that the unit's set is evidenced).
`flags` sets row fields on every row of a source (a judgment-call flag such as
`declarations_only` that the changed text requires; canonical.validate_catalog checks it).
A module item may also give `inline_asm_citation`: {entry: [citation, ...]} for its
evidenced inline-assembly rows, restated for the per-function attribution (an asm-only
archive macro counts once per invocation: docs/DECISIONS.md).

`mangling_rewrite` rules expand into further renames: every name in the
catalog and symbols.json they change (the expanded list is in the receipt).
Header copies are merged onto the tree with `git merge-file TREE BASE COPY`
(a conflict refuses). Then every tracked source and shared header gets the
`rewrite` rules (Python re, in order) and the plain-text renames; `sources`
replaces whole files after that, `moves` renames files. The catalog's names
(symbol, symbol_prefix, population reference tables, relocation receipts) and
manifests/symbols.json are renamed exactly, never by substring.

Affected populations are those holding a changed source or naming an old
mangling. Each is recompiled as whole translation units with its recorded
options (the build's own grouping), every region must reproduce its complete
original extent, and the result goes through the canonical V4 audit used by
build.py (bounds, padding, emitted-section coverage, reference and operand
checks). With --header-users, units that include a changed header are also
recompiled and must verify with their manifest rows unchanged. A header-only
spec (no affected population) implies --header-users.

Compiler-numbered names (`_$E<n>` initializers and atexit destructors, `$S<n>`
static-local guard flags) follow a unit's declaration count, so a header change
renumbers them without changing a byte: after compiling, each unit's names are
re-bound by content and position (initializer_rebind.py; the map is kept as
rebound-initializers.json) in the rows, reference tables, receipts and
`// @ref` comments, before the strict audit compares every byte as usual. A
header user may change only by that re-binding and its `$T<n>` scope-table
spelling (folded as verify_manifest.receipt_key folds it).

Any miss, unbound reference or old name left anywhere refuses: every written
file is restored and the catalog is untouched. On success the catalog is
written once. Run ./build.command afterwards; it remains the final gate.
"""
import argparse
import json
from pathlib import Path
import re
import shutil
import subprocess
import sys
import time

REPOSITORY = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(Path(__file__).resolve().parent))

from canonical import CanonicalAudit, compile_group_key, separate_unit_populations, sha, validate_includes
from source_paths import source_path
import catalog_io

CATALOG = catalog_io.locate(REPOSITORY)
SYMBOLS = REPOSITORY / 'manifests/symbols.json'


def load_spec(path, catalog):
    spec = json.loads(Path(path).read_text())
    renames = spec.get('renames', {})
    if isinstance(renames, str):
        renames = json.loads((REPOSITORY / renames).read_text())
    rules = [(re.compile(r['pattern']), r['replace']) for r in spec.get('mangling_rewrite', [])]
    if rules:
        # Expand mangling rules over every name the catalog and symbols.json hold.
        names = {n for region in catalog['regions'] for n in
                 [region['symbol'], region['symbol_prefix'], *(region.get('references') or {}),
                  *(r['symbol'] for r in region.get('relocations') or [])]}
        names |= {s['mangled_prefix'] for s in json.loads(SYMBOLS.read_text())['symbols'] if s.get('mangled_prefix')}
        for name in sorted(names):
            new = name
            for pattern, replace in rules:
                new = pattern.sub(replace, new)
            if new != name and renames.setdefault(name, new) != new:
                raise SystemExit('conflicting renames for %s' % name)
    if any(old == new or not old or not new for old, new in renames.items()) or \
            len(set(renames.values())) != len(renames) or set(renames) & set(renames.values()):
        raise SystemExit('renames must map distinct old names to distinct, new names')
    spec['renames'] = renames
    spec['rules'] = [(re.compile(r['pattern'], re.M), r['replace']) for r in spec.get('rewrite', [])]
    return spec


def rewrite(text, spec):
    for pattern, replace in spec['rules']:
        text = pattern.sub(replace, text)
    renames = spec['renames']
    if renames:
        # One pass, longest name first: a replacement is never renamed again.
        names = re.compile('|'.join(map(re.escape, sorted(renames, key=len, reverse=True))))
        text = names.sub(lambda match: renames[match[0]], text)
    return text


def leftover(text, renames):
    """Old names still present once every new name is blanked (a new name may contain an old one)."""
    for new in sorted(renames.values(), key=len, reverse=True):
        text = text.replace(new, '\0')
    return sorted(old for old in renames if old in text)


def merged_header(path, copy, base):
    """The tree header with the worker's change (copy against base) merged in."""
    import tempfile
    original = subprocess.run(['git', 'show', '%s:%s' % (base, path)], cwd=REPOSITORY,
                              capture_output=True, check=True).stdout
    with tempfile.TemporaryDirectory() as scratch:
        base_file = Path(scratch) / 'base'; base_file.write_bytes(original)
        merged = subprocess.run(['git', 'merge-file', '-p', str(REPOSITORY / path), str(base_file),
                                 str(REPOSITORY / copy)], capture_output=True)
    if merged.returncode:
        raise SystemExit('header merge conflict: %s' % path)
    return merged.stdout


def planned_files(spec):
    """{path: new bytes} for every tracked source/header the batch changes, plus removed paths."""
    files = {}
    for path, copy in spec.get('headers', {}).items():
        if not spec.get('header_base'):
            raise SystemExit('header copies need header_base (the commit they were made from)')
        if not path.startswith('src/shared/'):
            raise SystemExit('header outside src/shared: %s' % path)
        files[path] = merged_header(path, copy, spec['header_base']) if (REPOSITORY / path).exists() else \
            (REPOSITORY / copy).read_bytes()
    tracked = sorted(p.relative_to(REPOSITORY).as_posix() for root in ('src/recovered', 'src/shared')
                     for p in (REPOSITORY / root).rglob('*') if p.suffix in ('.cpp', '.hpp', '.h'))
    for path in sorted(set(tracked) | set(files)):
        current = files[path] if path in files else (REPOSITORY / path).read_bytes()
        text = rewrite(current.decode('latin-1'), spec).encode('latin-1')
        if not (REPOSITORY / path).exists() or text != (REPOSITORY / path).read_bytes():
            files[path] = text
    for path, draft in spec.get('sources', {}).items():
        source_path(REPOSITORY, path)
        files[path] = (REPOSITORY / draft).read_bytes()
    removed = []
    for old, new in spec.get('moves', {}).items():
        source_path(REPOSITORY, old); source_path(REPOSITORY, new)
        files[new] = files.pop(old, None) or (REPOSITORY / old).read_bytes()
        removed.append(old)
    return files, removed


COMPILER_NUMBERED = re.compile(r'^(?:_?\$[ES]\d+|\$T\d+)$')


def frozen_bounds(config):
    """bound(population, row): what the population's frozen inventory pass says about
    the row's extent (its inventory row and the gap row behind its body), or None when
    the pass has no row at that entry. Equal values mean the audit reads the same
    independent bound for the row under either population."""
    parsed = {}

    def table(path, key):
        if (path, key) not in parsed:
            rows = {}
            for line in (REPOSITORY / path).read_text().splitlines():
                if line.strip():
                    row = json.loads(line)
                    rows[int(row[key], 16)] = json.dumps(row, sort_keys=True)
            parsed[path, key] = rows
        return parsed[path, key]

    def bound(pid, row):
        inputs = config['populations'][pid]
        inventory = table(inputs['inventory'], 'entry')
        if row['entry'] not in inventory:
            return None
        return inventory[row['entry']], table(inputs['gaps'], 'start').get(row['entry'] + row['body_bytes'])
    return bound


def gr_only_difference(row_options, unit_options):
    """True when the unit's option set is the row's plus /GR (lane F measurement: a row
    frozen before its unit gained /GR joins under the unit's options)."""
    return '/GR' in unit_options and [o for o in unit_options if o != '/GR'] == list(row_options)


def join_rows(catalog, module, rows, joins, config, own_bound=False, adopt_gr=False, adopt_options=False, renumber=None):
    """Move the `joins` rows into the module's population (see the module docstring);
    return the populations they leave. `own_bound`: a row the module population's pass
    does not freeze equally keeps its own frozen population as `bound_population`.
    `adopt_gr` (`"joins_options": "unit_gr"`): a row whose option set is the unit's without
    /GR joins and takes the unit's options (it must still reproduce its complete extent).
    `adopt_options` (`"joins_options": "unit"`): a row under any other recorded option set
    takes the unit's options (the same condition).
    `renumber` ({entry: {old: new}}, `"joins_renumber"`): a joining row's unit-counter names
    (`_$E<n>`, `_$S<n>`) take the numbers refile_unit.py gave them past the unit's, so the
    re-binding after the compile (rebind_initializers) never meets two rows with one name."""
    home = {r['population'] for r in rows if r['entry'] not in joins}
    if len(home) != 1:
        raise SystemExit('%s: regions of several populations' % module)
    pid = home.pop()
    members = [r for r in catalog['regions'] if r['population'] == pid]
    table = dict(members[0].get('references') or {})
    options = members[0]['options']
    bound = frozen_bounds(config)
    policies = {p['id'] for p in catalog.get('populations', []) if p.get('compiler_output_policy')}
    left = set()
    for row in rows:
        if row['entry'] not in joins or row['population'] == pid:
            continue
        name = '%s: join %08x' % (module, row['entry'])
        mapping = (renumber or {}).get(row['entry'])
        if mapping:
            if any(not COMPILER_NUMBERED.match(n) or not COMPILER_NUMBERED.match(v) for n, v in mapping.items()):
                raise SystemExit('%s: joins_renumber names only compiler-numbered names' % name)
            for key in ('symbol', 'symbol_prefix'):
                row[key] = mapping.get(row.get(key), row.get(key))
            for relocation in row.get('relocations') or []:
                relocation['symbol'] = mapping.get(relocation['symbol'], relocation['symbol'])
            if row.get('references'):
                row['references'] = {mapping.get(n, n): v for n, v in row['references'].items()}
        if row['options'] != options:
            if not (adopt_options or (adopt_gr and gr_only_difference(row['options'], options))):
                raise SystemExit('%s: recorded under another option set' % name)
            row['options'] = list(options)
        frozen = row.get('bound_population', row['population'])
        own = bound(frozen, row)
        here = bound(pid, row)
        if frozen == pid:
            row.pop('bound_population', None)       # back in the population that froze it
        elif here is None or here != own:
            if not own_bound:
                raise SystemExit('%s: %s holds no equal frozen bound for it' % (name, pid))
            if own is None:
                raise SystemExit('%s: %s holds no frozen bound for it' % (name, frozen))
            if {pid, frozen} & policies:
                raise SystemExit('%s: a closed compiler-output population' % name)
            row['bound_population'] = frozen
        else:
            row.pop('bound_population', None)       # the unit's pass freezes it equally
        own = {}
        for relocation in row.get('relocations') or []:
            own.setdefault(relocation['symbol'], relocation['target'])
        for symbol, target in sorted(own.items()):
            if not COMPILER_NUMBERED.match(symbol) and table.get(symbol, target) != target:
                raise SystemExit('%s: %s is %08x in %s and %08x in the row' % (name, symbol, table[symbol], pid, target))
        # An own-bound join brings only the names its accepted relocations bind, never its
        # donor population's whole table (another unit's COMDAT copies of the unit's own
        # names would re-bind the unit's vtables and stand-ins).
        brought = own if 'bound_population' in row else {**(row.get('references') or {}), **own}
        # A renumbered row's unit-counter names are its own placeholders (no unit name has their
        # numbers): it brings them, so the re-binding after the compile finds their addresses.
        placeholders = set((mapping or {}).values())
        for symbol, target in sorted(brought.items()):
            if not COMPILER_NUMBERED.match(symbol) or symbol in placeholders:
                table.setdefault(symbol, target)
        left.add(row['population'])
        row['population'] = pid
        # The build joins a group's sources in the order of their first rows: a joining row
        # must not move the module ahead of the sources (switch files) its population
        # already compiles before it. It takes a fresh order behind every row.
        if row['source_order'] < min(r['source_order'] for r in rows if r['entry'] not in joins):
            row['source_order'] = max(r['source_order'] for r in catalog['regions']) + 1
    for row in catalog['regions']:
        if row['population'] == pid:
            row['references'] = table
    return left


def apply_modules(catalog, spec, files, removed, config=None):
    """Point the listed regions at their module source (see the module docstring);
    return the affected population ids."""
    from canonical import RECORDED_OPTIONS
    affected = set()
    for module, item in spec.get('modules', {}).items():
        source_path(REPOSITORY, module)
        joins = {int(e, 16) for e in item.get('joins', [])}
        entries = {int(e, 16) for e in item['regions']} | joins
        rows = [r for r in catalog['regions'] if r['entry'] in entries]
        if len(rows) != len(entries):
            raise SystemExit('%s: %d of %d regions are accepted' % (module, len(rows), len(entries)))
        if joins:
            if item.get('joins_bound', 'equal') not in ('equal', 'own'):
                raise SystemExit('%s: joins_bound is "equal" or "own"' % module)
            if item.get('joins_options', 'same') not in ('same', 'unit_gr', 'unit'):
                raise SystemExit('%s: joins_options is "same", "unit_gr" or "unit"' % module)
            affected |= join_rows(catalog, module, rows, joins, config, item.get('joins_bound') == 'own',
                                  item.get('joins_options') == 'unit_gr', item.get('joins_options') == 'unit',
                                  {int(e, 16): m for e, m in item.get('joins_renumber', {}).items()})
        if len({r['population'] for r in rows}) != 1:
            raise SystemExit('%s: regions of several populations' % module)
        files[module] = (REPOSITORY / item['draft']).read_bytes()
        # Evidenced inline assembly rows restate their citations for the module's
        # per-function attribution (canonical.module_inline_asm_blocks); the flag stays.
        citations = {int(e, 16): c for e, c in item.get('inline_asm_citation', {}).items()}
        for row in rows:
            if row['entry'] in citations:
                if row.get('inline_asm') != 'evidenced':
                    raise SystemExit('%s: %08x is not an evidenced inline-asm row' % (module, row['entry']))
                row['inline_asm_citation'] = citations[row['entry']]
        for row in rows:
            if row['source'] != module and row['source'] not in removed:
                removed.append(row['source'])
            row['source'] = module
            row.pop('semantic_name', None)
            affected.add(row['population'])
    for path in removed:
        files.pop(path, None)
    for pid, options in spec.get('options', {}).items():
        if options not in RECORDED_OPTIONS:
            raise SystemExit('%s: options %s are not a recorded option set' % (pid, options))
        rows = [r for r in catalog['regions'] if r['population'] == pid]
        if not rows:
            raise SystemExit('%s: no such population' % pid)
        for row in rows:
            row['options'] = options
        affected.add(pid)
    return affected


def rename_catalog(catalog, renames, files, moves):
    """Rename the catalog's names exactly; return the affected population ids."""
    affected = set()
    tables = {}
    for region in catalog['regions']:
        pid = region['population']
        if pid not in tables:
            old = region.get('references') or {}
            table = {}
            for name, value in old.items():
                new = renames.get(name, name)
                if table.setdefault(new, value) != value:
                    raise SystemExit('%s: %s would bind two addresses' % (pid, new))
            tables[pid] = table
            if any(name in renames for name in old):
                affected.add(pid)
        region['references'] = tables[pid]
        for key in ('symbol', 'symbol_prefix'):
            if region[key] in renames:
                region[key] = renames[region[key]]; affected.add(pid)
        for relocation in region.get('relocations') or []:
            if relocation['symbol'] in renames:     # the fresh compile rewrites the receipt;
                relocation['symbol'] = renames[relocation['symbol']]; affected.add(pid)   # known names stay current
        if region['source'] in moves:
            region['source'] = moves[region['source']]
            if region.get('semantic_name'):
                region['semantic_name'] = Path(region['source']).stem
        if region['source'] in files:
            affected.add(pid)
    return affected


def header_users(catalog, headers, populations):
    """Populations outside `populations` whose sources include a changed header (transitively)."""
    include = re.compile(rb'#\s*include\s+"([^"]+)"')
    graph = {p.relative_to(REPOSITORY / 'src').as_posix(): set(include.findall(p.read_bytes()))
             for p in (REPOSITORY / 'src/shared').rglob('*') if p.suffix in ('.h', '.hpp')}
    changed = {h[len('src/'):] for h in headers}
    reaches = {}
    def reach(name):
        if name not in reaches:
            reaches[name] = False
            reaches[name] = name in changed or any(reach(n.decode()) for n in graph.get(name, ()))
        return reaches[name]
    users = set()
    for region in catalog['regions']:
        if region['population'] in populations or region['population'] in users:
            continue
        if any(reach(n.decode()) for n in include.findall((REPOSITORY / region['source']).read_bytes())):
            users.add(region['population'])
    return users


def bind_changed(catalog, populations, files, config):
    """Bindings a changed source adds, exactly as accept_sources.py derives them
    (@ref comments, then known names the text mentions, RTTI under /GR); the
    population's existing (renamed) table keeps precedence."""
    import diff_candidate as diff
    from accept_sources import bind, header_parts, rtti_references, with_headers
    known = diff.known_references(catalog)
    audit = None
    for pid in sorted(populations):
        rows = [r for r in catalog['regions'] if r['population'] == pid]
        if not rows:
            continue        # every row of the population left through `joins`
        table = rows[0]['references']
        for row in rows:
            if row['source'] not in files:
                continue
            text = files[row['source']].decode('latin-1')
            for name, value in diff.SOURCE_REF.findall(text):
                if table.setdefault(name, int(value, 16)) != int(value, 16):
                    raise SystemExit('%s: @ref %s differs from the accepted binding' % (row['source'], name))
            for name, value in bind(header_parts(text), known).items():
                table.setdefault(name, value)
            if '/GR' in row['options']:
                if audit is None:
                    from verify_manifest import Audit
                    audit = Audit(REPOSITORY, config['executable'])
                for name, value in rtti_references(with_headers(text), audit.original_type_descriptors()).items():
                    table.setdefault(name, value)


def rebind_source_refs(catalog, rebound, files):
    """{path: new bytes} for accepted sources of re-bound populations whose
    `// @ref _$E<n>=` comments name a re-bound initializer; each row's
    source_sha256 follows."""
    import initializer_rebind
    changed = {}
    for row in catalog['regions']:
        mapping = rebound.get(row['population'])
        if not mapping:
            continue
        path = row['source']
        if path not in changed:
            current = files[path] if path in files else (REPOSITORY / path).read_bytes()
            text = initializer_rebind.rebind_source_refs(current.decode('latin-1'), mapping).encode('latin-1')
            changed[path] = text if text != current else None
        if changed[path] is not None:
            row['source_sha256'] = sha(changed[path])
    return {p: d for p, d in changed.items() if d is not None}


def header_user_drifted(text, mapping, after, refs):
    """True when a header user's row changed beyond what a header change may
    renumber: its initializers re-bound (`mapping`, with the `// @ref` comment
    hash that follows them) and its `$T<n>` scope-table spelling (folded as
    verify_manifest.receipt_key folds it; the table is bound by `$T`)."""
    from verify_manifest import receipt_key
    row = json.loads(text)
    if mapping:
        import initializer_rebind
        initializer_rebind.rebind_rows([row], mapping)
    if row['source'] in refs:
        row['source_sha256'] = after['source_sha256']
    key = lambda r: json.dumps(dict(r, relocations=receipt_key(r.get('relocations') or [])), sort_keys=True)
    return key(row) != key(after)


def use_header_users(requested, headers, populations):
    """A batch that changes headers but no accepted source, name or binding (a
    header-only spec) has no affected population, so the header users are its
    only verification: they are always recompiled then."""
    return bool(requested or (headers and not populations))


def write_files(files, removed, backup):
    """Write the batch; every overwritten or removed file is copied to `backup` first."""
    written = []
    for path in list(files) + removed:
        target = REPOSITORY / path
        if target.exists():
            copy = backup / path; copy.parent.mkdir(parents=True, exist_ok=True)
            copy.write_bytes(target.read_bytes())
    for path, data in files.items():
        target = REPOSITORY / path
        written.append((path, target.exists()))
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(data)
    for path in removed:
        (REPOSITORY / path).unlink()
    return written


def restore(backup, written, removed):
    for path, existed in written:
        target = REPOSITORY / path
        if existed:
            target.write_bytes((backup / path).read_bytes())
        elif target.exists():
            target.unlink()
    for path in removed:
        (REPOSITORY / path).write_bytes((backup / path).read_bytes())


def rebind_initializers(catalog, compiled, load, original):
    """Re-bind the compiler-numbered `_$E<n>` initializers of each compiled unit
    by content and position (initializer_rebind.py): a header change that
    renumbers them changes no byte. Returns {population: {old: new}} for the
    populations whose names moved; a population whose units disagree keeps
    its names (and refuses as before). `load` parses a compiled object."""
    import initializer_rebind
    by_population = {}
    for group, obj in compiled:
        pid = group[0]['population']
        table = group[0].get('references') or {}
        if initializer_rebind.uses_initializers(group, table):
            by_population.setdefault(pid, []).append(
                initializer_rebind.group_mapping(load(obj), group, table, original))
    rebound = {}
    for pid, mappings in sorted(by_population.items()):
        mapping = initializer_rebind.merge_population_mappings(mappings)
        if mapping:
            initializer_rebind.rebind_rows([r for r in catalog['regions'] if r['population'] == pid], mapping)
            rebound[pid] = mapping
    return rebound


def verify(catalog, populations, config, output, phase, rebound_out=None):
    """Compile `populations` as whole canonical units and run the strict audit;
    re-bind renumbered `_$E<n>` initializers (rebind_initializers), then record
    each region's fresh symbol and relocation receipt."""
    rebound_out = {} if rebound_out is None else rebound_out
    from build import compile_sources
    from compile_candidates import compare_candidate
    from coff import Coff
    rows = [r for r in catalog['regions'] if r['population'] in populations]
    (output / 'compile').mkdir()
    compiled = compile_sources(config, dict(catalog, regions=rows), output / 'compile')
    phase('compile')
    audit = CanonicalAudit(REPOSITORY, catalog, config)
    phase('audit setup')
    rebound_out.update(rebind_initializers(catalog, compiled, lambda obj: Coff(obj.read_bytes()), audit.original))
    misses = []
    for group, obj in compiled:
        coff = Coff(obj.read_bytes())
        for row in group:
            candidate = dict(row, method='%08x' % row['entry'], region_bytes=row['full_extent_bytes'])
            result = compare_candidate(audit, coff, candidate, obj, row['options'], 0)
            if result['status'] != 'extent-exact':
                misses.append(dict(entry='%08x' % row['entry'], source=row['source'], status=result['status'],
                                   reason=result.get('reason'), differing_bytes=result.get('differing_bytes')))
                continue
            row['symbol'] = result['symbol']
            row['relocations'] = result['relocations']
    if misses:
        return misses, None
    manifest = audit.verify_fresh(output / 'compile', compiled, complete=False)
    try:
        audit.verify_inputs_unchanged()
    except ValueError as error:
        raise SystemExit('refused: %s' % error)
    phase('strict audit')
    return [], manifest


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('spec', type=Path)
    parser.add_argument('--config', type=Path, default=REPOSITORY / 'private/original-target.local.json')
    parser.add_argument('--output', type=Path, help='new ignored directory (default private/.../rename-<name>)')
    parser.add_argument('--dry-run', action='store_true', help='report what would change; write nothing')
    parser.add_argument('--header-users', action='store_true',
                        help='also recompile and verify unchanged units that include a changed header')
    args = parser.parse_args()
    started = time.monotonic(); marks = [started]; timings = {}
    def phase(name):
        now = time.monotonic(); timings[name] = round(now - marks[-1], 1); marks.append(now)
        print('phase %s: %.1fs' % (name, timings[name]), file=sys.stderr, flush=True)
    config = json.loads(args.config.read_bytes())
    catalog_blobs = {}
    catalog = catalog_io.load(CATALOG, blobs=catalog_blobs)
    catalog_digest = catalog_io.digest(CATALOG, catalog_blobs)
    spec = load_spec(args.spec, catalog)
    phase('load')
    files, removed = planned_files(spec)
    moves = spec.get('moves', {})
    modules = apply_modules(catalog, spec, files, removed, config)
    sources = {p: d for p, d in files.items() if p.startswith('src/recovered/')}
    headers = sorted(p for p in files if p.startswith('src/shared/'))
    populations = rename_catalog(catalog, spec['renames'], sources, moves) | modules
    unknown = [p for p in sources if p not in {r['source'] for r in catalog['regions']}]
    if unknown:
        raise SystemExit('changed files are not accepted sources: %s' % unknown[:5])
    users = header_users(catalog, headers, populations) if use_header_users(args.header_users, headers, populations) else set()
    if not populations and not users:
        raise SystemExit('refused: the batch changes no accepted source, name or reference and no header user was '
                         'selected, so nothing would be verified')
    phase('plan')
    summary = dict(name=spec['name'], renames=spec['renames'], changed_sources=sorted(sources),
                   changed_headers=headers, moved=moves, affected_populations=sorted(populations),
                   affected_regions=sum(r['population'] in populations for r in catalog['regions']),
                   header_user_populations=len(users))
    if args.dry_run:
        print(json.dumps(summary, indent=1)); return
    output = (args.output or REPOSITORY / 'private/original-target-20261020' / ('rename-' + spec['name'])).resolve()
    if output.exists():
        raise SystemExit('output exists: %s' % output)
    if not any(output.is_relative_to(REPOSITORY / name) for name in ('private', 'build')):
        raise SystemExit('output must be under ignored private/ or build/')
    output.mkdir(parents=True)
    backup = output / 'backup'
    # symbols.json: exact JSON string tokens only, so the file keeps its layout.
    symbols = SYMBOLS.read_text()
    renamed = symbols
    for old, new in spec['renames'].items():
        renamed = renamed.replace(json.dumps(old), json.dumps(new))
    if renamed != symbols:
        json.loads(renamed)
        files['manifests/symbols.json'] = renamed.encode()
    written = write_files(files, removed, backup)
    try:
        from regenerate_seeds import sync_headers
        for region in catalog['regions']:
            if region['source'] in sources:
                region['source_sha256'] = sha(sources[region['source']])
                if region.get('source_quality_family') == 'ghidra-seed-sweep':
                    region['source_quality_family'] = 'hand-written'    # as accept_sources records it
        sync_headers(catalog)
        for source, fields in spec.get('flags', {}).items():
            rows = [r for r in catalog['regions'] if r['source'] == source]
            if not rows:
                raise SystemExit('flags: %s is not an accepted source' % source)
            for row in rows:
                row.update(fields)
        bind_changed(catalog, populations, sources, config)
        left = leftover(catalog_io.dumps(catalog), spec['renames'])
        if left:    # checked again on the final text; this refuses before compiling
            raise SystemExit('refused: old names left after the rename (tree restored): %s' % left[:10])
        phase('write and bind')
        before = {r['entry']: json.dumps(r, sort_keys=True) for r in catalog['regions'] if r['population'] in users}
        rebound = {}
        misses, manifest = verify(catalog, populations | users, config, output, phase, rebound)
        if rebound:
            (output / 'rebound-initializers.json').write_text(json.dumps(rebound, indent=1, sort_keys=True) + '\n')
        if misses:
            (output / 'misses.json').write_text(json.dumps(misses, indent=1) + '\n')
            raise SystemExit('refused, %d regions no longer match (tree restored):\n%s' %
                             (len(misses), json.dumps(misses[:20], indent=1)))
        # The receipts are fresh now: references into a source-form module must name its symbols.
        from canonical import validate_modules
        try:
            validate_modules(REPOSITORY, catalog)
        except ValueError as error:
            raise SystemExit('refused (tree restored): %s' % error)
        # Re-bound initializer names: `// @ref _$E<n>=` comments follow the catalog
        # (comment text only; the compiled bytes are unchanged).
        refs = rebind_source_refs(catalog, rebound, files)
        if refs:
            written += write_files({p: d for p, d in refs.items() if p not in files}, [], backup)
            for path in set(refs) & set(files):     # already written (and backed up) by the batch
                (REPOSITORY / path).write_bytes(refs[path])
            files.update(refs)
        drifted = [r['source'] for r in catalog['regions'] if r['entry'] in before and
                   header_user_drifted(before[r['entry']], rebound.get(r['population']), r, refs)]
        if drifted:
            raise SystemExit('refused: header users outside the batch changed: %s' % drifted[:5])
        # Provenance as accept_sources.py records it: the frozen population and
        # its verified acceptance, here one per affected population.
        accepted = {}
        for row in manifest['accepted']:
            accepted.setdefault(row['population'], []).append(row)
        for population in catalog['populations']:
            pid = population['id']
            if pid not in populations or pid not in accepted:
                continue        # (a population every row left through `joins` keeps its record, without rows)
            folder = output / pid; folder.mkdir()
            rows = [{k: v for k, v in r.items() if k != 'references'}
                    for r in catalog['regions'] if r['population'] == pid]
            table = next(r['references'] for r in catalog['regions'] if r['population'] == pid)
            (folder / 'population.json').write_text(json.dumps(dict(rename=spec['name'], references=table, regions=rows)))
            (folder / 'accepted.json').write_text(json.dumps(dict(accepted=[
                {k: v for k, v in r.items() if k != 'references'} for r in accepted[pid]])))
            population['historical_population_sha256'] = sha((folder / 'population.json').read_bytes())
            population['historical_acceptance_sha256'] = sha((folder / 'accepted.json').read_bytes())
        text = catalog_io.dumps(catalog)
        left = sorted(set(leftover(text, spec['renames'])) |
                      {old for path, data in files.items() for old in leftover(data.decode('latin-1'), spec['renames'])})
        if left:
            raise SystemExit('refused: old names left after the rename (tree restored): %s' % left[:10])
        for path, data in files.items():
            if (REPOSITORY / path).read_bytes() != data:
                raise SystemExit('refused: %s changed during verification (tree restored)' % path)
        if catalog_io.digest(CATALOG) != catalog_digest:
            raise SystemExit('refused: the catalog changed while the batch ran (tree restored)')
        catalog_io.dump(catalog, CATALOG)
    except BaseException:
        restore(backup, written, removed)
        raise
    phase('write catalog')
    # Keep the logs, receipts and verification report; the objects are several GB.
    for path in [*(output / 'compile').glob('compile-*/accepted.*'), output / 'compile/fresh-accepted.json']:
        path.unlink()
    shutil.rmtree(backup, ignore_errors=True)
    summary.update(regions_verified=manifest['matched_source_regions'], body_bytes=manifest['body_bytes'],
                   seconds=round(time.monotonic() - started, 1), timings=timings,
                   spec_sha256=sha(args.spec.read_bytes()), catalog_sha256=catalog_io.digest(CATALOG))
    (output / 'receipt.json').write_text(json.dumps(summary, indent=1) + '\n')
    print(json.dumps(dict({k: v for k, v in summary.items() if k not in ('renames', 'changed_sources', 'affected_populations')},
                          renames=len(spec['renames']), changed_sources=len(sources),
                          affected_populations=len(populations)), indent=1))


if __name__ == '__main__':
    main()
