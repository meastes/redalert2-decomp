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
from library_link import coff_sections_with_types, short_imports

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
    for position, (member, obj) in enumerate(archive_members(path)):
        name = member.split('\\')[-1]
        for index, (sname, raw, _relocs, _syms, _types) in enumerate(coff_sections_with_types(obj)):
            out[(name, position, index)] = raw
    return out


def parts(audit, library, accepted=()):
    """[(section name, bytes)] and receipts for every library region, each proven
    against the original image. A row that an accepted source region now covers
    (a landing since the manifest was generated) is skipped and reported, so the
    catalog stays authoritative; regenerate library.json to drop it for good."""
    if not library:
        return [], []
    extents = sorted((r['entry'], r['entry'] + r['full_extent_bytes']) for r in accepted)
    import bisect
    starts = [a for a, _ in extents]
    cache, shorts = {}, {}
    parts, receipts = [], []
    superseded = []
    for row in sorted(library['regions'], key=lambda r: int(r['entry'], 16)):
        entry = int(row['entry'], 16)
        i = bisect.bisect_right(starts, entry + row['bytes'] - 1)
        if any(a < entry + row['bytes'] and entry < b for a, b in extents[max(0, i - 2):i + 1]):
            superseded.append(dict(entry=entry, bytes=row['bytes'], member=row['member'], reason='covered by an accepted source region'))
            continue
        path = library['files'][row['archive_sha256']]
        if row.get('kind') == 'linker-import-thunk':
            # Short-format import object: the archive names the import, the linker's fixed
            # thunk form `jmp dword ptr [slot]` supplies the bytes; both are checked.
            if path not in shorts:
                shorts[path] = {t[0]: t for t in short_imports(path)}
            found = shorts[path].get(row['member_index'])
            require(found is not None and found[1] == row['symbols'][0] and row['import_'].split('!')[1] == found[2],
                    'import object %s missing from %s' % (row['symbols'][0], path.name))
            require(len(row['relocations']) == 1 and row['relocations'][0]['offset'] == 2 and row['bytes'] == 6,
                    'malformed thunk row at %08x' % entry)
            raw = b'\xff\x25\0\0\0\0'
        else:
            if path not in cache:
                cache[path] = _sections(path)
            raw = cache[path].get((row['member'], row['member_index'], row['section_index']))
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
                             symbols=row['symbols'][:3], transferred_sha256=row['sha256'], credit='library',
                             kind=row.get('kind', 'archive-section')))
    if superseded:
        receipts.append(dict(superseded_by_accepted_source=superseded))
    return parts, receipts
