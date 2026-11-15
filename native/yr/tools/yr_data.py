#!/usr/bin/env python3
"""Storage and a load table for the YR globals the tree declares but no source defines (lane Y2).

The YR tree only declares most of its globals (`extern RulesClass *Rule;`, `DAT_007E1708`): the
original keeps them in gamemd.exe's data. For each such data symbol a native build references,
this writes (OUT, a C++ file; names, addresses and extents only, never bytes):

  * its storage, under its exact symbol name, sized for the native layout; and
  * a row of ra2_original_globals, which the port (load_original_data,
    native/port/src/original_image.cpp) fills from the user's gamemd.exe before the YR static
    initialisers run (native/yr/tools/defer_initializers.py):
      - bytes: scalars and arrays of scalars, and structures without pointers (GUIDs): the
        original bytes over the extent (layout-identical);
      - ptr / ptrs: one data pointer or an array of them: each 32-bit address becomes a native
        pointer (the storage of the global there, else the mapped image's data);
    and a row of ra2_original_storage (every global's storage by original address, so pointers
    to globals reach the native object).
  * .bss globals (past .data's file-backed part) get zero storage only. Class-typed globals and
    pointers to pointers get zero storage (twice the original extent: LP64 at most doubles a
    layout) and are listed: their layout is not the original's natively.

The address is the one in the name (DAT_007E1708, PTR_..._0081A3C4) or native/yr/data/globals.tsv
(derived from matched code, yr_pins.py). The declaration (type, pointer depth, array bounds) comes
from the generated YR units and headers (native/tools/original_data.py's reader). The extent of an
array without a bound, or of a global whose type is not read, is the distance to the next known
global (capped).

    python3 native/yr/tools/yr_data.py --inputs <link_inputs.txt> --host <objects> --gen <gen-yr> --out <file.cpp>
"""
import argparse
import bisect
import json
import re
import subprocess
import sys
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / 'native/tools'))
import link_closure   # noqa: E402
import original_data  # noqa: E402

GLOBALS = ROOT / 'native/yr/data/globals.tsv'
RDATA = (0x7E1000, 0x811074)
DATA = (0x812000, 0x87E000, 0xB79BE4)     # start, end of the file-backed part, end
CAP = 0x10000
POD_STRUCTS = {'GUID', 'IID', 'CLSID', '_GUID', 'RECT', 'POINT', 'tagRECT', 'tagPOINT',
               'TheaterDataType',   # theater.h: chars, floats and ints only (lane Y3)
               'VoxelRenderOrientation'}   # voxdrsys.h: nine ints (lane Y3)
POD_SIZES = {'TheaterDataType': 0x70, 'VoxelRenderOrientation': 36}
ADDRESS = re.compile(r'_([0-9A-Fa-f]{8})$')


def known_addresses():
    by_name = {}
    for line in GLOBALS.read_text().splitlines():
        if not line or line.startswith('#') or line.startswith('name\t'):
            continue
        name, address = line.split('\t')[:2]
        by_name[name] = int(address, 16)
    return by_name


def text_index(gen):
    """Lines of the generated units and headers, by identifier they mention (declarations only)."""
    index = defaultdict(list)
    word = re.compile(r'[A-Za-z_]\w*')
    for path in list((gen / 'hdr').rglob('*')) + list((gen / 'units').glob('*.cpp')):
        if not path.is_file():
            continue
        for line in path.read_text(encoding='latin-1').splitlines():
            if 'extern' not in line and ';' not in line:
                continue
            for w in set(word.findall(line)):
                index[w].append(line)
    return index


def enum_values(gen):
    """The enumerators of the generated headers and units with their values (THEATER_COUNT is 6,
    wave.cpp's _RADIUS_TABLE_SIZE 300), for array bounds written with an enumerator
    (`Theaters[THEATER_COUNT]`, `RadiusTable[_RADIUS_TABLE_SIZE][_RADIUS_TABLE_SIZE]`; lane Y3)."""
    values = {}
    for path in list((gen / 'hdr').rglob('*')) + list((gen / 'units').glob('*.cpp')):
        if not path.is_file():
            continue
        text = re.sub(r'//[^\n]*|/\*.*?\*/', '', path.read_text(encoding='latin-1'), flags=re.S)
        for body in re.findall(r'\benum\s+\w*\s*(?::\s*\w+\s*)?\{([^}]*)\}', text):
            nxt = 0
            for item in body.split(','):
                m = re.match(r'\s*(\w+)\s*(?:=\s*(.+?))?\s*$', item)
                if not m:
                    continue
                if m.group(2) is not None:
                    v = m.group(2).strip()
                    try:
                        nxt = int(v, 0)
                    except ValueError:
                        if v not in values:
                            continue
                        nxt = values[v]
                values.setdefault(m.group(1), nxt)
                nxt += 1
    return values


ENUM_VALUES = {}


def dims(name, lines):
    """The array bounds of `name`'s declaration (numbers or enumerators), or None when it is not read."""
    rx = re.compile(r'\b%s\b\s*((?:\[\s*\w*\s*\]\s*)+)' % re.escape(name))
    for line in lines:
        m = rx.search(line)
        if m:
            bounds = re.findall(r'\[\s*(\w*)\s*\]', m.group(1))
            bounds = [str(ENUM_VALUES[b]) if b in ENUM_VALUES else b for b in bounds]
            if all(b.isdigit() for b in bounds):
                out = 1
                for b in bounds:
                    out *= int(b)
                return out
            return None
    return None


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--inputs', type=Path, required=True)
    ap.add_argument('--host', nargs='*', default=[])
    ap.add_argument('--gen', type=Path, required=True)
    ap.add_argument('--out', type=Path, required=True)
    ap.add_argument('--aliases', type=Path, help='yr_aliases.py output: address names bound elsewhere (no storage here)')
    ap.add_argument('--system-libs', nargs='*', default=[])
    ap.add_argument('--frameworks', nargs='*', default=[])
    a = ap.parse_args()
    paths = [l.strip() for l in a.inputs.read_text().splitlines() if l.strip()] + [str(p) for p in a.host]
    paths = [p for p in paths if not p.endswith(a.out.with_suffix('.cpp.o').name)]
    system = link_closure.system_exports(frameworks=a.frameworks, libs=a.system_libs)
    link_closure.DATA_FROM = RDATA[0]
    missing, _ = link_closure.missing_symbols(paths, system=system)
    rows = link_closure.classify(missing)
    known = known_addresses()
    bound = set()
    if a.aliases and a.aliases.exists():
        bound = {l.split('\t')[0] for l in a.aliases.read_text().splitlines() if l and not l.startswith(('#', 'alias\t'))}
    rows = [r for r in rows if r['symbol'] not in bound]
    demangled = {r['symbol']: r['name'] for r in rows}
    wanted = []
    for r in rows:
        name = demangled[r['symbol']]
        m = ADDRESS.search(name)
        address = known.get(name)
        if address is None and m and int(m.group(1), 16) >= RDATA[0]:
            address = int(m.group(1), 16)
        if address is None or address < RDATA[0] or address >= DATA[2]:
            continue
        wanted.append((address, r['symbol'], name))
    wanted.sort()
    starts = sorted(set(known.values()) | {w[0] for w in wanted})
    index = text_index(a.gen)
    ENUM_VALUES.update(enum_values(a.gen))
    covers = []                  # (start, end, symbol) of storage with a declared extent
    # Enumerations are loaded as their 32-bit value (VC6 and the native build both give an
    # enum int width): LastTheater starts at THEATER_NONE, not zero (lane Y3).
    enums = set()
    for path in (a.gen / 'hdr').rglob('*'):
        if path.is_file():
            enums.update(re.findall(r'\benum\s+(\w+)\s*(?::\s*\w+\s*)?\{', path.read_text(encoding='latin-1')))
    out, places, report = [], [], defaultdict(list)
    defs = []
    primary = {}                 # address -> the storage variable of the first name there
    aliases = []
    for n, (address, symbol, name) in enumerate(wanted):
        if address in primary:
            # One original address, several names (VisibleSurface and DAT_00887308): one storage.
            aliases.append('__asm__(".globl %s\\n%s = %s");   // %s, the same address' % (symbol, symbol, primary[address], name))
            continue
        inside = next((c for c in covers if c[0] < address < c[1]), None)
        if inside:
            # A name inside a declared array (DAT_007E1BD0 is Theaters[0]'s radar brightness):
            # an offset into that array's storage, not a copy of its own (lane Y3).
            aliases.append('__asm__(".globl %s\\n%s = %s + %d");   // %s, inside %s' % (
                symbol, symbol, inside[2], address - inside[0], name, inside[2]))
            continue
        primary[address] = symbol
        short = name.split('::')[-1]
        lines = index.get(short, [])
        decl = original_data.declaration(short, lines)
        if decl is None and re.match(r'(?:IID|CLSID|LIBID|FMTID)_\w+$', short):
            decl = ('GUID', 0, False)     # declared by the Win32 headers (objidl.h: EXTERN_C const IID)
        nxt = starts[bisect.bisect_right(starts, address)] if bisect.bisect_right(starts, address) < len(starts) else address + 4
        gap = min(nxt - address, CAP)
        bss = DATA[1] <= address < DATA[2]
        shape, extent, storage = None, gap, 2 * gap
        if decl:
            base, stars, is_array = decl
            # The COM headers declare `EXTERN_C const CLSID CLSID_X;` (isun.h): the macro and the
            # qualifiers are not part of the type.
            base = ' '.join(w for w in base.split() if w not in ('EXTERN_C', 'const', 'volatile', 'static'))
            count = dims(short, lines) if is_array else 1
            if stars == 1:
                extent = 4 * (count if count else gap // 4)
                shape, storage = (1 if not is_array else 2), 2 * extent
            elif stars == 0 and base in original_data.SCALARS:
                width = original_data.SCALAR_WIDTH.get(base, 4)
                extent = width * count if count else gap
                shape, storage = 0, extent
            elif stars == 0 and base in enums:
                extent = 4 * count if count else gap
                shape, storage = 0, extent
            elif stars == 0 and base in POD_STRUCTS:
                extent = 16 * count if base in ('GUID', 'IID', 'CLSID', '_GUID') and count else (gap if not count else extent)
                if base in POD_SIZES and count:
                    extent = POD_SIZES[base] * count
                    covers.append((address, address + extent, symbol))
                shape, storage = 0, extent
        if storage <= 0:
            storage = 8
        var = 'yr_g%d' % n
        defs.append('alignas(16) unsigned char %s[%d] __asm__("%s");   // %s 0x%08X%s' % (
            var, storage, symbol, name, address, '' if shape is not None else ' (not loaded: %s)' % (
                'bss' if bss else 'type not read or not portable')))
        places.append((address, extent, var, 1 if shape in (1, 2) else 0))
        if bss:
            report['bss'].append(name)
            continue
        if shape is None:
            report['zero'].append(name)
            continue
        out.append('    {%s, 0x%08X, %d, %d},   // %s' % (var, address, extent, shape, name))
        report['loaded'].append(name)
    text = ['// Generated by native/yr/tools/yr_data.py (lane Y2): storage for the YR globals no source defines',
            '// and the table the port fills from the user\'s gamemd.exe (names, addresses, extents; no bytes).',
            '#include <stdint.h>',
            'struct ra2_original_global { void *storage; uint32_t address; uint32_t extent; uint32_t shape; };',
            'struct ra2_original_place { uint32_t address; uint32_t extent; void *storage; uint32_t pointers; };',
            'extern "C" {'] + defs + aliases + [
            'extern const ra2_original_global ra2_original_globals[] = {'] + out + ['    {0, 0, 0, 0}};',
            'extern const ra2_original_place ra2_original_storage[] = {'] + [
            '    {0x%08X, %d, %s, %d},' % p for p in sorted(places)] + ['    {0, 0, 0, 0}};', '}', '']
    a.out.parent.mkdir(parents=True, exist_ok=True)
    a.out.write_text('\n'.join(text))
    a.out.with_suffix('.json').write_text(json.dumps({k: sorted(v) for k, v in report.items()}, indent=1) + '\n')
    print('yr_data: %d globals defined (%d loaded from gamemd.exe, %d .bss, %d zero)' % (
        len(wanted), len(report['loaded']), len(report['bss']), len(report['zero'])))


if __name__ == '__main__':
    main()
