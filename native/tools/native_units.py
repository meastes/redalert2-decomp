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


# VC6 gives every enum the underlying type int, so an opaque `enum X;` is a
# complete type there. ISO C++ only allows that with a fixed underlying type, and
# then every declaration must repeat it: for each enum declared opaquely anywhere
# in the tree, `enum X;` -> `enum X : int;` and `enum X {` -> `enum X : int {`
# (int is exactly VC6's representation, so layouts and values are unchanged).
OPAQUE_ENUM = re.compile(r'\benum[ \t]+([A-Za-z_]\w*)[ \t]*;')
OPAQUE_ENUMS = set()


def scan_opaque_enums(texts):
    for text in texts:
        OPAQUE_ENUMS.update(OPAQUE_ENUM.findall(text))


def fix_enums(text):
    if not OPAQUE_ENUMS or 'enum' not in text:
        return text
    def opaque(m):
        return 'enum %s : int;' % m.group(1) if m.group(1) in OPAQUE_ENUMS else m.group(0)
    text = OPAQUE_ENUM.sub(opaque, text)
    def body(m):
        return 'enum %s : int%s' % (m.group(1), m.group(2)) if m.group(1) in OPAQUE_ENUMS else m.group(0)
    return re.sub(r'\benum[ \t]+([A-Za-z_]\w*)(\s*\{)', body, text)


def _balanced(text, i, open_ch, close_ch):
    """Index just past the bracket group starting at text[i] == open_ch, or -1."""
    depth = 0
    for j in range(i, len(text)):
        c = text[j]
        if c == open_ch:
            depth += 1
        elif c == close_ch:
            depth -= 1
            if depth == 0:
                return j + 1
        elif c in ';{}' and open_ch == '(':
            return -1
    return -1


def _postfix_end(text, i):
    """End of the postfix-expression operand of a C-style cast at text[i]
    (identifier / qualified name or parenthesised expression, then calls,
    subscripts, . -> members); returns (end, has_call_or_parens) or (-1, False)."""
    n = len(text)
    while i < n and text[i] in ' \t':
        i += 1
    called = False
    if i < n and text[i] == '(':
        i = _balanced(text, i, '(', ')')
        if i < 0:
            return -1, False
        called = True
    else:
        m = re.compile(r'(?:::)?[A-Za-z_]\w*(?:::[A-Za-z_]\w*)*').match(text, i)
        if not m or m.group(0) in ('const', 'volatile', 'throw', 'override', 'return', 'sizeof'):
            return -1, False
        i = m.end()
    while i < n:
        if text[i] == '(':
            i = _balanced(text, i, '(', ')')
            if i < 0:
                return -1, False
            called = True
        elif text[i] == '[':
            i = _balanced(text, i, '[', ']')
            if i < 0:
                return -1, False
        elif text.startswith('->', i) or (text[i] == '.' and i + 1 < n and (text[i + 1].isalpha() or text[i + 1] == '_')):
            i += 2 if text[i] == '-' else 1
            m = re.compile(r'[A-Za-z_]\w*').match(text, i)
            if not m:
                return -1, False
            i = m.end()
        else:
            break
    return i, called


# VC6 lets a cast to a reference type take an rvalue (it reinterprets a
# temporary copy); ISO C++ does not. Wrap the operand in ra2port::as_lvalue
# (prelude), which binds the temporary for the full expression and is the
# identity on lvalues, so the cast reads the same bytes VC6's did.
REINTERPRET_REF = re.compile(r'\breinterpret_cast[ \t]*<')
CSTYLE_REF = re.compile(r'\((?:[ \t]*const[ \t]+)?[ \t]*[A-Za-z_][\w:]*(?:[ \t]*<[^()<>;]*(?:<[^()<>;]*>[^()<>;]*)*>)?'
                        r'(?:[ \t]+const)?[ \t]*&[ \t]*\)')


def fix_reference_casts(text):
    if '&' not in text:
        return text
    out = []
    pos = 0
    for m in REINTERPRET_REF.finditer(text):
        if m.start() < pos:
            continue
        close = _balanced(text, m.end() - 1, '<', '>')
        if close < 0 or '&' not in text[m.end():close]:
            continue
        j = close
        while j < len(text) and text[j] in ' \t':
            j += 1
        if j >= len(text) or text[j] != '(':
            continue
        end = _balanced(text, j, '(', ')')
        if end < 0:
            continue
        out.append(text[pos:j + 1])
        out.append('ra2port::as_lvalue(%s))' % text[j + 1:end - 1])
        pos = end
    out.append(text[pos:])
    text = ''.join(out)
    out = []
    pos = 0
    for m in CSTYLE_REF.finditer(text):
        if m.start() < pos:
            continue
        k = m.start() - 1
        while k >= 0 and text[k] in ' \t':
            k -= 1
        prev_word = re.search(r'([A-Za-z_]\w*)$', text[:k + 1])
        if k >= 0 and (text[k].isalnum() or text[k] in '_)]') and not (prev_word and prev_word.group(1) == 'return'):
            continue   # f(Type &) is a declarator or call, not a cast
        line_start = text.rfind('\n', 0, m.start()) + 1
        if '//' in text[line_start:m.start()] or re.search(r'operator\s*\S{1,3}\s*$', text[line_start:m.start()]):
            continue   # a comment, or operator@(Type &) declarator
        end, called = _postfix_end(text, m.end())
        if end < 0 or not called:
            continue
        operand = text[m.end():end]
        out.append(text[pos:m.end()])
        out.append('ra2port::as_lvalue(%s)' % operand.strip())
        pos = end
    out.append(text[pos:])
    return ''.join(out)


# Win32 message parameters and the *_PTR integers carry pointers (SendMessage
# lParam, DialogBoxParam init values, SetWindowLong user data, list-box item
# data). On Win32 they are 32 bits; the port implements that API on LP64, so
# they become pointer-sized as on Win64: WPARAM/UINT_PTR/ULONG_PTR/DWORD_PTR/
# SIZE_T -> uintptr_t, LPARAM/LRESULT/INT_PTR/LONG_PTR -> intptr_t. Every
# typedef of these names (win32_api.hpp, ghidra_types.hpp, seed prologues) is
# rewritten, so all units agree on the native API's argument widths. Values
# below 2^31 are unchanged; a pointer is no longer truncated.
# Off by default (--win64-params): 21 recovered sources still spell these
# parameters as unsigned int / long in their own prototypes, window and dialog
# procedures and EnumChildWindows callbacks, and fail with it (native/README.md).
WIN64_PARAMS = False
POINTER_WIDTH = {'WPARAM': 'uintptr_t', 'UINT_PTR': 'uintptr_t', 'ULONG_PTR': 'uintptr_t',
                 'DWORD_PTR': 'uintptr_t', 'SIZE_T': 'uintptr_t', 'LPARAM': 'intptr_t',
                 'LRESULT': 'intptr_t', 'INT_PTR': 'intptr_t', 'LONG_PTR': 'intptr_t'}
TYPEDEF_LIST = re.compile(r'^([ \t]*)typedef[ \t]+([A-Za-z_][\w \t]*?)[ \t]+'
                          r'([A-Za-z_]\w*(?:[ \t]*,[ \t]*[A-Za-z_]\w*)*)[ \t]*;', re.M)


def fix_pointer_width_typedefs(text):
    if not WIN64_PARAMS or not any(n in text for n in POINTER_WIDTH):
        return text
    def split(m):
        names = [n.strip() for n in m.group(3).split(',')]
        wide = [n for n in names if n in POINTER_WIDTH]
        if not wide or m.group(2).strip() in ('struct', 'class', 'enum', 'union'):
            return m.group(0)
        rest = [n for n in names if n not in POINTER_WIDTH]
        out = []
        if rest:
            out.append('%stypedef %s %s;' % (m.group(1), m.group(2), ', '.join(rest)))
        out.append(' '.join('typedef %s %s;' % (POINTER_WIDTH[n], n) for n in wide)
                   + ' /* native: pointer-sized as on Win64 */')
        return (m.group(1) if not rest else ' ').join(out) if rest else m.group(1) + out[0]
    return TYPEDEF_LIST.sub(split, text)


def port(text):
    text = fix_pointer_width_typedefs(text)
    text = fix_enums(text)
    text = fix_reference_casts(text)
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


def load_fixups():
    data = json.loads((ROOT / 'native/port/fixups.json').read_text())
    fixups = {}
    for f in data['fixups']:
        fixups.setdefault(f['source'], []).append(f)
    renames = [(re.compile(r'(?<![\w:])%s\b' % re.escape(r['from'])), r['to']) for r in data.get('renames', [])]
    return fixups, renames


def apply_fixups(path, text, fixups, renames, used):
    for f in fixups.get(path, []):
        if f['old'] not in text:
            raise SystemExit('stale fixup for %s: %s' % (path, f['old']))
        text = text.replace(f['old'], f['new'])
        used.append(path)
    for rx, to in renames:
        text = rx.sub(to, text)
    return text


def emit(target, text):
    if not target.exists() or target.read_text(errors='replace') != text:
        target.write_text(text)


def load_drafts():
    """Unmatched launch-path functions carried as their best parked draft
    (native/drafts.json; never exact, each flagged with its score). A draft is
    appended to the compile group of its host_source, as the exact-tree join
    puts a candidate into its unit."""
    by_host = OrderedDict()
    for d in json.loads((ROOT / 'native/drafts.json').read_text())['drafts']:
        by_host.setdefault(d['host_source'], []).append(d)
    return by_host


def draft_part(d, fixups, renames, used, manifest, unit, draft_root):
    row = dict(entry=d['entry'], name=d.get('name'), plan=d['plan'], score=d['score'],
               last_lane_score=d.get('last_lane_score'), draft=d['draft'],
               draft_found_by=d.get('draft_found_by'), raw_seed=d.get('raw_seed', False),
               exact=False, unit=unit)
    manifest.append(row)
    path = draft_root / d['draft']
    if d.get('native', 'compiles') != 'compiles':
        row['status'] = d['native']
        return ''
    if not path.is_file():
        row['status'] = 'missing draft (private lane scratch not present)'
        return ''
    text = path.read_text(errors='replace')
    if '__asm' in text or '_asm ' in text:
        row['status'] = 'skipped: inline assembly'
        return ''
    row['status'] = 'included'
    text = apply_fixups(d['draft'], text, fixups, renames, used)
    return ('// NON-EXACT DRAFT %s (%s, score %s): %s\n#line 1 "%s"\n%s\n'
            % (d['entry'], d['plan'], d['score'], d['draft'], path.as_posix(), port(text)))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--out', type=Path, required=True)
    ap.add_argument('--subsystem', default='core', choices=sorted(SUBSYSTEMS) + ['all'])
    ap.add_argument('--drafts', action='store_true', help='append the native/drafts.json drafts to their host groups')
    ap.add_argument('--win64-params', action='store_true',
                    help='pointer-sized WPARAM/LPARAM/LRESULT/*_PTR (milestone-2 work; 21 sources fail with it)')
    ap.add_argument('--draft-root', type=Path, default=ROOT,
                    help='checkout whose build/class-work holds the lane drafts (default: this one)')
    args = ap.parse_args()
    out = args.out.resolve()
    global WIN64_PARAMS
    WIN64_PARAMS = args.win64_params
    (out / 'overlay/shared').mkdir(parents=True, exist_ok=True)
    (out / 'units').mkdir(parents=True, exist_ok=True)
    catalog = catalog_io.load(catalog_io.locate(ROOT), references=False)
    fixups, renames = load_fixups()
    used = []
    headers = {h: h.read_text(errors='replace') for h in sorted((ROOT / 'src/shared').glob('*.hpp'))}
    texts = {r['source']: source_path(ROOT, r['source']).read_text(errors='replace') for r in catalog['regions']}
    scan_opaque_enums(list(headers.values()) + list(texts.values()))
    for h, text in headers.items():
        rel = h.relative_to(ROOT).as_posix()
        emit(out / 'overlay/shared' / h.name, port(apply_fixups(rel, text, fixups, renames, used)))
    prefixes = None if args.subsystem == 'all' else SUBSYSTEMS[args.subsystem]
    report = dict(subsystem=args.subsystem, groups=[], skipped=[])
    names = []
    by_host = load_drafts() if args.drafts else {}
    manifest = []
    for index, (key, rows) in enumerate(groups(catalog).items()):
        sources = [r['source'] for r in rows]
        if prefixes is not None and not any(s.startswith(p) for s in sources for p in prefixes):
            continue
        parts = []
        for r in rows:
            text = texts[r['source']]
            if '__asm' in text or '_asm ' in text:
                report['skipped'].append(dict(source=r['source'], reason='inline assembly: native implementation needed'))
                continue
            text = apply_fixups(r['source'], text, fixups, renames, used)
            parts.append('#line 1 "%s"\n%s\n' % ((ROOT / r['source']).as_posix(), port(text)))
        if not parts:
            continue
        name = 'g%04d_%s.cpp' % (index, key[0].replace('population-', 'p'))
        for source in sources:
            for d in by_host.get(source, []):
                parts.append(draft_part(d, fixups, renames, used, manifest, 'units/' + name, args.draft_root))
        emit(out / 'units' / name,
             '// generated by native/tools/native_units.py: %s %s\n' % (key[0], ' '.join(key[1])) + ''.join(parts))
        names.append('units/' + name)
        report['groups'].append(dict(unit=name, population=key[0], options=list(key[1]), sources=sources))
    if args.drafts:
        (out / 'drafts.json').write_text(json.dumps(dict(
            about='Non-exact drafts in this native tree (from native/drafts.json): no exact-match credit.',
            drafts=manifest), indent=1))
    unused = sorted(set(fixups) - set(used))
    if unused and args.subsystem == 'all':
        raise SystemExit('fixups for sources not in the tree: %s' % ', '.join(unused))
    report['fixups'] = sorted(set(used))
    report['opaque_enums'] = sorted(OPAQUE_ENUMS)
    (out / 'units.cmake').write_text('set(RA2_NATIVE_UNITS\n%s)\n' % ''.join(
        '  "${RA2_NATIVE_GEN}/%s"\n' % n for n in names))
    (out / 'report.json').write_text(json.dumps(report, indent=1))
    print('%d units, %d sources, %d skipped, %d fixed-up files' % (
        len(names), sum(len(g['sources']) for g in report['groups']), len(report['skipped']), len(set(used))))


if __name__ == '__main__':
    main()
