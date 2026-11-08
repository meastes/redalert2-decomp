#!/usr/bin/env python3
"""Give repeated definitions VC6's "select any" link semantics (pre-link step).

The exact build never links: each compile group is compared region by region,
so a group may carry its own copy of a function that another group also
defines (an out-of-line `template <>` member VC6 emitted as a COMDAT in every
unit, or a callee a source repeats so its unit compiles: "the accepted
separate copy remains authoritative"). The native game links every object, so
such names are defined more than once. This step marks every definition after
the first (in link order, i.e. the original's unit order) as a weak definition
(Mach-O N_WEAK_DEF) in the object file, so the linker keeps the first copy, as
the VC6 linker keeps one COMDAT. The list is written next to the inputs.

    python3 native/tools/weaken_duplicates.py --inputs gen/link_inputs.txt --report gen/duplicates.json
"""
import argparse
import json
import struct
import subprocess
from pathlib import Path

N_WEAK_DEF = 0x0080
LC_SYMTAB = 0x2
MH_MAGIC_64 = 0xFEEDFACF


def definitions(paths):
    """{object path: [external defined names]} (nm in batches)."""
    out = {}
    for i in range(0, len(paths), 400):
        batch = paths[i:i + 400]
        text = subprocess.run(['nm', '-g', '-U'] + batch, capture_output=True, text=True).stdout
        current = batch[0] if len(batch) == 1 else None
        for line in text.splitlines():
            if line.endswith(':') and line[:-1] in batch:
                current = line[:-1]
                continue
            parts = line.split()
            if current and len(parts) == 3 and parts[1] in 'TSD':
                out.setdefault(current, []).append(parts[2])
    return out


def weaken(path, names):
    data = bytearray(Path(path).read_bytes())
    if struct.unpack_from('<I', data, 0)[0] != MH_MAGIC_64:
        return []
    ncmds = struct.unpack_from('<I', data, 16)[0]
    off = 32
    changed = []
    for _ in range(ncmds):
        cmd, size = struct.unpack_from('<II', data, off)
        if cmd == LC_SYMTAB:
            symoff, nsyms, stroff, _strsize = struct.unpack_from('<IIII', data, off + 8)
            for i in range(nsyms):
                e = symoff + 16 * i
                strx, ntype, _sect, desc = struct.unpack_from('<IBBH', data, e)
                if not (ntype & 0x01) or (ntype & 0x0E) != 0x0E:   # external, defined in a section
                    continue
                end = data.index(0, stroff + strx)
                name = data[stroff + strx:end].decode()
                if name in names and not desc & N_WEAK_DEF:
                    struct.pack_into('<H', data, e + 6, desc | N_WEAK_DEF)
                    changed.append(name)
        off += size
    if changed:
        Path(path).write_bytes(bytes(data))
    return changed


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--inputs', type=Path, required=True)
    ap.add_argument('--report', type=Path, required=True)
    args = ap.parse_args()
    paths = [p for p in args.inputs.read_text().split('\n') if p.strip() and p.endswith('.o')]
    first, repeats = {}, {}
    defs = definitions(paths)
    for p in paths:
        for name in defs.get(p, []):
            if name in first and first[name] != p:
                repeats.setdefault(p, set()).add(name)
            else:
                first.setdefault(name, p)
    rows = []
    for p, names in repeats.items():
        # Names that are already weak (inline functions, template
        # instantiations) need nothing; only strong repeats are changed.
        for n in sorted(weaken(p, names)):
            rows.append(dict(symbol=n, kept=Path(first[n]).name, weakened=Path(p).name))
    args.report.write_text(json.dumps(dict(
        about='Strong definitions repeated across recovered objects; later copies made weak (weaken_duplicates.py). '
              'A re-run after the first finds them already weak, so only a fresh build lists them.',
        count=len(rows), duplicates=rows), indent=1))
    print('weaken_duplicates: %d repeated definitions weakened' % len(rows))


if __name__ == '__main__':
    main()
