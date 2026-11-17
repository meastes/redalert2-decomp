#!/usr/bin/env python3
"""Identify C runtime / library code in the original executable by archive identity.

For every code section of every member of the given COFF archives (VC6's
LIBCMT.LIB, COMSUPP.LIB, LIBC.LIB, LIBCPMT.LIB, LIBCIMT.LIB by default, in that
priority order), mask the relocation sites and search the original .text for the
bytes. Longest sections are placed first, a section identical across libraries
goes to the earlier library in the list, and later sections may not overlap an
earlier placement, so a short member (a one-instruction thunk) can only land in
a gap. Unexplained runs are then tested for near identity (a member of the same
length whose fixed bytes agree at 98% or more; a differing build of one object).

Output: each identified range with library, member and the symbols the section
defines; overlaps with accepted regions (library code that was accepted as C
source and should move to the library ledger); unexplained runs; near-identity
candidates. This is identification only, from the original bytes and the
archives; linking the identified members in place of scaffolding is a build
change made separately. Nothing here is source credit.

    python3 tools/original_target/library_identity.py                      # summary to stdout
    python3 tools/original_target/library_identity.py --out build/library-identity
    python3 tools/original_target/library_identity.py --lib path/to/OTHER.LIB --region 0x781000 0x79a000
"""
import argparse
import json
import re
import struct
import sys
from collections import defaultdict
from pathlib import Path

REPOSITORY = Path(__file__).resolve().parents[2]
MEDIA_LIB = REPOSITORY / 'private/original-target-20261020/toolchain/media/VC98/LIB'
DEFAULT_LIBS = ['LIBCMT.LIB', 'COMSUPP.LIB', 'LIBC.LIB', 'LIBCPMT.LIB', 'LIBCIMT.LIB']
IMAGE_SCN_CNT_CODE = 0x20
REL_SIZES = {0x06: 4, 0x07: 4, 0x14: 4, 0x0B: 2, 0x0C: 2, 0x0D: 2, 0x0E: 2, 0x0F: 2, 0x10: 2, 0x11: 2}


# ------------------------------------------------------------------ COFF archive
def archive_members(path):
    data = path.read_bytes()
    if data[:8] != b'!<arch>\n':
        raise ValueError('%s is not a COFF archive' % path.name)
    pos = 8
    longnames = b''
    while pos + 60 <= len(data):
        hdr = data[pos:pos + 60]
        name = hdr[:16].rstrip()
        size_field = hdr[48:58].strip(b' \0')
        if not size_field.isdigit():
            break
        size = int(size_field)
        body = data[pos + 60:pos + 60 + size]
        pos += 60 + size + (size & 1)
        if name in (b'/', b''):
            continue                              # linker members (symbol index)
        if name == b'//':
            longnames = body
            continue
        if name.startswith(b'/'):
            off = int(name[1:])
            end = longnames.index(b'\0', off)
            name = longnames[off:end]
        yield name.rstrip(b'/').decode('latin-1'), body


def coff_code_sections(obj):
    """[(section_name, raw bytes, [(offset, size)], [symbols])] for each code section."""
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
        if not (chars & IMAGE_SCN_CNT_CODE) or rawsize == 0 or rawptr == 0:
            continue
        raw = obj[rawptr:rawptr + rawsize]
        relocs = [(va, REL_SIZES.get(rtype, 4)) for va, _sym, rtype in
                  (struct.unpack_from('<IIH', obj, relptr + r * 10) for r in range(nrel))]
        defined = [n for n, v, sec, cls in symbols if sec == s + 1 and cls == 2 and v == 0] or \
                  [n for n, v, sec, cls in symbols if sec == s + 1 and cls == 2]
        out.append((sname, raw, relocs, defined))
    return out


# ------------------------------------------------------------------ executable
def load_text(executable):
    import pefile
    pe = pefile.PE(str(executable), fast_load=True)
    for s in pe.sections:
        if s.Name.rstrip(b'\0') == b'.text':
            return pe.OPTIONAL_HEADER.ImageBase + s.VirtualAddress, s.get_data()
    raise ValueError('no .text in %s' % executable)


def accepted_regions():
    sys.path.insert(0, str(REPOSITORY / 'tools/original_target'))
    try:
        import catalog_io
        cat = catalog_io.load(catalog_io.locate(REPOSITORY), references=False)
        return sorted(((r['entry'], r['entry'] + r['body_bytes'], r.get('symbol') or '') for r in cat['regions']))
    except Exception as e:                        # the catalog is optional for identification
        print('catalog not read (%s): accepted overlaps not reported' % e, file=sys.stderr)
        return []


def masked(raw, relocs):
    mask = bytearray(b'\1' * len(raw))
    for off, size in relocs:
        for k in range(off, min(off + size, len(raw))):
            mask[k] = 0
    return bytes(mask)


def pattern(raw, mask):
    return b''.join(re.escape(bytes([b])) if m else b'.' for b, m in zip(raw, mask))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--config', type=Path, default=REPOSITORY / 'private/original-target.local.json')
    ap.add_argument('--lib', action='append', type=Path, help='archive to search, in priority order (default: VC98 %s)' % ', '.join(DEFAULT_LIBS))
    ap.add_argument('--region', nargs=2, type=lambda x: int(x, 0), metavar=('LO', 'HI'),
                    help='library region of .text to report coverage for (default 0x781000 to end of .text)')
    ap.add_argument('--min-fixed', type=int, default=10, help='minimum unmasked bytes for a section to be searched')
    ap.add_argument('--out', type=Path, help='directory for identity.json and table.md')
    args = ap.parse_args()
    executable = Path(json.loads(args.config.read_text())['executable'])
    base, text = load_text(executable)
    hi_text = base + len(text)
    lo, hi = args.region if args.region else (0x781000, hi_text)
    libs = args.lib or [MEDIA_LIB / n for n in DEFAULT_LIBS]

    sections = []
    for order, lib in enumerate(libs):
        try:
            for member, obj in archive_members(lib):
                for sname, raw, relocs, syms in coff_code_sections(obj):
                    sections.append(dict(lib=lib.name, order=order, member=member.split('\\')[-1], section=sname,
                                         raw=raw, mask=masked(raw, relocs), symbols=syms))
        except ValueError as e:
            print('skipped: %s' % e, file=sys.stderr)
    print('%d code sections from %s' % (len(sections), ', '.join(l.name for l in libs)), file=sys.stderr)

    taken = bytearray(len(text))
    placements, ambiguous = [], []
    for s in sorted(sections, key=lambda s: (-len(s['raw']), s['order'])):
        fixed = sum(s['mask'])
        if fixed < args.min_fixed:
            continue
        n = len(s['raw'])
        hits = [m.start() for m in re.finditer(pattern(s['raw'], s['mask']), text, re.S)]
        free = [h for h in hits if not any(taken[h:h + n])]
        inside = [h for h in free if lo <= base + h < hi]
        if inside:
            chosen = inside if (len(inside) == 1 or fixed >= 24) else []
            if not chosen:
                ambiguous.append(dict(lib=s['lib'], member=s['member'], section=s['section'], symbols=s['symbols'][:4],
                                      bytes=n, hits=['%08x' % (base + h) for h in inside[:8]]))
        else:
            chosen = free if (len(free) == 1 and fixed >= 32) else []     # outside the region: strict
        for h in chosen:
            taken[h:h + n] = b'\1' * n
            placements.append(dict(va=base + h, bytes=n, fixed=fixed, lib=s['lib'], member=s['member'],
                                   section=s['section'], symbols=s['symbols'][:6]))
    placements.sort(key=lambda p: p['va'])

    # unexplained runs and near-identity candidates
    gaps = []
    i = lo - base
    while i < hi - base:
        if taken[i]:
            i += 1; continue
        j = i
        while j < hi - base and not taken[j]:
            j += 1
        run = text[i:j]
        kind = 'fill' if run.strip(b'\xcc\x90\x00') == b'' else 'code/data'
        if kind == 'code/data':                   # linker import thunks: jmp dword ptr [IAT] between fill bytes
            k = thunks = 0
            while k < len(run) and (run[k] in (0xCC, 0x90, 0x00) or run[k:k + 2] == b'\xff\x25'):
                if run[k:k + 2] == b'\xff\x25':
                    thunks += 1; k += 6
                else:
                    k += 1
            if k >= len(run) and thunks:
                kind = 'import thunks (%d)' % thunks
        gaps.append(dict(va=base + i, bytes=j - i, kind=kind))
        i = j
    first_lib = min((p['va'] for p in placements if lo <= p['va'] < hi), default=lo)
    for g in gaps:
        if g['kind'] != 'fill' and g['va'] < first_lib:
            g['kind'] = 'before first library member'
    near = []
    for g in gaps:
        if g['kind'] != 'code/data' or g['bytes'] < 48:
            continue
        body = text[g['va'] - base:g['va'] - base + g['bytes'] + 64]
        best = None
        for s in sections:
            L = len(s['raw'])
            if L < 48 or L > g['bytes'] + 16 or L > len(body):
                continue
            fixed = sum(s['mask'])
            agree = sum(1 for k in range(L) if s['mask'][k] and s['raw'][k] == body[k])
            if fixed and agree >= 0.98 * fixed and (best is None or agree / fixed > best[0]):
                diffs = [k for k in range(L) if s['mask'][k] and s['raw'][k] != body[k]]
                best = (agree / fixed, dict(va='%08x' % g['va'], bytes=L, lib=s['lib'], member=s['member'], section=s['section'],
                                            symbols=s['symbols'][:3], agreement=round(100.0 * agree / fixed, 2),
                                            differing_offsets=['0x%x' % d for d in diffs[:8]]))
        if best:
            near.append(best[1])

    # overlaps with accepted regions
    regions = accepted_regions()
    overlaps = []
    for p in placements:
        for start, end, symbol in regions:
            if start < p['va'] + p['bytes'] and p['va'] < end:
                overlaps.append(dict(va='%08x' % p['va'], bytes=p['bytes'], member=p['member'], symbol=p['symbols'][:1],
                                     accepted_entry='%08x' % start, accepted_symbol=symbol[:60]))
                break

    in_region = [p for p in placements if lo <= p['va'] < hi]
    outside = [p for p in placements if not (lo <= p['va'] < hi)]
    covered = sum(taken[lo - base:hi - base])
    by_lib = defaultdict(int)
    for p in in_region:
        by_lib[p['lib']] += p['bytes']
    summary = dict(executable=str(executable), region=['%08x' % lo, '%08x' % hi], region_bytes=hi - lo,
                   first_library_member='%08x' % first_lib, identified_bytes=covered,
                   identified_percent=round(100.0 * covered / max(1, hi - lo), 2),
                   placements_in_region=len(in_region), by_library=dict(by_lib),
                   placements_outside=len(outside), outside_bytes=sum(p['bytes'] for p in outside),
                   unexplained_runs=len([g for g in gaps if g['kind'] == 'code/data']),
                   unexplained_bytes=sum(g['bytes'] for g in gaps if g['kind'] == 'code/data'),
                   before_first_member_bytes=sum(g['bytes'] for g in gaps if g['kind'] == 'before first library member'),
                   import_thunk_bytes=sum(g['bytes'] for g in gaps if g['kind'].startswith('import thunks')),
                   fill_bytes=sum(g['bytes'] for g in gaps if g['kind'] == 'fill'),
                   near_identity=len(near), ambiguous_sections=len(ambiguous),
                   accepted_overlaps=len(overlaps), accepted_overlap_bytes=sum(o['bytes'] for o in overlaps))
    print(json.dumps(summary, indent=1))
    print('largest unexplained runs:')
    for g in sorted((g for g in gaps if g['kind'] == 'code/data'), key=lambda g: -g['bytes'])[:12]:
        print('  %08x %6d B' % (g['va'], g['bytes']))
    for n in near:
        print('near identity: %s %d B %s %s %.2f%% differing at %s' % (n['va'], n['bytes'], n['lib'], n['member'], n['agreement'], n['differing_offsets']))
    if args.out:
        args.out.mkdir(parents=True, exist_ok=True)
        (args.out / 'identity.json').write_text(json.dumps(dict(
            summary=summary, placements=[dict(p, va='%08x' % p['va']) for p in placements],
            gaps=[dict(g, va='%08x' % g['va']) for g in gaps], near_identity=near, ambiguous=ambiguous,
            accepted_overlaps=overlaps), indent=1))
        L = ['# Library identity (generated by library_identity.py)', '',
             'Region %s-%s: %s of %s bytes identified (%.2f%%); first library member at %s; %d unexplained code/data runs (%s B); '
             '%s B before the first member; %s B fill; %d near-identity candidates; %d placements overlap accepted regions (%s B).' % (
                 summary['region'][0], summary['region'][1], format(covered, ','), format(hi - lo, ','), summary['identified_percent'],
                 summary['first_library_member'], summary['unexplained_runs'], format(summary['unexplained_bytes'], ','),
                 format(summary['before_first_member_bytes'], ','), format(summary['fill_bytes'], ','), len(near),
                 len(overlaps), format(summary['accepted_overlap_bytes'], ',')),
             '', '## Identified ranges', '', '| va | bytes | library | member | section | symbols |', '|---|---:|---|---|---|---|']
        L += ['| %08x | %d | %s | %s | %s | %s |' % (p['va'], p['bytes'], p['lib'], p['member'], p['section'],
                                                   ' '.join(p['symbols'][:3]).replace('|', '/')) for p in placements]
        L += ['', '## Unexplained runs', '', '| va | bytes | kind |', '|---|---:|---|']
        L += ['| %08x | %d | %s |' % (g['va'], g['bytes'], g['kind']) for g in gaps if g['kind'] != 'fill']
        L += ['', '## Near identity (same length, 98%+ of fixed bytes agree)', '', '| va | bytes | library | member | agreement | differing offsets |', '|---|---:|---|---|---:|---|']
        L += ['| %s | %d | %s | %s | %.2f | %s |' % (n['va'], n['bytes'], n['lib'], n['member'], n['agreement'], ' '.join(n['differing_offsets'])) for n in near]
        L += ['', '## Library code inside accepted regions (candidates to move from source credit to the library ledger)', '',
              '| va | bytes | member | symbol | accepted entry | accepted symbol |', '|---|---:|---|---|---|---|']
        L += ['| %s | %d | %s | %s | %s | %s |' % (o['va'], o['bytes'], o['member'], ' '.join(o['symbol']), o['accepted_entry'],
                                                  o['accepted_symbol'].replace('|', '/')) for o in overlaps]
        (args.out / 'table.md').write_text('\n'.join(L) + '\n')
        print('wrote', args.out / 'identity.json', 'and', args.out / 'table.md')


if __name__ == '__main__':
    main()
