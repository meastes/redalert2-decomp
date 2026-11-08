#!/usr/bin/env python3
"""Interior-address globals bound to the member at their offset (layout mapping).

VC6 gives a global's interior address and the member at that offset one storage:
`unknown_session_difficulty_a3d2d8` *is* unknown_session_a3d298's member at +0x40.
The native tree gives each named global its own storage (data/bss_globals.cpp),
so a write through the member is not seen through the name and vice versa.

This tool finds every named data global whose original address lies inside
another named global of class type (the owner; extent = the class's i686-msvc
size), maps the offset to the member path of the owner's class (clang record
layouts of the port-rewritten src/shared headers for i686-pc-windows-msvc, the
VC6 layout, and for arm64, the native one; members are matched by path), and
writes data/member_aliases.tsv: the name's native symbol becomes an assembler
alias of the owner's symbol plus the member's native offset, emitted in the
unit that defines the owner (tools/native_units.py) or next to the owner's BSS
storage (tools/data_globals.py). One storage per original address; no bytes.

Each row is classified by the name's declared type against the member's:
  exact       same type (typedef spellings of the same VC6 type are equal);
  compatible  both scalars of one width on both targets (int vs unsigned, ...);
  range       the name's extent covers members whose arm64 relative layout is
              the i686 one (scalar runs, char buffers);
  conflict    anything else (listed, not merged): a pointer in an int slot,
              a name past the members' end, a different width.

    python3 native/tools/member_aliases.py --build <full native build> [--report out.json]
Rows already written are kept unless --fresh (an alias defines the name, so the
next run no longer sees it as BSS).
"""
import argparse
import bisect
import json
import re
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[1]
sys.path.insert(0, str(ROOT / 'tools/original_target'))
sys.path.insert(0, str(HERE))
import catalog_io  # noqa: E402
from data_globals import msvc_key, demangle  # noqa: E402

OUT = ROOT / 'native/data/member_aliases.tsv'
PRELUDE = ROOT / 'native/port/include/ra2port/prelude.hpp'
FLAGS = ['-std=c++17', '-fms-extensions', '-fms-compatibility', '-fdelayed-template-parsing', '-Xclang', '-fno-wchar',
         '-fshort-wchar', '-w', '-fsyntax-only', '-Xclang', '-fdump-record-layouts-complete']

# Same VC6 type under different spellings (src declarations vs the overlay dump).
CANON = {
    'long': 'int', 'RA2_LONG': 'int', 'LONG': 'int', 'BOOL': 'int', 'signed int': 'int', 'INT': 'int',
    'unsigned long': 'unsigned int', 'RA2_ULONG': 'unsigned int', 'DWORD': 'unsigned int', 'UINT': 'unsigned int',
    'unsigned': 'unsigned int', 'ULONG': 'unsigned int', 'uint': 'unsigned int',
    '_Bool': 'bool', 'BYTE': 'unsigned char', 'uchar': 'unsigned char', 'byte': 'unsigned char',
    'WORD': 'unsigned short', 'ushort': 'unsigned short', 'wchar_t': 'unsigned short',
    'short int': 'short', 'undefined4': 'unsigned int', 'undefined2': 'unsigned short', 'undefined1': 'unsigned char',
    'undefined': 'unsigned char', 'long long': '__int64', 'unsigned long long': 'unsigned __int64',
}
SCALAR = {'char': 1, 'signed char': 1, 'unsigned char': 1, 'bool': 1, 'short': 2, 'unsigned short': 2, 'int': 4,
          'unsigned int': 4, 'float': 4, 'double': 8, '__int64': 8, 'unsigned __int64': 8}


def canon(t):
    t = re.sub(r'\b(struct|class|enum|union|const|volatile)\b', ' ', t)
    t = ' '.join(t.split())
    t = re.sub(r'\s*\*\s*', '*', t)
    t = re.sub(r'\s*([<>,])\s*', r'\1', t)
    base = t.rstrip('*')
    stars = len(t) - len(base)
    base = CANON.get(base, base)
    return base + '*' * stars


def split_array(t):
    """('int', [8, 3]) of 'int[8][3]'."""
    dims = [int(d) for d in re.findall(r'\[(\d+)\]', t)]
    return re.sub(r'\[\d+\]', '', t).strip(), dims


def parse_layouts(text):
    """{record name: (size, [(path, offset, type)])} from -fdump-record-layouts output."""
    out = {}
    for block in text.split('*** Dumping AST Record Layout')[1:]:
        lines = [l for l in block.split('\n') if '|' in l]
        if not lines:
            continue
        m = re.match(r'\s*(\d+)\s*\|\s*(?:struct|class|union)\s+(.*?)(?:\s+\(empty\))?\s*$', lines[0])
        if not m:
            continue
        name = m.group(2).strip()
        size = None
        nodes, stack = [], []
        for l in lines[1:]:
            sm = re.search(r'\[sizeof=(\d+)', l)
            if sm:
                size = int(sm.group(1))
                continue
            lm = re.match(r'\s*(\d+)(?::\d+-\d+)?\s*\|(\s*)(.*?)\s*$', l)
            if not lm or not lm.group(3):
                continue
            depth = (len(lm.group(2)) - 1) // 2
            body = lm.group(3)
            if re.match(r'(struct|union|class) \S+::\(anonymous', body):
                key, typ = '(anon)', body
            elif body.endswith('(base)') or body.endswith('(primary base)') or body.endswith('(virtual base)'):
                key, typ = 'base:' + re.sub(r'\s*\((primary |virtual )?base\)$', '', body), body
            elif 'vftable pointer' in body or 'vtable pointer' in body or 'vbtable pointer' in body:
                key, typ = '(vptr)', 'vptr'
            else:
                fm = re.match(r'(.*?)\s*([A-Za-z_]\w*)$', body)
                if not fm:
                    continue
                typ, key = fm.group(1).strip(), fm.group(2)
            del stack[depth:]
            stack.append(key)
            nodes.append(('.'.join(stack), int(lm.group(1)), typ, depth))
        out[canon(name)] = (size, nodes)
    return out


def forward(types):
    """Class names in template arguments (pointer elements need only a declaration)."""
    out = set()
    for t in types:
        if '<' in t:
            for w in re.findall(r'[A-Za-z_]\w*', t[t.index('<'):]):
                if w not in ('int', 'unsigned', 'char', 'short', 'bool', 'float', 'double', 'long', 'void', 'const',
                             '__int64', 'signed') and w[0].isupper():
                    out.add(w)
    return out


def clang_layouts(build, header_types, triple):
    """Record layouts of the given {header: {type}} for one target (a header whose
    types fail together is retried one type at a time)."""
    tmp = Path(tempfile.mkdtemp(prefix='ra2-layouts-'))
    result = {}
    jobs = []
    for header, types in sorted(header_types.items()):
        jobs.append((header, sorted(types), len(types) > 1))
    k = 0
    while jobs:
        header, types, split = jobs.pop(0)
        k += 1
        src = tmp / ('t%05d.cpp' % k)
        src.write_text('#include "shared/%s"\n' % header +
                       ''.join('class %s;\n' % w for w in sorted(forward(types))) +
                       ''.join('int ra2_layout_%d = sizeof(%s);\n' % (i, t) for i, t in enumerate(types)))
        args = ['clang++'] + (['-target', 'i686-pc-windows-msvc', '-DRA2PORT_ALLOW_HOST'] if triple == 'i686'
                              else ['-arch', 'arm64'])
        args += FLAGS + ['-I', str(build / 'gen/overlay'), '-include', str(PRELUDE), str(src)]
        p = subprocess.run(args, capture_output=True, text=True)
        if p.returncode:
            if split:
                jobs += [(header, [t], False) for t in types]
            continue
        for name, v in parse_layouts(p.stdout).items():
            result.setdefault(name, v)
    return result


def object_symbols(paths):
    """{symbol: object} of defined data symbols, and all undefined symbols."""
    defined, undefined = {}, set()
    for i in range(0, len(paths), 400):
        out = subprocess.run(['nm', '-g', '-A'] + paths[i:i + 400], capture_output=True, text=True).stdout
        for line in out.splitlines():
            path, _, rest = line.rpartition(': ')
            parts = rest.split()
            if len(parts) == 2 and parts[0] == 'U':
                undefined.add(parts[1])
            elif len(parts) == 3 and parts[1] in 'SDBC':
                defined.setdefault(parts[2], path)
    return defined, undefined


def declarations(names):
    """{name: (canonical type without array, dims or None)} from src/ declarations."""
    want = set(names)
    word = re.compile(r'[A-Za-z_]\w*')
    found = {}
    for f in sorted((ROOT / 'src').rglob('*')):
        if f.suffix not in ('.cpp', '.hpp', '.h', '.c'):
            continue
        for line in f.read_text(errors='replace').splitlines():
            if ';' not in line or line.startswith((' ', '\t', '#', '//')) and 'extern' not in line:
                continue
            for w in set(word.findall(line)) & want:
                if w in found:
                    continue
                m = re.match(r'^\s*(?:extern\s+"C"\s+)?(?:extern\s+)?(?:static\s+)?((?:const\s+)?[A-Za-z_][\w:<>,\s\*]*?)'
                             r'\s*(\*+\s*(?:const\s*)?)?\b(?:\w+::)*%s\b\s*((?:\[[^\]]*\])*)\s*(?:;|=|\{)' % w, line)
                if not m or m.group(1).split()[-1] in ('return', 'goto', 'case', 'else', 'delete'):
                    continue
                dims = re.findall(r'\[([^\]]*)\]', m.group(3) or '')
                found[w] = (canon(m.group(1) + (m.group(2) or '')),
                            [int(d) if d.strip().isdigit() else None for d in dims] if dims else None)
    return found


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--build', type=Path, required=True)
    ap.add_argument('--report', type=Path)
    ap.add_argument('--fresh', action='store_true')
    args = ap.parse_args()
    build = args.build.resolve()
    kept = {}
    if OUT.exists() and not args.fresh:
        for line in OUT.read_text().splitlines():
            if line and not line.startswith(('#', 'symbol\t')):
                kept[line.split('\t')[0]] = line
    inputs = [p for p in (build / 'gen/link_inputs.txt').read_text().split('\n') if p.strip()]
    inputs += sorted(str(h) for h in build.glob('CMakeFiles/ra2_host_objs.dir/**/*.o'))
    defined, undefined = object_symbols([p for p in inputs if p.endswith('.o')])
    data_syms = sorted(set(defined) | undefined)
    dem = dict(zip(data_syms, demangle(data_syms)))
    catalog = catalog_io.load(catalog_io.locate(ROOT), references=True)
    by_key = {}
    for table in catalog['population_references'].values():
        for msvc, addr in table.items():
            k = msvc_key(msvc)
            if k and k[1] != 'func':
                by_key.setdefault(k[0], set()).add(addr)
    # Native data names with one original address.
    names = {}
    for sym in data_syms:
        d = dem[sym]
        if sym.startswith('__Z'):
            if '(' in d or d.startswith(('vtable', 'typeinfo', 'VTT', 'guard')):
                continue
            q = d
        else:
            q = sym[1:]
        addrs = by_key.get(q)
        if not addrs:
            m = re.search(r'_(?:0x)?([0-9a-fA-F]{6,8})$', q)
            if m and 0x79A000 <= int(m.group(1), 16) < 0xB2D000 and not q.endswith('_native'):
                addrs = {int(m.group(1), 16)}
        if addrs and len(addrs) == 1:
            names[sym] = dict(symbol=sym, name=q, address=next(iter(addrs)), object=defined.get(sym))
    decl = declarations({n['name'].split('::')[-1] for n in names.values()})
    for n in names.values():
        n['decl'] = decl.get(n['name'].split('::')[-1])
    # Owners: class-typed (not pointer, not array) whose class a src/shared header defines.
    overlay = build / 'gen/overlay/shared'
    class_header = {}
    for h in sorted(overlay.glob('*.hpp')):
        for m in re.finditer(r'^\s*(?:class|struct)\s+(?:__declspec\(\w+\)\s+)?([A-Za-z_]\w*)\s*(?::[^;{]*)?\{',
                             h.read_text(errors='replace'), re.M):
            class_header.setdefault(m.group(1), h.name)
    owners, header_types = [], {}
    skipped_owners = []
    for n in names.values():
        if not n['decl'] or n['decl'][1] is not None:
            continue
        t = n['decl'][0]
        if t.endswith('*') or t in SCALAR:
            continue
        base = t.split('<')[0]
        if base not in class_header:
            if base[:1].isupper() or '<' in t:
                skipped_owners.append(dict(name=n['name'], type=t, reason='no src/shared definition'))
            continue
        owners.append(n)
        header_types.setdefault(class_header[base], set()).add(t)
    l32 = clang_layouts(build, header_types, 'i686')
    l64 = clang_layouts(build, header_types, 'arm64')
    addr_index = sorted((n['address'], n['symbol']) for n in names.values())
    keys = [a for a, _ in addr_index]
    rows, conflicts, unmapped = [], [], []
    for o in sorted(owners, key=lambda n: n['address']):
        t = o['decl'][0]
        if t not in l32 or t not in l64:
            skipped_owners.append(dict(name=o['name'], type=t, reason='no clang layout'))
            continue
        size32, nodes32 = l32[t]
        _, nodes64 = l64[t]
        if o['object'] is None:
            skipped_owners.append(dict(name=o['name'], type=t, reason='owner not defined natively'))
            continue
        off64 = {p: off for p, off, _, _ in nodes64}
        i = bisect.bisect_right(keys, o['address'])
        while i < len(keys) and keys[i] < o['address'] + size32:
            child = names[addr_index[i][1]]
            i += 1
            if child['symbol'] == o['symbol'] or child['address'] == o['address']:
                continue
            rel = child['address'] - o['address']
            row = map_member(child, rel, nodes32, off64, l32, l64)
            row.update(owner=o['name'], owner_symbol=o['symbol'], owner_type=t, offset32=rel, owner_size=size32,
                       owner_size64=l64[t][0],
                       owner_object=Path(o['object']).name)
            if row['kind'] == 'conflict':
                conflicts.append(row)
            elif child['object'] and not child['object'].endswith(('/bss_globals.cpp.o', '/original_data.cpp.o')):
                row['kind'] = 'defined'
                unmapped.append(row)
            else:
                rows.append(row)
    # One owner per name: an owner that is itself merged into another is reached
    # through the outer owner's layout (its members are expanded there); of the
    # remaining owners the innermost (smallest class) holds the name.
    merged_owners = {r['symbol'] for r in rows}
    rows = [r for r in rows if r['owner_symbol'] not in merged_owners]
    best = {}
    for r in rows:
        if r['symbol'] not in best or r['owner_size'] < best[r['symbol']]['owner_size']:
            best[r['symbol']] = r
    rows = sorted(best.values(), key=lambda r: (r['owner'], r['offset32'], r['symbol']))
    conflicts = [c for c in conflicts if c['symbol'] not in best and c['owner_symbol'] not in merged_owners]
    lines = ['# generated by native/tools/member_aliases.py: named globals inside a class-typed global (the owner) '
             'bound to the member at their offset: symbol = owner symbol + native offset (assembler alias in the '
             'owner\'s unit or BSS storage). Layouts from clang (i686-pc-windows-msvc / arm64) of src/shared.',
             'symbol\towner_symbol\toffset64\toffset32\tkind\tname\towner\tmember\tname_type\tmember_type\towner_size64']
    seen = set()
    for r in rows:
        seen.add(r['symbol'])
        lines.append('\t'.join(str(x) for x in (r['symbol'], r['owner_symbol'], r['offset64'], r['offset32'], r['kind'],
                                                r['name'], r['owner'], r['member'], r['name_type'], r['member_type'],
                                                r['owner_size64'])))
    for sym, line in sorted(kept.items()):
        if sym not in seen:
            lines.append(line)
    OUT.write_text('\n'.join(lines) + '\n')
    kinds = {}
    for r in rows:
        kinds[r['kind']] = kinds.get(r['kind'], 0) + 1
    print('member aliases: %d owners with layouts, %d names merged (%s), %d conflicts, %d defined by a source, '
          '%d owners skipped' % (len({r['owner'] for r in rows + conflicts}), len(rows),
                                 ', '.join('%d %s' % (v, k) for k, v in sorted(kinds.items())), len(conflicts),
                                 len(unmapped), len(skipped_owners)))
    if args.report:
        args.report.write_text(json.dumps(dict(merged=rows, conflicts=conflicts, defined_by_source=unmapped,
                                               skipped_owners=skipped_owners), indent=1))


def elem_sizes(t, l32, l64):
    t = canon(t)
    if t.endswith('*'):
        return 4, 8
    if t in SCALAR:
        return SCALAR[t], SCALAR[t]
    for name in (t, 'enum ' + t):
        if name in l32 and name in l64:
            return l32[name][0], l64[name][0]
    return None, None


def map_member(child, rel, nodes32, off64, l32, l64):
    """Member of the owner at i686 offset rel and the native offset for the child."""
    ntype, ndims = child['decl'] if child['decl'] else ('?', None)
    row = dict(symbol=child['symbol'], name=child['name'], name_type=ntype + ''.join('[%s]' % d for d in ndims or []),
               kind='conflict', member='', member_type='', offset64='')
    # Deepest node containing rel (nodes are pre-order with absolute offsets).
    chain = []
    for idx, (path, off, typ, depth) in enumerate(nodes32):
        end = None
        for p2, o2, _, d2 in nodes32[idx + 1:]:
            if d2 <= depth:
                end = o2
                break
        etyp, dims = split_array(typ)
        e32, e64 = elem_sizes(etyp, l32, l64) if dims else (None, None)
        if dims and e32:
            n = 1
            for d in dims:
                n *= d
            end = off + n * e32
        elif typ.rstrip().endswith('*') or canon(typ) in SCALAR:
            end = off + elem_sizes(typ, l32, l64)[0]
        if end is None:
            end = off + (1 << 30)
        if off <= rel < end and off <= rel:
            chain.append((path, off, typ, depth, e32, e64, dims))
    if not chain:
        row['member'] = '(past the members)'
        return row
    want = canon(ntype) if ntype != '?' else None
    for path, off, typ, depth, e32, e64, dims in chain:
        if path not in off64:
            continue
        etyp = split_array(typ)[0]
        if dims and e32:
            k, r = divmod(rel - off, e32)
            if r == 0 and want and (canon(etyp) == want or (ndims and canon(etyp) == want)):
                row.update(member='%s[%d]' % (path, k), member_type=typ, offset64=off64[path] + k * e64, kind='exact')
                return row
        elif off == rel and want and canon(typ) == want and not ndims:
            row.update(member=path, member_type=typ, offset64=off64[path], kind='exact')
            return row
    # Not an exact member: the innermost scalar or scalar array at rel.
    path, off, typ, depth, e32, e64, dims = chain[-1]
    row.update(member=path, member_type=typ)
    if path not in off64:
        return row
    etyp = canon(split_array(typ)[0])
    nsize = SCALAR.get(want) if want and not ndims else None
    if dims and e32 and e32 == e64 and etyp in SCALAR:
        # A scalar array is laid out alike on both targets: any interior name maps 1:1.
        k = rel - off
        within = (nsize is None or k + nsize <= e32 * prod(dims))
        if within and (want in SCALAR or (ndims and want in SCALAR)):
            row.update(offset64=off64[path] + k, kind='compatible' if (nsize == e32 or ndims) and want != etyp
                       else 'range', member='%s+%d' % (path, k) if k else path)
            if want == etyp:
                row['kind'] = 'exact'
            return row
    if not dims and off == rel and etyp in SCALAR and want in SCALAR and SCALAR[etyp] == SCALAR[want]:
        row.update(offset64=off64[path], kind='compatible')
        return row
    if not dims and off == rel and ndims and want in SCALAR and etyp in SCALAR:
        # An array name over a run of scalar members: merge when the run keeps its relative layout.
        if run_preserved(chain, rel, ndims, want, nodes32, off64):
            row.update(offset64=off64[path], kind='range')
            return row
    return row


def prod(dims):
    n = 1
    for d in dims:
        n *= d
    return n


def run_preserved(chain, rel, ndims, want, nodes32, off64):
    if None in ndims:
        return False
    span = SCALAR[want] * prod(ndims)
    base64 = None
    for path, off, typ, depth in nodes32:
        if rel <= off < rel + span:
            if path not in off64:
                return False
            if base64 is None:
                base64 = off64[path] - (off - rel)
            if off64[path] - base64 != off - rel:
                return False
            if not (canon(typ) in SCALAR or re.search(r'\[\d+\]', typ) and canon(split_array(typ)[0]) in SCALAR):
                return False
    return base64 is not None


if __name__ == '__main__':
    main()
