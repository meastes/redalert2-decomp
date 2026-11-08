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
    """Every code section of every member, with its relocation list (offset, type, symbol)
    and the member's position in the archive (import libraries repeat member names)."""
    out = []
    for position, (member, obj) in enumerate(LI.archive_members(path)):
        index = 0
        for sname, raw, relocs, syms, rtypes in coff_sections_with_types(obj):
            out.append(dict(member=member.split('\\')[-1], member_index=position, section_index=index, section=sname,
                            raw=raw, relocs=rtypes, mask=LI.masked(raw, relocs), symbols=syms))
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

    symbols = []                                  # indexed by symbol table position (aux entries padded)
    i = 0
    while i < nsyms:
        raw = obj[symptr + i * 18: symptr + (i + 1) * 18]
        value, secnum, _type, sclass, naux = struct.unpack_from('<IhHBB', raw, 8)
        symbols.append((symname(raw[:8]), value, secnum, sclass))
        symbols.extend([('', 0, 0, 0)] * naux)
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
        rtypes = [(va, rtype, symbols[symidx][0] if symidx < len(symbols) else '')
                  for va, symidx, rtype in (struct.unpack_from('<IIH', obj, relptr + r * 10) for r in range(nrel))]
        relocs = [(va, LI.REL_SIZES.get(rtype, 4)) for va, rtype, _name in rtypes]
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
    deferred = {}
    for s in sorted(allsec, key=lambda s: (-len(s['raw']), s['order'])):
        fixed = sum(s['mask'])
        if fixed < 10:
            continue
        n = len(s['raw'])
        rx = LI.pattern(s['raw'], s['mask'])
        hits = [m.start() for m in re.finditer(rx, text, re.S)]
        free = [h for h in hits if lo <= base + h < hi and not any(taken[h:h + n])]
        if not free:
            continue
        if len(free) > 1 and fixed < 24:
            deferred.setdefault((s['raw'], s['mask']), []).append(s)     # identical short sections: second pass
            continue
        for h in free:
            taken[h:h + n] = b'\1' * n
            rows.append((base + h, s))
    # Second pass: a group of byte-identical sections (the same deleting destructor
    # emitted for several classes, two copies of a helper) whose free hits are no more
    # numerous than the sections is assigned in address order to the sections in
    # archive/member/section order; bytes and member provenance are exact, only the
    # symbol naming each copy follows the linker's emission order.
    for (raw, mask), group in deferred.items():
        n = len(raw)
        rx = LI.pattern(raw, mask)
        free = [m.start() for m in re.finditer(rx, text, re.S) if lo <= base + m.start() < hi and not any(taken[m.start():m.start() + n])]
        group = sorted(group, key=lambda s: (s['order'], s['member_index'], s['section_index']))
        if not free or len(free) > len(group):
            continue
        for h, s in zip(sorted(free), group):
            taken[h:h + n] = b'\1' * n
            s = dict(s, assignment='by order among %d identical sections' % len(group))
            rows.append((base + h, s))
    rows.sort(key=lambda t: t[0])
    return rows


IMPORT_LIB_DIR = TOOLCHAIN / 'media/VC98/LIB'


def import_name_matches(symbol, name):
    """`__imp__GetTickCount@4` / `__imp__wsprintfA` / `__imp_Name` name the PE import `name`."""
    if not symbol.startswith('__imp_'):
        return False
    s = symbol[6:]
    if s.startswith('_'):
        s = s[1:]
    if '@' in s:
        s = s[:s.index('@')]
    return s == name


def short_imports(path):
    """[(position, symbol, import name, dll)] for the short-format import objects of an
    import library (IMPORT_OBJECT_HEADER: sig 0,0xFFFF; the linker synthesizes the
    `jmp dword ptr [__imp_X]` thunk for these itself)."""
    out = []
    for position, (member, obj) in enumerate(LI.archive_members(path)):
        if len(obj) < 22 or struct.unpack_from('<HH', obj, 0) != (0, 0xFFFF):
            continue
        ordinal_or_hint = struct.unpack_from('<H', obj, 16)[0]
        flags = struct.unpack_from('<H', obj, 18)[0]
        name_type = (flags >> 2) & 7
        strings = obj[20:].split(b'\0')
        symbol = strings[0].decode('latin-1'); dll = strings[1].decode('latin-1') if len(strings) > 1 else ''
        imported = symbol
        if name_type == 0:                        # IMPORT_OBJECT_ORDINAL: the PE names it #N
            imported = '#%d' % ordinal_or_hint
        else:
            if name_type in (2, 3):               # NOPREFIX / UNDECORATE
                imported = imported.lstrip('_@?')
            if name_type == 3 and '@' in imported:
                imported = imported[:imported.index('@')]
        out.append((position, symbol, imported, dll))
    return out


def import_thunks(text, base, executable, covered, accepted):
    """Linker import thunks `jmp dword ptr [IAT slot]`: identified by the import the slot
    names, against the VC98 import library of that DLL (the member whose 6-byte code
    section relocates against `__imp_<name>`)."""
    import pefile
    pe = pefile.PE(str(executable), fast_load=True)
    pe.parse_data_directories()
    iat = {}
    for entry in pe.DIRECTORY_ENTRY_IMPORT:
        for imp in entry.imports:
            iat[imp.address] = (entry.dll.decode().upper(), (imp.name or b'').decode() or '#%d' % imp.ordinal)
    libraries = {}
    rows, skipped = [], []
    for off in range(len(text) - 6):
        if text[off] != 0xFF or text[off + 1] != 0x25:
            continue
        slot = struct.unpack_from('<I', text, off + 2)[0]
        if slot not in iat:
            continue
        va = base + off
        # A lone `jmp [IAT]` inside game code can be a compiled tail call (MSTimerClass::operator long
        # is one); only runs of thunks, or thunks in the library/tail bands, are linker output.
        def is_thunk(o):
            return 0 <= o < len(text) - 6 and text[o] == 0xFF and text[o + 1] == 0x25 and struct.unpack_from('<I', text, o + 2)[0] in iat
        if va < 0x781000 and not (is_thunk(off - 6) or is_thunk(off + 6)):
            skipped.append(dict(entry='%08x' % va, bytes=6, reason='isolated jmp [IAT] in game code: left to the catalog (compiled tail call?)'))
            continue
        if any(covered[off:off + 6]) or any(a < va + 6 and va < b for a, b, _ in accepted):
            continue
        dll, name = iat[slot]
        libname = dll.replace('.DLL', '.LIB')
        path = IMPORT_LIB_DIR / libname
        if not path.exists():
            skipped.append(dict(entry='%08x' % va, bytes=6, reason='import library %s not on the VC98 disc (%s!%s)' % (libname, dll, name)))
            continue
        if libname not in libraries:
            libraries[libname] = (path, sha(path.read_bytes()), archive_sections(path), short_imports(path))
        _, digest, sections, shorts = libraries[libname]
        original = text[off:off + 6]
        candidates = [x for x in sections if len(x['raw']) == 6 and x['raw'][:2] == b'\xff\x25'
                      and any(import_name_matches(sym, name) for _, _, sym in x['relocs'])]
        if len(candidates) == 1:                  # long-format import library: the thunk is a member section
            x = candidates[0]
            patched = bytearray(x['raw']); struct.pack_into('<I', patched, 2, slot)
            if bytes(patched) != original:
                skipped.append(dict(entry='%08x' % va, bytes=6, reason='patched thunk differs from original')); continue
            row = dict(member=x['member'], member_index=x['member_index'], section_index=x['section_index'], section=x['section'],
                       symbols=[sym for _, _, sym in x['relocs']][:2], kind='import-library-section')
        else:                                     # short-format import object: the linker synthesizes the thunk
            found = [t for t in shorts if t[2] == name]
            if len(found) != 1 or original != b'\xff\x25' + struct.pack('<I', slot):
                def undecorated(symbol):
                    s = symbol.lstrip('_@?'); return s[:s.index('@')] if '@' in s else s
                other = [t for t in shorts if undecorated(t[1]) == name.lstrip('#') or (name.startswith('#') and False)]
                reason = ('%d short import objects of %s name %s' % (len(found), libname, name) if not other else
                          'import convention differs: the VC98 %s imports %s as %s, the original by %s; linked against another %s' % (
                              libname, other[0][1], other[0][2], 'name' if not name.startswith('#') else 'ordinal', libname))
                skipped.append(dict(entry='%08x' % va, bytes=6, reason=reason)); continue
            position, symbol, _imported, _dll = found[0]
            row = dict(member='short import object', member_index=position, section_index=-1, section='(linker thunk)',
                       symbols=[symbol], kind='linker-import-thunk')
        covered[off:off + 6] = b'\1' * 6
        row.update(entry='%08x' % va, bytes=6, archive=libname, archive_role='VC6 RTM import library', archive_sha256=digest,
                   relocations=[dict(offset=2, type=6, resolved=slot)], sha256=sha(original), import_=dll + '!' + name)
        rows.append(row)
    return rows, skipped, {k: (v[0], v[1]) for k, v in libraries.items()}


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
        for roff, rtype, _name in s['relocs']:
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
                         archive_sha256=s['archive_sha256'], member=s['member'], member_index=s['member_index'],
                         section_index=s['section_index'], **({'assignment': s['assignment']} if s.get('assignment') else {}),
                         section=s['section'], symbols=s['symbols'][:8], relocations=relocations, sha256=sha(original)))
    covered = bytearray(len(text))
    for r in rows:
        e = int(r['entry'], 16) - base; covered[e:e + r['bytes']] = b'\1' * r['bytes']
    thunk_rows, thunk_skips, import_libs = import_thunks(text, base, executable, covered, accepted)
    rows += thunk_rows; skipped += thunk_skips
    rows.sort(key=lambda r: int(r['entry'], 16))
    archive_meta = [(name, role, sha(path.read_bytes())) for name, role, path, _ in archives]
    archive_meta += [(name, 'VC6 RTM import library', digest) for name, (path, digest) in sorted(import_libs.items())]
    archives_out = [dict(name=name, role=role, sha256=digest, members_used=sum(1 for r in rows if r['archive_sha256'] == digest))
                    for name, role, digest in archive_meta]
    archives_out = [a for a in archives_out if a['members_used']]      # only archives the build will open
    doc = dict(schema='original-target-library-v1',
               scope='Identified library code linked from pinned archives in place of original-byte scaffold; own ledger category, not source credit. '
                     'Generated by tools/original_target/library_link.py; identity by relocation-masked byte search (library_identity.py).',
               region=['%08x' % lo, '%08x' % hi], archives=archives_out, regions=rows,
               skipped_overlapping_accepted=[s for s in skipped if 'accepted_entry' in s],
               skipped_other=[s for s in skipped if 'accepted_entry' not in s])
    total = sum(r['bytes'] for r in rows)
    print('library regions: %d (%s B, of which %d import thunks %s B); skipped overlapping accepted source: %d (%s B); other skips: %d' % (
        len(rows), format(total, ','), len(thunk_rows), format(6 * len(thunk_rows), ','), len(doc['skipped_overlapping_accepted']),
        format(sum(s['bytes'] for s in doc['skipped_overlapping_accepted']), ','), len(doc['skipped_other'])))
    for sk in doc['skipped_other'][:6]:
        print('  skip', sk)
    for a in archives_out:
        print('  %s (%s): %d members used, sha256 %s' % (a['name'], a['role'], a['members_used'], a['sha256'][:16]))
    if args.write:
        MANIFEST.write_text(json.dumps(doc, indent=1) + '\n')
        toolchain = REPOSITORY / 'manifests/toolchain.json'
        pins = json.loads(toolchain.read_text())
        pins.setdefault('libraries', {})
        for a in archives_out:
            if a['members_used']:
                pins['libraries']['%s (%s)' % (a['name'], a['role'])] = a['sha256']
        toolchain.write_text(json.dumps(pins, indent=2) + '\n')
        print('wrote', MANIFEST, 'and the library pins in', toolchain.name)


if __name__ == '__main__':
    main()
