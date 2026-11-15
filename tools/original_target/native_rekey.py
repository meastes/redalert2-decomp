#!/usr/bin/env python3
"""Re-key the native tree for an apply_rename spec, from the spec itself (not by hand).

    native_rekey.py SPEC [--dry-run]

The native port names original functions and data in several spellings: MSVC decorated names
(native/data/address_aliases.tsv, owner_sizes.tsv, `__asm__` aliases in bss_globals.cpp and
original_data.cpp), Itanium manglings (signature_adapters.cpp, the link-closure baseline),
demangled `Scope::name` text and plain identifiers (drafts, link rewrites, initterm.json).
Every rename of the spec becomes the matching edits in each spelling:

  renames / overload_renames   the exact MSVC names; the identifier the decorated name carries
                               (`?NAME@SCOPE@@...`, a C name `_NAME`) becomes the new one in
                               Itanium form (`<len>SCOPE<len>NAME` -> lengths recomputed),
                               as `SCOPE::NAME`, and, for an unscoped name, as a whole identifier;
  member_renames               `<len>CLASS<len>OLD`, `CLASS::OLD`, and the class's decorated names.

A member access written `.OLD` / `->OLD` in native code is not re-keyed (its static type is not
known here): such tokens are listed under `review` and the native build is the check. A rename
that changes only a signature (same identifier) re-keys its MSVC names; its Itanium manglings are
listed under `review`. Only tracked text files under native/ are edited.
"""
import json
import re
import subprocess
import sys
from pathlib import Path

REPOSITORY = Path(__file__).resolve().parents[2]


def identifier(mangled):
    """(name, [scopes innermost first]) the decorated name spells, or None (operators, templates)."""
    if mangled.startswith('?') and not mangled.startswith(('??', '?$')):
        head = mangled[1:].split('@@', 1)[0]
        parts = head.split('@')
        if all(re.fullmatch(r'[A-Za-z_]\w*', p) for p in parts):
            return parts[0], parts[1:]
        return None
    if re.fullmatch(r'_[A-Za-z_]\w*', mangled):
        return mangled[1:], []
    return None


def itanium(name, scopes):
    """The Itanium nested-name spelling of `name` in `scopes` (innermost first)."""
    return ''.join('%d%s' % (len(s), s) for s in reversed(scopes)) + '%d%s' % (len(name), name)


def rules(spec):
    """[(kind, compiled pattern, replacement)] for the spec's renames."""
    out, review = [], []
    pairs = dict(spec.get('renames') or {})
    if isinstance(spec.get('renames'), str):
        pairs = json.loads((REPOSITORY / spec['renames']).read_text())
    for old, item in (spec.get('overload_renames') or {}).items():
        pairs[old] = item['new']
    for old, item in (spec.get('address_renames') or {}).items():
        review.append('address-scoped rename %s -> %s: re-key by hand (native names carry no address)' % (old, item['new']))
    seen = set()
    for old, new in sorted(pairs.items(), key=lambda kv: -len(kv[0])):
        out.append(('msvc', re.compile(r'(?<![\w?@$])%s(?![\w@])' % re.escape(old)), new))
        a, b = identifier(old), identifier(new)
        if not a or not b:
            continue
        (oname, oscopes), (nname, nscopes) = a, b
        if (oname, tuple(oscopes)) == (nname, tuple(nscopes)):
            review.append('%s -> %s changes only the signature: Itanium manglings not re-keyed' % (old, new))
            continue
        key = (oname, tuple(oscopes), nname, tuple(nscopes))
        if key in seen:
            continue
        seen.add(key)
        out += name_rules(oname, oscopes, nname, nscopes)
    for item in spec.get('global_renames') or []:
        out.append(('msvc-prefix', re.compile(r'(?<![\w?@$])\?%s@@' % re.escape(item['old'])), '?%s@@' % item['new']))
        out += name_rules(item['old'], [], item['new'], [])
    for item in spec.get('function_renames') or []:
        out.append(('msvc-prefix', re.compile(r'(?<![\w?@$])\?%s@@' % re.escape(item['old'])), '?%s@@' % item['new']))
        out += name_rules(item['old'], [], item['new'], [])
    for item in spec.get('member_renames') or []:
        scopes = [item['class']]
        out += name_rules(item['old'], scopes, item['new'], scopes)
        out.append(('msvc-prefix', re.compile(r'\?%s@%s@' % (re.escape(item['old']), re.escape(item['class']))),
                    '?%s@%s@' % (item['new'], item['class'])))
        out.append(('offsetof', re.compile(r'(offsetof\(\s*%s\s*,\s*)%s\b' % (re.escape(item['class']), re.escape(item['old']))),
                    r'\g<1>%s' % item['new']))
    return out, review


def name_rules(oname, oscopes, nname, nscopes):
    # the length prefixes fix the extent; only a digit before would change the first length
    # The length prefixes fix the extent. A free function's name follows `_Z` directly (a
    # scoped one would otherwise match inside another scope's `<len>Scope<len>name`).
    lead = r'(?<![0-9])' if oscopes else r'(?<=_Z)'
    rules_ = [('itanium', re.compile(lead + re.escape(itanium(oname, oscopes))), itanium(nname, nscopes))]
    if oscopes:
        rules_.append(('scoped', re.compile(r'(?<![\w:])%s::%s(?!\w)' % ('::'.join(map(re.escape, reversed(oscopes))),
                                                                          re.escape(oname))),
                       '%s::%s' % ('::'.join(reversed(nscopes)), nname)))
    else:
        rules_.append(('identifier', re.compile(r'(?<![\w$?@])%s(?!\w)' % re.escape(oname)), nname))
        rules_.append(('c-symbol', re.compile(r'(?<![\w$?@])_%s(?!\w)' % re.escape(oname)), '_' + nname))
    return rules_


def tracked_native():
    files = subprocess.run(['git', 'ls-files', 'native'], cwd=REPOSITORY, capture_output=True, text=True).stdout.split()
    for path in files:
        p = REPOSITORY / path
        if p.suffix in ('.png', '.bmp', '.wav', '.pal', '.bin', '.ico', '.icns'):
            continue
        try:
            yield path, p.read_text(encoding='utf-8')
        except (UnicodeDecodeError, IsADirectoryError, FileNotFoundError):
            continue


def rekey(spec, dry_run=False):
    """{path: {kind: count}} of the edits; writes unless dry_run. Also returns review notes."""
    rule_list, review = rules(spec)
    changed = {}
    members = [(m['class'], m['old']) for m in spec.get('member_renames') or []]
    for path, text in tracked_native():
        new, counts = text, {}
        for kind, pattern, replacement in rule_list:
            new, n = pattern.subn(replacement, new)
            if n:
                counts[kind] = counts.get(kind, 0) + n
        for klass, old in members:
            for match in re.finditer(r'(?:\.|->)%s\b' % re.escape(old), new):
                review.append('%s:%d: member access %s (type not known here; the native build checks it)' % (
                    path, new.count('\n', 0, match.start()) + 1, match.group(0)))
        if new != text:
            changed[path] = counts
            if not dry_run:
                (REPOSITORY / path).write_text(new, encoding='utf-8')
    return changed, review


def main():
    import argparse
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('spec', type=Path)
    parser.add_argument('--dry-run', action='store_true')
    args = parser.parse_args()
    spec = json.loads(args.spec.read_text())
    changed, review = rekey(spec, args.dry_run)
    for path, counts in sorted(changed.items()):
        print('%s %s' % (path, ' '.join('%s=%d' % kv for kv in sorted(counts.items()))))
    for note in review:
        print('review: ' + note)
    print('native_rekey: %d files %s' % (len(changed), 'would change' if args.dry_run else 'changed'))


if __name__ == '__main__':
    main()
