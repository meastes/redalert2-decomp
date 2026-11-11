#!/usr/bin/env python3
"""Module ledger: prove a module's compiler-emitted data and write its address map.

    module_ledger.py BUILD_DIR [--write] [--config CONFIG]

A module is one tracked source in the form of an original translation unit
(several accepted regions, no addresses in the source). `manifests/original_target/
modules.json` declares each one:

  {"version": {"source": "src/recovered/version.cpp", "header": "src/shared/version.hpp",
               "data": [["007f9700", "007f9978"]]}}

`data` are the original's address ranges the module's data occupies. From a
build made with --keep-intermediates this tool takes the module's fresh
object, lays every initialized data section it emits (tables, statics,
string literals, vtables, RTTI) at the original address the population's
reference table (or the audit's RTTI binding) gives its first symbol,
resolves its relocations to original addresses and compares the bytes. The
declared ranges must be tiled completely by such sections, with only zero
alignment fill between them. Sections whose original copy lies outside the
ranges (a vtable the original linker kept from another object) are listed as
kept elsewhere. Nothing is credited to .text; this is the data ledger line.

--write records the result as `manifests/original_target/modules/<name>.json`:
the module's functions (symbol, entry, size), its data (symbol, address,
size, section, kind), the references into it from other units and its
references out, so the source itself carries no addresses.
"""
import argparse
import json
from pathlib import Path
import struct
import sys

REPOSITORY = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(Path(__file__).resolve().parent))

from coff import Coff
import catalog_io

MODULES = REPOSITORY / 'manifests/original_target/modules.json'
MAPS = REPOSITORY / 'manifests/original_target/modules'
DIR32 = 6
RTTI_PREFIXES = ('??_R0', '??_R1', '??_R2', '??_R3', '??_R4')


def load_modules(path=MODULES):
    if not path.exists():
        return {}
    modules = json.loads(path.read_text())
    for name, item in modules.items():
        item['ranges'] = [(int(a, 16), int(b, 16)) for a, b in item['data']]
    return modules


def section_kind(names):
    name = names[0]
    if name.startswith('??_7'):
        return 'vtable'
    if name.startswith(RTTI_PREFIXES):
        return 'rtti'
    if name.startswith('??_C@'):
        return 'string'
    if '?1??' in name or name.startswith('_?'):
        return 'static'
    return 'table'


def data_sections(coff):
    """(index, section, [(value, name)]) of every initialized, non-code data section."""
    out = []
    for index, section in enumerate(coff.sections, 1):
        flags = section['flags']
        if not section['size'] or flags & 0x20 or not flags & 0x40 or section['name'].startswith('.debug'):
            continue
        names = sorted((s['value'], s['name']) for s in coff.symbols.values()
                       if s['section'] == index and (s['storage'] == 2 or
                                                     s['storage'] == 3 and not s['name'].startswith('.')))
        out.append((index, section, names))
    return out


def prove_data(coff, bindings, original, module_code):
    """Lay every data section at its original address and compare.

    bindings: name -> original address (reference table plus RTTI copies);
    original(va, n) -> bytes; module_code: name -> entry of the module's
    own functions (a vtable slot or table pointer may name one).
    Returns ([copy], [failure]); a copy is a dict with address, bytes, names, kind."""
    def address_of(name):
        if name in bindings:
            return bindings[name]
        return module_code.get(name)
    copies, failures = [], []
    for index, section, names in data_sections(coff):
        if not names:
            failures.append(dict(section=index, reason='no symbol names the section'))
            continue
        value, first = names[0]
        locator = section['relocs'] and names[0][0] == 4 and first.startswith('??_7')
        base = address_of(first)
        if base is None:
            failures.append(dict(section=index, symbol=first, reason='no original address bound'))
            continue
        base -= value
        data = bytearray(section['data'])
        unbound = None
        for offset, symbol_index, kind in section['relocs']:
            target = coff.symbols[symbol_index]['name']
            where = address_of(target)
            if kind != DIR32 or where is None or offset + 4 > len(data):
                unbound = target; break
            addend = struct.unpack_from('<I', data, offset)[0]
            struct.pack_into('<I', data, offset, (where + addend) & 0xffffffff)
        if unbound is not None:
            failures.append(dict(section=index, symbol=first, reason='relocation target unbound: ' + unbound))
            continue
        try:
            same = bytes(data) == original(base, len(data))
        except ValueError:
            same = False
        record = dict(address=base, bytes=len(data), names=[n for _, n in names], kind=section_kind([first]),
                      section=index, locator=bool(locator))
        (copies if same else failures).append(record if same else dict(record, reason='bytes differ from the original'))
    return copies, failures


def coverage(copies, ranges, original):
    """Tile each declared range with the proven copies: only zero fill of fewer than four
    bytes may separate them. Returns (per-range results, copies outside every range)."""
    inside = []
    results = []
    used = set()
    for start, end in ranges:
        rows = sorted((c for c in copies if start <= c['address'] < end), key=lambda c: c['address'])
        cursor, gaps, proven = start, [], 0
        for c in rows:
            used.add(id(c))
            if c['address'] < cursor:
                gaps.append(dict(at='%08x' % c['address'], reason='overlaps the previous copy'))
            elif c['address'] - cursor >= 4 or any(original(cursor, c['address'] - cursor)):
                gaps.append(dict(at='%08x' % cursor, bytes=c['address'] - cursor, reason='not compiler-emitted'))
            proven += c['bytes']
            cursor = c['address'] + c['bytes']
        tail = end - cursor
        if tail >= 4 or tail < 0 or (tail > 0 and any(original(cursor, tail))):
            gaps.append(dict(at='%08x' % cursor, bytes=tail, reason='not compiler-emitted'))
        results.append(dict(start='%08x' % start, end='%08x' % end, bytes=end - start, proven_bytes=proven,
                            alignment_bytes=end - start - proven if not gaps else None, sections=len(rows),
                            complete=not gaps, gaps=gaps))
        inside.extend(rows)
    outside = [c for c in copies if id(c) not in used]
    return results, outside


def module_rows(catalog, source):
    return sorted((r for r in catalog['regions'] if r['source'] == source), key=lambda r: r['entry'])


def references_into(catalog, rows, ranges):
    """Relocations of other units whose target lies in the module's code or data."""
    code = {r['entry'] for r in rows}
    spans = [(r['entry'], r['entry'] + r['full_extent_bytes']) for r in rows] + list(ranges)
    hits = []
    for region in catalog['regions']:
        if region['source'] == rows[0]['source']:
            continue
        for relocation in region.get('relocations') or []:
            t = relocation['target']
            if any(a <= t < b for a, b in spans):
                hits.append(dict(site='%08x' % region['entry'], symbol=relocation['symbol'],
                                 target='%08x' % t, function_entry=t in code))
    return hits


def locate_object(build_dir, source):
    receipt = json.loads((build_dir / 'compile-receipt.json').read_text())
    for group in receipt['groups']:
        if source in group['sources']:
            return build_dir / ('compile-%03d' % group['group']) / 'accepted.obj', group
    raise SystemExit('%s: no compile group holds %s (build with --keep-intermediates)' % (build_dir, source))


def ledger(build_dir, catalog, original, modules):
    verification = json.loads((build_dir / 'fresh-verification.json').read_text())
    rtti = {n: int(a, 16) for n, a in verification.get('rtti_original_copies', {}).items()}
    out = {}
    for name, item in modules.items():
        rows = module_rows(catalog, item['source'])
        if not rows:
            raise SystemExit('%s: no accepted region names %s' % (name, item['source']))
        obj_path, group = locate_object(build_dir, item['source'])
        coff = Coff(obj_path.read_bytes())
        bindings = dict(rows[0]['references'])
        bindings.update(rtti)
        module_code = {r['symbol']: r['entry'] for r in rows}
        copies, failures = prove_data(coff, bindings, original, module_code)
        ranges, outside = coverage(copies, item['ranges'], original)
        undefined = sorted(s['name'] for s in coff.symbols.values() if s['section'] == 0 and s['storage'] == 2
                           and s['name'] not in module_code)
        out[name] = dict(
            module=name, source=item['source'], header=item.get('header'), population=rows[0]['population'],
            options=rows[0]['options'], compile_group=group['group'],
            code=[dict(symbol=r['symbol'], entry='%08x' % r['entry'], body_bytes=r['body_bytes'],
                       padding_bytes=r['padding_bytes']) for r in rows],
            code_bytes=sum(r['full_extent_bytes'] for r in rows),
            data=[dict(symbol=c['names'][0], address='%08x' % c['address'], bytes=c['bytes'],
                       kind=c['kind'], section=c['section']) for c in sorted(copies, key=lambda c: c['address'])],
            data_ranges=ranges, data_bytes_proven=sum(r['proven_bytes'] for r in ranges),
            data_complete=all(r['complete'] for r in ranges),
            kept_elsewhere=[dict(symbol=c['names'][0], address='%08x' % c['address'], bytes=c['bytes'], kind=c['kind'])
                            for c in sorted(outside, key=lambda c: c['address'])],
            failures=failures,
            references_in=references_into(catalog, rows, item['ranges']),
            references_out=[dict(symbol=n, address='%08x' % bindings[n] if n in bindings else None) for n in undefined])
    return out


def summary_line(entry):
    kept = entry['kept_elsewhere']
    return ('%s: %d functions, %d code bytes; data %s: %d of %d bytes compiler-emitted in %d sections (%s); '
            '%d original copies kept elsewhere (%d bytes: %s); %d references in, %d out%s' % (
                entry['module'], len(entry['code']), entry['code_bytes'],
                ', '.join('%s-%s' % (r['start'], r['end']) for r in entry['data_ranges']),
                entry['data_bytes_proven'], sum(r['bytes'] for r in entry['data_ranges']), len(entry['data']),
                'complete' if entry['data_complete'] else 'INCOMPLETE',
                len(kept), sum(c['bytes'] for c in kept), ', '.join(sorted({c['kind'] for c in kept})) or '-',
                len(entry['references_in']), len(entry['references_out']),
                '' if not entry['failures'] else '; %d sections REFUSED' % len(entry['failures'])))


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('build_dir', type=Path)
    parser.add_argument('--write', action='store_true', help='write manifests/original_target/modules/<name>.json')
    parser.add_argument('--config', type=Path, default=REPOSITORY / 'private/original-target.local.json')
    args = parser.parse_args()
    import pefile
    config = json.loads(args.config.read_text())
    pe = pefile.PE(config['executable'], fast_load=True)
    base = pe.OPTIONAL_HEADER.ImageBase

    def original(va, count):
        data = pe.get_data(va - base, count)
        if len(data) != count:
            raise ValueError('outside the image')
        return data
    catalog = catalog_io.load(catalog_io.locate(REPOSITORY), references=True)
    modules = load_modules()
    if not modules:
        raise SystemExit('no modules declared in %s' % MODULES)
    result = ledger(args.build_dir.resolve(), catalog, original, modules)
    failed = False
    for name, entry in result.items():
        print(summary_line(entry))
        failed |= bool(entry['failures']) or not entry['data_complete']
        if args.write:
            MAPS.mkdir(parents=True, exist_ok=True)
            (MAPS / (name + '.json')).write_text(json.dumps(entry, indent=1) + '\n')
    if failed:
        raise SystemExit(1)


if __name__ == '__main__':
    main()
