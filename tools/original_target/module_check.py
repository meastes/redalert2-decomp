#!/usr/bin/env python3
"""Agreement checks for source-form modules (manifests/original_target/modules.json).

    module_check.py            (also run by canonical.validate_catalog: the ordinary gate)

A module's class may be declared twice: in the module's own header and, while
declaration-count-fragile units keep lobby.hpp's declaration set, in that header
(docs/DECISIONS.md "split declaration"). Two mechanical checks keep the split
honest; either failing refuses the catalog:

1. Symbols. Every relocation of another unit whose target is one of the
   module's function entries names the decorated symbol the module's object
   defines for it (no second spelling, no stand-in prototype); every relocation
   into the module's declared data ranges names a symbol of the module's map.
2. Declarations. For `class` in each header of `declarations`, the two class
   bodies give the same object size, the same member offsets and sizes, the
   same virtual functions in the same order, and, for every member function
   both declare, the same signature in the terms VC6 mangles (return type,
   parameter types, access, const). Spelling differences that do not change
   the layout or a mangled name a unit uses (`unsigned long` against
   `unsigned int` on a data member, four bit-fields against one flags word)
   are reported as known differences, not hidden.

The parser covers the declarations these classes use (fundamental types,
arrays, bit-fields, pointers, virtual and non-virtual member functions,
access labels, nested enums); anything else refuses with the line.
"""
import json
from pathlib import Path
import re
import sys

REPOSITORY = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(Path(__file__).resolve().parent))

MODULES = REPOSITORY / 'manifests/original_target/modules.json'
MAPS = REPOSITORY / 'manifests/original_target/modules'

# VC6 x86 sizes under /Zp4 (alignment = min(size, 4)).
SIZES = {'char': 1, 'unsigned char': 1, 'signed char': 1, 'bool': 1,
         'short': 2, 'unsigned short': 2, 'signed short': 2,
         'int': 4, 'unsigned int': 4, 'unsigned': 4, 'long': 4, 'unsigned long': 4, 'signed': 4,
         'float': 4, 'double': 8}
# Spellings that mangle alike (the same VC6 code) and spellings that only differ in width name.
SAME_MANGLING = {'unsigned': 'unsigned int', 'signed': 'int'}
SAME_LAYOUT = {'unsigned long': 'unsigned int', 'long': 'int'}


class Refusal(ValueError):
    pass


def strip_comments(text):
    text = re.sub(r'/\*.*?\*/', '', text, flags=re.S)
    return re.sub(r'//[^\n]*', '', text)


def class_body(text, name):
    """The text between the braces of `class NAME {`; refuses nested braces other than enums and inline bodies."""
    match = re.search(r'\bclass\s+%s\s*(?::[^{]*)?\{' % re.escape(name), text)
    if not match:
        raise Refusal('no class %s' % name)
    depth, start = 1, match.end()
    for index in range(start, len(text)):
        if text[index] == '{':
            depth += 1
        elif text[index] == '}':
            depth -= 1
            if depth == 0:
                return text[start:index]
    raise Refusal('class %s: unbalanced braces' % name)


def normalize_type(spelling, same=SAME_MANGLING):
    words = spelling.replace('*', ' * ').replace('&', ' & ').split()
    words = [w for w in words if w not in ('const',)] + (['const'] if 'const' in words else [])
    # 'const' position does not change a mangling for these simple types; keep it as a flag.
    base = ' '.join(w for w in words if w not in ('*', '&', 'const'))
    base = same.get(base, base)
    return base + ''.join(' ' + w for w in words if w in ('*', '&', 'const'))


def split_params(text):
    text = text.strip()
    if text in ('', 'void'):
        return []
    return [normalize_type(re.sub(r'\b[A-Za-z_]\w*\s*$', '', p).strip() if not re.fullmatch(r'[A-Za-z_][\w ]*\*?', p.strip()) or ' ' in p.strip() or '*' in p else p.strip())
            for p in text.split(',')]


def parse_class(text, name):
    """(members, functions, virtuals): members = [(name, type, bits, count)], functions =
    {name: dict(access, virtual, const, ret, params)}, virtuals = [name in declaration order]."""
    body = class_body(strip_comments(text), name)
    # Drop enum bodies (constants, no layout) and inline function bodies.
    body = re.sub(r'\benum\s+\w*\s*\{[^}]*\}\s*;', '', body)
    body = re.sub(r'\{[^{}]*\}', ';', body)      # an inline body ends its declaration
    access = 'private'
    members, functions, virtuals = [], {}, []
    for raw in body.split(';'):
        line = raw.strip()
        if not line:
            continue
        label = re.match(r'(public|private|protected)\s*:\s*(.*)$', line, flags=re.S)
        if label:
            access, line = label.group(1), label.group(2).strip()
            if not line:
                continue
        virtual = line.startswith('virtual ')
        if virtual:
            line = line[len('virtual '):].strip()
        function = re.fullmatch(r'(.*?)\s*(~?\b[A-Za-z_]\w*)\s*\(([^)]*)\)\s*(const)?\s*', line, flags=re.S)
        if function:
            ret, fname, params, const = function.groups()
            ret = normalize_type(ret) if ret.strip() else ''
            functions[fname] = dict(access=access, virtual=virtual, const=bool(const), ret=ret, params=split_params(params))
            if virtual:
                virtuals.append(fname)
            continue
        member = re.fullmatch(r'((?:unsigned|signed|const|long|short|char|int|bool|float|double|\s)+?)\s*(\**)\s*([A-Za-z_]\w*)\s*(\[\s*(\d+)\s*\])?\s*(?::\s*(\d+))?\s*', line)
        if not member:
            raise Refusal('class %s: unsupported declaration: %s' % (name, line[:60]))
        spelling, stars, mname, _, count, bits = member.groups()
        spelling = ' '.join(spelling.split())
        if spelling not in SIZES and not stars:
            raise Refusal('class %s: unsupported member type %s' % (name, spelling))
        members.append((mname, spelling + ('*' * len(stars)), int(bits) if bits else None, int(count) if count else 1))
    return members, functions, virtuals


def layout(members, polymorphic):
    """[(name, offset, size)] and the object size, VC6 /Zp4: bit-fields of one declared type share a
    unit while they fit; alignment is min(size, 4); the whole rounds to its largest alignment."""
    out, offset, align = [], 4 if polymorphic else 0, 4 if polymorphic else 1
    unit = None     # (offset, size, bits used, type) of the open bit-field unit
    for name, spelling, bits, count in members:
        size = 4 if spelling.endswith('*') else SIZES[spelling]
        a = min(size, 4); align = max(align, a)
        if bits is not None:
            if unit and unit[3] == spelling and unit[2] + bits <= unit[1] * 8:
                out.append((name, unit[0], 0)); unit = (unit[0], unit[1], unit[2] + bits, spelling)
                continue
            offset = (offset + a - 1) // a * a
            unit = (offset, size, bits, spelling); out.append((name, offset, 0)); offset += size
            continue
        unit = None
        offset = (offset + a - 1) // a * a
        out.append((name, offset, size * count)); offset += size * count
    return out, (offset + align - 1) // align * align


def compare_declarations(class_name, headers, read=None):
    """Compare the class across headers; return (problems, known differences). Each header is a repository path."""
    read = read or (lambda path: (REPOSITORY / path).read_text(errors='replace'))
    parsed = []
    for header in headers:
        members, functions, virtuals = parse_class(read(header), class_name)
        parsed.append((header, members, functions, virtuals, layout(members, bool(virtuals))))
    problems, known = [], []
    first, *rest = parsed
    for other in rest:
        pair = '%s against %s' % (other[0], first[0])
        if first[4][1] != other[4][1]:
            problems.append('%s: object size %d against %d' % (pair, other[4][1], first[4][1]))
        if first[3] != other[3]:
            problems.append('%s: virtual functions %s against %s' % (pair, other[3], first[3]))
        a = [(o, s) for _, o, s in first[4][0] if s]
        b = [(o, s) for _, o, s in other[4][0] if s]
        bits_a = [(o, n) for n, o, s in first[4][0] if not s]
        bits_b = [(o, n) for n, o, s in other[4][0] if not s]
        if a != b and not (bits_a or bits_b):
            problems.append('%s: member offsets/sizes differ: %s against %s' % (pair, b, a))
        elif bits_a or bits_b:
            # Bit-fields on one side, a word on the other: the words must tile the same bytes.
            words_a = sorted({o for o, _ in bits_a}); words_b = sorted({o for o, _ in bits_b})
            if [x for x in a if x[0] not in words_b] != [x for x in b if x[0] not in words_a] or \
                    any(all(x[0] != w for x in b) for w in words_a) or any(all(x[0] != w for x in a) for w in words_b):
                problems.append('%s: member layout differs around bit-fields: %s/%s against %s/%s' % (pair, b, bits_b, a, bits_a))
            else:
                known.append('%s: bit-fields %s stand where %s declares a word' % (
                    pair, [n for _, n in bits_a] or [n for _, n in bits_b],
                    'the module header' if bits_a else 'the other header'))
        names_a = {n: t for n, t, _, _ in first[1]}; names_b = {n: t for n, t, _, _ in other[1]}
        for n in sorted(set(names_a) & set(names_b)):
            if names_a[n] != names_b[n]:
                known.append('%s: member %s is %s against %s (same width)' % (pair, n, names_b[n], names_a[n]))
        for fname in sorted(set(first[2]) & set(other[2])):
            fa, fb = first[2][fname], other[2][fname]
            for key in ('access', 'virtual', 'const', 'ret', 'params'):
                if fa[key] != fb[key]:
                    problems.append('%s: %s %s %s against %s (changes the decorated name)' % (
                        pair, fname, key, fb[key], fa[key]))
        only_a = sorted(set(first[2]) - set(other[2])); only_b = sorted(set(other[2]) - set(first[2]))
        if only_a or only_b:
            known.append('%s: declares %s only; the module header %s only' % (pair, only_b or '-', only_a or '-'))
    return problems, known


def check_symbols(catalog, item, module_map=None):
    """Relocations of other units into the module must use the module's own symbols."""
    rows = [r for r in catalog['regions'] if r['source'] == item['source']]
    if not rows:
        return ['no accepted region names %s' % item['source']]
    entries = {r['entry']: r['symbol'] for r in rows}
    spans = [(int(a, 16), int(b, 16)) for a, b in item['data']]
    data_symbols = {d['symbol'] for d in (module_map or {}).get('data', [])}
    problems = []
    for region in catalog['regions']:
        if region['source'] == item['source']:
            continue
        for relocation in region.get('relocations') or []:
            target = relocation['target']
            if target in entries and relocation['symbol'] != entries[target]:
                problems.append('%08x refers to %08x as %s; the module defines %s' % (
                    region['entry'], target, relocation['symbol'], entries[target]))
            elif any(a <= target < b for a, b in spans) and data_symbols and relocation['symbol'] not in data_symbols:
                problems.append('%08x refers to module data %08x as %s, not a symbol of the module map' % (
                    region['entry'], target, relocation['symbol']))
    return problems


def load_modules(path=MODULES):
    return json.loads(path.read_text()) if path.exists() else {}


def check(catalog, modules=None, read=None):
    """All checks over the declared modules; (problems, known differences) with the module named."""
    problems, known = [], []
    for name, item in (modules if modules is not None else load_modules()).items():
        map_path = MAPS / (name + '.json')
        module_map = json.loads(map_path.read_text()) if map_path.exists() else None
        problems += ['%s: %s' % (name, p) for p in check_symbols(catalog, item, module_map)]
        if item.get('class') and item.get('declarations'):
            try:
                p, k = compare_declarations(item['class'], item['declarations'], read)
            except Refusal as error:
                p, k = [str(error)], []
            problems += ['%s: %s' % (name, x) for x in p]
            known += ['%s: %s' % (name, x) for x in k]
    return problems, known


def main():
    import catalog_io
    catalog = catalog_io.load(catalog_io.locate(REPOSITORY), references=False)
    problems, known = check(catalog)
    for line in known:
        print('known difference: ' + line)
    for line in problems:
        print('REFUSED: ' + line)
    if not problems:
        print('module agreement: %d module(s) pass' % len(load_modules()))
    raise SystemExit(1 if problems else 0)


if __name__ == '__main__':
    main()
