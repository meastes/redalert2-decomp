#!/usr/bin/env python3
"""Name-keyed native rules that match nothing (ctest native-name-rules).

Several native tools key a rule on an identifier of the recovered source: a regex
in native_units.py's RULES, a fixups.json rename, view_audit.py's view notes, a
member/bss alias row. A rename in src/ that the rule does not follow leaves the
rule silently matching nothing (lane C22: native_units.py's whole-table rule for
the RadiusCount table 0x007A5908 kept the old name unknown_radius_count_7a5908
after a rename to MapClass_RadiusCount, the native table shrank to 4 bytes and
tank shells damaged no cell). This check fails when such a rule keys on a name
the tree no longer has:

  native_units.py   every rule and module regex must match the source corpus,
                    and every identifier it spells must still be (part of) an
                    identifier in src/, the drafts, or the port (a CRT/Win32 name
                    the port implements);
  fixups.json       every rename's `from` must still occur in src/;
  view_audit.py     every NOT_VIEWS / VIEW_OF key must still be a class in src/;
  member_aliases.tsv, bss_aliases.tsv
                    every row's name must be used by src/ or native/, or be the
                    target of another alias row (a chain such as
                    unknown_short_8325d0 -> DAT_008325d0 -> Map.MapSize.Width).

    python3 native/tools/name_rules_check.py [--root DIR]
"""
import argparse
import csv
import glob
import json
import re
import sys
from pathlib import Path

IDENT = re.compile(r'[A-Za-z_]\w*')


def corpus(root, patterns):
    texts = []
    for pattern in patterns:
        for path in glob.glob(str(root / pattern), recursive=True):
            try:
                texts.append(Path(path).read_text(errors='replace'))
            except (IsADirectoryError, OSError):
                pass
    return '\n'.join(texts)


RECOVERED = re.compile(r'^(?:unknown|Unknown|seed|Leaf|Owner|Callee|FUN|DAT|PTR)_|_[0-9a-fA-F]{6,8}_?$|[A-Z]\w*_|_[A-Z]')


def regex_names(pattern):
    """Recovered-source identifiers a regex spells (escapes such as \\b and \\s dropped): placeholder
    and address-suffixed names and mixed-case names with an underscore (MapClass_RadiusCount,
    Wrap_Overflow). Lower-case CRT names (wcsicmp, _snprintf) and type keywords are not keys on
    the recovered source."""
    text = re.sub(r'\\[A-Za-z]', ' ', pattern)
    return {t for t in IDENT.findall(text) if len(t) >= 5 and not t.isupper() and RECOVERED.search(t)}


WORD_GROUP = re.compile(r'\((?:\?:)?([A-Za-z0-9_]+(?:\|[A-Za-z0-9_]+)+)\)')


def expansions(pattern, limit=64):
    """The pattern with each group of plain word alternatives ((?:count|starts)) expanded, so every
    spelled name is checked on its own (unknown_radius_(?:count|starts)_7a5908 names two)."""
    out, todo = [], [pattern]
    while todo and len(out) + len(todo) < limit:
        p = todo.pop()
        m = WORD_GROUP.search(p)
        if not m:
            out.append(p)
            continue
        todo.extend(p[:m.start()] + alt + p[m.end():] for alt in m.group(1).split('|'))
    return out + todo


def itanium_last(symbol):
    """The last name of an Itanium symbol (__ZN9OwnerDraw23unknown_registry_a73640E -> unknown_registry_a73640)."""
    i, last = symbol.find('N') + 1 if symbol.startswith('__ZN') else 3, None
    while i < len(symbol) and symbol[i].isdigit():
        j = i
        while symbol[j].isdigit():
            j += 1
        n = int(symbol[i:j])
        last = symbol[j:j + n]
        i = j + n
    return last or symbol


def table(path):
    with open(path) as f:
        return list(csv.DictReader((line for line in f if not line.startswith('#')), delimiter='\t'))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--root', type=Path, default=Path(__file__).resolve().parents[2])
    args = ap.parse_args()
    root = args.root
    sys.path.insert(0, str(root / 'native/tools'))
    import native_units

    src = corpus(root, ['src/**/*.cpp', 'src/**/*.hpp', 'src/**/*.h', 'native/drafts/*.cpp'])
    src_names = set(IDENT.findall(src))
    port_names = set(IDENT.findall(corpus(root, ['native/port/**/*.h', 'native/port/**/*.hpp', 'native/port/**/*.cpp'])))
    native_names = set(IDENT.findall(corpus(root, ['native/link/*.cpp', 'native/link/*.h', 'native/data/*.cpp',
                                                   'native/platform/**/*.cpp', 'native/port/**/*.cpp'])))
    joined = ' ' + ' '.join(sorted(src_names)) + ' '
    problems = []

    rules = [('RULES[%d]' % i, rx) for i, (rx, _) in enumerate(native_units.RULES)]
    rules += [(name, value) for name, value in sorted(vars(native_units).items()) if isinstance(value, re.Pattern)]
    for key, rx in rules:
        names = set().union(*(regex_names(p) for p in expansions(rx.pattern)))
        if names and not rx.search(src):
            problems.append('native_units.py %s matches nothing in src/ or the drafts: %s' % (key, rx.pattern[:120]))
        for name in sorted(names):
            if name in port_names or name in src_names:
                continue
            if not re.search(r'_[0-9a-fA-F]{6,8}$', name) and name in joined:
                continue    # a fragment of a longer name (Wrap_ of Wrap_Overflow)
            problems.append('native_units.py %s spells %s, which src/ no longer has (renamed? the rule must '
                            'take the new spelling)' % (key, name))

    fixups = json.loads((root / 'native/port/fixups.json').read_text())
    for r in fixups.get('renames', []):
        if not re.search(r'(?<![\w:])%s\b' % re.escape(r['from']), src):
            problems.append('fixups.json rename %s -> %s: %s no longer occurs in src/' % (r['from'], r['to'], r['from']))

    view_audit = (root / 'native/tools/view_audit.py').read_text()
    for block in ('NOT_VIEWS', 'VIEW_OF'):
        m = re.search(r'^%s = \{(.*?)^\}' % block, view_audit, re.S | re.M)
        for key in re.findall(r"^\s*'(\w+)':", m.group(1) if m else '', re.M):
            if key not in src_names:
                problems.append('view_audit.py %s names %s, which src/ no longer has' % (block, key))

    used = src_names | native_names
    rows = [('member_aliases.tsv', r['symbol'], r['owner_symbol']) for r in table(root / 'native/data/member_aliases.tsv')]
    rows += [('bss_aliases.tsv', r['alias'], r['target']) for r in table(root / 'native/data/bss_aliases.tsv')]
    targets = {target for _, _, target in rows}
    for name, symbol, _ in rows:
        plain = itanium_last(symbol) if symbol.startswith('__Z') else symbol[1:] if symbol.startswith('_') else symbol
        if plain in used or symbol in targets:
            continue
        problems.append('%s: %s is used by neither src/ nor native/ and is no alias target (stale row)' % (name, symbol))

    if problems:
        print('name-keyed native rules that match nothing (%d):' % len(problems))
        for p in problems:
            print('  ' + p)
        return 1
    print('name-keyed native rules: all %d native_units rules, %d fixup renames, view notes and %d alias rows '
          'key on names the tree has' % (len(rules), len(fixups.get('renames', [])), len(rows)))
    return 0


if __name__ == '__main__':
    sys.exit(main())
