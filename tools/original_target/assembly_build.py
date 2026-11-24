"""Link-time side of the handwritten-assembly ledger: re-assemble the adopted sources
with the pinned MASM, take each manifest row's procedure bytes, patch the recorded
relocation values and prove them equal to the original before placement. Rows of the
revised, binary-derived category (assembly_tools.REVISED_ALLOWLIST; user decision
2026-11-24) are audited (assembly_tools.validate_manifest) and proved the same way,
and their receipts carry their own credit."""
import json
import struct
from pathlib import Path

import assembly_tools as AT
import tree

REPOSITORY = AT.REPOSITORY
MANIFEST = REPOSITORY / tree.ASSEMBLY            # the tree's ledger (rebuild/manifest/assembly.json in the rebuild)


def require(condition, message):
    if not condition:
        raise SystemExit('assembly ledger: ' + message)


def revised_sources(doc):
    return list((doc.get('revised') or {}).get('sources', []))


def load(config, pinned=None):
    if not MANIFEST.exists():
        return None
    doc = json.loads(MANIFEST.read_text())
    require(doc.get('schema') == 'original-target-assembly-v1', 'unsupported manifest schema')
    found = AT.check_assembler(config, pinned)
    require(all(found[k] == doc['assembler'][k] for k in ('ML.EXE', 'ML.ERR')), 'assembler differs from the manifest')
    for source in doc['sources'] + revised_sources(doc):
        require((REPOSITORY / source).exists(), 'missing source %s' % source)
    AT.validate_manifest(doc, REPOSITORY)
    for include, digest in doc.get('includes', {}).items():
        require((REPOSITORY / include).exists() and AT.sha((REPOSITORY / include).read_bytes()) == digest, 'include changed: %s' % include)
    return doc


def parts(config, audit, assembly, destination, accepted=()):
    if not assembly:
        return [], []
    objects = AT.assemble_ledger(config, assembly['sources'], revised_sources(assembly), Path(destination) / 'assemble')
    parsed = {source: AT.read_object(obj.read_bytes()) for source, obj in objects.items()}
    extents = sorted((r['entry'], r['entry'] + r['full_extent_bytes']) for r in accepted)
    parts, receipts, superseded = [], [], []
    for row in sorted(assembly['regions'], key=lambda r: int(r['entry'], 16)):
        entry = int(row['entry'], 16)
        if any(a < entry + row['bytes'] and entry < b for a, b in extents):
            superseded.append(dict(entry=entry, bytes=row['bytes'], symbol=row['symbol'], reason='covered by an accepted source region'))
            continue
        require(AT.sha((REPOSITORY / row['source']).read_bytes()) == row['source_sha256'], 'source changed: %s' % row['source'])
        symbols, sections = parsed[row['source']]
        sec = sections[row['section_index']]
        require(sec is not None and sec['name'] == row['section'], 'section %s missing in %s' % (row['section'], row['source']))
        start = row['section_offset']
        data = bytearray(sec['raw'][start:start + row['bytes']])
        require(len(data) == row['bytes'], 'procedure %s shorter than the manifest' % row['symbol'])
        for relocation in row['relocations']:
            struct.pack_into('<I', data, relocation['offset'], relocation['resolved'])
        require(AT.sha(data) == row['sha256'] and bytes(data) == audit.original(entry, len(data)),
                'procedure %s differs from the original at %08x' % (row['symbol'], entry))
        parts.append(('.text$%08x' % entry, bytes(data)))
        receipts.append(dict(entry=entry, bytes=len(data), source=row['source'], symbol=row['symbol'], section=row['section'],
                             transferred_sha256=row['sha256'],
                             credit='handwritten assembly, revised (binary-derived lines)' if row.get('revised')
                             else 'handwritten assembly'))
    if superseded:
        receipts.append(dict(superseded_by_accepted_source=superseded))
    return parts, receipts
