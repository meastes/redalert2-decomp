#!/usr/bin/env python3
"""Clean-up end point census (lane K1, 2026-11-14): the five-line block the user agreed on.

    cleanup_census.py --block            the five lines
    cleanup_census.py --units [--all]    the per-unit work list (decompiler-style tokens descending)
    cleanup_census.py --json             everything as JSON
    cleanup_census.py --structure [--json]  the structure block (lane S1, 2026-11-17; see STRUCTURE below)
    [--references DIR]                   private/reference by default; absent -> counterpart "unknown"

Read from the tracked tree only: the unit map (manifests/original_target/unit_map.json), the module
maps (manifests/original_target/modules/*.json), the catalog (manifests/original_target/), the
non-exact manifest (non_exact.json), the generated-data manifest and the text of the sources the
catalog names. Nothing is compiled. The reference clones are read only for the counterpart test and
only when present; without them every counterpart reads "unknown" and the tool still runs.

Units. A unit is an original translation unit of the unit map, excluding the library units
(libcmt/*, comsupp/*). A unit is counted when it has at least one file. Its files are: the module
source of its module map (manifests/original_target/modules/<unit>.json "source"), every source
the catalog names for one of the unit's entries, every source non_exact.json names for one of them,
and the unit's header (manifests/original_target/modules.json "header"). A file holding rows of two
units counts for both.

Counterpart. A unit has a counterpart when TibSun has code/<unit>.cpp (or .c) or mischa85/RedAlert2
has src/<unit>.cpp (or .c), file names compared case-insensitively. "no" needs every reference
clone present; with one absent and no hit in the other the answer is "unknown".

ra2-decomp counterpart (per unit, --units and --json only; not part of line 1): "yes" when the
Yuri's Revenge decompilation ra2-decomp has src/**/<unit>.cpp (case-insensitive), "no" when it
does not, "unknown" when its clone (private/reference/ra2-decomp) is absent.

Decompiler-style tokens (identifier occurrences in code: comments, string and character literals and
`__asm` blocks or lines removed first; every occurrence counts), by category:
  register   a 32-bit register used as an identifier: eax ebx ecx edx esi edi ebp esp, alone or with
             an underscore suffix (ecx_word). 16- and 8-bit names (ax, dl, ...) are not counted: they
             are also ordinary short names.
  ghidra_local  Ghidra variable names: local_<hex>, <prefix>Var<N> (iVar1, uVar2, pcVar3, ppuVar4),
             param_<N>, in_<REGISTER>... and in_stack_..., extraout_..., unaff_..., <prefix>Stack_<hex>
             (auStack_10, uStack_8), stack0x<hex>, register0x<hex>.
  ghidra_symbol  Ghidra global labels: FUN_..., DAT_..., PTR_..., LAB_..., thunk_FUN_..., switchD_...,
             caseD_....
  ghidra_type  Ghidra-only type names: undefined, undefined1..undefined8, uint3, int3, float10.
             (uint, byte, dword and the like are ordinary typedefs and are not counted.)
  ghidra_op  Ghidra operator macros used as calls: CONCAT<N>(, SUB<N>(, ZEXT<N>(, SEXT<N>(,
             CARRY<N>(, SCARRY<N>(, SBORROW<N>(, POPCOUNT<N>(.
Placeholder names (counted separately; allowed only when no real name can be established):
  identifiers starting unknown_ or Unknown followed by a capital, digit or underscore
  (unknown_0x24, Unknown_004F52F0, UnknownSessionA3D29C), and the address-named view and seed scopes
  (Leaf_/Call_/Cleanup_/Target_/Append_/Thunk_/View_/Owner_<hex>, seed_<hex>).
Address-named paths (lane CN2, 2026-11-16): a path is address-named when its file stem or any folder
on it carries a 6-8 digit hex address token (rename_files.ADDRESS_TOKEN), so a file inside
header_statics/<address>/ counts like a file named <address>.cpp. The tree line below the five
counts every tracked file under src/ that is address-named and every folder under src/ whose own
name is, which is what a reader of the tree sees. Files under src/recovered/unidentified/
(accepted sources with no evidenced name, each group's reason in its README; lane CN2) are shown
separately, and so are files under src/recovered/awaiting_join/ (accepted views with an evidenced
name that waits on a join, a layout or a population policy; reason per group in its README): the
end point allows address-named files in those two folders and nowhere else.

The five lines:
  1. Counterpart units reading as ordinary source: done = no decompiler-style token, no placeholder
     name and no address-named file in any of the unit's files; done / units with a counterpart.
  2. Units without a counterpart: done = no decompiler-style token and no address-named file
     (placeholder names allowed); done / units without a counterpart.
  3. Functions in their unit's file: catalog rows of a unit whose source is not the unit's module
     source (a unit without a module map has every row outside), against the recorded leftovers
     the end point allows: shared initialisers (rows in src/recovered/header_statics/, the
     header-static groups one include emits, and compiler-numbered $E/$S rows), rows held by
     Print_CRCs or WinMain (the module map's residual cause names them) and the .CRT$XCU groups
     (the residual cause names .CRT$XCU). The rows the module maps record as residuals (Route A
     split / flipped / kept, STATUS's numbers) and the rows no unit map entry covers (library units
     aside) are shown too. A row a unit's `joined` list in modules.json records (an accepted join
     under a DECISIONS ruling, e.g. the $E rows the original .CRT$XCU table places, with their
     unit_evidence tier) belongs to that unit when the unit map has no entry for it (lane E2): the
     join is the recorded evidence. The unit map itself is not extended with them, since
     xcu_units.py reads it for its prefix+map tier.
  4. Data: typed data produced from source and proven at its original addresses (the module maps'
     proven copies, deduplicated by address, as STATUS's non-code line reads them) / typed total
     (proven plus the unproven bytes of declared data ranges; module-map data items with no proof
     are counted as items, since the tracked tree carries no size for them); untyped bytes still
     taken from the original = STATUS's file-backed non-code bytes minus the proven bytes, the
     unproven typed bytes and the generated data modules (shown separately, not source).
  5. Non-exact drafts: count and bytes of manifests/original_target/non_exact.json.
  Tree line (not numbered): address-named files and folders under src/ (see Address-named paths).
"""
import argparse
from collections import Counter, defaultdict
import json
from pathlib import Path
import re
import sys

sys.path.insert(0, str(Path(__file__).resolve().parent))

REPOSITORY = Path(__file__).resolve().parents[2]
LIBRARY_UNIT = re.compile(r'^(?:libcmt|comsupp)/')

REGISTER = re.compile(r'^(?:e[abcd]x|e[sd]i|e[bs]p)(?:_\w+)?$')
GHIDRA_LOCAL = re.compile(r'^(?:local_[0-9a-fA-F]+(?:_\d+)?|[a-z]{1,5}Var\d+|param_\d+|in_(?:[A-Z]{2,}\w*|stack_\w+)'
                          r'|extraout_\w+|unaff_\w+|[a-z]{1,5}Stack_[0-9a-fA-F]+\w*|stack0x[0-9a-fA-F]+'
                          r'|register0x[0-9a-fA-F]+)$')
GHIDRA_SYMBOL = re.compile(r'^(?:thunk_FUN|FUN|DAT|PTR|LAB|switchD|caseD)_\w+$')
GHIDRA_TYPE = re.compile(r'^(?:undefined[1-8]?|uint3|int3|float10)$')
GHIDRA_OP = re.compile(r'^(?:CONCAT|SUB|ZEXT|SEXT|CARRY|SCARRY|SBORROW|POPCOUNT)\d{1,2}$')
PLACEHOLDER = re.compile(r'^(?:unknown_\w*|Unknown[A-Z0-9_]\w*|(?:Leaf|Call|Cleanup|Target|Append|Thunk|View|Owner)'
                         r'_[0-9a-fA-F]{6,8}(?:_\d+)?|seed_[0-9a-fA-F]{6,8})$')
CATEGORIES = ('register', 'ghidra_local', 'ghidra_symbol', 'ghidra_type', 'ghidra_op')
IDENTIFIER = re.compile(r'[A-Za-z_]\w*')
LEXEME = re.compile(r'//[^\n]*|/\*.*?\*/|"(?:\\.|[^"\\\n])*"|\'(?:\\.|[^\'\\\n])*\'', re.S)
ASM = re.compile(r'\b_?_asm\b\s*(?:\{[^}]*\}|[^\n]*)')
COMPILER_NUMBERED = re.compile(r'^_?\$[ES]\d+$')
HEADER_STATICS = 'src/recovered/header_statics/'
UNIDENTIFIED = 'src/recovered/unidentified/'
AWAITING_JOIN = 'src/recovered/awaiting_join/'
from rename_files import ADDRESS_TOKEN


def address_named(path):
    """True when the file stem or any folder on the path carries an address token."""
    parts = Path(path).parts
    return any(ADDRESS_TOKEN.search(Path(part).stem if i == len(parts) - 1 else part) for i, part in enumerate(parts))


def address_named_tree(paths):
    """{'files': [...], 'directories': [...]} of the address-named files of PATHS and the folders
    on them whose own name carries an address token."""
    files = sorted(p for p in paths if address_named(p))
    directories = set()
    for p in paths:
        parts = Path(p).parts[:-1]
        for i, part in enumerate(parts):
            if ADDRESS_TOKEN.search(part):
                directories.add(Path(*parts[:i + 1]).as_posix())
    return dict(files=files, directories=sorted(directories))


def tracked_sources(repository):
    """Tracked files under src/ (git ls-files; every file under src/ when git is unavailable)."""
    import subprocess
    repository = Path(repository)
    try:
        out = subprocess.run(['git', 'ls-files', 'src'], cwd=repository, capture_output=True, text=True, check=True)
        return [p for p in out.stdout.split('\n') if p]
    except (OSError, subprocess.CalledProcessError):
        return sorted(p.relative_to(repository).as_posix() for p in (repository / 'src').rglob('*') if p.is_file())


def code_only(text):
    """The text with comments, string and character literals and inline-assembly blocks blanked
    (line structure kept): register names inside `__asm` are instructions, not variables."""
    text = LEXEME.sub(lambda m: '\n' * m.group(0).count('\n') if m.group(0)[0] == '/' else ' ', text)
    return ASM.sub(lambda m: '\n' * m.group(0).count('\n'), text)


def classify(identifier, following=''):
    """The decompiler-style category of one identifier occurrence, 'placeholder', or None."""
    if REGISTER.match(identifier):
        return 'register'
    if GHIDRA_LOCAL.match(identifier):
        return 'ghidra_local'
    if GHIDRA_SYMBOL.match(identifier):
        return 'ghidra_symbol'
    if GHIDRA_TYPE.match(identifier):
        return 'ghidra_type'
    if GHIDRA_OP.match(identifier) and following.lstrip().startswith('('):
        return 'ghidra_op'
    if PLACEHOLDER.match(identifier):
        return 'placeholder'
    return None


def scan(text):
    """Counter of categories (CATEGORIES plus 'placeholder') over one file's code."""
    code = code_only(text)
    counts = Counter()
    for m in IDENTIFIER.finditer(code):
        kind = classify(m.group(0), code[m.end():m.end() + 8])
        if kind:
            counts[kind] += 1
    return counts


# ----------------------------------------------------------------------------- counterparts

def reference_stems(references):
    """{'tibsun': set|None, 'mischa85': set|None} of lower-case source stems; None when absent."""
    references = Path(references)
    found = {}
    for key, folder in (('tibsun', references / 'TibSun/code'), ('mischa85', references / 'mischa85-RedAlert2/src')):
        if folder.is_dir():
            found[key] = {p.stem.lower() for p in folder.rglob('*') if p.suffix.lower() in ('.cpp', '.c')}
        else:
            found[key] = None
    return found


def ra2decomp_stems(references):
    """Lower-case stems of ra2-decomp's src/**/*.cpp, or None when the clone is absent."""
    folder = Path(references) / 'ra2-decomp/src'
    if not folder.is_dir():
        return None
    return {p.stem.lower() for p in folder.rglob('*') if p.suffix.lower() == '.cpp'}


def ra2decomp_counterpart(unit, stems):
    """'yes' | 'no' | 'unknown' (clone absent): ra2-decomp has src/**/<unit>.cpp."""
    if stems is None:
        return 'unknown'
    return 'yes' if unit.rsplit('/', 1)[-1].lower() in stems else 'no'


def counterpart(unit, stems):
    """'yes' | 'no' | 'unknown' (a clone absent and no hit in the other)."""
    stem = unit.rsplit('/', 1)[-1].lower()
    if any(s is not None and stem in s for s in stems.values()):
        return 'yes'
    return 'no' if all(s is not None for s in stems.values()) else 'unknown'


# ----------------------------------------------------------------------------- the census

def load_inputs(repository=REPOSITORY):
    """Tracked inputs: unit map, module maps, module declarations, catalog regions, drafts, generated data."""
    import catalog_io
    repository = Path(repository)
    unit_map = json.loads((repository / 'manifests/original_target/unit_map.json').read_text())['units']
    modules = {}
    for path in sorted((repository / 'manifests/original_target/modules').glob('*.json')):
        entry = json.loads(path.read_text())
        modules[entry['module']] = entry
    declared = repository / 'manifests/original_target/modules.json'
    declared = json.loads(declared.read_text()) if declared.exists() else {}
    catalog = catalog_io.load(catalog_io.locate(repository), references=False)
    drafts = repository / 'manifests/original_target/non_exact.json'
    drafts = json.loads(drafts.read_text()).get('rows', []) if drafts.exists() else []
    generated = repository / 'manifests/original_target/generated_data.json'
    generated = json.loads(generated.read_text()).get('modules', []) if generated.exists() else []
    return dict(unit_map=unit_map, modules=modules, declared=declared, regions=catalog['regions'],
                drafts=drafts, generated=generated, tree=tracked_sources(repository))


def reader(repository):
    cache = {}

    def read(path):
        if path not in cache:
            try:
                cache[path] = (Path(repository) / path).read_text(errors='replace')
            except OSError:
                cache[path] = None
        return cache[path]
    return read


def leftover_class(row, residual):
    """The end-point leftover class of a row outside its unit's file, or None."""
    cause = (residual or {}).get('cause', '')
    if '.CRT$XCU' in cause:
        return 'xcu'
    if re.search(r'\b(?:Print_CRCs|WinMain)\b', cause):
        return 'print_crcs_winmain'
    if row['source'].startswith(HEADER_STATICS) or COMPILER_NUMBERED.match(function_name(row.get('symbol', ''))):
        return 'shared_initialiser'
    return None


def function_name(symbol):
    """'_$E12' -> '_$E12'; '?Foo@Bar@@...' -> 'Foo'."""
    if symbol.startswith('?'):
        return symbol[1:].split('@', 1)[0]
    return symbol


def census(inputs, read, stems, file_backed_non_code=None, ra2decomp=None):
    """The whole census as a dict: units (per-unit rows), the five measures and their parts."""
    if file_backed_non_code is None:
        from status import FILE_BACKED_NON_CODE as file_backed_non_code
    unit_map, modules, declared = inputs['unit_map'], inputs['modules'], inputs['declared']
    owner, library = {}, set()
    for unit, entries in unit_map.items():
        for e in entries:
            if LIBRARY_UNIT.match(unit):
                library.add(int(e, 16))
            else:
                owner[int(e, 16)] = unit
    for unit, item in declared.items():
        if LIBRARY_UNIT.match(unit):
            continue
        for joined in item.get('joined', []):
            owner.setdefault(int(joined['entry'], 16), unit)
    files = defaultdict(set)
    rows_of = defaultdict(list)
    unassigned = []
    for row in inputs['regions']:
        unit = owner.get(row['entry'])
        if unit is None:
            if row['entry'] not in library:
                unassigned.append(row)
            continue
        files[unit].add(row['source'])
        rows_of[unit].append(row)
    for draft in inputs['drafts']:
        unit = owner.get(int(draft['entry'], 16))
        if unit and draft.get('source'):
            files[unit].add(draft['source'])
    for unit in set(owner.values()):
        if unit in modules and modules[unit].get('source'):
            files[unit].add(modules[unit]['source'])
        header = (declared.get(unit) or {}).get('header') or (modules.get(unit) or {}).get('header')
        if header:
            files[unit].add(header)

    scans = {}
    units = []
    for unit in sorted(files):
        total, missing = Counter(), []
        for path in sorted(files[unit]):
            if path not in scans:
                text = read(path)
                scans[path] = None if text is None else scan(text)
            if scans[path] is None:
                missing.append(path)
            else:
                total.update(scans[path])
        tokens = sum(total[c] for c in CATEGORIES)
        named = sorted(p for p in files[unit] if address_named(p))
        units.append(dict(unit=unit, counterpart=counterpart(unit, stems),
                          ra2decomp_counterpart=ra2decomp_counterpart(unit, ra2decomp), files=len(files[unit]),
                          tokens=tokens, by_category={c: total[c] for c in CATEGORIES if total[c]},
                          placeholders=total['placeholder'], address_named_files=len(named),
                          missing_files=missing))
    for u in units:
        if u['counterpart'] == 'yes':
            u['done'] = not u['tokens'] and not u['placeholders'] and not u['address_named_files']
        else:
            u['done'] = not u['tokens'] and not u['address_named_files']

    # 3. rows outside their unit's file
    residuals = {}
    for m in modules.values():
        for r in m.get('residuals', []):
            residuals[int(r['entry'], 16)] = r
    outside, leftovers = [], Counter()
    leftover_bytes = Counter()
    recorded = [0, 0]
    for unit, rows in rows_of.items():
        home = (modules.get(unit) or {}).get('source')
        for row in rows:
            if row['source'] == home:
                continue
            outside.append(row)
            residual = residuals.get(row['entry'])
            if residual:
                recorded[0] += 1
                recorded[1] += row['body_bytes']
            kind = leftover_class(row, residual)
            if kind:
                leftovers[kind] += 1
                leftover_bytes[kind] += row['body_bytes']

    # 4. data
    import status
    proven, by_kind, _ = status.proven_data(list(modules.values()))
    unproven_range_bytes = sum(max(0, r['bytes'] - r['proven_bytes']) for m in modules.values()
                               for r in m.get('data_ranges', []))
    unproven_items = sum(len(m.get('data_unproven', [])) for m in modules.values())
    generated = sum(int(g['range'][1], 16) - int(g['range'][0], 16) for g in inputs['generated'])
    untyped = file_backed_non_code - proven - unproven_range_bytes - generated

    if 'tree' in inputs:
        tree = inputs['tree']
    else:                                         # no tree listing: the files the inputs name
        tree = sorted({p for fs in files.values() for p in fs} | {r['source'] for r in inputs['regions']})
    named_tree = address_named_tree(tree)

    with_cp = [u for u in units if u['counterpart'] == 'yes']
    without = [u for u in units if u['counterpart'] == 'no']
    unknown = [u for u in units if u['counterpart'] == 'unknown']
    return dict(
        units=units,
        counterpart_units=dict(done=sum(u['done'] for u in with_cp), total=len(with_cp)),
        other_units=dict(done=sum(u['done'] for u in without), total=len(without)),
        unknown_counterpart_units=dict(done=sum(u['done'] for u in unknown), total=len(unknown)),
        references={k: v is not None for k, v in stems.items()},
        functions=dict(outside_rows=len(outside), outside_bytes=sum(r['body_bytes'] for r in outside),
                       recorded_residual_rows=recorded[0], recorded_residual_bytes=recorded[1],
                       leftover_rows=sum(leftovers.values()), leftover_bytes=sum(leftover_bytes.values()),
                       leftovers={k: [leftovers[k], leftover_bytes[k]] for k in
                                  ('shared_initialiser', 'print_crcs_winmain', 'xcu')},
                       unassigned_rows=len(unassigned), unassigned_bytes=sum(r['body_bytes'] for r in unassigned)),
        data=dict(proven=proven, proven_by_kind=dict(by_kind), typed_total=proven + unproven_range_bytes,
                  unproven_range_bytes=unproven_range_bytes, unproven_items=unproven_items,
                  generated=generated, file_backed=file_backed_non_code, untyped_from_original=untyped),
        drafts=dict(count=len(inputs['drafts']), bytes=sum(int(d['bytes']) for d in inputs['drafts'])),
        address_named=dict(files=len(named_tree['files']), directories=len(named_tree['directories']),
                           unidentified=sum(p.startswith(UNIDENTIFIED) for p in named_tree['files']),
                           awaiting_join=sum(p.startswith(AWAITING_JOIN) for p in named_tree['files']),
                           file_list=named_tree['files'], directory_list=named_tree['directories']),
    )


def block(result):
    """The five-line Clean-up end point block and the address-named tree line."""
    cp, other, unk = result['counterpart_units'], result['other_units'], result['unknown_counterpart_units']
    f, d, n = result['functions'], result['data'], result['drafts']
    unknown_note = ''
    if unk['total']:
        unknown_note = '; counterpart unknown (reference clones absent): {:,} unit(s), {:,} done'.format(
            unk['total'], unk['done'])
    cp_text = '{:,} / {:,}'.format(cp['done'], cp['total']) if any(result['references'].values()) else 'unknown'
    other_text = ('{:,} / {:,}'.format(other['done'], other['total']) if all(result['references'].values())
                  else 'unknown' if not other['total'] else '{:,} / {:,} (so far)'.format(other['done'], other['total']))
    lo = f['leftovers']
    return '\n'.join([
        '1. Counterpart units reading as ordinary source (no decompiler-style token, no placeholder name, no '
        'address-named file): {}{}'.format(cp_text, unknown_note),
        '2. Units without a counterpart clean (no decompiler-style token, no address-named file; unknown_0xNN '
        'allowed): {}'.format(other_text),
        "3. Functions outside their unit's file / recorded leftovers: {:,} rows, {:,} bytes / {:,} rows, {:,} bytes "
        '(shared initialisers {:,}, held by Print_CRCs/WinMain {:,}, .CRT$XCU groups {:,}); module maps record '
        '{:,} of the outside rows as residuals; {:,} rows ({:,} bytes) in no unit'.format(
            f['outside_rows'], f['outside_bytes'], f['leftover_rows'], f['leftover_bytes'],
            lo['shared_initialiser'][0], lo['print_crcs_winmain'][0], lo['xcu'][0], f['recorded_residual_rows'],
            f['unassigned_rows'], f['unassigned_bytes']),
        '4. Data: typed data produced from source (proven) / typed total: {:,} / {:,} bytes ({:,} typed item(s) '
        'unproven, size not tracked); untyped bytes still taken from the original: {:,} of {:,} file-backed '
        '(generated data modules {:,} bytes, not source)'.format(
            d['proven'], d['typed_total'], d['unproven_items'], d['untyped_from_original'], d['file_backed'],
            d['generated']),
        '5. Non-exact drafts: {:,} functions, {:,} bytes'.format(n['count'], n['bytes']),
        'Address-named paths under src/ (an address token in a file stem or a folder name): {:,} files, '
        '{:,} directories; {:,} of the files in src/recovered/unidentified/, {:,} in src/recovered/awaiting_join/, '
        '{:,} elsewhere'.format(
            result['address_named']['files'], result['address_named']['directories'],
            result['address_named']['unidentified'], result['address_named']['awaiting_join'],
            result['address_named']['files'] - result['address_named']['unidentified']
            - result['address_named']['awaiting_join']),
    ])


def unit_lines(result, show_all=False):
    rows = sorted(result['units'], key=lambda u: (-u['tokens'], -u['placeholders'], -u['address_named_files'],
                                                  u['unit']))
    out = ['unit\tcounterpart\tra2-decomp counterpart\ttokens\tplaceholders\taddress_named_files\tfiles\tby_category']
    for u in rows:
        if u['done'] and not show_all:
            continue
        out.append('\t'.join([u['unit'], u['counterpart'], u['ra2decomp_counterpart'], str(u['tokens']), str(u['placeholders']),
                              str(u['address_named_files']), str(u['files']),
                              ' '.join('%s=%d' % kv for kv in sorted(u['by_category'].items()))]))
    return '\n'.join(out)


# ----------------------------------------------------------------------------- structure (lane S1)

STRUCTURE = """Structure census (lane S1, 2026-11-17): what the sister decompilations criticised, counted.

Read from tracked source (git ls-files src) and, for (d) only, the catalog and the reference clones.
Code is scanned with comments, strings and inline assembly blanked (code_only), except (f).
  (a) class/struct definitions with a body ('class X {', 'struct X : base {', 'template<..> class X {',
      'typedef struct X {'); forward declarations and 'enum class' are not definitions. A definition
      inside #ifdef RA2_NON_MATCHING counts like any other. Classes with more than one definition
      under src/, with the count per class and the files.
  (b) RA2_ preprocessor switches: RA2_ names read by #if/#ifdef/#ifndef/#elif/defined(), excluding
      include guards (an #ifndef X whose next directive is #define X); distinct switches and the
      files reading each. A guard name (_HPP/_H/_INC) read in another file (is that header included?)
      counts as a switch and is shown separately.
  (c) files under src/recovered/ that are not a unit's own file (a module map's "source"), by the
      number of catalog rows they hold (one-function files = exactly one row) and by folder.
  (d) functions whose counterpart is a class member while ours is a free function or a member of a
      stand-in class (Unknown*, an address-named or placeholder class). Counterparts: mischa85
      names.csv by exact address (same game.exe); ra2-decomp (Yuri's Revenge, other addresses) and
      TibSun by a unit-scoped name match: our name without its address suffix equals the method
      name of a 'Class::Method(' definition in that reference's file of our unit (ra2-decomp
      src/**/<unit>.cpp, TibSun code/<unit>.cpp) and no free function of that name is defined there.
  (e) address-suffixed identifiers: identifiers ending _<address> or _0x<address> with the address in
      the image's range 0x00400000-0x00BFFFFF (6-8 hex digits); distinct identifiers and occurrences.
  (f) '// @ref' comment lines (lines whose comment starts '@ref').
  (g) extern declarations at file scope in .cpp files (a line starting 'extern ', not 'extern "C" {'),
      with the files carrying them, the names some header under src/ already declares, and the names
      declared extern in more than one .cpp file (each a header's job).
"""

DEFINITION = re.compile(r'(?<![\w:])(?:typedef\s+)?(class|struct)\s+(?:__declspec\s*\([^)]*\)\s*)?'
                        r'([A-Za-z_][\w:]*)\s*(?:<[^;{}()]*>\s*)?(?::(?!:)[^;{}()]*)?\{')
ENUM_CLASS = re.compile(r'\benum\s+$')
DIRECTIVE = re.compile(r'^[ \t]*#[ \t]*(\w+)(.*)$', re.M)
RA2_NAME = re.compile(r'\bRA2_\w+')
ADDRESS_SUFFIXED = re.compile(r'^[A-Za-z_]\w*?_(?:0x)?(0{0,2}[4-9a-bA-B][0-9a-fA-F]{5})$')
LONG_SUFFIX = re.compile(r'_(?:0x)?00[0-9a-fA-F]{6}$')
REF_LINE = re.compile(r'^\s*//\s*@ref\b', re.M)
EXTERN_LINE = re.compile(r'^extern\s+(?!"C"\s*\{)(.*)$', re.M)
DECLARED_NAME = re.compile(r'([A-Za-z_]\w*)\s*(?:\[[^\]]*\]\s*)*(?:\([^;]*\))?\s*(?:const\s*)?;?\s*$')
STAND_IN_CLASS = re.compile(r'^(?:Unknown|unknown_)|_[0-9a-fA-F]{6,8}(?:_\d+)?$|^(?:Leaf|Call|Cleanup|Target|Append|'
                            r'Thunk|View|Owner)_')
METHOD_DEFINITION = re.compile(r'(?<![\w:~])([A-Za-z_]\w*)(?:<[^;{}()]*>)?::(~?[A-Za-z_]\w*)\s*\(')
SOURCE_SUFFIXES = ('.cpp', '.hpp', '.h', '.inc', '.c')


def class_definitions(text):
    """[(kind, name)] of the class/struct definitions with a body in TEXT (code already blanked)."""
    found = []
    for m in DEFINITION.finditer(text):
        if ENUM_CLASS.search(text[max(0, m.start() - 8):m.start()]):
            continue
        found.append((m.group(1), m.group(2)))
    return found


def preprocessor_switches(text):
    """Set of RA2_ names a file reads in #if/#ifdef/#ifndef/#elif, include guards excluded."""
    directives = [(m.group(1), m.group(2)) for m in DIRECTIVE.finditer(text)]
    names = set()
    for i, (word, rest) in enumerate(directives):
        if word not in ('if', 'ifdef', 'ifndef', 'elif'):
            continue
        found = set(RA2_NAME.findall(rest))
        if word == 'ifndef' and len(found) == 1 and i + 1 < len(directives):
            nxt = directives[i + 1]
            if nxt[0] == 'define' and nxt[1].split()[:1] == list(found):
                continue                                  # include guard
        names |= found
    return names


def address_suffixed(text):
    """Counter of address-suffixed identifiers in the code of TEXT."""
    out = Counter()
    for m in IDENTIFIER.finditer(code_only(text)):
        hit = ADDRESS_SUFFIXED.match(m.group(0))
        if hit and 0x400000 <= int(hit.group(1), 16) <= 0xBFFFFF:
            out[m.group(0)] += 1
    return out


def extern_names(text):
    """[(name, line)] of file-scope extern declarations in TEXT (code already blanked)."""
    out = []
    for m in EXTERN_LINE.finditer(text):
        body = m.group(1).strip()
        if body.startswith('"C"'):
            body = body[3:].strip()
        hit = DECLARED_NAME.search(body.split('=')[0].rstrip().rstrip(';') + ';')
        if hit:
            out.append((hit.group(1), m.group(0).strip()))
    return out


def symbol_parts(symbol):
    """(name, class or None, is_member) of a decorated MSVC name; (symbol, None, False) otherwise."""
    if not symbol.startswith('?') or symbol.startswith('??'):
        return symbol.lstrip('_'), None, False
    head = symbol[1:]
    name, _, rest = head.partition('@')
    if rest.startswith('@'):
        return name, None, False
    cls = rest.split('@', 1)[0]
    return name, cls, True


def base_name(name):
    """Our function name without its address suffix (Process_004bbf00 -> Process)."""
    return re.sub(r'_(?:0x)?[0-9a-fA-F]{6,8}$', '', name)


def reference_methods(path):
    """({method name: set of classes}, set of free function names) defined in one reference file."""
    try:
        text = code_only(Path(path).read_text(errors='replace'))
    except OSError:
        return {}, set()
    methods = defaultdict(set)
    for m in METHOD_DEFINITION.finditer(text):
        tail = text[m.end():m.end() + 400]
        close = tail.find(')')
        if close >= 0 and re.match(r'\s*(?:const\s*)?(?:throw\s*\([^)]*\)\s*)?(?::[^;{]*)?\{', tail[close + 1:]):
            methods[m.group(2)].add(m.group(1))
    free = set(re.findall(r'^[A-Za-z_][\w\s\*&<>,:]*?\b([A-Za-z_]\w*)\s*\([^;{]*\)\s*\{', text, re.M))
    free = {f for f in free if f not in methods}
    return methods, free


def unit_reference_files(references):
    """{'ra2-decomp': {stem: path}, 'tibsun': {stem: path}} (lower-case stems), empty when absent."""
    references = Path(references)
    out = {}
    for key, folder, pattern in (('ra2-decomp', references / 'ra2-decomp/src', '**/*.cpp'),
                                 ('tibsun', references / 'TibSun/code', '*')):
        files = {}
        if folder.is_dir():
            for p in folder.glob(pattern):
                if p.suffix.lower() in ('.cpp', '.c'):
                    files.setdefault(p.stem.lower(), p)
        out[key] = files
    return out


def mischa85_names(references):
    """{address: decorated name} from mischa85 names.csv, or None when absent."""
    path = Path(references) / 'mischa85-RedAlert2/config/ra2-1.006/names.csv'
    if not path.exists():
        return None
    import csv
    out = {}
    with path.open() as f:
        for row in csv.DictReader(f):
            try:
                out[int(row['address'], 16)] = row['symbol']
            except (KeyError, ValueError):
                continue
    return out


def member_counterparts(regions, unit_of, references):
    """Rows whose counterpart is a class member while ours is free or on a stand-in class."""
    names = mischa85_names(references)
    files = unit_reference_files(references)
    cache = {}
    rows = []
    for row in regions:
        ours, our_class, our_member = symbol_parts(row.get('symbol', ''))
        if ours.startswith(('$', '_$', '??')) or row.get('symbol', '').startswith('??'):
            continue
        ours_kind = 'free' if not our_member else 'stand_in' if STAND_IN_CLASS.search(our_class) else None
        if ours_kind is None:
            continue
        hits = {}
        if names is not None and row['entry'] in names:
            name, cls, member = symbol_parts(names[row['entry']])
            if member and not STAND_IN_CLASS.search(cls):
                hits['mischa85'] = '%s::%s' % (cls, name)
        unit = unit_of.get(row['entry'])
        stem = unit.rsplit('/', 1)[-1].lower() if unit else None
        want = base_name(ours)
        for key in ('ra2-decomp', 'tibsun'):
            path = files.get(key, {}).get(stem) if stem else None
            if path is None:
                continue
            if path not in cache:
                cache[path] = reference_methods(path)
            methods, free = cache[path]
            classes = methods.get(want)
            if classes and want not in free and len(classes) == 1:
                hits[key] = '%s::%s' % (next(iter(classes)), want)
        if hits:
            rows.append(dict(entry='%08x' % row['entry'], ours=row.get('symbol'), kind=ours_kind, unit=unit,
                             source=row['source'], bytes=row.get('body_bytes', 0), counterparts=hits))
    return rows


def structure(repository=REPOSITORY, references=None, tree=None, regions=None, unit_map=None, modules=None,
              read=None):
    """The structure census (STRUCTURE) as a dict."""
    repository = Path(repository)
    references = Path(references) if references else repository / 'private/reference'
    tree = tracked_sources(repository) if tree is None else tree
    read = read or reader(repository)
    sources = [p for p in tree if p.endswith(SOURCE_SUFFIXES)]

    definitions = defaultdict(list)
    switches = defaultdict(set)
    suffixed = Counter()
    suffixed_files = set()
    ref_lines, ref_files = 0, set()
    externs = defaultdict(list)
    header_text = []
    for path in sources:
        text = read(path)
        if text is None:
            continue
        code = code_only(text)
        for kind, name in class_definitions(code):
            definitions[name].append(path)
        for name in preprocessor_switches(text):
            switches[name].add(path)
        found = address_suffixed(text)
        if found:
            suffixed.update(found)
            suffixed_files.add(path)
        n = len(REF_LINE.findall(text))
        if n:
            ref_lines += n
            ref_files.add(path)
        if path.endswith(('.cpp', '.c')):
            for name, line in extern_names(code):
                externs[name].append(path)
        else:
            header_text.append(code)
    multi = {k: v for k, v in definitions.items() if len(v) > 1}
    top = sorted(multi.items(), key=lambda kv: (-len(kv[1]), kv[0]))

    headers = '\n'.join(header_text)
    header_names = set(IDENTIFIER.findall(headers))
    extern_lines = sum(len(v) for v in externs.values())
    extern_files = {p for v in externs.values() for p in v}
    in_header = {k for k in externs if k in header_names}
    repeated = {k: v for k, v in externs.items() if len(set(v)) > 1}

    if regions is None or unit_map is None or modules is None:
        inputs = load_inputs(repository)
        regions, unit_map, modules = inputs['regions'], inputs['unit_map'], inputs['modules']
    unit_of = {}
    for unit, entries in unit_map.items():
        for e in entries:
            unit_of[int(e, 16)] = unit
    homes = {m.get('source') for m in modules.values() if m.get('source')}
    rows_in = Counter(r['source'] for r in regions)
    recovered = [p for p in tree if p.startswith('src/recovered/') and p.endswith(('.cpp', '.c'))]
    outside = [p for p in recovered if p not in homes]
    one = [p for p in outside if rows_in[p] == 1]
    several = [p for p in outside if rows_in[p] > 1]
    none = [p for p in outside if rows_in[p] == 0]

    def folder(p):
        return Path(p).parent.as_posix()
    one_folders = Counter(folder(p) for p in one)
    members = member_counterparts(regions, unit_of, references)
    by_source = Counter()
    for m in members:
        for k in m['counterparts']:
            by_source[k] += 1

    return dict(
        classes=dict(defined=len(definitions), multiply_defined=len(multi),
                     extra_definitions=sum(len(v) - 1 for v in multi.values()),
                     top=[dict(name=k, definitions=len(v), files=sorted(set(v))) for k, v in top[:20]]),
        switches=dict(count=len(switches), files=len({p for v in switches.values() for p in v}),
                      guard_tests=sum(1 for k in switches if re.search(r'_(?:HPP|H|INC)$', k)),
                      by_switch={k: sorted(v) for k, v in sorted(switches.items(), key=lambda kv: (-len(kv[1]), kv[0]))}),
        files_outside=dict(unit_files=len([p for p in recovered if p in homes]), outside=len(outside),
                           one_function=len(one), one_function_folders=len(one_folders),
                           one_function_top_folders=len({Path(p).parts[2] for p in one if len(Path(p).parts) > 3}),
                           several_functions=len(several), no_row=len(none),
                           top_folders=one_folders.most_common(20)),
        member_counterparts=dict(rows=len(members), bytes=sum(m['bytes'] for m in members),
                                 free=sum(m['kind'] == 'free' for m in members),
                                 stand_in=sum(m['kind'] == 'stand_in' for m in members),
                                 by_reference=dict(by_source),
                                 references={'mischa85': mischa85_names(references) is not None,
                                             **{k: bool(v) for k, v in unit_reference_files(references).items()}},
                                 list=members),
        address_suffixed=dict(identifiers=len(suffixed), occurrences=sum(suffixed.values()),
                              files=len(suffixed_files),
                              eight_digit=sum(1 for k in suffixed if LONG_SUFFIX.search(k)),
                              six_digit=sum(1 for k in suffixed if not LONG_SUFFIX.search(k))),
        ref_lines=dict(lines=ref_lines, files=len(ref_files)),
        externs=dict(lines=extern_lines, names=len(externs), files=len(extern_files),
                     declared_in_a_header=len(in_header), in_several_cpp=len(repeated)),
    )


def structure_block(result):
    """The structure lines (a)-(g)."""
    c, s, f, m = result['classes'], result['switches'], result['files_outside'], result['member_counterparts']
    a, r, e = result['address_suffixed'], result['ref_lines'], result['externs']
    top = ', '.join('%s %d' % (t['name'], t['definitions']) for t in c['top'])
    refs = ', '.join('%s %d' % kv for kv in sorted(m['by_reference'].items())) or 'none'
    absent = [k for k, v in m['references'].items() if not v]
    return '\n'.join([
        '(a) Classes/structs defined more than once under src/: {:,} of {:,} defined names ({:,} extra '
        'definitions); top 20: {}'.format(c['multiply_defined'], c['defined'], c['extra_definitions'], top),
        '(b) RA2_ preprocessor switches: {:,} distinct ({:,} of them another header\'s include guard tested '
        'elsewhere), read in {:,} files'.format(s['count'], s['guard_tests'], s['files']),
        "(c) Files under src/recovered/ outside their unit's file: {:,} (one function {:,} in {:,} folders under {:,} "
        "top-level folders, several {:,}, no catalog row {:,}) beside {:,} unit files".format(
            f['outside'], f['one_function'], f['one_function_folders'], f['one_function_top_folders'],
            f['several_functions'], f['no_row'], f['unit_files']),
        '(d) Functions whose counterpart is a class member but ours is free or on a stand-in class: {:,} rows, '
        '{:,} bytes (free {:,}, stand-in {:,}; by reference: {}){}'.format(
            m['rows'], m['bytes'], m['free'], m['stand_in'], refs,
            '; absent: ' + ', '.join(absent) if absent else ''),
        '(e) Address-suffixed identifiers: {:,} distinct ({:,} name_00xxxxxx, {:,} name_xxxxxx), {:,} occurrences '
        'in {:,} files'.format(a['identifiers'], a['eight_digit'], a['six_digit'], a['occurrences'], a['files']),
        '(f) "// @ref" comment lines: {:,} in {:,} files'.format(r['lines'], r['files']),
        '(g) extern declarations in .cpp files: {:,} lines naming {:,} entities in {:,} files; {:,} names already '
        'declared in a header, {:,} declared extern in more than one .cpp'.format(
            e['lines'], e['names'], e['files'], e['declared_in_a_header'], e['in_several_cpp']),
    ])


def main():
    parser = argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    parser.add_argument('--repository', type=Path, default=REPOSITORY)
    parser.add_argument('--references', type=Path, help='reference clones (default <repository>/private/reference)')
    parser.add_argument('--block', action='store_true')
    parser.add_argument('--units', action='store_true')
    parser.add_argument('--all', action='store_true', help='with --units, also list units already done')
    parser.add_argument('--json', action='store_true')
    parser.add_argument('--structure', action='store_true', help='the structure block (a)-(g); with --json as JSON')
    args = parser.parse_args()
    references = args.references or args.repository / 'private/reference'
    if args.structure:
        result = structure(args.repository, references)
        print(json.dumps(result, indent=1) if args.json else structure_block(result))
        return
    result = census(load_inputs(args.repository), reader(args.repository), reference_stems(references),
                    ra2decomp=ra2decomp_stems(references))
    if args.json:
        print(json.dumps(result, indent=1))
    if args.units:
        print(unit_lines(result, args.all))
    if args.block or not (args.json or args.units):
        print(block(result))


if __name__ == '__main__':
    main()
