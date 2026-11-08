#!/usr/bin/env python3
"""Write native/initterm.json: the original's C++ static-initialiser order.

The original's CRT runs `_initterm(__xc_a, __xc_z)` before WinMain: a table of
function pointers in .data (.CRT$XCA..XCZ). This tool reads that table from the
pinned target (hash-checked, private/, never stored) and records, in order, the
catalog region each pointer names: its address, original symbol and source.
Only addresses and names are written (a byte-free manifest, like the catalog);
no code or data bytes leave the executable.

Entries whose symbol is a compiler initialiser (`_$E<n>`) are 'implicit': the
native compiler emits the same initialiser from the global definitions in that
source. Every other entry is an 'explicit' function the table names directly
(a plain init function); the native build calls it at the same position.

    python3 native/tools/initterm_manifest.py [--exe private/.../game.exe]
"""
import argparse
import hashlib
import json
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'tools/original_target'))
import catalog_io  # noqa: E402

SHA256 = 'e6418111c2c36081a1795fb000e0927beea1630170b334c5013d12102602e70f'
DEFAULT_EXE = ROOT / 'private/original-target-20261020/inputs/game.exe'


def sections(data):
    pe = struct.unpack_from('<I', data, 0x3C)[0]
    count = struct.unpack_from('<H', data, pe + 6)[0]
    optional = struct.unpack_from('<H', data, pe + 20)[0]
    base = struct.unpack_from('<I', data, pe + 24 + 28)[0]
    out = []
    for i in range(count):
        o = pe + 24 + optional + 40 * i
        name = data[o:o + 8].rstrip(b'\0').decode()
        vsize, va, rsize, raw = struct.unpack_from('<IIII', data, o + 8)
        out.append((name, base + va, vsize, rsize, raw))
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--exe', type=Path, default=DEFAULT_EXE)
    ap.add_argument('--out', type=Path, default=ROOT / 'native/initterm.json')
    args = ap.parse_args()
    data = args.exe.read_bytes()
    if hashlib.sha256(data).hexdigest() != SHA256:
        raise SystemExit('not the pinned target')
    secs = sections(data)

    def u32(addr):
        for name, va, vsize, rsize, raw in secs:
            if va <= addr < va + rsize:
                return struct.unpack_from('<I', data, raw + addr - va)[0]
        raise ValueError(hex(addr))

    catalog = catalog_io.load(catalog_io.locate(ROOT), references=False)
    by_entry = {int(r['entry']): r for r in catalog['regions']}
    # .CRT$XCA..XCZ: __xc_a is the null slot at the start of .data; the list
    # runs to the null slot before the C initialisers (__xi_a), which are
    # CRT-internal and native-irrelevant.
    data_start = next(va for name, va, *_ in secs if name == '.data')
    if u32(data_start) != 0:
        raise SystemExit('__xc_a is not at the start of .data')
    entries = []
    addr = data_start + 4
    while True:
        value = u32(addr)
        if value == 0:
            following = u32(addr + 4)
            if following == 0 or following not in by_entry:
                break
        else:
            row = by_entry.get(value)
            if row is None:
                raise SystemExit('initialiser %#x is not a catalog region' % value)
            entries.append(dict(index=len(entries), slot='0x%08X' % addr, entry='0x%08X' % value,
                                symbol=row['symbol'], source=row['source'],
                                kind='implicit' if row['symbol'].startswith('_$E') else 'explicit'))
        addr += 4
    head = json.dumps(dict(
        about=('Original C++ static-initialiser order (_initterm over .CRT$XCA..XCZ of the pinned target), '
               'read by native/tools/initterm_manifest.py: slot and entry addresses and catalog names only. '
               'implicit = a compiler _$E initialiser the native compiler re-emits from the source; '
               'explicit = a named init function the native build calls at that position.'),
        target_sha256=SHA256, table_start='0x%08X' % data_start, table_end='0x%08X' % addr,
        entries=[]), indent=1)
    # one entry per line keeps the manifest small and diffable
    body = ',\n'.join('  ' + json.dumps(e) for e in entries)
    args.out.write_text(head.replace('"entries": []', '"entries": [\n%s\n ]' % body) + '\n')
    print('%d initialisers (%d explicit), table 0x%08X-0x%08X' % (
        len(entries), sum(e['kind'] == 'explicit' for e in entries), data_start, addr))


if __name__ == '__main__':
    main()
