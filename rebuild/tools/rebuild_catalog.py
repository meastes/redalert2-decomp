"""Write the rebuild tree's canonical catalog (rebuild/manifest/catalog/) and its build config.

Only rebuilt units are catalogued; every other byte of the image stays original scaffold, as
in main's first phase. Each unit is one population `rebuild-<unit>` (one translation unit,
its own reference table). Its rows keep main's frozen evidence unchanged: bound, body and
padding hashes, padding provenance and the inventory pass, which the row names as its
`bound_population` (main's population that froze it). The build config is main's private
config plus one entry per rebuild population, pointing at the home population's inventory
(written to build/r/rebuild-config.json, never tracked).

Every row of a catalogued unit must be exact in the last `rebuild.py check` of the unit's
current source text; otherwise the unit is left out and reported.
"""
import hashlib
import json
from pathlib import Path

import rebuild as R

KEEP = ('entry', 'method', 'body_bytes', 'body_sha256', 'padding_bytes', 'full_extent_bytes',
        'full_extent_sha256', 'padding_sha256', 'boundary_confidence', 'switch_data_bytes', 'padding_provenance')


def sha(data):
    return hashlib.sha256(data).hexdigest()


def external_omitted(coff, exact_names, resolve):
    """No-duplicates COMDATs (external functions under /Gy) the object emits that are not exact rows
    but bind to an image address: the audit would omit them as copies kept elsewhere, which the review
    ruling of 2026-11-18 forbids (an external function is never a discarded duplicate). Budgeted per
    unit in structure_budget.json (external_omitted_by_unit), allowed only to fall (lane R7)."""
    out = []
    for f in coff._functions:
        name = f['name']
        if name in exact_names or name.startswith(('??_G', '??_E', '_$E')):
            continue
        records = [s for s in coff.symbols.values() if s['section'] == f['section'] and s['storage'] == 3
                   and s['type'] == 0 and len(s['auxiliary']) == 18]
        address = resolve(name)[0]
        if len(records) != 1 or records[0]['auxiliary'][14] == 2 or address is None:
            continue
        # A copy equal to the image modulo relocations is the original function compiled again (the
        # audit's original-duplicate check verifies it relocated); only a differing one is a draft.
        section = coff.sections[f['section'] - 1]
        ours = bytearray(section['data'])
        try:
            kept = bytearray(_audit().original(address, len(ours)))
        except ValueError:
            kept = bytearray()
        for offset, _, _ in section['relocs']:
            ours[offset:offset + 4] = kept[offset:offset + 4] = bytes(4)
        if ours != kept:
            out.append(name)
    return out


_AUDIT = []


def _audit():
    if not _AUDIT:
        from verify_manifest import Audit
        _AUDIT.append(Audit(R.ROOT, json.loads(R.CONFIG.read_text())['executable']))
    return _AUDIT[0]


def write():
    catalog = R.main_catalog()
    by_entry = {r['entry']: r for r in catalog['regions']}
    # A compiler-generated header row main never froze (DECISIONS row 22: "compiler-generated rows land as
    # empty rows naming header and class"; owner, lane R35): the unit row names `frozen_bound` {population},
    # a frozen pass whose inventory holds the entry with the row's body, and whose gap row after it holds
    # the padding; the region takes that pass's bound (the audit checks it against the pass again).
    for unit_info in R.load_json(R.UNITS, {}).values():
        for row in unit_info['rows']:
            frozen = row.get('frozen_bound')
            entry = int(row['entry'], 16)
            if not frozen or entry in by_entry:
                continue
            body = _audit().original(entry, row['body'])
            pad = _audit().original(entry + row['body'], row['padding']) if row['padding'] else b''
            by_entry[entry] = dict(entry=entry, method=row['entry'], body_bytes=row['body'], body_sha256=sha(body),
                                   padding_bytes=row['padding'], full_extent_bytes=row['body'] + row['padding'],
                                   full_extent_sha256=sha(body + pad), padding_sha256=sha(pad),
                                   boundary_confidence='Frozen independent analysis hypothesis (%s); compiler-generated '
                                   'header row: %s' % (frozen['population'], frozen['note']),
                                   padding_provenance=None, population=frozen['population'])
    populations = {p['id']: p for p in catalog['populations']}
    units = R.load_json(R.UNITS, {})
    config = json.loads(R.CONFIG.read_text())
    regions, pops, tables, needed, refused = [], [], {}, set(), []
    omitted_total = [0]
    for unit, info in sorted(units.items(), key=lambda kv: kv[1]['range'][0]):
        last = R.load_json(R.WORK / 'last' / (unit + '.json'), None)
        source = R.ROOT / info['source']
        if last is None or last['source_sha256'] != sha(source.read_bytes()):
            refused.append('%s: no check of the current text' % unit); continue
        # A unit the whole-file build refused for this exact text stays out until its text changes.
        held = info.get('build_refused')
        if held and held.get('source_sha256') == last['source_sha256']:
            refused.append('%s: whole-file build refused: %s' % (unit, held['reason'][:100])); continue
        # A unit with inline assembly carries main's evidenced-asm flag and TibSun citations on the rows
        # main flagged (main's rule, user 2026-11-07); the whole-file build's audit checks each block
        # against the cited archive lines and refuses a block no flagged row accounts for (lane R4).
        from canonical import inline_asm_blocks
        has_asm = bool(inline_asm_blocks(source.read_text(errors='replace')))
        import guard, re as _re
        source_text = source.read_text(errors='replace')
        def defined_here(sym):
            name = guard.demangle(sym)
            return bool(name) and bool(_re.search(r'^(?![ \t#/])[^\n;{}]*?(?<![\w:])%s\s*\(' % _re.escape(name), source_text, _re.M))
        results = {r['entry']: r for r in last['results']}
        exact = [row for row in info['rows'] if results.get(row['entry'], {}).get('status') == 'exact']
        # A row frozen in one of main's closed compiler-output populations (main's A14 array-construction
        # policy) is claimed by the unit that emits it exact (review session, 2026-11-20: closed populations
        # and discard policies are main-only). Its bound evidence stays that population's inventory; the
        # policy itself never enters the rebuild catalog, and units.json records the row's origin
        # (`claimed_from`) so the populations to retire are known.
        if not exact:
            refused.append('%s: no exact row' % unit); continue
        # Every row of mischa85's module range (the unit's own functions) must be exact unless the
        # unit guards its drafts (partial_ok); rows outside it (initializers, compiler-emitted
        # copies the object may or may not hold) count when exact.
        core = info.get('core')
        inside = [r for r in info['rows'] if core and core[0] <= r['entry'] < core[1]] if core else info['rows']
        missed = [r for r in inside if results.get(r['entry'], {}).get('status') != 'exact']
        if missed and not info.get('partial_ok'):
            refused.append('%s: %d of %d own rows not exact' % (unit, len(missed), len(inside))); continue
        # Every initializer the object registers (.CRT$XCU) must be an exact row: the audit checks
        # the whole table against the original.
        from coff import Coff
        coff = Coff(Path(last['object']).read_bytes())
        registered = {coff.symbols[x]['name'] for s in coff.sections if s['name'].startswith('.CRT$XC')
                      for o, x, k in s['relocs']}
        exact_names = {r['symbol'] for r in exact}
        if registered - exact_names:
            refused.append('%s: initializers not exact rows: %s' % (unit, ' '.join(sorted(registered - exact_names))[:80]))
            continue
        # Every other function the object emits must bind to an original address (a copy kept
        # elsewhere); an unbound one would be unaccounted code.
        resolve_u = R.resolver(unit, info)
        loose = [s['name'] for s in coff._functions if s['name'] not in exact_names and
                 not s['name'].startswith(('??_G', '??_E')) and resolve_u(s['name'])[0] is None]
        # Data the object defines (.bss, .data) must bind by name or by the data binder. A symbol the
        # unit declares unplaced-unreferenced (owner decision per use; DECISIONS, user 2026-11-18) has no
        # address: the audit checks its conditions (data_bindings.unplaced).
        # A symbol declared layout (DECISIONS, "Rebuild data no exact code refers to", A) is bound by the
        # audit from its position (data_bindings.bind, run-anchored where a parked function's data is missing).
        unplaced = {n for n, k in (info.get('data_bound_by') or {}).items() if k in ('unplaced-unreferenced', 'layout')}
        for i, s in enumerate(coff.sections, 1):
            if s['flags'] & 0xc0 and not s['flags'] & 0x1020 and not s['name'].startswith(('.CRT', '.debug', '.drectve')):
                loose += [n['name'] for n in coff.symbols.values() if n['section'] == i and n['storage'] in (2, 3)
                          and not n['name'].startswith(('.', '$')) and resolve_u(n['name'])[0] is None
                          and n['name'] not in unplaced]
        # Floating constants (COMDAT .rdata __real@) must bind too: the build refuses an unbound one.
        loose += [n['name'] for n in coff.symbols.values() if n['name'].startswith('__real@')
                  and n['section'] > 0 and resolve_u(n['name'])[0] is None and n['name'] not in
                  {b['symbol'] for r in exact for b in results[r['entry']].get('relocations', [])}]
        # A draft compiled into the object (a row of this unit that is not exact) has no place in the
        # image: the build refuses its section. Drafts go under RA2_NON_MATCHING (guard.py).
        own_rows = {int(r['entry'], 16) for r in info['rows']}
        import guard, re as _re
        source_text = source.read_text(errors='replace')
        def defined_here(sym):
            name = guard.demangle(sym)
            return bool(name) and bool(_re.search(r'^(?![ \t#/])[^\n;{}]*?(?<![\w:])%s\s*\(' % _re.escape(name), source_text, _re.M))
        exact_entries = {int(r['entry'], 16) for r in exact}
        drafts = [s['name'] for s in coff._functions if s['name'] not in exact_names and
                  not s['name'].startswith(('_$', '??_')) and '?$' not in s['name'] and
                  resolve_u(s['name'])[0] in own_rows - exact_entries and defined_here(s['name'])]
        if drafts:
            refused.append('%s: unguarded drafts: %s' % (unit, ' '.join(drafts)[:100]))
            continue
        if loose:
            refused.append('%s: emitted functions with no row or binding: %s' % (unit, ' '.join(loose)[:100]))
            continue
        budget = json.loads((R.MANIFEST / 'structure_budget.json').read_text())
        claimed_names = exact_names | {results[r['entry']].get('symbol') for r in exact}
        omitted = [n for n in external_omitted(coff, claimed_names, resolve_u)
                   if resolve_u(n)[0] not in exact_entries]
        if len(omitted) > budget.get('external_omitted_by_unit', {}).get(unit, 0):
            refused.append('%s: external functions compiled but not exact rows (the audit would omit them as kept '
                           'copies; guard them): %s' % (unit, ' '.join(omitted)[:100]))
            continue
        omitted_total[0] += len(omitted)
        # The structure gate per unit (user, 2026-11-17): stray externs and address-suffixed names
        # within the unit's budget (rebuild/manifest/structure_budget.json; zero for a new unit).
        import rebuild_census
        faults = rebuild_census.unit_faults(unit, source.read_text(errors='replace'))
        if faults:
            refused.append('%s: structure: %s' % (unit, '; '.join(faults)))
            continue
        pid = 'rebuild-' + unit.replace('_', '-')
        homes = {}
        # A row padded from the aligned split of its own pass is bound there (rebuild.aligned_gap_after).
        bound_of = lambda row, main: row.get('gap_population') or main.get('bound_population', main['population'])
        for row in exact:
            main = by_entry[int(row['entry'], 16)]
            own = bound_of(row, main)
            homes[own] = homes.get(own, 0) + row['body']
        home = max(homes, key=homes.get)
        pops.append(dict(id=pid, inventory_sha256=populations[home]['inventory_sha256'],
                         gaps_sha256=populations[home]['gaps_sha256'],
                         historical_population_sha256=populations[home]['historical_population_sha256'],
                         historical_acceptance_sha256=populations[home]['historical_acceptance_sha256']))
        config['populations'][pid] = dict(config['populations'][home])
        table = tables.setdefault(pid, {})
        resolve = R.resolver(unit, info)
        for row in exact:
            main = by_entry[int(row['entry'], 16)]
            result = results[row['entry']]
            own = bound_of(row, main)
            region = {k: main[k] for k in KEEP if k in main}
            if has_asm and main.get('inline_asm') and defined_here(result['symbol']):     # not a header template
                region.update(inline_asm=main['inline_asm'], inline_asm_citation=main['inline_asm_citation'])
            if row.get('gap_padding'):
                # Padding from the gap row of the row's own frozen pass (rebuild.gap_after): the
                # extent grows by that gap; bound and body stay main's (lane R3).
                audit = _audit()
                pad = audit.original(main['entry'] + main['body_bytes'], row['gap_padding'])
                full = audit.original(main['entry'], main['body_bytes'] + row['gap_padding'])
                region.update(padding_bytes=len(pad), full_extent_bytes=len(full), padding_sha256=sha(pad),
                              full_extent_sha256=sha(full))
            region.update({k: info[k] for k in ('data_bound_by', 'data_bound_by_note', 'data_bound_by_rows', 'data_pending', 'data_bound_by_group') if info.get(k)})
            region.update(symbol=result['symbol'], symbol_prefix=result['symbol'], options=info['options'],
                          relocations=result['relocations'], source_sha256=last['source_sha256'],
                          source=info['source'], population=pid, source_order=main['entry'],
                          source_quality_family='rebuild')
            if own != home:
                region['bound_population'] = own
                needed.add(own)
            for binding in result['relocations']:
                if binding['symbol'] != '$local':
                    table[binding['symbol']] = binding['target']
            regions.append(region)
        # Every other name the object defines or references (kept-elsewhere copies, vtables and
        # their slots, literals) that resolves gets its binding; the audit decides what it accepts.
        from coff import Coff
        coff = Coff(Path(last['object']).read_bytes())
        for symbol in coff.symbols.values():
            name = symbol['name']
            if name.startswith(('.', '$L', '$SG')) or symbol['storage'] not in (2, 3) or name in table:
                continue
            address, label = resolve(name)
            if address is not None:
                table[name] = address
        needed.add(home)
    for own in sorted(needed - {p['id'] for p in pops}):
        pops.append({k: v for k, v in populations[own].items() if k != 'compiler_output_policy'})
    headers = [dict(path=p.relative_to(R.ROOT).as_posix(), sha256=sha(p.read_bytes()))
               for p in sorted((R.REBUILD / 'include').rglob('*')) if p.is_file()]
    for region in regions:
        region['references'] = tables[region['population']]
    out = dict(schema=catalog['schema'], target=catalog['target'],
               baseline_manifest_sha256=catalog['baseline_manifest_sha256'],
               populations=pops, population_references={}, regions=sorted(regions, key=lambda r: r['entry']),
               headers=headers, source_quality_checkpoint=catalog.get('source_quality_checkpoint'))
    import catalog_io
    target = R.MANIFEST / 'catalog'
    catalog_io.dump(out, target)
    # Units with no catalogued row stay pending (canonical.rebuild_pending_sources).
    catalogued = {r['source'] for r in regions}
    for unit, info in units.items():
        info['status'] = 'catalogued' if info['source'] in catalogued else 'pending'
    R.write_json(R.UNITS, units)
    R.write_json(R.WORK / 'rebuild-config.json', config)
    print('catalog: %d units, %d rows, %d body bytes; %d populations (%d bound-only); external functions omitted %d' % (
        len(catalogued), len(regions), sum(r['body_bytes'] for r in regions), len(pops),
        len(pops) - len(catalogued), omitted_total[0]))
    for line in refused:
        print('  left out: ' + line)
