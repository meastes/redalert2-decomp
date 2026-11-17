#!/usr/bin/env python3
"""Field maps from the original game's i686 object layouts to the port's arm64 ones (lane C6).

The original's saves hold VC6 (i686) object images: AbstractClass::Load 0x00410120
reads Size_Of() bytes over the object, other loads read sizeof(*this). The port's
objects have arm64 layouts (8-byte pointers and vtable pointers, other padding).
For each type read from a save (data/original_save_types.txt) this tool takes the
clang record layouts of the generated unit that defines its Load, once for
i686-pc-windows-msvc (VC6's layout: the overlay headers' i686 branches) and once
for the native arm64 dialect, pairs the two layouts' leaf members by their path
(bases, members, array elements) and writes a list of copy operations:

  COPY   n bytes as they are (same-size scalars, arrays of them, unions without pointers)
  WIDEN  n 4-byte values to 8 bytes (pointers, zero-extended; intptr_t, sign-extended)
  BITS   a bit-field (VC6's allocation unit, its bit range in each layout)

Vtable pointers are not copied (the object keeps its own; the loads rebuild them
with their NoInit constructors anyway), nor are members only one layout has
(reported). The output is a C++ table read by port/src/original_save.cpp.

    python3 native/tools/original_layouts.py --build <native build> --out <file.cpp> [--report r.json] [-j N]
        [--target windows --cxx <llvm-mingw x86_64-w64-mingw32-clang++>]

The x86_64 Windows build (lane C15) pairs the i686 layouts with its own (LLP64: long is
4 bytes as on VC6; pointers and the pointer-width rewrites, long long there, 8).

--game yr (lane Y10): the same for Yuri's Revenge (gamemd.exe's saves): the YR units
(build/gen-yr/units, native/yr/data/original_save_types.txt) with the YR build's flags; for
i686 the rewritten mingw-w64 headers take VC6's long (yr_i686_support). The classes' vtables
are not known here (no gamemd.exe manifest): VPTR_NAMED operations name each owning class by
its MSVC RTTI type descriptor name and the port finds the vtable in gamemd.exe at run time.
"""
import argparse
import difflib
import json
import re
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[1]
PRELUDE = ROOT / 'native/port/include/ra2port/prelude.hpp'
TYPES = ROOT / 'native/data/original_save_types.txt'
# The native build's dialect (CMakeLists.txt ra2_dialect), as member_aliases.py/view_audit.py use it.
DIALECT = ['-std=c++17', '-fms-extensions', '-fms-compatibility', '-fdelayed-template-parsing', '-Xclang', '-fno-wchar',
           '-fshort-wchar', '-fwrapv', '-fno-strict-aliasing', '-mms-bitfields', '-Wno-incompatible-ms-struct',
           '-fwritable-strings', '-w', '-fsyntax-only', '-Xclang', '-fdump-record-layouts-complete',
           '-Xclang', '-fdump-record-layouts-canonical']
# The native side is the arm64 game (macOS) or the x86_64 Windows one (lane C15: LLP64, so
# long is 4 bytes and the pointer-width rewrites are long long; compiled by the build's own
# llvm-mingw clang++ with its forced dialect header, as cmake/windows.cmake does).
WIN_DIALECT = ROOT / 'native/platform/win/dialect_win.hpp'
TRIPLES = {'i686': ['-target', 'i686-pc-windows-msvc', '-DRA2PORT_ALLOW_HOST'], 'arm64': ['-arch', 'arm64'],
           'win64': ['-include', str(WIN_DIALECT)]}
NATIVE = 'arm64'          # --target windows: 'win64'
CXX = {'i686': 'clang++', 'arm64': 'clang++', 'win64': 'clang++'}
# Builtin sizes per target (canonical types: typedefs resolved).
SCALAR = {'char': 1, 'signed char': 1, 'unsigned char': 1, '_Bool': 1, 'bool': 1,
          'short': 2, 'unsigned short': 2, 'wchar_t': 2, 'char16_t': 2,
          'int': 4, 'unsigned int': 4, 'float': 4, 'char32_t': 4,
          'long long': 8, 'unsigned long long': 8, 'double': 8, 'long double': 8}
LONG = {'i686': 4, 'arm64': 8, 'win64': 4}
KEYWORDS = re.compile(r'\b(class|struct|union|enum)\s+')


def canonical(name):
    """The runtime's spelling (original_save.cpp canonical()): no record keywords, no spaces."""
    return KEYWORDS.sub('', name).replace(' ', '')


class Node:
    __slots__ = ('offset', 'bits', 'label', 'type', 'name', 'children')

    def __init__(self, offset, bits, label, type_, name):
        self.offset, self.bits, self.label, self.type, self.name = offset, bits, label, type_, name
        self.children = []


class Record:
    def __init__(self, name, kind, root):
        self.name, self.kind, self.root = name, kind, root
        self.size = None


LINE = re.compile(r'^\s*(\d+)(?::(\d+)-(\d+))?\s*\|(\s*)(.*?)\s*$')


def parse(text):
    """{canonical name: Record} from -fdump-record-layouts output."""
    out = {}
    for block in text.split('*** Dumping AST Record Layout')[1:]:
        lines = [l for l in block.split('\n') if '|' in l]
        if not lines:
            continue
        head = LINE.match(lines[0])
        if not head:
            continue
        hm = re.match(r'(class|struct|union)\s+(.*?)(?:\s+\(empty\))?$', head.group(5))
        if not hm:
            continue
        rec = Record(canonical(hm.group(2)), hm.group(1), Node(0, None, 'record', head.group(5), ''))
        stack = [(-1, rec.root)]
        for l in lines[1:]:
            sm = re.search(r'\[sizeof=(\d+)', l)
            if sm:
                rec.size = int(sm.group(1))
                continue
            m = LINE.match(l)
            if not m or not m.group(5):
                continue
            depth = (len(m.group(4)) - 1) // 2
            body = m.group(5)
            bits = (int(m.group(2)), int(m.group(3))) if m.group(2) is not None else None
            if re.search(r'\((?:\S+ )?v[fb]?table pointer\)$', body) or 'vftable pointer' in body:
                node = Node(int(m.group(1)), None, 'vptr', '', '')
            elif re.search(r'\((primary |virtual )?base\)$', body):
                t = re.sub(r'\s*\((primary |virtual )?base\)$', '', body)
                node = Node(int(m.group(1)), None, 'base', t, 'base:' + canonical(t))
            elif bits and re.fullmatch(r'(?:(?:un)?signed\s+)?(?:char|short|int|long|long long|_Bool|bool)', body.strip()):
                continue                # an unnamed bit-field (VC6 padding: `unsigned char : 8`)
            elif '(anonymous at ' in body and body.rstrip().endswith(')'):
                node = Node(int(m.group(1)), bits, 'field', body, '')
            else:
                fm = re.match(r'(.*?)\s*([A-Za-z_]\w*)$', body)
                if not fm:
                    continue
                node = Node(int(m.group(1)), bits, 'field', fm.group(1).strip(), fm.group(2))
            while stack and stack[-1][0] >= depth:
                stack.pop()
            stack[-1][1].children.append(node)
            stack.append((depth, node))
        if rec.size is not None:
            out[rec.name] = rec
    return out


class Leaf:
    __slots__ = ('path', 'offset', 'kind', 'size', 'count', 'bits', 'unit', 'signed', 'type')

    def __init__(self, path, offset, kind, size, count=1, bits=None, unit=0, signed=False, type_=''):
        self.path, self.offset, self.kind, self.size, self.count = path, offset, kind, size, count
        self.bits, self.unit, self.signed, self.type = bits, unit, signed, type_


def split_array(t):
    dims = [int(d) for d in re.findall(r'\[(\d+)\]', t)]
    base = re.sub(r'\[\d+\]', '', t).strip() if dims else t
    n = 1
    for d in dims:
        n *= d
    return base, n, bool(dims)


def is_pointer(t):
    t = t.strip()
    if '::*' in t:
        return False
    return t.endswith('*') or '(*)' in t or '(**)' in t or t.endswith('&')


class Flattener:
    def __init__(self, recs, triple, problems):
        self.recs, self.triple, self.problems = recs, triple, problems

    def scalar_size(self, t):
        t = re.sub(r'\b(const|volatile)\b', '', t).strip()
        t = ' '.join(t.split())
        if t in ('long', 'unsigned long'):
            return LONG[self.triple]
        return SCALAR.get(t)

    def leaves(self, node, base, prefix, out, owner='', owner_base=0):
        """Leaves under node; the dump's offsets are from the start of the record the
        block dumps, which sits at base. owner is the complete object the node lies in
        (the written type, or a member object's type) and owner_base its offset: an
        i686 vtable pointer is a 'vptr' leaf naming that class and the pointer's offset
        in it (the original's vtable address there, for writing: lane C17)."""
        anonymous = 0
        for child in node.children:
            off = base + child.offset
            if child.label == 'vptr':
                if self.triple == 'i686' and owner:
                    out.append(Leaf(prefix + '(vptr %d)' % (off - owner_base), off, 'vptr', 4,
                                    unit=off - owner_base, type_=owner))
                continue
            if child.label == 'base':
                self.leaves(child, base, prefix + child.name + '.', out, owner, owner_base)
                continue
            name = child.name
            if not name:
                name = '(anonymous %d)' % anonymous     # by order: the offsets differ between layouts
                anonymous += 1
            path = prefix + name
            t = child.type
            if child.children:
                if t.startswith('union ') or t.startswith('const union '):
                    out.append(self.union_leaf(path, off, t, child))
                else:
                    # A member object laid out inline: its own complete object (vtable pointers).
                    mtype = raw_name(t)
                    self.leaves(child, base, path + '.', out, mtype, off)
                continue
            if child.bits:
                unit = self.scalar_size(t) or 4
                out.append(Leaf(path, off, 'bits', unit, bits=child.bits, unit=unit, type_=t))
                continue
            self.typed(path, off, t, out)

    def union_leaf(self, path, off, t, node):
        rec = self.recs.get(canonical(t))
        size = rec.size if rec else self.extent(node)
        pointer = any(c.label == 'field' and is_pointer(c.type) for c in node.children)
        return Leaf(path, off, 'union', size, signed=pointer, type_=t)

    def extent(self, node):
        """Bytes a member (an anonymous union or struct the dump does not lay out on its
        own) covers from its offset: the furthest end of its members."""
        end = 0
        for c in node.children:
            if c.label == 'vptr':
                size = 4 if self.triple == 'i686' else 8
            elif c.children:
                size = self.extent(c)
            elif c.bits:
                size = self.scalar_size(c.type) or 4
            else:
                base, n, _ = split_array(c.type)
                if is_pointer(base):
                    size = (4 if self.triple == 'i686' else 8) * n
                else:
                    one = self.scalar_size(base)
                    if one is None:
                        rec = self.recs.get(canonical(re.sub(r'\b(const|volatile)\b', '', base).strip()))
                        one = rec.size if rec else 4
                    size = one * n
            end = max(end, c.offset - node.offset + size)
        return end

    def typed(self, path, off, t, out):
        base, n, array = split_array(t)
        if '::*' in base:
            out.append(Leaf(path, off, 'unsupported', None, n, type_=t))
            return
        if is_pointer(base):
            out.append(Leaf(path, off, 'pointer', 4 if self.triple == 'i686' else 8, n, type_=t))
            return
        size = self.scalar_size(base)
        if size is not None:
            plain = re.sub(r'\b(const|volatile)\b', '', base).strip()
            kind = 'long' if plain in ('long', 'unsigned long') else 'scalar'
            out.append(Leaf(path, off, kind, size, n, signed=plain == 'long', type_=t))
            return
        if base.startswith('enum ') or base.startswith('const enum '):
            out.append(Leaf(path, off, 'enum', None, n, type_=t))
            return
        rec = self.recs.get(canonical(re.sub(r'\b(const|volatile)\b', '', base).strip()))
        if rec is None:
            out.append(Leaf(path, off, 'unknown', None, n, type_=t))
            return
        if rec.kind == 'union':
            for k in range(n):
                out.append(self.union_leaf(path + ('[%d]' % k if array else ''), off + k * rec.size, base, rec.root))
            return
        for k in range(n):
            self.leaves(rec.root, off + k * rec.size, path + ('[%d].' % k if array else '.'), out,
                        raw_name(rec.root.type), off + k * rec.size)


def raw_name(t):
    """A record's name as the dump spells it, without its keyword, outer cv or '(empty)': the
    vtable owner's name (spaces kept, for MSVC mangling: 'DynamicVectorClass<const char *>')."""
    t = re.sub(r'\s*\(empty\)$', '', t.strip())
    t = re.sub(r'^(?:(?:const|volatile)\s+)*', '', t)
    return KEYWORDS.sub('', t, count=1) if re.match(r'(class|struct|union|enum)\s', t) else t


def flatten(recs, name, triple, problems):
    rec = recs[name]
    out = []
    Flattener(recs, triple, problems).leaves(rec.root, 0, '', out, raw_name(rec.root.type), 0)
    out.sort(key=lambda l: l.offset)
    # Sizes the type does not give (enums): up to the next leaf or the record's end, at most 4.
    for i, l in enumerate(out):
        if l.size is None and l.kind in ('enum', 'unknown'):
            nxt = out[i + 1].offset if i + 1 < len(out) else rec.size
            per = (nxt - l.offset) // max(l.count, 1)
            l.size = min(4, per) if l.kind == 'enum' else per
    return out


def ops_for(recs_i, recs_a, name, problems):
    """[(kind, src, dst, n, unit, sbit, dbit, width)] for one type, or None."""
    li = flatten(recs_i, name, 'i686', problems)
    la = flatten(recs_a, name, NATIVE, problems)
    by_path = {l.path: l for l in la}
    ops, unmatched = [], []
    for l in li:
        if l.kind == 'vptr':
            # The original's vtable address for this pointer (written; a read keeps the
            # object's own): the RTTI record of the owning class with this object offset.
            # n: the owning class's primary vtable (its RTTI record at object offset 0); dst:
            # the pointer's offset in that object. The port finds the vtable for that offset
            # from the primary one's RTTI type descriptor in game.exe at run time (the
            # manifest lists only some secondary vtables, e.g. not INoticeSink/INoticeSource).
            if GAME == 'yr':
                td = msvc_type_descriptor(l.type, recs_i)
                if td is None:
                    MISSING_VTABLES.add('%s+%d' % (l.type, l.unit))
                    ops.append(('VPTR', l.offset, l.unit, 0, 0, 0, 0, 0, 0))
                else:
                    ops.append(('VPTR_NAMED', l.offset, l.unit, vtable_name_index(td), 0, 0, 0, 0, 0))
                continue
            va = rtti_vtable(l.type, 0, name)
            if not va:
                MISSING_VTABLES.add('%s+%d' % (l.type, l.unit))
            ops.append(('VPTR', l.offset, l.unit, va, 0, 0, 0, 0, 0))
            continue
        a = by_path.pop(l.path, None)
        if a is None:
            unmatched.append(('i686 only', l.path, l.type))
            continue
        if l.kind == 'bits' or a.kind == 'bits':
            if l.kind != a.kind:
                unmatched.append(('bit-field in one layout', l.path, l.type))
                continue
            lo, hi = l.bits
            alo, ahi = a.bits
            ops.append(('BITS', l.offset, a.offset, 1, l.unit, lo, alo, hi - lo + 1, a.unit))
            continue
        if l.kind == 'pointer' or (l.kind == 'long' and l.size == 4 and a.size == 8):
            if a.kind not in ('pointer', 'long'):
                unmatched.append(('pointer in one layout', l.path, l.type))
                continue
            # POINTER: widened on reading as WIDEN; written as a swizzle id (lane C17).
            kind = 'POINTER' if l.kind == 'pointer' and a.kind == 'pointer' else ('WIDEN_S' if l.signed else 'WIDEN')
            ops.append((kind, l.offset, a.offset, l.count, 0, 0, 0, 0, 0))
            continue
        if l.kind == 'union':
            if l.size == a.size:
                ops.append(('COPY', l.offset, a.offset, l.size, 0, 0, 0, 0, 0))
            elif l.signed and l.size == 4 and a.size == 8:
                ops.append(('WIDEN', l.offset, a.offset, 1, 0, 0, 0, 0, 0))
            else:
                problems.append('%s: union %s %s/%s bytes: copied %d' % (name, l.path, l.size, a.size, min(l.size or 0, a.size or 0)))
                ops.append(('COPY', l.offset, a.offset, min(l.size or 0, a.size or 0), 0, 0, 0, 0, 0))
            continue
        if l.kind == 'unsupported' or l.size is None or a.size is None:
            unmatched.append(('unsupported', l.path, l.type))
            continue
        if l.size != a.size:
            unmatched.append(('size %d/%d' % (l.size, a.size), l.path, l.type))
            continue
        ops.append(('COPY', l.offset, a.offset, l.size * l.count, 0, 0, 0, 0, 0))
    for p, a in by_path.items():
        unmatched.append(('arm64 only', p, a.type))
    # Merge adjacent copies.
    ops.sort(key=lambda o: (o[1], o[2]))
    merged = []
    for o in ops:
        if merged and o[0] == 'COPY' and merged[-1][0] == 'COPY':
            p = merged[-1]
            if p[1] + p[3] == o[1] and p[2] + p[3] == o[2]:
                merged[-1] = ('COPY', p[1], p[2], p[3] + o[3], 0, 0, 0, 0, 0)
                continue
        merged.append(o)
    return merged, unmatched


def dump(build, unit, triple, cache):
    out = cache / ('%s.%s.txt' % (unit.stem, triple))
    if out.exists() and out.stat().st_mtime >= unit.stat().st_mtime:
        return out.read_text(errors='replace')
    if GAME == 'yr':
        args = yr_args(build, unit, triple, cache)
    else:
        args = [CXX[triple]] + TRIPLES[triple] + DIALECT + ['-I', str(build / 'gen/overlay'), '-include', str(PRELUDE), str(unit)]
    p = subprocess.run(args, capture_output=True, text=True)
    out.write_text(p.stdout)
    return p.stdout


def read_types(path):
    """[(type name, unit hint or None)], {type name: the original's size}, {type name: its Load's address}"""
    rows, sizes, loads = [], {}, {}
    for line in path.read_text().splitlines():
        line = line.split('#', 1)[0].strip()
        if not line:
            continue
        parts = line.split()
        hint = None
        for p in parts[1:]:
            if p.startswith('size='):
                sizes[parts[0]] = int(p[5:], 0)
            elif p.startswith('load='):
                loads[parts[0]] = int(p[5:], 0)
            else:
                hint = p
        rows.append((parts[0], hint))
    return rows, sizes, loads


def rtti_names():
    if GAME == 'yr':
        return []
    path = ROOT / 'manifests/rtti_classes.json'
    try:
        return sorted({v['class_name'] for v in json.loads(path.read_text())['vtables']})
    except (OSError, ValueError, KeyError):
        return []


def suggest(name, load, units_text):
    """What a listed type that no unit defines is probably called now (a rename): the class
    whose function carries the listed Load address in the units' address comments, else the
    nearest RTTI names and classes with a Load."""
    if load is not None:
        rx = re.compile(r'([A-Za-z_]\w*(?:<[^>\n]*>)?)::\w+\s*\([^\n]*\(0x0*%X\b' % load, re.I)
        for _, text in units_text:
            m = rx.search(text)
            if m:
                return 'its Load 0x%08X is now %s\'s: rename the row' % (load, m.group(1))
    with_load = set()
    rx = re.compile(r'^[A-Za-z_][^\n;]*?\b([A-Za-z_]\w*)::Load\s*\(', re.M)
    for _, text in units_text:
        with_load.update(rx.findall(text))
    pool = sorted(set(rtti_names()) | with_load)
    near = difflib.get_close_matches(name, pool, n=3, cutoff=0.6)
    if near:
        return 'nearest RTTI/Load class names: %s' % ', '.join(near)
    return 'no RTTI or Load class name is close (give the row load=<address> to find a renamed class)'


def find_unit(build, name, hint, units_text):
    """The unit defining <name>::Load (else Size_Of); a hint names the member function
    that reads the type instead (generated unit names change with the catalog)."""
    plain = name.split('<')[0]
    for kind in ([hint] if hint else ['Load', 'Size_Of']):
        rx = re.compile(r'^[A-Za-z_][^\n;]*\b%s::%s\s*\(' % (re.escape(plain), kind), re.M)
        for path, text in units_text:
            if rx.search(text):
                return path
    return None


KIND_CODE = {'COPY': 0, 'WIDEN': 1, 'WIDEN_S': 2, 'BITS': 3, 'POINTER': 4, 'VPTR': 5, 'VPTR_NAMED': 6}

# The original's vtables (manifests/rtti_classes.json, from the executable's RTTI: class,
# vtable address, the vtable pointer's offset in the complete object).
RTTI_VTABLES = None
MISSING_VTABLES = set()


def rtti_key(name):
    """A class name as both spellings compare: no record keywords, const or spaces, bool."""
    key = canonical(re.sub(r'\bconst\b', '', name).replace('_Bool', 'bool'))
    return re.sub(r'(?<=[<,])const', '', key)     # layout names come without spaces already


def rtti_vtable(cls, offset, outer=''):
    """The original's vtable of cls at offset. A class some units instantiate apart (a
    template's COMDAT copies: two DynamicVectorClass<TechnoTypeClass *> vtables) has several;
    for a member object, the one the outer class's own constructors store (their
    vptr_writers meet), else the first."""
    global RTTI_VTABLES
    if RTTI_VTABLES is None:
        RTTI_VTABLES = {}
        try:
            for v in json.loads((ROOT / 'manifests/rtti_classes.json').read_text())['vtables']:
                key = (rtti_key(v['class_name']), int(v.get('object_offset', 0)))
                RTTI_VTABLES.setdefault(key, []).append((int(v['vtable'], 16), set(v.get('vptr_writers', []))))
        except (OSError, ValueError, KeyError):
            pass
    found = RTTI_VTABLES.get((rtti_key(cls), offset), [])
    if not found:
        return 0
    if len(found) > 1 and outer and rtti_key(outer) != rtti_key(cls):
        writers = set()
        for va, w in RTTI_VTABLES.get((rtti_key(outer), 0), []):
            writers |= w
        for va, w in found:
            if w & writers:
                return va
    return found[0][0]


# ---- Yuri's Revenge (--game yr, lane Y10) ----
GAME = 'ra2'
YR_TYPES = ROOT / 'native/yr/data/original_save_types.txt'
YR_SDK = ROOT / 'native/yr/sdk'
YR_DEFINES = ['-DRA2_NATIVE_YR=1', '-DWIN32', '-D_WIN32', '-D__GNUC_MINOR__=2', '-D__GNUC__=4', '-DNDEBUG']
YR_WARNINGS = ['-Wno-error=address-of-temporary', '-Wno-register', '-Wno-c++11-narrowing']


def yr_i686_support(build, cache):
    """The i686 dump's own preludes, made from the YR build's: ra2port/prelude.hpp with VC6's
    long (RA2_LONG is long, 4 bytes on i686, so the rewritten mingw-w64 headers' RA2_LONG and
    the compiler's MS builtins agree), the YR prelude without the host's C types and _WIN64,
    and _mingw.h's __LONG32 as long. The C++ headers the tree includes come from
    native/yr/sdk/i686 (the i686-msvc dump has no C++ library)."""
    d = cache / 'i686'
    (d / 'ov').mkdir(parents=True, exist_ok=True)

    def put(path, text):
        if not path.exists() or path.read_text() != text:
            path.write_text(text)
    pre = PRELUDE.read_text()
    for a, b in (('typedef int32_t RA2_LONG;', 'typedef long RA2_LONG;'),
                 ('typedef uint32_t RA2_ULONG;', 'typedef unsigned long RA2_ULONG;')):
        if a not in pre:
            raise SystemExit('original_layouts: prelude.hpp has no %r' % a)
        pre = pre.replace(a, b)
    put(d / 'prelude_i686.hpp', pre)
    yr = (YR_SDK / 'yr_prelude.h').read_text()
    a = yr.index('// The C runtime types are the host')
    end = 'typedef va_list __gnuc_va_list;\n'
    yr = yr[:a] + yr[yr.index(end) + len(end):]
    yr = yr.replace('#ifndef _WIN64\n#define _WIN64 1\n#endif\n', '')
    yr = yr.replace('#include "yr_crt.h"', '#include "%s"' % (YR_SDK / 'yr_crt.h'))
    put(d / 'yr_prelude_i686.h', yr)
    mingw = (build / 'gen-yr/mingw/_mingw.h').read_text(encoding='latin-1')
    if NATIVE == 'win64' and '#define __LONG32 long\n' in mingw:
        mingw = mingw.replace('#define __LONG32 long\n', '#define __LONG32 RA2_LONG\n', 1)   # (Windows: long kept; lane Y13)
    if '#define __LONG32 RA2_LONG\n' not in mingw:
        raise SystemExit('original_layouts: gen-yr/mingw/_mingw.h has no __LONG32 RA2_LONG')
    put(d / 'ov/_mingw.h', mingw.replace('#define __LONG32 RA2_LONG\n', '#define __LONG32 long\n'))
    return d


def yr_args(build, unit, triple, cache):
    gen = build / 'gen-yr'
    inc = ['-I', str(YR_SDK), '-I', str(gen / 'hdr'), '-I', str(gen / 'hdr/vqalib')]
    if triple == 'i686':
        d = yr_i686_support(build, cache)
        return (['clang++', '-target', 'i686-pc-windows-msvc', '-ffreestanding', '-U_MSC_VER', '-D_X86_=1',
                 '-DRA2PORT_ALLOW_HOST'] + YR_DEFINES + ['-I', str(YR_SDK / 'i686')] + inc + DIALECT + YR_WARNINGS +
                ['-include', str(d / 'prelude_i686.hpp'), '-include', str(d / 'yr_prelude_i686.h'),
                 '-idirafter', str(d / 'ov'), '-idirafter', str(gen / 'mingw'), str(unit)])
    if triple == 'win64':
        # The x86_64 Windows build's YR units (native/yr/yr.cmake, lane Y13): yr_win_first.h before the
        # dialect header, the rewritten Win32 headers ahead of the toolchain's (-isystem).
        return ([CXX[triple], '-include', str(YR_SDK / 'yr_win_first.h')] + TRIPLES[triple] + YR_DEFINES + inc + DIALECT +
                YR_WARNINGS + ['-include', str(PRELUDE), '-include', str(YR_SDK / 'yr_prelude.h'),
                               '-isystem', str(gen / 'mingw'), str(unit)])
    return ([CXX[triple]] + TRIPLES[triple] + YR_DEFINES + inc + DIALECT + YR_WARNINGS +
            ['-include', str(PRELUDE), '-include', str(YR_SDK / 'yr_prelude.h'), '-idirafter', str(gen / 'mingw'), str(unit)])


VTABLE_NAMES = []


def vtable_name_index(td):
    if td not in VTABLE_NAMES:
        VTABLE_NAMES.append(td)
    return VTABLE_NAMES.index(td)


MSVC_BUILTIN = {'char': 'D', 'signed char': 'C', 'unsigned char': 'E', 'short': 'F', 'unsigned short': 'G',
                'int': 'H', 'unsigned int': 'I', 'long': 'J', 'unsigned long': 'K', 'float': 'M', 'double': 'N',
                'bool': '_N', '_Bool': '_N', 'long long': '_J', 'unsigned long long': '_K', 'wchar_t': 'G',
                'void': 'X'}


def split_args(s):
    out, depth, cur = [], 0, ''
    for ch in s:
        if ch == '<':
            depth += 1
        elif ch == '>':
            depth -= 1
        if ch == ',' and depth == 0:
            out.append(cur.strip())
            cur = ''
        else:
            cur += ch
    if cur.strip():
        out.append(cur.strip())
    return out


def split_scope(name):
    parts, depth, cur, i = [], 0, '', 0
    while i < len(name):
        if name[i] == '<':
            depth += 1
        elif name[i] == '>':
            depth -= 1
        if depth == 0 and name.startswith('::', i):
            parts.append(cur)
            cur = ''
            i += 2
            continue
        cur += name[i]
        i += 1
    parts.append(cur)
    return parts


class Mangler:
    """MSVC's mangling of a class name for its RTTI type descriptor (".?AV" name "@@"), for the
    shapes the saved YR classes have: plain and nested names, templates whose arguments are
    builtins, pointers to records, records, enums and integers; back-references of names."""

    def __init__(self, recs):
        self.recs = recs

    def kind(self, name):
        rec = self.recs.get(canonical(name))
        if rec is None:
            return None
        return {'class': 'V', 'struct': 'U', 'union': 'T'}[rec.kind]

    def qualified(self, name, names):
        out = ''
        for part in reversed(split_scope(name)):
            out += self.simple(part, names)
        return out + '@'

    def simple(self, part, names):
        if '<' in part:
            base = part[:part.index('<')]
            args = split_args(part[part.index('<') + 1:part.rindex('>')])
            inner = []          # template argument lists have their own back-references
            text = '?$' + base + '@' + ''.join(self.arg(a, inner) for a in args) + '@'
            return text
        if part in names:
            return str(names.index(part))
        if len(names) < 10:
            names.append(part)
        return part + '@'

    def arg(self, a, names):
        a = a.strip()
        if a.endswith('*'):
            target = a[:-1].strip()
            cv = 'A'
            if re.match(r'const\b', target) or re.search(r'\bconst$', target):
                cv = 'B'
                target = re.sub(r'^const\s+|\s+const$', '', target).strip()
            return 'P' + cv + self.type(target, names)
        a = re.sub(r'\bconst\s+', '', a).strip()
        if re.fullmatch(r'-?\d+', a):
            v = int(a)
            neg = v < 0
            v = abs(v)
            if 1 <= v <= 10:
                enc = str(v - 1)
            else:
                enc = ''.join(chr(ord('A') + int(c, 16)) for c in ('%X' % v)) + '@' if v else 'A@'
            return '$0' + ('?' if neg else '') + enc
        if a.endswith('*'):
            target = a[:-1].strip()
            cv = 'A'
            if target.startswith('const '):
                cv, target = 'B', target[6:].strip()
            return 'P' + cv + self.type(target, names)
        return self.type(a, names)

    def type(self, t, names):
        t = t.strip()
        km = re.match(r'(class|struct|union|enum)\s+', t)
        if km:
            t = t[km.end():]
            return {'class': 'V', 'struct': 'U', 'union': 'T', 'enum': 'W4'}[km.group(1)] + self.qualified(t, names)
        if t in MSVC_BUILTIN:
            return MSVC_BUILTIN[t]
        k = self.kind(t)
        if k is None:
            if self.is_enum(t):
                return 'W4' + self.qualified(t, names)
            raise KeyError(t)
        return k + self.qualified(t, names)

    def is_enum(self, t):
        return t in ENUMS


ENUMS = set()


def msvc_type_descriptor(cls, recs):
    """".?AV<mangled>@" for a class (None when a name cannot be mangled here)."""
    m = Mangler(recs)
    try:
        k = m.kind(cls) or 'V'
        return '.?A' + k + m.qualified(cls, [])
    except (KeyError, ValueError) as e:
        MANGLE_FAILURES.add('%s (%s)' % (cls, e))
        return None


MANGLE_FAILURES = set()


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    ap.add_argument('--build', required=True, type=Path)
    ap.add_argument('--out', required=True, type=Path)
    ap.add_argument('--types', type=Path, default=TYPES)
    ap.add_argument('--report', type=Path)
    ap.add_argument('--members', type=Path,
                    help='write each map\'s i686 members (path, offset, size, count, kind) as JSON: the save comparison\'s '
                         'member names (tools/original_save_compare.py, lane C17)')
    ap.add_argument('-j', type=int, default=8)
    ap.add_argument('--check', action='store_true', help='exit 1 when a problem is found (the test)')
    ap.add_argument('--target', choices=('arm64', 'windows'), default='arm64',
                    help='the native layouts: the arm64 game (default) or the x86_64 Windows one (LLP64)')
    ap.add_argument('--cxx', help='the native compiler (--target windows: the build\'s x86_64-w64-mingw32-clang++)')
    ap.add_argument('--game', choices=('ra2', 'yr'), default='ra2',
                    help='the game: Red Alert 2 (default) or Yuri\'s Revenge (the YR units of a -DRA2_NATIVE_YR=ON build)')
    args = ap.parse_args()
    global NATIVE, GAME
    GAME = args.game
    if GAME == 'yr' and args.types == TYPES:
        args.types = YR_TYPES
    if args.target == 'windows':
        NATIVE = 'win64'
        if not args.cxx:
            ap.error('--target windows needs --cxx (the build\'s llvm-mingw clang++)')
        CXX['win64'] = args.cxx
    build = args.build.resolve()
    gen = build / ('gen-yr' if GAME == 'yr' else 'gen')
    cache = gen / 'original_layouts'
    cache.mkdir(parents=True, exist_ok=True)
    rows, sizes, loads = read_types(args.types)
    # The build's units (gen/units.cmake): gen/units keeps files of earlier configures
    # whose groups the catalog has since renumbered or renamed.
    listed = re.findall(r'units/([^"/\s]+\.cpp)', (gen / 'units.cmake').read_text())
    units = sorted(gen / 'units' / n for n in listed)
    units_text = [(u, u.read_text(errors='replace')) for u in units]
    where = {}
    problems = []
    for name, hint in rows:
        u = find_unit(build, name, hint, units_text)
        if u is None:
            problems.append('%s (data/original_save_types.txt): no generated unit defines %s::Load or Size_Of; '
                            'renamed or removed? %s' % (name, name, suggest(name, loads.get(name), units_text)))
            continue
        where[name] = u
    # And every unit that reads a stream itself (its records: the structs and lists it loads).
    reader = re.compile(r'(?:stream|Stream|pStm|stm)\w*(?:\.Checked\(\))?\s*->\s*Read\(')
    needed = sorted(set(where.values()) | {u for u, text in units_text if reader.search(text)})

    def both(u):
        return u, parse(dump(build, u, 'i686', cache)), parse(dump(build, u, NATIVE, cache))
    with ThreadPoolExecutor(args.j) as ex:
        parsed = {u: (ri, ra) for u, ri, ra in ex.map(both, needed)}
    tables, report = [], {}
    member_recs = {}
    for name, _ in rows:
        if name not in where:
            continue
        ri, ra = parsed[where[name]]
        key = canonical(name)
        if key not in ri or key not in ra:
            problems.append('%s (data/original_save_types.txt): %s has no record layout of that name; %s'
                            % (name, where[name].name, suggest(name, loads.get(name), units_text)))
            continue
        ops, unmatched = ops_for(ri, ra, key, problems)
        for o in ops:
            if o[0] == 'VPTR' and not o[3]:
                problems.append('%s: no original vtable (%s) for its vtable pointer at +%d'
                                % (name, 'its owner\'s RTTI name was not mangled' if GAME == 'yr'
                                   else 'manifests/rtti_classes.json', o[1]))
        if name in sizes and sizes[name] != ri[key].size:
            problems.append('%s: i686 layout %d bytes in %s, the original\'s object %d' % (name, ri[key].size, where[name].name, sizes[name]))
        for u in unmatched:
            problems.append('%s: %s %s (%s)' % (name, u[0], u[1], u[2]))
        tables.append((key, ri[key].size, ra[key].size, ops))
        member_recs[key] = ri
        report[key] = {'unit': where[name].name, 'i686_size': ri[key].size, 'arm64_size': ra[key].size,
                       'ops': len(ops), 'unmatched': [list(u) for u in unmatched]}
    # Every other record those units lay out (members, list elements, structs a load
    # reads on its own): kept when all units that lay it out agree and both layouts
    # pair completely; a record two units lay out differently (a light view) is left out.
    roots = {t[0] for t in tables}
    others, conflicts = {}, set()
    for u in needed:
        ri, ra = parsed[u]
        for key in sorted(set(ri) & set(ra)):
            if key in roots or key in conflicts or 'anonymous' in key or 'unnamed' in key:
                continue
            if ri[key].kind == 'union' or ri[key].size > 65536:
                continue                     # the pathfinding node pools and the like: never saved
            sub = []
            ops, unmatched = ops_for(ri, ra, key, sub)
            if unmatched or sub or len(ops) > 2000:
                continue
            entry = (key, ri[key].size, ra[key].size, ops)
            if key in others and others[key] != entry:
                conflicts.add(key)
                del others[key]
                continue
            others[key] = entry
            member_recs.setdefault(key, ri)
    tables.extend(others[k] for k in sorted(others))
    report_others = {'records': len(others), 'conflicting names left out': sorted(conflicts),
                     'names not mangled': sorted(MANGLE_FAILURES),
                     'vtable pointers without an original vtable': sorted(MISSING_VTABLES)}
    lines = ['// generated by native/tools/original_layouts.py from data/original_save_types.txt: do not edit.',
             '// i686 (VC6) to %s field maps of the types read from the original game\'s saves (lane C6).'
             % ('arm64' if NATIVE == 'arm64' else 'x86_64 Windows'),
             '#include "%s"' % (ROOT / 'native/port/include/ra2port/original_layouts.hpp'), '',
             'extern "C" const OriginalLayoutOp ra2port_original_ops[] = {']
    first = 0
    index = []
    for key, si, sa, ops in tables:
        index.append((key, si, sa, first, len(ops)))
        for o in ops:
            lines.append('    {%d, %d, %d, %d, %d, %d, %d, %d, %d},' % (KIND_CODE[o[0]], o[1], o[2], o[3], o[4], o[5], o[6], o[7], o[8]))
        first += len(ops)
    if first == 0:
        lines.append('    {0, 0, 0, 0, 0, 0, 0, 0, 0},')
    lines.append('};')
    lines.append('extern "C" const OriginalLayout ra2port_original_layouts[] = {')
    for key, si, sa, f, n in index:
        lines.append('    {"%s", %d, %d, %d, %d},' % (key, si, sa, f, n))
    lines.append('};')
    lines.append('extern "C" const unsigned ra2port_original_layout_count = %d;' % len(index))
    if VTABLE_NAMES:
        lines.append('extern "C" const char *const ra2port_original_vtable_names[] = {')
        lines.extend('    "%s",' % n for n in VTABLE_NAMES)
        lines.append('};')
    text = '\n'.join(lines) + '\n'
    if not args.out.exists() or args.out.read_text() != text:
        args.out.write_text(text)
    if args.members:
        members = {}
        for key, si, sa, _ in tables:
            leaves = flatten(member_recs[key], key, 'i686', []) if key in member_recs else []
            members[key] = {'i686_size': si, 'native_size': sa,
                            'members': [[l.path, l.offset, (l.size or 0) * max(l.count, 1) if l.kind != 'bits' else l.unit,
                                         l.kind] for l in leaves]}
        args.members.write_text(json.dumps(members, separators=(',', ':')) + '\n')
    if args.report:
        args.report.write_text(json.dumps({'problems': problems, 'types': report, 'others': report_others}, indent=1) + '\n')
    if args.check:
        for p in problems:
            print('original layouts: PROBLEM: ' + p, file=sys.stderr)
    print('original layouts: %d types, %d ops, %d problems, %d unmatched members' %
          (len(index), first, len(problems), sum(len(r['unmatched']) for r in report.values())))
    return 1 if args.check and problems else 0


if __name__ == '__main__':
    sys.exit(main())
