#!/usr/bin/env python3
"""Rebuild trial tool (lane R, 2026-11-17): unit files checked against main's frozen bounds.

    rebuild.py init UNIT [--source NAME] [--options N]   add UNIT to rebuild/manifest/units.json from
                                                         mischa85's module range and main's frozen rows
    rebuild.py check UNIT [--asm ENTRY] [--learn]        compile rebuild/src/<unit>.cpp once, compare every
                                                         row with the original (references resolved), list
                                                         the object's other functions; --learn records the
                                                         symbol a row's exact bytes were found under
    rebuild.py catalog                                    write rebuild/manifest/catalog/ (the rebuild tree's
                                                         canonical catalog: only rebuilt units, everything
                                                         else stays original bytes) from the last checks
    rebuild.py census                                     structure census of the rebuild tree

Data lives beside the sources, never inside them (rebuild/manifest/):
  units.json        unit -> source, options, rows (entry, body, padding, symbol), status
  symbols.csv       extra name -> address bindings (names the rebuild tree introduced)
  provenance.csv    one line per accepted function: a adopted text / b old body reshaped / c fresh

References resolve in this order: the unit's own bindings in units.json (per-unit names such as
_$E<n>, string literals), rebuild/manifest/symbols.csv, names main's catalog binds to one address
only, mischa85 config/ra2-1.006/names.csv. A name bound to two addresses or to none refuses.
Bounds and padding come only from main's catalog rows (its frozen inventory passes); a unit row
that main has not accepted has no independent bound here and is reported, never accepted.
"""
import argparse
import csv
import json
import os
from pathlib import Path
import sys
import time

os.environ['RA2_TREE'] = 'rebuild'
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'tools/original_target'))

import catalog_io                                   # noqa: E402
from verify_manifest import Audit                   # noqa: E402

REBUILD = ROOT / 'rebuild'
MANIFEST = REBUILD / 'manifest'
UNITS = MANIFEST / 'units.json'
SYMBOLS = MANIFEST / 'symbols.csv'
MISCHA = ROOT / 'private/reference/mischa85-RedAlert2'
CONFIG = ROOT / 'private/original-target.interactive.json'
WORK = ROOT / 'build/r'
OPTION_SETS = [['/O2', '/Ob2', '/G5', '/Gy'], ['/O2', '/Ob2', '/G6', '/Gy'],
               ['/O1', '/Ob2', '/G5', '/Gy'], ['/O2', '/Ob2', '/G5', '/Gy', '/Gr'],
               ['/O2', '/Ob1', '/G5', '/Gy'], ['/O2', '/Ob2', '/G5', '/Gy', '/GR'],
               ['/O2', '/Ob2', '/G5', '/Gy', '/Gr', '/GR']]


def load_json(path, default):
    return json.loads(path.read_text()) if path.exists() else default


def write_json(path, value):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(value, indent=1, sort_keys=False) + '\n')


_CATALOG = []


def main_catalog():
    """Main's catalog (read-only here), cached in build/r keyed by its index digest."""
    if not _CATALOG:
        import hashlib, pickle
        index = ROOT / 'manifests/original_target/index.json'
        key = hashlib.sha256(index.read_bytes()).hexdigest()[:16]
        cache = WORK / ('main-catalog-%s.pickle' % key)
        if cache.exists():
            _CATALOG.append(pickle.loads(cache.read_bytes()))
        else:
            _CATALOG.append(catalog_io.load(ROOT / 'manifests/original_target'))
            cache.parent.mkdir(parents=True, exist_ok=True)
            cache.write_bytes(pickle.dumps(_CATALOG[0], protocol=pickle.HIGHEST_PROTOCOL))
    return _CATALOG[0]


def mischa_modules():
    with open(MISCHA / 'config/ra2-1.006/modules.csv') as f:
        return {r['unit']: r for r in csv.DictReader(f)}


def mischa_names():
    with open(MISCHA / 'config/ra2-1.006/names.csv') as f:
        return {r['symbol']: int(r['address'], 16) for r in csv.DictReader(f)}


_GLOBAL = []


def global_table(catalog):
    """Names main binds to exactly one address (population tables and accepted symbols)."""
    if _GLOBAL:
        return _GLOBAL[0]
    _GLOBAL.append(_global_table(catalog))
    return _GLOBAL[0]


def _global_table(catalog):
    seen = {}
    for table in catalog['population_references'].values():
        for name, address in table.items():
            seen.setdefault(name, set()).add(address)
    for row in catalog['regions']:
        seen.setdefault(row['symbol'], set()).add(row['entry'])
    return {n: next(iter(a)) for n, a in seen.items() if len(a) == 1}, {n for n, a in seen.items() if len(a) > 1}


def extra_symbols():
    if not SYMBOLS.exists():
        return {}
    with open(SYMBOLS) as f:
        return {r['symbol']: int(r['address'], 16) for r in csv.DictReader(f)}


def init_unit(unit, source=None, option_index=None, start=None, end=None, auto=False):
    """start/end widen mischa85's module range to the unit's whole object contribution: its
    `_$E` initializers before it and its inline/template copies after it (the original linker
    keeps each object's code together). Symbols already assigned to a row are kept."""
    units = load_json(UNITS, {})
    previous = {r['entry']: r for r in units.get(unit, {}).get('rows', [])}
    modules = mischa_modules()
    module = modules[unit]
    start = int(start, 16) if start else int(module['start'], 16)
    end = int(end, 16) if end else int(module['end'], 16)
    if auto:
        # The unit's object contribution: the run of `_$E` initializer rows just before mischa85's
        # range, and after it everything up to the next module (whose own leading `_$E` run is its).
        rows_sorted = sorted(main_catalog()['regions'], key=lambda r: r['entry'])
        before = [r for r in rows_sorted if r['entry'] < start]
        while before and before[-1]['symbol'].startswith('_$E') and \
                before[-1]['entry'] + before[-1]['full_extent_bytes'] >= (start if not before[:-1] else start) - 16:
            start = before.pop()['entry']
        # Main's own module map may put an initializer run further back (a gap of rows main has not
        # accepted between it and the unit's first function): the run whose rows main files under
        # this unit's source is the unit's too (lane R2).
        while before and before[-1]['symbol'].startswith('_$E') and \
                before[-1]['source'].rsplit('/', 1)[-1] == unit + '.cpp':
            start = before.pop()['entry']
        # After it: the run of compiler-emitted copies (template members, deleting destructors,
        # vector iterators) up to the next module or the next row of any other kind.
        following = sorted((int(m['start'], 16) for m in modules.values() if m['category'] != 'unknown' and
                            int(m['start'], 16) >= end))
        nxt = following[0] if following else end
        for r in [r for r in rows_sorted if end <= r['entry'] < nxt]:
            s = r['symbol']
            if not (s.startswith(('??_G', '??_E', '??_H', '??_I', '?get@Leaf_', '?get@Storage_')) or '?$' in s):
                break
            end = r['entry'] + r['full_extent_bytes']
    catalog = main_catalog()
    rows = sorted((r for r in catalog['regions'] if start <= r['entry'] < end), key=lambda r: r['entry'])
    names = {a: s for s, a in mischa_names().items()}
    options = {tuple(r['options']) for r in rows}
    if option_index is not None:
        chosen = OPTION_SETS[option_index]
    else:
        counts = {}
        for r in rows:
            counts[tuple(r['options'])] = counts.get(tuple(r['options']), 0) + r['body_bytes']
        chosen = list(max(counts, key=counts.get)) if counts else OPTION_SETS[0]
    overrides = units.get(unit, {}).get('symbol_overrides', {})
    units[unit] = dict(
        core=[module['start'][2:].lower().zfill(8), module['end'][2:].lower().zfill(8)],
        symbol_overrides=overrides, source='rebuild/src/%s.cpp' % (source or unit), options=chosen, status='pending',
        range=['%08x' % start, '%08x' % end], mischa85=module['category'] + ': ' + module['evidence'],
        main_options=sorted(map(list, options)),
        rows=[dict(entry='%08x' % r['entry'], body=r['body_bytes'], padding=r['padding_bytes'],
                   symbol=names.get(r['entry'], r['symbol']), main_symbol=r['symbol'],
                   main_source=r['source']) for r in rows],
        references=units.get(unit, {}).get('references', {}))
    overrides = units[unit].get('symbol_overrides', {})
    for row in units[unit]['rows']:
        if row['entry'] in previous:
            row['symbol'] = previous[row['entry']]['symbol']
        row['symbol'] = overrides.get(row['entry'], row['symbol'])
    if previous:
        units[unit]['options'] = units_options = load_json(UNITS, {})[unit]['options']
        for key in ('partial_ok',):
            if key in load_json(UNITS, {})[unit]:
                units[unit][key] = load_json(UNITS, {})[unit][key]
    covered = sum(r['body_bytes'] + r['padding_bytes'] for r in rows)
    write_json(UNITS, units)
    print('%s: %d rows, %d of %d bytes of the module range accepted in main; options %s' % (
        unit, len(rows), covered, end - start, ' '.join(chosen)))


def include_closure(text, roots, seen=None, here=None, prefix=''):
    """Quoted includes of `text` resolved in `roots` (first hit), transitively; an include is
    first looked for beside the including header (`here`), as the compiler does."""
    import re
    seen = {} if seen is None else seen
    for spelled in re.findall(r'^\s*#\s*include\s+"([^"]+)"', text, flags=re.M):
        name = spelled
        if here is not None and (here / spelled.replace('\\', '/')).exists():
            name = prefix + spelled
            if name not in seen:
                seen[name] = here / spelled.replace('\\', '/')
                include_closure(seen[name].read_text(errors='replace'), roots, seen, seen[name].parent, prefix)
            continue
        if name in seen:
            continue
        for root in roots:
            if (root / name.replace('\\', '/')).exists():
                seen[name] = root / name.replace('\\', '/')
                sub = name.replace('\\', '/').rpartition('/')[0]
                include_closure(seen[name].read_text(errors='replace'), roots, seen,
                                seen[name].parent if sub else None, sub + '/' if sub else '')
                break
        else:
            seen[name] = None
    return seen


def adopt(unit, origin='mischa85', source_only=False, headers_only=False):
    """Copy an origin's unit file and the headers it needs that the rebuild tree lacks.
    Existing rebuild headers are never overwritten (one definition per class): a differing
    origin copy is reported for a manual merge. Each copied file is listed in
    rebuild/manifest/adopted.csv with its origin and commit."""
    import hashlib, subprocess
    roots = {'mischa85': (MISCHA, ['src', 'include']),
             'tibsun': (ROOT / 'private/reference/TibSun', ['code', 'code'])}[origin]
    base, (src_dir, inc_dir) = roots[0], roots[1]
    commit = subprocess.run(['git', '-C', str(base), 'rev-parse', '--short', 'HEAD'], capture_output=True,
                            text=True).stdout.strip() or (base / 'dbfbfbf').exists() and 'dbfbfbf' or '?'
    source = base / src_dir / (unit + '.cpp')
    target = REBUILD / 'src' / (unit + '.cpp')
    ledger = MANIFEST / 'adopted.csv'
    rows = list(csv.DictReader(open(ledger))) if ledger.exists() else []
    def record(path, origin_path):
        rel = path.relative_to(ROOT).as_posix()
        rows[:] = [r for r in rows if r['file'] != rel]
        rows.append(dict(file=rel, origin=origin, commit=commit, origin_path=origin_path.relative_to(base).as_posix(),
                         sha256=hashlib.sha256(path.read_bytes()).hexdigest()))
    if headers_only:
        target = source     # the closure of the origin's unit file; the unit itself is not copied
    elif target.exists():
        print('kept existing %s' % target.relative_to(ROOT))
    else:
        target.write_bytes(source.read_bytes()); record(target, source); print('adopted %s' % source.relative_to(base))
    if not source_only:
        closure = include_closure(target.read_text(errors='replace'), [REBUILD / 'include', base / inc_dir, base / src_dir])
        for name, path in sorted(closure.items()):
            dest = REBUILD / 'include' / name.replace('\\', '/')
            if path is None:
                print('  unresolved include "%s"' % name)
            elif dest.exists():
                other = base / inc_dir / name.replace('\\', '/')
                if other.exists() and other.read_bytes() != dest.read_bytes():
                    print('  differs from origin (kept rebuild copy): %s' % name)
            else:
                dest.parent.mkdir(parents=True, exist_ok=True)
                dest.write_bytes(path.read_bytes()); record(dest, path); print('  header %s' % name)
    with open(ledger, 'w', newline='') as f:
        w = csv.DictWriter(f, fieldnames=['file', 'origin', 'commit', 'origin_path', 'sha256'])
        w.writeheader(); w.writerows(sorted(rows, key=lambda r: r['file']))


def compile_unit(unit, info, tag=''):
    import build
    from canonical import compiler_arguments
    config = json.loads(CONFIG.read_text())
    folder = WORK / 'check' / (unit + tag) / time.strftime('%H%M%S')
    folder.mkdir(parents=True, exist_ok=True)
    source = ROOT / info['source']
    obj = folder / (unit + '.obj')
    started = time.monotonic()
    build.compile_units(config, folder, [dict(cwd=folder, source=source, obj=obj, log=folder / 'compile.log',
                                              returncode=folder / 'rc.txt',
                                              args=compiler_arguments(info['options']))],
                        'rebuild-check', parallel=False, timeout=600)
    log = (folder / 'compile.log').read_text(errors='replace') if (folder / 'compile.log').exists() else ''
    rc = int((folder / 'rc.txt').read_text().strip() or 1) if (folder / 'rc.txt').exists() else 1
    return obj if rc == 0 and obj.exists() else None, log, time.monotonic() - started, folder


def resolver(unit, info):
    catalog = main_catalog()
    unique, ambiguous = global_table(catalog)
    extra = extra_symbols()
    mischa = mischa_names()
    # The unit's own main populations bind per-unit names (string literals, _$E, statics) first.
    pops = {r['population'] for r in catalog['regions']
            if int(info['range'][0], 16) <= r['entry'] < int(info['range'][1], 16)}
    own = {}
    for p in sorted(pops):
        for name, address in catalog['population_references'].get(p, {}).items():
            own.setdefault(name, set()).add(address)
    local = {n: next(iter(a)) for n, a in own.items() if len(a) == 1}
    # The rows of every rebuild unit bind their own symbols to their entries.
    rows = {}
    for name, other in load_json(UNITS, {}).items():
        for row in other['rows']:
            # Initializer and guard names repeat in every unit: only this unit's own rows bind them.
            if row['symbol'].startswith(('_$E', '_$S', '$T')) and other.get('source') != info.get('source'):
                continue
            rows[row['symbol']] = int(row['entry'], 16)

    def resolve(name):
        per_unit = name.startswith(('_$E', '_$S', '$T'))
        for table, label in ((info.get('references', {}), 'unit'), (extra, 'symbols.csv'), (rows, 'rows'), (local, 'main-unit'),
                             (unique, 'main'), (mischa, 'mischa85')):
            if per_unit and label in ('main', 'mischa85', 'symbols.csv', 'main-unit'):
                continue    # initializer names repeat in every unit: only this unit's own bindings
            if name in table:
                value = table[name]
                return (int(value, 16) if isinstance(value, str) else value), label
        # Main spells VectorClass's element accessor Fetch (TibSun's name); mischa85 GetItem.
        if name.startswith('?GetItem@?$VectorClass@'):
            alias = '?Fetch@' + name[len('?GetItem@'):]
            if alias in unique:
                return unique[alias], 'main (Fetch spelling)'
        return None, ('ambiguous in main' if name in ambiguous else 'unbound')
    return resolve


def compare(audit, coff, symbol_name, entry, body, padding, resolve):
    """Resolved bytes of `symbol_name` against the original body (+ frozen padding, which an
    object never holds: VC6 aligns in the linker). Returns (status, differing, bindings, problems)."""
    import struct
    from verify_manifest import local_label_target
    symbol, section, start, end = coff.function(symbol_name)
    actual = bytearray(section['data'][start:end]); bindings, problems, unbound = [], [], []
    for offset, index, kind in section['relocs']:
        if not start <= offset < end:
            continue
        offset -= start
        target_symbol = coff.symbols[index]
        local = local_label_target(coff, target_symbol, symbol['section'], start, end, entry)
        if local is not None:
            target, label = local, 'local'
        else:
            target, label = resolve(target_symbol['name'])
        if target is None or kind not in (6, 20):
            problems.append('%s (%s)' % (target_symbol['name'], label))
            if kind in (6, 20):
                unbound.append((target_symbol['name'], offset, kind, struct.unpack_from('<I', actual, offset)[0]))
            continue
        addend = struct.unpack_from('<I', actual, offset)[0]
        if label in ('main', 'main-unit', 'mischa85'):
            unbound.append((target_symbol['name'], offset, kind, addend))     # weak: may be a per-unit copy
        resolved = (target + addend - (entry + offset + 4 if kind == 20 else 0)) & 0xffffffff
        struct.pack_into('<I', actual, offset, resolved)
        bindings.append(dict(offset=offset, kind=kind, symbol='$local' if local is not None else target_symbol['name'],
                             target=target, addend=addend, resolved=resolved))
    expected = audit.original(entry, body)
    pad = audit.original(entry + body, padding) if padding else b''
    if len(actual) == body + padding and bytes(actual[body:]) == pad:
        actual = actual[:body]
    differing = sum(a != b for a, b in zip(actual, expected)) + abs(len(actual) - len(expected))
    status = 'exact' if not differing and not problems and pad == b'\x90' * padding else 'differs'
    compare.actual = bytes(actual)
    # Unbound names whose sites are the only differences: the address each site implies.
    compare.inferred = []
    if unbound and len(actual) == len(expected):
        fields = {o + i for _, o, _, _ in unbound for i in range(4)}
        if all(i in fields for i, (a, b) in enumerate(zip(actual, expected)) if a != b):
            for name, offset, kind, addend in unbound:
                if not any(a != b for a, b in zip(actual[offset:offset + 4], expected[offset:offset + 4])) and \
                        name not in {n for n, *_ in unbound if n in problems_names(problems)}:
                    continue
                value = struct.unpack_from('<I', expected, offset)[0]
                target = (value - addend + (entry + offset + 4 if kind == 20 else 0)) & 0xffffffff
                compare.inferred.append((name, target))
    return status, differing, len(actual), bindings, problems


def problems_names(problems):
    return {p.rsplit(' (', 1)[0] for p in problems}


def disasm_diff(audit, actual_bytes, entry, body):
    import capstone, difflib
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    import re
    def norm(i, lo, hi):
        text = '%s %s' % (i.mnemonic, i.op_str)
        if i.mnemonic.startswith('j'):     # local branch targets differ whenever lengths differ
            text = re.sub(r'0x[0-9a-f]+', lambda m: 'L' if lo <= int(m.group(0), 16) < hi else m.group(0), text)
        return text
    a = [norm(i, entry, entry + len(actual_bytes)) for i in md.disasm(bytes(actual_bytes), entry)]
    b = [norm(i, entry, entry + body) for i in md.disasm(audit.original(entry, body), entry)]
    return '\n'.join(difflib.unified_diff(b, a, 'original', 'rebuild', n=2, lineterm=''))


PER_UNIT = ('_$E', '_$S', '??_C@', '$T', '_?', '??_7', '??_R', '_IID_', '__GUID_', '_CLSID_')


def learn_bindings(unit, info, inferred):
    """Record names whose every site (in rows that differ only at unbound sites) implies one
    address: per-unit names (initialisers, literals, function statics) in the unit's own table,
    others in symbols.csv with the evidence. A name implying two addresses is reported."""
    rows = list(csv.DictReader(open(SYMBOLS))) if SYMBOLS.exists() else []
    known = {r['symbol'] for r in rows}
    for name, targets in sorted(inferred.items()):
        if len(targets) != 1:
            print('  conflicting sites for %s: %s' % (name, {('%08x' % t): v for t, v in targets.items()}))
            continue
        (target, sites), = targets.items()
        if name.startswith(PER_UNIT) or '@?' in name or not name.startswith('?'):   # C-named statics are per unit
            info.setdefault('references', {})[name] = '%08x' % target
        elif name not in known:
            rows.append(dict(symbol=name, address='0x%08X' % target,
                             evidence='inferred: %s rows %s differ only at its sites' % (unit, ' '.join(sites[:4]))))
        print('  learned %s = %08x (%d sites)' % (name, target, len(sites)))
    with open(SYMBOLS, 'w', newline='') as f:
        w = csv.DictWriter(f, fieldnames=['symbol', 'address', 'evidence'])
        w.writeheader(); w.writerows(sorted(rows, key=lambda r: r['symbol']))


def check(unit, asm=(), learn=False):
    from coff import Coff
    units = load_json(UNITS, {})
    info = units[unit]
    obj, log, seconds, folder = compile_unit(unit, info)
    print('%s: compile %.1fs (%s)' % (unit, seconds, folder.relative_to(ROOT)))
    if obj is None:
        print('\n'.join(l for l in log.splitlines() if 'error' in l or 'warning C4' in l and False)[:6000] or log[-3000:])
        return 2
    config = json.loads(CONFIG.read_text())
    audit = Audit(ROOT, config['executable'])
    coff = Coff(obj.read_bytes())
    resolve = resolver(unit, info)
    functions = {s['name']: s for s in coff._functions}
    used, results, exact_bytes, inferred = set(), [], 0, {}
    claimed = {row['symbol'] for row in info['rows']}
    for row in info['rows']:
        entry = int(row['entry'], 16)
        name = row['symbol']
        found = name if name in functions else None
        if found and learn and resolve(found)[1] == 'symbols.csv' and resolve(found)[0] != entry:
            found = None    # the name belongs to another address (a vtable slot says so): learn again
        result = dict(entry=row['entry'], symbol=name, body=row['body'])
        if found is None and learn and row['body'] >= 8:     # tiny bodies (ret) match any empty function
            # The symbol whose resolved bytes equal the original body (renamed in the rebuild tree).
            # Among byte-identical candidates (template instances share code) the name closest to
            # main's spelling for the address wins.
            import difflib
            exact_names = []
            for candidate in functions:
                if candidate in used or candidate in claimed:
                    continue
                bound, label = resolve(candidate)
                if bound is not None and bound != entry and label != 'rows':
                    continue    # the name is another copy's (a template instance kept elsewhere)
                try:
                    status, *_ = compare(audit, coff, candidate, entry, row['body'], row['padding'], resolve)
                except ValueError:
                    continue
                if status == 'exact':
                    exact_names.append(candidate)
            if exact_names:
                found = max(exact_names, key=lambda n: difflib.SequenceMatcher(None, n, row['main_symbol']).ratio())
                row['symbol'] = found
        if found is None:
            result.update(status='missing')
        else:
            used.add(found)
            status, differing, size, bindings, problems = compare(audit, coff, found, entry, row['body'],
                                                                  row['padding'], resolve)
            result.update(status=status, differing=differing, size=size, problems=problems,
                          relocations=bindings)
            for name, target in compare.inferred:
                inferred.setdefault(name, {}).setdefault(target, []).append(row['entry'])
            if status == 'exact':
                exact_bytes += row['body']
            if row['entry'] in asm or (asm and 'all' in asm and status != 'exact'):
                print(disasm_diff(audit, compare.actual, entry, row['body']))
        results.append(result)
    others = sorted(n for n in functions if n not in used)
    exact = sum(r['status'] == 'exact' for r in results)
    total = sum(r['body'] for r in info['rows'])
    for r in results:
        if r['status'] != 'exact':
            print('  %s %-8s %s %s %s' % (r['entry'], r['status'], r['symbol'][:90],
                                         r.get('differing', ''), ' '.join(r.get('problems', []))[:300]))
    print('%s: %d/%d rows exact, %d/%d body bytes; other functions in the object: %d' % (
        unit, exact, len(results), exact_bytes, total, len(others)))
    for n in others:
        address, label = resolve(n)
        print('    other: %s -> %s (%s)' % (n[:110], '%08x' % address if address is not None else '-', label))
    write_json(WORK / 'last' / (unit + '.json'), dict(unit=unit, object=str(obj), results=results, others=others,
                                                       source_sha256=__import__('hashlib').sha256(
                                                           (ROOT / info['source']).read_bytes()).hexdigest(),
                                                       time=time.time()))
    if learn and inferred:
        learn_bindings(unit, info, inferred)
    if learn:
        write_json(UNITS, units)
    return 0 if exact == len(results) else 1


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = parser.add_subparsers(dest='command', required=True)
    p = sub.add_parser('init'); p.add_argument('unit'); p.add_argument('--source'); p.add_argument('--options', type=int)
    p.add_argument('--start'); p.add_argument('--end'); p.add_argument('--auto', action='store_true')
    p = sub.add_parser('check'); p.add_argument('unit'); p.add_argument('--asm', action='append', default=[])
    p.add_argument('--learn', action='store_true')
    p = sub.add_parser('adopt'); p.add_argument('unit'); p.add_argument('--origin', default='mischa85')
    p.add_argument('--source-only', action='store_true'); p.add_argument('--headers-only', action='store_true')
    sub.add_parser('catalog'); sub.add_parser('census')
    args = parser.parse_args()
    if args.command == 'adopt':
        adopt(args.unit, args.origin, args.source_only, args.headers_only)
    elif args.command == 'init':
        init_unit(args.unit, args.source, args.options, args.start, args.end, args.auto)
    elif args.command == 'check':
        sys.exit(check(args.unit, args.asm, args.learn))
    elif args.command == 'catalog':
        # The structure gate (user, 2026-11-17): a tree over its structure budget does not land.
        import rebuild_census
        violations = rebuild_census.gate(rebuild_census.main(quiet=True))
        if violations:
            print('catalog refused by the structure gate: ' + '; '.join(violations))
            sys.exit(3)
        import rebuild_catalog
        rebuild_catalog.write()
    elif args.command == 'census':
        import rebuild_census
        rebuild_census.main()


if __name__ == '__main__':
    main()
