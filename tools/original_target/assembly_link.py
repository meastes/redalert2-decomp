#!/usr/bin/env python3
"""Generate the handwritten-assembly manifest: procedures of the adopted TibSun `.asm`
sources that, assembled with the pinned MASM, reproduce the original bytes.

Each source is assembled fresh, split into procedures at its public symbols, and
every procedure is searched in the original .text with relocation sites masked
(trailing alignment fill the original does not keep is trimmed from the extent).
A row records source path and SHA-256, section, symbol, offset, length, the
relocation values read from the original and the SHA-256 of the original bytes;
build.py (assembly_build.py) re-assembles, patches and proves each row again
before linking it in place of scaffold. Evidenced handwritten assembly is its own
ledger category, not C/C++ credit.

Revised, binary-derived lines (user decision 2026-11-24): the allowlisted procedures
of the ra2-decomp `src/ra2asm` files in <tree asm>/ra2asm (assembly_tools.
REVISED_ALLOWLIST) are placed after every archive procedure, so they never displace
an archive row; each carries `revised: binary-derived` and its cited archive parent,
and an archive miss of the same procedure leaves the miss list. Their own category:
Reproduced .text only, never C/C++.

    python3 tools/original_target/assembly_link.py --write     # manifests/original_target/assembly.json
    RA2_TREE=rebuild python3 tools/original_target/assembly_link.py --write   # rebuild/manifest/assembly.json
"""
import argparse
import json
import re
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import assembly_tools as AT  # noqa: E402
import library_identity as LI  # noqa: E402
import tree  # noqa: E402

REPOSITORY = AT.REPOSITORY
MANIFEST = REPOSITORY / tree.ASSEMBLY
SOURCES = sorted(p.relative_to(REPOSITORY).as_posix() for p in (REPOSITORY / tree.ASM).glob('*.asm'))
INCLUDES = sorted(p.relative_to(REPOSITORY).as_posix() for p in (REPOSITORY / tree.ASM).glob('*.inc'))
ARCHIVE = dict(name='OpenTS-Developers/TibSun', commit='dbfbfbfaac40364ce7e1cd8fad7bcade5c6fb0d4', path='code/<file>.asm',
               licence='GPL-3.0-or-later with EA section 7 terms (THIRD_PARTY.md); adopted unchanged, user decision 2026-11-06')


def accepted_extents():
    sys.path.insert(0, str(REPOSITORY / 'tools/original_target'))
    import catalog_io
    cat = catalog_io.load(catalog_io.locate(REPOSITORY), references=False)
    return sorted((r['entry'], r['entry'] + r['full_extent_bytes'], r.get('symbol') or '') for r in cat['regions'])


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--config', type=Path, default=REPOSITORY / 'private/original-target.local.json')
    ap.add_argument('--write', action='store_true')
    args = ap.parse_args()
    config = json.loads(args.config.read_text())
    assembler = AT.check_assembler(config, None)
    base, text = LI.load_text(Path(config['executable']))
    accepted = accepted_extents()
    work = REPOSITORY / 'build/assembly-link'
    revised_sources = sorted(AT.REVISED_ALLOWLIST)
    objects = AT.assemble_ledger(config, SOURCES, revised_sources, work)
    taken = bytearray(len(text))
    rows, misses = [], []
    candidates = []
    for source, obj in objects.items():
        symbols, sections = AT.read_object(obj.read_bytes())
        allowed = AT.REVISED_ALLOWLIST.get(source)
        for si, start, end, name in AT.procedures(symbols, sections):
            if allowed is not None and name not in allowed:
                continue                      # per-source allowlist: the archive keeps every other procedure
            candidates.append((allowed is not None, end - start, source, sections[si], start, end, name))
    # Archive procedures first (longest first), then the revised ones (longest first).
    for revised, _, source, sec, start, end, name in sorted(candidates, key=lambda c: (c[0], -c[1])):
        placed = False
        for trim in range(0, 64):
            L = end - start - trim
            if L < 8:
                break
            raw = sec['raw'][start:start + L]
            mask = AT.masked(raw, sec['relocs'], start)
            fixed = sum(mask)
            if fixed < 8:
                break
            hits = [m.start() for m in re.finditer(LI.pattern(raw, mask), text, re.S)]
            free = [h for h in hits if not any(taken[h:h + L])]
            if not free or (len(free) > 1 and fixed < 24):
                continue
            h = free[0]
            va = base + h
            overlap = next(((a, b, s) for a, b, s in accepted if a < va + L and va < b), None)
            if overlap:
                misses.append(dict(source=source, symbol=name, bytes=L, reason='overlaps accepted region %08x %s' % (overlap[0], overlap[2][:40])))
                placed = True; break
            original = text[h:h + L]
            patched = bytearray(raw); relocations = []; ok = True
            for off, rtype, _sym in sec['relocs']:
                o = off - start
                if 0 <= o < L:
                    width = AT.REL_SIZES.get(rtype)
                    if width != 4 or o + 4 > L:
                        ok = False; break
                    value = struct.unpack_from('<I', original, o)[0]
                    struct.pack_into('<I', patched, o, value)
                    relocations.append(dict(offset=o, type=rtype, resolved=value))
            if not ok or bytes(patched) != original:
                misses.append(dict(source=source, symbol=name, bytes=L, reason='relocation/patch mismatch')); placed = True; break
            taken[h:h + L] = b'\1' * L
            rows.append(dict(entry='%08x' % va, bytes=L, source=source, source_sha256=AT.sha((REPOSITORY / source).read_bytes()),
                             section=sec['name'], section_index=sec['index'], symbol=name, section_offset=start,
                             trimmed_fill=trim, relocations=relocations, sha256=AT.sha(original),
                             **(dict(revised=AT.REVISED_FLAG, archive_parent=AT.REVISED_ALLOWLIST[source][name])
                                if revised else {})))
            placed = True
            break
        if not placed:
            misses.append(dict(source=source, symbol=name, bytes=end - start, reason='no exact placement'))
    rows.sort(key=lambda r: int(r['entry'], 16))
    # An archive procedure that a revised row reproduces is no longer a miss (its bytes are built).
    revised_placed = {r['symbol'] for r in rows if r.get('revised')}
    misses = [m for m in misses if not (m['source'] in SOURCES and m['symbol'] in revised_placed)]
    doc = dict(schema='original-target-assembly-v1',
               scope='Evidenced handwritten assembly: TibSun archive .asm sources adopted unchanged, assembled with the pinned MASM, '
                     'each procedure proven against the original bytes; own ledger category, not C/C++ credit.',
               archive=ARCHIVE, assembler=dict(flags=AT.ML_FLAGS, **assembler), sources=SOURCES,
               includes={i: AT.sha((REPOSITORY / i).read_bytes()) for i in INCLUDES},
               revised=dict(scope='Revised, binary-derived lines (user decision 2026-11-24): the allowlisted procedures of '
                                  'ra2-decomp src/ra2asm text adopted unchanged, each revising a cited TibSun archive '
                                  'procedure with lines derived from the binary; own ledger category, Reproduced .text '
                                  'only, never C/C++; rows flagged revised: binary-derived.',
                            reference=AT.REVISED_REFERENCE, sources=revised_sources,
                            allowlist={s: sorted(v) for s, v in AT.REVISED_ALLOWLIST.items()}),
               regions=rows, misses=misses)
    AT.validate_manifest(doc)
    total = sum(r['bytes'] for r in rows)
    revised_total = sum(r['bytes'] for r in rows if r.get('revised'))
    print('assembly regions: %d procedures, %s B (of which revised, binary-derived: %d procedures, %s B); misses: %d'
          % (len(rows), format(total, ','), len(revised_placed), format(revised_total, ','), len(misses)))
    for m in misses:
        print('  miss', m)
    if args.write:
        MANIFEST.write_text(json.dumps(doc, indent=1) + '\n')
        toolchain = REPOSITORY / 'manifests/toolchain.json'
        pins = json.loads(toolchain.read_text())
        pins['assembler'] = assembler
        toolchain.write_text(json.dumps(pins, indent=2) + '\n')
        print('wrote', MANIFEST, 'and the assembler pin')


if __name__ == '__main__':
    main()
