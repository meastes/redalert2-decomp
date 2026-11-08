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
