"""Structure census of the rebuild tree: tools/original_target/cleanup_census.py --structure pointed at
rebuild/ (tracked files under rebuild/src and rebuild/include, the rebuild catalog's rows, the units of
rebuild/manifest/units.json). Line (c) counts rebuild/src files that are not a unit's own file.
Exceptions are listed after the block: the RA2_NON_MATCHING draft guards and every address-suffixed
identifier with the file it is in (lane R).

Gate (lane R2, user 2026-11-17): `rebuild.py catalog` refuses to write the catalog when a structure
measure exceeds its budget in rebuild/manifest/structure_budget.json: scope-qualified duplicate class
definitions, files outside a unit, free-vs-member rows and tool-data comment lines have budget zero;
address-suffixed names in headers may only fall. Per unit (rebuild_catalog.py): a unit is left out of the
catalog when its source has more stray externs or address-suffixed names than its budget entry (units
landed before the gate keep their counts as budgets, to be lowered; a new unit's budget is zero).
Duplicates are counted by qualified name: a class nested in another class or local to a function is
named by its enclosing scopes (OwnerDraw::CellData and MapGen::CellData are two classes); the bare-name
count of the shared census is printed beside it."""
import json, re, subprocess
from collections import Counter
from pathlib import Path
import rebuild as R
import catalog_io
import cleanup_census as C


DEFINITION_AT = re.compile(r'(?<![\w:])(?:typedef\s+)?(class|struct|union)\s+(?:__declspec\s*\([^)]*\)\s*)?([A-Za-z_]\w*)[^;{()]*\{')
SCOPE_NAME = re.compile(r'(?:(?:class|struct|union|namespace)\s+(?:__declspec\s*\([^)]*\)\s*)?([A-Za-z_]\w*)[^;{()]*|([A-Za-z_~][\w:~]*)\s*\([^;{}]*\)[^;{}()]*)$')


def qualified_definitions(code):
    """[qualified name] of the class/struct definitions in CODE (comments and literals blanked): each
    named by its enclosing class, namespace or function scopes."""
    found, stack, i = [], [], 0
    starts = {m.end() - 1: m for m in DEFINITION_AT.finditer(code)
              if not code[:m.start()].rstrip().endswith(('<', ','))}    # template parameters
    for i, ch in enumerate(code):
        if ch == '{':
            head = code[max(0, i - 300):i]
            head = head[max(head.rfind(';'), head.rfind('}'), head.rfind('{')) + 1:].strip()
            m = SCOPE_NAME.search(head)
            stack.append((m.group(1) or m.group(2) + '()') if m else '')
            if i in starts and not C.ENUM_CLASS.search(code[max(0, starts[i].start() - 8):starts[i].start()]):
                found.append('::'.join([s for s in stack[:-1] if s] + [starts[i].group(2)]))
        elif ch == '}' and stack:
            stack.pop()
    return found


def structure_measures(tree, read, result):
    """The gated measures: scope-qualified duplicates (with their files) and the shared census's
    counts of the other lines."""
    defs = {}
    for p in tree:
        if not p.endswith(C.SOURCE_SUFFIXES):
            continue
        for name in qualified_definitions(C.code_only(read(p) or '')):
            defs.setdefault(name, []).append(p)
    dups = {k: v for k, v in defs.items() if len(v) > 1}
    return dict(duplicates=len(dups), duplicate_list=dups,
                outside=result['files_outside']['outside'],
                free_vs_member=result['member_counterparts']['rows'] if 'rows' in result['member_counterparts'] else
                result['member_counterparts']['free'] + result['member_counterparts']['stand_in'],
                tool_data=result['ref_lines']['lines'],
                externs=result['externs']['lines'],
                address_suffixed=result['address_suffixed']['identifiers'],
                address_suffixed_headers=len({n for p in tree if p.startswith('rebuild/include/')
                                              for n in C.address_suffixed(read(p) or '')}))


CHATTER = re.compile(r'\b[Ll]ane R\d*\b|\bcodex\d\b|\bWindows workers?\b|rebuild/(?:manifest|tools)|provenance\.csv|'
                     r'\bmanifest/|\btools/\w+\.py|rebuild trial|\(rebuild\)|\bold tree\b')


def placeholder_measures(tree, read):
    """Process chatter (budget zero), placeholder names and inherited constructs (budgets that only
    fall; review session, 2026-11-18)."""
    unknown, vt, field, fun = set(), set(), set(), set()
    chatter = guards = geometry = pragmas = extern_c = 0
    for p in tree:
        text = read(p) or ''
        chatter += sum(1 for line in text.splitlines() if CHATTER.search(line))
        code = C.code_only(text)
        unknown |= set(re.findall(r'\b[Uu]nknown\w*', code))
        vt |= set(re.findall(r'\bvt_entry_[0-9A-Fa-f]+\b', code))
        guards += len(re.findall(r'#ifdef RA2_NON_MATCHING', text))
        if p.startswith('rebuild/include/'):
            field |= set(re.findall(r'\bfield_[0-9A-Fa-f]+\b', code))
            fun |= set(re.findall(r'\bFUN_[0-9A-Fa-f]+\b', code))
        if p.startswith('rebuild/src/'):
            geometry += len(re.findall(r'^\s*#\s*define\s+GEOMETRY_H\b', text, re.M))
            pragmas += len(re.findall(r'^\s*#\s*pragma\s+auto_inline\s*\(\s*off', text, re.M))
            extern_c += len(re.findall(r'^[ \t]*extern\s+"C"', text, re.M))
    return dict(process_chatter=chatter, unknown_names=len(unknown), vt_entry_names=len(vt),
                field_names_headers=len(field), fun_names_headers=len(fun), draft_guards=guards,
                inherited_geometry_define=geometry, inherited_auto_inline_off=pragmas, extern_c_in_sources=extern_c)


# Gated keys (may only fall). draft_guards is reported only: the guards are the parked tail, and a new
# guarded draft is a legitimate parked attempt (owner ruling, 2026-11-18).
PLACEHOLDER_KEYS = ('process_chatter', 'unknown_names', 'vt_entry_names', 'field_names_headers', 'fun_names_headers',
                    'inherited_geometry_define', 'inherited_auto_inline_off', 'extern_c_in_sources')


LARGE = 1024    # a parked function of 1 KB or more (the checkpoint measure: "parked large functions pass 500 KB")


def parked_tail():
    """(rows, body bytes, rows of LARGE bytes or more, their bytes) not exact: the parked tail behind the
    guards. One definition for the census and progress.py (owner, 2026-11-18): every row of every unit in
    units.json that its check of the unit's current text does not find exact. A unit whose last check is
    missing or of older text is checked again first; a unit that does not compile counts all its rows."""
    import hashlib, io, contextlib
    rows = total = large = large_total = 0
    for unit, info in R.load_json(R.UNITS, {}).items():
        source = R.ROOT / info['source']
        last = R.load_json(R.WORK / 'last' / (unit + '.json'), None)
        current = hashlib.sha256(source.read_bytes()).hexdigest() if source.exists() else None
        if current and (last is None or last.get('source_sha256') != current):
            with contextlib.redirect_stdout(io.StringIO()):
                R.check(unit)
            last = R.load_json(R.WORK / 'last' / (unit + '.json'), None)
        fresh = bool(last) and last.get('source_sha256') == current
        status = {r['entry']: r['status'] for r in last['results']} if fresh else {}
        sensitive = info.get('count_sensitive') or {}
        for r in info['rows']:
            if status.get(r['entry']) != 'exact' and r['entry'] not in sensitive:
                rows += 1; total += r['body']
                if r['body'] >= LARGE:
                    large += 1; large_total += r['body']
    return rows, total, large, large_total


def count_sensitive():
    """(rows, body bytes) parked as "exact form, count-sensitive" (review ruling 2026-11-18, lane R20): rows
    whose exact text flips with the declaration count before them when an evidenced header changes. Each is
    listed in its unit's `count_sensitive` (entry -> windows, header, note) in units.json, guarded in the
    source, and kept apart from the parked tail; `countsens.py` tries them again after a header change."""
    rows = total = 0
    for info in R.load_json(R.UNITS, {}).values():
        body = {r['entry']: r['body'] for r in info['rows']}
        for entry in info.get('count_sensitive') or {}:
            rows += 1; total += body.get(entry, 0)
    return rows, total


def judgment_calls(regions):
    """(rows, body bytes) of the accepted rows flagged as judgment calls in provenance.csv (AGENTS.md
    Decision policy: real computed or looked-up values kept and flagged, each with its evidence)."""
    import csv
    accepted = {'%08x' % r['entry'] for r in regions}
    path = R.MANIFEST / 'provenance.csv'
    flagged = [r for r in csv.DictReader(open(path)) if (r.get('judgment') or '').strip() and r['entry'] in accepted] \
        if path.exists() else []
    return len(flagged), sum(int(r['body_bytes']) for r in flagged)


def unplaced_objects(regions):
    """(symbols, bytes) of the zero-initialised objects catalogued units leave unplaced and unreferenced
    (data_bound_by: unplaced-unreferenced; DECISIONS, user 2026-11-18): no address, nothing earned. Bytes are
    each symbol's extent in its unit's last checked object."""
    import data_bindings
    from coff import Coff
    sources = {r['source'] for r in regions}
    count = size = 0
    for unit, info in R.load_json(R.UNITS, {}).items():
        names = [n for n, k in (info.get('data_bound_by') or {}).items() if k == data_bindings.UNPLACED]
        last = R.load_json(R.WORK / 'last' / (unit + '.json'), None)
        if not names or info['source'] not in sources or not last:
            continue
        # The unit's last check must be of its current text; a stale or missing object is checked again
        # (the census never counts a symbol it cannot find in the object).
        import hashlib
        current = hashlib.sha256((R.ROOT / info['source']).read_bytes()).hexdigest()
        if last.get('source_sha256') != current or not Path(last['object']).exists():
            R.check(unit)
            last = R.load_json(R.WORK / 'last' / (unit + '.json'), None)
        coff = Coff(open(last['object'], 'rb').read())
        for name in names:
            try:
                index = data_bindings.defining_section(coff, name)
            except data_bindings.BindingRefused as error:
                print('  unplaced: %s: %s (not counted)' % (unit, error))
                continue
            symbols = data_bindings.section_symbols(coff, index)
            values = [v for v, _ in symbols] + [coff.sections[index - 1]['size']]
            position = [n for _, n in symbols].index(name)
            count += 1
            size += values[position + 1] - values[position]
    return count, size


def unit_faults(unit, source_text):
    """Per-unit structure faults over the unit's budget: (externs, budget), (suffixed, budget)."""
    budget = json.loads(BUDGET.read_text()) if BUDGET.exists() else {}
    import move_externs
    externs = len([n for n, l in C.extern_names(C.code_only(source_text)) if not move_externs.defines(source_text, n)])
    suffixed = len(set(C.address_suffixed(source_text)))
    faults = []
    if externs > budget.get('externs_by_unit', {}).get(unit, 0):
        faults.append('%d stray externs (budget %d)' % (externs, budget.get('externs_by_unit', {}).get(unit, 0)))
    if suffixed > budget.get('address_suffixed_by_unit', {}).get(unit, 0):
        faults.append('%d address-suffixed names (budget %d)' % (suffixed, budget.get('address_suffixed_by_unit', {}).get(unit, 0)))
    return faults


BUDGET = R.MANIFEST / 'structure_budget.json'


def gate(measures):
    """Violations of the budget (empty when the tree may land)."""
    budget = json.loads(BUDGET.read_text()) if BUDGET.exists() else {}
    return ['%s %d over budget %d' % (k, measures[k], budget.get(k, 0)) for k in
            ('duplicates', 'outside', 'free_vs_member', 'tool_data', 'address_suffixed_headers', 'address_suffixed')
            + PLACEHOLDER_KEYS if measures[k] > budget.get(k, 0)]


def main(quiet=False):
    files = subprocess.run(['git', '-C', str(R.ROOT), 'ls-files', 'rebuild/src', 'rebuild/include'],
                           capture_output=True, text=True).stdout.split()
    untracked = subprocess.run(['git', '-C', str(R.ROOT), 'ls-files', '--others', '--exclude-standard',
                                'rebuild/src', 'rebuild/include'], capture_output=True, text=True).stdout.split()
    tree = sorted(set(files) | set(untracked))
    cat = catalog_io.load(R.MANIFEST / 'catalog')
    units = R.load_json(R.UNITS, {})
    unit_map = {u: [r['entry'] for r in i['rows']] for u, i in units.items()}
    modules = {u: dict(source=i['source']) for u, i in units.items()}
    read = C.reader(R.ROOT)
    result = C.structure(R.ROOT, None, tree, cat['regions'], unit_map, modules, read)
    homes = {i['source'] for i in units.values()}
    srcs = [p for p in tree if p.startswith('rebuild/src/') and p.endswith(('.cpp', '.c'))]
    outside = [p for p in srcs if p not in homes]
    result['files_outside'].update(unit_files=len([p for p in srcs if p in homes]), outside=len(outside),
                                   one_function=0, one_function_folders=0, one_function_top_folders=0,
                                   several_functions=0, no_row=len(outside))
    measures = structure_measures(tree, read, result)
    measures.update(placeholder_measures(tree, read))
    measures['judgment_rows'], measures['judgment_bytes'] = judgment_calls(cat['regions'])
    measures['unplaced_symbols'], measures['unplaced_bytes'] = unplaced_objects(cat['regions'])
    measures['count_sensitive_rows'], measures['count_sensitive_bytes'] = count_sensitive()
    if quiet:
        return measures
    print(C.structure_block(result).replace('under src/', 'under rebuild/').replace('src/recovered/', 'rebuild/src/'))
    guards = Counter()
    suffixed = []
    for p in tree:
        text = read(p) or ''
        guards[p] += len(re.findall(r'#ifdef RA2_NON_MATCHING', text))
        for name in sorted(set(C.address_suffixed(text))):
            suffixed.append('%s (%s)' % (name, p))
    print('Exceptions: RA2_NON_MATCHING draft guards %d in %s' % (
        sum(guards.values()), ', '.join('%s %d' % kv for kv in sorted(guards.items()) if kv[1])))
    print('  switches: ' + ', '.join('%s (%d files)' % (k, len(v)) for k, v in result['switches']['by_switch'].items()))
    print('  address-suffixed: ' + ('; '.join(suffixed[:60]) or 'none'))
    print('  multiply defined: ' + ', '.join('%s %s' % (t['name'], t['files']) for t in result['classes']['top']))
    print('Gate: duplicates by qualified name %d%s; externs in sources %d; address-suffixed %d; outside %d; '
          'free-vs-member %d; tool data %d' % (measures['duplicates'], (' ' + ', '.join('%s %s' % kv for kv in
          sorted(measures['duplicate_list'].items()))) if measures['duplicates'] else '', measures['externs'],
          measures['address_suffixed'], measures['outside'], measures['free_vs_member'], measures['tool_data']))
    print('Gate (placeholders, only fall): process chatter %d; Unknown* names %d; vt_entry_NN %d; field_NN in headers %d; '
          'FUN_ in headers %d' % (measures['process_chatter'], measures['unknown_names'], measures['vt_entry_names'],
                                  measures['field_names_headers'], measures['fun_names_headers']))
    print('Parked tail (reported, not gated): RA2_NON_MATCHING guards %d; %d rows not exact, %d body bytes; '
          '1 KB or more: %d rows, %d body bytes' % ((measures['draft_guards'],) + parked_tail()))
    print('Judgment calls (reported, not gated): %d rows, %d body bytes' % (
        measures['judgment_rows'], measures['judgment_bytes']))
    print('Count-sensitive rows (exact form, parked by declaration count; not in the parked tail): %d rows, %d body bytes' % (
        measures['count_sensitive_rows'], measures['count_sensitive_bytes']))
    print('Unplaced unreferenced objects (data_bound_by: unplaced-unreferenced; earn nothing): %d symbols, %d bytes' % (
        measures['unplaced_symbols'], measures['unplaced_bytes']))
    print('Gate (inherited, to be evidenced): #define GEOMETRY_H in sources %d; #pragma auto_inline(off) %d; '
          'extern "C" in sources %d' % (measures['inherited_geometry_define'], measures['inherited_auto_inline_off'],
                                         measures['extern_c_in_sources']))
    print('Gate: ' + ('; '.join(gate(measures)) or 'within budget'))
    return measures


if __name__ == '__main__':
    main()
