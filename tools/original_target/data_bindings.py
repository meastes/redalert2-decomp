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
            Run-anchored fallback, rebuild tree only (docs/DECISIONS.md, "Rebuild data no exact code
            refers to", A; review session 2026-11-19), used only when the whole
            section does not bind as above (its bound symbols disagree on one base,
            because a parked function's data is missing from the object): the
            nearest reference-bound symbols of the same section below and above the
            symbol (in the object's order) agree on one base; no pending data range
            (B below) lies between the lower anchor's start and the upper anchor's
            end; every symbol between the anchors is reference-bound or itself
            declared layout; and no reference-bound address or independently known
            item starts inside the symbol's derived extent. The items between the
            anchors then fill the distance exactly; the caller compares the bytes.

Pending data ranges (the same ruling, B; row field `data_pending`: [{address, size, owner}],
            opt-in per source, the same on every row): data the original holds only
            because a parked function emits it (a function-local static, a literal).
            Never defined in source; the bytes stay original scaffolding, earn
            nothing, and are counted. A range closes when its owner lands: the audit
            refuses a range whose owner is an accepted row of the source, and one
            that overlaps a symbol the object binds.

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

unplaced-unreferenced (docs/DECISIONS.md, "A zero-initialised .bss object whose existence is
            evidenced by exact code rows but whose address nothing references"; user decision
            2026-11-18, opt-in per source, each use an owner decision) The symbol stays
            without an address. It must be defined in a zero-filled (.bss) section of the
            object; no relocation of the object may target it; the rows that evidence it
            (row field `data_bound_by_rows`: {symbol: [entry, ...]}) must be accepted rows of
            the same source; and at every audit the unit's unclaimed, unreferenced bss gaps
            (holes in the span of its reference-bound bss symbols, alignment fill excluded,
            that no 4-byte value of the original image points into) must hold all such
            symbols of the object jointly, one symbol per gap. It earns nothing and compares
            no byte of the image.

Header select-any groups (docs/DECISIONS.md, "Unreferenced zero-initialised select-any COMDAT .bss
            objects emitted by a system or library header"; review session 2026-11-19; opt-in per source,
            row field `data_bound_by_group`: [{symbols: [name, ...], header, rows: [entry, ...]}], each
            symbol also declared `layout`): unreferenced zero-filled select-any objects a header emits
            bind as one group to an exact hole. Each symbol is the only symbol of a COMDAT .bss section
            of the object, no relocation of the object targets it, and its name is not written in the
            source; the evidencing rows (the header's use) are accepted rows of the source. The hole is
            a gap between the object's reference-bound .bss items (alignment fill below the first
            symbol's alignment allowed) that no 4-byte value of the image points into, and the group laid
            out in the object's order, each symbol at its own alignment, fills it exactly; exactly one
            such hole exists in the unit's .bss range. The addresses are our object's emission order (a
            hypothesis: the linker orders separate COMDATs); zero bytes, nothing credited.

content    (docs/DECISIONS.md, "Rebuild data forms", review session 2026-11-20, user-approved
            2026-11-20; rebuild tree only, opt-in per source) A table of pointers no code
            references, located by its bytes. The symbol is the only one of its initialised
            (never .bss) section; the section has relocations and every 4-byte slot of it is
            a DIR32 relocation target (an array of pointers; a table with any other field
            refuses); each relocation targets a bound symbol or a string literal of the
            object (a relocation-free COMDAT ??_C@ section of its own). Each literal is
            bound by its text where the image holds it: at an aligned occurrence that begins
            a literal (preceded by a zero byte or the start of its section), the only such
            occurrence or, when the text occurs more than once, the one occurrence adjacent
            (within alignment zero fill) to an already bound literal of the run. The
            object's bytes relocated against those addresses must occur exactly once
            (aligned) in the image's initialised non-code sections; the extent is enclosed
            as for layout; no reference-bound address, independently known item or 4-byte
            image value points into it. For this kind the extent is our object's section
            size: every byte of that extent is compared by equality with the image at the
            one place it occurs, and the enclosure checks that nothing else lies inside or
            between it and its independent neighbours. Only the declaration (name, element
            type, one array) stays a hypothesis. Zero .text credit by itself.

Extents are never taken from the candidate's bytes (content: the extent is the object's own
section size, but every byte of it is compared and its place is the image's one occurrence):
the anchors are independent and every byte between them is compared.
"""
LAYOUT, ELIMINATION, UNPLACED, CONTENT = 'layout', 'elimination', 'unplaced-unreferenced', 'content'
KINDS = (LAYOUT, ELIMINATION, UNPLACED, CONTENT)


class BindingRefused(ValueError):
    pass


def pending(rows):
    """[(address, size, owner)] of the one `data_pending` declaration of a source's rows."""
    found = None
    for row in rows:
        value = row.get('data_pending')
        if value is None:
            continue
        if not isinstance(value, list) or not value:
            raise BindingRefused('data_pending must be a non-empty list of {address, size, owner}')
        ranges = []
        for item in value:
            if not isinstance(item, dict) or set(item) != {'address', 'size', 'owner'} or \
                    not isinstance(item['size'], int) or item['size'] <= 0:
                raise BindingRefused('data_pending entries are {address, size, owner} with a positive size')
            ranges.append((int(item['address'], 16), item['size'], int(item['owner'], 16)))
        if found is not None and found != ranges:
            raise BindingRefused('data_pending differs between rows')
        found = ranges
    return found or []


def check_pending(ranges, accepted_entries, bound):
    """Refuse a pending range whose owner landed or that overlaps a bound symbol.

    bound: [(address, extent)] of the object's symbols bound to image addresses."""
    for address, size, owner in ranges:
        if owner in accepted_entries:
            raise BindingRefused('pending data %08x+%d: its owner %08x is an accepted row (close the range)'
                                 % (address, size, owner))
        for start, extent in bound:
            if start < address + size and address < start + extent:
                raise BindingRefused('pending data %08x+%d overlaps a bound symbol at %08x' % (address, size, start))


def run_anchored(coff, index, name, references, pending_ranges, layout_names, starts):
    """(address, detail) of a layout symbol bound by its nearest bound neighbours, or BindingRefused."""
    section = coff.sections[index - 1]
    symbols = section_symbols(coff, index)
    extents = {n: (symbols[i + 1][0] if i + 1 < len(symbols) else section['size']) - v
               for i, (v, n) in enumerate(symbols)}
    position = next(i for i, (v, n) in enumerate(symbols) if n == name)
    below = next((i for i in range(position - 1, -1, -1) if symbols[i][1] in references), None)
    above = next((i for i in range(position + 1, len(symbols)) if symbols[i][1] in references), None)
    if below is None or above is None:
        raise BindingRefused('%s: no bound symbol of its section on %s side' % (name, 'the lower' if below is None else 'the upper'))
    (low_value, low_name), (high_value, high_name) = symbols[below], symbols[above]
    base = references[low_name] - low_value
    if references[high_name] - high_value != base:
        raise BindingRefused('%s: its anchors %s and %s disagree on the base' % (name, low_name, high_name))
    loose = [n for v, n in symbols[below + 1:above] if n not in references and n not in layout_names]
    if loose:
        raise BindingRefused('%s: unbound symbols between its anchors: %s' % (name, loose[:3]))
    run = (references[low_name], references[high_name] + extents[high_name])
    for address, size, owner in pending_ranges:
        if address < run[1] and run[0] < address + size:
            raise BindingRefused('%s: pending data %08x+%d lies between its anchors' % (name, address, size))
    address = base + symbols[position][0]
    end = address + extents[name]
    claims = {a for n, a in references.items() if n != name} | set(starts)
    if any(address <= a < end for a in claims):
        raise BindingRefused('%s: an independently known item starts inside %08x-%08x' % (name, address, end))
    return address, dict(section=index, base=base, size=section['size'], run_anchors=(low_name, high_name),
                         run=run)


def declared(rows):
    """The one `data_bound_by` declaration of a source's rows ({} when none)."""
    found = {}
    for row in rows:
        value = row.get('data_bound_by')
        if value is None:
            continue
        if not isinstance(value, dict) or not value or any(v not in KINDS for v in value.values()):
            raise BindingRefused('data_bound_by must map symbols to "layout", "elimination", "%s" or "%s"' % (UNPLACED, CONTENT))
        note = row.get('data_bound_by_note')
        if not isinstance(note, str) or not note.strip():
            raise BindingRefused('data_bound_by needs a non-empty data_bound_by_note')
        evidence = row.get('data_bound_by_rows') or {}
        for name, kind in value.items():
            if found.get(name, kind) != kind:
                raise BindingRefused('data_bound_by differs between rows for %s' % name)
            if (kind == UNPLACED) != bool(evidence.get(name)):
                raise BindingRefused('%s: data_bound_by_rows names the evidencing rows of an %s symbol, and only those'
                                     % (name, UNPLACED))
            found[name] = kind
        if set(evidence) - set(value):
            raise BindingRefused('data_bound_by_rows names undeclared symbols')
    return found


def evidence_rows(rows):
    """{symbol: [entry, ...]} of the `data_bound_by_rows` declaration of a source's rows."""
    for row in rows:
        if row.get('data_bound_by_rows'):
            return {name: [int(e, 16) if isinstance(e, str) else e for e in entries]
                    for name, entries in row['data_bound_by_rows'].items()}
    return {}


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


def groups(rows):
    """The one `data_bound_by_group` declaration of a source's rows ([] when none)."""
    found = None
    for row in rows:
        value = row.get('data_bound_by_group')
        if value is None:
            continue
        if not isinstance(value, list) or not value or any(
                not isinstance(g, dict) or set(g) != {'symbols', 'header', 'rows'} or not g['symbols'] or
                not isinstance(g['header'], str) or not g['header'].strip() or not g['rows'] for g in value):
            raise BindingRefused('data_bound_by_group entries are {symbols, header, rows}, all non-empty')
        if found is not None and found != value:
            raise BindingRefused('data_bound_by_group differs between rows')
        found = value
    return found or []


def group_hole(coff, group, references, pointed, accepted_entries, source_text=''):
    """{symbol: (address, detail)} for one header select-any group, or BindingRefused."""
    names = group['symbols']
    rows = [int(e, 16) if isinstance(e, str) else e for e in group['rows']]
    missing = [e for e in rows if e not in accepted_entries]
    if missing:
        raise BindingRefused('%s: evidencing rows not accepted rows of the source: %s' %
                             (names[0], ' '.join('%08x' % e for e in missing)))
    targeted = {coff.symbols[x]['name'] for s in coff.sections for _, x, _ in s['relocs']}
    layout = []
    for name in names:
        if name in references:
            raise BindingRefused('%s: declared in a group but already bound by a reference' % name)
        if name in targeted:
            raise BindingRefused('%s: a relocation of the object targets it' % name)
        plain = name.lstrip('?_').split('@')[0]
        if plain and source_text and plain in source_text:
            raise BindingRefused('%s: its name is written in the source (a group is header-emitted only)' % name)
        index = defining_section(coff, name)
        section = coff.sections[index - 1]
        if not section['flags'] & 0x80 or section['flags'] & 0x60 or not section['flags'] & 0x1000:
            raise BindingRefused('%s: not a select-any (COMDAT) zero-filled section' % name)
        if [n for _, n in section_symbols(coff, index)] != [name]:
            raise BindingRefused('%s: not the only symbol of its section' % name)
        layout.append((name, section['size'], alignment(section['flags'])))
    items = []
    for index, section in enumerate(coff.sections, 1):
        if not section['flags'] & 0x80 or section['flags'] & 0x60:
            continue
        symbols = section_symbols(coff, index)
        for i, (value, name) in enumerate(symbols):
            if name in references:
                extent = (symbols[i + 1][0] if i + 1 < len(symbols) else section['size']) - value
                items.append((references[name], max(extent, 1)))
    if not items:
        raise BindingRefused('%s: no .bss symbol of the object is bound' % names[0])
    items.sort()
    low, high = items[0][0], max(a + e for a, e in items)
    hits = pointed(low, high)
    holes, cursor = [], None
    for (address, extent), following in zip(items, [a for a, _ in items[1:]] + [None]):
        end = address + extent if following is None else min(address + extent, following)
        if following is not None and following > end:
            holes.append((end, following))

    def fit(start, stop):
        out, cursor = [], start
        for name, size, align in layout:
            cursor = (cursor + align - 1) // align * align
            out.append((name, cursor))
            cursor += size
        return out if out[0][1] - start < layout[0][2] and cursor == stop else None
    found = [(a, b, fit(a, b)) for a, b in holes if not any(a <= p < b for p in hits)]
    found = [f for f in found if f[2]]
    if len(found) != 1:
        raise BindingRefused('%s: %d exact unreferenced holes for the group in the unit\'s .bss %08x-%08x'
                             % (' '.join(names), len(found), low, high))
    start, stop, placed = found[0]
    return {name: (address, dict(group=names, hole=('%08x' % start, '%08x' % stop), header=group['header'],
                                 order='hypothesis: our object\'s emission order'))
            for name, address in placed}


def zero_gap(original, low, high, align):
    """Whether [low, high) is alignment fill: shorter than the alignment and all zero."""
    return 0 <= high - low < max(align, 1) and not (high > low and any(original(low, high - low)))


def content(coff, name, references, find, original, below_ends, above_starts, pointed):
    """{symbol: (address, detail)} for one content-bound object and the literals it points to,
    or BindingRefused (see the module text, "content")."""
    import struct
    import tree
    if not tree.REBUILD:
        raise BindingRefused('%s: content binding is the rebuild tree ruling only' % name)
    if find is None or pointed is None:
        raise BindingRefused('%s: content binding needs the image search' % name)
    if name in references:
        raise BindingRefused('%s: declared content but already bound by a reference' % name)
    index = defining_section(coff, name)
    section = coff.sections[index - 1]
    if section['flags'] & 0xA0 or not section['flags'] & 0x40:
        raise BindingRefused('%s: not in an initialised data section' % name)
    symbols = section_symbols(coff, index)
    if [n for _, n in symbols] != [name] or symbols[0][0] != 0:
        raise BindingRefused('%s: not the only symbol of its section' % name)
    extent = section['size']
    align = alignment(section['flags'])
    # Pointer tables only (review session 2026-11-20): relocations required, and every 4-byte slot of
    # the extent is one.
    slots = sorted(offset for offset, _, _ in section['relocs'])
    if not slots:
        raise BindingRefused('%s: no relocation (content binding is for pointer tables)' % name)
    if extent % 4 or slots != list(range(0, extent, 4)):
        raise BindingRefused('%s: not an array of pointers (every 4-byte slot a relocation)' % name)
    table = dict(references)
    literals, ambiguous = {}, {}
    for offset, target, kind in section['relocs']:
        if kind != 6:
            raise BindingRefused('%s: relocation kind %d at +%d' % (name, kind, offset))
        symbol = coff.symbols[target]['name']
        if symbol in table or symbol in literals or symbol in ambiguous:
            continue
        if not symbol.startswith('??_C@'):
            raise BindingRefused('%s: relocation +%d targets %s, neither bound nor a literal' % (name, offset, symbol))
        lit_index = defining_section(coff, symbol)
        lit = coff.sections[lit_index - 1]
        if not lit['flags'] & 0x1000 or lit['flags'] & 0xA0 or lit['relocs'] or \
                [n for _, n in section_symbols(coff, lit_index)] != [symbol]:
            raise BindingRefused('%s: literal %s is not a COMDAT section of its own' % (name, symbol))
        text = bytes(lit['data'])
        step = alignment(lit['flags'])
        hits = [a for a in find(text, literal=True) if a % step == 0]    # each hit begins a literal
        if not hits:
            raise BindingRefused('%s: literal %s not in the image' % (name, symbol))
        if len(hits) == 1:
            literals[symbol] = (hits[0], len(text))
        else:
            ambiguous[symbol] = (hits, len(text), step)
    bound = dict(literals)
    while ambiguous:
        progress = False
        for symbol, (hits, size, step) in list(ambiguous.items()):
            near = [a for a in hits if any(zero_gap(original, b + n, a, step) or zero_gap(original, a + size, b, step)
                                           for b, n in bound.values())]
            if len(near) == 1:
                bound[symbol] = (near[0], size)
                del ambiguous[symbol]
                progress = True
            elif len(near) > 1:
                raise BindingRefused('%s: literal %s adjacent to the run at %d places' % (name, symbol, len(near)))
        if not progress:
            raise BindingRefused('%s: literals neither unique nor adjacent to the run: %s' % (name, sorted(ambiguous)[:3]))
    table.update({s: a for s, (a, _) in bound.items()})
    data = bytearray(section['data'])
    for offset, target, kind in section['relocs']:
        add = struct.unpack_from('<I', data, offset)[0]
        struct.pack_into('<I', data, offset, (table[coff.symbols[target]['name']] + add) & 0xffffffff)
    hits = [a for a in find(bytes(data)) if a % align == 0]
    if len(hits) != 1:
        raise BindingRefused('%s: its relocated bytes occur %d times in the image' % (name, len(hits)))
    address = hits[0]
    end = address + extent
    starts = set(above_starts) | {a for a, _ in bound.values()}
    low, high = enclosed(address, extent, align, below_ends, starts, original)
    if any(address <= a < end for a in set(references.values()) | set(above_starts)):
        raise BindingRefused('%s: an independently known item starts inside %08x-%08x' % (name, address, end))
    if pointed(address, end):
        raise BindingRefused('%s: a 4-byte value of the image points into %08x-%08x' % (name, address, end))
    for symbol, (a, size) in bound.items():
        if any(a <= x < a + size for n, x in references.items() if n != symbol):
            raise BindingRefused('%s: literal %s at %08x overlaps a bound item' % (name, symbol, a))
    out = {name: (address, dict(section=index, extent=extent, lower_anchor=low, upper_anchor=high,
                                literals=len(bound)))}
    out.update({s: (a, dict(literal_of=name, extent=size)) for s, (a, size) in bound.items()})
    return out


def bind(coff, declared_bindings, references, original, below_ends, above_starts, pending_ranges=(), grouped=(),
         find=None, pointed=None):
    """{symbol: (address, kind, detail)} for the declared symbols, or BindingRefused.

    references: name -> original address (the population's table, RTTI layout included);
    below_ends / above_starts: end and start addresses of independently known items
    (other units' proven copies, RTTI structures, reference-bound items)."""
    out = {}
    layout = [n for n, k in declared_bindings.items() if k == LAYOUT and n not in grouped]
    elimination = [n for n, k in declared_bindings.items() if k == ELIMINATION]
    for name in [n for n, k in declared_bindings.items() if k == CONTENT]:
        for symbol, (address, detail) in content(coff, name, references, find, original, below_ends,
                                                 above_starts, pointed).items():
            out[symbol] = (address, CONTENT, detail)
    for name in layout:
        if name in references:
            raise BindingRefused('%s: declared layout but already bound by a reference' % name)
        index = defining_section(coff, name)
        section = coff.sections[index - 1]
        try:
            base, symbols = layout_base(coff, index, references)
            low, high = enclosed(base, section['size'], alignment(section['flags']), below_ends, above_starts, original)
        except BindingRefused as whole:
            import tree
            if not tree.REBUILD:
                raise   # the run-anchored fallback is the rebuild's ruling A (DECISIONS); main's tree keeps whole-section binding only
            try:
                address, detail = run_anchored(coff, index, name, references, pending_ranges, set(layout),
                                               above_starts)
            except BindingRefused as local:
                raise BindingRefused('%s (whole section: %s)' % (local, whole))
            out[name] = (address, LAYOUT, detail)
            continue
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


def unplaced(coff, names, references, evidence, accepted_entries, referenced):
    """{symbol: detail} for the declared unplaced-unreferenced symbols, or BindingRefused.

    evidence: symbol -> [entry, ...] (data_bound_by_rows); accepted_entries: entries of the
    source's accepted rows; referenced(low, high) -> the set of addresses in [low, high) that a
    4-byte value of the original image equals."""
    out = {}
    if not names:
        return out
    targeted = {coff.symbols[x]['name'] for s in coff.sections for _, x, _ in s['relocs']}
    gaps_of = {}
    for name in names:
        if name in references:
            raise BindingRefused('%s: declared %s but bound by a reference' % (name, UNPLACED))
        if name in targeted:
            raise BindingRefused('%s: a relocation of the object targets it' % name)
        rows = evidence.get(name) or []
        missing = [e for e in rows if e not in accepted_entries]
        if not rows or missing:
            raise BindingRefused('%s: evidencing rows not accepted rows of the source: %s' %
                                 (name, ' '.join('%08x' % e for e in missing) or 'none named'))
        index = defining_section(coff, name)
        section = coff.sections[index - 1]
        if not section['flags'] & 0x80 or section['flags'] & 0x60 or section['relocs']:
            raise BindingRefused('%s: not in a zero-filled (.bss) section' % name)
        symbols = section_symbols(coff, index)
        extents = {n: (symbols[i + 1][0] if i + 1 < len(symbols) else section['size']) - v
                   for i, (v, n) in enumerate(symbols)}
        if index not in gaps_of:
            bound = sorted((references[n], n) for v, n in symbols if n in references)
            if not bound:
                raise BindingRefused('%s: no symbol of its bss section is bound' % name)
            holes = []
            for (address, n), (following, _) in zip(bound, bound[1:] + [(None, None)]):
                end = address + extents[n] if following is None else min(address + extents[n], following)
                if following is not None and following > end:
                    holes.append((end, following))
            low, high = bound[0][0], bound[-1][0] + extents[bound[-1][1]]
            pointed = referenced(low, high)
            free = [(a, b) for a, b in holes if not (b - a < 8 and b % 8 == 0 and a % 8)
                    and not any(a <= p < b for p in pointed)]
            gaps_of[index] = (free, (low, high))
        out[name] = dict(section=index, extent=extents[name], rows=['%08x' % e for e in rows])
    for index, (free, span) in gaps_of.items():
        mine = sorted((d['extent'], n) for n, d in out.items() if d['section'] == index)[::-1]
        sizes = sorted((b - a for a, b in free), reverse=True)
        if len(sizes) < len(mine) or any(size < need for size, (need, _) in zip(sizes, mine)):
            raise BindingRefused('%s: the unit\'s %d unclaimed, unreferenced bss gaps in %08x-%08x cannot hold '
                                 'them jointly' % (' '.join(n for _, n in mine), len(free), span[0], span[1]))
        for _, n in mine:
            out[n].update(span=['%08x' % span[0], '%08x' % span[1]],
                          gaps=['%08x+%d' % (a, b - a) for a, b in free])
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

