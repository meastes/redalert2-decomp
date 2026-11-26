"""Generated data modules (docs/DECISIONS.md "fastmath tables as a generated data module").

manifests/generated_data.json lists each module (the tree's own; the earlier tree's
manifests/original_target/generated_data.json stays frozen): a tracked generator that writes
MASM source computing every entry from its formula (no original bytes and no carried values in
the repository), the original address range it must reproduce, its symbols and inner points,
its flagged constants (every constant read from the image rather than derived) and its
citation. At build time each generator writes its source under the build folder; the pinned
MASM assembles it (under the compiler lock); the object must hold exactly one non-empty
section, initialised data, without relocations; its public symbols must be exactly the
manifest's, each bound by an accepted reference, all of them to one section base equal to the
range start; each inner point (symbol plus offset) must be reached by an accepted relocation
that resolves there; the section length must equal the range, and its bytes must equal the
original's: the whole block with every name bound, or nothing. The proven bytes then replace
the scaffold's copy of the range. Nothing is credited to .text or to acceptance; the data
measure reports the module on its own "with generated modules" sub-line (review-session
ruling 2026-11-26, lane O104).
"""
import hashlib
import importlib
import json
from pathlib import Path

REPOSITORY = Path(__file__).resolve().parents[2]
MANIFEST = REPOSITORY / 'manifests/generated_data.json'


def require(condition, message):
    if not condition:
        raise SystemExit('generated data module: ' + message)


def load():
    if not MANIFEST.exists():
        return None
    doc = json.loads(MANIFEST.read_text())
    require(doc.get('schema') == 'original-target-generated-data-v1', 'unsupported manifest schema')
    for module in doc['modules']:
        require((REPOSITORY / module['generator']).is_file(), 'missing generator %s' % module['generator'])
        start, end = (int(x, 16) for x in module['range'])
        require(end > start, 'empty range for %s' % module['name'])
        require(module.get('symbols'), 'module %s names no symbols' % module['name'])
        for constant in module.get('flagged_constants', []):
            require(constant.get('name') and constant.get('note'), 'flagged constant needs a name and a note')
    return doc


def inputs(doc):
    return [MANIFEST, *[REPOSITORY / m['generator'] for m in doc['modules']]] if doc else []


def generate(module, folder):
    """Write the module's source; returns its path."""
    path = Path(module['generator'])
    import sys
    sys.path.insert(0, str((REPOSITORY / path).parent))
    generator = importlib.import_module(path.stem)
    folder.mkdir(parents=True, exist_ok=True)
    source = folder / (module['name'] + '.asm')
    with open(source, 'w', newline='') as handle:
        handle.write(generator.module_text())
    for constant in module.get('flagged_constants', []):
        require(getattr(generator, constant['name'], None) == int(constant['value'], 16),
                'flagged constant %s differs from the generator' % constant['name'])
    # Every constant the generator reads from the image is listed in the manifest (and so on the STATUS sub-line).
    require(sorted(getattr(generator, 'FLAGGED_CONSTANTS', ())) == sorted(c['name'] for c in module.get('flagged_constants', [])),
            '%s: the generator\'s FLAGGED_CONSTANTS and the manifest differ' % module['name'])
    return source


def prove(module, coff, references, original, inner=None):
    """(start, bytes) after checking the assembled object against the original. INNER: {(symbol, offset): bound}
    for the manifest's inner points (whether an accepted relocation resolves there)."""
    start, end = (int(x, 16) for x in module['range'])
    sections = [(i, s) for i, s in enumerate(coff.sections, 1) if s['size'] and not s['name'].startswith(('.debug', '.drectve'))]
    require(len(sections) == 1, '%s: expected one non-empty section, found %d' % (module['name'], len(sections)))
    index, section = sections[0]
    require(section['flags'] & 0x40 and not section['flags'] & 0xa0, '%s: section is not initialised data' % module['name'])
    require(not section['relocs'], '%s: data with relocations' % module['name'])
    publics = [s for s in coff.symbols.values() if s['section'] == index and s['storage'] == 2]
    require(publics, '%s: no public symbols' % module['name'])
    require(sorted(s['name'] for s in publics) == sorted(module['symbols']),
            '%s: public symbols %s, the manifest names %s' % (module['name'], sorted(s['name'] for s in publics),
                                                             sorted(module['symbols'])))
    bases = set()
    for symbol in publics:
        require(symbol['name'] in references, '%s: %s is not bound by an accepted reference' % (module['name'], symbol['name']))
        bases.add(references[symbol['name']] - symbol['value'])
    require(bases == {start}, '%s: bound symbols place the section at %s, not %08x' % (
        module['name'], sorted('%08x' % b for b in bases), start))
    for point in module.get('inner_points', []):
        require((inner or {}).get((point['symbol'], point['offset'])),
                '%s: inner point %s (%s+%d) is not bound by an accepted relocation' % (
                    module['name'], point['name'], point['symbol'], point['offset']))
    data = bytes(section['data'])
    require(len(data) == end - start, '%s: section is %d bytes, the range %d' % (module['name'], len(data), end - start))
    require(data == original(start, len(data)), '%s: bytes differ from the original' % module['name'])
    return start, data


def place(config, doc, accepted, original, destination, scaffold, pe):
    """Generate, assemble, prove and place every module; returns the receipts."""
    if not doc:
        return []
    import assembly_tools
    from coff import Coff
    references, resolved = {}, {}
    for row in accepted:
        for name, address in (row.get('references') or {}).items():
            references.setdefault(name, set()).add(address)
        for relocation in row.get('relocations') or []:
            if relocation.get('kind') == 6 and 'resolved' in relocation:
                resolved.setdefault(relocation['symbol'], set()).add(relocation['resolved'])
    receipts = []
    base = pe.OPTIONAL_HEADER.ImageBase
    for module in doc['modules']:
        folder = Path(destination) / 'generated'
        source = generate(module, folder)
        relative = source.resolve().relative_to(REPOSITORY) if source.resolve().is_relative_to(REPOSITORY) else source.resolve()
        objects = assembly_tools.assemble(config, [str(relative)], folder / 'assemble')
        coff = Coff(Path(objects[str(relative)]).read_bytes())
        names = {s['name'] for s in coff.symbols.values() if s['storage'] == 2}
        bound = {}
        for name in names:
            addresses = references.get(name, set())
            require(len(addresses) <= 1, '%s: %s bound at several addresses' % (module['name'], name))
            if addresses:
                bound[name] = next(iter(addresses))
        start = int(module['range'][0], 16)
        inner = {}
        for point in module.get('inner_points', []):
            # Bound when an accepted relocation to the symbol resolves to the point, the symbol itself bound at
            # the section base (prove checks the base): table symbol plus addend.
            base_at = bound.get(point['symbol'])
            inner[point['symbol'], point['offset']] = base_at is not None and \
                base_at + point['offset'] in resolved.get(point['symbol'], set())
        start, data = prove(module, coff, bound, original, inner)
        section = next(s for s in pe.sections if s.VirtualAddress <= start - base < s.VirtualAddress + s.SizeOfRawData)
        path = scaffold / (section.Name.rstrip(b'\0').decode() + '.bin')
        image = bytearray(path.read_bytes())
        offset = start - base - section.VirtualAddress
        require(offset + len(data) <= len(image), '%s: range outside the raw section' % module['name'])
        image[offset:offset + len(data)] = data
        path.write_bytes(bytes(image))
        receipts.append(dict(name=module['name'], generator=module['generator'], range=module['range'],
                             bytes=len(data), sha256=hashlib.sha256(data).hexdigest(),
                             source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),
                             bound_symbols=sorted(bound) + sorted(p['name'] for p in module.get('inner_points', [])),
                             flagged_constants=[c['name'] for c in module.get('flagged_constants', [])]))
    return receipts
