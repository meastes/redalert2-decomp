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


def unit_faults(unit, source_text):
    """Per-unit structure faults over the unit's budget: (externs, budget), (suffixed, budget)."""
    budget = json.loads(BUDGET.read_text()) if BUDGET.exists() else {}
    externs = len(C.extern_names(C.code_only(source_text)))
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
            ('duplicates', 'outside', 'free_vs_member', 'tool_data', 'address_suffixed_headers')
            if measures[k] > budget.get(k, 0)]


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
    print('Gate: ' + ('; '.join(gate(measures)) or 'within budget'))
    return measures


if __name__ == '__main__':
    main()
