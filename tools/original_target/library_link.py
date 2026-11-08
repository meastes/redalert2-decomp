#!/usr/bin/env python3
"""Generate the library manifest: archive sections of the C runtime and support
libraries that the build links in place of original-byte scaffold.

Each row names an archive (pinned by SHA-256), a member, a code section and the
relocation values read from the original image at that section's address; the
member bytes with those values patched in must equal the original bytes exactly
(checked here and again by build.py before linking). Identity comes from
library_identity.py (relocation sites masked, longest first, non-overlapping);
sections that overlap an accepted source region are left out and listed, so the
catalog stays authoritative for what it already covers.

This is identified library code: its own ledger category, never source credit.

    python3 tools/original_target/library_link.py --write          # manifests/original_target/library.json
    python3 tools/original_target/library_link.py                  # report only
"""
import argparse
import hashlib
import json
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import library_identity as LI  # noqa: E402

REPOSITORY = Path(__file__).resolve().parents[2]
MANIFEST = REPOSITORY / 'manifests/original_target/library.json'
TOOLCHAIN = REPOSITORY / 'private/original-target-20261020/toolchain'
# Priority order: the original links Service Pack 3's LIBCMT (tzset.obj identity,
# OPEN_MISSES "Library identity" 2026-11-07); RTM LIBCMT only for members SP3
# did not change in a way that matters; COMSUPP has no SP3 update.
DEFAULT_ARCHIVES = [('LIBCMT.LIB', 'VC6 SP3', TOOLCHAIN / 'media-sp3/VC98/LIB/LIBCMT.LIB'),
                    ('LIBCMT.LIB', 'VC6 RTM', TOOLCHAIN / 'media/VC98/LIB/LIBCMT.LIB'),
                    ('COMSUPP.LIB', 'VC6 RTM', TOOLCHAIN / 'media/VC98/LIB/COMSUPP.LIB')]
REL_WIDTH = {0x06: 4, 0x07: 4, 0x14: 4}


def sha(data):
    return hashlib.sha256(data).hexdigest()


def archive_sections(path):
    """Every code section of every member, with its relocation list (offset, type)."""
    out = []
    for member, obj in LI.archive_members(path):
        index = 0
        for sname, raw, relocs, syms, rtypes in coff_sections_with_types(obj):
            out.append(dict(member=member.split('\\')[-1], section_index=index, section=sname, raw=raw,
                            relocs=rtypes, mask=LI.masked(raw, relocs), symbols=syms))
            index += 1
    return out


def coff_sections_with_types(obj):
    """Like library_identity.coff_code_sections but also returns (offset, type) pairs."""
    if len(obj) < 20:
        return []
    machine, nsec, _ts, symptr, nsyms, optsize, _chars = struct.unpack_from('<HHIIIHH', obj, 0)
    if machine != 0x14C:
        return []
    strtab = obj[symptr + nsyms * 18:]

    def symname(raw):
        if raw[:4] == b'\0\0\0\0':
            off = struct.unpack_from('<I', raw, 4)[0]
            end = strtab.find(b'\0', off)
            return strtab[off:end if end >= 0 else None].decode('latin-1')
        return raw.rstrip(b'\0').decode('latin-1')

    symbols = []
    i = 0
    while i < nsyms:
        raw = obj[symptr + i * 18: symptr + (i + 1) * 18]
        value, secnum, _type, sclass, naux = struct.unpack_from('<IhHBB', raw, 8)
        symbols.append((symname(raw[:8]), value, secnum, sclass))
        i += 1 + naux
    out = []
    shdr = 20 + optsize
    for s in range(nsec):
        h = obj[shdr + s * 40: shdr + (s + 1) * 40]
        sname = h[:8].rstrip(b'\0').decode('latin-1')
        _vs, _va, rawsize, rawptr, relptr, _ln, nrel, _nl, chars = struct.unpack_from('<IIIIIIHHI', h, 8)
        if not (chars & LI.IMAGE_SCN_CNT_CODE) or rawsize == 0 or rawptr == 0:
            continue
        raw = obj[rawptr:rawptr + rawsize]
        rtypes = [(va, rtype) for va, _sym, rtype in (struct.unpack_from('<IIH', obj, relptr + r * 10) for r in range(nrel))]
        relocs = [(va, LI.REL_SIZES.get(rtype, 4)) for va, rtype in rtypes]
        defined = [n for n, v, sec, cls in symbols if sec == s + 1 and cls == 2 and v == 0] or \
                  [n for n, v, sec, cls in symbols if sec == s + 1 and cls == 2]
        out.append((sname, raw, relocs, defined, rtypes))
    return out


def accepted_extents():
    sys.path.insert(0, str(REPOSITORY / 'tools/original_target'))
    import catalog_io
    cat = catalog_io.load(catalog_io.locate(REPOSITORY), references=False)
    return sorted((r['entry'], r['entry'] + r['full_extent_bytes'], r.get('symbol') or '') for r in cat['regions'])


def place(text, base, lo, hi, archives):
    """Non-overlapping placement, longest section first, earlier archive first on ties."""
    import re
    taken = bytearray(len(text))
    rows = []
    for order, (name, role, path, sections) in enumerate(archives):
        for s in sections:
            s['order'] = order; s['archive'] = name; s['role'] = role; s['archive_sha256'] = sha(path.read_bytes())
    allsec = [s for a in archives for s in a[3]]
    for s in sorted(allsec, key=lambda s: (-len(s['raw']), s['order'])):
        fixed = sum(s['mask'])
        if fixed < 10:
            continue
        n = len(s['raw'])
        rx = LI.pattern(s['raw'], s['mask'])
        hits = [m.start() for m in re.finditer(rx, text, re.S)]
        free = [h for h in hits if lo <= base + h < hi and not any(taken[h:h + n])]
        if not free or (len(free) > 1 and fixed < 24):
            continue
        for h in free:
            taken[h:h + n] = b'\1' * n
            rows.append((base + h, s))
    rows.sort(key=lambda t: t[0])
    return rows


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--config', type=Path, default=REPOSITORY / 'private/original-target.local.json')
    ap.add_argument('--region', nargs=2, type=lambda x: int(x, 0), metavar=('LO', 'HI'), default=(0x781000, 0x79A000))
    ap.add_argument('--write', action='store_true', help='write %s' % MANIFEST.relative_to(REPOSITORY))
    args = ap.parse_args()
    executable = Path(json.loads(args.config.read_text())['executable'])
    base, text = LI.load_text(executable)
    lo, hi = args.region
    archives = [(name, role, path, archive_sections(path)) for name, role, path in DEFAULT_ARCHIVES if path.exists()]
    accepted = accepted_extents()
    rows, skipped = [], []
    for va, s in place(text, base, lo, hi, archives):
        n = len(s['raw']); off = va - base
        original = text[off:off + n]
        overlap = next(((a, b, sym) for a, b, sym in accepted if a < va + n and va < b), None)
        if overlap:
            skipped.append(dict(entry='%08x' % va, bytes=n, member=s['member'], symbols=s['symbols'][:2],
                                accepted_entry='%08x' % overlap[0], accepted_symbol=overlap[2][:50]))
            continue
        patched = bytearray(s['raw'])
        relocations = []
        ok = True
        for roff, rtype in s['relocs']:
            width = REL_WIDTH.get(rtype)
            if width is None or roff + width > n:
                ok = False; break
            value = struct.unpack_from('<I', original, roff)[0]
            struct.pack_into('<I', patched, roff, value)
            relocations.append(dict(offset=roff, type=rtype, resolved=value))
        if not ok or bytes(patched) != original:
            skipped.append(dict(entry='%08x' % va, bytes=n, member=s['member'], symbols=s['symbols'][:2],
                                reason='relocation type unsupported' if not ok else 'patched member differs from original'))
            continue
        rows.append(dict(entry='%08x' % va, bytes=n, archive=s['archive'], archive_role=s['role'],
                         archive_sha256=s['archive_sha256'], member=s['member'], section_index=s['section_index'],
                         section=s['section'], symbols=s['symbols'][:8], relocations=relocations, sha256=sha(original)))
    archives_out = [dict(name=name, role=role, sha256=sha(path.read_bytes()), members_used=sum(1 for r in rows if r['archive_sha256'] == sha(path.read_bytes())))
                    for name, role, path, _ in archives]
    doc = dict(schema='original-target-library-v1',
               scope='Identified library code linked from pinned archives in place of original-byte scaffold; own ledger category, not source credit. '
                     'Generated by tools/original_target/library_link.py; identity by relocation-masked byte search (library_identity.py).',
               region=['%08x' % lo, '%08x' % hi], archives=archives_out, regions=rows,
               skipped_overlapping_accepted=[s for s in skipped if 'accepted_entry' in s],
               skipped_other=[s for s in skipped if 'accepted_entry' not in s])
    total = sum(r['bytes'] for r in rows)
    print('library regions: %d (%s B); skipped overlapping accepted source: %d (%s B); other skips: %d' % (
        len(rows), format(total, ','), len(doc['skipped_overlapping_accepted']),
        format(sum(s['bytes'] for s in doc['skipped_overlapping_accepted']), ','), len(doc['skipped_other'])))
    for a in archives_out:
        print('  %s (%s): %d members used, sha256 %s' % (a['name'], a['role'], a['members_used'], a['sha256'][:16]))
    if args.write:
        MANIFEST.write_text(json.dumps(doc, indent=1) + '\n')
        print('wrote', MANIFEST)


if __name__ == '__main__':
    main()
