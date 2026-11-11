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

The shape comes from the global's declaration in its user source, a
src/shared header or a tracked native draft (native/drafts) (the MSVC name alone cannot tell `char *p` from `char p[]`).

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

SCALAR_WIDTH = {'char': 1, 'unsigned char': 1, 'signed char': 1, 'bool': 1, 'BYTE': 1, 'byte': 1, 'uchar': 1,
                'short': 2, 'unsigned short': 2, 'wchar_t': 2, 'WORD': 2, 'ushort': 2, 'int': 4, 'unsigned int': 4,
                'unsigned': 4, 'long': 4, 'unsigned long': 4, 'float': 4, 'DWORD': 4, 'UINT': 4, 'BOOL': 4,
                'LONG': 4, 'uint': 4, 'double': 8, '__int64': 8, 'unsigned __int64': 8}
SCALARS = {'char', 'signed char', 'unsigned char', 'short', 'unsigned short', 'wchar_t', 'int', 'unsigned int',
           'unsigned', 'long', 'unsigned long', 'float', 'double', 'bool', '__int64', 'unsigned __int64',
           'BYTE', 'WORD', 'DWORD', 'UINT', 'BOOL', 'LONG', 'byte', 'ushort', 'uint', 'undefined', 'undefined1',
           'undefined2', 'undefined4', 'undefined8', 'uchar', 'short int', 'long int', 'signed int',
           'unsigned short int', 'unsigned long int', 'unsigned char const', 'signed short', 'int16_t', 'uint16_t',
           'int32_t', 'uint32_t', 'int8_t', 'uint8_t'}
STRIP = re.compile(r'\b(extern|static|const|volatile|__declspec\(\w+\)|"C")\b|"C"')


def declaration(name, lines):
    """(base type, stars, is_array) of `name`'s declaration, or None. A
    multi-dimensional array (`unsigned char t[5][4]`) is an array too (Scout1098)."""
    rx = re.compile(r'^\s*(?:extern\s+"C"\s+)?(?:extern\s+)?([A-Za-z_][\w\s]*?)\s*(\*+\s*(?:const\s*)?)?\b%s\b\s*((?:\[[^\]]*\]\s*)+)?\s*(?:;|=|\))' % re.escape(name))
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
    # One declaration naming several objects (`extern char const a[], b[];`,
    # `extern double const x, y;`; Scout1177, M22): the declarator naming `name`.
    multi = re.compile(r'^\s*(?:extern\s+"C"\s+)?(?:extern\s+)?([A-Za-z_][\w\s]*?)\s+([^;(){}=]*,[^;(){}=]*);')
    for line in lines:
        if name not in line:
            continue
        m = multi.match(line)
        if not m:
            continue
        for part in m.group(2).split(','):
            d = re.match(r'^\s*(?:const\s+)?(\*+\s*(?:const\s*)?)?(\w+)\s*((?:\[[^\]]*\]\s*)+)?$', part)
            if d and d.group(2) == name:
                base = ' '.join(STRIP.sub(' ', m.group(1)).split())
                if base:
                    return base, (d.group(1) or '').count('*'), d.group(3) is not None
    return None


_SHARED = None


def shared_text():
    global _SHARED
    if _SHARED is None:
        _SHARED = '\n'.join(h.read_text(errors='replace') for h in sorted((ROOT / 'src/shared').glob('*.hpp')))
    return _SHARED


_RECOVERED = None


def struct_source(base):
    """The text holding struct `base`'s definition: src/shared, else the one body
    the recovered sources and tracked drafts give it (a struct a user source
    defines for itself, such as UnknownVoxelView; Scout1098). Two different
    bodies of one name (light views) give none."""
    global _RECOVERED
    pattern = r'\b(?:struct|class)\s+%s\s*\{([^{}]*)\};' % re.escape(base)
    if re.search(pattern, shared_text()):
        return shared_text()
    if _RECOVERED is None:
        files = sorted((ROOT / 'src/recovered').rglob('*.cpp')) + sorted((ROOT / 'native/drafts').glob('*.cpp'))
        _RECOVERED = [f.read_text(errors='replace') for f in files]
    bodies = set()
    for text in _RECOVERED:
        if base in text:
            bodies.update(' '.join(b.split()) for b in re.findall(pattern, text))
    if len(bodies) == 1:
        return 'struct %s {%s};' % (base, bodies.pop().replace('; ', ';\n'))
    return ''


def struct_members(body):
    """The member lines of a struct body, one member per line (`int X, Y;` is
    `int X;` and `int Y;`)."""
    out = []
    for line in body.split('\n'):
        line = line.split('//')[0].strip()
        m = re.match(r'^([A-Za-z_][\w\s]*?)\s+(\w+\s*(?:,\s*\w+\s*)+);$', line)
        if m:
            out.extend('%s %s;' % (m.group(1), n.strip()) for n in m.group(2).split(','))
        else:
            out.append(line)
    return out


def pointer_struct(base):
    """Number of members of struct `base` when every member is a data pointer, else 0."""
    m = re.search(r'\b(?:struct|class)\s+%s\s*\{([^{}]*)\};' % re.escape(base), struct_source(base))
    if not m:
        return 0
    members = struct_members(m.group(1))
    members = [l for l in members if l and l not in ('public:', 'private:', 'protected:')]
    if not members or not all(re.match(r'^[\w\s]+\*\s*(?:const\s+)?\w+;$', l) for l in members):
        return 0
    return len(members)


def record_native_size(fields):
    """Native size of a record of 4-byte fields ('p' = 8-byte pointer slot)."""
    off = 0
    for c in fields:
        if c == 'p':
            off = (off + 7) & ~7
            off += 8
        else:
            off += 4
    align = 8 if 'p' in fields else 4
    return (off + align - 1) & ~(align - 1)


def record_fields(base):
    """'p'/'4' per member of struct `base` when every member is a data pointer or a
    32-bit scalar (at most 15 members), else ''."""
    m = re.search(r'\b(?:struct|class)\s+%s\s*\{([^{}]*)\};' % re.escape(base), struct_source(base))
    if not m:
        return ''
    out = ''
    for line in struct_members(m.group(1)):
        if not line or line in ('public:', 'private:', 'protected:'):
            continue
        f = re.match(r'^((?:const\s+)?[A-Za-z_][\w\s]*?)\s*(\*+)?\s*(?:const\s+)?(\w+)\s*;$', line)
        if not f:
            return ''
        if f.group(2):
            out += 'p'
            continue
        t = ' '.join(f.group(1).replace('const', ' ').split())
        if t in ('int', 'unsigned int', 'unsigned', 'long', 'unsigned long', 'float', 'DWORD', 'UINT', 'BOOL', 'LONG') \
                or re.search(r'\benum\s+%s\b' % re.escape(t), shared_text()):
            out += '4'
            continue
        return ''
    return out if 0 < len(out) <= 15 else ''


def scalar_record_size(base):
    """Size of struct `base` when every member is a scalar or a fixed array of
    scalars (char/short/int/float/double, enums as int; no pointers, no member
    functions): natural alignment, which VC6 (/Zp8) and arm64 lay out alike, so
    the original bytes load as they are. Else 0."""
    m = re.search(r'\b(?:struct|class)\s+%s\s*\{([^{}]*)\};' % re.escape(base), struct_source(base))
    if not m:
        return 0
    off = 0
    align = 1
    for line in struct_members(m.group(1)):
        if not line or line in ('public:', 'private:', 'protected:'):
            continue
        f = re.match(r'^([A-Za-z_][\w\s]*?)\s+(\w+)\s*((?:\[\s*(?:0x[0-9A-Fa-f]+|\d+)\s*\])*)\s*;$', line)
        if not f:
            return 0
        t = ' '.join(f.group(1).split())
        nested = False
        if t in SCALAR_WIDTH and t not in ('long', 'unsigned long', 'LONG'):
            width = SCALAR_WIDTH[t]
        elif t in ('long', 'unsigned long', 'LONG') or re.search(r'\benum\s+%s\b' % re.escape(t), shared_text()):
            width = 4
        elif t != base and scalar_record_size(t) % 4 == 0 and scalar_record_size(t):
            width = scalar_record_size(t)    # a nested scalar record (PixelFXColor), 4-aligned
            nested = True
        else:
            return 0
        count = 1
        for dim in re.findall(r'\[\s*(0x[0-9A-Fa-f]+|\d+)\s*\]', f.group(3)):
            count *= int(dim, 0)
        step = 4 if nested else width
        off = (off + step - 1) & ~(step - 1)
        off += width * count
        align = max(align, step)
    return (off + align - 1) & ~(align - 1) if off else 0


def declared_count(name, lines):
    for line in lines:
        m = re.search(r'\b%s\s*\[\s*(\w+)\s*\]' % re.escape(name), line)
        if m:
            if m.group(1).isdigit():
                return int(m.group(1))
            c = re.search(r'\b%s\s*=\s*(\d+)\b' % re.escape(m.group(1)), shared_text())
            if c:
                return int(c.group(1))
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
    # src/ and the tracked native drafts (a draft declares the globals it adds).
    # Qualified names (OwnerDraw::unknown_7add6c, CD::RequiredCD): the TSV names
    # the global with its scope, a source declares it by its bare name inside
    # `namespace X {` (extern) or `class X {` (a static member). Their lines are
    # indexed by bare word with the scopes their file opens (Scout1121: every
    # namespaced and class-static .data/.rdata global was "no declaration found"
    # and kept a zero placeholder, e.g. the menus' dialog back art name).
    scoped = {}
    scope_rx = re.compile(r'\b(?:namespace|class|struct)\s+([A-Za-z_]\w*)\s*(?::[^;{]*)?\{')
    for f in sorted((ROOT / 'src').rglob('*')) + sorted((ROOT / 'native/drafts').glob('*.cpp')):
        if f.suffix not in ('.cpp', '.hpp', '.h', '.c'):
            continue
        text = f.read_text(errors='replace')
        scopes = None
        for line in text.splitlines():
            if 'extern' in line and ';' in line:
                for w in set(word.findall(line)):
                    index.setdefault(w, []).append(line)
            if ('extern' in line or 'static' in line) and ';' in line and '(' not in line.split(';')[0].split('//')[0].replace('__declspec(', ''):
                if scopes is None:
                    scopes = set(scope_rx.findall(text))
                if scopes:
                    for w in set(word.findall(line.split('//')[0])):
                        scoped.setdefault(w, []).append((scopes, line))

    def lines_of(name):
        """Declaration lines of `name`; a qualified name's come from files that
        open its innermost scope."""
        if '::' not in name:
            return index.get(name, [])
        scope, bare = name.rsplit('::', 1)
        scope = scope.rsplit('::', 1)[-1]
        return [line for scopes, line in scoped.get(bare, []) if scope in scopes]

    def bare(name):
        return name.rsplit('::', 1)[-1]
    # Kept scalar-array rows: extent over the declared elements (see below).
    for sym, r in rows.items():
        if r['shape'] != 'bytes':
            continue
        decl = declaration(bare(r['name']), lines_of(r['name']))
        if decl and decl[1] == 0 and decl[2] and decl[0] in SCALARS:
            count = declared_count(bare(r['name']), lines_of(r['name']))
            width = SCALAR_WIDTH.get(decl[0])
            if count and width:
                r['extent'] = max(r['extent'], width * count)
    skipped = []
    # Names data/address_aliases.tsv binds to another name's storage (one storage
    # per address): their storage is the other name's, never loaded twice.
    aliased = set()
    aliases = ROOT / 'native/data/address_aliases.tsv'
    if aliases.exists():
        for line in aliases.read_text().splitlines():
            f = line.split('\t')
            if len(f) > 3 and f[3] == 'data':
                aliased.add(f[0])
    for sym in [s for s in rows if s in aliased]:
        del rows[sym]
    # Names data/member_aliases.tsv binds to a member of a global this file stores
    # (tools/member_aliases.py; Scout1288: source_436100_1..3 inside CLSID_BombClass):
    # an assembler alias of the owner's storage plus the member's native offset,
    # emitted after the owner's storage; no storage or load of their own.
    member_of = {}
    member_tsv = ROOT / 'native/data/member_aliases.tsv'
    if member_tsv.exists():
        for line in member_tsv.read_text().splitlines():
            f = line.split('\t')
            if line and not line.startswith(('#', 'symbol\t')) and len(f) > 2:
                member_of[f[0]] = (f[1], int(f[2]))
    for line in TSV.read_text().splitlines():
        if line.startswith('#') or line.startswith('address\t'):
            continue
        addr, section, extent, sym, name, msvc, user = (line.split('\t') + [''] * 7)[:7]
        if sym in rows:
            continue
        if sym in aliased:
            skipped.append(dict(symbol=sym, address=addr, reason='address alias'))
            continue
        decl = declaration(bare(name), lines_of(name))
        if decl is None:
            skipped.append(dict(symbol=sym, address=addr, reason='no declaration found'))
            continue
        base, stars, array = decl
        extent = int(extent)
        if stars == 0 and base in SCALARS:
            shape = 'bytes'
            if not array and base in SCALAR_WIDTH:
                # One scalar: its own width (the TSV extent runs to the next known
                # symbol: SocketReceiveBuffer's 168 bytes would cover others' places).
                extent = SCALAR_WIDTH[base]
            if array:
                # A scalar array runs over its declared elements even where an
                # interior name starts (unknown_screen_widths_7add68[3]: the TSV
                # extent stops at OwnerDraw::unknown_7add6c, element 1).
                count = declared_count(bare(name), lines_of(name))
                width = SCALAR_WIDTH.get(base)
                if count and width:
                    extent = max(extent, width * count)
        elif stars == 1 and not array:
            shape = 'ptr'
        elif stars == 1 and array:
            shape = 'ptrs'
        elif stars == 0 and record_fields(base) and (array or 'p' not in record_fields(base)):
            # A struct of 4-byte fields (data pointers and 32-bit scalars):
            # without pointers it is layout-identical (bytes); with them the
            # loader converts each pointer field into an 8-byte native slot
            # (shape 3: field count and pointer mask in the high bits).
            fields = record_fields(base)
            count = declared_count(bare(name), lines_of(name)) if array else 1
            if count is None and ('p' not in fields or extent % (4 * len(fields)) == 0):
                # An unsized array of records: the TSV extent (to the next known
                # symbol) in whole records (Scout1103: DriveRawTracks/ShipRawTracks,
                # 13 tracks of a points pointer and three ints; a pointer record
                # array only when the extent is whole records).
                count = extent // (4 * len(fields))
            if not count:
                skipped.append(dict(symbol=sym, address=addr, reason='record array without a count'))
                continue
            extent = 4 * len(fields) * count
            if 'p' not in fields:
                shape = 'bytes'
            else:
                shape = 'rec:' + fields
        elif stars == 0 and array and pointer_struct(base):
            # An array of structs whose members are all data pointers (name
            # tables such as CategoryNames): an array of pointers, extent from
            # the declared count (the TSV extent stops at the next interior name).
            count = declared_count(bare(name), lines_of(name))
            if count is None:
                skipped.append(dict(symbol=sym, address=addr, reason='pointer struct array without a count'))
                continue
            shape, extent = 'ptrs', 4 * pointer_struct(base) * count
        elif stars == 0 and scalar_record_size(base):
            # A struct of scalars and scalar arrays only (TheaterTypeStruct): the
            # same layout on both targets, so its bytes load as they are. The
            # extent is the declared elements, else the TSV extent (an unsized
            # array runs to the next known symbol).
            size = scalar_record_size(base)
            count = declared_count(bare(name), lines_of(name)) if array else 1
            if count:
                extent = size * count
            shape = 'bytes'
        else:
            skipped.append(dict(symbol=sym, address=addr, reason='shape %s%s%s' % (base, '*' * stars, '[]' if array else '')))
            continue
        rows[sym] = dict(address=addr, extent=extent, shape=shape, name=name, user=user)
    # Every member alias of an owner stored here, also one whose name the previous
    # build already defined (through this file) and so no longer lists as unsourced.
    members = {}
    for sym, (owner, offset) in member_of.items():
        if owner in rows and owner != sym:
            members.setdefault(owner, []).append((sym, offset))
            rows.pop(sym, None)
    out = ['// generated by native/tools/original_data.py: storage for the initialised globals the',
           '// original stores and no permitted text supplies; ra2port::load_original_data fills them',
           '// from the user\'s installed game.exe at run time (AGENTS.md "Native phase", user decision',
           '// 2026-11-08). Addresses, extents and shapes only: no original bytes are written here.',
           '// Shapes: 0 bytes (scalars, scalar arrays), 1 one data pointer, 2 an array of data pointers,',
           '// 3 records of 4-byte fields (bits 4-7 the field count, bits 8-22 the pointer fields).',
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
            for line in lines_of(r['name']):
                m = re.search(r'\b%s\s*((?:\[\s*\d+\s*\]\s*)+)' % re.escape(bare(r['name'])), line)
                if m:
                    elements = 1
                    for dim in re.findall(r'\d+', m.group(1)):
                        elements *= int(dim)   # every dimension (unknown_road_library_7e0fe4[8][4])
                    declared[int(r['address'], 16)] = min(r['extent'], 4 * elements)
                    break
            else:
                declared.setdefault(int(r['address'], 16), r['extent'])
    by_addr = {}
    for sym, r in rows.items():
        by_addr.setdefault(int(r['address'], 16), []).append((sym, r))
    table, places = [], []
    # An array name and a single-pointer name of one address: the array covers both.
    rank = {'ptrs': 0, 'ptr': 1, 'bytes': 2}
    for k, addr in enumerate(sorted(by_addr)):
        group = sorted(by_addr[addr], key=lambda x: (rank.get(x[1]['shape'], 0), x[0]))
        shape = group[0][1]['shape']
        n = max(r['extent'] for _, r in group)
        for sym, r in group:
            out.append('// row\t%s\t%s\t%d\t%s\t%s\t%s' % (sym, r['address'], r['extent'], r['shape'], r['name'], r['user']))
        size = n if shape == 'bytes' else 2 * n
        if shape.startswith('rec:'):
            fields = shape[4:]
            size = (n // (4 * len(fields))) * record_native_size(fields)
        size = max(16, (size + 15) & ~15)
        main_sym = group[0][0]
        out.append('alignas(16) unsigned char ra2_orig_%04d[%d] __asm__("%s");' % (k, size, main_sym))
        for sym, _ in group[1:]:
            out.append('__asm__(".globl %s\\n.set %s, %s\\n");' % (sym, sym, main_sym))
        for sym, _ in group:
            for member, offset in sorted(members.get(sym, []), key=lambda x: x[1]):
                out.append('__asm__(".globl %s\\n.set %s, %s + %d\\n");  // member alias (data/member_aliases.tsv)'
                           % (member, member, sym, offset))
        if shape.startswith('rec:'):
            fields = shape[4:]
            mask = sum(1 << i for i, c in enumerate(fields) if c == 'p')
            code = '0x%Xu' % (3 | (len(fields) << 4) | (mask << 8))
        else:
            code = '%d' % {'bytes': 0, 'ptr': 1, 'ptrs': 2}[shape]
        table.append('    {ra2_orig_%04d, 0x%08Xu, %du, %s},' % (k, addr, n, code))
        # A place covers what the native storage holds: one pointer for `ptr`, the
        # declared elements for `ptrs` (the TSV extent runs to the next symbol and
        # can include the string literals that follow; pointers to those must
        # stay in the image). Pointer slots are 8 bytes natively (pointers=1).
        if shape.startswith('rec:'):
            places.append((addr, 4, 'ra2_orig_%04d' % k, 0))   # the table's start only
        elif shape == 'ptr':
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
