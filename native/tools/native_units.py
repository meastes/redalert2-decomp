#!/usr/bin/env python3
"""Generate the native (arm64, clang) view of the recovered tree.

The exact build compiles each catalog compile group (population + options, in
source order) as ONE translation unit: the group's sources are concatenated,
so later files rely on declarations made by earlier ones. The native build
keeps that unit structure. This tool never edits src/; it writes:

  OUT/overlay/shared/*.hpp   every src/shared header after the port rewrites
  OUT/units/<group>.cpp      one native TU per selected compile group
  OUT/units.cmake            RA2_NATIVE_UNITS list for CMake
  OUT/report.json            groups, sources, skipped rows and why

Port rewrites (mechanical, semantics-preserving for VC6 x86 -> LP64 arm64):
  * long / unsigned long        -> RA2_LONG / RA2_ULONG (32-bit, as on VC6)
  * integer literal suffix L    -> dropped (a VC6 long literal is 32-bit)
  * operator new(unsigned int   -> operator new(RA2_SIZE_T  (size_t on LP64)
  * layout size checks          -> disabled (they assert the 32-bit layout)
Rows whose source carries __asm (evidenced inline assembly) are skipped and
listed: they need a native behaviour implementation in native/port.

    python3 native/tools/native_units.py --out build/native/gen --subsystem core
"""
import argparse
import json
import re
import sys
from collections import OrderedDict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'tools/original_target'))
import catalog_io                    # noqa: E402  (pure Python, read-only)
from source_paths import source_path  # noqa: E402

# Subsystems: a compile group is selected when any of its sources lives under
# one of these prefixes (the whole group is taken, as the exact build does).
SUBSYSTEMS = {
    'core': [
        'src/recovered/classes/Random/', 'src/recovered/classes/RandomStraw/',
        'src/recovered/classes/CRCEngine/', 'src/recovered/classes/WWCRCEngine/',
        'src/recovered/classes/Matrix3D/', 'src/recovered/classes/Quaternion/',
        'src/recovered/classes/Vector3/', 'src/recovered/classes/Rect/',
        'src/recovered/unowned/Fast_',
    ],
}

LONG_PROTECT = [(re.compile(r'\blong(\s+)long\b'), 'RA2_LL_PLACEHOLDER'),
                (re.compile(r'\blong(\s+)double\b'), 'RA2_LD_PLACEHOLDER')]
RULES = [
    (re.compile(r'\bunsigned\s+long(\s+int)?\b'), 'RA2_ULONG'),
    (re.compile(r'\bsigned\s+long(\s+int)?\b'), 'RA2_LONG'),
    (re.compile(r'\blong\s+int\b'), 'RA2_LONG'),
    (re.compile(r'\blong\b'), 'RA2_LONG'),
    (re.compile(r'\b((?:0[xX][0-9a-fA-F]+)|(?:\d+))([uU]?)[lL]([uU]?)\b'), r'\1\2\3'),
    (re.compile(r'operator\s+new(\s*\[\s*\])?\s*\(\s*unsigned\s+int\b'), r'operator new\1(RA2_SIZE_T'),
    (re.compile(r'^([ \t]*typedef[ \t]+char[ \t]+\w+[ \t]*\[.*\?[ \t]*1[ \t]*:[ \t]*-1[ \t]*\][ \t]*;)', re.M),
     r'/* native: 32-bit layout check */ // \1'),
    # VC6 CRT wide-string functions take 16-bit characters; libc's take 32-bit
    # wchar_t. The port supplies UTF-16 versions under ra2_ names.
    (re.compile(r'(?<![\w.])(_?wcs\w+|_?wcsicmp|_?wcsnicmp|vswprintf|swprintf|_vsnwprintf|_snwprintf|towupper|towlower|iswspace|iswdigit|iswalpha|_wtoi|wtoi)\b(?=\s*\()'),
     r'ra2_\1'),
]


# VC6 (/Ze) puts a for-init declaration in the enclosing scope, and recovered
# sources rely on it. Hoist it there: `{ for(int i=0;` -> `{ int i=0; for(;`.
# Only when the for follows ; { or } so an unbraced if/else body is untouched.
FOR_INIT = re.compile(r'([;{}][ \t]*\n?[ \t]*)for[ \t]*\([ \t]*'
                      r'((?:const[ \t]+)?(?:unsigned[ \t]+|signed[ \t]+)?[A-Za-z_][\w:]*(?:<[^;()]*?>)?[ \t\*&]+'
                      r'[A-Za-z_]\w*[ \t]*=[^;()]*(?:\([^;()]*\)[^;()]*)*);')


def port(text):
    text = FOR_INIT.sub(lambda m: '%s%s; for(;' % (m.group(1), m.group(2)), text)
    for rx, ph in LONG_PROTECT:
        text = rx.sub(ph, text)
    for rx, rep in RULES:
        text = rx.sub(rep, text)
    return text.replace('RA2_LL_PLACEHOLDER', 'long long').replace('RA2_LD_PLACEHOLDER', 'long double')


def groups(catalog):
    separate = {p['id'] for p in catalog.get('populations', []) if p.get('separate_units')}
    out = OrderedDict()
    for row in sorted(catalog['regions'], key=lambda r: r['source_order']):
        key = (row['population'], tuple(row['options']))
        if row['population'] in separate:
            key += (row['source'],)
        out.setdefault(key, []).append(row)
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--out', type=Path, required=True)
    ap.add_argument('--subsystem', default='core', choices=sorted(SUBSYSTEMS) + ['all'])
    args = ap.parse_args()
    out = args.out.resolve()
    (out / 'overlay/shared').mkdir(parents=True, exist_ok=True)
    (out / 'units').mkdir(parents=True, exist_ok=True)
    for h in sorted((ROOT / 'src/shared').glob('*.hpp')):
        target = out / 'overlay/shared' / h.name
        text = port(h.read_text(errors='replace'))
        if not target.exists() or target.read_text(errors='replace') != text:
            target.write_text(text)
    catalog = catalog_io.load(catalog_io.locate(ROOT), references=False)
    prefixes = None if args.subsystem == 'all' else SUBSYSTEMS[args.subsystem]
    report = dict(subsystem=args.subsystem, groups=[], skipped=[])
    names = []
    fixups = {}
    for f in json.loads((ROOT / 'native/port/fixups.json').read_text())['fixups']:
        fixups.setdefault(f['source'], []).append(f)
    report['fixups'] = []
    for index, (key, rows) in enumerate(groups(catalog).items()):
        sources = [r['source'] for r in rows]
        if prefixes is not None and not any(s.startswith(p) for s in sources for p in prefixes):
            continue
        parts = []
        for r in rows:
            text = source_path(ROOT, r['source']).read_text(errors='replace')
            if '__asm' in text or '_asm ' in text:
                report['skipped'].append(dict(source=r['source'], reason='inline assembly: native implementation needed'))
                continue
            for f in fixups.get(r['source'], []):
                if f['old'] not in text:
                    raise SystemExit('stale fixup for %s: %s' % (r['source'], f['old']))
                text = text.replace(f['old'], f['new'])
                report['fixups'].append(r['source'])
            parts.append('#line 1 "%s"\n%s\n' % ((ROOT / r['source']).as_posix(), port(text)))
        if not parts:
            continue
        name = 'g%04d_%s.cpp' % (index, key[0].replace('population-', 'p'))
        target = out / 'units' / name
        text = '// generated by native/tools/native_units.py: %s %s\n' % (key[0], ' '.join(key[1])) + ''.join(parts)
        if not target.exists() or target.read_text(errors='replace') != text:
            target.write_text(text)
        names.append(name)
        report['groups'].append(dict(unit=name, population=key[0], options=list(key[1]), sources=sources))
    (out / 'units.cmake').write_text('set(RA2_NATIVE_UNITS\n%s)\n' % ''.join(
        '  "${RA2_NATIVE_GEN}/units/%s"\n' % n for n in names))
    (out / 'report.json').write_text(json.dumps(report, indent=1))
    print('%d units, %d sources, %d skipped' % (len(names), sum(len(g['sources']) for g in report['groups']),
                                                len(report['skipped'])))


if __name__ == '__main__':
    main()
