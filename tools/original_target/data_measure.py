#!/usr/bin/env python3
"""Data reproduced from source: the second measured line beside the code figure (user, 2026-11-25).

    data_measure.py BUILD_DIR            measure a build made with --keep-intermediates
    data_measure.py --receipt BUILD_DIR  print the measure a whole-file build recorded (data-measure.json)

The measure, named once ("data reproduced from source"): the file-backed non-code bytes of the
original image (headers, .rdata, raw .data, .rsrc up to their virtual sizes; 711,662 bytes) that a
compiled unit's object emits and that equal the original at the address the object's section is
bound to. Each data section of every compile group of the whole-file build (string literals,
float constants, tables, vtables, RTTI, initialised globals, initializer tables, zero-filled
globals inside raw .data) is laid out by the audit's own duplicate check
(verify_manifest.original_duplicate, the check the build already requires of every allocatable data
section) and the bytes it compares are taken as the proven ranges; copies are deduplicated by
address. The ??_R3 attribute bit is a recorded known difference (docs/DECISIONS.md "Class hierarchy
descriptor ambiguous bit"): counted, listed. A vtable slot naming a virtual function the tree does
not compile keeps the original entry in that check; such slots are left out of the measure.
Zero-initialised data past raw .data (bss) has no bytes to prove: its laid-out symbols are counted
on their own line, outside the file-backed total.

Data stays out of acceptance and out of Reproduced .text: this is a report, never a credit
(docs/DATA_AS_SOURCE.md). Nothing here is written into sources; outputs hold counts and addresses
only.

The by-kind split classifies every file-backed byte once (docs/DATA_AS_SOURCE.md, lane D's kinds):
headers and the debug directory, imports, resources from the PE directories; vtables (with their
locator slot) and RTTI (COL, CHD, BCA, BCD, TD) from the 770 vtables of manifests/rtti_classes.json
and the RTTI graph they point to; float constants from the `__real@` names the catalog binds;
string literals from the inventory's string types; library data from the linked library's
relocation targets (library.json) that no accepted row references; other .rdata is a constant
table, other raw .data an initialised global (runs of four or more zero bytes on their own line).
"Referenced by matched code, not yet emitted" runs from each relocation target of an accepted row
to the next known item start (at most 4 KB) over bytes no object proves.
"""
import argparse
from array import array
import json
from pathlib import Path
import struct
import sys

REPOSITORY = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(Path(__file__).resolve().parent))

KINDS = (
    ('strings', 'String literals'),
    ('floats', 'Float constants (`__real@`)'),
    ('tables', 'Other constant tables (.rdata)'),
    ('vtables', 'Vtables (with locator slot)'),
    ('rtti', 'RTTI (COL, CHD, BCA, BCD, TD)'),
    ('globals', 'Initialised globals (non-zero)'),
    ('zero_runs', 'Initialised globals (zero-valued runs in .data)'),
    ('library', 'Library data (CRT only)'),
    ('imports', 'Import (descriptors, IAT, ILT, names)'),
    ('resources', 'Resources (.rsrc)'),
    ('headers', 'Headers, debug directory'),
)
CODE = {name: index + 1 for index, (name, _) in enumerate(KINDS)}
BSS = len(KINDS) + 1
STRING_TYPES = ('string', 'TerminatedCString', 'unicode', 'TerminatedUnicode')
INVENTORY = 'private/original-target-20261020/inventory/defined-data.jsonl'
RECEIPT = 'data-measure.json'
REFERENCE_REACH = 4096


def is_data_section(section):
    return section['size'] and section['flags'] & 0xc0 and not section['flags'] & 0x20 and \
        not section['name'].startswith(('.debug', '.drectve'))


def placeholder_slots(audit, coff, index, section, candidates, address):
    """Vtable slots the audit keeps from the original (a virtual function the tree does not compile):
    a zero DIR32 to an undefined symbol no reference or RTTI binding names. [(va, 4)]."""
    from verify_manifest import prefix_bound_values
    names = [s['name'] for s in coff.symbols.values() if s['section'] == index and s['storage'] == 2]
    if not any(n.startswith('??_7') for n in names):
        return []
    references = audit.merged_references(candidates)
    layout = audit.rtti_layout(coff, candidates)
    rtti = audit.rtti_names(coff)
    out = []
    for offset, symbol_index, kind in section['relocs']:
        target = coff.symbols[symbol_index]
        name = target['name']
        if name in rtti:
            continue
        if prefix_bound_values(references, name) or audit.rtti_binding(name) is not None or name in layout:
            continue
        if kind == 6 and target['section'] == 0 and offset + 4 <= len(section['data']) and \
                not struct.unpack_from('<I', section['data'], offset)[0]:
            out.append((address + offset, 4))
    return out


def initializer_table(audit, coff, section, candidates):
    """Address of a unit's `.CRT$XC*` registrations in the original (the audit's initializer_entry
    accepts the section when the table occurs exactly once), else None."""
    table = bytearray(section['size'])
    for offset, symbol_index, kind in section['relocs']:
        name = coff.symbols[symbol_index]['name']
        entries = [c['entry'] for c in candidates.values() if name == c['symbol_prefix']] or \
            [c['entry'] for c in candidates.values() if name.startswith(c['symbol_prefix'])]
        if len(entries) != 1:
            return None
        struct.pack_into('<I', table, offset, entries[0])
    for original in audit.pe.sections:
        if original.Characteristics & 0x20:
            continue
        at = original.get_data().find(bytes(table))
        if at >= 0:
            return audit.base + original.VirtualAddress + at
    return None


def prove(audit, groups):
    """Lay every data section of every compile group as the audit's duplicate check does and record
    the original ranges it compares. groups: [(key, object path)] after audit.verify_fresh.
    Returns dict(copies=[(va, size, first symbol, section name)], placeholders=[(va, 4)],
    bss=[(va, size)], refused=[(object, section, name)])."""
    from coff import Coff
    copies, placeholders, bss, refused = [], [], [], []
    original = type(audit).original
    reads = []

    def recording(va, count):
        data = original(audit, va, count)
        reads.append((va, count))
        return data
    audit.original_vtables()
    audit.original_type_descriptors()
    for key, path in groups:
        coff = Coff(Path(path).read_bytes())
        candidates = audit.population(dict(canonical_compile_group=key))[0]
        audit.bind_data_only(coff, candidates)
        audit.rtti_layout(coff, candidates)
        for index, section in enumerate(coff.sections, 1):
            if not is_data_section(section):
                continue
            names = sorted((s['value'], s['name']) for s in coff.symbols.values()
                           if s['section'] == index and (s['storage'] == 2 or
                                                        s['storage'] == 3 and not s['name'].startswith('.')))
            first = names[0][1] if names else ''
            del reads[:]
            audit.original = recording
            try:
                ok = audit.original_duplicate(coff, index, section, candidates)
            except ValueError:
                ok = False
            finally:
                del audit.original
            if not ok:
                refused.append((str(path), index, first))
                continue
            if section['name'].startswith('.CRT$XC'):
                where = initializer_table(audit, coff, section, candidates)
                if where is None:
                    refused.append((str(path), index, first))
                else:
                    copies.append((where, section['size'], first, section['name']))
                continue
            if section['flags'] & 0x80 and not section['flags'] & 0x60:
                references = audit.merged_references(candidates)
                for position, (value, name) in enumerate(names):
                    size = (names[position + 1][0] if position + 1 < len(names) else section['size']) - value
                    if name in references and size > 0:
                        bss.append((references[name], size))
            for va, count in set(reads):
                copies.append((va, count, first, section['name']))
            if first.startswith('??_7') or any(n.startswith('??_7') for _, n in names):
                full = max(set(reads), key=lambda r: r[1], default=None)
                if full:
                    placeholders.extend(placeholder_slots(audit, coff, index, section, candidates, full[0] - (
                        4 if names and names[0][0] == 4 else 0)))
    return dict(copies=copies, placeholders=placeholders, bss=bss, refused=refused)


class Image:
    """The original image's file-backed non-code bytes and their kinds (one byte per VA)."""

    def __init__(self, pe, base):
        self.pe, self.base = pe, base
        last = max(s.VirtualAddress + s.Misc_VirtualSize for s in pe.sections)
        self.kind = bytearray(last)
        self.backed = []        # [start, end) file-backed non-code ranges (VAs)

    def mark(self, start, end, code):
        lo, hi = max(start - self.base, 0), min(end - self.base, len(self.kind))
        if hi > lo:
            self.kind[lo:hi] = bytes([code]) * (hi - lo)

    def mark_if(self, start, end, code, allowed):
        for va in range(max(start, self.base), min(end, self.base + len(self.kind))):
            if self.kind[va - self.base] in allowed:
                self.kind[va - self.base] = code

    def word(self, va):
        return struct.unpack('<I', self.pe.get_data(va - self.base, 4))[0]

    def bytes_at(self, va, count):
        return self.pe.get_data(va - self.base, count)


def classify(pe, catalog, repository=REPOSITORY):
    """Kind of every byte (Image.kind); bss bytes get BSS, code and unbacked bytes 0."""
    base = pe.OPTIONAL_HEADER.ImageBase
    image = Image(pe, base)
    image.mark(base, base + pe.OPTIONAL_HEADER.SizeOfHeaders, CODE['headers'])
    image.backed.append((base, base + pe.OPTIONAL_HEADER.SizeOfHeaders))
    data_sections = []
    for s in pe.sections:
        start = base + s.VirtualAddress
        name = s.Name.rstrip(b'\0').decode()
        if s.Characteristics & 0x20:
            continue
        backed = start + min(s.SizeOfRawData, s.Misc_VirtualSize)
        image.backed.append((start, backed))
        if name == '.rsrc':
            image.mark(start, backed, CODE['resources'])
        elif name == '.rdata':
            image.mark(start, backed, CODE['tables'])
        else:
            image.mark(start, backed, CODE['globals'])
            raw = s.get_data()[:backed - start]
            run = None
            for offset in range(len(raw) + 1):
                zero = offset < len(raw) and raw[offset] == 0
                if zero and run is None:
                    run = offset
                elif not zero and run is not None:
                    if offset - run >= 4:
                        image.mark(start + run, start + offset, CODE['zero_runs'])
                    run = None
            image.mark(backed, start + s.Misc_VirtualSize, BSS)
            data_sections.append((start, backed))
    debug = pe.OPTIONAL_HEADER.DATA_DIRECTORY[6]
    if debug.Size:
        image.mark(base + debug.VirtualAddress, base + debug.VirtualAddress + debug.Size, CODE['headers'])
    _imports(pe, image)
    # Library data: targets of the linked library members' relocations into data that no accepted row
    # references (marked first, so the specific kinds below take precedence).
    accepted_targets = set()
    for table in catalog['population_references'].values():
        accepted_targets.update(v for v in table.values() if isinstance(v, int))
    library = json.loads((repository / 'manifests/original_target/library.json').read_text())
    starts = item_starts(image, repository)
    for region in library['regions']:
        for relocation in region.get('relocations', []):
            target = relocation.get('resolved')
            if target is None or target in accepted_targets or not image.kind_at(target) in (
                    CODE['tables'], CODE['globals'], CODE['zero_runs']):
                continue
            end = next_start(starts, target, REFERENCE_REACH)
            image.mark_if(target, end, CODE['library'], (CODE['tables'], CODE['globals'], CODE['zero_runs']))
    # Strings (inventory string types), then floats, vtables and RTTI.
    path = repository / INVENTORY
    if path.exists():
        for line in path.open():
            row = json.loads(line)
            t = row['datatype']
            if t in STRING_TYPES or t.startswith(('char[', 'wchar16[')):
                a = int(row['address'], 16)
                if image.kind_at(a) not in (CODE['headers'], CODE['resources'], CODE['imports'], 0, BSS):
                    image.mark(a, a + row['length'], CODE['strings'])
    for table in catalog['population_references'].values():
        for name, value in table.items():
            if name.startswith('__real@') and isinstance(value, int):
                image.mark(value, value + (4 if name.startswith('__real@4') else 8), CODE['floats'])
    _rtti(image, repository)
    return image


def _kind_at(self, va):
    offset = va - self.base
    return self.kind[offset] if 0 <= offset < len(self.kind) else 0


Image.kind_at = _kind_at


def _imports(pe, image):
    base = image.base
    directory = pe.OPTIONAL_HEADER.DATA_DIRECTORY[1]
    iat = pe.OPTIONAL_HEADER.DATA_DIRECTORY[12]
    code = CODE['imports']
    image.mark(base + directory.VirtualAddress, base + directory.VirtualAddress + directory.Size, code)
    image.mark(base + iat.VirtualAddress, base + iat.VirtualAddress + iat.Size, code)
    if not hasattr(pe, 'DIRECTORY_ENTRY_IMPORT'):
        pe.parse_data_directories(directories=[1])
    for entry in getattr(pe, 'DIRECTORY_ENTRY_IMPORT', []):
        d = entry.struct
        count = len(entry.imports) + 1
        for rva in (d.OriginalFirstThunk, d.FirstThunk):
            if rva:
                image.mark(base + rva, base + rva + 4 * count, code)
        name = pe.get_string_at_rva(d.Name)
        image.mark(base + d.Name, base + d.Name + len(name) + 1 + ((len(name) + 1) & 1), code)
        for item in entry.imports:
            if item.name and item.hint_name_table_rva:
                size = 2 + len(item.name) + 1
                image.mark(base + item.hint_name_table_rva, base + item.hint_name_table_rva + size + (size & 1), code)


def _rtti(image, repository):
    tables = json.loads((repository / 'manifests/rtti_classes.json').read_text())['vtables']
    seen = set()

    def td(address):
        if address in seen:
            return
        seen.add(address)
        raw = image.bytes_at(address + 8, 512)
        image.mark(address, address + 8 + raw.find(b'\0') + 1, CODE['rtti'])

    def chd(address):
        if address in seen:
            return
        seen.add(address)
        image.mark(address, address + 16, CODE['rtti'])
        count, array_at = image.word(address + 8), image.word(address + 12)
        image.mark(array_at, array_at + 4 * count + 1, CODE['rtti'])
        for i in range(count):
            bcd = image.word(array_at + 4 * i)
            image.mark(bcd, bcd + 24, CODE['rtti'])
            td(image.word(bcd))
    type_info = set()
    for table in tables:
        vtable = int(table['vtable'], 16)
        image.mark(vtable - 4, vtable + 4 * len(table['slots']), CODE['vtables'])
        locator = image.word(vtable - 4)
        image.mark(locator, locator + 20, CODE['rtti'])
        td(image.word(locator + 12))
        type_info.add(image.word(image.word(locator + 12)))
        chd(image.word(locator + 16))
    # Type descriptors no vtable's graph reaches (typeid, catch clauses, throw info): every 4-aligned
    # {type_info vtable, 0, ".?A...@@"} in initialised data (verify_manifest.original_type_descriptors).
    if len(type_info) == 1:
        needle = struct.pack('<II', type_info.pop(), 0)
        for section in image.pe.sections:
            if section.Characteristics & 0x20 or not section.Characteristics & 0x40:
                continue
            data = section.get_data()[:min(section.SizeOfRawData, section.Misc_VirtualSize)]
            at = data.find(needle)
            while at >= 0:
                if not at % 4 and data[at + 8:at + 11] == b'.?A':
                    td(image.base + section.VirtualAddress + at)
                at = data.find(needle, at + 1)


def item_starts(image, repository=REPOSITORY, extra=()):
    """Sorted known item starts in data: inventory items plus extra addresses."""
    starts = set(extra)
    path = repository / INVENTORY
    if path.exists():
        for line in path.open():
            starts.add(int(json.loads(line)['address'], 16))
    return array('I', sorted(starts))


def next_start(starts, address, reach):
    from bisect import bisect_right
    i = bisect_right(starts, address)
    end = starts[i] if i < len(starts) else address + reach
    return min(end, address + reach)


def ranges_of(flags, base):
    """['start-end' hex] of the runs of non-zero bytes in flags (one byte per VA from base)."""
    out, at = [], flags.find(1)
    while at >= 0:
        end = flags.find(0, at)
        end = len(flags) if end < 0 else end
        out.append('%08x-%08x' % (base + at, base + end))
        at = flags.find(1, end)
    return out


def union(ranges):
    out = []
    for start, end in sorted(ranges):
        if out and start <= out[-1][1]:
            out[-1][1] = max(out[-1][1], end)
        else:
            out.append([start, end])
    return out


def measure(pe, catalog, proof, repository=REPOSITORY):
    """The by-kind table and the headline from a proof (prove()) and the original image."""
    image = classify(pe, catalog, repository)
    base = image.base
    backed = bytearray(len(image.kind))
    for start, end in image.backed:
        backed[start - base:end - base] = b'\1' * (end - start)
    proven = bytearray(len(image.kind))
    for start, end in union((va, va + n) for va, n, _, _ in proof['copies']):
        lo, hi = start - base, end - base
        proven[lo:hi] = bytes(backed[lo:hi])
    held_back = 0
    for va, n in proof['placeholders']:
        for offset in range(va - base, va - base + n):
            held_back += proven[offset]
            proven[offset] = 0
    # Referenced by matched code, not yet emitted.
    targets = set()
    for row in catalog['regions']:
        for relocation in row.get('relocations') or []:
            if relocation.get('kind') == 6:
                targets.add(relocation['target'])
    starts = item_starts(image, repository, [va for va, _, _, _ in proof['copies']] + list(targets))
    referenced = bytearray(len(image.kind))
    for target in targets:
        offset = target - base
        if not 0 <= offset < len(image.kind) or not (backed[offset] or image.kind[offset] == BSS):
            continue
        end = next_start(starts, target, REFERENCE_REACH) - base
        stop = proven.find(1, offset, end)
        end = end if stop < 0 else stop
        if end > offset:
            referenced[offset:end] = b'\1' * (end - offset)
    rows = {name: dict(bytes=0, proven=0, referenced=0, rest=0) for name, _ in KINDS}
    rows['bss'] = dict(bytes=0, proven=0, referenced=0, rest=0)
    names = {code: name for name, code in CODE.items()}
    names[BSS] = 'bss'
    counts = {}
    for code, p, r in zip(image.kind, proven, referenced):
        if code:
            counts[code, p, r] = counts.get((code, p, r), 0) + 1
    for (code, p, r), n in counts.items():
        row = rows[names[code]]
        row['bytes'] += n
        if p:
            row['proven'] += n
        elif r:
            row['referenced'] += n
        else:
            row['rest'] += n
    file_backed = sum(row['bytes'] for name, row in rows.items() if name != 'bss')
    reproduced = sum(row['proven'] for name, row in rows.items() if name != 'bss')
    bss_laid = sum(e - s for s, e in union((va, va + n) for va, n in proof['bss'])
                   if image.kind_at(s) == BSS)
    copies = {(va, n) for va, n, _, _ in proof['copies']}
    return dict(measure='data reproduced from source', file_backed_bytes=file_backed, reproduced_bytes=reproduced,
                reproduced_percent=round(100.0 * reproduced / file_backed, 3), kinds=rows,
                kind_labels=dict(KINDS, bss='Zero-initialised globals (bss)'),
                vtable_slots_kept_from_original=len(proof['placeholders']), vtable_slot_bytes_left_out=held_back,
                bss_laid_out_bytes=bss_laid, compared_ranges=len(copies), refused_sections=len(proof['refused']),
                refused=[dict(object=Path(o).parent.name, section=i, symbol=s) for o, i, s in proof['refused']][:50],
                # Addresses only (no bytes): the reproduced and the referenced-not-emitted ranges, for analysis.
                reproduced_ranges=ranges_of(proven, base), referenced_ranges=ranges_of(referenced, base))


def run_on_build(audit, catalog, destination, groups):
    """Measure inside a whole-file build (build.py, objects still present); write data-measure.json."""
    proof = prove(audit, groups)
    result = measure(audit.pe, catalog, proof)
    result['rtti_known_differences'] = len(getattr(audit, 'rtti_known_differences', {}) or {})
    (Path(destination) / RECEIPT).write_text(json.dumps(result, indent=1) + '\n')
    return result


def table_markdown(result):
    lines = ['| Kind | Bytes | Reproduced from source | Referenced by matched code, not yet emitted | Rest |',
             '| --- | ---: | ---: | ---: | ---: |']
    total = dict(bytes=0, proven=0, referenced=0, rest=0)
    for name, label in KINDS:
        row = result['kinds'][name]
        lines.append('| %s | %s | %s | %s | %s |' % (label, *(format(row[k], ',') for k in
                                                             ('bytes', 'proven', 'referenced', 'rest'))))
        for k in total:
            total[k] += row[k]
    lines.append('| **File-backed total** | **%s** | **%s** | **%s** | **%s** |' % tuple(
        format(total[k], ',') for k in ('bytes', 'proven', 'referenced', 'rest')))
    row = result['kinds']['bss']
    lines.append('| Zero-initialised globals (bss), not file-backed | %s | (no bytes to prove; %s laid out) | %s | %s |' % (
        format(row['bytes'], ','), format(result['bss_laid_out_bytes'], ','), format(row['referenced'], ','),
        format(row['bytes'] - row['referenced'], ',')))
    return '\n'.join(lines)


def latest_build_figure(repository=REPOSITORY):
    """(reproduced bytes, file-backed bytes, receipt time) from the newest exact whole-file build receipt under
    build/ that carries the measure (gated builds: build/r/wfo*), else None."""
    found = []
    for pattern in ('build/r/wfo*/build-receipt.json', 'build/*/build-receipt.json'):
        found.extend(Path(repository).glob(pattern))
    for path in sorted(found, key=lambda p: p.stat().st_mtime, reverse=True):
        try:
            receipt = json.loads(path.read_text())
        except (OSError, ValueError):
            continue
        if receipt.get('whole_file_exact') and 'data_reproduced_bytes' in receipt:
            import time
            when = time.strftime('%Y-%m-%dT%H:%MZ', time.gmtime(path.stat().st_mtime))
            return receipt['data_reproduced_bytes'], receipt['data_file_backed_bytes'], when
    return None


def progress_columns(repository=REPOSITORY):
    """The progress.csv columns of the series (its own columns; no break in the others): bytes and share of the
    file-backed non-code bytes, from the newest gated build's receipt; empty when no build measured it."""
    figure = latest_build_figure(repository)
    if figure is None:
        return dict(data_reproduced_bytes='', data_reproduced_share='')
    return dict(data_reproduced_bytes=figure[0], data_reproduced_share='%.3f%%' % (100.0 * figure[0] / figure[1]))


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('build', type=Path, help='a whole-file build folder')
    parser.add_argument('--receipt', action='store_true', help='print the recorded data-measure.json only')
    parser.add_argument('--config', type=Path, default=REPOSITORY / 'build/r/rebuild-config.json')
    parser.add_argument('--table', action='store_true', help='print the by-kind table as Markdown')
    args = parser.parse_args()
    if args.receipt:
        result = json.loads((args.build / RECEIPT).read_text())
    else:
        import catalog_io
        from canonical import CanonicalAudit, compile_group_key, separate_unit_populations
        catalog = catalog_io.load(REPOSITORY / 'manifests/catalog')
        config = json.loads(args.config.read_text())
        audit = CanonicalAudit(REPOSITORY, catalog, config)
        groups = []
        expected = {}
        separate = separate_unit_populations(catalog)
        for row in sorted(catalog['regions'], key=lambda r: r['source_order']):
            expected.setdefault(compile_group_key(row, separate), []).append(row)
        compiled = []
        for index, rows in enumerate(expected.values()):
            path = args.build / ('compile-%03d' % index) / 'accepted.obj'
            if not path.exists():
                raise SystemExit('%s: no compile groups (build with --keep-intermediates)' % args.build)
            compiled.append((rows, path))
            groups.append(('fresh-%03d' % index, path))
        audit.verify_fresh(args.build, compiled, jobs=1)
        result = run_on_build(audit, catalog, args.build, groups)
    if args.table:
        print(table_markdown(result))
    print('%s: %s of %s file-backed non-code bytes (%.1f%%)' % (
        result['measure'], format(result['reproduced_bytes'], ','), format(result['file_backed_bytes'], ','),
        result['reproduced_percent']))


if __name__ == '__main__':
    main()
