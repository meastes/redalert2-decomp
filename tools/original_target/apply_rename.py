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
   "flags": {"src/recovered/ownrdraw.cpp": {"declarations_only": true, "declarations_only_note": "..."}},
   "row_flags": {"0040c690": {"unit_evidence": "xcu-prefix"}},
   "address_renames": {"OLD MANGLING": {"new": "NEW MANGLING", "at": ["004ba0b0", ...]}},
   "xcu_rewrites": {"0073bd20": "initializer", "0073bd40": "0073bd20"}}

`xcu_rewrites` (docs/DECISIONS.md "$E initialiser/atexit rows placed by the original .CRT$XCU
table", owner 2026-11-14): rows whose source is rewritten from a seed or stand-in into the
real dynamic-initializer form (a definition the compiler emits `_$E<n>` functions for). Each
listed row gives up its old name and binds the fresh `_$E<n>` by independent evidence: an
`initializer` row by its slot in the original's .CRT$XCU table (the unit's entries of the fresh
object's .CRT$XCU section in order against the original table run the population's rows hold),
a destructor row (value: its initializer's entry) by the atexit registration in the fresh
initializer. The old name is renamed in the row's own population and in the tables of
populations whose rows relocate to it; other populations' tables only held it as a stale copy
and drop it (logged in the receipt).

`address_renames` rename a name only where it is bound at one of the listed addresses: two
template instances the source used to spell alike (`VectorClass<T *>` and `VectorClass<T const *>`
both written `T *`) share every decorated name across populations, and the original's RTTI says
which address is which instance. A region is renamed when its entry is listed, a reference table
entry when its value is, a relocation when its target is, and a `// @ref OLD=ADDRESS` comment when
its address is; the same name bound elsewhere keeps its spelling. After the batch no listed address
may still carry the old name.

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
`"joins_rebound": {"0073bd20": "population-46"}` (owner ruling 2026-11-14, ruling 4): a joining
row takes another frozen independent pass as its `bound_population` (an owner-named row-less
population holding the aligned split of the row's own pass): the pass must bound the row's body
exactly as its frozen one does, and the gap behind it gives the frozen padding (a whole gap of
original NOPs, promote_sweep.gap_padding).
`"joins_options": "unit_gr"` lets a row frozen without /GR take the unit's /GR;
`"joins_options": "unit"` lets a row frozen under any other recorded option set take the
unit's (docs/DECISIONS.md "Stand-ins frozen under another optimisation option than their
unit"; refile_unit.py join-own --adopt-options checks that the unit's set is evidenced).
`flags` sets row fields on every row of a source (a judgment-call flag such as
`declarations_only` that the changed text requires; canonical.validate_catalog checks it).
`row_flags` sets row fields on one row by entry (`unit_evidence`, refile_unit.py join-xcu).
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
import collections
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
    scoped = {}
    for old, item in spec.get('address_renames', {}).items():
        if old in renames or not item.get('new') or item['new'] == old or not item.get('at'):
            raise SystemExit('address_renames: %s needs a new name and addresses, and no plain rename' % old)
        scoped[old] = (item['new'], {int(a, 16) for a in item['at']})
    spec['address_renames'] = scoped
    rewrites = {}
    for entry, kind in spec.get('xcu_rewrites', {}).items():
        if kind != 'initializer' and not re.fullmatch(r'[0-9a-fA-F]{8}', kind or ''):
            raise SystemExit('xcu_rewrites: %s is "initializer" or its initializer\'s entry' % entry)
        rewrites[int(entry, 16)] = kind if kind == 'initializer' else int(kind, 16)
    for entry, kind in rewrites.items():
        if kind != 'initializer' and rewrites.get(kind) != 'initializer':
            raise SystemExit('xcu_rewrites: destructor %08x names %08x, not a listed initializer' % (entry, kind))
    spec['xcu_rewrites'] = rewrites
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
    scoped = spec.get('address_renames') or {}
    if scoped:
        refs = re.compile(r'(@ref\s+)(%s)=(0x)?([0-9a-fA-F]+)' %
                          '|'.join(map(re.escape, sorted(scoped, key=len, reverse=True))))

        def scope(match):
            new, at = scoped[match[2]]
            name = new if int(match[4], 16) in at else match[2]
            return '%s%s=%s%s' % (match[1], name, match[3] or '', match[4])
        text = refs.sub(scope, text)
    return text


def scoped_name(scoped, name, address):
    """The address-scoped rename of `name` bound at `address`, or None."""
    item = scoped.get(name)
    return item[0] if item and address in item[1] else None


def scoped_left(catalog, scoped):
    """(name, address) pairs that still carry an address-scoped old name."""
    left = set()
    for region in catalog['regions']:
        for name, value in (region.get('references') or {}).items():
            if scoped_name(scoped, name, value):
                left.add((name, value))
        for key in ('symbol', 'symbol_prefix'):
            if scoped_name(scoped, region[key], region.get('entry')):
                left.add((region[key], region['entry']))
        for relocation in region.get('relocations') or []:
            if scoped_name(scoped, relocation['symbol'], relocation.get('target')):
                left.add((relocation['symbol'], relocation.get('target')))
    return sorted(left)


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


def rebind_bound(row, holder, config):
    """`joins_rebound`: the row's bound and frozen padding from `holder`'s pass. The pass must
    bound the same body (entry, length, bytes) as the row's frozen pass; the padding is the whole
    NOP gap behind it there."""
    import xcu_units
    from canonical import sha
    from promote_sweep import gap_padding
    if holder not in config['populations']:
        raise SystemExit('joins_rebound: %s is no population' % holder)
    frozen = row.get('bound_population', row['population'])
    parsed = {}
    for pid in (frozen, holder):
        inputs = config['populations'][pid]
        entry = next((json.loads(l) for l in (REPOSITORY / inputs['inventory']).read_text().splitlines()
                      if l.strip() and int(json.loads(l)['entry'], 16) == row['entry']), None)
        if entry is None:
            raise SystemExit('joins_rebound: %s bounds no row at %08x' % (pid, row['entry']))
        parsed[pid] = entry
    body = lambda e: [(r['start'].lower(), r.get('length') or int(r['end_inclusive'], 16) + 1 - int(r['start'], 16),
                       r.get('bytes_hex', '').lower()) for r in e['ranges']]
    if body(parsed[frozen]) != body(parsed[holder]) or len(parsed[holder]['ranges']) != 1:
        raise SystemExit('joins_rebound: %s and %s bound %08x differently' % (frozen, holder, row['entry']))
    gaps = {}
    for line in (REPOSITORY / config['populations'][holder]['gaps']).read_text().splitlines():
        if line.strip():
            gap = json.loads(line); gaps[int(gap['start'], 16)] = gap
    image = xcu_units.Image(Path(config['executable']).read_bytes())
    original = lambda va, n: bytes(image.data[image.offset(va):image.offset(va) + n])
    end = row['entry'] + row['body_bytes']
    padding_hex, gap = gap_padding(original, gaps, end)
    if not gap:
        raise SystemExit('joins_rebound: %s has no whole NOP gap behind %08x' % (holder, row['entry']))
    pad = bytes.fromhex(padding_hex)
    whole = original(row['entry'], row['body_bytes']) + pad
    row.update(bound_population=holder, padding_bytes=len(pad), padding_sha256=sha(pad),
               full_extent_bytes=len(whole), full_extent_sha256=sha(whole))


def gr_only_difference(row_options, unit_options):
    """True when the unit's option set is the row's plus /GR (lane F measurement: a row
    frozen before its unit gained /GR joins under the unit's options)."""
    return '/GR' in unit_options and [o for o in unit_options if o != '/GR'] == list(row_options)


STALE_DROPPED = []     # join_rows: stale table entries dropped (receipt `stale_dropped`)


def stale_names(members, joins, *names):
    """The `names` no row of the unit (its population's rows that are not joining) relocates
    to: entries its reference table holds only as stale copies."""
    used = {r['symbol'] for row in members if row['entry'] not in joins for r in row.get('relocations') or []}
    return {n for n in names if n not in used}


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
                if symbol in stale_names(members, joins, symbol):
                    # docs/DECISIONS.md (owner 2026-11-14): a name no row of the unit relocates to is
                    # a stale table entry, not a conflict; it leaves the table (logged), and the
                    # joining row's binding is compared in full like every used reference.
                    STALE_DROPPED.append(dict(population=pid, name=symbol, was='%08x' % table[symbol],
                                              now='%08x' % target, row='%08x' % row['entry']))
                    del table[symbol]
                    continue
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
            for entry, holder in sorted(item.get('joins_rebound', {}).items()):
                if int(entry, 16) not in joins or item.get('joins_bound') != 'own':
                    raise SystemExit('%s: joins_rebound %s needs an own-bound join of that row' % (module, entry))
                rebind_bound(next(r for r in rows if r['entry'] == int(entry, 16)), holder, config)
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


def rename_catalog(catalog, renames, files, moves, scoped=None):
    """Rename the catalog's names exactly; return the affected population ids."""
    scoped = scoped or {}
    affected = set()
    tables = {}
    for region in catalog['regions']:
        pid = region['population']
        if pid not in tables:
            old = region.get('references') or {}
            table = {}
            for name, value in old.items():
                new = renames.get(name) or scoped_name(scoped, name, value) or name
                if table.setdefault(new, value) != value:
                    raise SystemExit('%s: %s would bind two addresses' % (pid, new))
            tables[pid] = table
            if any(name in renames or scoped_name(scoped, name, value) for name, value in old.items()):
                affected.add(pid)
        region['references'] = tables[pid]
        for key in ('symbol', 'symbol_prefix'):
            new = renames.get(region[key]) or scoped_name(scoped, region[key], region.get('entry'))
            if new:
                region[key] = new; affected.add(pid)
        for relocation in region.get('relocations') or []:
            new = renames.get(relocation['symbol']) or scoped_name(scoped, relocation['symbol'], relocation.get('target'))
            if new:     # the fresh compile rewrites the receipt; known names stay current
                relocation['symbol'] = new; affected.add(pid)
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


XCU_PLACEHOLDER = 900000000     # `_$E<900000000 + entry>`: no unit counter reaches it


def xcu_placeholder(entry):
    return '_$E%d' % (XCU_PLACEHOLDER + entry)


def place_xcu_rewrites(catalog, rewrites):
    """Rename each `xcu_rewrites` row's old name to its placeholder `_$E<9xxxxxxxx>` (bound to a
    fresh `_$E<n>` after the compile, bind_xcu_rewrites). The rename reaches the row's own
    population and every population whose rows relocate to the old name; any other table held
    the old name only as a stale copy and drops it. Returns (affected populations,
    {population: [dropped names]})."""
    by_entry = {r['entry']: r for r in catalog['regions']}
    renames, homes = {}, {}
    for entry in sorted(rewrites):
        row = by_entry.get(entry)
        if row is None:
            raise SystemExit('xcu_rewrites: %08x is not an accepted row' % entry)
        renames[row['symbol']] = xcu_placeholder(entry)
        homes[row['symbol']] = (row['population'], entry)
    users = collections.defaultdict(set)
    for row in catalog['regions']:
        for relocation in row.get('relocations') or []:
            if relocation['symbol'] in renames:
                users[relocation['symbol']].add(row['population'])
    affected, dropped, tables = set(), collections.defaultdict(list), {}
    for row in catalog['regions']:
        pid = row['population']
        if pid not in tables:
            old = row.get('references') or {}
            table = {}
            for name, value in old.items():
                if name in renames:
                    home, entry = homes[name]
                    if pid != home and pid not in users[name]:
                        dropped[pid].append(name)
                        continue
                    if value != entry:
                        raise SystemExit('xcu_rewrites: %s: %s is bound at %08x, not %08x' % (pid, name, value, entry))
                    name = renames[name]
                if table.setdefault(name, value) != value:
                    raise SystemExit('%s: %s would bind two addresses' % (pid, name))
            tables[pid] = table
            if table != old:
                affected.add(pid)
        row['references'] = tables[pid]
        for key in ('symbol', 'symbol_prefix'):
            if row[key] in renames or (key == 'symbol_prefix' and row['entry'] in rewrites):
                row[key] = xcu_placeholder(row['entry']); affected.add(pid)
        for relocation in row.get('relocations') or []:
            if relocation['symbol'] in renames:
                relocation['symbol'] = renames[relocation['symbol']]; affected.add(pid)
    for pid in affected:
        home = {p for p, _ in homes.values()}
        if pid not in home:
            continue
        table = tables[pid]
        for name, (p, entry) in homes.items():
            if p == pid:
                table.setdefault(xcu_placeholder(entry), entry)
    return affected, {p: sorted(n) for p, n in dropped.items()}


def xcu_table(executable, entries):
    """The original's C++ initializer table as a list of initializer addresses (xcu_units)."""
    import xcu_units
    image = xcu_units.Image(Path(executable).read_bytes())
    return [value for _, value in xcu_units.table_slots(image, entries)]


def bind_xcu_rewrites(catalog, compiled, load, rewrites, table):
    """{population: {placeholder: fresh `_$E<n>`}} for the `xcu_rewrites` rows: an initializer
    takes the name the fresh object's .CRT$XCU section holds at its slot of the original table
    run (the run is the one window of the table, as long as the section, whose entries are all
    rows of the population, that holds the row's entry); a destructor takes the one other `_$E`
    its fresh initializer relocates to (the atexit registration)."""
    import initializer_rebind
    if not rewrites:
        return {}
    index = {}
    for slot, address in enumerate(table):
        index.setdefault(address, slot)
    result = {}
    for group, obj in compiled:
        pid = group[0]['population']
        mine = [e for e in rewrites if any(r['entry'] == e for r in group)]
        if not mine:
            continue
        coff = load(obj)
        names = []
        for section in coff.sections:
            if section['name'] == '.CRT$XCU':
                names += [coff.symbols[i]['name'] for _, i, _ in sorted(section['relocs'])]
        population = {r['entry'] for r in catalog['regions'] if r['population'] == pid}
        mapping = {}
        for entry in sorted(e for e in mine if rewrites[e] == 'initializer'):
            slot = index.get(entry)
            if slot is None:
                raise SystemExit('xcu_rewrites: %08x is not in the original .CRT$XCU table' % entry)
            starts = [s for s in range(max(0, slot - len(names) + 1), slot + 1)
                      if s + len(names) <= len(table) and all(a in population for a in table[s:s + len(names)])]
            if len(starts) != 1:
                raise SystemExit('xcu_rewrites: %08x: %d windows of the original table fit the %d entries of '
                                 '%s\'s .CRT$XCU' % (entry, len(starts), len(names), pid))
            mapping[xcu_placeholder(entry)] = names[slot - starts[0]]
        for entry in sorted(e for e in mine if rewrites[e] != 'initializer'):
            initializer = mapping.get(xcu_placeholder(rewrites[entry]))
            extent = initializer_rebind.function_extent(coff, initializer) if initializer else None
            targets = sorted({n for *_, n in (extent[1] if extent else [])
                              if initializer_rebind.is_initializer(n) and n != initializer})
            if len(targets) != 1:
                raise SystemExit('xcu_rewrites: %08x: its initializer registers %d destructors' % (entry, len(targets)))
            mapping[xcu_placeholder(entry)] = targets[0]
        if len(set(mapping.values())) != len(mapping):
            raise SystemExit('xcu_rewrites: %s: two rows bind one fresh name' % pid)
        result.setdefault(pid, {}).update(mapping)
    return result


def rebind_initializers(catalog, compiled, load, original, fixed=None):
    """Re-bind the compiler-numbered `_$E<n>` initializers of each compiled unit
    by content and position (initializer_rebind.py): a header change that
    renumbers them changes no byte. `fixed` ({population: {old: fresh}},
    bind_xcu_rewrites) is taken as given. Returns {population: {old: new}} for the
    populations whose names moved; a population whose units disagree keeps
    its names (and refuses as before). `load` parses a compiled object."""
    import initializer_rebind
    by_population = {}
    for group, obj in compiled:
        pid = group[0]['population']
        table = group[0].get('references') or {}
        if initializer_rebind.uses_initializers(group, table):
            by_population.setdefault(pid, []).append(
                initializer_rebind.group_mapping(load(obj), group, table, original,
                                                 fixed=(fixed or {}).get(pid)))
    rebound = {}
    for pid, mappings in sorted(by_population.items()):
        mapping = initializer_rebind.merge_population_mappings(mappings)
        if mapping:
            initializer_rebind.rebind_rows([r for r in catalog['regions'] if r['population'] == pid], mapping)
            rebound[pid] = mapping
    return rebound


def verify(catalog, populations, config, output, phase, rebound_out=None, rewrites=None):
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
    load = lambda obj: Coff(obj.read_bytes())
    fixed = bind_xcu_rewrites(catalog, compiled, load, rewrites, xcu_table(
        config['executable'], {r['entry'] for r in catalog['regions']})) if rewrites else {}
    rebound_out.update(rebind_initializers(catalog, compiled, load, audit.original, fixed))
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
    rewritten, stale = place_xcu_rewrites(catalog, spec['xcu_rewrites']) if spec['xcu_rewrites'] else (set(), {})
    populations = rename_catalog(catalog, spec['renames'], sources, moves, spec['address_renames']) | modules | rewritten
    unknown = [p for p in sources if p not in {r['source'] for r in catalog['regions']}]
    if unknown:
        raise SystemExit('changed files are not accepted sources: %s' % unknown[:5])
    users = header_users(catalog, headers, populations) if use_header_users(args.header_users, headers, populations) else set()
    if not populations and not users:
        raise SystemExit('refused: the batch changes no accepted source, name or reference and no header user was '
                         'selected, so nothing would be verified')
    phase('plan')
    summary = dict(name=spec['name'], renames=spec['renames'],
                   address_renames={old: dict(new=new, at=['%08x' % a for a in sorted(at)])
                                    for old, (new, at) in spec['address_renames'].items()},
                   changed_sources=sorted(sources),
                   changed_headers=headers, moved=moves, affected_populations=sorted(populations),
                   affected_regions=sum(r['population'] in populations for r in catalog['regions']),
                   header_user_populations=len(users),
                   xcu_rewrites={'%08x' % e: (k if k == 'initializer' else '%08x' % k)
                                 for e, k in sorted(spec['xcu_rewrites'].items())},
                   xcu_rewrites_stale_dropped=stale, stale_dropped=STALE_DROPPED)
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
        for entry, fields in spec.get('row_flags', {}).items():
            rows = [r for r in catalog['regions'] if r['entry'] == int(entry, 16)]
            if not rows:
                raise SystemExit('row_flags: %s is not an accepted row' % entry)
            rows[0].update(fields)
        bind_changed(catalog, populations, sources, config)
        left = leftover(catalog_io.dumps(catalog), spec['renames']) + scoped_left(catalog, spec['address_renames'])
        if left:    # checked again on the final text; this refuses before compiling
            raise SystemExit('refused: old names left after the rename (tree restored): %s' % left[:10])
        phase('write and bind')
        before = {r['entry']: json.dumps(r, sort_keys=True) for r in catalog['regions'] if r['population'] in users}
        rebound = {}
        misses, manifest = verify(catalog, populations | users, config, output, phase, rebound,
                                  rewrites=spec['xcu_rewrites'])
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
        left = sorted(set(leftover(text, spec['renames'])) | set(scoped_left(catalog, spec['address_renames'])) |
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
