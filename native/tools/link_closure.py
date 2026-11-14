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

The x86_64 Windows build (README "Windows build") passes --target windows, the
toolchain's llvm-nm and the mingw-w64 sysroot. COFF names have no leading
underscore: they are read with one added, so every list, class and alias TSV
keeps the Mach-O spelling, and the C file names the COFF symbols. Win32 entry
points of the original's import table are never taken from the Windows import
libraries: unless the port defines one, it is a trap as on macOS (the port's
handles and structures are not the system's). Aliases become linker
-alternatename directives in the C file (no ld64 -alias_list).
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


NM = 'nm'
PREFIX = ''          # '_' on Windows: COFF names are read in the Mach-O spelling


def nm_symbols(paths):
    """(defined, undefined) external symbol sets of objects/archives."""
    defined, undefined = set(), set()
    for i in range(0, len(paths), 400):
        out = subprocess.run([NM, '-g'] + paths[i:i + 400], capture_output=True, text=True).stdout
        for line in out.splitlines():
            parts = line.split()
            if len(parts) == 2 and parts[0] == 'U':
                undefined.add(PREFIX + parts[1])
            elif len(parts) == 3 and parts[1] not in ('U',):
                defined.add(PREFIX + parts[2])
    return defined, undefined


def tbd_exports(path):
    """Exported names of a text stub (.tbd): symbols, and the Objective-C classes,
    metaclasses, exception types and ivars it lists without their prefixes."""
    text = Path(path).read_text(errors='replace')
    names = set(re.findall(r"'?(_[A-Za-z0-9_$.]+)'?", text))
    for key, prefixes in (('objc-classes', ('_OBJC_CLASS_$_', '_OBJC_METACLASS_$_')),
                          ('objc-eh-types', ('_OBJC_EHTYPE_$_',)), ('objc-ivars', ('_OBJC_IVAR_$_',))):
        for group in re.findall(key + r':\s*\[([^\]]*)\]', text):
            for name in re.findall(r"[A-Za-z_][A-Za-z0-9_.$]*", group):
                names.update(prefix + name for prefix in prefixes)
    return names


def system_exports(sdk='', frameworks=('Foundation', 'AppKit'), libs=()):
    """Names the system defines: libSystem, libc++, libobjc and the named frameworks
    of the SDK (SDK path or name, default xcrun's: the host's macOS SDK; an iOS
    build passes its sysroot), and the libraries given (default Homebrew's SDL3
    dylib; an iOS build passes its static SDL3)."""
    cmd = ['xcrun'] + (['--sdk', sdk] if sdk and not os.path.isabs(sdk) else []) + ['--show-sdk-path']
    sdk_path = sdk if sdk and os.path.isabs(sdk) else subprocess.run(cmd, capture_output=True, text=True).stdout.strip()
    names = set()
    stubs = [sdk_path + '/usr/lib/libSystem.B.tbd', sdk_path + '/usr/lib/libc++.tbd', sdk_path + '/usr/lib/libc++abi.tbd',
             sdk_path + '/usr/lib/libobjc.A.tbd'] + glob.glob(sdk_path + '/usr/lib/system/*.tbd')
    # CoreFoundation re-exports the toll-free bridged classes Foundation's users name.
    for framework in list(frameworks) + (['CoreFoundation'] if frameworks else []):
        stubs += glob.glob('%s/System/Library/Frameworks/%s.framework/**/*.tbd' % (sdk_path, framework), recursive=True)
    for tbd in stubs:
        if os.path.exists(tbd):
            names.update(tbd_exports(tbd))
    for lib in (libs or glob.glob('/opt/homebrew/lib/libSDL3*.dylib')):
        out = subprocess.run(['nm', '-gU', lib], capture_output=True, text=True).stdout
        names.update(l.split()[-1] for l in out.splitlines() if l.strip() and not l.endswith(':'))
    # A sanitizer build (-fsanitize=address/undefined) links the toolchain's
    # runtime dylib: its entry points are not unrecovered code.
    clang_rt = subprocess.run(['xcrun', 'clang', '--print-resource-dir'], capture_output=True, text=True).stdout.strip()
    # The compiler's builtins (libclang_rt.osx/ios/iossim.a: __isPlatformVersionAtLeast
    # behind @available, the host's .mm; Scout1248) are linked by the driver.
    rt_libs = [clang_rt + '/lib/darwin/libclang_rt.%s.a' % os_name for os_name in ('osx', 'ios', 'iossim')]
    for lib in glob.glob(clang_rt + '/lib/darwin/libclang_rt.*san_osx_dynamic.dylib') + [l for l in rt_libs if os.path.exists(l)]:
        out = subprocess.run(['nm', '-gU', lib], capture_output=True, text=True).stdout
        names.update(l.split()[-1] for l in out.splitlines() if l.strip())
    return names


# The libraries the Windows game links (the driver's defaults with -static, the
# SDL3 static library's system libraries and the port's), in the mingw-w64 sysroot.
WINDOWS_LIBS = ['c++', 'unwind', 'mingw32', 'mingwex', 'moldname', 'ucrt', 'ucrtbase', 'winpthread', 'pthread',
                'kernel32', 'user32', 'gdi32', 'winmm', 'imm32', 'ole32', 'oleaut32', 'version', 'uuid', 'advapi32',
                'setupapi', 'shell32', 'ws2_32', 'iphlpapi', 'dinput8', 'winspool', 'comdlg32', 'm']
# Names lld and the mingw-w64 startup objects provide.
WINDOWS_LINKER_NAMES = {'__ImageBase', '__CTOR_LIST__', '__DTOR_LIST__', '__RUNTIME_PSEUDO_RELOC_LIST__',
                        '__RUNTIME_PSEUDO_RELOC_LIST_END__', '_tls_used', '_tls_index', '__dso_handle'}


def windows_system_exports(sysroot, libs=()):
    """Names the Windows link's libraries define (llvm-nm of the sysroot's archives,
    the startup objects, the compiler's builtins and the libraries given)."""
    paths = [p for p in ('%s/lib/lib%s.a' % (sysroot, l) for l in WINDOWS_LIBS) if os.path.exists(p)]
    paths += glob.glob(sysroot + '/lib/*.o')
    paths += glob.glob(os.path.dirname(sysroot) + '/lib/clang/*/lib/windows/libclang_rt.builtins-x86_64.a')
    paths += [l for l in libs if l]
    names = set()
    for i in range(0, len(paths), 50):
        out = subprocess.run([NM, '-g', '--defined-only'] + paths[i:i + 50], capture_output=True, text=True).stdout
        names.update(PREFIX + l.split()[-1] for l in out.splitlines() if l.strip() and not l.endswith(':'))
    names.update(PREFIX + n for n in WINDOWS_LINKER_NAMES)
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


def classify(missing):
    """Rows (symbol, name, cls, kind) of undefined symbols."""
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
    return rows


def missing_symbols(paths, aliases=(), system=None):
    """Undefined symbols of a link over paths (after the system libraries), less
    those an alias TSV binds to a defined name; (missing, alias pairs)."""
    defined, undefined = nm_symbols(paths)
    if PREFIX:
        # Windows: the original's Win32 imports come from the port or are traps;
        # COMDAT .refptr.X stubs are the compiler's.
        imports = {'_' + i['name'] for i in json.loads((ROOT / 'manifests/imports.json').read_text())['imports']}
        system = (system or set()) - imports
        undefined = {s for s in undefined if not s.startswith(('_.refptr.', '_.weak.'))}
    # ___dso_handle, the Mach-O header and section boundary symbols
    # (section$start$SEG$SECT, e.g. a coverage build's guards) and Objective-C selector
    # stubs (_objc_msgSend$sel, the host's .mm) are synthesized by the linker.
    if system is None:
        system = system_exports()
    missing = sorted(s for s in undefined - defined - system - {'___dso_handle', '__mh_execute_header'}
                     if not s.startswith(('section$', 'segment$', '_objc_msgSend$')))
    alias_list = []
    for tsv in aliases:
        if not tsv.exists():
            continue
        for line in tsv.read_text().split('\n'):
            if not line or line.startswith('#') or line.startswith('alias\t'):
                continue
            alias, target = line.split('\t')[:2]
            if alias in missing and target in defined and alias not in defined:
                alias_list.append((target, alias))
    taken = {a for _, a in alias_list}
    if PREFIX:
        # Windows: names written in the LP64 (macOS) mangling in the native sources
        # (signature_adapters.cpp's labels, the alias TSVs) spell intptr_t/uintptr_t
        # (and size_t) as long/unsigned long (l, m); Win64 mangles them as long long /
        # unsigned long long (x, y). A missing name whose LP64 spelling is defined, or
        # is an alias TSV entry whose target is defined (in either spelling), binds to it;
        # a missing LP64 spelling (a label) binds to its defined Win64 spelling.
        tsv = {}
        for path in aliases:
            if path.exists():
                for line in path.read_text().split('\n'):
                    if line and not line.startswith('#') and not line.startswith('alias\t'):
                        alias, target = line.split('\t')[:2]
                        tsv.setdefault(alias, target)
        for m in missing:
            if m in taken or not m.startswith('__Z'):
                continue
            win64 = next((w for w in spellings(m, {'l': 'x', 'm': 'y'}) if w in defined), None)
            if win64:
                alias_list.append((win64, m))
                taken.add(m)
                continue
            for lp64 in spellings(m, {'x': 'l', 'y': 'm'}):
                target = lp64 if lp64 in defined else tsv.get(lp64)
                if target and target not in defined:
                    target = next((w for w in spellings(target, {'l': 'x', 'm': 'y'}) if w in defined), None)
                if target and target != m:
                    alias_list.append((target, m))
                    taken.add(m)
                    break
    return [m for m in missing if m not in taken], alias_list


def spellings(name, swap):
    """The mangled name with any subset of the characters in swap replaced, outside
    its length-prefixed identifiers (at most 2^10 spellings)."""
    at, i = [], 0
    while i < len(name):
        if name[i].isdigit():
            j = i
            while j < len(name) and name[j].isdigit():
                j += 1
            i = j + int(name[i:j])
            continue
        if name[i] in swap:
            at.append(i)
        i += 1
    at = at[-10:]
    for mask in range(1, 1 << len(at)):
        chars = list(name)
        for bit, i in enumerate(at):
            if mask >> bit & 1:
                chars[i] = swap[chars[i]]
        yield ''.join(chars)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--inputs', type=Path, required=True)
    ap.add_argument('--host', nargs='*', default=[])
    ap.add_argument('--out', type=Path, required=True)
    ap.add_argument('--list-undefined', type=Path, help='also write the bare undefined list (for data_globals.py)')
    ap.add_argument('--aliases', type=Path, nargs='*', default=[], help='alias TSVs (alias, target, ...)')
    ap.add_argument('--alias-list', type=Path, help='write the applicable aliases as an ld64 -alias_list file')
    ap.add_argument('--sdk', default='', help='SDK path or name for the system exports (default: the host macOS SDK)')
    ap.add_argument('--frameworks', nargs='*', default=[], help='SDK frameworks the host links (their exports are system)')
    ap.add_argument('--system-libs', nargs='*', default=[], help='libraries whose exports are system (default: Homebrew SDL3)')
    ap.add_argument('--target', choices=('apple', 'windows'), default='apple', help='object format and system libraries')
    ap.add_argument('--nm', default='nm', help="nm for the objects (Windows: the toolchain's llvm-nm)")
    ap.add_argument('--sysroot', default='', help='Windows: the mingw-w64 sysroot (<llvm-mingw>/x86_64-w64-mingw32)')
    args = ap.parse_args()
    global NM, PREFIX
    NM = args.nm
    if args.target == 'windows':
        PREFIX = '_'
    paths =[p for p in args.inputs.read_text().split('\n') if p.strip()]
    paths += [p for p in args.host if p.strip()]
    # Names of one original address (address_aliases.py, data_globals.py): bound
    # to the defined name through ld64 -alias_list instead of a trap.
    if args.target == 'windows':
        system = windows_system_exports(args.sysroot, args.system_libs)
    else:
        system = system_exports(args.sdk, args.frameworks, [l for l in args.system_libs if l])
    missing, alias_list = missing_symbols(paths, args.aliases, system)
    taken = {a for _, a in alias_list}
    if args.alias_list:
        args.alias_list.write_text(''.join('%s %s\n' % pair for pair in sorted(set(alias_list))))
    rows = classify(missing)
    out = ['/* generated by native/tools/link_closure.py: link closure for ra2-native.',
           ' * NOT GAME CODE. Traps for unrecovered functions, zero placeholders for',
           ' * unsourced data; see link_closure.json for the classified list. */',
           '#include <stdio.h>', '#include <stdlib.h>',
           'void ra2port_trap_reached(const char *name);  /* --run-to summary (port) */',
           'static void ra2_unrecovered(const char *name)',
           '{', '    fprintf(stderr, "ra2-native: unrecovered %s reached\\n", name);',
           '    ra2port_trap_reached(name);',
           '    /* RA2_NATIVE_TRAP_CONTINUE=1 lists every trap on a path instead of stopping',
           '       at the first (diagnostics only: the caller then reads an undefined result). */',
           '    if (!getenv("RA2_NATIVE_TRAP_CONTINUE")) abort();', '}']
    for k, r in enumerate(rows):
        label = r['symbol'][len(PREFIX):].replace('\\', '\\\\').replace('"', '\\"')
        if r['kind'] == 'function':
            text = r['name'].replace('\\', '\\\\').replace('"', '\\"').replace('*/', '* /')
            out.append('void ra2_trap_%05d(void) __asm__("%s");' % (k, label))
            out.append('void ra2_trap_%05d(void) { ra2_unrecovered("%s"); }' % (k, text[:200]))
        else:
            size = 4096 if r['cls'] in ('vtable', 'typeinfo') else 65536
            out.append('unsigned char ra2_hole_%05d[%d] __asm__("%s") __attribute__((aligned(16)));' % (k, size, label))
    if PREFIX and alias_list:
        # COFF: each alias resolves to its target through a linker directive.
        out.append('__asm__(".section .drectve,\\"yni\\"\\n"')
        for target, alias in sorted(set(alias_list)):
            out.append('        ".ascii \\" -alternatename:%s=%s\\"\\n"' % (alias[1:], target[1:]))
        out.append('        ".text\\n");')
    args.out.write_text('\n'.join(out) + '\n')
    counts = {}
    for r in rows:
        counts[r['cls']] = counts.get(r['cls'], 0) + 1
    Path(str(args.out.with_suffix('')) + '.json').write_text(json.dumps(dict(
        about='Symbols the ra2-native inputs reference that nothing defines yet (link_closure.py).',
        total=len(rows), by_class=counts, address_aliases=len(taken), symbols=rows), indent=0))
    if args.list_undefined:
        args.list_undefined.write_text('\n'.join(r['symbol'] for r in rows) + '\n')
    print('link closure: %d undefined (%s); %d resolved by address aliases' % (
        len(rows), ', '.join('%s %d' % kv for kv in sorted(counts.items())), len(taken)))


if __name__ == '__main__':
    main()
