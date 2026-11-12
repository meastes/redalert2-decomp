"""Re-bind compiler-numbered static initializers (`_$E<n>`) by content and position.

VC6 names a unit's dynamic initializers and their atexit destructors `_$E<n>`,
where n comes from a per-unit counter that every declaration the unit sees
advances (an anonymous enum in a header, a static data member). An honest
header change therefore renumbers them without changing a byte: the
catalog's `_$E41` (symbol, symbol_prefix, reference table key, relocation
receipt, `// @ref` comment) then names another function of the fresh object,
or none.

The number is never an identity. `group_mapping` finds, for every `_$E` name
the catalog uses for one compiled unit, the fresh function it now is:

1. content: a fresh `_$E` function whose bytes outside its relocation fields
   equal the original bytes at the row's entry (or at the address the
   reference table binds the name to), whose relocations have the receipt's
   offsets, kinds and addends, and whose other targets carry the receipt's
   names. Exactly one candidate binds the name; a relocation between two
   initializers (an initializer pushing its atexit destructor) then binds
   the target pair too, so an initializer and its destructor move together,
   and so does a static-local guard flag (`_$S<n>`) the destructor tests;
   Names content binds ambiguously (identical functions, e.g. empty
   initializers) pair off in numbering order between their bound neighbours
   when the unbound names and the matching candidates there are equally many;
2. position: a name content cannot bind (its bytes really differ) takes the
   shift of the nearest content-bound name before it in numbering order
   (after it when there is none; only names bound with a relocation receipt
   are anchors), so the audit compares it with the function
   in its source position and reports the real byte difference.

Nothing here accepts anything: the strict audit compares the re-bound
functions with the original bytes exactly as before. A wrong binding can
only refuse (a function compared at the wrong address differs), never hide
a difference, because every binding the audit relies on is checked there.
"""
import re
import struct

INITIALIZER = re.compile(r'_\$E(\d+)')
# The same counter numbers a function's static-local guard flags (`_$S<n>`,
# data the initializer's atexit destructor tests): they move with it.
# A guard flag of a function's own local static carries the function's scope
# in its mangling (`_?$S51@?4??Motion_Capture@@YIXXZ@4EA`); the number moves,
# the scope stays.
COUNTED = re.compile(r'_\$([ES])(\d+)|_\?\$S(\d+)(@.+)')
LOCAL = '$local'
# `// @ref _$E41=006B4530` in accepted source (diff_candidate.SOURCE_REF form).
SOURCE_INITIALIZER_REF = re.compile(r'(@ref\s+)([^\s=]+)(=)')


def is_initializer(name):
    return bool(name) and INITIALIZER.fullmatch(name) is not None


def counted_form(name):
    """'E', 'S' or 'S' plus the scope for a counter-numbered name; else None."""
    match = COUNTED.fullmatch(name) if name else None
    if match is None:
        return None
    return match[1] or 'S' + match[4]


def is_counted(name):
    return counted_form(name) is not None


def number(name):
    return int(INITIALIZER.fullmatch(name).group(1))


def uses_initializers(rows, references):
    """True when a unit's rows or its population's table name any `_$E`."""
    return any(is_counted(n) for n in references) or any(
        is_initializer(row.get('symbol_prefix')) or is_initializer(row.get('symbol')) or
        any(is_counted(r.get('symbol')) for r in row.get('relocations') or []) for row in rows)


def fresh_initializers(coff):
    """{name: (bytes, [(offset, kind, addend, target name), ...])} of every
    `_$E` function the object defines, over its compiler extent."""
    result = {}
    names = sorted({s['name'] for s in coff.symbols.values()
                    if s['section'] > 0 and s['type'] & 0x20 and is_initializer(s['name'])})
    for name in names:
        extent = function_extent(coff, name)
        if extent is not None:
            result[name] = extent
    return result


def function_extent(coff, prefix):
    """(bytes, sorted relocations) of the function `prefix` selects, or None."""
    try:
        _, section, start, end = coff.function(prefix)
    except ValueError:
        return None
    data = bytes(section['data'][start:end])
    relocs = []
    for offset, index, kind in section['relocs']:
        if start <= offset < end:
            local = offset - start
            addend = struct.unpack_from('<I', data, local)[0] if local + 4 <= len(data) else None
            relocs.append((local, kind, addend, coff.symbols[index]['name']))
    return data, sorted(relocs)


def _masked_equal(data, relocs, original):
    if original is None or len(original) < len(data):
        return False
    fields = set()
    for offset, *_ in relocs:
        fields.update(range(offset, offset + 4))
    return all(data[i] == original[i] for i in range(len(data)) if i not in fields)


def _receipt_pairs(receipt, relocs):
    """Initializer target pairs (old, fresh) the receipt implies for this
    fresh function, or None when the relocation structure differs."""
    if receipt is None:
        return {}
    old = sorted((r['offset'], r['kind'], r.get('addend'), r['symbol']) for r in receipt)
    if [(o, k) for o, k, _, _ in old] != [(o, k) for o, k, _, _ in relocs]:
        return None
    pairs = {}
    for (_, _, addend, name), (_, _, fresh_addend, fresh_name) in zip(old, relocs):
        if addend is not None and addend != fresh_addend:
            return None
        if is_counted(name) or is_counted(fresh_name):
            if counted_form(name) != counted_form(fresh_name):
                return None
            if pairs.setdefault(name, fresh_name) != fresh_name:
                return None
        elif name != LOCAL and name != fresh_name:
            return None
    return pairs


def group_mapping(coff, rows, references, original):
    """{old name: fresh name} for the `_$E` names one compiled unit's rows use
    (identity entries omitted). `rows` are the unit's catalog rows,
    `references` the population's binding table, original(address, length)
    reads the original executable (None or short when unreadable)."""
    used = set()
    contexts = {}       # old name -> (address, length or None, receipt or None)
    for row in rows:
        for key in ('symbol', 'symbol_prefix'):
            if is_initializer(row.get(key)):
                used.add(row[key])
        for r in row.get('relocations') or []:
            if is_initializer(r.get('symbol')):
                used.add(r['symbol'])
        if is_initializer(row.get('symbol_prefix')):
            # The fresh COMDAT extent runs to the next function: body and padding.
            contexts[row['symbol_prefix']] = (row['entry'], row.get('full_extent_bytes', row['body_bytes']),
                                              row.get('relocations'))
    bare = set()
    for name, address in (references or {}).items():
        if is_initializer(name):
            if name not in used:
                bare.add(name)
            used.add(name)
            contexts.setdefault(name, (address, None, None))
    if not used:
        return {}
    fresh = fresh_initializers(coff)
    guards = {s['name'] for s in coff.symbols.values() if is_counted(s['name']) and not is_initializer(s['name'])}
    mapping = {}

    def assign(old, new):
        """Bind old -> new with its implied pairs; False on a conflict."""
        pending, added = [(old, new)], {}
        while pending:
            o, n = pending.pop()
            current = mapping.get(o, added.get(o))
            if current is not None:
                if current != n:
                    return False
                continue
            if n in mapping.values() or n in added.values() or n not in (fresh if is_initializer(n) else guards):
                return False
            added[o] = n
            if o in contexts:
                pairs = _receipt_pairs(contexts[o][2], fresh[n][1])
                if pairs is None:
                    return False
                pending.extend(pairs.items())
        mapping.update(added)
        return True

    def matches(old, new):
        address, length, receipt = contexts[old]
        data, relocs = fresh[new]
        if length is not None and len(data) != length:
            return False
        if _receipt_pairs(receipt, relocs) is None:
            return False
        try:
            there = original(address, len(data))
        except ValueError:
            return False
        return _masked_equal(data, relocs, there)

    # 0. Rows with a stable name whose receipt names counted symbols (a function
    #    registering its local static's atexit destructor, testing its guard
    #    flag): the fresh function's relocation in the same place names the
    #    symbol now. Only the relocation structure is used; the audit compares
    #    the bytes.
    for row in rows:
        receipt = row.get('relocations') or []
        if is_initializer(row.get('symbol_prefix')) or not any(is_counted(r['symbol']) for r in receipt):
            continue
        extent = function_extent(coff, row['symbol_prefix'])
        pairs = _receipt_pairs(receipt, extent[1]) if extent is not None else None
        for old, new in sorted((pairs or {}).items()):
            assign(old, new)
    # 1. Content, rows (with receipts) before bare references, until no name binds.
    def bind_by_content(names):
        progress = True
        while progress:
            progress = False
            for old in names:
                if old in mapping:
                    continue
                taken = set(mapping.values())
                candidates = [n for n in fresh if n not in taken and matches(old, n)]
                if len(candidates) > 1 and old in candidates:
                    candidates = [old]     # an unchanged unit keeps its numbers
                if len(candidates) == 1 and assign(old, candidates[0]):
                    progress = True
    named = sorted((n for n in contexts if n not in bare), key=number)

    def bind_in_order():
        """Names content binds ambiguously (identical functions): between two
        bound neighbours, the unbound names and the unbound candidates that
        match them pair off in numbering order when they are equally many."""
        progress = False
        anchors = sorted((number(o), number(n)) for o, n in mapping.items() if is_initializer(o) and o in named)
        bounds = [(-1, -1)] + anchors + [(float('inf'), float('inf'))]
        taken = set(mapping.values())
        for (lo_old, lo_new), (hi_old, hi_new) in zip(bounds, bounds[1:]):
            olds = [o for o in named if o not in mapping and lo_old < number(o) < hi_old]
            if not olds:
                continue
            news = sorted((n for n in fresh if n not in taken and lo_new < number(n) < hi_new and
                           any(matches(o, n) for o in olds)), key=number)
            if len(news) == len(olds) and all(matches(o, n) for o, n in zip(olds, news)):
                for o, n in zip(olds, news):
                    progress = assign(o, n) or progress
        return progress

    bind_by_content(named)
    while bind_in_order():
        bind_by_content(named)
    bind_by_content(sorted(bare, key=number))
    # 2. Position, for the names the rows themselves use that content left
    #    unbound (real differences): the nearest anchor's shift. A bare
    #    reference-table name binds by content only (it may belong to another
    #    unit of the population, and an unbound one only refuses).
    anchors = sorted((number(o), number(n) - number(o)) for o, n in mapping.items()
                     if is_initializer(o) and o in contexts and contexts[o][2] is not None)
    for old in sorted(used - set(mapping) - bare, key=number):
        if not anchors:
            break
        k = number(old)
        before = [d for a, d in anchors if a < k]
        shift = before[-1] if before else anchors[0][1]
        new = '_$E%d' % (k + shift)
        if new in fresh and new not in mapping.values():
            mapping[old] = new
    return {o: n for o, n in mapping.items() if o != n}


def merge_population_mappings(mappings):
    """One mapping per population from its units' mappings; None when two
    units of the population bind one name differently (the population's one
    reference table cannot hold both)."""
    merged = {}
    for mapping in mappings:
        for old, new in mapping.items():
            if merged.setdefault(old, new) != new:
                return None
    if len(set(merged.values())) != len(merged):
        return None
    return merged


def rename(name, mapping):
    return mapping.get(name, name) if is_counted(name) else name


def rebind_rows(rows, mapping):
    """Apply `mapping` to a population's rows in place: symbol, symbol_prefix,
    relocation receipts, and the population's one reference table (rebuilt
    once, shared by every row as the catalog holds it)."""
    if not mapping or not rows:
        return
    targets = set(mapping.values())
    tables = {}
    for row in rows:
        table = row.get('references') or {}
        if id(table) not in tables:
            new = {}
            for name, value in table.items():
                if is_counted(name) and name not in mapping and name in targets:
                    continue    # stale: the fresh name now denotes another bound initializer
                key = rename(name, mapping)
                if new.setdefault(key, value) != value:
                    raise ValueError('re-bound %s would bind two addresses' % key)
            tables[id(table)] = new
        row['references'] = tables[id(table)]
        for key in ('symbol', 'symbol_prefix'):
            row[key] = rename(row[key], mapping)
        for r in row.get('relocations') or []:
            r['symbol'] = rename(r['symbol'], mapping)


def rebind_source_refs(text, mapping):
    """`// @ref _$E<n>=ADDRESS` comments renamed by `mapping` (all at once)."""
    return SOURCE_INITIALIZER_REF.sub(lambda m: m[1] + mapping.get(m[2], m[2]) + m[3], text)
