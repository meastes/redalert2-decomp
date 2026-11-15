#!/usr/bin/env python3
"""Clean-up end point census (lane K1, 2026-11-14): the five-line block the user agreed on.

    cleanup_census.py --block            the five lines
    cleanup_census.py --units [--all]    the per-unit work list (decompiler-style tokens descending)
    cleanup_census.py --json             everything as JSON
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
Address-named files: rename_files.address_named (a 6-8 digit hex address token in the file stem).

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
     aside) are shown too.
  4. Data: typed data produced from source and proven at its original addresses (the module maps'
     proven copies, deduplicated by address, as STATUS's non-code line reads them) / typed total
     (proven plus the unproven bytes of declared data ranges; module-map data items with no proof
     are counted as items, since the tracked tree carries no size for them); untyped bytes still
     taken from the original = STATUS's file-backed non-code bytes minus the proven bytes, the
     unproven typed bytes and the generated data modules (shown separately, not source).
  5. Non-exact drafts: count and bytes of manifests/original_target/non_exact.json.
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
from rename_files import address_named


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
                drafts=drafts, generated=generated)


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
    )


def block(result):
    """The five-line Clean-up end point block."""
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


def main():
    parser = argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    parser.add_argument('--repository', type=Path, default=REPOSITORY)
    parser.add_argument('--references', type=Path, help='reference clones (default <repository>/private/reference)')
    parser.add_argument('--block', action='store_true')
    parser.add_argument('--units', action='store_true')
    parser.add_argument('--all', action='store_true', help='with --units, also list units already done')
    parser.add_argument('--json', action='store_true')
    args = parser.parse_args()
    references = args.references or args.repository / 'private/reference'
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
