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
TRIPLES = {'i686': ['-target', 'i686-pc-windows-msvc', '-DRA2PORT_ALLOW_HOST'], 'arm64': ['-arch', 'arm64']}
# Builtin sizes per target (canonical types: typedefs resolved).
SCALAR = {'char': 1, 'signed char': 1, 'unsigned char': 1, '_Bool': 1, 'bool': 1,
          'short': 2, 'unsigned short': 2, 'wchar_t': 2, 'char16_t': 2,
          'int': 4, 'unsigned int': 4, 'float': 4, 'char32_t': 4,
          'long long': 8, 'unsigned long long': 8, 'double': 8, 'long double': 8}
LONG = {'i686': 4, 'arm64': 8}
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

    def leaves(self, node, base, prefix, out):
        """Leaves under node; the dump's offsets are from the start of the record the
        block dumps, which sits at base."""
        anonymous = 0
        for child in node.children:
            if child.label == 'vptr':
                continue
            off = base + child.offset
            if child.label == 'base':
                self.leaves(child, base, prefix + child.name + '.', out)
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
                    self.leaves(child, base, path + '.', out)
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
            self.leaves(rec.root, off + k * rec.size, path + ('[%d].' % k if array else '.'), out)


def flatten(recs, name, triple, problems):
    rec = recs[name]
    out = []
    Flattener(recs, triple, problems).leaves(rec.root, 0, '', out)
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
    la = flatten(recs_a, name, 'arm64', problems)
    by_path = {l.path: l for l in la}
    ops, unmatched = [], []
    for l in li:
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
            ops.append(('WIDEN_S' if l.signed else 'WIDEN', l.offset, a.offset, l.count, 0, 0, 0, 0, 0))
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
    args = ['clang++'] + TRIPLES[triple] + DIALECT + ['-I', str(build / 'gen/overlay'), '-include', str(PRELUDE), str(unit)]
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


KIND_CODE = {'COPY': 0, 'WIDEN': 1, 'WIDEN_S': 2, 'BITS': 3}


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    ap.add_argument('--build', required=True, type=Path)
    ap.add_argument('--out', required=True, type=Path)
    ap.add_argument('--types', type=Path, default=TYPES)
    ap.add_argument('--report', type=Path)
    ap.add_argument('-j', type=int, default=8)
    ap.add_argument('--check', action='store_true', help='exit 1 when a problem is found (the test)')
    args = ap.parse_args()
    build = args.build.resolve()
    cache = build / 'gen/original_layouts'
    cache.mkdir(parents=True, exist_ok=True)
    rows, sizes, loads = read_types(args.types)
    # The build's units (gen/units.cmake): gen/units keeps files of earlier configures
    # whose groups the catalog has since renumbered or renamed.
    listed = re.findall(r'units/([^"/\s]+\.cpp)', (build / 'gen/units.cmake').read_text())
    units = sorted(build / 'gen/units' / n for n in listed)
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
        return u, parse(dump(build, u, 'i686', cache)), parse(dump(build, u, 'arm64', cache))
    with ThreadPoolExecutor(args.j) as ex:
        parsed = {u: (ri, ra) for u, ri, ra in ex.map(both, needed)}
    tables, report = [], {}
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
        if name in sizes and sizes[name] != ri[key].size:
            problems.append('%s: i686 layout %d bytes in %s, the original\'s object %d' % (name, ri[key].size, where[name].name, sizes[name]))
        for u in unmatched:
            problems.append('%s: %s %s (%s)' % (name, u[0], u[1], u[2]))
        tables.append((key, ri[key].size, ra[key].size, ops))
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
    tables.extend(others[k] for k in sorted(others))
    report_others = {'records': len(others), 'conflicting names left out': sorted(conflicts)}
    lines = ['// generated by native/tools/original_layouts.py from data/original_save_types.txt: do not edit.',
             '// i686 (VC6) to arm64 field maps of the types read from the original game\'s saves (lane C6).',
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
    text = '\n'.join(lines) + '\n'
    if not args.out.exists() or args.out.read_text() != text:
        args.out.write_text(text)
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
