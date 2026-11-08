"""Link-time side of the library ledger: read manifests/original_target/library.json,
locate each pinned archive by SHA-256, extract the named member section, patch the
recorded relocation values and prove the bytes equal the original before they
enter the placement container as fixed-address `.text$VA` fragments.

Library code is its own ledger category (identified, reproducibly supplied
components); it is never source credit and the whole-file comparison is unchanged.
"""
import hashlib
import json
import struct
from pathlib import Path

from library_identity import archive_members
from library_link import coff_sections_with_types

REPOSITORY = Path(__file__).resolve().parents[2]
MANIFEST = REPOSITORY / 'manifests/original_target/library.json'
DEFAULT_DIRECTORIES = [REPOSITORY / 'private/original-target-20261020/toolchain/media-sp3/VC98/LIB',
                       REPOSITORY / 'private/original-target-20261020/toolchain/media/VC98/LIB']


def sha(data):
    return hashlib.sha256(data).hexdigest()


def require(condition, message):
    if not condition:
        raise SystemExit('library ledger: ' + message)


def load(config, pinned=None):
    """The library manifest with each archive resolved to a file whose SHA-256 matches.
    Returns None when the project has no library manifest."""
    if not MANIFEST.exists():
        return None
    doc = json.loads(MANIFEST.read_text())
    require(doc.get('schema') == 'original-target-library-v1', 'unsupported manifest schema')
    directories = [Path(p) for p in config.get('libraries', [])] + DEFAULT_DIRECTORIES
    if pinned:
        for archive in doc['archives']:
            key = '%s (%s)' % (archive['name'], archive['role'])
            require(pinned.get(key) == archive['sha256'], 'archive %s not pinned in manifests/toolchain.json' % key)
    files = {}
    for archive in doc['archives']:
        if not any(r['archive_sha256'] == archive['sha256'] for r in doc['regions']):
            continue
        found = None
        for directory in directories:
            candidate = directory / archive['name']
            if candidate.exists() and sha(candidate.read_bytes()) == archive['sha256']:
                found = candidate; break
        require(found is not None, 'no file with SHA-256 %s for %s (%s) under %s' % (
            archive['sha256'][:16], archive['name'], archive['role'], ', '.join(str(d) for d in directories)))
        files[archive['sha256']] = found
    doc['files'] = files
    return doc


def _sections(path):
    out = {}
    for member, obj in archive_members(path):
        name = member.split('\\')[-1]
        for index, (sname, raw, _relocs, _syms, _types) in enumerate(coff_sections_with_types(obj)):
            out[(name, index)] = raw
    return out


def parts(audit, library):
    """[(section name, bytes)] and receipts for every library region, each proven
    against the original image."""
    if not library:
        return [], []
    cache = {}
    parts, receipts = [], []
    for row in sorted(library['regions'], key=lambda r: int(r['entry'], 16)):
        entry = int(row['entry'], 16)
        path = library['files'][row['archive_sha256']]
        if path not in cache:
            cache[path] = _sections(path)
        raw = cache[path].get((row['member'], row['section_index']))
        require(raw is not None, 'member %s section %d missing from %s' % (row['member'], row['section_index'], path.name))
        require(len(raw) == row['bytes'], 'member %s length differs from the manifest' % row['member'])
        data = bytearray(raw)
        for relocation in row['relocations']:
            struct.pack_into('<I', data, relocation['offset'], relocation['resolved'])
        require(sha(data) == row['sha256'] and bytes(data) == audit.original(entry, len(data)),
                'library member %s differs from the original at %08x' % (row['member'], entry))
        parts.append(('.text$%08x' % entry, bytes(data)))
        receipts.append(dict(entry=entry, bytes=len(data), archive=row['archive'], archive_role=row['archive_role'],
                             archive_sha256=row['archive_sha256'], member=row['member'], section_index=row['section_index'],
                             symbols=row['symbols'][:3], transferred_sha256=row['sha256'], credit='library'))
    return parts, receipts
