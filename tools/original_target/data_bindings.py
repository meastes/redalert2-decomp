"""Data-only symbols bound by section layout or by bss elimination (docs/DECISIONS.md
"Data-only symbols bound by section layout / bss by elimination").

A unit's initialised data section can hold objects no code references (a table of
pointers to arrays: only the table is named by code). Such a symbol has no binding of
its own. Two narrow rules give it one; both are declared per source (the row field
`data_bound_by`: {symbol: "layout" | "elimination"} with `data_bound_by_note`, set on
every row of the source) and both refuse unless every condition holds:

layout      The symbol lies in an initialised, non-code section of the object that
            also defines a symbol the population's references bind (the anchor
            inside); every reference-bound symbol of the section agrees on one
            section base. The section is enclosed: below it the nearest
            independently proven item (another unit's proven copy, an RTTI
            structure or a reference-bound item of known extent) ends at most
            alignment-1 zero bytes before the base; above it an independently
            known item (a reference-bound address, an RTTI structure or another
            unit's proven copy) starts at most alignment-1 zero bytes after the
            section end. The section itself is compared byte for byte by the
            caller (the audit's relocated_data_duplicate, the ledger's
            prove_data). An open edge refuses.

elimination The symbol is the only symbol of a zero-filled (.bss) section of the
            object that the references do not bind; every other symbol of the
            section is bound. The unit's bss span runs from the lowest bound
            symbol to the first reference-bound address at or above the end of
            the highest one; the bound symbols (extent: to the next symbol of the
            object's section, alignment fill under 8 bytes excluded) leave exactly
            one free hole in that span; its length equals the symbol's extent;
            and the object's layout puts the symbol at the same end of its
            section as the hole lies in the span (both last, or both first).
            It earns nothing by itself (zero bytes); it only lets a pointer to
            the symbol resolve.

Extents are never taken from the candidate's bytes: the anchors are independent and
every byte between them is compared.
"""
LAYOUT, ELIMINATION = 'layout', 'elimination'
KINDS = (LAYOUT, ELIMINATION)


class BindingRefused(ValueError):
    pass


def declared(rows):
    """The one `data_bound_by` declaration of a source's rows ({} when none)."""
    found = {}
    for row in rows:
        value = row.get('data_bound_by')
        if value is None:
            continue
        if not isinstance(value, dict) or not value or any(v not in KINDS for v in value.values()):
            raise BindingRefused('data_bound_by must map symbols to "layout" or "elimination"')
        note = row.get('data_bound_by_note')
        if not isinstance(note, str) or not note.strip():
            raise BindingRefused('data_bound_by needs a non-empty data_bound_by_note')
        for name, kind in value.items():
            if found.get(name, kind) != kind:
                raise BindingRefused('data_bound_by differs between rows for %s' % name)
            found[name] = kind
    return found


def alignment(flags):
    code = (flags >> 20) & 0xF
    return 1 << (code - 1) if code else 1


def section_symbols(coff, index):
    return sorted((s['value'], s['name']) for s in coff.symbols.values()
                  if s['section'] == index and (s['storage'] == 2 or
                                                s['storage'] == 3 and not s['name'].startswith('.')))


def defining_section(coff, name):
    hits = [s for s in coff.symbols.values() if s['name'] == name and s['section'] > 0]
    if len(hits) != 1:
        raise BindingRefused('%s: not defined exactly once in the object' % name)
    return hits[0]['section']


def layout_base(coff, index, references):
    section = coff.sections[index - 1]
    if section['flags'] & 0x20 or not section['flags'] & 0x40:
        raise BindingRefused('section %d is not initialised data' % index)
    symbols = section_symbols(coff, index)
    bases = {references[n] - v for v, n in symbols if n in references}
    if not bases:
        raise BindingRefused('section %d: no symbol of the section is bound by a reference' % index)
    if len(bases) != 1:
        raise BindingRefused('section %d: bound symbols disagree on the section base' % index)
    return bases.pop(), symbols


def enclosed(base, size, align, below_ends, above_starts, original):
    """Both edges of [base, base+size) meet independent items with only zero fill."""
    lower = [e for e in below_ends if e <= base]
    if not lower:
        raise BindingRefused('open lower edge at %08x' % base)
    gap = base - max(lower)
    if gap >= max(align, 4) or gap and any(original(max(lower), gap)):
        raise BindingRefused('lower edge at %08x: %d bytes before the section are not zero fill' % (base, gap))
    end = base + size
    upper = [s for s in above_starts if s >= end]
    if not upper:
        raise BindingRefused('open upper edge at %08x' % end)
    gap = min(upper) - end
    if gap >= 8 or gap and any(original(end, gap)):
        raise BindingRefused('upper edge at %08x: %d bytes after the section are not zero fill' % (end, gap))
    return max(lower), min(upper)


def bind(coff, declared_bindings, references, original, below_ends, above_starts):
    """{symbol: (address, kind, detail)} for the declared symbols, or BindingRefused.

    references: name -> original address (the population's table, RTTI layout included);
    below_ends / above_starts: end and start addresses of independently known items
    (other units' proven copies, RTTI structures, reference-bound items)."""
    out = {}
    layout = [n for n, k in declared_bindings.items() if k == LAYOUT]
    elimination = [n for n, k in declared_bindings.items() if k == ELIMINATION]
    for name in layout:
        if name in references:
            raise BindingRefused('%s: declared layout but already bound by a reference' % name)
        index = defining_section(coff, name)
        base, symbols = layout_base(coff, index, references)
        section = coff.sections[index - 1]
        low, high = enclosed(base, section['size'], alignment(section['flags']), below_ends, above_starts, original)
        value = next(v for v, n in symbols if n == name)
        out[name] = (base + value, LAYOUT, dict(section=index, base=base, size=section['size'],
                                                 lower_anchor=low, upper_anchor=high))
    if elimination:
        table = dict(references)
        table.update({n: a for n, (a, _, _) in out.items()})
        for name in elimination:
            if name in table:
                raise BindingRefused('%s: declared elimination but already bound' % name)
            index = defining_section(coff, name)
            section = coff.sections[index - 1]
            if not section['flags'] & 0x80 or section['flags'] & 0x60:
                raise BindingRefused('%s: elimination needs a zero-filled (.bss) section' % name)
            symbols = section_symbols(coff, index)
            extents = {n: (symbols[i + 1][0] if i + 1 < len(symbols) else section['size']) - v
                       for i, (v, n) in enumerate(symbols)}
            unbound = [n for v, n in symbols if n not in table]
            if unbound != [name]:
                raise BindingRefused('%s: other unbound symbols in its bss section: %s' %
                                     (name, [n for n in unbound if n != name][:4]))
            bound = sorted((table[n], table[n] + extents[n], n) for v, n in symbols if n != name)
            low, top = bound[0][0], max(e for _, e, _ in bound)
            later = [a for a in references.values() if a >= top]
            if not later:
                raise BindingRefused('%s: no reference-bound address closes the unit bss span' % name)
            high = min(later)
            holes, cursor = [], low
            for start, end, n in bound:
                if start < cursor:
                    raise BindingRefused('%s: bound bss items overlap at %08x (%s)' % (name, start, n))
                if start > cursor:
                    holes.append((cursor, start))
                cursor = end
            if high > cursor:
                holes.append((cursor, high))
            free = [(a, b) for a, b in holes if not (b - a < 8 and b % 8 == 0 and a % 8)]
            if len(free) != 1:
                raise BindingRefused('%s: %d free holes in the unit bss span %08x-%08x' % (name, len(free), low, high))
            start, end = free[0]
            if end - start != extents[name] or start % 4:
                raise BindingRefused('%s: hole %08x-%08x does not fit its extent %d' % (name, start, end, extents[name]))
            last_in_object = symbols[-1][1] == name
            first_in_object = symbols[0][1] == name
            if not (last_in_object and end == high or first_in_object and start == low):
                raise BindingRefused('%s: the object does not put it where the hole lies' % name)
            out[name] = (start, ELIMINATION, dict(section=index, span=(low, high), hole=(start, end)))
    return out


def independent_items(references, rtti, other_copies, extent_of):
    """(below_ends, above_starts): ends and starts of independently known items.

    other_copies: [(address, bytes)] proven copies of other units (module maps);
    rtti: name -> address of RTTI structures (bound through the original's RTTI graph);
    extent_of(name) -> bytes of a reference-bound item of this object, or None."""
    ends, starts = set(), set()
    for address, size, *_ in other_copies:
        ends.add(address + size); starts.add(address)
    for address in rtti.values():
        starts.add(address)
    for name, address in references.items():
        starts.add(address)
        size = extent_of(name)
        if size:
            ends.add(address + size)
    return ends, starts


def other_proven_copies(maps_dir, source):
    """[(address, bytes, source)] of every proven data copy in the module maps of sources other
    than `source` (all of them when source is None)."""
    import json
    out = []
    if not maps_dir.exists():
        return out
    for path in sorted(maps_dir.glob('*.json')):
        entry = json.loads(path.read_text())
        if entry.get('source') == source:
            continue
        for copy in entry.get('data', []) + entry.get('kept_elsewhere', []):
            out.append((int(copy['address'], 16), copy['bytes'], entry.get('source')))
    return out

