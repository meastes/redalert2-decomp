#!/usr/bin/env python3
"""Defer the YR units' static initialisers until the YR globals hold their values (lane Y2).

The original's _initterm runs every static initialiser with gamemd.exe's data already in
place (it is part of the image). Natively the YR globals the tree only declares are filled
from the user's gamemd.exe by the port (load_original_data, before WinMain), so an
initialiser that reads one (MapClass's TabClass reads a sidebar name) must run after that,
not while dyld loads the program. This pre-link step renames each YR object's initialiser
pointer section (__DATA,__mod_init_func, S_MOD_INIT_FUNC_POINTERS) to an ordinary data
section __DATA,__yr_init_func: dyld no longer calls the pointers, the linker keeps them in
link order (the units' order) with their relocations, and native/yr/glue/yr_original_data.cpp
calls them, in that order, once the data is loaded (ra2_native_original_records). Object files
already renamed are left as they are.

    python3 native/yr/tools/defer_initializers.py --inputs <link_inputs.txt>
"""
import argparse
import struct
from pathlib import Path

MH_MAGIC_64 = 0xFEEDFACF
LC_SEGMENT_64 = 0x19
S_MOD_INIT_FUNC_POINTERS = 0x9


def defer(path, section=b'__yr_init_func'):
    data = bytearray(path.read_bytes())
    if struct.unpack_from('<I', data, 0)[0] != MH_MAGIC_64:
        return 0
    ncmds = struct.unpack_from('<I', data, 16)[0]
    off, changed = 32, 0
    for _ in range(ncmds):
        cmd, size = struct.unpack_from('<II', data, off)
        if cmd == LC_SEGMENT_64:
            nsects = struct.unpack_from('<I', data, off + 64)[0]
            s = off + 72
            for _ in range(nsects):
                name = bytes(data[s:s + 16]).rstrip(b'\0')
                flags = struct.unpack_from('<I', data, s + 64)[0]
                if name == b'__mod_init_func' and (flags & 0xFF) == S_MOD_INIT_FUNC_POINTERS:
                    data[s:s + 16] = section.ljust(16, b'\0')
                    struct.pack_into('<I', data, s + 64, flags & ~0xFF)   # S_REGULAR
                    changed += 1
                s += 80
        off += size
    if changed:
        path.write_bytes(bytes(data))
    return changed


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--inputs', type=Path)
    ap.add_argument('--object', type=Path, help='one object (an engine\'s ld -r output, lane Y8) instead of --inputs')
    ap.add_argument('--section', default='__yr_init_func', help='the section the pointers move to')
    ap.add_argument('--only', default='/gen-yr/units/', help='objects whose path contains this')
    a = ap.parse_args()
    if a.object:
        paths = [a.object]
    else:
        paths = [Path(l.strip()) for l in a.inputs.read_text().splitlines() if l.strip() and a.only in l]
    n = sum(defer(p, a.section.encode()) for p in paths)
    print('defer_initializers: %d initialiser sections deferred in %d objects' % (n, len(paths)))


if __name__ == '__main__':
    main()
