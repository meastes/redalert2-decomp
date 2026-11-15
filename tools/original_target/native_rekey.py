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

A member access written `.OLD` / `->OLD` (or an unqualified use inside the class's own code) in native
code is typed by the native compiler: with `--build DIR` the tool builds the native tree and renames
each use the compiler reports as the old member (`no member named 'OLD' in 'Y'`, `use of undeclared
identifier 'OLD'`), then builds again until it passes; an error it cannot explain refuses. A rename
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


SELF_SET = re.compile(r'^__asm__\("\.globl (_\w+)\\n\.set \1, \1\\n"\);[^\n]*\n', re.M)


def merged(path, text):
    """After a rename merges two names of one entity: an alias table row whose alias and target
    are now one name, a repeated row, and a `.set X, X` alias line go."""
    text = SELF_SET.sub('', text)
    if not path.endswith('.tsv'):
        return text
    out, seen = [], set()
    for line in text.split('\n'):
        cells = line.split('\t')
        if line and not line.startswith('#'):
            if path.endswith('aliases.tsv') and len(cells) > 1 and cells[0] == cells[1]:
                continue
            if line in seen:
                continue
            seen.add(line)
        out.append(line)
    return '\n'.join(out)


FIXUPS = 'native/port/fixups.json'


def fixups_follow(spec, dry_run=False):
    """native/port/fixups.json patches accepted source text (`source`, `old` -> `new`). When the batch's
    member renames changed that text, a fixup whose `old` no longer occurs is re-keyed: the member
    renames are applied to its `old` and `new`, and kept only when the renamed `old` occurs in the
    source exactly (the source's tokens were renamed by type, so the same tokens follow). Returns the
    re-keyed entries' sources."""
    members = spec.get('member_renames') or []
    path = REPOSITORY / FIXUPS
    if not members or not path.exists():
        return []
    document = json.loads(path.read_text())
    done = []
    for entry in document.get('fixups', []):
        source = REPOSITORY / entry.get('source', '')
        if not entry.get('source') or not source.exists():
            continue
        text = source.read_text(encoding='latin-1')
        if entry['old'] in text:
            continue
        # each rename is taken only where it brings the text closer to the source: the same spelling
        # may name another class's member in the fixup (e.g. a view's padding array) and stays
        def present(candidate):
            return sum(line in text for line in candidate.split('\n'))
        old, new = entry['old'], entry['new']
        taken = []
        for m in members:
            pattern = re.compile(r'(?<![\w$?@])%s(?!\w)' % re.escape(m['old']))
            for match in reversed(list(pattern.finditer(old))):
                trial = old[:match.start()] + m['new'] + old[match.end():]
                if present(trial) > present(old):
                    old = trial
                    taken.append(pattern)
        # the renames the fixup's `old` took follow into its replacement text (the same members, in
        # the replacement's own layout); renames it did not take stay out of it
        for pattern in taken:
            new = pattern.sub(next(m['new'] for m in members if pattern.pattern.endswith(re.escape(m['old']) + r'(?!\w)')), new)
        if old != entry['old'] and old in text:
            entry['old'], entry['new'] = old, new
            done.append(entry['source'])
    if done and not dry_run:
        path.write_text(json.dumps(document, indent=1) + '\n')
    return done


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
            new = merged(path, new)
        if new != text:
            changed[path] = counts
            if not dry_run:
                (REPOSITORY / path).write_text(new, encoding='utf-8')
    for source in fixups_follow(spec, dry_run):
        changed.setdefault(FIXUPS, {}).setdefault('fixup', 0)
        changed[FIXUPS]['fixup'] += 1
    return changed, review


COMPILER_ERROR = re.compile(r"(/[^:\s]+?\.(?:cpp|hpp|h)):(\d+):(\d+): error: "
                            r"(?:no member named '(\w+)' in '(?:[\w:]+::)?(\w+)'|use of undeclared identifier '(\w+)')")


def compiler_places(output, members):
    """[(path, line, column, old, class or None)] of compiler errors that name a member rename's old
    name: the compiler resolved the use's static type and found only the new member there."""
    olds = {m['old'] for m in members}
    places = set()
    for path, line, column, member, owner, undeclared in COMPILER_ERROR.findall(output):
        name = member or undeclared
        if name in olds:
            places.add((path, int(line), int(column), name, owner or None))
    return sorted(places)


def rename_at(path, line, column, old, new):
    """Rename `old` at path:line:column (the compiler's location; inside a macro argument it
    points at the macro, so the line's one spelling of `old` is taken). False when unplaceable."""
    lines = Path(path).read_text(encoding='utf-8').split('\n')
    text = lines[line - 1]
    if text[column - 1:column - 1 + len(old)] == old:
        at = column - 1
    else:
        found = [m.start() for m in re.finditer(r'\b%s\b' % re.escape(old), text)]
        if len(found) != 1:
            return False
        at = found[0]
    lines[line - 1] = text[:at] + new + text[at + len(old):]
    Path(path).write_text('\n'.join(lines), encoding='utf-8')
    return True


def compiler_rekey(spec, build_dir, rounds=30, build=None):
    """Member renames in native-only code by the native compiler's type resolution: build, rename
    each use the compiler reports as the old member name (its class now has only the new one),
    and build again until the build passes. Refuses (SystemExit) when an error it cannot place,
    or another error, remains. Returns [(path, line, old, class)] of the renames made."""
    members = spec.get('member_renames') or []
    new_of = {m['old']: m['new'] for m in members}
    if build is None:
        def build():
            # -k 0: every failing unit reports in one round, not only the first
            result = subprocess.run(['cmake', '--build', str(build_dir), '--', '-k', '0'], cwd=REPOSITORY,
                                    capture_output=True, text=True)
            return result.returncode, result.stdout + result.stderr
    made = []
    for _ in range(rounds):
        code, output = build()
        if code == 0:
            return made
        places = compiler_places(output, members)
        if not places:
            errors = [l for l in output.splitlines() if ' error: ' in l][:8]
            raise SystemExit('native_rekey: the native build fails with errors the renames do not explain:\n' + '\n'.join(errors))
        # bottom-up, so earlier columns of a line stay where the compiler saw them
        for path, line, column, old, owner in sorted(places, key=lambda p: (p[0], -p[1], -p[2])):
            if not path.startswith(str(REPOSITORY / 'native')) and '/native/' not in path:
                raise SystemExit('native_rekey: %s:%d names %s outside native/ (a source the batch renames)' % (path, line, old))
            if not rename_at(path, line, column, old, new_of[old]):
                raise SystemExit('native_rekey: cannot place %s at %s:%d' % (old, path, line))
            made.append((Path(path).resolve().relative_to(REPOSITORY.resolve()).as_posix(), line, old, owner))
    raise SystemExit('native_rekey: the native build still fails after %d rounds' % rounds)


def main():
    import argparse
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('spec', type=Path)
    parser.add_argument('--dry-run', action='store_true')
    parser.add_argument('--build', type=Path, help='a configured native build folder: after the textual re-key, '
                        'member uses only the compiler can type are renamed from its errors (compiler_rekey)')
    args = parser.parse_args()
    spec = json.loads(args.spec.read_text())
    changed, review = rekey(spec, args.dry_run)
    for path, counts in sorted(changed.items()):
        print('%s %s' % (path, ' '.join('%s=%d' % kv for kv in sorted(counts.items()))))
    for note in review:
        print('review: ' + note)
    print('native_rekey: %d files %s' % (len(changed), 'would change' if args.dry_run else 'changed'))
    if args.build and not args.dry_run and spec.get('member_renames'):
        for path, line, old, owner in compiler_rekey(spec, args.build):
            print('compiler: %s:%d %s (in %s)' % (path, line, old, owner or 'its class scope'))


if __name__ == '__main__':
    main()
