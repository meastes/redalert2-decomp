#!/usr/bin/env python3
"""Re-file one original translation unit into one source (Route A, docs/DECISIONS.md).

    refile_unit.py unit-map --from FUNCTIONS.json      write manifests/original_target/unit_map.json
    refile_unit.py list [--top N]                      units by accepted bytes, with their canonical group
    refile_unit.py plan UNIT                           what `run` would do (no compiler)
    refile_unit.py run UNIT [--form hoist|inplace|verbatim] [--no-joins] [--attempts N]
    refile_unit.py native [--check]                    re-key native path-keyed data after a run
    refile_unit.py causes                              re-state recorded causes: no absolute local paths; refused
                                                       joins that the plan's header-state screen now holds back
    refile_unit.py join UNIT ENTRY                     retry one residual row of a re-filed unit alone
    refile_unit.py join-template-batch UNIT...         the same for several units in one apply_rename batch
    refile_unit.py join-template UNIT [--dry-run]      container members (shared/vector_class.hpp) frozen in
                                                       other populations into the unit, each on its own bound
        [--family vector|class_factory|type_list]      the one template header whose members join (default
                                                       vector; class_factory: shared/class_factory.hpp,
                                                       type_list: shared/type_list.hpp)
    refile_unit.py maps BUILD_DIR                      module maps from a --keep-intermediates build

The canonical build already compiles each population as one translation unit (its
sources joined in source order). `run` pours a unit's rows of its largest population
(its canonical compile group) into `src/recovered/<unit>.cpp`, functions in address
order, and applies it through apply_rename.py `modules` (one source, several catalog
rows, one population): the population recompiles as one unit, every region must
reproduce its complete original extent, and the strict audit runs as for any batch.
The unit keeps its own header state: its `#define` switches and its includes, in their
original order, gathered into one block at the top with repeats dropped. Names are
unchanged; `// @ref` lines and seed markers are dropped (the population's reference
table already binds every name; the module map carries the addresses).

Nothing blocks (rule 3). When a row no longer matches under the gathered block, the
block is held where the row needs it: lines of later files stay behind that row (an
include block mid-file, recorded in the map with the row that needs it); if that is
not enough, the files up to that row keep their lines in place; when holding stalls
(no new row to hold, no further file to leave in place) the next form keeps every file's
lines in place (the canonical unit text with repeats dropped) and the last is the
canonical text itself. A row that differs even there is not exact in any form: it stays
in its own file as a residual of kind "flipped" with the comparator's reason, and the
remaining rows start again from the first form. No source line is ever re-written to
make a row match. Rows that cannot join the file stay in their existing files and are
listed in the map as residuals with the cause:

  - a row whose file holds `__asm` (evidenced inline assembly keeps its per-file
    attribution);
  - a row of the unit living in another population: it joins only if it is exact
    under the unit's state (same option set, the same frozen bound in the unit's
    inventory pass, no name bound to another address by the unit's own table, and
    the strict comparison after the move, in both populations). A candidate whose file
    brings header state the unit lacks (shared/ghidra_types.hpp, another include, another
    switch) is screened out before any compile and stays "split" with that cause. The map lists a row
    held back before any compile as kind "split" (entry, bytes, holding population:
    the work list for a later script) and one that was moved and differed as kind
    "flipped" with its cause; rows that stay by rule are kind "kept";
  - a row the lane names with --keep ENTRY=CAUSE (for example a native check that
    reads per file and would misread the row inside the unit's file).

Binding precedence is the unit's own: the population's reference table (header
file-statics and the compiler-numbered `$S`/`_$E` names bind to the unit's accepted
relocation targets; apply_rename re-binds renumbered names by content and position).
A joining row adds only names the unit's table does not bind; there is no name-map
binding.

A row whose native fixup cannot be re-keyed into the poured text (its text cannot be made
unique in the module) is found before anything is applied: `run` keeps it in its own file
(kind "kept") and pours the rest again, so the tree is never converted with native data
that no longer applies.

`native` re-keys native/port/fixups.json, native/drafts.json host paths,
native/port/view_layouts.json, native/behaviour_rewrites.json and the path column of
native/data/vtable_views.tsv to the module path and regenerates native/initterm.json,
so the native configure checks stay green. `maps` runs module_ledger.py --write.
"""
import argparse
import collections
import json
from pathlib import Path
import re
import subprocess
import sys
import time

REPOSITORY = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(Path(__file__).resolve().parent))

import apply_rename
import catalog_io

UNIT_MAP = REPOSITORY / 'manifests/original_target/unit_map.json'
MODULES = REPOSITORY / 'manifests/original_target/modules.json'
PENDING = 'build/refile'      # ignored scratch: drafts, specs, apply_rename outputs, moved-path records

SEED_MARKER = ('// seed-generated: tools/original_target/regenerate_seeds.py may rewrite this file;',
               '// delete this line when editing it by hand.')
REF_LINE = re.compile(r'^\s*//\s*@ref\s+\S+=(?:0x)?[0-9a-fA-F]+\s*$')
DIRECTIVE = re.compile(r'^\s*#\s*(\w+)')
INCLUDE = re.compile(r'^\s*#\s*include\s*(["<][^">]+[">])')
DEFINE = re.compile(r'^\s*#\s*define\s+(\w+)(\()?')
UNDEF = re.compile(r'^\s*#\s*undef\s+(\w+)')
COMPILER_NUMBERED = re.compile(r'^(?:_?\$[ES]\d+|\$T\d+|_\$E\d+)$')


# ----------------------------------------------------------------------------- unit map

def load_unit_map(path=UNIT_MAP):
    """{unit: [entry, ...]} (ints, address order) from the tracked unit map."""
    document = json.loads(Path(path).read_text())
    return {unit: sorted(int(e, 16) for e in entries) for unit, entries in document['units'].items()}


def write_unit_map(functions, target=UNIT_MAP):
    """The salvage assignment (ra2-decomp twin map plus mischa85 unit ranges, build/salvage/ra2decomp
    cols.py: FUNCTIONS.json is its {unit: [{entry: '0x...'}]} output) as a tracked, byte-free map."""
    units = {unit: sorted({'%08x' % int(f['entry'], 16) for f in rows}) for unit, rows in sorted(functions.items())}
    seen = {}
    for unit, entries in units.items():
        for entry in entries:
            if seen.setdefault(entry, unit) != unit:
                raise SystemExit('%s is assigned to %s and %s' % (entry, seen[entry], unit))
    lines = ['{', ' "about": "Original translation unit of each function entry (Route A unit map): the salvage '
             'assignment from the ra2-decomp twin map (build/salvage/ra2decomp/twins_detail.tsv) and mischa85/RedAlert2 '
             'config/ra2-1.006/modules.csv unit ranges; unit names are the original source file stems. Addresses only; '
             'written by tools/original_target/refile_unit.py unit-map.",', ' "units": {']
    lines += ['  %s: [%s]%s' % (json.dumps(unit), ', '.join('"%s"' % e for e in entries),
                                '' if index == len(units) - 1 else ',')
              for index, (unit, entries) in enumerate(units.items())]
    lines += [' }', '}']
    catalog_io.write_text_lf(Path(target), '\n'.join(lines) + '\n')
    return units


# ----------------------------------------------------------------------------- the pour

def split_lines(text):
    """[(kind, text)] for every logical line (a backslash-continued directive is one line).
    kind: include | define | undef (top level only: outside every #if) | ref | marker | other."""
    out, depth = [], 0
    lines = text.split('\n')
    index = 0
    while index < len(lines):
        line = lines[index]
        logical = line
        while logical.rstrip('\r').endswith('\\') and DIRECTIVE.match(logical.split('\n')[0]) and index + 1 < len(lines):
            index += 1
            logical += '\n' + lines[index]
        index += 1
        directive = DIRECTIVE.match(logical)
        kind = 'other'
        if directive:
            word = directive.group(1)
            if word in ('if', 'ifdef', 'ifndef'):
                depth += 1
            elif word == 'endif':
                depth = max(0, depth - 1)
            elif depth == 0 and word in ('include', 'define', 'undef'):
                kind = word
        elif REF_LINE.match(logical):
            kind = 'ref'
        elif logical.strip() in SEED_MARKER:
            kind = 'marker'
        out.append((kind, logical))
    return out


def directive_key(kind, line):
    """What makes two directives repeats of each other: the header an include names; a
    define's or undef's whole text without trailing comment and spacing."""
    if kind == 'include':
        match = INCLUDE.match(line)
        return ('include', match.group(1)) if match else ('include', line.strip())
    bare = re.sub(r'\s*//[^\n]*$', '', line.split('\n')[0]) + line[len(line.split('\n')[0]):]
    return (kind, ' '.join(bare.split()))


def pour(files, form='hoist', held=(), in_place=(), banner=None):
    """One module text from the unit's files.

    files: [(entry, path, text)] in address order. form: 'hoist' gathers each segment's
    top-level include/define/undef lines, in their original order with repeats dropped,
    into a block at the segment's start; a new segment starts after every entry in
    `held` (so no later line moves ahead of that row). A file whose entry is in
    `in_place` (and every file under form 'inplace') keeps its lines where they are,
    repeats dropped. 'verbatim' joins the files unchanged except for the dropped
    `// @ref` and seed-marker lines.
    Returns (text, info): info has blocks [(first entry of the segment, [lines])],
    switches (object-like macro names the unit defines), chunks {path: body text}."""
    seen = set()
    active = {}                 # macro name -> defining key (an #undef makes a later define count again)
    blocks, body_parts, chunks, switches = [], [], {}, []
    segment = None

    def keep(kind, line):
        if form == 'verbatim':
            return True
        key = directive_key(kind, line)
        if kind == 'include':
            if key in seen:
                return False
            seen.add(key)
            return True
        if kind == 'define':
            name = DEFINE.match(line).group(1)
            if active.get(name) == key:
                return False
            active[name] = key
            return True
        name = UNDEF.match(line)
        if name:
            active.pop(name.group(1), None)
        return True

    for index, (entry, path, text) in enumerate(files):
        gather = form == 'hoist' and entry not in in_place
        if form == 'hoist' and (segment is None or (index and files[index - 1][0] in held)):
            segment = dict(first=entry, lines=[], slot=len(body_parts))
            blocks.append(segment)
            body_parts.append(None)         # the block's place
        body = []
        for kind, line in split_lines(text):
            if kind in ('ref', 'marker'):
                continue
            match = DEFINE.match(line)      # (a guarded define counts as a switch too)
            if match and not match.group(2) and match.group(1) not in switches:
                switches.append(match.group(1))
            if kind in ('include', 'define', 'undef'):
                if not keep(kind, line):
                    continue
                if gather:
                    segment['lines'].append(line)
                    continue
            body.append(line)
        chunk = '\n'.join(body).strip('\n')
        chunks[path] = chunk
        body_parts.append(chunk)
    parts = []
    if banner:
        parts.append(banner.rstrip('\n'))
    for index, part in enumerate(body_parts):
        if part is None:
            block = next(b for b in blocks if b['slot'] == index)
            if block['lines']:
                parts.append('\n'.join(block['lines']))
        elif part:
            parts.append(part)
    text = '\n\n\n'.join(parts) + '\n'
    info = dict(blocks=[(b['first'], list(b['lines'])) for b in blocks if b['lines']], switches=switches, chunks=chunks)
    return text, info


def banner_text(unit, population, form):
    return ('// %s.cpp: the original translation unit re-filed as one source (Route A): the accepted\n'
            '// bodies of its canonical compile group in address order, under the unit\'s own header\n'
            '// state (its switches and includes in their original order). The module map\n'
            '// manifests/original_target/modules/%s.json carries the addresses and the rows that\n'
            '// stay in their own files.\n' % (unit, unit))


# ----------------------------------------------------------------------------- planning

def unit_rows(catalog, entries):
    wanted = set(entries)
    return sorted((r for r in catalog['regions'] if r['entry'] in wanted), key=lambda r: r['entry'])


def main_group(rows):
    """The unit's canonical compile group: the (population, option set) holding most of its
    accepted bytes. One population may record two option sets; each compiles as its own unit."""
    weight = collections.Counter()
    for row in rows:
        weight[row['population'], tuple(row['options'])] += row['body_bytes']
    return max(sorted(weight), key=lambda key: weight[key]) if weight else (None, None)


def main_population(rows):
    return main_group(rows)[0]


def plan_unit(catalog, unit, entries, read, modules=None, joins=True, bounds=None):
    """Which rows pour into the unit's file, which may try to join, which stay and why.

    read(path) -> text. bounds(population, row) -> a comparable frozen bound, None when the
    population's inventory pass has no row for it (apply_rename.frozen_bounds); bounds=None
    skips that comparison.
    Returns dict(population, options, members, candidates, residuals, foreign, module)."""
    rows = unit_rows(catalog, entries)
    counts = collections.Counter(r['source'] for r in catalog['regions'])
    module_sources = {item['source'] for item in (modules or {}).values()}
    residuals, members, candidates = [], [], []

    def residual(row, kind, cause):
        residuals.append(residual_row(row, kind, cause))
    free = []
    for row in rows:
        if row['source'] in module_sources or counts[row['source']] > 1:
            residual(row, 'kept', 'already in the module source %s' % row['source'])
        else:
            free.append(row)
    pid, options = main_group(free)
    options = list(options) if options is not None else None
    table = next((r.get('references') or {} for r in free if r['population'] == pid), {})
    separate = pid in {p['id'] for p in catalog.get('populations', []) if p.get('separate_units')}
    for row in free:
        if re.search(r'\b_?_asm\b', read(row['source'])):     # (`__asm` or `_asm`)
            residual(row, 'kept', 'evidenced inline assembly: the row keeps its per-file attribution')
        elif row['population'] == pid and row['options'] == options and separate:
            residual(row, 'kept', '%s compiles each source alone (separate units): no unit file to join' % pid)
        elif row['population'] == pid and row['options'] == options:
            members.append(row)
        elif row['population'] == pid:
            residual(row, 'split', 'held by %s under another option set (%s), a compile group of its own'
                     % (pid, ' '.join(row['options'])))
        elif not joins:
            residual(row, 'split', 'held by %s (joins not tried in this run)' % row['population'])
        elif row['options'] != options:
            residual(row, 'split', 'held by %s under another option set (%s)' % (row['population'], ' '.join(row['options'])))
        elif bounds is not None and (bounds(pid, row) is None or bounds(pid, row) != bounds(row['population'], row)):
            residual(row, 'split', 'frozen in another population\'s inventory pass (%s)' % row['population'])
        else:
            conflict = join_conflict(row, table, read(row['source']))
            if conflict:
                residual(row, 'split', 'held by %s; binding: %s' % (row['population'], conflict))
            else:
                candidates.append(row)
    if candidates:
        # Screen before any compile: a row whose file brings header state the unit lacks
        # (shared/ghidra_types.hpp, another include, another switch) is not attempted.
        state = unit_state([read(r['source']) for r in members], read)
        screened = []
        for row in candidates:
            cause = header_state_cause(read(row['source']), state)
            if cause:
                residual(row, 'split', 'held by %s; %s' % (row['population'], cause))
            else:
                screened.append(row)
        candidates = screened
    if candidates:
        # A join may not empty a population: the catalog keeps no population without rows
        # (apply_rename binds each population through its first row). Its rows stay split.
        sizes = collections.Counter(r['population'] for r in catalog['regions'])
        leaving = collections.Counter(r['population'] for r in candidates)
        emptied = {p for p, n in leaving.items() if n == sizes[p]}
        for row in [r for r in candidates if r['population'] in emptied]:
            residual(row, 'split', 'held by %s; the join would leave that population without rows' % row['population'])
        candidates = [r for r in candidates if r['population'] not in emptied]
    foreign = [r for r in catalog['regions'] if r['population'] == pid and r['options'] == options and
               r['entry'] not in set(entries)]
    return dict(unit=unit, population=pid, options=options, members=members, candidates=candidates,
                residuals=residuals, foreign=len(foreign), module='src/recovered/%s.cpp' % unit)


# An absolute local path: an optional drive, a leading separator at a token start, folders.
# A repo-relative path (src/recovered/x.cpp) has no leading separator and is left alone.
LOCAL_PATH = re.compile(r'(?<![\w.\-])(?:[A-Za-z]:)?[\\/](?:[^\s\\/:]+[\\/])+')


def scrub_cause(text):
    """A recorded cause as it may stand in a tracked manifest: the folders of every absolute
    local path are dropped (the file name stays), so no cause names a checkout, a worktree
    or a home directory; white space is single."""
    return ' '.join(LOCAL_PATH.sub('', text).split())


def restate_causes(modules):
    """Pass every recorded residual and held-row cause of the Route A entries through
    scrub_cause, in place. Returns [(unit, entry, old, new)] for the causes that changed."""
    changed = []
    for unit, item in modules.items():
        if item.get('route') != 'A':
            continue
        for row in item.get('residuals', []) + item.get('held_rows', []):
            new = scrub_cause(row.get('cause', ''))
            if new != row.get('cause', ''):
                changed.append((unit, row['entry'], row['cause'], new))
                row['cause'] = new
    return changed


def unit_state(texts, read):
    """The header state of a unit: the top-level include/define/undef lines of its texts
    (its members' files, or its module source) and the headers they include, transitively.
    read(path) -> text; a path that is not there is skipped."""
    def safe(path):
        try:
            return read(path)
        except (KeyError, OSError):
            return None
    text = '\n'.join(texts)
    return module_state(text, include_names(text, safe))


def header_state_cause(text, state):
    """Why a row's file cannot join a unit as it is, or None: the top-level include/define/
    undef lines it has and the unit's state lacks."""
    own = own_lines(text, state)
    if not own:
        return None
    return 'needs header state the unit does not have: %s' % '; '.join(' '.join(line.split()) for line in own)[:200]


REFUSED_JOIN = 'the unit does not compile or verify with the rows joining from other populations'


def reclassify_refused_joins(modules, read):
    """Re-state rows recorded as "flipped" because a join attempt was refused: under the
    plan's screen a row whose file brings header state its unit lacks is never attempted,
    so it is "split" with that cause. In place, Route A entries only; rows that pass the
    screen stay as they are. Returns [(unit, entry, cause)]."""
    changed = []
    for unit, item in modules.items():
        if item.get('route') != 'A':
            continue
        rows = [r for r in item.get('residuals', []) if r.get('kind') == 'flipped' and r.get('cause', '').startswith(REFUSED_JOIN)]
        if not rows:
            continue
        try:
            state = unit_state([read(item['source'])], read)
        except (KeyError, OSError):
            continue
        for row in rows:
            try:
                cause = header_state_cause(read(row['source']), state)
            except (KeyError, OSError):
                cause = None
            if cause:
                row['kind'] = 'split'
                row['cause'] = scrub_cause('held by %s; %s' % (row['population'], cause))
                changed.append((unit, row['entry'], row['cause']))
    return changed


def residual_row(row, kind, cause):
    """A row of the unit that stays in its own file. kind: 'split' = held by another frozen
    population (entry, bytes and the holding population are what a later script needs to
    pick it up); 'flipped' = not exact when moved into the unit's file; 'kept' = stays by
    rule (evidenced inline assembly, a row already in another module source)."""
    return dict(kind=kind, entry='%08x' % row['entry'], bytes=row['body_bytes'], population=row['population'],
                source=row['source'], cause=scrub_cause(cause))


def join_conflict(row, table, text):
    """Why a row of another population cannot bind under the unit's table, or None.
    The unit's table has precedence: a name the row was accepted with at another address
    (a header file-static of the other unit, another copy of a COMDAT) is a real
    difference, not something to re-bind. Compiler-numbered names are re-bound by
    content and position after the compile (apply_rename), so they never conflict here."""
    own = {}
    for relocation in row.get('relocations') or []:
        own.setdefault(relocation['symbol'], relocation['target'])
    for name, value in re.findall(r'@ref\s+([^\s=]+)=(?:0x)?([0-9a-fA-F]+)', text):
        own.setdefault(name, int(value, 16))
    for name, target in sorted(own.items()):
        if COMPILER_NUMBERED.match(name):
            continue
        if name in table and table[name] != target:
            return '%s is %08x in the unit and %08x in the row\'s population' % (name, table[name], target)
    return None


def join_references(row, table):
    """Names a joining row adds to the unit's table: those the table does not bind
    (never a compiler-numbered name: the fresh compile numbers those)."""
    added = {}
    for name, target in sorted((row.get('references') or {}).items()):
        if name not in table and not COMPILER_NUMBERED.match(name):
            added[name] = target
    for relocation in row.get('relocations') or []:
        name = relocation['symbol']
        if name not in table and not COMPILER_NUMBERED.match(name):
            added.setdefault(name, relocation['target'])
    return added


# ----------------------------------------------------------------------------- native re-key

def rekey_fixups(fixups, moved, module_text, chunks):
    """Re-key native fixups of moved sources to the module path.

    A fixup is a replace-all of `old` in its file; in the module the same text may occur
    in other functions, so each occurrence in the file's own chunk is widened backwards
    by whole lines until it is unique in the module. Fixups apply in list order, so each
    is placed against the text the earlier ones leave (a widened text may hold a line an
    earlier fixup already changed). Returns (new fixups, problems)."""
    out, problems = [], []
    current = module_text
    chunks = dict(chunks)
    for fixup in fixups:
        path = fixup['source']
        if path not in moved:
            out.append(fixup)
            continue
        module, old, new = moved[path], fixup['old'], fixup['new']
        chunk = chunks.get(path, '')
        hits = [m.start() for m in re.finditer(re.escape(old), chunk)]
        if not hits:
            if current.count(old) == 1:       # a gathered line (the include block)
                out.append(dict(fixup, source=module))
                current = current.replace(old, new)
            else:
                problems.append('%s: fixup text %r is not in its part of %s' % (path, old[:50], module))
            continue
        if current.count(old) == len(hits):
            out.append(dict(fixup, source=module))
            current = current.replace(old, new)
            chunks[path] = chunk.replace(old, new)
            continue
        if chunk not in current:
            problems.append('%s: its text is not contiguous in %s' % (path, module))
            continue
        # Last occurrence first: a widened text reaches backwards only, so placing the later
        # occurrences first leaves the earlier ones' positions and context as they are, and
        # the fixups apply in the order they are listed.
        for hit in reversed(hits):
            chunk = chunks[path]
            start = hit
            wide_old, wide_new = old, new
            while current.count(wide_old) != 1 and start > 0:
                previous = chunk.rfind('\n', 0, start - 1)
                start = previous + 1 if previous >= 0 else 0
                wide_old = chunk[start:hit] + old
                wide_new = chunk[start:hit] + new
            if current.count(wide_old) != 1 or chunk.count(wide_old) != 1:
                problems.append('%s: fixup text %r cannot be made unique in %s' % (path, old[:50], module))
                break
            out.append(dict(fixup, source=module, old=wide_old, new=wide_new))
            current = current.replace(wide_old, wide_new)
            chunks[path] = chunk.replace(wide_old, wide_new)
    return out, problems


NATIVE_NOT_UNIQUE = 'native fixup cannot be made unique in the module'


def unplaceable_fixups(fixups, moved, module_text, chunks):
    """{source: cause} for the moved sources whose native fixups cannot be re-keyed into the
    module text (checked before anything is applied; the same placement rekey_native makes:
    the fixups in list order, each against the text the earlier ones leave)."""
    out = collections.OrderedDict()
    text, parts = module_text, dict(chunks)
    for fixup in fixups:
        path = fixup['source']
        if path not in moved:
            continue
        fixed, errors = rekey_fixups([fixup], moved, text, parts)
        if errors:
            detail = errors[0].split(': ', 1)[-1]
            out.setdefault(path, NATIVE_NOT_UNIQUE if 'cannot be made unique' in detail else
                           'native fixup cannot be re-keyed into the module: %s' % detail)
            continue
        for item in fixed:
            text = text.replace(item['old'], item['new'])
            parts[path] = parts.get(path, '').replace(item['old'], item['new'])
    return out


def native_fixups(repository=REPOSITORY):
    path = repository / 'native/port/fixups.json'
    return json.loads(path.read_text())['fixups'] if path.exists() else []


def rekey_native(moved, repository=REPOSITORY, write=True):
    """Re-key native path-keyed data for {old source: module source}; returns (summary, problems)."""
    summary, problems = {}, []
    texts = {module: (repository / module).read_text(errors='replace') for module in set(moved.values())}
    # The chunks as the module holds them: re-derive each file's part from the recorded pour.
    chunks = {}
    for record in sorted((repository / PENDING).glob('*/moved.json')):
        chunks.update(json.loads(record.read_text()).get('chunks', {}))
    path = repository / 'native/port/fixups.json'
    if path.exists():
        raw = path.read_text()
        document = json.loads(raw)
        before = document['fixups']
        # One pass per module over its fixups in list order (they apply in that order);
        # each re-keyed fixup (or the fixups it widens into) keeps its place in the list.
        placed = {}
        for module in sorted(set(moved.values())):
            subset = {p: m for p, m in moved.items() if m == module}
            for index, fixup in enumerate(before):
                if fixup['source'] in subset:
                    placed.setdefault(module, []).append(index)
            pending = [before[i] for i in placed.get(module, [])]
            state_text, state_chunks = texts[module], dict(chunks)
            for index, fixup in zip(placed.get(module, []), pending):
                fixed, errors = rekey_fixups([fixup], subset, state_text, state_chunks)
                problems += errors
                for item in fixed:
                    state_text = state_text.replace(item['old'], item['new'])
                    state_chunks[fixup['source']] = state_chunks.get(fixup['source'], '').replace(item['old'], item['new'])
                placed[module, index] = fixed
        after = []
        for index, fixup in enumerate(before):
            if fixup['source'] in moved:
                after += placed.get((moved[fixup['source']], index), [])
            else:
                after.append(fixup)
        summary['fixups'] = sum(1 for f in before if f['source'] in moved)
        document['fixups'] = after
        if write and summary['fixups'] and not problems:
            # the file's own JSON style (so a re-key changes only the moved rows)
            for style in (dict(indent=1, ensure_ascii=False), dict(indent=1), dict(indent=2, ensure_ascii=False)):
                if json.dumps(dict(document, fixups=before), **style) + '\n' == raw:
                    break
            catalog_io.write_text_lf(path, json.dumps(document, **style) + '\n')
    path = repository / 'native/drafts.json'
    if path.exists():
        text = path.read_text()
        document = json.loads(text)
        count = 0
        for draft in document['drafts']:
            if draft.get('host_source') in moved:
                old = draft['host_source']
                count += 1
                # exact JSON string tokens only, so the file keeps its layout
                text = text.replace('"host_source": %s' % json.dumps(old), '"host_source": %s' % json.dumps(moved[old]))
        summary['draft_hosts'] = count
        if write and count:
            json.loads(text)
            catalog_io.write_text_lf(path, text)
    path = repository / 'native/port/view_layouts.json'
    if path.exists():
        text = path.read_text()
        document = json.loads(text)
        count = 0
        names = collections.Counter()
        for view in document['views']:
            if view['source'] in moved:
                count += 1
                names[(moved[view['source']], view['view'])] += 1
                text = text.replace('"source": %s' % json.dumps(view['source']),
                                    '"source": %s' % json.dumps(moved[view['source']]))
        problems += ['view_layouts.json: view %s is listed %d times for %s' % (view, n, module)
                     for (module, view), n in names.items() if n > 1]
        summary['views'] = count
        if write and count:
            json.loads(text)
            catalog_io.write_text_lf(path, text)
    path = repository / 'native/behaviour_rewrites.json'
    if path.exists():
        text = path.read_text()
        count = 0
        for old, new in moved.items():
            token = json.dumps(old)
            if token in text:
                count += text.count(token)
                text = text.replace(token, json.dumps(new))
        summary['behaviour_rewrites'] = count
        if write and count:
            json.loads(text)
            catalog_io.write_text_lf(path, text)
    path = repository / 'native/data/vtable_views.tsv'
    if path.exists():
        lines = path.read_text().split('\n')
        count = 0
        for index, line in enumerate(lines):
            first, tab, rest = line.partition('\t')
            if tab and first in moved:
                lines[index] = moved[first] + tab + rest
                count += 1
        keys = collections.Counter(tuple(l.split('\t')[:2]) for l in lines if '\t' in l and not l.startswith('#'))
        problems += ['vtable_views.tsv: %s %s is listed %d times' % (k[0], k[1], n) for k, n in keys.items() if n > 1]
        summary['vtable_views'] = count
        if write and count:
            catalog_io.write_text_lf(path, '\n'.join(lines))
    return summary, problems


# ----------------------------------------------------------------------------- run

COMPILE_FAILED = re.compile(r'Compile failed: (\S+)')


def refusal_reason(tail, read_log=None):
    """One line saying why apply_rename refused, fit for a tracked manifest: the compiler's
    first error when the compile failed, otherwise the last log line; folders of local
    paths are dropped either way (a cause never names a checkout or a home directory)."""
    lines = tail.strip().splitlines()
    last = lines[-1] if lines else 'no output'
    failed = COMPILE_FAILED.search(last)
    if failed:
        if read_log is None:
            def read_log(path):
                try:
                    return Path(path).read_text(errors='replace')
                except OSError:
                    return ''
        error = next((line for line in read_log(failed.group(1)).splitlines() if ' error ' in line), None)
        last = 'compile failed: ' + error.strip() if error else 'compile failed'
    return scrub_cause(last)[:200]


def apply_attempt(unit, number, module, text, regions, joins, config_path):
    """One apply_rename.py batch; (ok, misses or None, log tail, seconds)."""
    folder = REPOSITORY / PENDING / unit / ('attempt-%02d' % number)
    if folder.exists():
        raise SystemExit('%s exists (an earlier run of this unit): remove build/refile/%s first' % (folder, unit))
    folder.mkdir(parents=True)
    draft = folder / Path(module).name
    catalog_io.write_text_lf(draft, text)
    item = dict(draft=draft.relative_to(REPOSITORY).as_posix(), regions=['%08x' % e for e in regions])
    if joins:
        item['joins'] = ['%08x' % e for e in joins]
    spec = dict(name='refile-%s-%02d' % (unit, number), modules={module: item})
    catalog_io.write_text_lf((folder / 'spec.json'), json.dumps(spec, indent=1) + '\n')
    started = time.monotonic()
    result = subprocess.run([sys.executable, '-B', str(Path(__file__).with_name('apply_rename.py')),
                             str(folder / 'spec.json'), '--config', str(config_path), '--output', str(folder / 'out')],
                            cwd=REPOSITORY, capture_output=True, text=True)
    seconds = round(time.monotonic() - started, 1)
    catalog_io.write_text_lf((folder / 'apply.log'), result.stdout + result.stderr)
    misses = None
    if (folder / 'out/misses.json').exists():
        misses = json.loads((folder / 'out/misses.json').read_text())
    return result.returncode == 0, misses, (result.stdout + result.stderr)[-1500:], seconds


def after_miss(form, held, in_place, missed, entries, start='hoist'):
    """The next step when rows of the unit itself differ: (form, held, in_place, flipped).
    Gathered form: a row that differs is held; a held row that still differs puts the files up
    to it in place; when neither set grows (holding has stalled) the plainer forms follow:
    every file's lines in place, then the canonical text. Rows that differ in the canonical
    text are not exact in any form: they are returned as flipped (they stay in their own
    files) and the remaining rows start again from `start`."""
    if form == 'hoist':
        fresh = missed - held
        stuck = missed & held
        grown = set(in_place)
        if stuck:
            # Holding the block behind the row was not enough: the files up to it keep their lines.
            last = max(stuck)
            grown |= {e for e in entries if e <= last}
        if not fresh and grown == set(in_place):
            return 'inplace', set(held), grown, set()
        return 'hoist', set(held) | fresh, grown, set()
    if form == 'inplace':
        return 'verbatim', set(held), set(in_place), set()
    return start, set(), set(), set(missed)


def flipped_cause(miss):
    """The recorded cause of a row that is not exact in any form of the unit's text."""
    reason = ' '.join(str(miss.get('reason') or miss.get('status') or 'differs').split())
    return ('not exact in the unit\'s text in any form (one include block, blocks held, lines in '
            'place, canonical text): %s' % reason[:200])


def run(unit, form, joins, attempts, config_path, name=None, keep=None):
    started = time.monotonic()
    config = json.loads(Path(config_path).read_text())
    catalog = catalog_io.load(catalog_io.locate(REPOSITORY))
    units = load_unit_map()
    if unit not in units:
        raise SystemExit('%s: not in %s' % (unit, UNIT_MAP.relative_to(REPOSITORY)))
    modules = json.loads(MODULES.read_text()) if MODULES.exists() else {}
    if unit in modules:
        raise SystemExit('%s is already a module (manifests/original_target/modules.json)' % unit)
    read = lambda path: (REPOSITORY / path).read_text(errors='replace')
    plan = plan_unit(catalog, unit, units[unit], read, modules, joins, apply_rename.frozen_bounds(config))
    module = 'src/recovered/%s.cpp' % (name or unit)
    if (REPOSITORY / module).exists():
        raise SystemExit('%s exists' % module)
    members = plan['members']
    residuals = plan['residuals']
    for row in [r for r in members if r['entry'] in (keep or {})]:
        # Stated by the lane with its cause (e.g. a native check that reads per file).
        members.remove(row)
        residuals.append(residual_row(row, 'kept', keep[row['entry']]))
    if len(members) < 2:
        raise SystemExit('%s: %d row(s) in its canonical group; nothing to re-file' % (unit, len(members)))
    candidates = {r['entry']: r for r in plan['candidates'] if r['entry'] not in (keep or {})}
    by_entry = {r['entry']: r for r in members + plan['candidates']}
    held, in_place, log = set(), set(), []
    start = form
    fixups = native_fixups()
    number = 0
    accepted = None
    while number < attempts:
        number += 1
        rows = sorted(members + list(candidates.values()), key=lambda r: r['entry'])
        files = [(r['entry'], r['source'], read(r['source'])) for r in rows]
        text, info = pour(files, form, held, in_place, banner_text(unit, plan['population'], form))
        # Before anything is applied: a row whose native fixup cannot be re-keyed into this text
        # stays in its own file (kind "kept"), and the rest are poured again. The tree is never
        # converted with native data that no longer applies.
        stuck = unplaceable_fixups(fixups, {r['source']: module for r in rows}, text, info['chunks'])
        if stuck:
            number -= 1
            for row in [r for r in rows if r['source'] in stuck]:
                by_entry.pop(row['entry'], None)
                if candidates.pop(row['entry'], None) is None:
                    members.remove(row)
                held.discard(row['entry'])
                in_place.discard(row['entry'])
                residuals.append(residual_row(row, 'kept', stuck[row['source']]))
                print('  %08x kept in its own file: %s' % (row['entry'], stuck[row['source']]), flush=True)
            if len(members) < 2:
                raise SystemExit('%s: %d row(s) left after native fixups kept rows back; nothing to re-file' % (
                    unit, len(members)))
            continue
        ok, misses, tail, seconds = apply_attempt(unit, number, module, text, [r['entry'] for r in rows if
                                                                             r['entry'] not in candidates],
                                                  sorted(candidates), config_path)
        log.append(dict(attempt=number, form=form, held=['%08x' % e for e in sorted(held)],
                        in_place=len(in_place), joins=len(candidates), ok=ok, seconds=seconds,
                        misses=None if misses is None else [m['entry'] for m in misses]))
        print('attempt %d: form %s, %d held, %d in place, %d joins: %s (%.0fs)' % (
            number, form, len(held), len(in_place), len(candidates),
            'exact' if ok else ('%d rows differ' % len(misses) if misses is not None else 'refused'), seconds), flush=True)
        if ok:
            accepted = (rows, text, info)
            break
        missed = {int(m['entry'], 16) for m in misses or []}
        joined_missed = missed & set(candidates)
        outside = {e for e in missed if e not in by_entry}
        if misses is None:
            # A compile error or another refusal: a join is the first suspect, then the form.
            if candidates:
                for row in candidates.values():
                    residuals.append(residual_row(
                        row, 'flipped', REFUSED_JOIN + ' (attempt %d: %s)' % (number, refusal_reason(tail))))
                candidates = {}
            elif form == 'hoist':
                form = 'inplace'
            elif form == 'inplace':
                form = 'verbatim'
            else:
                raise SystemExit('%s: refused in verbatim form:\n%s' % (unit, tail))
            continue
        if joined_missed or (outside and candidates):
            # A joining row that differs under the unit's state, or whose leaving changes
            # its old population, stays where it is.
            drop = joined_missed or set(candidates)
            for entry in sorted(drop):
                row = candidates.pop(entry)
                cause = ('differs under the unit\'s state' if entry in joined_missed else
                         'its population changes when it leaves (%s)' % ', '.join('%08x' % e for e in sorted(outside)[:4]))
                residuals.append(residual_row(row, 'flipped', cause))
            continue
        if outside:
            # Rows of other units in the same compile group differ: try the next plainer form.
            if form == 'verbatim':
                raise SystemExit('%s: other rows of %s differ even in verbatim form: %s' % (
                    unit, plan['population'], sorted('%08x' % e for e in outside)))
            form = 'inplace' if form == 'hoist' else 'verbatim'
            continue
        form, held, in_place, flipped = after_miss(form, held, in_place, missed, [r['entry'] for r in rows], start)
        if flipped:
            # Not exact in any form: the rows stay in their own files (rule 3) and the rest start again.
            causes = {int(m['entry'], 16): flipped_cause(m) for m in misses}
            for entry in sorted(flipped):
                row = by_entry.pop(entry)
                members.remove(row)
                residuals.append(residual_row(row, 'flipped', causes[entry]))
            print('  %d rows flipped (not exact in any form); %d rows remain' % (len(flipped), len(members)), flush=True)
            if len(members) < 2:
                raise SystemExit('%s: %d row(s) left after %d flipped; nothing to re-file' % (
                    unit, len(members), len(flipped)))
    if accepted is None:
        raise SystemExit('%s: no exact form in %d attempts (see build/refile/%s)' % (unit, attempts, unit))
    rows, text, info = accepted
    joined = [r for r in rows if r['population'] != plan['population']]
    moved = {r['source']: module for r in rows}
    record = dict(unit=unit, module=module, moved=moved, chunks=info['chunks'], attempts=log)
    catalog_io.write_text_lf((REPOSITORY / PENDING / unit / 'moved.json'), json.dumps(record, indent=1) + '\n')
    entry = collections.OrderedDict(source=module, route='A', unit=unit + '.cpp', data=[])
    entry['population'] = plan['population']
    entry['form'] = dict(hoist='one include block', inplace='lines in place', verbatim='canonical text')[form] \
        if not (held or in_place) else 'include blocks held'
    entry['switches'] = info['switches']
    entry['include_blocks'] = [dict(before='%08x' % first, lines=len(lines)) for first, lines in info['blocks']]
    if held:
        entry['held_rows'] = [dict(entry='%08x' % e, cause='differs when the lines of later files are gathered ahead of it')
                              for e in sorted(held)]
    if in_place:
        entry['in_place_through'] = '%08x' % max(in_place)
    entry['joined'] = [dict(entry='%08x' % r['entry'], population=r['population']) for r in joined]
    entry['residuals'] = sorted(residuals, key=lambda r: r['entry'])
    entry['unit_functions'] = len(units[unit])
    modules[unit] = entry
    catalog_io.write_text_lf(MODULES, json.dumps(modules, indent=1) + '\n')
    summary, problems = rekey_native(moved)
    initterm = subprocess.run([sys.executable, '-B', str(REPOSITORY / 'native/tools/initterm_manifest.py')],
                              cwd=REPOSITORY, capture_output=True, text=True)
    summary['initterm'] = 'regenerated' if initterm.returncode == 0 else 'NOT regenerated: run refile_unit.py native'
    seconds = round(time.monotonic() - started, 1)
    report = dict(unit=unit, module=module, population=plan['population'], functions=len(rows),
                  bytes=sum(r['body_bytes'] for r in rows), joined=len(joined), residual_rows=len(residuals),
                  residual_bytes=sum(r['bytes'] for r in residuals),
                  **{kind + '_rows': sum(1 for r in residuals if r['kind'] == kind) for kind in ('split', 'flipped', 'kept')},
                  **{kind + '_bytes': sum(r['bytes'] for r in residuals if r['kind'] == kind) for kind in ('split', 'flipped', 'kept')},
                  switches=len(info['switches']),
                  include_blocks=len(info['blocks']), held=len(held), in_place=len(in_place), form=entry['form'],
                  attempts=number, seconds=seconds, native=summary, native_problems=problems)
    catalog_io.write_text_lf((REPOSITORY / PENDING / unit / 'report.json'), json.dumps(report, indent=1) + '\n')
    print(json.dumps(report, indent=1))
    if problems:
        raise SystemExit('native re-key problems (the exact tree is converted; fix native data by hand):\n  ' +
                         '\n  '.join(problems))


# ----------------------------------------------------------------------------- split rows by cause

# Read-only classification of "split" residuals (status.py's Route A line): what a row's own
# file shows, against the header state of the unit's module. No compile, nothing is written.
SPLIT_CLASSES = (
    ('template', 'template members'),
    ('header_state', 'needs another unit\'s header state'),
    ('stand_in', 'stand-ins'),
    ('emitted', 'emitted by another source'),
    ('separate_units', 'held by a population that compiles each source alone'),
    ('option_set', 'other option set'),
    ('own_population', 'the unit\'s own second population'),
    ('frozen_bound', 'no bound in the unit\'s frozen pass'),
    ('unclassified', 'not yet classified'),
)
FROZEN_BOUND = 'frozen in another population\'s inventory pass'
COMMENT = re.compile(r'//[^\n]*|/\*.*?\*/', re.S)
CONTAINER_SWITCH = re.compile(r'^\w+_IMPLEMENT(?:_\w+)?$')
EXPLICIT_INSTANTIATION = re.compile(r'^[ \t]*template[ \t]+(?:class|struct)[ \t]+\w+[ \t]*<', re.M)
LOCAL_TYPE = re.compile(r'^[ \t]*(?:class|struct)[ \t]+\w+[ \t]*(?::[^;{]*)?\{', re.M)
SEED_NAMESPACE = re.compile(r'^[ \t]*namespace[ \t]+seed_[0-9a-fA-F]{8}\b', re.M)


def place_joined(module_text, entries, chunks, entry, chunk):
    """The module text with `chunk` (the joining row's own text) placed in address order:
    before the part of the first row above `entry`, else at the end. `entries` are the
    module's rows in address order and `chunks` their parts in the same order (the pour's
    record); a part that is no longer found verbatim refuses (returns None)."""
    later = [c for e, c in zip(entries, chunks) if e > entry]
    if not later:
        return module_text.rstrip('\n') + '\n\n\n' + chunk.strip('\n') + '\n'
    at = module_text.find(later[0])
    if at < 0 or module_text.count(later[0]) != 1:
        return None
    return module_text[:at] + chunk.strip('\n') + '\n\n\n' + module_text[at:]


def join_one(unit, entry, config_path):
    """Retry one residual row of a re-filed unit alone (a row dropped together with other
    joins was never tested by itself): the row's text goes into the unit's file in address
    order and the batch runs as any join (apply_rename `joins`: same option set, equal
    frozen bound, the unit's table first, both populations re-verified). Exact: the row
    joins and leaves the residual list. Not exact: the tree is unchanged and the row's
    recorded cause becomes what this attempt showed."""
    modules = json.loads(MODULES.read_text())
    item = modules.get(unit)
    if not item or item.get('route') != 'A':
        raise SystemExit('%s is not a re-filed unit' % unit)
    residual = next((r for r in item.get('residuals', []) if int(r['entry'], 16) == entry), None)
    if residual is None:
        raise SystemExit('%08x is not a residual row of %s' % (entry, unit))
    catalog = catalog_io.load(catalog_io.locate(REPOSITORY))
    module = item['source']
    rows = sorted((r for r in catalog['regions'] if r['source'] == module), key=lambda r: r['entry'])
    row = next(r for r in catalog['regions'] if r['entry'] == entry)
    record_path = REPOSITORY / PENDING / unit / 'moved.json'
    record = json.loads(record_path.read_text()) if record_path.exists() else dict(unit=unit, module=module, moved={}, chunks={})
    chunks = list(record.get('chunks', {}).values())
    entries = [r['entry'] for r in rows]
    if len(chunks) != len(entries):
        if any(e > entry for e in entries):
            raise SystemExit('%s: no pour record to place %08x in address order (re-pour the unit instead)' % (unit, entry))
        chunks = [''] * len(entries)
    module_text = (REPOSITORY / module).read_text(errors='replace')
    text, info = pour([(entry, row['source'], (REPOSITORY / row['source']).read_text(errors='replace'))], 'inplace')
    # (directives the unit's file already holds are repeats there too)
    state = module_state(module_text)
    chunk = '\n'.join(line for kind, line in split_lines(text)
                      if not (kind in ('include', 'define') and directive_key(kind, line) in state)).strip('\n')
    placed = place_joined(module_text, entries, chunks, entry, chunk)
    if placed is None:
        raise SystemExit('%s: the unit\'s file changed since its pour; cannot place %08x' % (unit, entry))
    folder = REPOSITORY / PENDING / unit
    number = 1 + max([int(p.name.split('-')[1]) for p in folder.glob('attempt-*')] or [0])
    ok, misses, tail, seconds = apply_attempt(unit, number, module, placed, entries, [entry], config_path)
    if not ok:
        cause = flipped_cause(next((m for m in misses if int(m['entry'], 16) == entry), misses[0])) if misses else \
            'tried alone, the unit does not compile or verify with it: ' + refusal_reason(tail)
        if misses and all(int(m['entry'], 16) != entry for m in misses):
            cause = 'tried alone: other rows differ when it joins (%s)' % ', '.join(m['entry'] for m in misses[:4])
        residual.update(kind='flipped', cause=scrub_cause(cause))
        catalog_io.write_text_lf(MODULES, json.dumps(modules, indent=1) + '\n')
        print('%s %08x: not exact alone (%.0fs): %s' % (unit, entry, seconds, residual['cause']))
        return False
    item['residuals'] = [r for r in item['residuals'] if r is not residual]
    item.setdefault('joined', []).append(dict(entry='%08x' % entry, population=row['population']))
    catalog_io.write_text_lf(MODULES, json.dumps(modules, indent=1) + '\n')
    record.setdefault('moved', {})[row['source']] = module
    record.setdefault('chunks', {})[row['source']] = chunk
    catalog_io.write_text_lf(record_path, json.dumps(record, indent=1) + '\n')
    summary, problems = rekey_native({row['source']: module})
    subprocess.run([sys.executable, '-B', str(REPOSITORY / 'native/tools/initterm_manifest.py')], cwd=REPOSITORY,
                   capture_output=True, text=True)
    print('%s %08x: joined exact (%.0fs); native %s %s' % (unit, entry, seconds, summary, problems or ''))
    return True


def include_names(text, read, base='src'):
    """Quoted headers a text includes, transitively: names relative to src/ (shared/x.hpp).
    read(path) -> text or None. A name is looked up under src/, then beside the header that
    includes it; a header that is not there is skipped."""
    seen, pending = set(), [(text, '')]
    while pending:
        current, folder = pending.pop()
        for kind, line in split_lines(current):
            match = INCLUDE.match(line) if kind == 'include' else None
            if not match or not match.group(1).startswith('"'):
                continue
            name = match.group(1).strip('"')
            for candidate in (name, (folder + '/' + name) if folder else name):
                if candidate in seen:
                    break
                body = read('%s/%s' % (base, candidate))
                if body is not None:
                    seen.add(candidate)
                    pending.append((body, candidate.rpartition('/')[0]))
                    break
    return sorted(seen)


def module_state(text, closure=()):
    """Directive keys a module text already holds at top level (its header state): every
    include/define/undef line, plus the headers of its include closure (names relative to
    src/, e.g. shared/x.hpp)."""
    keys = {directive_key(kind, line) for kind, line in split_lines(text) if kind in ('include', 'define', 'undef')}
    keys |= {('include', '"%s"' % name) for name in closure}
    return keys


def own_lines(text, state):
    """Top-level include/define/undef lines of a row's file that the unit's state lacks."""
    return [line for kind, line in split_lines(text)
            if kind in ('include', 'define', 'undef') and directive_key(kind, line) not in state]


def split_class(text, state, holder=None):
    """The cause class of one split row, read from its own file (first that applies):
      template      the file sets a container implementation switch (VECTOR_IMPLEMENT_* and the
                    like) or explicitly instantiates a template: one member per file;
      header_state  the file has a top-level include/define/undef line the unit's module does not
                    hold (another switch, another header, shared/ghidra_types.hpp);
      stand_in      placeholder form: the function is written on a type the file defines for
                    itself, or inside a seed namespace;
      emitted       the file holds comments only: the row has no source of its own (an inline
                    constructor, a dynamic initialiser or a compiler-generated destructor that
                    another source of its population emits), so it moves only with its emitter;
      `holder`      else the class read from the holding population (holder_class), if any;
      unclassified  none of these shows."""
    for kind, line in split_lines(text):
        match = DEFINE.match(line) if kind == 'define' else None
        if match and CONTAINER_SWITCH.match(match.group(1)):
            return 'template'
    if EXPLICIT_INSTANTIATION.search(text):
        return 'template'
    if own_lines(text, state):
        return 'header_state'
    if LOCAL_TYPE.search(text) or SEED_NAMESPACE.search(text):
        return 'stand_in'
    if not COMMENT.sub('', text).strip():
        return 'emitted'
    return holder or 'unclassified'


def holder_facts(catalog):
    """What holder_class reads from the catalog: (separate-unit populations, rows per
    population, options per entry)."""
    return ({p['id'] for p in catalog.get('populations', []) if p.get('separate_units')},
            collections.Counter(r['population'] for r in catalog['regions']),
            {r['entry']: list(r['options']) for r in catalog['regions'] if r.get('options') is not None})


def holder_class(row, entry, facts, leaving):
    """The cause class of a split row read from its holding population (first that applies):
      separate_units  the population compiles each source alone (no compile group to join from);
      option_set      the row compiles under another option set than the unit's group;
      own_population  every row of the population is a split row of this unit: the unit's own
                      second compile group, which a join would empty;
      frozen_bound    the recorded cause: the unit's population's frozen inventory pass gives no
                      equal bound for the row (a mixed population of several units);
    else None. `leaving` counts the unit's split rows per population."""
    separate, sizes, options = facts
    population = row.get('population')
    if population in separate:
        return 'separate_units'
    mine = options.get(int(row['entry'], 16)) if isinstance(row.get('entry'), str) else None
    if mine is not None and entry.get('options') is not None and mine != list(entry['options']):
        return 'option_set'
    if population and leaving[population] == sizes[population]:
        return 'own_population'
    if row.get('cause', '').startswith(FROZEN_BOUND):
        return 'frozen_bound'
    return None


def classify_split_rows(entries, read, catalog=None):
    """[(entry, row, class)] for the split residuals of the Route A module entries (each with
    `source` and `residuals`). read(path) -> text or None (a missing file leaves the row
    unclassified). With `catalog` the rows the text does not decide are classed by their
    holding population (holder_class)."""
    facts = holder_facts(catalog) if catalog is not None else None
    out = []
    for entry in entries:
        split = [r for r in entry.get('residuals', []) if r.get('kind') == 'split']
        if not split:
            continue
        module = read(entry['source']) if entry.get('source') else None
        state = module_state(module, include_names(module, read)) if module is not None else None
        leaving = collections.Counter(r.get('population') for r in split)
        for row in split:
            text = read(row['source']) if row.get('source') and state is not None else None
            holder = holder_class(row, entry, facts, leaving) if facts is not None else None
            out.append((entry, row, split_class(text, state, holder) if text is not None else 'unclassified'))
    return out


def split_classes(entries, read, catalog=None):
    """{class: [rows, bytes]} over the split residuals of the Route A module entries
    (classify_split_rows)."""
    out = {key: [0, 0] for key, _ in SPLIT_CLASSES}
    for _, row, key in classify_split_rows(entries, read, catalog):
        out[key][0] += 1
        out[key][1] += row['bytes']
    return out


# ----------------------------------------------------------------------------- container template joins

# Container template members (docs/DECISIONS.md "Route A split rows", option (ii), limited to
# the one container header; lane C1 trial). With every VectorClass/DynamicVectorClass member
# defined in shared/vector_class.hpp, a unit that uses a list emits that list's members
# itself, as the original unit did. A member row frozen and accepted in another population
# joins its unit's translation unit when it reproduces its complete extent there; it keeps
# the population that froze it as `bound_population`, and the strict audit reads its bound,
# gap padding and provenance from that population's pass (canonical.bound_population).
CONTAINER_HEADER = '"shared/vector_class.hpp"'
CONTAINER_VARIANTS = ('VECTOR_IMPLEMENT_DYNAMIC_INSERT_AFTER_INDEX', 'VECTOR_IMPLEMENT_DYNAMIC_DELETE_ALL')
CONTAINER_SWITCH_LINE = re.compile(r'^[ \t]*#[ \t]*define[ \t]+(VECTOR_IMPLEMENT_\w+)[ \t]*$')
CONTAINER_ALL_BLOCK = re.compile(r'^[ \t]*#[ \t]*ifndef[ \t]+VECTOR_IMPLEMENT_ALL[ \t]*\n[ \t]*#[ \t]*define[ \t]+'
                                 r'VECTOR_IMPLEMENT_ALL[ \t]*\n[ \t]*#[ \t]*endif[^\n]*\n?', re.M)
CONTAINER_INSTANTIATION = re.compile(r'^[ \t]*template[ \t]+class[ \t]+((?:Dynamic)?VectorClass[ \t]*<.*>)[ \t]*;[ \t]*$')


CLASS_FACTORY_INCLUDE = re.compile(r'^[ \t]*#[ \t]*include[ \t]+"shared/class_factory(?:_\w+)?\.hpp"[ \t]*$')
TYPE_LIST_SWITCH_LINE = re.compile(r'^[ \t]*#[ \t]*define[ \t]+((?:VECTOR|TYPELIST)_IMPLEMENT_\w+)[ \t]*$')


def class_factory_member_file(text):
    """A TClassFactory<T> member of shared/class_factory.hpp (or one of its former
    one-member headers): an explicit instantiation or specialization of TClassFactory<T>."""
    if not any(CLASS_FACTORY_INCLUDE.match(l) for l in text.splitlines()):
        return False
    return bool(re.search(r'^[ \t]*template[ \t]+class[ \t]+TClassFactory[ \t]*<', text, re.M) or
                re.search(r'^[ \t]*template[ \t]*<[ \t]*>[^;{]*\bTClassFactory[ \t]*<', text, re.M))


def class_factory_own_lines(text, state):
    """Directive lines a class-factory member file brings that the unit lacks; its include of
    the family header (or a former one-member header) does not count once the unit holds
    shared/class_factory.hpp."""
    holds = ('include', '"shared/class_factory.hpp"') in state
    return [line for kind, line in split_lines(text)
            if kind in ('include', 'define', 'undef') and directive_key(kind, line) not in state
            and not (holds and CLASS_FACTORY_INCLUDE.match(line))]


def type_list_member_file(text):
    """A TypeList<T> member of shared/type_list.hpp: an explicit specialization of a
    TypeList<T> member."""
    if '#include "shared/type_list.hpp"' not in text:
        return False
    return bool(re.search(r'^[ \t]*template[ \t]*<[ \t]*>[^;{]*\bTypeList[ \t]*<', text, re.M) or
                re.search(r'^[ \t]*template[ \t]+class[ \t]+TypeList[ \t]*<', text, re.M))


def type_list_own_lines(text, state):
    """Directive lines a TypeList member file brings that the unit lacks; its member switches
    (`#define VECTOR_IMPLEMENT_*`, `TYPELIST_IMPLEMENT_*`) and the header's include once the
    unit's closure holds it do not count."""
    return [line for kind, line in split_lines(text)
            if kind in ('include', 'define', 'undef') and directive_key(kind, line) not in state
            and not TYPE_LIST_SWITCH_LINE.match(line)]


def container_member_file(text):
    """True when a row's file is a container template member of the one container header:
    it includes shared/vector_class.hpp and explicitly instantiates or specializes a
    VectorClass/DynamicVectorClass member, and nothing else of its own."""
    if '#include ' + CONTAINER_HEADER not in text:
        return False
    return bool(re.search(r'^[ \t]*template[ \t]*<[ \t]*>[^;{]*\b(?:Dynamic)?VectorClass[ \t]*<', text, re.M) or
                any(CONTAINER_INSTANTIATION.match(l) for l in text.splitlines()))


def container_own_lines(text, state):
    """Top-level directive lines a container member file brings that the joining unit's state
    lacks (the row then does not join). `// @ref` and seed markers, the container header's
    include once the unit holds it, and the former member switches (`#define VECTOR_IMPLEMENT_*`,
    `#ifndef VECTOR_IMPLEMENT_ALL` blocks) are not counted: a unit that selects no member gets
    every member. A declaration-variant switch is an own line."""
    text = CONTAINER_ALL_BLOCK.sub('', text)
    own = []
    for kind, line in split_lines(text):
        switch = CONTAINER_SWITCH_LINE.match(line)
        if switch:
            if switch.group(1) in CONTAINER_VARIANTS:
                own.append(line)
            continue
        if kind in ('include', 'define', 'undef') and directive_key(kind, line) not in state:
            own.append(line)
    return own


def anchor_lines(module_text, entries):
    """{entry: line index} of the comment block that opens each unit row whose address the
    module text names in a `//` line (the pour's per-row header comment)."""
    lines = module_text.split('\n')
    found = {}
    # Non-exact drafts (#ifdef RA2_NON_MATCHING blocks, non_exact.py) never anchor an accepted row.
    import non_exact
    guarded = {n for a, b in non_exact.guarded_blocks(module_text)[0] for n in range(a - 1, b + 1)}
    for entry in entries:
        pattern = re.compile(r'^\s*//.*\b0x0*%X\b' % entry, re.I)
        at = next((i for i, l in enumerate(lines) if i not in guarded and pattern.match(l)), None)
        if at is None:
            continue
        while at > 0 and lines[at - 1].lstrip().startswith('//'):
            at -= 1
        found[entry] = at
    return found


def place_in_address_order(module_text, unit_entries, chunks, run_header=None):
    """The module text with each joining chunk ([(entry, chunk)]) placed in address order:
    before the comment block of the first unit row above its entry whose address the text
    names, else behind the text. Rows of the unit without an address comment are skipped
    as anchors (the next named row above is used). With `run_header`, the chunks that land
    at one place form one compact run under that line (no blank lines between them).
    Returns (text, {entry: (first, last)}) with 1-based line numbers."""
    anchors = anchor_lines(module_text, sorted(unit_entries))
    ordered = sorted(anchors)
    lines = module_text.rstrip('\n').split('\n')
    inserts = collections.defaultdict(list)
    for entry, chunk in sorted(chunks):
        above = next((e for e in ordered if e > entry), None)
        inserts[anchors[above] if above is not None else len(lines)].append((entry, chunk))
    out, spans = [], {}
    for index in range(len(lines) + 1):
        run = inserts.get(index, [])
        if run and run_header is not None:
            if out and out[-1].strip():
                out += ['', '']
            out.append(run_header)
        for entry, chunk in run:
            if run_header is None and out and out[-1].strip():
                out += ['', '']
            first = len(out) + 1
            out += chunk.split('\n')
            spans[entry] = (first, len(out))
            if run_header is None:
                out += ['', '']
        if run and run_header is not None:
            out += ['', '']
        if index < len(lines):
            out.append(lines[index])
    return '\n'.join(out).rstrip('\n') + '\n', spans


def screen_errors(log, spans, offset=0):
    """{entry: first error line} for compiler errors inside a joining row's lines, and the
    errors outside every joining row. `offset`: lines ahead of the module text."""
    inside, outside = {}, []
    for line in log.splitlines():
        match = re.search(r'\((\d+)\)\s*:\s*(?:fatal )?error ', line)
        if not match:
            continue
        number = int(match.group(1)) - offset
        owner = next((e for e, (first, last) in spans.items() if first <= number <= last), None)
        if owner is None:
            outside.append(line.strip())
        else:
            inside.setdefault(owner, line.strip())
    return inside, outside


def screen_compile(config, catalog, pid, module, text, folder, options=None):
    """Compile the unit's compile group with `text` as the module source (as build.py joins
    it); (object path or None, compile log, lines ahead of the module text). `options`: compile
    under that option set instead of the group's (the option-evidence screen of join-own)."""
    from build import compile_units
    from canonical import compiler_arguments
    from canonical import separate_unit_populations
    rows = sorted((r for r in catalog['regions'] if r['population'] == pid), key=lambda r: r['source_order'])
    if pid in separate_unit_populations(catalog):     # each source is its own translation unit
        rows = [r for r in rows if r['source'] == module]
    sources = list(collections.OrderedDict.fromkeys(r['source'] for r in rows))
    parts, offset = [], 0
    for source in sources:
        if source == module:
            offset = sum(p.count(b'\n') + 1 for p in parts)
            parts.append(text.encode())
        else:
            parts.append((REPOSITORY / source).read_bytes())
    folder.mkdir(parents=True)
    source = folder / 'accepted.cpp'
    source.write_bytes(b'\n'.join(parts))
    unit = dict(cwd=folder, source=source, obj=folder / 'accepted.obj', log=folder / 'compile.log',
                returncode=folder / 'returncode.txt', args=compiler_arguments(options or rows[0]['options']))
    compile_units(config, folder, [unit], 'screen', parallel=False)
    log = (folder / 'compile.log').read_text(errors='replace') if (folder / 'compile.log').exists() else ''
    ok = (folder / 'returncode.txt').exists() and (folder / 'returncode.txt').read_text().strip() == '0'
    return (folder / 'accepted.obj' if ok and (folder / 'accepted.obj').exists() else None), log, offset


def screen_rows(audit, obj, rows, table, bounds):
    """{entry: reason} for the rows that do not reproduce their complete extent in `obj`;
    each row is compared against its own population's frozen bound (`bounds`: pid ->
    (inventory, gaps) as the audit loads them)."""
    from coff import Coff
    from compile_candidates import compare_candidate
    coff = Coff(obj.read_bytes())
    missed = {}
    for row in rows:
        candidate = dict(row, method='%08x' % row['entry'], region_bytes=row['full_extent_bytes'], references=table)
        try:
            result = compare_candidate(audit, coff, candidate, obj, row['options'], 0)
        except (ValueError, KeyError) as error:
            missed[row['entry']] = 'refused: %s' % error
            continue
        if result['status'] != 'extent-exact':
            missed[row['entry']] = result.get('reason') or '%s bytes differ' % result.get('differing_bytes')
    return missed


def own_frozen_bound(config, row, parsed={}):
    """Why the row's own frozen pass cannot bound it, or None: an inventory row at the entry
    and, when the row has padding, a gap row behind its body (a missing one is a listed
    refusal, never a KeyError)."""
    pid = row.get('bound_population', row['population'])
    inputs = config['populations'].get(pid)
    if inputs is None:
        return 'no frozen inputs for %s' % pid
    for key, field in (('inventory', 'entry'), ('gaps', 'start')):
        if (pid, key) not in parsed:
            parsed[pid, key] = {int(json.loads(l)[field], 16) for l in (REPOSITORY / inputs[key]).read_text().splitlines()
                                if l.strip()}
    if row['entry'] not in parsed[pid, 'inventory']:
        return 'no inventory row in its own frozen pass (%s)' % pid
    if row['padding_bytes'] and row['entry'] + row['body_bytes'] not in parsed[pid, 'gaps']:
        return 'no gap row behind its body in its own frozen pass (%s)' % pid
    return None


def join_template_plan(catalog, config, item, read, closure, module=None, family='vector'):
    """Container member rows of a re-filed unit that may try to join `module` (default the
    unit's file): (candidates {entry: row}, refused {entry: cause}, pid). Split and flipped
    residuals both count."""
    module = module or item['source']
    members = [r for r in catalog['regions'] if r['source'] == module]
    pid, options = members[0]['population'], members[0]['options']
    table = dict(members[0].get('references') or {})
    policies = {p['id'] for p in catalog.get('populations', []) if p.get('compiler_output_policy')}
    by_entry = {r['entry']: r for r in catalog['regions']}
    counts = collections.Counter(r['source'] for r in catalog['regions'])
    state = module_state(read(module), closure)
    candidates, refused = {}, {}
    for residual in item.get('residuals', []):
        if residual.get('kind') not in ('split', 'flipped'):
            continue
        row = by_entry.get(int(residual['entry'], 16))
        if row is None or row['source'] == module:
            continue
        text = read(row['source'])
        if not FAMILIES[family]['member'](text) or row['population'] == pid:
            continue
        own = FAMILIES[family]['own'](text, state)
        if row['options'] != options:
            refused[row['entry']] = 'another option set (%s)' % ' '.join(row['options'])
        elif counts[row['source']] > 1:
            refused[row['entry']] = 'its file holds several rows (%s)' % row['source']
        elif {pid, row['population']} & policies:
            refused[row['entry']] = 'a closed compiler-output population'
        elif own:
            refused[row['entry']] = 'needs header state the unit does not have: %s' % '; '.join(
                ' '.join(l.split()) for l in own)[:200]
        elif own_frozen_bound(config, row):
            refused[row['entry']] = own_frozen_bound(config, row)
        else:
            conflict = join_conflict(row, table, text)
            if conflict:
                refused[row['entry']] = 'binding: %s' % conflict
            else:
                candidates[row['entry']] = row
                table.update(join_references(dict(row, references={}), table))    # its relocations only
    return candidates, refused, table, pid


def unit_without_member_switches(module_text):
    """The unit text without its container member switches: with none defined, the
    container header defines every member and the unit instantiates what it uses
    (declaration variants stay)."""
    lines = []
    for line in module_text.split('\n'):
        switch = CONTAINER_SWITCH_LINE.match(line)
        if switch and switch.group(1) not in CONTAINER_VARIANTS and switch.group(1) != 'VECTOR_IMPLEMENT_ALL':
            continue
        lines.append(line)
    return '\n'.join(lines)


MEMBER_LAYOUT_LINE = re.compile(r'^//\s*Layout and behaviour: shared/(?:vector_class|type_list)\.hpp'
                                r'(?:, shared/vector_class\.hpp)?\.?\s*$')
MEMBER_RUN_HEADER = '// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):'
FAMILIES = {
    'vector': dict(member=container_member_file, own=container_own_lines, strip=True, use='its lists', one='its list',
                   header=MEMBER_RUN_HEADER),
    'class_factory': dict(member=class_factory_member_file, own=class_factory_own_lines, strip=False,
                          use='its class factories', one='TClassFactory<T> (the constructor)',
                          header='// Class factory members this unit emits by its own use of TClassFactory<T> '
                                 '(bodies: shared/class_factory.hpp):'),
    'type_list': dict(member=type_list_member_file, own=type_list_own_lines, strip=False, use='its type lists',
                      one='its type list',
                      header='// TypeList members this unit emits by its own use of its type lists '
                             '(bodies: shared/type_list.hpp):'),
}


def member_comment(text):
    """The leading description of a container member file (its comment lines before the
    first directive or declaration, `// @ref` and seed markers dropped): what a member the
    unit's own use emits keeps in the unit's text."""
    lines = []
    for line in text.split('\n'):
        if REF_LINE.match(line) or line in SEED_MARKER or MEMBER_LAYOUT_LINE.match(line):
            continue
        if not line.lstrip().startswith('//'):
            if line.strip():
                break
            continue
        lines.append(line)
    return '\n'.join(lines)


def emitted_join_text(module_text, unit_entries, candidates, read, strip_switches=True, family='vector'):
    """The unit text (member switches dropped when `strip_switches`) with each candidate's
    description in address order: the unit's own use of the list emits the member, so the
    row brings no code of its own (no explicit instantiation, whose extra members no row
    accounts for)."""
    if strip_switches:
        module_text = unit_without_member_switches(module_text)
    chunks = [(e, member_comment(read(r['source'])) or '// %08x: emitted by this unit\'s use of %s' % (
        e, FAMILIES[family]['one']))
              for e, r in sorted(candidates.items())]
    return place_in_address_order(module_text, unit_entries, chunks, FAMILIES[family]['header'])


def swap_headers(copies):
    """Write header copies ({tree path: copy path}) into the tree for the screen compiles;
    returns the tree's bytes to put back (restore_headers), also at exit."""
    import atexit
    saved = {path: (REPOSITORY / path).read_bytes() for path in copies}
    for path, copy in copies.items():
        (REPOSITORY / path).write_bytes((REPOSITORY / copy).read_bytes())
    atexit.register(restore_headers, saved)
    return saved


def restore_headers(saved):
    """Put the tree's headers back once (the exit hook then finds nothing to do, so a header
    the apply wrote stays)."""
    for path, data in saved.items():
        if (REPOSITORY / path).read_bytes() != data:
            (REPOSITORY / path).write_bytes(data)
    saved.clear()


def header_spec(copies):
    """The apply_rename `headers` / `header_base` fields for header copies made from HEAD."""
    if not copies:
        return {}
    base = subprocess.run(['git', 'rev-parse', 'HEAD'], cwd=REPOSITORY, capture_output=True, text=True,
                          check=True).stdout.strip()
    return dict(headers=dict(copies), header_base=base)


def join_template(unit, config_path, dry_run=False, attempts=8, into=None, family='vector', header_copies=None):
    """Join a re-filed unit's container template member rows into its translation unit
    (`into`: another source of the unit, e.g. the file that defines its lists): only rows the
    unit's own use emits join, each with its description only. `header_copies` ({tree path:
    copy}): the family header's new form, compiled in the screens and applied with the join
    in the same apply_rename batch (its header users are verified there)."""
    header_copies = header_copies or {}
    started = time.monotonic()
    config = json.loads(Path(config_path).read_text())
    catalog = catalog_io.load(catalog_io.locate(REPOSITORY))
    modules = json.loads(MODULES.read_text(), object_pairs_hook=collections.OrderedDict)
    item = modules.get(unit)
    if item is None or item.get('route') != 'A':
        raise SystemExit('%s is not a re-filed unit' % unit)
    module = into or item['source']
    strip = into is None and FAMILIES[family]['strip']
    read = lambda path: (REPOSITORY / path).read_text(errors='replace')
    from build import include_closure
    from canonical import validate_headers
    headers = validate_headers(REPOSITORY, catalog)
    closure = include_closure(read(module).encode(), headers, {})
    state = module_state(read(module), closure)
    candidates, refused, table, pid = join_template_plan(catalog, config, item, read, closure, module, family)
    members = sorted((r for r in catalog['regions'] if r['source'] == module), key=lambda r: r['entry'])
    unit_entries = [r['entry'] for r in members]
    print('%s: %d %s member rows: %d may try, %d refused before any compile' % (
        unit, len(candidates) + len(refused), family, len(candidates), len(refused)), flush=True)
    folder = REPOSITORY / PENDING / unit / ('join-template-%d' % time.time_ns())
    from verify_manifest import Audit
    audit = Audit(REPOSITORY, config['executable'])
    log, flipped, number, implicit = [], {}, 0, set()
    saved_headers = swap_headers(header_copies)
    # Screen 0: the unit alone; members its own use already emits need no explicit line.
    if candidates:
        base = unit_without_member_switches(read(module)) if strip else read(module)
        obj, compile_log, _ = screen_compile(config, catalog, pid, module, base, folder / 'screen-00')
        if obj is not None:
            from coff import Coff
            coff = Coff(obj.read_bytes())
            names = {s['name'] for s in coff.symbols.values()}
            for entry, row in candidates.items():
                if any(n.startswith(row['symbol_prefix']) for n in names) and row['entry'] not in screen_rows(
                        audit, obj, [row], table, None):
                    implicit.add(entry)
            missed0 = screen_rows(audit, obj, members, table, None)
            if missed0:
                for e in list(candidates):
                    flipped[e] = 'rows of the unit differ without its member switches (%s)' % ', '.join(
                        '%08x' % m for m in sorted(missed0)[:4])
                    candidates.pop(e)
            log.append(dict(screen=0, unit_rows_differ={'%08x' % e: r for e, r in sorted(missed0.items())}))
        else:
            log.append(dict(screen=0, compiled=False, errors=[l for l in compile_log.splitlines() if ' error ' in l][:5]))
        log.append(dict(screen=0, implicit=['%08x' % e for e in sorted(implicit)]))
        for e in [e for e in candidates if e not in implicit]:
            flipped[e] = 'not emitted by the unit\'s own use of %s' % FAMILIES[family]['use']
            candidates.pop(e)
        print('screen 0: %d rows emitted by the unit\'s own use' % len(implicit), flush=True)
    while candidates and number < attempts:
        number += 1
        text, spans = emitted_join_text(read(module), unit_entries, candidates, read, strip, family)
        obj, compile_log, offset = screen_compile(config, catalog, pid, module, text, folder / ('screen-%02d' % number))
        if obj is None:
            inside, outside = screen_errors(compile_log, spans, offset)
            drop = dict(inside) if inside else {e: 'the unit does not compile with the joining rows (%s)' % (
                outside[0][:160] if outside else 'no error line') for e in candidates}
            for e, cause in drop.items():
                flipped[e] = ('does not compile in the unit: %s' % cause[:200]) if inside else cause
                candidates.pop(e)
            log.append(dict(screen=number, compiled=False, dropped=['%08x' % e for e in sorted(drop)]))
            print('screen %d: compile errors, %d rows leave' % (number, len(drop)), flush=True)
            continue
        missed_members = screen_rows(audit, obj, members, table, None)
        missed = screen_rows(audit, obj, list(candidates.values()), table, None)
        log.append(dict(screen=number, compiled=True, differ={'%08x' % e: r for e, r in sorted(missed.items())},
                        unit_rows_differ=['%08x' % e for e in sorted(missed_members)]))
        print('screen %d: %d of %d joining rows exact, %d unit rows differ' % (
            number, len(candidates) - len(missed), len(candidates), len(missed_members)), flush=True)
        for e, reason in missed.items():
            flipped[e] = 'differs under the unit\'s state (%s)' % reason
            candidates.pop(e)
        if missed_members and not missed:
            for e in list(candidates):
                flipped[e] = 'rows of the unit differ when it joins (%s)' % ', '.join(
                    '%08x' % m for m in sorted(missed_members)[:4])
                candidates.pop(e)
            continue
        if not missed:
            break
    restore_headers(saved_headers)
    report = dict(unit=unit, module=module, population=pid,
                  screened_exact=sorted('%08x' % e for e in candidates),
                  screened_exact_bytes=sum(r['body_bytes'] for r in candidates.values()),
                  implicit=sorted('%08x' % e for e in implicit if e in candidates),
                  refused={'%08x' % e: c for e, c in sorted({**refused, **flipped}.items())},
                  refused_bytes=sum(r['body_bytes'] for r in catalog['regions'] if r['entry'] in {**refused, **flipped}),
                  screens=log)
    folder.mkdir(parents=True, exist_ok=True)
    if dry_run or not candidates:
        report['seconds'] = round(time.monotonic() - started, 1)
        catalog_io.write_text_lf(folder / 'report.json', json.dumps(report, indent=1) + '\n')
        print(json.dumps({k: v for k, v in report.items() if k != 'screens'}, indent=1))
        return report
    # apply_rename recompiles the unit and every population a row leaves and runs the strict
    # audit on all of them; a population that changes when rows leave keeps them.
    by_entry = {r['entry']: r for r in catalog['regions']}
    joined, applied = None, 0
    while candidates and applied < attempts:
        applied += 1
        text, spans = emitted_join_text(read(module), unit_entries, candidates, read, strip, family)
        attempt = folder / ('apply-%02d' % applied)
        attempt.mkdir(parents=True)
        draft = attempt / Path(module).name
        catalog_io.write_text_lf(draft, text)
        spec_item = dict(draft=draft.relative_to(REPOSITORY).as_posix(), regions=['%08x' % e for e in unit_entries],
                         joins=['%08x' % e for e in sorted(candidates)], joins_bound='own')
        catalog_io.write_text_lf(attempt / 'spec.json', json.dumps(dict(
            name='join-template-%s-%02d' % (unit, applied), renames={}, modules={module: spec_item},
            **header_spec(header_copies)), indent=1) + '\n')
        result = subprocess.run([sys.executable, '-B', str(Path(__file__).with_name('apply_rename.py')),
                                 str(attempt / 'spec.json'), '--config', str(config_path), '--output', str(attempt / 'out')],
                                cwd=REPOSITORY, capture_output=True, text=True)
        catalog_io.write_text_lf(attempt / 'apply.log', result.stdout + result.stderr)
        if result.returncode == 0:
            joined = dict(candidates)
            break
        misses = json.loads((attempt / 'out/misses.json').read_text()) if (attempt / 'out/misses.json').exists() else None
        if misses is None:
            raise SystemExit('%s: apply_rename refused the join:\n%s' % (unit, (result.stdout + result.stderr)[-2000:]))
        missed = {int(m['entry'], 16) for m in misses}
        drop = missed & set(candidates)
        cause = 'differs under the unit\'s state'
        if not drop:
            outside = {by_entry[e]['population'] for e in missed if e in by_entry}
            drop = {e for e, r in candidates.items() if r['population'] in outside} or set(candidates)
            cause = 'its population changes when it leaves (%s)' % ', '.join('%08x' % e for e in sorted(missed)[:4])
        for e in drop:
            flipped[e] = cause
            candidates.pop(e)
        print('apply %d: %d rows differ, %d joining rows leave' % (applied, len(missed), len(drop)), flush=True)
    if joined is None:
        raise SystemExit('%s: no row joined (see %s)' % (unit, folder))
    causes = {**refused, **flipped}
    item.setdefault('joined', [])
    for e, row in sorted(joined.items()):
        item['joined'].append(collections.OrderedDict(entry='%08x' % e, bytes=row['body_bytes'],
                                                      population=row['population'],
                                                      bound='own frozen bound (%s)' % row['population']))
    residuals = []
    for residual in item.get('residuals', []):
        e = int(residual['entry'], 16)
        if e in joined:
            continue
        if e in causes:
            residual = dict(residual, cause='held by %s; not joined: %s' % (residual['population'], scrub_cause(causes[e])))
        residuals.append(residual)
    item['residuals'] = residuals
    catalog_io.write_text_lf(MODULES, json.dumps(modules, indent=1) + '\n')
    moved = {row['source']: module for row in joined.values()}
    record = dict(unit=unit, module=module, moved=moved)
    catalog_io.write_text_lf(folder / 'moved.json', json.dumps(record, indent=1) + '\n')
    summary, problems = rekey_native(moved)
    initterm = subprocess.run([sys.executable, '-B', str(REPOSITORY / 'native/tools/initterm_manifest.py')],
                              cwd=REPOSITORY, capture_output=True, text=True)
    summary['initterm'] = 'regenerated' if initterm.returncode == 0 else 'NOT regenerated: run refile_unit.py native'
    report.update(joined=sorted('%08x' % e for e in joined), joined_bytes=sum(r['body_bytes'] for r in joined.values()),
                  left_populations=sorted({r['population'] for r in joined.values()}),
                  refused={'%08x' % e: c for e, c in sorted(causes.items())}, apply_attempts=applied,
                  seconds=round(time.monotonic() - started, 1), native=summary, native_problems=problems)
    catalog_io.write_text_lf(folder / 'report.json', json.dumps(report, indent=1) + '\n')
    print(json.dumps({k: v for k, v in report.items() if k != 'screens'}, indent=1))
    if problems:
        raise SystemExit('native re-key problems:\n  ' + '\n  '.join(problems))
    return report


def join_template_batch(units, config_path, attempts=8, family='vector'):
    """join_template for several units in one apply_rename batch (one audit setup, one
    catalog write): each unit is screened alone first (join_template --dry-run), then every
    unit's draft and its screened rows go into one spec. A row that differs in the batch
    leaves and the batch runs again."""
    started = time.monotonic()
    screened = {}
    for unit in units:
        report = join_template(unit, config_path, dry_run=True, family=family)
        if report['screened_exact']:
            screened[unit] = report
    catalog = catalog_io.load(catalog_io.locate(REPOSITORY))
    modules = json.loads(MODULES.read_text(), object_pairs_hook=collections.OrderedDict)
    by_entry = {r['entry']: r for r in catalog['regions']}
    read = lambda path: (REPOSITORY / path).read_text(errors='replace')
    plan = {}
    for unit, report in screened.items():
        module = report['module']
        members = sorted((r for r in catalog['regions'] if r['source'] == module), key=lambda r: r['entry'])
        plan[unit] = dict(module=module, entries=[r['entry'] for r in members],
                          candidates={int(e, 16): by_entry[int(e, 16)] for e in report['screened_exact']},
                          causes={int(e, 16): c for e, c in report['refused'].items()})
    folder = REPOSITORY / PENDING / ('batch-%d' % time.time_ns())
    joined, applied = None, 0
    while any(p['candidates'] for p in plan.values()) and applied < attempts:
        applied += 1
        attempt = folder / ('apply-%02d' % applied)
        attempt.mkdir(parents=True)
        spec_modules = {}
        for unit, p in plan.items():
            if not p['candidates']:
                continue
            text, _ = emitted_join_text(read(p['module']), p['entries'], p['candidates'], read,
                                        FAMILIES[family]['strip'], family)
            draft = attempt / (unit + '.cpp')
            catalog_io.write_text_lf(draft, text)
            spec_modules[p['module']] = dict(draft=draft.relative_to(REPOSITORY).as_posix(),
                                             regions=['%08x' % e for e in p['entries']],
                                             joins=['%08x' % e for e in sorted(p['candidates'])], joins_bound='own')
        catalog_io.write_text_lf(attempt / 'spec.json', json.dumps(dict(
            name='join-template-batch-%02d' % applied, renames={}, modules=spec_modules), indent=1) + '\n')
        result = subprocess.run([sys.executable, '-B', str(Path(__file__).with_name('apply_rename.py')),
                                 str(attempt / 'spec.json'), '--config', str(config_path), '--output', str(attempt / 'out')],
                                cwd=REPOSITORY, capture_output=True, text=True)
        catalog_io.write_text_lf(attempt / 'apply.log', result.stdout + result.stderr)
        if result.returncode == 0:
            joined = True
            break
        misses = json.loads((attempt / 'out/misses.json').read_text()) if (attempt / 'out/misses.json').exists() else None
        if misses is None:
            # The strict audit refused whole objects (a vtable or code section no row accounts
            # for): the units whose module text the refusal names leave the batch.
            log = result.stdout + result.stderr
            named = set(re.findall(r'source (src/recovered/[^,\]]+)', log))
            out = [u for u, p in plan.items() if p['module'] in named and p['candidates']]
            if not out:
                raise SystemExit('batch: apply_rename refused:\n%s' % log[-2000:])
            reason = re.search(r'(Unsupported allocatable data section|Unaccounted emitted code section bytes)[^\[]*', log)
            for unit in out:
                for e in list(plan[unit]['candidates']):
                    plan[unit]['causes'][e] = 'the unit without member switches fails the strict audit (%s)' % (
                        reason.group(0).strip()[:120] if reason else 'refused')
                    plan[unit]['candidates'].pop(e)
            print('batch apply %d: strict audit refused %s; they leave' % (applied, ', '.join(out)), flush=True)
            continue
        missed = {int(m['entry'], 16) for m in misses}
        dropped = 0
        for unit, p in plan.items():
            drop = missed & set(p['candidates'])
            if not drop and missed & set(p['entries']):
                drop = set(p['candidates'])         # the unit's own rows differ: none of its joins
            for e in drop:
                p['causes'][e] = 'differs in the unit (batch apply %d)' % applied
                p['candidates'].pop(e)
                dropped += 1
        if not dropped:
            outside = {by_entry[e]['population'] for e in missed if e in by_entry}
            for unit, p in plan.items():
                for e in [e for e, r in p['candidates'].items() if r['population'] in outside]:
                    p['causes'][e] = 'its population changes when it leaves'
                    p['candidates'].pop(e)
                    dropped += 1
        print('batch apply %d: %d rows differ, %d joining rows leave' % (applied, len(missed), dropped), flush=True)
        if not dropped:
            raise SystemExit('batch: misses outside every joining row: %s' % sorted('%08x' % e for e in missed)[:8])
    if not joined:
        raise SystemExit('batch: nothing joined (see %s)' % folder)
    moved, summary = {}, {}
    for unit, p in plan.items():
        item = modules[unit]
        item.setdefault('joined', [])
        for e, row in sorted(p['candidates'].items()):
            item['joined'].append(collections.OrderedDict(entry='%08x' % e, bytes=row['body_bytes'], population=row['population'],
                                                          bound='own frozen bound (%s)' % row['population']))
            moved[row['source']] = p['module']
        residuals = []
        for residual in item.get('residuals', []):
            e = int(residual['entry'], 16)
            if e in p['candidates']:
                continue
            if e in p['causes']:
                residual = dict(residual, cause='held by %s; not joined: %s' % (residual['population'], scrub_cause(p['causes'][e])))
            residuals.append(residual)
        item['residuals'] = residuals
        summary[unit] = dict(rows=len(p['candidates']), bytes=sum(r['body_bytes'] for r in p['candidates'].values()))
    catalog_io.write_text_lf(MODULES, json.dumps(modules, indent=1) + '\n')
    catalog_io.write_text_lf(folder / 'moved.json', json.dumps(dict(moved=moved), indent=1) + '\n')
    native, problems = rekey_native(moved)
    subprocess.run([sys.executable, '-B', str(REPOSITORY / 'native/tools/initterm_manifest.py')], cwd=REPOSITORY,
                   capture_output=True, text=True)
    report = dict(units=summary, rows=sum(s['rows'] for s in summary.values()),
                  bytes=sum(s['bytes'] for s in summary.values()), apply_attempts=applied,
                  seconds=round(time.monotonic() - started, 1), native=native, native_problems=problems)
    catalog_io.write_text_lf(folder / 'report.json', json.dumps(report, indent=1) + '\n')
    print(json.dumps(report, indent=1))
    return report


# ----------------------------------------------------------------------------- own-bound joins of code-bearing rows

# Lane F measurement (review session's call 2026-11-13): the container-join machinery
# (own frozen bound, bound_population, both populations re-verified by apply_rename) applied
# to split rows that bring their own code: stand-ins (placeholder form) and the rows of a
# unit's own second population. The row's text goes into the unit's file in address order,
# as it is, with only the directives the unit already holds dropped; a row whose file needs
# header state the unit lacks never tries. Nothing is re-written to make a row match.
OWN_JOIN_CLASSES = ('stand_in', 'own_population')


def dead_view_files(cache={}):
    """Sources native/data/vtable_views.tsv classes `dead` (no other object references the
    functions the file defines): the check reads the file's object, so inside a unit's object
    it no longer holds."""
    if 'files' not in cache:
        path = REPOSITORY / 'native/data/vtable_views.tsv'
        rows = [l.split('\t') for l in path.read_text().splitlines() if l and not l.startswith('#')] if path.exists() else []
        cache['files'] = {f[0] for f in rows if len(f) > 2 and f[2] == 'dead'}
    return cache['files']


# A unit-counter name (`_$E<n>` initializer or atexit destructor, `_$S<n>` guard flag): its
# number is never an identity (initializer_rebind.py), so a joining row's names, numbered by its
# own population's compile, take numbers no name of the unit uses before the compile; the
# re-binding by content then gives them the unit's fresh names (apply_rename, own_join_screen).
UNIT_COUNTED = re.compile(r'^_\$([ES])(\d+)$')
UNIT_COUNTED_TOKEN = re.compile(r'_\$[ES](\d+)')
PLACEHOLDER_OFFSET = 100000
ADOPT_OPTIONS_CLASSES = ('stand_in', 'own_population')
REF_BINDING = re.compile(r'@ref\s+([^\s=]+)=(?:0x)?([0-9a-fA-F]+)')


def joins_under_options(row_options, unit_options, key, adopt_gr=False, adopt_options=False):
    """True when a split row of class `key` may join a unit compiled under `unit_options`: the
    same set; the unit's /GR only (--adopt-gr); or any other set for a stand-in or a row of the
    unit's own second population (--adopt-options; the evidence check runs before screening)."""
    if list(row_options) == list(unit_options):
        return True
    if adopt_options and key in ADOPT_OPTIONS_CLASSES:
        return True
    return bool(adopt_gr and apply_rename.gr_only_difference(row_options, unit_options))


def counted_names(row, text=''):
    """The `_$E<n>`/`_$S<n>` names a row uses (symbol, prefix, relocations, references, @ref lines)."""
    names = {row.get('symbol'), row.get('symbol_prefix')}
    names |= {r.get('symbol') for r in row.get('relocations') or []}
    names |= set(row.get('references') or {})
    names |= {n for n, _ in REF_BINDING.findall(text)}
    return {n for n in names if n and UNIT_COUNTED.match(n)}


def renumber_counted(candidates, chunks, members, table, module_text):
    """{entry: {old: new}}: the unit-counter names of the joining rows, renumbered past every
    number the unit's rows, table and text use (one block per donor population, so two donors'
    `_$E4` stay apart; an initializer and the destructor it registers keep their pairing)."""
    used = [int(m) for m in UNIT_COUNTED_TOKEN.findall(module_text)]
    for row in members:
        used += [int(UNIT_COUNTED.match(n).group(2)) for n in counted_names(row)]
    used += [int(UNIT_COUNTED.match(n).group(2)) for n in table if UNIT_COUNTED.match(n)]
    # Far past the unit's own numbers: a placeholder must never equal a name the fresh compile
    # gives (initializer_rebind keeps an unchanged name when content binds several candidates).
    following = max(used + [0]) + 1 + PLACEHOLDER_OFFSET
    by_population = collections.defaultdict(set)
    for e, row in candidates.items():
        by_population[row['population']] |= counted_names(row, chunks.get(e, ''))
    mappings = {}
    for population in sorted(by_population):
        mapping = {}
        for name in sorted(by_population[population], key=lambda n: (int(UNIT_COUNTED.match(n).group(2)), n)):
            mapping[name] = '_$%s%d' % (UNIT_COUNTED.match(name).group(1), following)
            following += 1
        mappings[population] = mapping
    return {e: mappings[row['population']] for e, row in candidates.items() if mappings[row['population']]}


def renumbered_row(row, mapping):
    """A copy of `row` with its unit-counter names renamed by `mapping`."""
    import copy
    out = copy.deepcopy(row)
    for key in ('symbol', 'symbol_prefix'):
        if out.get(key) in mapping:
            out[key] = mapping[out[key]]
    for r in out.get('relocations') or []:
        r['symbol'] = mapping.get(r['symbol'], r['symbol'])
    if out.get('references'):
        out['references'] = {mapping.get(n, n): v for n, v in out['references'].items()}
    return out


def renumbered_text(text, mapping):
    """`// @ref` lines of a joining row's text with its unit-counter names renamed."""
    import initializer_rebind
    return initializer_rebind.rebind_source_refs(text, mapping)


def counted_references(row, text):
    """{name: address} of the unit-counter names a (renumbered) joining row binds: its @ref
    lines and relocation receipts (join_references leaves these out)."""
    out = {}
    for relocation in row.get('relocations') or []:
        if UNIT_COUNTED.match(relocation['symbol']):
            out.setdefault(relocation['symbol'], relocation['target'])
    for name, value in REF_BINDING.findall(text):
        if UNIT_COUNTED.match(name):
            out.setdefault(name, int(value, 16))
    return out


def rebound_for_screen(coff, rows, table, original):
    """(rows, table) copies with the unit-counter names re-bound to the fresh object by content
    and position (initializer_rebind.group_mapping), as apply_rename re-binds them after its
    compile; the rows unchanged when nothing moves."""
    import copy
    import initializer_rebind
    if not initializer_rebind.uses_initializers(rows, table):
        return rows, table
    shared = dict(table)
    copies = []
    for row in rows:
        clone = copy.deepcopy(dict(row, references=None))
        clone['references'] = shared
        copies.append(clone)
    mapping = initializer_rebind.group_mapping(coff, copies, shared, original)
    if mapping:
        initializer_rebind.rebind_rows(copies, mapping)
    return copies, (copies[0]['references'] if copies else shared)


def record_option_causes(item, causes):
    """Residuals of a module item recorded as "another option set" take the cause measured by a
    join-own --adopt-options run ({entry: cause}); other residuals keep theirs."""
    out = []
    for r in item.get('residuals', []):
        e = int(r['entry'], 16)
        if e in causes and 'another option set' in r.get('cause', ''):
            r = dict(r, cause='held by %s; not joined: %s' % (r['population'], scrub_cause(causes[e])))
        out.append(r)
    item['residuals'] = out


def options_note(row_options, unit_options):
    """The manifest note of a row joined under its unit's option set, or None (same set)."""
    if list(row_options) == list(unit_options):
        return None
    if apply_rename.gr_only_difference(row_options, unit_options):
        return 'joined under the unit\'s /GR (frozen without it)'
    return 'joined under the unit\'s options (frozen under %s)' % ' '.join(row_options)


def other_options(row_options, unit_options):
    """True when a row's frozen option set differs from its unit's by more than the unit's /GR
    (the `--adopt-options` case)."""
    return list(row_options) != list(unit_options) and not apply_rename.gr_only_difference(row_options, unit_options)


def option_evidence(config, catalog, audit, unit, module, pid, members, read, table, option_sets, folder):
    """{option set (tuple): (unit rows that differ, unit rows)} : the unit compiled unchanged
    under each other option set. The unit's own set is evidenced against X only when some of
    its rows reproduce under its own set and not under X (docs/DECISIONS.md "Stand-ins frozen
    under another optimisation option than their unit")."""
    out = {}
    for n, options in enumerate(sorted(option_sets)):
        obj, log, _ = screen_compile(config, catalog, pid, module, read(module), folder / ('options-%02d' % n),
                                     options=list(options))
        if obj is None:
            out[options] = (len(members), len(members))      # the unit does not even compile under X
            continue
        out[options] = (len(screen_rows(audit, obj, members, table, None)), len(members))
    return out


def refuse_unevidenced(candidates, chunks, refused, evidence):
    """Move the candidates whose frozen option set the unit's set is not evidenced against
    (option_evidence: no unit row differs under it) to `refused`. Rows whose set has no
    evidence entry (the unit's own set, or the unit's /GR only) stay."""
    for e, row in list(candidates.items()):
        differ, total = evidence.get(tuple(row['options']), (1, 1))
        if not differ:
            refused[e] = ('the unit\'s option set is not evidenced against %s (all %d unit rows reproduce '
                          'under it too)' % (' '.join(row['options']), total))
            candidates.pop(e)
            chunks.pop(e)


def own_join_plan(catalog, config, unit, item, read, closure, classes, fixups, adopt_gr=False, refuse=None,
                  adopt_options=False):
    """(candidates {entry: row}, chunks {entry: text}, refused {entry: cause}, table, pid) for the
    split rows of `unit` of the given split classes."""
    module = item['source']
    members = [r for r in catalog['regions'] if r['source'] == module]
    pid, options = members[0]['population'], members[0]['options']
    table = dict(members[0].get('references') or {})
    policies = {p['id'] for p in catalog.get('populations', []) if p.get('compiler_output_policy')}
    by_entry = {r['entry']: r for r in catalog['regions']}
    counts = collections.Counter(r['source'] for r in catalog['regions'])
    module_text = read(module)
    state = module_state(module_text, closure)
    candidates, chunks, refused = {}, {}, {}
    # A unit whose rows carry a data_bound_by declaration (canonical.validate_data_bound_by: on
    # every row of the source) is not joined into: the joining row would need that flag too.
    bound_by = any(r.get('data_bound_by') for r in members)
    facts = holder_facts(catalog)
    leaving = collections.Counter(r.get('population') for r in item.get('residuals', []) if r.get('kind') == 'split')
    for _, residual, key in classify_split_rows([item], read, catalog):
        # A comment-only row (an atexit destructor, an initializer another source of its
        # population emits) of the unit's own second population moves with its emitter.
        if key == 'emitted' and 'own_population' in classes and \
                holder_class(residual, item, facts, leaving) == 'own_population':
            key = 'own_population'
        if key not in classes:
            continue
        if bound_by:
            refused[int(residual['entry'], 16)] = 'the unit\'s rows carry a data_bound_by declaration (it would have to cover the row)'
            continue
        if refuse and int(residual['entry'], 16) in refuse:
            refused[int(residual['entry'], 16)] = refuse[int(residual['entry'], 16)]
            continue
        row = by_entry.get(int(residual['entry'], 16))
        if row is None or row['source'] == module:
            continue
        text = read(row['source'])
        own = own_lines(text, state)
        # --adopt-options reaches stand-ins and the unit's own second population (DECISIONS rows
        # "Stand-ins frozen under another optimisation option" and "Own-population rows frozen under
        # another option set"); other classes keep the /GR rule
        if not joins_under_options(row['options'], options, key, adopt_gr, adopt_options):
            refused[row['entry']] = 'another option set (%s)' % ' '.join(row['options'])
        elif counts[row['source']] > 1:
            refused[row['entry']] = 'its file holds several rows (%s)' % row['source']
        elif row['population'] == pid:
            refused[row['entry']] = 'already in the unit population'
        elif {pid, row['population']} & policies:
            refused[row['entry']] = 'a closed compiler-output population'
        elif re.search(r'\b_?_asm\b', text):
            refused[row['entry']] = 'evidenced inline assembly keeps its per-file attribution'
        elif own:
            refused[row['entry']] = 'needs header state the unit does not have: %s' % '; '.join(
                ' '.join(l.split()) for l in own)[:200]
        elif own_frozen_bound(config, row):
            refused[row['entry']] = own_frozen_bound(config, row)
        elif row['source'] in dead_view_files():
            refused[row['entry']] = ('its file holds a vtable view classed dead, which the native check '
                                     '(vtable_views.py) reads per object')
        else:
            conflict = join_conflict(row, table, text)
            if conflict:
                refused[row['entry']] = 'binding: %s' % conflict
                continue
            chunk = own_join_chunk(text, state)
            candidates[row['entry']] = row
            chunks[row['entry']] = chunk
    # native fixups of a joining file must re-key into the unit's text
    if candidates:
        text, _ = place_in_address_order(module_text, [r['entry'] for r in members], sorted(chunks.items()))
        moved = {candidates[e]['source']: module for e in candidates}
        bad = unplaceable_fixups(fixups, moved, text, {candidates[e]['source']: c for e, c in chunks.items()})
        for e in [e for e in candidates if candidates[e]['source'] in bad]:
            refused[e] = bad[candidates[e]['source']]
            candidates.pop(e)
            chunks.pop(e)
    # unit-counter names (_$E<n>, _$S<n>) take numbers the unit does not use (renumber_counted)
    renumber = renumber_counted(candidates, chunks, members, table, module_text)
    for e, mapping in renumber.items():
        candidates[e] = dict(renumbered_row(candidates[e], mapping), renumber=mapping)
        chunks[e] = renumbered_text(chunks[e], mapping)
    for e, row in candidates.items():
        table.update(join_references(dict(row, references={}), table))
        for name, target in counted_references(row, chunks[e]).items():
            table.setdefault(name, target)
    return candidates, chunks, refused, table, pid


def own_join_screen(config, catalog, audit, unit, module, pid, members, read, candidates, chunks, table, folder,
                    attempts, log):
    """Screen compiles of the unit with the joining rows' text placed in address order:
    rows that do not compile or reproduce their complete extent leave; when the unit's own
    rows differ, the joining rows are halved until the culprits are found. Returns
    (exact {entry: row}, flipped {entry: cause})."""
    unit_entries = [r['entry'] for r in members]
    flipped = {}
    counter = [0]

    def screen(subset):
        counter[0] += 1
        text, spans = place_in_address_order(read(module), unit_entries, sorted((e, chunks[e]) for e in subset))
        obj, compile_log, offset = screen_compile(config, catalog, pid, module, text, folder / ('screen-%03d' % counter[0]))
        if obj is None:
            inside, outside = screen_errors(compile_log, spans, offset)
            log.append(dict(screen=counter[0], rows=len(subset), compiled=False, inside=len(inside),
                            outside=outside[:3]))
            return None, inside, outside
        from coff import Coff
        rows, bound = rebound_for_screen(Coff(obj.read_bytes()), members + [candidates[e] for e in sorted(subset)],
                                         table, audit.original)
        missed = screen_rows(audit, obj, rows[len(members):], bound, None)
        missed_members = screen_rows(audit, obj, rows[:len(members)], bound, None)
        log.append(dict(screen=counter[0], rows=len(subset), compiled=True, differ=len(missed),
                        unit_rows_differ=['%08x' % e for e in sorted(missed_members)][:8]))
        return (missed, missed_members), None, None

    def solve(subset, depth=0):
        """The exact rows of `subset`."""
        subset = set(subset)
        while subset and counter[0] < attempts:
            result, inside, outside = screen(subset)
            if result is None:
                if inside:
                    for e, cause in inside.items():
                        flipped[e] = 'does not compile in the unit: %s' % cause[:200]
                    subset -= set(inside)
                    continue
                if len(subset) == 1:
                    e = next(iter(subset))
                    flipped[e] = 'the unit does not compile with it (%s)' % ((outside or ['no error line'])[0][:160])
                    return set()
                half = sorted(subset)
                return solve(half[:len(half) // 2], depth + 1) | solve(half[len(half) // 2:], depth + 1)
            missed, missed_members = result
            for e, reason in missed.items():
                flipped[e] = 'differs under the unit\'s state (%s)' % reason
            subset -= set(missed)
            if missed:
                continue
            if not missed_members:
                return subset
            if len(subset) == 1:
                e = next(iter(subset))
                flipped[e] = 'rows of the unit differ when it joins (%s)' % ', '.join(
                    '%08x' % m for m in sorted(missed_members)[:4])
                return set()
            half = sorted(subset)
            first = solve(half[:len(half) // 2], depth + 1)
            second = solve(half[len(half) // 2:], depth + 1)
            union = first | second
            if union == set(half):
                # each half is exact alone, not together: the second half leaves
                for e in second:
                    flipped[e] = 'rows of the unit differ when it joins together with %08x..' % min(first)
                union = first
            subset = union
            continue        # the union is screened again
        for e in subset:
            flipped.setdefault(e, 'screen budget spent')
        return set()

    exact = solve(set(candidates))
    return {e: candidates[e] for e in exact}, flipped


def join_own(units, config_path, classes=OWN_JOIN_CLASSES, dry_run=False, attempts=80, apply_attempts=8, adopt_gr=False,
             refuse=None, adopt_options=False):
    """Own-bound joins of code-bearing split rows (stand-ins, the unit's own second population)
    for several units: each unit is screened alone, then every unit's draft goes into one
    apply_rename batch (`joins_bound: own`); a row that differs in the batch leaves and the batch
    runs again. Writes a report with rows, bytes and causes per class.
    `adopt_options` (implies `adopt_gr`): rows frozen under another option set join under the
    unit's, only when the unit compiled unchanged under the row's set loses some of its own
    rows (the unit's set is evidenced, not chosen for the joining rows); the report lists that
    evidence per unit."""
    adopt_gr = adopt_gr or adopt_options
    started = time.monotonic()
    config = json.loads(Path(config_path).read_text())
    catalog = catalog_io.load(catalog_io.locate(REPOSITORY))
    modules = json.loads(MODULES.read_text(), object_pairs_hook=collections.OrderedDict)
    read = lambda path: (REPOSITORY / path).read_text(errors='replace')
    from build import include_closure
    from canonical import validate_headers
    from verify_manifest import Audit
    headers = validate_headers(REPOSITORY, catalog)
    audit = Audit(REPOSITORY, config['executable'])
    fixups = native_fixups()
    folder = REPOSITORY / PENDING / ('join-own-%d' % time.time_ns())
    plan, report_units = {}, {}
    for unit in units:
        item = modules.get(unit)
        if item is None or item.get('route') != 'A':
            print('%s: not a re-filed unit' % unit, flush=True)
            continue
        module = item['source']
        closure = include_closure(read(module).encode(), headers, {})
        classes_of = {int(r['entry'], 16): k for _, r, k in classify_split_rows([item], read, catalog)}
        candidates, chunks, refused, table, pid = own_join_plan(catalog, config, unit, item, read, closure, classes, fixups,
                                                                adopt_gr, refuse, adopt_options)
        members = sorted((r for r in catalog['regions'] if r['source'] == module), key=lambda r: r['entry'])
        log = []
        exact, flipped = {}, {}
        evidence = {}
        if adopt_options and candidates:
            unit_options = members[0]['options']
            sets = {tuple(r['options']) for r in candidates.values() if other_options(r['options'], unit_options)}
            evidence = option_evidence(config, catalog, audit, unit, module, pid, members, read, table, sets,
                                       folder / unit / 'options')
            refuse_unevidenced(candidates, chunks, refused, evidence)
        if candidates:
            exact, flipped = own_join_screen(config, catalog, audit, unit, module, pid, members, read, candidates,
                                             chunks, table, folder / unit, attempts, log)
        print('%s: %d tried, %d screened exact, %d refused before compile, %d not exact' % (
            unit, len(candidates), len(exact), len(refused), len(flipped)), flush=True)
        plan[unit] = dict(module=module, entries=[r['entry'] for r in members], candidates=exact,
                          chunks={e: chunks[e] for e in exact}, causes={**refused, **flipped}, classes=classes_of)
        report_units[unit] = dict(screens=log, option_evidence={
            ' '.join(k): dict(unit_rows_differ=v[0], unit_rows=v[1]) for k, v in sorted(evidence.items())})
    by_entry = {r['entry']: r for r in catalog['regions']}
    applied, joined = 0, None
    while not dry_run and any(p['candidates'] for p in plan.values()) and applied < apply_attempts:
        applied += 1
        attempt = folder / ('apply-%02d' % applied)
        attempt.mkdir(parents=True)
        spec_modules = {}
        for unit, p in plan.items():
            if not p['candidates']:
                continue
            text, _ = place_in_address_order(read(p['module']), p['entries'], sorted((e, p['chunks'][e]) for e in p['candidates']))
            draft = attempt / (unit + '.cpp')
            catalog_io.write_text_lf(draft, text)
            spec_modules[p['module']] = dict(draft=draft.relative_to(REPOSITORY).as_posix(),
                                             regions=['%08x' % e for e in p['entries']],
                                             joins=['%08x' % e for e in sorted(p['candidates'])], joins_bound='own',
                                             **({'joins_renumber': {'%08x' % e: r['renumber'] for e, r in
                                                                    sorted(p['candidates'].items()) if r.get('renumber')}}
                                                if any(r.get('renumber') for r in p['candidates'].values()) else {}),
                                             **(dict(joins_options='unit') if adopt_options else
                                                dict(joins_options='unit_gr') if adopt_gr else {}))
        catalog_io.write_text_lf(attempt / 'spec.json', json.dumps(dict(
            name='join-own-batch-%02d' % applied, renames={}, modules=spec_modules), indent=1) + '\n')
        result = subprocess.run([sys.executable, '-B', str(Path(__file__).with_name('apply_rename.py')),
                                 str(attempt / 'spec.json'), '--config', str(config_path), '--output', str(attempt / 'out')],
                                cwd=REPOSITORY, capture_output=True, text=True)
        catalog_io.write_text_lf(attempt / 'apply.log', result.stdout + result.stderr)
        if result.returncode == 0:
            joined = True
            break
        misses = json.loads((attempt / 'out/misses.json').read_text()) if (attempt / 'out/misses.json').exists() else None
        if misses is None:
            out_log = result.stdout + result.stderr
            named = set(re.findall(r'source (src/recovered/[^,\]]+)', out_log))
            out = [u for u, p in plan.items() if p['module'] in named and p['candidates']]
            if not out:
                raise SystemExit('join-own: apply_rename refused:\n%s' % out_log[-2000:])
            for unit in out:
                for e in list(plan[unit]['candidates']):
                    reason = re.search(r'(Unsupported allocatable data section \S+|Unaccounted emitted code section bytes)', out_log)
                    plan[unit]['causes'][e] = 'the joined unit fails the strict audit (%s)' % (
                        reason.group(1) if reason else 'refused')
                    plan[unit]['candidates'].pop(e)
            continue
        missed = {int(m['entry'], 16) for m in misses}
        dropped = 0
        for unit, p in plan.items():
            drop = missed & set(p['candidates'])
            if not drop and missed & set(p['entries']):
                drop = set(p['candidates'])
            for e in drop:
                p['causes'][e] = 'differs in the batch apply (%d)' % applied
                p['candidates'].pop(e)
                dropped += 1
        if not dropped:
            outside = {by_entry[e]['population'] for e in missed if e in by_entry}
            for unit, p in plan.items():
                for e in [e for e, r in p['candidates'].items() if r['population'] in outside]:
                    p['causes'][e] = 'its population changes when it leaves (%s)' % ', '.join(
                        '%08x' % m for m in sorted(missed)[:4])
                    p['candidates'].pop(e)
                    dropped += 1
        print('batch apply %d: %d rows differ, %d joining rows leave' % (applied, len(missed), dropped), flush=True)
        if not dropped:
            raise SystemExit('join-own: misses outside every joining row: %s' % sorted('%08x' % e for e in missed)[:8])
    summary = collections.OrderedDict()
    for unit, p in plan.items():
        summary[unit] = dict(joined=sorted('%08x' % e for e in p['candidates']) if (joined or dry_run) else [],
                             joined_bytes=sum(r['body_bytes'] for r in p['candidates'].values()) if (joined or dry_run) else 0,
                             classes={'%08x' % e: k for e, k in p['classes'].items() if k in classes},
                             causes={'%08x' % e: c for e, c in sorted(p['causes'].items())},
                             screens=report_units[unit]['screens'],
                             option_evidence=report_units[unit]['option_evidence'])
    report = dict(dry_run=dry_run, adopt_gr=adopt_gr, adopt_options=adopt_options, applied=bool(joined), apply_attempts=applied, units=summary,
                  rows=sum(len(s['joined']) for s in summary.values()),
                  bytes=sum(s['joined_bytes'] for s in summary.values()), seconds=round(time.monotonic() - started, 1))
    folder.mkdir(parents=True, exist_ok=True)
    if joined:
        moved, chunks = {}, {}
        catalog_options = {p['module']: next(r['options'] for r in catalog['regions'] if r['source'] == p['module'])
                           for p in plan.values()}
        for unit, p in plan.items():
            item = modules[unit]
            item.setdefault('joined', [])
            for e, row in sorted(p['candidates'].items()):
                item['joined'].append(collections.OrderedDict(entry='%08x' % e, bytes=row['body_bytes'],
                                                              population=row['population'],
                                                              bound='own frozen bound (%s)' % row['population']))
                note = options_note(row['options'], catalog_options[p['module']]) if adopt_gr else None
                if note:
                    item['joined'][-1]['options'] = note
                moved[row['source']] = p['module']
                chunks[row['source']] = p['chunks'][e]
            residuals = []
            for residual in item.get('residuals', []):
                e = int(residual['entry'], 16)
                if e in p['candidates']:
                    continue
                if e in p['causes']:
                    residual = dict(residual, cause='held by %s; not joined: %s' % (
                        residual['population'], scrub_cause(p['causes'][e])))
                residuals.append(residual)
            item['residuals'] = residuals
        catalog_io.write_text_lf(MODULES, json.dumps(modules, indent=1) + '\n')
        catalog_io.write_text_lf(folder / 'moved.json', json.dumps(dict(moved=moved, chunks=chunks), indent=1) + '\n')
        native, problems = rekey_native(moved)
        subprocess.run([sys.executable, '-B', str(REPOSITORY / 'native/tools/initterm_manifest.py')], cwd=REPOSITORY,
                       capture_output=True, text=True)
        report.update(native=native, native_problems=problems)
    elif not dry_run and adopt_options:
        # nothing joined: the rows recorded as "another option set" get their measured causes
        for unit, p in plan.items():
            record_option_causes(modules[unit], p['causes'])
        catalog_io.write_text_lf(MODULES, json.dumps(modules, indent=1) + '\n')
    catalog_io.write_text_lf(folder / 'report.json', json.dumps(report, indent=1) + '\n')
    print('join-own: %d rows, %d bytes (%s); report %s' % (report['rows'], report['bytes'],
                                                         'applied' if joined else 'not applied', folder / 'report.json'))
    return report


def own_join_chunk(text, state):
    """A joining row's part of the unit text, as join-own places it: the row's file poured
    in place, the directives the unit already holds dropped."""
    body, _ = pour([(0, '', text)], 'inplace')
    return '\n'.join(line for kind, line in split_lines(body)
                     if not (kind in ('include', 'define') and directive_key(kind, line) in state)).strip('\n')


def git_show(revision, path):
    result = subprocess.run(['git', 'show', '%s:%s' % (revision, path)], cwd=REPOSITORY, capture_output=True)
    return result.stdout if result.returncode == 0 else None


def unjoin(unit, entries, base):
    """Undo own-bound joins of `unit` (all its joined rows, or `entries`): each row's file,
    catalog row and residual come back as committed at `base` (the revision before the join)
    and its part leaves the unit's text. Unjoining every joined row of a unit also restores the
    unit's module item and its population's reference table from `base`, so a join followed
    by an unjoin gives back the pre-join tree. Native path-keyed rows (fixups, vtable views)
    re-keyed to the unit go back to the row's file; drafts, view layouts and behaviour
    rewrites that name a moved file refuse (restore by hand)."""
    modules = json.loads(MODULES.read_text(), object_pairs_hook=collections.OrderedDict)
    base_modules = json.loads(git_show(base, MODULES.relative_to(REPOSITORY).as_posix()),
                              object_pairs_hook=collections.OrderedDict)
    item = modules.get(unit)
    if item is None or not item.get('joined'):
        raise SystemExit('%s has no joined rows' % unit)
    module = item['source']
    earlier = {int(j['entry'], 16) for j in base_modules.get(unit, {}).get('joined', [])}
    joined = {int(j['entry'], 16): j for j in item['joined'] if int(j['entry'], 16) not in earlier}   # since `base`
    wanted = set(entries) if entries else set(joined)
    if wanted - set(joined):
        raise SystemExit('not joined rows of %s: %s' % (unit, ' '.join('%08x' % e for e in sorted(wanted - set(joined)))))
    whole = wanted == set(joined)
    cpath = catalog_io.locate(REPOSITORY)
    catalog = catalog_io.load(cpath)
    old = {r['entry']: r for r in catalog_io.load_git(REPOSITORY, base)['regions']}
    # The unit text is rebuilt from `base`: its text then, with the parts of the rows joined
    # since that stay joined, placed as join-own places them. The current text must be exactly
    # that placement with every joined-since row (no other edit since `base`).
    base_text = git_show(base, module)
    if base_text is None:
        raise SystemExit('%s is not in %s' % (module, base))
    base_text = base_text.decode(errors='replace')
    from build import include_closure
    from canonical import validate_headers
    state = module_state(base_text, include_closure(base_text.encode(), validate_headers(REPOSITORY, catalog), {}))
    base_entries = [r['entry'] for r in old.values() if r['source'] == module]
    since, sources = {}, {}
    for entry in joined:
        was = old.get(entry)
        if was is None or was['source'] == module:
            if entry in wanted:
                raise SystemExit('%08x: not joined since %s' % (entry, base))
            continue
        source = git_show(base, was['source'])
        if source is None:
            raise SystemExit('%08x: %s is not in %s' % (entry, was['source'], base))
        sources[entry] = (was['source'], source)
        since[entry] = own_join_chunk(source.decode(errors='replace'), state)
    placed, _ = place_in_address_order(base_text, base_entries, sorted(since.items()))
    if placed != (REPOSITORY / module).read_text():
        raise SystemExit('%s changed since %s other than by the joins; unjoin by hand' % (module, base))
    module_text, _ = place_in_address_order(base_text, base_entries,
                                            sorted((e, c) for e, c in since.items() if e not in wanted))
    if not any(e not in wanted for e in since):
        module_text = base_text
    moved = {}
    for entry in sorted(wanted):
        path, data = sources[entry]
        (REPOSITORY / path).parent.mkdir(parents=True, exist_ok=True)
        (REPOSITORY / path).write_bytes(data)
        moved[path] = module
    catalog_io.write_text_lf(REPOSITORY / module, module_text)
    digest = catalog_io.sha((REPOSITORY / module).read_bytes())
    pid = next(r['population'] for r in catalog['regions'] if r['source'] == module)
    unit_table = next((r.get('references') for r in old.values() if r['source'] == module), None) if whole else None
    for index, row in enumerate(catalog['regions']):
        if row['entry'] in wanted:
            was = old[row['entry']]
            donor = next((r for r in catalog['regions'] if r['population'] == was['population']
                          and r['entry'] not in wanted), None)
            catalog['regions'][index] = dict(was, references=donor['references'] if donor else was.get('references'))
        else:
            if row['source'] == module:
                row['source_sha256'] = digest
            if unit_table is not None and row['population'] == pid:
                row['references'] = unit_table
    # the population records (verification receipts apply_rename wrote) of the unit and of the
    # rows' donors as they were at `base`
    touched = {pid} | {old[e]['population'] for e in wanted}
    base_records = {p['id']: p for p in catalog_io.load_git(REPOSITORY, base, references=False).get('populations', [])}
    catalog['populations'] = [base_records.get(p['id'], p) if p['id'] in touched else p
                              for p in catalog.get('populations', [])]
    catalog_io.dump(catalog, cpath)
    # module item
    if whole:
        modules[unit] = base_modules[unit]
    else:
        base_residuals = {int(r['entry'], 16): r for r in base_modules[unit].get('residuals', [])}
        item['joined'] = [j for j in item['joined'] if int(j['entry'], 16) not in wanted]
        item['residuals'] = sorted(item.get('residuals', []) + [base_residuals[e] for e in sorted(wanted)],
                                   key=lambda r: int(r['entry'], 16))
    catalog_io.write_text_lf(MODULES, json.dumps(modules, indent=1) + '\n')
    problems = restore_native(moved, base)
    subprocess.run([sys.executable, '-B', str(REPOSITORY / 'native/tools/initterm_manifest.py')], cwd=REPOSITORY,
                   capture_output=True, text=True)
    print('%s: %d rows unjoined%s' % (unit, len(wanted), ' (the whole unit: module item restored)' if whole else ''))
    if problems:
        raise SystemExit('native rows to restore by hand:\n  ' + '\n  '.join(problems))


def restore_native(moved, base):
    """Native path-keyed rows of the files in `moved` ({file: unit module}) back to the file,
    from `base`: fixups (the re-keyed entries of a file sit where the file's entries were) and
    vtable views. Returns the rows it cannot restore."""
    problems = []
    path = REPOSITORY / 'native/port/fixups.json'
    raw = git_show(base, 'native/port/fixups.json')
    if path.exists() and raw is not None:
        before = json.loads(raw)['fixups']
        current_raw = path.read_text()
        document = json.loads(current_raw)
        current = document['fixups']
        def widened(fixup, base_fixup, module):
            """`fixup` is `base_fixup` re-keyed into `module` (rekey_fixups: the same text
            widened backwards by whole lines)."""
            if fixup['source'] != module or fixup['why'] != base_fixup['why'] or \
                    fixup.get('verified') != base_fixup.get('verified'):
                return False
            if not (fixup['old'].endswith(base_fixup['old']) and fixup['new'].endswith(base_fixup['new'])):
                return False
            return fixup['old'][:len(fixup['old']) - len(base_fixup['old'])] == \
                fixup['new'][:len(fixup['new']) - len(base_fixup['new'])]
        # rekey_native replaces each fixup of a moved file, in place, by one or more widened copies
        queues = {p: [f for f in before if f['source'] == p] for p in moved}
        out, changed, last = [], False, None
        for fixup in current:
            if last is not None and widened(fixup, last[1], moved[last[0]]):
                continue                                  # another occurrence of the same fixup
            source = next((p for p in moved if queues[p] and widened(fixup, queues[p][0], moved[p])), None)
            if source is None:
                out.append(fixup)
                last = None
                continue
            last = (source, queues[source].pop(0))
            out.append(last[1])
            changed = True
        problems += ['fixups.json: %d fixups of %s not found re-keyed' % (len(q), p) for p, q in queues.items() if q]
        if changed:
            for style in (dict(indent=1, ensure_ascii=False), dict(indent=1), dict(indent=2, ensure_ascii=False)):
                if json.dumps(document, **style) + '\n' == current_raw:
                    break
            catalog_io.write_text_lf(path, json.dumps(dict(document, fixups=out), **style) + '\n')
    path = REPOSITORY / 'native/data/vtable_views.tsv'
    raw = git_show(base, 'native/data/vtable_views.tsv')
    if path.exists() and raw is not None:
        keep = {}
        for line in raw.decode().split('\n'):
            first, tab, rest = line.partition('\t')
            if tab and first in moved:
                keep[(moved[first], rest)] = first
        lines = path.read_text().split('\n')
        for index, line in enumerate(lines):
            first, tab, rest = line.partition('\t')
            if tab and (first, rest) in keep:
                lines[index] = keep[(first, rest)] + tab + rest
        catalog_io.write_text_lf(path, '\n'.join(lines))
    for name, key in (('native/drafts.json', 'host_source'), ('native/port/view_layouts.json', 'source'),
                      ('native/behaviour_rewrites.json', None)):
        raw = git_show(base, name)
        if raw is None:
            continue
        text = raw.decode()
        for source in moved:
            if json.dumps(source) in text:
                problems.append('%s names %s' % (name, source))
    return problems


def units_with_own_join_rows(classes):
    """Route A units with split rows of the given classes, largest bytes first."""
    catalog = catalog_io.load(catalog_io.locate(REPOSITORY))
    modules = json.loads(MODULES.read_text())
    read = lambda path: (REPOSITORY / path).read_text(errors='replace')
    items = [dict(e, unit=u) for u, e in modules.items() if e.get('route') == 'A']
    sizes = collections.Counter()
    for entry, row, key in classify_split_rows(items, read, catalog):
        if key in classes:
            sizes[entry['unit']] += row['bytes']
    return [u for u, _ in sizes.most_common()]


def list_units(top):
    catalog = catalog_io.load(catalog_io.locate(REPOSITORY), references=False)
    units = load_unit_map()
    modules = json.loads(MODULES.read_text()) if MODULES.exists() else {}
    counts = collections.Counter(r['source'] for r in catalog['regions'])
    lines = []
    for unit, entries in units.items():
        rows = unit_rows(catalog, entries)
        if not rows:
            continue
        free = [r for r in rows if counts[r['source']] == 1]
        pid = main_population(free)
        inside = [r for r in free if r['population'] == pid]
        lines.append((sum(r['body_bytes'] for r in rows), unit, len(rows), pid or '-', len(inside),
                      sum(r['body_bytes'] for r in inside), len({r['population'] for r in rows}),
                      'module' if unit in modules else ''))
    lines.sort(reverse=True)
    print('%-14s %9s %5s %-16s %5s %9s %5s' % ('unit', 'bytes', 'rows', 'group', 'rows', 'bytes', 'pops'))
    for total, unit, count, pid, inside, inside_bytes, pops, flag in lines[:top]:
        print('%-14s %9d %5d %-16s %5d %9d %5d %s' % (unit, total, count, pid, inside, inside_bytes, pops, flag))
    done = [l for l in lines if l[7]]
    print('%d units with accepted rows; %d in module form' % (len(lines), len(done)))


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = parser.add_subparsers(dest='command', required=True)
    p = sub.add_parser('unit-map'); p.add_argument('--from', dest='functions', type=Path, required=True)
    p = sub.add_parser('list'); p.add_argument('--top', type=int, default=40)
    p = sub.add_parser('plan'); p.add_argument('unit'); p.add_argument('--no-joins', action='store_true')
    p = sub.add_parser('run'); p.add_argument('unit')
    p.add_argument('--form', choices=('hoist', 'inplace', 'verbatim'), default='hoist')
    p.add_argument('--no-joins', action='store_true')
    p.add_argument('--attempts', type=int, default=12)
    p.add_argument('--name', help='file stem when it differs from the unit name')
    p.add_argument('--keep', action='append', default=[], metavar='ENTRY=CAUSE',
                   help='leave this row in its own file, listed as a residual of kind "kept" with the cause')
    p = sub.add_parser('native'); p.add_argument('--check', action='store_true')
    sub.add_parser('causes')
    p = sub.add_parser('join'); p.add_argument('unit'); p.add_argument('entry')
    p = sub.add_parser('join-template'); p.add_argument('unit')
    p.add_argument('--into', help='another source of the unit to join into (e.g. the file that defines its lists)')
    p.add_argument('--dry-run', action='store_true', help='plan and screen compiles only; nothing is written')
    p.add_argument('--header', action='append', default=[], metavar='TREE=COPY',
                   help='the family header\'s new form (a copy made from HEAD): screened and applied with the join')
    p = sub.add_parser('join-template-batch'); p.add_argument('units', nargs='+')
    for name in ('join-template', 'join-template-batch'):
        sub.choices[name].add_argument('--family', choices=sorted(FAMILIES), default='vector',
                                       help='the one template header whose members join (default vector)')
    p = sub.add_parser('join-own'); p.add_argument('units', nargs='+')
    p.add_argument('--classes', default=','.join(OWN_JOIN_CLASSES), help='split classes that try (default: %(default)s)')
    p.add_argument('--dry-run', action='store_true', help='plan and screen compiles only; nothing is written')
    p.add_argument('--attempts', type=int, default=80, help='screen compiles per unit (24 was too few for wonline, mapgen)')
    p.add_argument('--refuse', action='append', default=[], metavar='ENTRY=CAUSE',
                   help='a row that does not try, with the cause recorded (e.g. one the native build refuses)')
    p.add_argument('--adopt-gr', action='store_true',
                   help='a row whose option set is the unit\'s without /GR joins under the unit\'s options')
    p.add_argument('--adopt-options', action='store_true',
                   help='(implies --adopt-gr) a row frozen under another option set joins under the unit\'s, when the '
                        'unit compiled under the row\'s set loses some of its own rows (docs/DECISIONS.md)')
    p = sub.add_parser('unjoin'); p.add_argument('unit'); p.add_argument('entries', nargs='*')
    p.add_argument('--base', required=True, help='the revision before the join (rows, files and residuals come back from it)')
    p = sub.add_parser('maps'); p.add_argument('build_dir', type=Path)
    for name in ('plan', 'run', 'native', 'maps', 'join', 'join-template', 'join-template-batch', 'join-own'):
        sub.choices[name].add_argument('--config', type=Path, default=catalog_io.config_path(REPOSITORY),
                                       help='compiler config (default: RA2_CONFIG, else private/original-target.local.json)')
    args = parser.parse_args()
    if args.command == 'unit-map':
        units = write_unit_map(json.loads(args.functions.read_text()))
        print('%d units, %d entries' % (len(units), sum(len(e) for e in units.values())))
    elif args.command == 'list':
        list_units(args.top)
    elif args.command == 'plan':
        config = json.loads(args.config.read_text())
        catalog = catalog_io.load(catalog_io.locate(REPOSITORY))
        modules = json.loads(MODULES.read_text()) if MODULES.exists() else {}
        plan = plan_unit(catalog, args.unit, load_unit_map()[args.unit],
                         lambda path: (REPOSITORY / path).read_text(errors='replace'), modules,
                         not args.no_joins, apply_rename.frozen_bounds(config))
        print(json.dumps(dict(unit=args.unit, population=plan['population'], options=plan['options'],
                              members=len(plan['members']), member_bytes=sum(r['body_bytes'] for r in plan['members']),
                              join_candidates=len(plan['candidates']),
                              join_candidate_bytes=sum(r['body_bytes'] for r in plan['candidates']),
                              other_rows_in_group=plan['foreign'], residuals=plan['residuals']), indent=1))
    elif args.command == 'run':
        keep = {}
        for item in args.keep:
            entry, _, cause = item.partition('=')
            if not cause.strip():
                raise SystemExit('--keep needs ENTRY=CAUSE')
            keep[int(entry, 16)] = cause.strip()
        run(args.unit, args.form, not args.no_joins, args.attempts, args.config, args.name, keep)
    elif args.command == 'native':
        moved = {}
        for record in sorted((REPOSITORY / PENDING).glob('*/moved.json')):
            moved.update(json.loads(record.read_text())['moved'])
        summary, problems = rekey_native(moved, write=not args.check)
        if not args.check:
            subprocess.run([sys.executable, '-B', str(REPOSITORY / 'native/tools/initterm_manifest.py')],
                           cwd=REPOSITORY, check=True)
        print(json.dumps(dict(summary, problems=problems), indent=1))
        raise SystemExit(1 if problems else 0)
    elif args.command == 'causes':
        modules = json.loads(MODULES.read_text(), object_pairs_hook=collections.OrderedDict)
        changed = restate_causes(modules)
        screened = reclassify_refused_joins(modules, lambda path: (REPOSITORY / path).read_text(errors='replace'))
        for unit, entry, cause in screened:
            print('%s %s: flipped -> split: %s' % (unit, entry, cause))
        if screened:
            print('%d refused joins re-stated as split (run `maps` on a kept-intermediates build to refresh the '
                  'module maps)' % len(screened))
        if changed or screened:
            catalog_io.write_text_lf(MODULES, json.dumps(modules, indent=1) + '\n')
        for unit, entry, old, new in changed:
            print('%s %s: %s' % (unit, entry, new))
        # The module maps repeat each unit's residuals: the same re-statement, file by file.
        maps = 0
        for path in sorted((MODULES.parent / 'modules').glob('*.json')):
            entry = json.loads(path.read_text(), object_pairs_hook=collections.OrderedDict)
            if restate_causes({path.stem: entry}):
                catalog_io.write_text_lf(path, json.dumps(entry, indent=1) + '\n')
                maps += 1
        print('%d causes re-stated in modules.json, %d module maps re-stated' % (len(changed), maps))
    elif args.command == 'join':
        raise SystemExit(0 if join_one(args.unit, int(args.entry, 16), args.config) else 1)
    elif args.command == 'unjoin':
        unjoin(args.unit, [int(e, 16) for e in args.entries], args.base)
    elif args.command == 'join-own':
        units = list(units_with_own_join_rows(args.classes.split(','))) if args.units == ['all'] else args.units
        refuse = {int(e, 16): c.strip() for e, _, c in (item.partition('=') for item in args.refuse)}
        join_own(units, args.config, tuple(args.classes.split(',')), args.dry_run, args.attempts, adopt_gr=args.adopt_gr,
                 refuse=refuse, adopt_options=args.adopt_options)
    elif args.command == 'join-template-batch':
        join_template_batch(args.units, args.config, family=args.family)
    elif args.command == 'join-template':
        copies = dict(item.split('=', 1) for item in args.header)
        join_template(args.unit, args.config, args.dry_run, into=args.into, family=args.family, header_copies=copies)
    elif args.command == 'maps':
        raise SystemExit(subprocess.call([sys.executable, '-B', str(Path(__file__).with_name('module_ledger.py')),
                                          str(args.build_dir), '--write', '--config', str(args.config)], cwd=REPOSITORY))


if __name__ == '__main__':
    main()
