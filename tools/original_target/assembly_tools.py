"""Shared pieces of the handwritten-assembly ledger: assemble a MASM source with the
pinned ML.EXE under Wine, read the object's code sections and public procedures,
and split a section into procedure extents.

The sources are the TibSun archive's `.asm` files adopted unchanged (THIRD_PARTY.md);
the original executable was assembled with MASM 6.13 (Rich header, 48 objects) and
MASM 6.15 from the VC6 Processor Pack reproduces the same bytes for these files.
"""
import hashlib
import json
import os
import shutil
import struct
import subprocess
from pathlib import Path

REPOSITORY = Path(__file__).resolve().parents[2]
DEFAULT_ASSEMBLER = REPOSITORY / 'private/original-target-20261020/toolchain/masm615'
ML_FLAGS = ['/nologo', '/c', '/coff', '/Cx']
REL_SIZES = {0x06: 4, 0x07: 4, 0x14: 4, 0x0B: 2, 0x0C: 2, 0x0D: 2, 0x0E: 2, 0x0F: 2, 0x10: 2, 0x11: 2}

# Revised, binary-derived lines (user decision 2026-11-24, in the owner session; AGENTS.md "No fake source"): a
# procedure whose TibSun archive parent is cited, carrying RA2's revisions of it as derived from the binary, taken
# from ra2-decomp's src/ra2asm text adopted unchanged (GPL-3.0-or-later, EA section 7 and OpenTS notices kept;
# THIRD_PARTY.md). Its own ledger category: Reproduced .text only, never C/C++. The allowlist is per source: only
# these procedures are taken from each file (its other procedures, e.g. Draw_Voxel_Regular_ASM/Reverse_ASM, stay
# built from the archive text), each with the archive parent it revises. Diffs against the archive: lane O72's
# bundle item (d); evidence: lane O71 (both under private/dossiers).
REVISED_DIRECTORY = 'src/recovered/asm/ra2asm'
REVISED_FLAG = 'binary-derived'
REVISED_REFERENCE = dict(name='gibbo101/ra2-decomp', commit='f70722414a172d30d124b7271a87d55657d259d8',
                         path='src/ra2asm/<file>.asm',
                         licence='GPL-3.0-or-later, Copyright 2026 ra2-decomp contributors, with the Electronic Arts '
                                 '(section 7 terms) and OpenTS notices kept; adopted unchanged (THIRD_PARTY.md), '
                                 'user decision 2026-11-24')
REVISED_ALLOWLIST = {
    REVISED_DIRECTORY + '/detproc.asm': {
        '_Detect_MMX_Availability': 'TibSun code/detproc.asm Detect_MMX_Availability'},
    REVISED_DIRECTORY + '/isoasm.asm': {
        '_Iso_Blit_Asm1': 'TibSun code/isoasm.asm Iso_Blit_Asm1'},
    REVISED_DIRECTORY + '/winasm.asm': {
        '_Draw_Voxel_Regular_Normals_ASM': 'TibSun code/winasm.asm Draw_Voxel_Regular_Normals_ASM',
        '_Draw_Voxel_Reverse_Normals_ASM': 'TibSun code/winasm.asm Draw_Voxel_Reverse_Normals_ASM',
        '_Draw_Voxel_Regular_Lighting_Normals_ASM': 'TibSun code/winasm.asm Draw_Voxel_Regular_Lighting_Normals_ASM',
        '_Draw_Voxel_Reverse_Lighting_Normals_ASM': 'TibSun code/winasm.asm Draw_Voxel_Reverse_Lighting_Normals_ASM'},
}
REVISED_NOTICES = (b'SPDX-License-Identifier: GPL-3.0-or-later', b'Copyright 2026 ra2-decomp contributors',
                   b'Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors', b"EA's GPL Section 7 terms apply")


def validate_manifest(doc, repository=REPOSITORY):
    """The strict audit's checks of the assembly ledger's two categories (raises SystemExit): rows from the archive
    sources carry no revised flag; rows from a revised source are on the allowlist, carry `revised: binary-derived`
    and their cited archive parent; the revised sources are exactly the allowlisted files, present with their
    notices; no procedure is placed from both categories."""
    def require(condition, message):
        if not condition:
            raise SystemExit('assembly ledger audit: ' + message)
    revised = doc.get('revised') or {}
    sources = revised.get('sources', [])
    require(sorted(sources) == sorted(REVISED_ALLOWLIST) if sources else True,
            'revised sources differ from the allowlist in assembly_tools.py')
    require(not (set(sources) & set(doc.get('sources', []))), 'a source listed in both categories')
    for source in sources:
        path = Path(repository) / source
        require(path.exists(), 'missing revised source %s' % source)
        head = path.read_bytes()[:600]
        require(all(n in head for n in REVISED_NOTICES), 'revised source %s lost its licence notices' % source)
    for source in doc.get('sources', []):
        require(source not in REVISED_ALLOWLIST and not source.startswith(REVISED_DIRECTORY + '/'),
                'revised source %s listed as archive text' % source)
    seen = {}
    for row in doc.get('regions', []):
        flag = row.get('revised')
        if row['source'] in sources:
            allowed = REVISED_ALLOWLIST[row['source']]
            require(row['symbol'] in allowed, 'procedure %s of %s is not on the revised allowlist'
                    % (row['symbol'], row['source']))
            require(flag == REVISED_FLAG and row.get('archive_parent') == allowed[row['symbol']],
                    'revised procedure %s needs revised: %s and its archive parent' % (row['symbol'], REVISED_FLAG))
        else:
            require(row['source'] in doc.get('sources', []), 'row %s from an unlisted source' % row['symbol'])
            require(flag is None and 'archive_parent' not in row,
                    'archive procedure %s carries a revised flag' % row['symbol'])
        require(row['symbol'] not in seen, 'procedure %s placed twice (%s, %s)'
                % (row['symbol'], seen.get(row['symbol']), row['source']))
        seen[row['symbol']] = row['source']


def sha(data):
    return hashlib.sha256(data).hexdigest()


def assembler_directory(config):
    return Path(config.get('assembler_directory', DEFAULT_ASSEMBLER))


def check_assembler(config, pinned):
    """The ML.EXE/ML.ERR pair in use, with their hashes checked against toolchain.json."""
    directory = assembler_directory(config)
    found = {}
    for name in ('ML.EXE', 'ML.ERR'):
        path = directory / name
        if not path.exists():
            raise SystemExit('assembler: %s missing (see ORIGINAL_TARGET_BUILD.md, handwritten assembly)' % path)
        found[name] = sha(path.read_bytes())
        if pinned and pinned.get(name) != found[name]:
            raise SystemExit('assembler: %s does not match the pin in manifests/toolchain.json' % name)
    return found


def assemble_ledger(config, archive, revised, destination):
    """Assemble both categories' sources; the revised files share their archive parents' names, so they assemble in
    a folder of their own. Returns {source: object path}."""
    objects = assemble(config, archive, Path(destination))
    if revised:
        objects.update(assemble(config, revised, Path(destination) / 'revised'))
    return objects


def assemble(config, sources, destination):
    """Assemble each source (a repository-relative path) into destination/<stem>.obj with
    the pinned ML under the project's Wine prefix, one at a time under the compiler
    lock. Returns {source: object path}; raises on any failure."""
    destination = Path(destination)
    destination.mkdir(parents=True, exist_ok=True)
    directory = assembler_directory(config)
    for name in ('ML.EXE', 'ML.ERR'):
        shutil.copyfile(directory / name, destination / name)
    native = os.name == 'nt'                     # the Windows gate runs ML.EXE natively (codex4 r172)
    env = os.environ.copy()
    if not native:
        env.update(WINEPREFIX=config['wine_prefix'], WINEDEBUG='-all')
    for include in sorted({inc for source in sources for inc in (REPOSITORY / source).parent.glob('*.inc')}):
        shutil.copyfile(include, destination / include.name)        # archive tables the sources include
    objects = {}
    from build import exclusive_lock             # the build's cross-platform compiler lock
    with exclusive_lock(config['lock']):
        for source in sources:
            src = REPOSITORY / source
            local = destination / src.name
            shutil.copyfile(src, local)
            obj = destination / (src.stem + '.obj')
            obj.unlink(missing_ok=True)
            program = [str((destination / 'ML.EXE').resolve())] if native else [config['wine'], 'ML.EXE']
            command = [*program, *ML_FLAGS, '/Fo' + obj.name, local.name]
            proc = subprocess.run(command, cwd=destination, env=env, capture_output=True, text=True, timeout=600)
            (destination / (src.stem + '.log')).write_text((proc.stdout + proc.stderr).replace('\r', ''))
            if proc.returncode != 0 or not obj.exists():
                raise SystemExit('assembler failed on %s: %s' % (source, (proc.stdout + proc.stderr)[-400:]))
            objects[source] = obj
    return objects


def read_object(obj):
    """(symbols, sections): symbols as [(name, value, section number, class)] indexed like the
    COFF symbol table; sections as a list (None for non-code) of dicts with name, raw bytes
    and relocations [(offset, type, symbol name)]."""
    machine, nsec, _ts, symptr, nsyms, optsize, _chars = struct.unpack_from('<HHIIIHH', obj, 0)
    if machine != 0x14C:
        raise ValueError('not an i386 COFF object')
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
        symbols.extend([('', 0, 0, 0)] * naux)
        i += 1 + naux
    sections = []
    for s in range(nsec):
        h = obj[20 + optsize + s * 40: 20 + optsize + (s + 1) * 40]
        name = h[:8].rstrip(b'\0').decode('latin-1')
        if name.startswith('/'):
            off = int(name[1:]); end = strtab.find(b'\0', off); name = strtab[off:end].decode('latin-1')
        _vs, _va, rawsize, rawptr, relptr, _ln, nrel, _nl, chars = struct.unpack_from('<IIIIIIHHI', h, 8)
        if not (chars & 0x20) or rawsize == 0 or rawptr == 0:
            sections.append(None); continue
        relocs = [(va, rtype, symbols[si][0] if si < len(symbols) else '')
                  for va, si, rtype in (struct.unpack_from('<IIH', obj, relptr + r * 10) for r in range(nrel))]
        sections.append(dict(name=name, index=s, raw=obj[rawptr:rawptr + rawsize], relocs=relocs))
    return symbols, sections


def procedures(symbols, sections):
    """[(section index, start, end, symbol)] for every public symbol of a code section,
    each extent running to the next public symbol or the section end."""
    out = []
    for si, sec in enumerate(sections):
        if not sec:
            continue
        starts = sorted({(v, n) for n, v, s, cls in symbols if s == si + 1 and cls == 2 and n})
        if not starts:
            starts = [(0, sec['name'])]
        for k, (v, n) in enumerate(starts):
            out.append((si, v, starts[k + 1][0] if k + 1 < len(starts) else len(sec['raw']), n))
    return out


def masked(raw, relocs, start):
    mask = bytearray(b'\1' * len(raw))
    for off, rtype, _ in relocs:
        o = off - start
        for k in range(o, min(o + REL_SIZES.get(rtype, 4), len(raw))):
            if k >= 0:
                mask[k] = 0
    return bytes(mask)
