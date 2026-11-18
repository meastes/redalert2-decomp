#!/usr/bin/env python3
"""YR globals inside a defined object become that object's members (lane Y15).

gamemd.exe names some fields of its global objects as globals of their own: Game::PlayerColor
(0x00A8B394) is Session.PrefColor, Unsorted::MuteSWLaunches (0x00A8B538) is a byte of Session,
DAT_00884D2E is Map's TabClass::unknown_byte_5546. VC6 gave the name and the field one storage;
yr_data.py gives every global no unit defines storage of its own, so a write through one name is
not seen through the other (lane YW2's LAN game mode, lane Y14's command bar).

This tool finds every such name: a global yr_data.py would define (or one already listed here)
whose gamemd.exe address lies inside a global object a YR unit defines (the owner: Session, Map,
Options, Scen..., by native/yr/data/globals.tsv), maps the offset to the owner's member by the
clang record layouts of the owner's unit (i686-pc-windows-msvc, VC6's layout, and the native ones,
tools/original_layouts.py --game yr), and writes native/yr/data/member_aliases.tsv. yr_units.py
appends to the owner's unit an assembler alias per row: the name's symbol is the owner's symbol
plus the member's native offset (arm64 for the Mac and iPhone, x86_64 Windows), so the name and the
member share one storage. Names whose declared type does not fit the member are listed, not merged.

    python3 native/yr/tools/yr_member_aliases.py --build <YR build> [--win-cxx <x86_64-w64-mingw32-clang++>]
        [--check] [--fresh]
--check: exit 1 when the table differs from what the build gives (the ctest).
"""
import argparse
import bisect
import json
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / 'native/tools'))
import original_data      # noqa: E402
import original_layouts as OL   # noqa: E402

GLOBALS = ROOT / 'native/yr/data/globals.tsv'
OUT = ROOT / 'native/yr/data/member_aliases.tsv'
HEAD = ['symbol', 'name', 'address', 'owner_symbol', 'owner', 'unit', 'offset32', 'offset_arm64', 'offset_win64',
        'member', 'kind']
CXXFILT = 'c++filt'


def known():
    out = {}
    for line in GLOBALS.read_text().splitlines():
        if line and not line.startswith(('#', 'name\t')):
            name, address = line.split('\t')[:2]
            out[name] = int(address, 16)
    return out


def demangle(symbols):
    """{Mach-O symbol: C++ name} (the leading underscore dropped, _Z names demangled)."""
    plain = [s[1:] for s in symbols]
    mangled = [s for s in plain if s.startswith('_Z')]
    out = dict(zip(mangled, subprocess.run([CXXFILT], input='\n'.join(mangled), capture_output=True,
                                           text=True).stdout.split('\n'))) if mangled else {}
    return {s: out.get(p, p) for s, p in zip(symbols, plain)}


def unit_data_symbols(objects):
    """{symbol: object} of the data the YR units define."""
    out = {}
    for i in range(0, len(objects), 300):
        text = subprocess.run(['nm', '-g', '-A'] + objects[i:i + 300], capture_output=True, text=True).stdout
        for line in text.splitlines():
            path, _, rest = line.rpartition(': ')
            parts = rest.split()
            if len(parts) == 3 and parts[1] in 'SDBC':
                out.setdefault(parts[2], path)
    return out


def yr_data_names(gen):
    """[(symbol, name, address)] of the storage yr_data.py writes (its own globals; not its aliases)."""
    rx = re.compile(r'__asm__\("(\S+)"\);\s+// (\S+) 0x([0-9A-F]{8})')
    text = (gen / 'yr_original_data.cpp').read_text()
    out = [(m.group(1), m.group(2), int(m.group(3), 16)) for m in rx.finditer(text)]
    # Other names of the same address (CurrentSWType and DAT_008809A0): yr_data.py's aliases.
    at = {s: ad for s, _, ad in out}
    same = re.compile(r'__asm__\("\.globl (\S+)\\n\S+ = (\S+)"\);\s+// (\S+), the same address')
    out += [(m.group(1), m.group(3), at[m.group(2)]) for m in same.finditer(text) if m.group(2) in at]
    return out


def leaf_at(leaves, rel):
    """(leaf, element index) of the i686 leaf covering byte rel, or (None, 0)."""
    for l in leaves:
        size = l.size or 0
        if l.offset <= rel < l.offset + size * max(l.count, 1):
            k = (rel - l.offset) // size if size else 0
            return l, k
    return None, 0


def members(rec):
    """{path: (offset, record type)} of the record-typed members laid out inline (paths as flatten's)."""
    out = {}

    def walk(node, prefix):
        for c in node.children:
            if c.label == 'base':
                walk(c, prefix + c.name + '.')
            elif c.label == 'field' and c.children and c.name:
                out[prefix + c.name] = (c.offset, OL.canonical(re.sub(r'\b(const|volatile)\b', '', c.type).strip()))
                walk(c, prefix + c.name + '.')
    walk(rec.root, '')
    return out


def classify(decl, leaf, rel, k):
    """'exact' (a scalar or pointer of the member's width at its start or at an element's),
    or a reason the name is not merged."""
    if decl is None:
        return 'conflict: the name\'s declaration was not read'
    base, stars, is_array = decl
    base = ' '.join(w for w in base.split() if w not in ('const', 'volatile', 'static', 'extern'))
    if leaf.offset + k * leaf.size != rel:
        return 'conflict: inside a member\'s element (offset %d of %s)' % (rel - leaf.offset - k * leaf.size, leaf.path)
    if stars:
        return 'exact' if leaf.kind == 'pointer' else 'conflict: a pointer name over a %s member' % leaf.kind
    if leaf.kind == 'pointer':
        return 'conflict: a scalar name over a pointer member'
    width = original_data.SCALAR_WIDTH.get(base) if base in original_data.SCALARS else None
    if width is None and leaf.kind == 'enum':
        width = 4
    if leaf.kind in ('scalar', 'enum', 'long') and width is not None and (width == leaf.size or
                                                                       (is_array and leaf.count > 1)):
        return 'exact'
    if leaf.kind == 'bits':
        # A byte name over the first bits of a bit-field's unit (Game::RecordingFlag is Session's
        # `unsigned Record : 1` and the bits after it): the unit's storage; -mms-bitfields keeps the bits.
        if width == 1 and leaf.bits and leaf.bits[0] == 0:
            return 'exact (bit-field byte)'
        return 'conflict: a bit-field member'
    return 'conflict: %s name over a %s member of %s bytes' % (base, leaf.kind, leaf.size)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--build', type=Path, required=True)
    ap.add_argument('--win-cxx', help='llvm-mingw x86_64-w64-mingw32-clang++ (the Windows offsets)')
    ap.add_argument('--check', action='store_true')
    ap.add_argument('--fresh', action='store_true')
    ap.add_argument('--report', type=Path)
    a = ap.parse_args()
    build = a.build.resolve()
    gen = build / 'gen-yr'
    OL.GAME = 'yr'
    cache = gen / 'original_layouts'
    cache.mkdir(parents=True, exist_ok=True)
    win_cxx = a.win_cxx
    if not win_cxx:
        tc = ROOT / 'private/toolchains/llvm-mingw-20261020/llvm-mingw-20261020-ucrt-macos-universal/bin'
        for r in [ROOT] + list(ROOT.parents):
            c = r / tc.relative_to(ROOT) / 'x86_64-w64-mingw32-clang++'
            if c.exists():
                win_cxx = str(c)
                break
    kept = {}
    if OUT.exists() and not a.fresh:
        for line in OUT.read_text().splitlines():
            if line and not line.startswith(('#', 'symbol\t')):
                f = line.split('\t')
                kept[f[0]] = (f[0], f[1], int(f[2], 16))
    addresses = known()
    objects = [p for p in (gen / 'link_inputs.txt').read_text().split('\n') if '/yr_units_objs.dir/' in p]
    defined = unit_data_symbols(objects)
    names = demangle(sorted(defined))
    # Owners: unit-defined objects whose name globals.tsv places, declared with a class type.
    owners = []
    for sym, obj in defined.items():
        name = names.get(sym, '')
        if name not in addresses:
            continue
        unit = gen / 'units' / Path(obj).name[:-2]
        if not unit.exists():
            continue
        short = name.split('::')[-1]
        lines = [l for l in unit.read_text(encoding='latin-1').splitlines() if short in l and ';' in l]
        decl = original_data.declaration(short, lines)
        if decl is None:
            # A template type (`DynamicVectorClass<ColorScheme *> ColorSchemes;`), which the reader skips.
            m = next((re.match(r'^\s*(?:extern\s+)?([A-Za-z_][\w:]*\s*<[^;=()]*>)\s*%s\s*;' % re.escape(short), l)
                      for l in lines if re.match(r'^\s*(?:extern\s+)?[A-Za-z_][\w:]*\s*<[^;=()]*>\s*%s\s*;' % re.escape(short), l)), None)
            if m:
                decl = (m.group(1), 0, False)
        if not decl or decl[1] or decl[2]:
            continue
        base = ' '.join(w for w in decl[0].split() if w not in ('const', 'volatile', 'static', 'extern'))
        if base in original_data.SCALARS or not re.match(r'[A-Za-z_][\w:<>, *]*$', base) or base.endswith('*'):
            continue
        owners.append(dict(symbol=sym, name=name, address=addresses[name], unit=unit, type=OL.canonical(base)))
    owners.sort(key=lambda o: o['address'])
    children = {s: (s, n, ad) for s, n, ad in yr_data_names(gen)}
    children.update(kept)
    child_list = sorted(children.values(), key=lambda c: c[2])
    keys = [c[2] for c in child_list]
    index, typedefs = {}, {}
    for path in list((gen / 'hdr').rglob('*.h')) + list((gen / 'units').glob('*.cpp')):
        for line in path.read_text(encoding='latin-1').splitlines():
            if ';' in line and ('extern' in line or 'static' in line or line[:1].isalpha()):
                # One-line scopes (`namespace Game { namespace Network { extern int PreCalcFrameRate; } }`,
                # `class GameModeOptionsClass { public: static int Instance; };`): one declaration a piece.
                pieces = [line]
                if re.match(r'\s*(?:namespace|class|struct)\s+\w+\s*\{', line):
                    body = re.sub(r'(?:namespace|class|struct)\s+\w+\s*\{|\b(?:public|private|protected)\s*:|\}', ' ', line)
                    pieces = [p.strip() + ';' for p in body.split(';') if p.strip()]
                scope = '::'.join(re.findall(r'(?:namespace|class|struct)\s+(\w+)\s*\{', line))
                for piece in pieces:
                    piece = re.sub(r'^\s*static\s+', '', piece)
                    for w in set(re.findall(r'[A-Za-z_]\w*', piece)):
                        index.setdefault(w, []).append(piece)
                        if scope:
                            index.setdefault(scope + '::' + w, []).append(piece)
            m = re.match(r'\s*typedef\s+([\w:<>, ]+?)\s+(\w+)\s*;', line)
            if m:
                typedefs.setdefault(m.group(2), OL.canonical(m.group(1)))
    parsed = {}

    if win_cxx:
        OL.CXX['win64'] = win_cxx

    def layouts(unit, triple):
        if (unit, triple) not in parsed:
            parsed[(unit, triple)] = OL.parse(OL.dump(build, unit, triple, cache))
        return parsed[(unit, triple)]
    rows, report = {}, {'owners': [], 'conflicts': [], 'skipped': []}
    for o in owners:
        i = bisect.bisect_right(keys, o['address'])
        inner = []
        while i < len(keys) and keys[i] < o['address'] + 0x10000:
            inner.append(child_list[i])
            i += 1
        if not inner:
            continue
        r32 = layouts(o['unit'], 'i686')
        if o['type'] not in r32:
            report['skipped'].append('%s: no i686 layout of %s in %s' % (o['name'], o['type'], o['unit'].name))
            continue
        size32 = r32[o['type']].size
        inner = [c for c in inner if c[2] < o['address'] + size32]
        if not inner:
            continue
        report['owners'].append('%s %s 0x%08X +%d' % (o['name'], o['type'], o['address'], size32))
        l32 = OL.flatten(r32, o['type'], 'i686', [])
        natives = {}
        for triple in ('arm64', 'win64'):
            if triple == 'win64' and not win_cxx:
                continue
            r = layouts(o['unit'], triple)
            natives[triple] = {l.path: l for l in OL.flatten(r, o['type'], triple, [])} if o['type'] in r else {}
        for sym, name, address in inner:
            rel = address - o['address']
            leaf, k = leaf_at(l32, rel)
            if leaf is None:
                report['conflicts'].append('%s 0x%08X: %s+%d is padding or not laid out' % (name, address, o['name'], rel))
                continue
            short = name.split('::')[-1]
            decl = original_data.declaration(short, index.get(name, []) or index.get(short, []))
            kind = classify(decl, leaf, rel, k)
            member = leaf.path + ('[%d]' % k if leaf.count > 1 else '')
            offs = []
            if decl and not decl[1] and not decl[2] and not kind.startswith('exact'):
                # A name of a class type at the start of a member of that class (DAT_008809F4 is
                # Map's RadarClass::LastDrawRect): the member object.
                t = OL.canonical(' '.join(w for w in decl[0].split() if w not in ('const', 'volatile')))
                hit = next((p for p, (off, mt) in members(r32[o['type']]).items() if off == rel and mt in (t, typedefs.get(t))), None)
                if hit:
                    nat = [members(layouts(o['unit'], tr)[o['type']]).get(hit) if tr in natives else None
                           for tr in ('arm64', 'win64')]
                    if nat[0] and (nat[1] or not win_cxx):
                        rows[sym] = [sym, name, '0x%08X' % address, o['symbol'], o['name'], o['unit'].name, str(rel),
                                     str(nat[0][0]), str(nat[1][0]) if nat[1] else '', hit, 'object']
                        continue
            for triple in ('arm64', 'win64'):
                n = natives.get(triple, {}).get(leaf.path)
                if n is None or (k and n.size != leaf.size and leaf.kind != 'pointer'):
                    offs.append(None)
                else:
                    offs.append(n.offset + k * n.size)
            if offs[0] is None or (win_cxx and offs[1] is None):
                kind = 'conflict: no native member %s' % leaf.path
            if not kind.startswith('exact'):
                report['conflicts'].append('%s 0x%08X: %s.%s: %s' % (name, address, o['name'], member, kind))
                continue
            rows[sym] = [sym, name, '0x%08X' % address, o['symbol'], o['name'], o['unit'].name, str(rel),
                         str(offs[0]), str(offs[1]) if offs[1] is not None else '', member, kind]
    lines = ['# Generated by native/yr/tools/yr_member_aliases.py (lane Y15): gamemd.exe globals that are members of a',
             '# global object a YR unit defines; yr_units.py makes each name an alias of owner + the member\'s native',
             '# offset (offset_arm64 on the Mac and iPhone, offset_win64 on Windows). Names and offsets only.',
             '\t'.join(HEAD)] + ['\t'.join(r) for _, r in sorted(rows.items(), key=lambda kv: (kv[1][4], int(kv[1][6])))]
    text = '\n'.join(lines) + '\n'
    if a.report:
        a.report.write_text(json.dumps(report, indent=1) + '\n')
    for c in report['conflicts']:
        print('yr_member_aliases: not merged:', c)
    if a.check:
        if not OUT.exists() or OUT.read_text() != text:
            print('yr_member_aliases: native/yr/data/member_aliases.tsv is not what the build gives; rerun the tool')
            sys.exit(1)
        print('yr_member_aliases: %d rows as the build gives' % len(rows))
        return
    OUT.write_text(text)
    print('yr_member_aliases: %d names bound to members (%d owners); %d not merged' % (
        len(rows), len(report['owners']), len(report['conflicts'])))


if __name__ == '__main__':
    main()
