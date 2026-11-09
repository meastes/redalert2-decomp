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
from member_aliases import ARM64, FLAGS, PRELUDE, parse_layouts  # noqa: E402

REAL = {'CellClass': 'cell_class.hpp'}
EXTRA = ['-Wno-error=address-of-temporary', '-Wno-error']
TRIPLES = {'i686': ['-target', 'i686-pc-windows-msvc', '-DRA2PORT_ALLOW_HOST'],
           'arm64': ARM64}
PAD = re.compile(r'^(unsigned )?char\[\d+\]$')


def layouts(build, path, triple):
    args = ['clang++'] + TRIPLES[triple] + FLAGS + EXTRA + ['-I', str(build / 'gen/overlay'),
                                                           '-include', str(PRELUDE), str(path)]
    p = subprocess.run(args, capture_output=True, text=True)
    return parse_layouts(p.stdout)


def real_layouts(build, names):
    """{name: (i686 layout, arm64 layout)} of the real classes (one TU per header).
    A real class whose header does not compile alone is left out (reported)."""
    tmp = Path(tempfile.mkdtemp(prefix='ra2-view-audit-'))
    out = {}
    per_header = {}
    for name in names:
        per_header.setdefault(REAL[name], []).append(name)

    def one(item):
        header, ns = item
        src = tmp / (header + '.cpp')
        src.write_text('#include "shared/%s"\n' % header +
                       ''.join('int ra2_view_audit_%d = sizeof(%s);\n' % (i, n) for i, n in enumerate(ns)))
        return ns, header, layouts(build, src, 'i686'), layouts(build, src, 'arm64')
    with ThreadPoolExecutor(8) as ex:
        for ns, header, l32, l64 in ex.map(one, sorted(per_header.items())):
            for name in ns:
                if name not in l32 or name not in l64:
                    if name == 'CellClass':
                        sys.exit('view_audit: no layout for %s from shared/%s' % (name, header))
                    print('view_audit: no layout for %s from shared/%s (skipped)' % (name, header), file=sys.stderr)
                    continue
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


CLASS_DEF = re.compile(r'^(?:class|struct)\s+(?:__declspec\(\w+\)\s+)?(\w+)\s*(:[^{;]*)?\{', re.M)
PADDED = re.compile(r'^\s+(?:unsigned )?char unknown_[0-9a-fA-F]+\[0x[0-9A-Fa-f]+(?:\s*-\s*0x[0-9A-Fa-f]+)?\];', re.M)
BASE = re.compile(r'(?:public|private|protected)?\s*(\w+)')


def definitions(text, reals=None):
    """(name, base names, body lines, padded) of every class or struct defined in text
    (with reals: and the real classes its leading comment calls it a view of)."""
    out = []
    for m in CLASS_DEF.finditer(text):
        end = text.find('\n};', m.end())
        body = text[m.end():end if end >= 0 else len(text)]
        nxt = CLASS_DEF.search(body)
        if nxt:
            body = body[:nxt.start()]
        bases = [b.group(1) for b in BASE.finditer((m.group(2) or '').lstrip(':'))
                 if b.group(1) not in ('public', 'private', 'protected', 'virtual')]
        row = (m.group(1), bases, body.count('\n'), bool(PADDED.search(body)))
        out.append(row + (said_targets(text, m.start(), reals),) if reals is not None else row)
    return out


def rtti_classes():
    """{class name: set of its base names} of the polymorphic classes (MSVC RTTI)."""
    path = ROOT / 'manifests/rtti_classes.json'
    out = {}
    for v in json.loads(path.read_text()).get('vtables', []):
        if v.get('object_offset', 0) == 0:
            out.setdefault(v['class_name'], set()).update(b['name'] for b in v.get('bases', []))
    return out


def stem_targets(name, reals):
    """Real classes whose stem (name without 'Class') a view's name contains:
    the one named last first (views are named <owner><target>View:
    UnknownBuildingUnitView is a UnitClass view), the longest first among those
    ending together (UnknownBuildingTypeView: BuildingTypeClass, then BuildingClass)."""
    found = []
    for r in reals:
        stem = r[:-5] if r.endswith('Class') else r
        at = name.rfind(stem)
        if len(stem) >= 4 and at >= 0 and stem != name:
            found.append((at + len(stem), len(stem), r))
    return [r for _, _, r in sorted(found, reverse=True)]


# Padded records named after a real class that are not views of it (their
# comments say what they are).
NOT_VIEWS = {
    'UnknownCellObject710160': 'an object found by its cell (foot.hpp), not a cell',
    'UnknownCellEntry28': 'an entry of a cell\'s +0x28 list (techno.hpp)',
    'UnknownCellEntryType': 'the type of such an entry (techno.hpp)',
    'UnknownTechno230View': 'the object a techno keeps at +0x230 (0x004E7DF0)',
    'UnknownHouseScenarioView': 'the scenario (RandomNumber at +0x218) as house_views.hpp reads it',
}
# Views whose name or comment does not name their real class.
VIEW_OF = {
    'UnknownBuildingSlot105': 'CellClass',     # "The cell slot 105 returns (+0x10A)" (building_class.hpp)
}
SAYS_VIEW = re.compile(r'\b(?:views? of|stand-ins? (?:of|for)|view until|as far as)\s+(?:the\s+)?(\w+Class|Tactical)\b|'
                       r'\b(\w+Class) as (?:\w+ )*(?:reads?|sees?|uses?) it')


def said_targets(text, at, reals):
    """Real classes the comment just before a definition names it a view of."""
    lines = text[:at].rstrip('\n').split('\n')
    block = []
    while lines and lines[-1].lstrip().startswith('//'):
        block.insert(0, lines.pop().lstrip('/ '))
    out = []
    for m in SAYS_VIEW.finditer(' '.join(block)):
        r = m.group(1) or m.group(2)
        if r in reals and r not in out:
            out.append(r)
    return out


def fit(v32, r32, size32):
    """How well a view's own members (not its padding) sit on the real class's
    i686 members: (members on a real member's start, members off any start), or
    None when one lies past the real class's end."""
    starts = {o for _, o, _ in r32}
    on = off = 0
    for path, o, typ in v32:
        if PAD.match(typ) and path.split('.')[-1].startswith('unknown_'):
            continue
        if size32 is not None and o >= size32:
            return None
        if o in starts:
            on += 1
        else:
            off += 1
    return on, off


LAYOUTS = ROOT / 'native/port/view_layouts.json'
LINE = re.compile(r'^#line \d+ "([^"]+)"', re.M)


def defining_source(unit, view, texts, defs, headers):
    """Repository path of the file that defines a view seen in a unit: the
    unit's own text (its #line marker names the source), else the one header
    defining that name (a real class's stand-in header when several do)."""
    t = texts[unit]
    m = re.search(r'^(?:class|struct)\s+(?:__declspec\(\w+\)\s+)?%s\s*(?::[^{;]*)?\{' % re.escape(view), t, re.M)
    if m:
        if unit.parent.name == 'link':
            return None                         # native code: fix it in place
        lines = [x for x in LINE.finditer(t) if x.start() < m.start()]
        if not lines:
            return None
        return Path(lines[-1].group(1)).resolve().relative_to(ROOT.resolve()).as_posix()
    hs = [h for h in headers if any(n == view for n, _, _, _ in defs[h]) and REAL.get(view) != h.name]
    inc = [h for h in hs if '#include "shared/%s"' % h.name in t]
    pick = inc or hs
    return ('src/shared/' + pick[0].name) if pick else None


def top(nodes):
    """(name, offset, type) of a record's own top-level members (no vptr or bases)."""
    out = []
    for p, o, typ, d in nodes:
        name = p.split('.')[-1]
        if d == 1 and name not in ('(vptr)', '(anon)') and not name.startswith('base:'):
            out.append((name, o, typ))
    return out


def respell(l32, l64, r32, r64):
    """{pad member: arm64 size} moving each mismatching member to the real
    class's arm64 offset by resizing the padding just before it; and the members
    padding cannot move (a pointer in an int slot, no padding before it, ...)."""
    t32, t64 = top(l32), dict((p, (o, typ)) for p, o, typ in top(l64))
    pads, stuck, delta, prev = {}, [], 0, None
    for path, off32, typ in t32:
        if PAD.match(typ) and path.startswith('unknown_'):
            prev = path
            continue
        exp, why = expected_offset(r32, r64, off32)
        cur = t64[path][0] + delta
        if exp is None or exp == cur:
            if exp is None:
                stuck.append('%s (%s)' % (path, why))
            prev = None
            continue
        if prev is None:
            stuck.append('%s (no padding before it)' % path)
            continue
        size = int(re.search(r'\[(\d+)\]', t64[prev][1]).group(1)) + (exp - cur)
        if size < 0:
            stuck.append('%s (padding would be negative)' % path)
            prev = None
            continue
        pads[prev] = size
        delta += exp - cur
        prev = None
    return pads, stuck


def in_tree(real, rtti):
    """Views checked (and respelled) are those of the game object tree
    (AbstractClass and its derived classes). Other stand-ins (the MapClass
    screen layers of map_view.hpp, network and GUI classes) are listed as
    suspect: their hand respellings (port/fixups.json, link/check_light_views*.cpp)
    stand."""
    return real == 'AbstractClass' or 'AbstractClass' in rtti.get(real, ())


def write_layouts(rows, layout_of, real, texts, defs, headers, fresh=False):
    out = []
    for row in rows:
        if id(row) not in layout_of:
            continue
        u, l32, l64, r = layout_of[id(row)]
        if not in_tree(r, rtti_classes()):
            continue
        src = defining_source(u, row['view'], texts, defs, headers)
        if src is None:
            continue
        pads, stuck = respell(l32, l64, leaves(real[r][0][1]), leaves(real[r][1][1]))
        if pads or stuck:
            out.append({'source': src, 'view': row['view'], 'real': r, 'pads': dict(sorted(pads.items())),
                        'stuck': stuck})
    # One view text compiled in several units: one respelling when they agree; a
    # view whose layout differs between units (another base definition) is left
    # as it is and listed.
    merged = {}
    for e in out:
        k = (e['source'], e['view'])
        if k in merged and (merged[k]['pads'] != e['pads'] or merged[k]['real'] != e['real']):
            merged[k]['conflict'] = True
        merged.setdefault(k, e)
    out = []
    for e in merged.values():
        if e.pop('conflict', False):
            e['stuck'] = ['(the layout differs between units: not respelled)'] + e['stuck']
            e['pads'] = {}
        out.append(e)
    # Views respelled before match now: their rows stay (merged with new pads)
    # unless --fresh.
    if LAYOUTS.exists() and not fresh:
        kept = {(e['source'], e['view']): e for e in json.loads(LAYOUTS.read_text())['views']}
        for e in out:
            k = (e['source'], e['view'])
            if k in kept and e['pads']:
                kept[k]['pads'].update(e['pads'])
                kept[k]['stuck'] = e['stuck']
            elif k in kept:
                kept[k]['stuck'] = e['stuck']
            else:
                kept[k] = e
        out = list(kept.values())
    out.sort(key=lambda e: (e['source'], e['view']))
    LAYOUTS.write_text(json.dumps({'about': (
        'Generated by native/tools/view_audit.py --write (Scout1111). Padded views (light stand-ins) of real '
        'classes read their members at VC6 offsets; on arm64 the real class has other offsets. native_units.py '
        'resizes each listed padding member to its arm64 size (RA2_NATIVE_PAD(arm64, vc6), port prelude), which '
        'puts the member after it at the real class\'s arm64 offset; the VC6 layout is unchanged. "stuck" members '
        'cannot be moved by padding (respell them in port/fixups.json).'),
        'views': out}, indent=1) + '\n')
    print('view_audit: wrote %s (%d views, %d pads, %d stuck members)' % (
        LAYOUTS.relative_to(ROOT), len(out), sum(len(e['pads']) for e in out), sum(len(e['stuck']) for e in out)),
        file=sys.stderr)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--build', required=True, type=Path)
    ap.add_argument('--view', action='append', default=[], help='CellClass=Name: also audit Name as a CellClass view')
    ap.add_argument('--report', type=Path)
    ap.add_argument('--cells-only', action='store_true', help='only the cell views (the M11 audit)')
    ap.add_argument('--write', action='store_true',
                    help='rewrite port/view_layouts.json: arm64 sizes of the padding before each mismatching member')
    ap.add_argument('--fresh', action='store_true', help='with --write: drop the rows written before')
    ap.add_argument('-j', type=int, default=4)
    a = ap.parse_args()
    build = a.build.resolve()
    units = sorted((build / 'gen/units').glob('*.cpp')) + sorted((ROOT / 'native/link').glob('*.cpp'))
    headers = sorted((build / 'gen/overlay/shared').glob('*.hpp'))
    texts = {u: u.read_text(errors='replace') for u in units + headers}
    defs = {f: definitions(texts[f]) for f in units + headers}

    # Real classes (Scout1111): every polymorphic class (RTTI) a src/shared header
    # defines; where several headers define one, the longest definition is the
    # real class and the others are stand-ins (views) of it.
    rtti = rtti_classes()
    by_name = {}
    for h in headers:
        for n, bases, lines, padded in defs[h]:
            if n in rtti:
                by_name.setdefault(n, []).append((lines, h.name))
    for n in by_name:
        REAL.setdefault(n, max(by_name[n])[1])

    said = {}
    for f in units + headers:
        for n, _, _, _, s in definitions(texts[f], REAL):
            if s:
                said.setdefault(n, [])
                said[n] += [r for r in s if r not in said[n]]
    views = {'CellClass': {'CellClass'}}
    for spec in a.view:
        r, _, n = spec.partition('=')
        views.setdefault(r, set()).add(n)
    target = {}    # view name -> candidate real classes (best fit decides)
    if not a.cells_only:
        for f in units + headers:
            for n, bases, lines, padded in defs[f]:
                if n in REAL:
                    if f.suffix == '.hpp' and f.name == REAL[n]:
                        continue
                    target.setdefault(n, [n])        # a stand-in of the real class itself
                    continue
                if not padded or n in NOT_VIEWS:
                    continue
                cands = []
                named = said.get(n, []) + [r for r in stem_targets(n, REAL) if r not in said.get(n, [])]
                if n in VIEW_OF:
                    named = [VIEW_OF[n]]        # the real classes the view's name names
                for b in bases:                      # a real base: a real class deriving from it,
                    if b in REAL:                    # those the name names first
                        derived = [r for r in REAL if b in rtti.get(r, ()) and r != b]
                        cands += [r for r in named if r in derived] + [r for r in derived if r not in named]
                if not bases or n in VIEW_OF:        # from offset 0: named after its real class
                    cands = named
                if cands:
                    target.setdefault(n, [])
                    target[n] += [c for c in cands if c not in target[n]]
    cast = re.compile(r'(?:\((\w+) *[&*]\) *&?\(?|reinterpret_cast<(\w+) *[&*]>\(&?)(?:Map\[|Cell_At\()')
    # A definition whose leading comment calls it a view of the cell.
    said = re.compile(r'//[^\n]*(?:view of (?:the )?CellClass|view of the cell|CellClass as [^\n]*sees? it|'
                      r'cell members used here|cell as [^\n]*sees? it)[^\n]*\n(?://[^\n]*\n)*'
                      r'(?:class|struct) (\w+)\b[^;{]*\{', re.I)
    for u in units:
        t = texts[u]
        found = [m.group(1) or m.group(2) for m in cast.finditer(t) if not member_of_cell(t, m.end())]
        found += said.findall(t)
        views['CellClass'].update(m for m in found if m not in ('AbstractClass', 'ObjectClass', 'TechnoClass',
                                                                'BuildingClass', 'FootClass'))
    for n in views['CellClass']:
        target[n] = ['CellClass']
    for r, ns in views.items():
        for n in ns:
            target[n] = [r]
    needed = sorted({r for cs in target.values() for r in cs})
    real = real_layouts(build, needed)
    target = {n: [r for r in cs if r in real] for n, cs in target.items()}
    target = {n: cs for n, cs in target.items() if cs}

    # Units to compile: each unit defining a view, and for a header's view one
    # unit including that header (the smallest).
    want = set()
    for u in units:
        if any(n in target for n, _, _, _ in defs[u]):
            want.add(u)
        elif any(n == 'CellClass' for n in target) and re.search(r'shared/(aircraft|map_cell)\.hpp', texts[u]):
            want.add(u)
    for u in units:
        if any(re.search(r'\b%s\b' % re.escape(n), texts[u]) for n in views['CellClass'] if n != 'CellClass'):
            want.add(u)
    for h in headers:
        if not any(n in target and not (n in REAL and REAL[n] == h.name) for n, _, _, _ in defs[h]):
            continue
        inc = '#include "shared/%s"' % h.name
        users = [u for u in units if inc in texts[u]]
        if users:
            want.add(min(users, key=lambda u: len(texts[u])))
    want = sorted(want)
    print('view_audit: %d views of %d real classes in %d units' % (len(target), len(needed), len(want)), file=sys.stderr)

    def job(u):
        return u, layouts(build, u, 'i686'), layouts(build, u, 'arm64')

    seen, rows, layout_of = {}, [], {}
    with ThreadPoolExecutor(a.j) as ex:
        for u, l32, l64 in ex.map(job, want):
            for n, cands in sorted(target.items()):
                if n not in l32 or n not in l64:
                    continue
                v32, v64 = leaves(l32[n][1]), leaves(l64[n][1])
                key = (n, tuple(v32))
                if key in seen:
                    seen[key]['units'].append(u.name)
                    continue
                # The first candidate (the class the name names) whose members all sit
                # on real members' starts, else the best fit.
                best = None
                for r in cands:
                    r32 = leaves(real[r][0][1])
                    if n == r and v32 == r32:
                        best = ('self', r)
                        break
                    f = fit(v32, r32, real[r][0][0] if n != r else None)
                    if f is not None and f[1] == 0:
                        best = (f, r)
                        break
                    if f is not None and (best is None or (f[0], -f[1]) > (best[0][0], -best[0][1])):
                        best = (f, r)
                if best is None:
                    seen[key] = {'view': n, 'real': None, 'units': [u.name], 'mismatches': []}
                    continue
                if best[0] == 'self':
                    seen[key] = {'view': n, 'real': n, 'units': [u.name], 'mismatches': [], 'self': True}
                    continue
                r = best[1]
                r32, r64 = leaves(real[r][0][1]), leaves(real[r][1][1])
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
                if bad:
                    layout_of[id(row)] = (u, l32[n][1], l64[n][1], r)
    if a.write:
        write_layouts(rows, layout_of, real, texts, defs, headers, a.fresh)
    for r in rows:
        r['checked'] = in_tree(r['real'], rtti)
    nbad = sum(1 for r in rows if r['mismatches'] and r['checked'])
    for r in rows:
        print('%-8s %s (%s view; %s%s)' % (('MISMATCH' if r['checked'] else 'suspect') if r['mismatches'] else 'ok',
                                         r['view'], r['real'], r['units'][0],
                                         ' +%d' % (len(r['units']) - 1) if len(r['units']) > 1 else ''))
        for b in r['mismatches']:
            print('         %s %s: VC6 +%s native +%s, real %s at %s' % (b['type'], b['member'], b['vc6'], b['native'],
                                                                        b['real'], b['expected']))
    unfit = sorted({k[0] for k, v in seen.items() if v['real'] is None})
    if unfit:
        print('view_audit: no real class fits %d padded views: %s' % (len(unfit), ', '.join(unfit)), file=sys.stderr)
    print('view_audit: %d views, %d with mismatches (%d suspect: outside the AbstractClass tree, '
          'not checked)' % (len(rows), nbad, sum(1 for r in rows if r['mismatches'] and not r['checked'])))
    if a.report:
        a.report.write_text(json.dumps(rows, indent=1) + '\n')
    return 1 if nbad else 0


if __name__ == '__main__':
    sys.exit(main())
