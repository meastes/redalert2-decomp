#!/usr/bin/env python3
"""Close the ra2-native link over what is not recovered yet, and report it.

Reads the game's link inputs (recovered objects in initialiser order, native
data objects, the ra2_port archive, the host objects), computes the symbols
they reference that neither they nor the system libraries (libSystem, libc++,
SDL3) define, and writes:

  OUT (.c)     one trap function per missing function (prints its name and
               aborts: unrecovered code was reached) and one zero placeholder per
               missing data symbol (unsourced initialised data, vtables,
               typeinfo). Opt-in (RA2_NATIVE_LINK_CLOSURE); never shipped.
  OUT.json     the same list, classified: the work queue toward a closed tree.

Classes: c-address (address-suffixed C names: seed callees, unrecovered
functions and globals), cxx-function, cxx-data, c-function, c-data, vtable,
typeinfo, win32 (imports the port does not provide yet), crt.

    python3 native/tools/link_closure.py --inputs gen/link_inputs.txt --host a.o;b.o --out gen/link_closure.c
"""
import argparse
import glob
import json
import os
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'tools/original_target'))


def nm_symbols(paths):
    """(defined, undefined) external symbol sets of objects/archives."""
    defined, undefined = set(), set()
    for i in range(0, len(paths), 400):
        out = subprocess.run(['nm', '-g'] + paths[i:i + 400], capture_output=True, text=True).stdout
        for line in out.splitlines():
            parts = line.split()
            if len(parts) == 2 and parts[0] == 'U':
                undefined.add(parts[1])
            elif len(parts) == 3 and parts[1] not in ('U',):
                defined.add(parts[2])
    return defined, undefined


def system_exports():
    sdk = subprocess.run(['xcrun', '--show-sdk-path'], capture_output=True, text=True).stdout.strip()
    names = set()
    for tbd in [sdk + '/usr/lib/libSystem.B.tbd', sdk + '/usr/lib/libc++.tbd', sdk + '/usr/lib/libc++abi.tbd'] + \
            glob.glob(sdk + '/usr/lib/system/*.tbd'):
        if os.path.exists(tbd):
            names.update(re.findall(r"'?(_[A-Za-z0-9_$.]+)'?", Path(tbd).read_text(errors='replace')))
    for lib in glob.glob('/opt/homebrew/lib/libSDL3*.dylib'):
        out = subprocess.run(['nm', '-gU', lib], capture_output=True, text=True).stdout
        names.update(l.split()[-1] for l in out.splitlines() if l.strip())
    return names


def catalog_data_names():
    """Names (C or demangled C++) the catalog binds to data (MSVC data symbols)."""
    try:
        import catalog_io
    except ImportError:
        return set()
    catalog = catalog_io.load(catalog_io.locate(ROOT), references=True)
    names = set()
    for table in catalog['population_references'].values():
        for msvc, addr in table.items():
            if addr < 0x79A000:
                continue
            if msvc.startswith('?'):
                m = re.match(r'\?([A-Za-z_]\w*)((?:@[A-Za-z_]\w*)*)@@[0-6]', msvc)
                if m:
                    parts = [m.group(1)] + [p for p in m.group(2).split('@') if p]
                    names.add('::'.join(reversed(parts)))
            elif msvc.startswith('_') and not msvc.startswith('__real@') and '@' not in msvc:
                names.add(msvc[1:])
    return names


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--inputs', type=Path, required=True)
    ap.add_argument('--host', nargs='*', default=[])
    ap.add_argument('--out', type=Path, required=True)
    ap.add_argument('--list-undefined', type=Path, help='also write the bare undefined list (for data_globals.py)')
    args = ap.parse_args()
    paths = [p for p in args.inputs.read_text().split('\n') if p.strip()]
    paths += [p for p in args.host if p.strip()]
    defined, undefined = nm_symbols(paths)
    # ___dso_handle and the Mach-O header are synthesized by the linker.
    missing = sorted(undefined - defined - system_exports() - {'___dso_handle', '__mh_execute_header'})
    demangled = subprocess.run(['c++filt'], input='\n'.join(missing), capture_output=True, text=True).stdout.split('\n')
    imports = {i['name'] for i in json.loads((ROOT / 'manifests/imports.json').read_text())['imports']}
    data_names = catalog_data_names()
    rows = []
    for sym, dem in zip(missing, demangled):
        name = sym[1:]
        if sym.startswith('__Z'):
            if dem.startswith('vtable for'):
                cls, kind = 'vtable', 'data'
            elif dem.startswith(('typeinfo', 'VTT for')):
                cls, kind = 'typeinfo', 'data'
            elif '(' in dem:
                cls, kind = 'cxx-function', 'function'
            else:
                cls, kind = 'cxx-data', 'data'
            name = dem
        elif name in imports:
            cls, kind = 'win32', 'function'
        else:
            m = re.search(r'_(?:0x)?([0-9a-fA-F]{6,8})$', name)
            is_data = name in data_names or (m is not None and int(m.group(1), 16) >= 0x79A000)
            if m:
                cls = 'c-address'
            else:
                cls = 'c-data' if is_data else 'c-function'
            kind = 'data' if is_data else 'function'
        rows.append(dict(symbol=sym, name=name, cls=cls, kind=kind))
    out = ['/* generated by native/tools/link_closure.py: link closure for ra2-native.',
           ' * NOT GAME CODE. Traps for unrecovered functions, zero placeholders for',
           ' * unsourced data; see link_closure.json for the classified list. */',
           '#include <stdio.h>', '#include <stdlib.h>',
           'static void ra2_unrecovered(const char *name)',
           '{', '    fprintf(stderr, "ra2-native: unrecovered %s reached\\n", name);',
           '    /* RA2_NATIVE_TRAP_CONTINUE=1 lists every trap on a path instead of stopping',
           '       at the first (diagnostics only: the caller then reads an undefined result). */',
           '    if (!getenv("RA2_NATIVE_TRAP_CONTINUE")) abort();', '}']
    for k, r in enumerate(rows):
        label = r['symbol'].replace('\\', '\\\\').replace('"', '\\"')
        if r['kind'] == 'function':
            text = r['name'].replace('\\', '\\\\').replace('"', '\\"').replace('*/', '* /')
            out.append('void ra2_trap_%05d(void) __asm__("%s");' % (k, label))
            out.append('void ra2_trap_%05d(void) { ra2_unrecovered("%s"); }' % (k, text[:200]))
        else:
            size = 4096 if r['cls'] in ('vtable', 'typeinfo') else 65536
            out.append('unsigned char ra2_hole_%05d[%d] __asm__("%s") __attribute__((aligned(16)));' % (k, size, label))
    args.out.write_text('\n'.join(out) + '\n')
    counts = {}
    for r in rows:
        counts[r['cls']] = counts.get(r['cls'], 0) + 1
    Path(str(args.out.with_suffix('')) + '.json').write_text(json.dumps(dict(
        about='Symbols the ra2-native inputs reference that nothing defines yet (link_closure.py).',
        total=len(rows), by_class=counts, symbols=rows), indent=0))
    if args.list_undefined:
        args.list_undefined.write_text('\n'.join(r['symbol'] for r in rows) + '\n')
    print('link closure: %d undefined (%s)' % (len(rows), ', '.join('%s %d' % kv for kv in sorted(counts.items()))))


if __name__ == '__main__':
    main()
