#!/usr/bin/env python3
"""Bind the YR tree's address names to the definitions at those addresses (lane Y2).

ra2-decomp's sources often call a matched function or name a global by its address
(`extern double __cdecl FUN_004CAC40(double);` is fastmath::sqrt, matched in fastmath.cpp;
`DAT_...` may be a global another unit defines by name). Their whole-executable link pins such
names to the original address; natively each one is bound to the native definition at that
address, as an ld64 alias (the same mechanism as the RA2 tree's data/address_aliases.tsv):

  * functions: the manifest's function at the address (data/manifest.csv, its MSVC symbol), when
    exactly one native definition has that qualified name (and, if several overloads do, the
    one with the same parameter count);
  * data: the global native/yr/data/globals.tsv gives at the address, when a unit defines it;
  * names: the tree also calls matched functions by the name data/functions.csv gives their
    address (Game::AudioGetTime for AudioGetTime at 0x004093B0; lane Y3). Such a call is bound
    to the manifest's function at that address when the parameter counts agree; each binding
    is listed in the output's comment lines, so a differing return type can be seen.

Writes a TSV (alias, target; Mach-O symbols) for tools/link_closure.py --aliases.

    python3 native/yr/tools/yr_aliases.py --inputs <link_inputs.txt> --out <aliases.tsv>
"""
import argparse
import csv
import re
import subprocess
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
MANIFEST = ROOT / 'yr/ra2-decomp/data/manifest.csv'
FUNCTIONS = ROOT / 'yr/ra2-decomp/data/functions.csv'   # address, size, flags, name
GLOBALS = ROOT / 'native/yr/data/globals.tsv'
NAME_ALIASES = ROOT / 'native/yr/data/name_aliases.tsv'   # one function, two names in the tree
ADDRESS = re.compile(r'(?:FUN|DAT|PTR|LAB)_([0-9A-Fa-f]{8})\b')


def msvc_name(sym):
    """Qualified name of an MSVC function symbol (?name@scope@@...), constructors and
    destructors as Class::Class / Class::~Class; None for operators and templates."""
    if not sym.startswith('?'):
        return sym.lstrip('_').split('@')[0] or None
    if sym.startswith('??0') or sym.startswith('??1'):
        m = re.match(r'\?\?([01])([A-Za-z_]\w*)((?:@[A-Za-z_]\w*)*)@@', sym)
        if not m:
            return None
        scopes = [m.group(2)] + [p for p in m.group(3).split('@') if p]
        cls = scopes[0]
        return '::'.join(reversed(scopes)) + '::' + ('~' if m.group(1) == '1' else '') + cls
    if sym.startswith('??'):
        return None
    m = re.match(r'\?([A-Za-z_]\w*)((?:@[A-Za-z_]\w*)*)@@', sym)
    if not m:
        return None
    return '::'.join(reversed([m.group(1)] + [p for p in m.group(2).split('@') if p]))


def demangle(names):
    out = subprocess.run(['c++filt'], input='\n'.join(names), capture_output=True, text=True).stdout.split('\n')
    return dict(zip(names, out))


NM = 'nm'
PREFIX = ''   # '_' on Windows (lane Y13): COFF names are read in the Mach-O spelling, as link_closure.py does


def nm(paths):
    defined, undefined = set(), set()
    for i in range(0, len(paths), 400):
        text = subprocess.run([NM, '-g'] + paths[i:i + 400], capture_output=True, text=True).stdout
        for line in text.splitlines():
            parts = line.split()
            if len(parts) == 2 and parts[0] == 'U':
                undefined.add(PREFIX + parts[1])
            elif len(parts) == 3:
                defined.add(PREFIX + parts[2])
    return defined, undefined - defined


def params(dem):
    """Parameter count of a demangled function name, or None."""
    m = re.search(r'\((.*)\)(?: const)?$', dem)
    if not m:
        return None
    inner, depth, count = m.group(1), 0, 0
    if not inner or inner == 'void':
        return 0
    for ch in inner:
        depth += ch in '<('
        depth -= ch in '>)'
        count += ch == ',' and depth == 0
    return count + 1


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--inputs', type=Path, required=True)
    ap.add_argument('--out', type=Path, required=True)
    ap.add_argument('--target', choices=('apple', 'windows'), default='apple')
    ap.add_argument('--nm', default='nm', help="nm for the objects (Windows: the toolchain's llvm-nm)")
    a = ap.parse_args()
    global NM, PREFIX
    NM = a.nm
    if a.target == 'windows':
        PREFIX = '_'
    paths = [l.strip() for l in a.inputs.read_text().splitlines() if l.strip()]
    defined, undefined = nm(paths)
    dem_undefined = demangle(sorted(undefined))
    # Plain (unmangled) C++ globals: _HouseTypes is HouseTypes (c++filt leaves the underscore).
    dem_undefined = {s: (d[1:] if not s.startswith('__Z') and d.startswith('_') else d) for s, d in dem_undefined.items()}
    data_by_name = {}
    for line in GLOBALS.read_text().splitlines():
        if line and not line.startswith('#') and not line.startswith('name\t'):
            name, address = line.split('\t')[:2]
            data_by_name[name] = int(address, 16)
    # Address names, and any global globals.tsv places at an address where a unit defines
    # another name (VisibleSurface is DAT_00887308; Map is DAT_0087F7E8).
    wanted = [s for s in undefined if ADDRESS.search(dem_undefined[s]) or dem_undefined[s] in data_by_name]
    dem_defined = demangle(sorted(defined))
    by_name = defaultdict(list)            # qualified name (no parameters) -> [(symbol, demangled)]
    for sym, dem in dem_defined.items():
        base = dem.split('(')[0] if '(' in dem else dem
        base = re.sub(r'^_', '', base) if not sym.startswith('__Z') else base
        by_name[base].append((sym, dem))
    functions = {int(r['address'], 16): r['symbol'] for r in csv.DictReader(MANIFEST.open())}
    data = {}
    for line in GLOBALS.read_text().splitlines():
        if line and not line.startswith('#') and not line.startswith('name\t'):
            name, address = line.split('\t')[:2]
            data.setdefault(int(address, 16), []).append(name)
    dem_wanted = dem_undefined
    rows, unbound = [], 0
    for sym in sorted(wanted):
        dem = dem_wanted[sym]
        m = ADDRESS.search(dem)
        address = int(m.group(1), 16) if m else data_by_name[dem]
        target = None
        if address in functions:
            name = msvc_name(functions[address])
            candidates = by_name.get(name, []) if name else []
            if len(candidates) > 1:
                n = params(dem)
                candidates = [c for c in candidates if params(c[1]) == n] or candidates
            if len(candidates) == 1:
                target = candidates[0][0]
        if target is None and address in data:
            hits = [c for n in data[address] for c in by_name.get(n, []) if c[0] != sym]
            if len(hits) == 1:
                target = hits[0][0]
        if target and target != sym:
            rows.append((sym, target))
        else:
            unbound += 1
    # Calls by the functions.csv name of a matched function.
    named = {}
    for r in csv.DictReader(FUNCTIONS.open()):
        if r['name'] and not ADDRESS.search(r['name']):
            named.setdefault(r['name'], set()).add(int(r['address'], 16))
    bound = {r[0] for r in rows}
    by_csv = []
    for sym in sorted(undefined - bound):
        dem = dem_undefined[sym]
        if not sym.startswith('__Z') or '(' not in dem:
            continue
        addresses = named.get(dem.split('(')[0], set())
        if len(addresses) != 1:
            continue
        address = next(iter(addresses))
        name = msvc_name(functions.get(address, '')) if address in functions else None
        candidates = [c for c in by_name.get(name, []) if params(c[1]) == params(dem)] if name else []
        if len(candidates) == 1 and candidates[0][0] != sym:
            rows.append((sym, candidates[0][0]))
            by_csv.append('# 0x%08X %s -> %s' % (address, dem, candidates[0][1]))
    for line in NAME_ALIASES.read_text().splitlines():
        if line and not line.startswith(('#', 'alias\t')):
            alias, target = line.split('\t')[:2]
            if alias in undefined and target in defined:
                rows.append((alias, target))
    a.out.write_text('# yr_aliases.py: YR address names bound to the definition at that address\n'
                     + ''.join(l + '\n' for l in by_csv) + 'alias\ttarget\n'
                     + ''.join('%s\t%s\n' % r for r in rows))
    print('yr_aliases: %d address names bound (%d by functions.csv name), %d left' % (len(rows), len(by_csv), unbound))


if __name__ == '__main__':
    main()
