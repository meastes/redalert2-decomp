#!/usr/bin/env python3
"""Audit light views of real classes against the real classes' native layout.

Recovered units read a real object (a map cell, a techno) through per-file or
per-header stand-in classes ("views") whose members sit behind VC6 padding
(`char unknown_000[0x122]; char unknown_122;`). On arm64 the real class has
other offsets (8-byte pointers and vptrs), so such a view reads or writes the
wrong bytes silently: Scout1108 found BuildingClass slot 52's in-file cell view
decrementing CellClass +0x122 natively inside AltOccupierPtr (+0x120 on arm64),
which left the bridge occupier 0x0000000000FF0000 and faulted Evaluate_Cell
0x006C6130.

For every view record this tool takes the clang record layouts of the unit
that defines it for i686-pc-windows-msvc (VC6's layout) and for arm64 (the
native dialect's), finds the real class's member at the view member's i686
offset, and compares the arm64 offsets. Padding members (`char`/`unsigned char`
arrays named unknown_*) are not compared. A view member inside a real pointer,
or at another native offset, is a mismatch: respell the view under
`#if defined(__LP64__)` (port/fixups.json) and static_assert it
(link/check_light_views_*.cpp). Views are discovered as the cast targets of
`Map[...]` in the generated units (cell views) and as other definitions of the
real class's own name; `--view` adds names.

    python3 native/tools/view_audit.py --build <full native build> [--report out.json] [-j N]
Exit 1 when a mismatch is found. Test tooling: it changes no build input.
"""
import argparse
import json
import re
import subprocess
import sys
import tempfile
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[1]
sys.path.insert(0, str(HERE))
from member_aliases import FLAGS, PRELUDE, parse_layouts  # noqa: E402

REAL = {'CellClass': 'cell_class.hpp'}
EXTRA = ['-Wno-error=address-of-temporary', '-Wno-error']
TRIPLES = {'i686': ['-target', 'i686-pc-windows-msvc', '-DRA2PORT_ALLOW_HOST'],
           'arm64': ['-arch', 'arm64', '-mms-bitfields']}
PAD = re.compile(r'^(unsigned )?char\[\d+\]$')


def layouts(build, path, triple):
    args = ['clang++'] + TRIPLES[triple] + FLAGS + EXTRA + ['-I', str(build / 'gen/overlay'),
                                                           '-include', str(PRELUDE), str(path)]
    p = subprocess.run(args, capture_output=True, text=True)
    return parse_layouts(p.stdout)


def real_layouts(build):
    tmp = Path(tempfile.mkdtemp(prefix='ra2-view-audit-'))
    out = {}
    for name, header in REAL.items():
        src = tmp / (name + '.cpp')
        src.write_text('#include "shared/%s"\nint ra2_view_audit = sizeof(%s);\n' % (header, name))
        l32, l64 = layouts(build, src, 'i686'), layouts(build, src, 'arm64')
        if name not in l32 or name not in l64:
            sys.exit('view_audit: no layout for %s from shared/%s' % (name, header))
        out[name] = (l32[name], l64[name])
    return out


def leaves(nodes):
    """(path, offset, type) of the data members (no bases, vptrs or anonymous records)."""
    paths = [n[0] for n in nodes]
    out = []
    for i, (path, off, typ, depth) in enumerate(nodes):
        if path.endswith('(vptr)') or path.split('.')[-1].startswith('base:') or path.endswith('(anon)'):
            continue
        if i + 1 < len(nodes) and nodes[i + 1][0].startswith(path + '.'):
            continue   # a record member: its leaves are compared
        out.append((path, off, typ))
    return out


def shown(path):
    """A member path without the dump's base, vptr and padding prefixes."""
    keep = [p for p in path.split('.') if not p.startswith('base:') and p not in ('(vptr)', '(anon)')]
    return '.'.join(keep[-2:]) if len(keep) > 1 and keep[-2].startswith('unknown_0') is False else (keep[-1] if keep else path)


def expected_offset(real32, real64, off32):
    """Native offset of the real class's byte at i686 offset off32, or (None, why)."""
    m64 = {p: o for p, o, t in real64}
    best = None
    for path, off, typ in real32:
        if off <= off32 and (best is None or off >= best[1]):
            best = (path, off, typ)
    if best is None or best[0] not in m64:
        return None, 'no real member at +0x%X' % off32
    path, off, typ = best
    if off == off32:
        return m64[path], path
    if '*' in typ or typ.startswith('vptr'):
        return None, 'inside the real pointer %s (+0x%X)' % (path, off)
    if '[' in typ or typ in ('double', '__int64', 'unsigned __int64', 'int', 'unsigned int'):
        return m64[path] + (off32 - off), path
    return None, 'inside the real member %s %s (+0x%X)' % (typ, path, off)


def member_of_cell(text, i):
    """True when the cell expression opened just before i is followed by `.member`
    (a cast of one of the cell's members, not of the cell)."""
    depth = 1
    while i < len(text) and depth:
        depth += {'[': 1, '(': 1, ']': -1, ')': -1}.get(text[i], 0)
        i += 1
    return text[i:i + 1] in ('.', '-')


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--build', required=True, type=Path)
    ap.add_argument('--view', action='append', default=[], help='CellClass=Name: also audit Name as a CellClass view')
    ap.add_argument('--report', type=Path)
    ap.add_argument('-j', type=int, default=4)
    a = ap.parse_args()
    build = a.build.resolve()
    real = real_layouts(build)
    units = sorted((build / 'gen/units').glob('*.cpp')) + sorted((ROOT / 'native/link').glob('*.cpp'))
    views = {'CellClass': {'CellClass'}}
    for spec in a.view:
        r, _, n = spec.partition('=')
        views.setdefault(r, set()).add(n)
    cast = re.compile(r'(?:\((\w+) *[&*]\) *&?\(?|reinterpret_cast<(\w+) *[&*]>\(&?)(?:Map\[|Cell_At\()')
    # A definition whose leading comment calls it a view of the cell.
    said = re.compile(r'//[^\n]*(?:view of (?:the )?CellClass|view of the cell|CellClass as [^\n]*sees? it|'
                      r'cell members used here|cell as [^\n]*sees? it)[^\n]*\n(?://[^\n]*\n)*'
                      r'(?:class|struct) (\w+)\b[^;{]*\{', re.I)
    texts = {}
    for u in units:
        t = u.read_text(errors='replace')
        texts[u] = t
        found = [m.group(1) or m.group(2) for m in cast.finditer(t) if not member_of_cell(t, m.end())]
        found += said.findall(t)
        views['CellClass'].update(m for m in found if m not in ('AbstractClass', 'ObjectClass', 'TechnoClass',
                                                                'BuildingClass', 'FootClass'))
    names = {n: r for r, ns in views.items() for n in ns}
    # Units that define a view, or use one from a header (a real class's own name
    # counts where a header other than the real one may define it).
    other = re.compile(r'shared/(aircraft|map_cell)\.hpp')

    def uses(u, n):
        t = texts[u]
        if re.search(r'\b(class|struct) %s\b[^;]*\{' % re.escape(n), t):
            return True
        if n in REAL:
            return bool(other.search(t))
        return re.search(r'\b%s\b' % re.escape(n), t) is not None
    want = [u for u in units if any(uses(u, n) for n in names)]
    print('view_audit: %d views in %d units' % (len(names), len(want)), file=sys.stderr)

    def job(u):
        return u, layouts(build, u, 'i686'), layouts(build, u, 'arm64')

    seen, rows = {}, []
    with ThreadPoolExecutor(a.j) as ex:
        for u, l32, l64 in ex.map(job, want):
            for n, r in names.items():
                if n not in l32 or n not in l64:
                    continue
                v32, v64 = leaves(l32[n][1]), leaves(l64[n][1])
                key = (n, tuple(v32))
                if key in seen:
                    seen[key]['units'].append(u.name)
                    continue
                r32, r64 = leaves(real[r][0][1]), leaves(real[r][1][1])
                if n == r and v32 == r32:
                    continue   # the real class itself
                m64 = {p: o for p, o, t in v64}
                bad = []
                for path, off32, typ in v32:
                    if PAD.match(typ) and path.split('.')[-1].startswith('unknown_') or path not in m64:
                        continue
                    exp, why = expected_offset(r32, r64, off32)
                    if exp != m64[path]:
                        bad.append({'member': shown(path), 'type': typ, 'vc6': '0x%X' % off32, 'native': '0x%X' % m64[path],
                                    'expected': None if exp is None else '0x%X' % exp, 'real': shown(why) if exp is not None else why})
                row = {'view': n, 'real': r, 'units': [u.name], 'mismatches': bad}
                seen[key] = row
                rows.append(row)
    nbad = sum(1 for r in rows if r['mismatches'])
    for r in rows:
        print('%-8s %s (%s view; %s%s)' % ('MISMATCH' if r['mismatches'] else 'ok', r['view'], r['real'], r['units'][0],
                                         ' +%d' % (len(r['units']) - 1) if len(r['units']) > 1 else ''))
        for b in r['mismatches']:
            print('         %s %s: VC6 +%s native +%s, real %s at %s' % (b['type'], b['member'], b['vc6'], b['native'],
                                                                        b['real'], b['expected']))
    print('view_audit: %d views, %d with mismatches' % (len(rows), nbad))
    if a.report:
        a.report.write_text(json.dumps(rows, indent=1) + '\n')
    return 1 if nbad else 0


if __name__ == '__main__':
    sys.exit(main())
