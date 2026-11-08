#!/usr/bin/env python3
"""Storage and a load table for the initialised data the original stores and no
permitted text supplies (data/unsourced_initialised.tsv): the port reads it from
the user's installed game.exe at run time (user decision 2026-11-08, AGENTS.md
"Native phase"). This tool writes names, addresses, extents and shapes only,
never bytes: native/data/original_data.cpp defines each global's storage (by its
symbol) and lists it for ra2port::load_original_data (port/src/original_image.cpp),
which maps game.exe's sections by virtual address (after checking the pinned
SHA-256) and fills each global before WinMain:

  * bytes:  a scalar or an array of scalars (char, short, int, float, double,
            bool, ...): the original bytes over its extent (layout-identical);
  * ptr:    one data pointer: the 32-bit address becomes a native pointer into
            the mapped image (string literals, tables), null stays null;
  * ptrs:   an array of data pointers: each element converted the same way.

A data pointer (to a scalar, a string or an object) becomes the native storage of
the global it points at (ra2_original_storage: these and data/bss_globals.cpp),
else a pointer into the mapped image's initialised data, else null.
Class-typed globals, pointers to pointers and function pointers are left out
(listed in the report): their layout is not the original's on LP64.

The shape comes from the global's declaration in its user source or a
src/shared header (the MSVC name alone cannot tell `char *p` from `char p[]`).

    python3 native/tools/original_data.py   [--report out.json]
Keeps the rows it wrote before (data_globals.py drops a defined global from the
unsourced list on its next run).
"""
import argparse
import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
TSV = ROOT / 'native/data/unsourced_initialised.tsv'
OUT = ROOT / 'native/data/original_data.cpp'

SCALARS = {'char', 'signed char', 'unsigned char', 'short', 'unsigned short', 'wchar_t', 'int', 'unsigned int',
           'unsigned', 'long', 'unsigned long', 'float', 'double', 'bool', '__int64', 'unsigned __int64',
           'BYTE', 'WORD', 'DWORD', 'UINT', 'BOOL', 'LONG', 'byte', 'ushort', 'uint', 'undefined', 'undefined1',
           'undefined2', 'undefined4', 'undefined8', 'uchar', 'short int', 'long int', 'signed int',
           'unsigned short int', 'unsigned long int', 'unsigned char const', 'signed short', 'int16_t', 'uint16_t',
           'int32_t', 'uint32_t', 'int8_t', 'uint8_t'}
STRIP = re.compile(r'\b(extern|static|const|volatile|__declspec\(\w+\)|"C")\b|"C"')


def declaration(name, lines):
    """(base type, stars, is_array) of `name`'s declaration, or None."""
    rx = re.compile(r'^\s*(?:extern\s+"C"\s+)?(?:extern\s+)?([A-Za-z_][\w\s]*?)\s*(\*+\s*(?:const\s*)?)?\b%s\b\s*(\[[^\]]*\])?\s*(?:;|=|\))' % re.escape(name))
    for text in [lines]:
        for line in text:
            if name not in line or '(' in line.split(name)[0]:
                continue
            m = rx.match(line)
            if not m:
                continue
            base = ' '.join(STRIP.sub(' ', m.group(1)).split())
            if not base or base in ('return', 'goto', 'case'):
                continue
            stars = (m.group(2) or '').count('*')
            return base, stars, m.group(3) is not None
    return None


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--report', type=Path)
    ap.add_argument('--fresh', action='store_true', help='ignore the rows written before')
    args = ap.parse_args()
    rows = {}
    if OUT.exists() and not args.fresh:
        for m in re.finditer(r'^// row\t(.*)$', OUT.read_text(), re.M):
            r = m.group(1).split('\t')
            rows[r[0]] = dict(address=r[1], extent=int(r[2]), shape=r[3], name=r[4], user=r[5])
    # Every source line that declares something extern (one pass over src/).
    index = {}
    word = re.compile(r'[A-Za-z_]\w*')
    for f in sorted((ROOT / 'src').rglob('*')):
        if f.suffix not in ('.cpp', '.hpp', '.h', '.c'):
            continue
        for line in f.read_text(errors='replace').splitlines():
            if 'extern' in line and ';' in line:
                for w in set(word.findall(line)):
                    index.setdefault(w, []).append(line)
    skipped = []
    for line in TSV.read_text().splitlines():
        if line.startswith('#') or line.startswith('address\t'):
            continue
        addr, section, extent, sym, name, msvc, user = (line.split('\t') + [''] * 7)[:7]
        if sym in rows:
            continue
        decl = declaration(name, index.get(name, []))
        if decl is None:
            skipped.append(dict(symbol=sym, address=addr, reason='no declaration found'))
            continue
        base, stars, array = decl
        extent = int(extent)
        if stars == 0 and base in SCALARS:
            shape = 'bytes'
        elif stars == 1 and not array:
            shape = 'ptr'
        elif stars == 1 and array:
            shape = 'ptrs'
        else:
            skipped.append(dict(symbol=sym, address=addr, reason='shape %s%s%s' % (base, '*' * stars, '[]' if array else '')))
            continue
        rows[sym] = dict(address=addr, extent=extent, shape=shape, name=name, user=user)
    out = ['// generated by native/tools/original_data.py: storage for the initialised globals the',
           '// original stores and no permitted text supplies; ra2port::load_original_data fills them',
           '// from the user\'s installed game.exe at run time (AGENTS.md "Native phase", user decision',
           '// 2026-11-08). Addresses, extents and shapes only: no original bytes are written here.',
           '// Shapes: 0 bytes (scalars, scalar arrays), 1 one data pointer, 2 an array of data pointers.',
           '// One storage per original address (other names of it are assembler aliases).',
           '// ra2_original_storage maps original addresses to native storage (these and',
           '// data/bss_globals.cpp), so a stored pointer to a global becomes a pointer to',
           '// its native storage.',
           '#include <stddef.h>',
           '#include <stdint.h>',
           'struct ra2_original_global { void *storage; uint32_t address; uint32_t extent; uint32_t shape; };',
           'struct ra2_original_place { uint32_t address; uint32_t extent; void *storage; uint32_t pointers; };',
           '']
    declared = {}
    for sym, r in rows.items():
        if r['shape'] == 'ptrs':
            for line in index.get(r['name'], []):
                m = re.search(r'\b%s\s*\[\s*(\d+)\s*\]' % re.escape(r['name']), line)
                if m:
                    declared[int(r['address'], 16)] = min(r['extent'], 4 * int(m.group(1)))
                    break
    by_addr = {}
    for sym, r in rows.items():
        by_addr.setdefault(int(r['address'], 16), []).append((sym, r))
    table, places = [], []
    rank = {'ptr': 0, 'ptrs': 1, 'bytes': 2}
    for k, addr in enumerate(sorted(by_addr)):
        group = sorted(by_addr[addr], key=lambda x: (rank[x[1]['shape']], x[0]))
        shape = group[0][1]['shape']
        n = max(r['extent'] for _, r in group)
        for sym, r in group:
            out.append('// row\t%s\t%s\t%d\t%s\t%s\t%s' % (sym, r['address'], r['extent'], r['shape'], r['name'], r['user']))
        size = n if shape == 'bytes' else 2 * n
        size = max(16, (size + 15) & ~15)
        main_sym = group[0][0]
        out.append('alignas(16) unsigned char ra2_orig_%04d[%d] __asm__("%s");' % (k, size, main_sym))
        for sym, _ in group[1:]:
            out.append('__asm__(".globl %s\\n.set %s, %s\\n");' % (sym, sym, main_sym))
        table.append('    {ra2_orig_%04d, 0x%08Xu, %du, %d},' % (k, addr, n, {'bytes': 0, 'ptr': 1, 'ptrs': 2}[shape]))
        # A place covers what the native storage holds: one pointer for `ptr`, the
        # declared elements for `ptrs` (the TSV extent runs to the next symbol and
        # can include the string literals that follow; pointers to those must
        # stay in the image). Pointer slots are 8 bytes natively (pointers=1).
        if shape == 'ptr':
            places.append((addr, 4, 'ra2_orig_%04d' % k, 1))
        elif shape == 'ptrs':
            places.append((addr, declared.get(addr, n), 'ra2_orig_%04d' % k, 1))
        else:
            places.append((addr, n, 'ra2_orig_%04d' % k, 0))
    bss = re.compile(r'^alignas\(\d+\) unsigned char (ra2_bss_\d+)\[(\d+)\] __asm__\("([^"]+)"\);\s*// 0x([0-9A-Fa-f]{8})')
    for line in (ROOT / 'native/data/bss_globals.cpp').read_text().splitlines():
        m = bss.match(line)
        if m and int(m.group(4), 16) not in by_addr:
            k = len(places)
            out.append('extern "C" unsigned char ra2_place_%04d[] __asm__("%s");' % (k, m.group(3)))
            places.append((int(m.group(4), 16), int(m.group(2)) // 2, 'ra2_place_%04d' % k, 0))
    out.append('')
    out.append('extern "C" const ra2_original_global ra2_original_globals[] = {')
    out += table
    out.append('    {0, 0u, 0u, 0},')
    out.append('};')
    out.append('extern "C" const ra2_original_place ra2_original_storage[] = {')
    for addr, n, name, pointers in sorted(places):
        out.append('    {0x%08Xu, %du, %s, %d},' % (addr, max(1, n), name, pointers))
    out.append('    {0u, 0u, 0, 0},')
    out.append('};')
    OUT.write_text('\n'.join(out) + '\n')
    shapes = {}
    for r in rows.values():
        shapes[r['shape']] = shapes.get(r['shape'], 0) + 1
    print('original data: %d globals (%s), %d left out' % (len(rows), ', '.join('%d %s' % (v, k) for k, v in sorted(shapes.items())), len(skipped)))
    if args.report:
        args.report.write_text(json.dumps(dict(rows=len(rows), shapes=shapes, skipped=skipped), indent=1))


if __name__ == '__main__':
    main()
