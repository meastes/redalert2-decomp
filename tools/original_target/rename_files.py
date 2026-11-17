#!/usr/bin/env python3
"""Address-free source file names (review session's call 2026-11-13, owner rulings; step 2 of
docs/HANDOFF.md "no address in source file names").

    rename_files.py plan [--list]          what would move, and what keeps its address and why
    rename_files.py apply --count N        the next N moves as one apply_rename batch, then the
                                           re-key of every tracked text that names a moved path
    rename_files.py rekey-text MOVES DIR --scope SUB...
                                           re-key the old paths in the text files of DIR's
                                           subfolders SUB (e.g. the Windows coordination folder's
                                           assignments and status; dated records stay as written)

A per-function source file (one accepted row) whose name carries an address gets the name of its
function without the address:
  - under classes/<Class>/ it stays in its folder and becomes <Method>.cpp;
  - elsewhere it moves to src/recovered/<unit>/<Class>_<Method>.cpp (<Function>.cpp for a free
    function; <unit> from manifests/original_target/unit_map.json).
Constructors are <Class>_Constructor, destructors <Class>_Destructor, scalar and vector deleting
destructors <Class>_Scalar_Deleting_Destructor / _Vector_Deleting_Destructor. A `seed_<addr>`
namespace is not part of the name. Files that keep their address name (counted in STATUS,
"Address-named source files remaining"):
  - collision: the new name is not unique in its folder (case-insensitive, existing files included);
  - seed name: the function's own name is a placeholder (FUN_, a Leaf/Call/Cleanup view, a $E
    initialiser, a conversion operator or array iterator, a name that is only an address);
Operators take the word form (operator_equal, operator_not_equal, operator_assign, ...: OPERATORS).
  - no spelling: a template class outside the class folders (the folder spelling is unknown) or a
    row with no unit.
Object bytes do not depend on the file name: every compile group is staged as accepted.cpp
(build.stage_fixed); proven on 20 files compiled under both names (lane F, build/name-proof).
"""
import argparse
import collections
import json
from pathlib import Path
import re
import subprocess
import sys
import time

REPOSITORY = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(Path(__file__).resolve().parent))
import catalog_io

ADDRESS_TOKEN = re.compile(r'(?:^|_)(?:0x)?(?=[0-9a-fA-F]*\d)[0-9a-fA-F]{6,8}(?=_|$)')
SEED_NAMESPACE = re.compile(r'^seed_[0-9a-fA-F]{6,8}$')
VIEW_CLASS = re.compile(r'^(?:Leaf|Call|Cleanup|Target|Append|Thunk|View)_[0-9a-fA-F]{6,8}(?:_\d+)?$')
SPECIAL = {'?0': 'Constructor', '?1': 'Destructor', '?_G': 'Scalar_Deleting_Destructor',
           '?_E': 'Vector_Deleting_Destructor'}
# Operators in the word form the tree's files already use (owner ruling 2026-11-13, DECISIONS
# "Own-population rows frozen under another option set; wonline /GR; operator file names").
# Conversion operators (??B, named by their target type) and the compiler's array iterators
# (??_H, ??_I) have no word here and keep their address.
OPERATORS = {'?2': 'operator_new', '?3': 'operator_delete', '?4': 'operator_assign', '?7': 'operator_not',
             '?8': 'operator_equal', '?9': 'operator_not_equal', '?A': 'operator_index', '?C': 'operator_arrow',
             '?D': 'operator_star', '?E': 'operator_increment', '?F': 'operator_decrement', '?G': 'operator_minus',
             '?H': 'operator_plus', '?I': 'operator_and', '?K': 'operator_divide', '?L': 'operator_modulo',
             '?M': 'operator_less', '?N': 'operator_less_equal', '?O': 'operator_greater',
             '?P': 'operator_greater_equal', '?R': 'operator_call', '?X': 'operator_multiply_assign',
             '?Y': 'operator_plus_assign', '?Z': 'operator_minus_assign', '?_0': 'operator_divide_assign',
             '?_2': 'operator_and_assign', '?_3': 'operator_or_assign', '?U': 'operator_or',
             '?S': 'operator_complement', '?_U': 'operator_new_array', '?_V': 'operator_delete_array'}
TEXT_SUFFIXES = {'.json', '.tsv', '.md', '.cpp', '.hpp', '.h', '.txt', '.py', '.cmake', '.sh'}


def address_named(path):
    return bool(ADDRESS_TOKEN.search(Path(path).stem))


def strip_address(name):
    return re.sub(r'_+', '_', ADDRESS_TOKEN.sub('', name)).strip('_')


def function_name(symbol):
    """(own name, rest of the symbol after it) of an MSVC symbol, or None for an operator or
    another special name; a C symbol (`_name`) gives (name, '')."""
    if not symbol.startswith('?'):
        bare = re.sub(r'@\d+$', '', symbol[1:] if symbol.startswith('_') else symbol)
        return (bare, '') if re.fullmatch(r'[A-Za-z_]\w*', bare) else None
    body = symbol[1:]
    for code in ('?_G', '?_E', '?0', '?1'):
        if body.startswith(code):
            return SPECIAL[code], body[len(code):]
    for code in sorted(OPERATORS, key=len, reverse=True):
        if body.startswith(code):
            return OPERATORS[code], body[len(code):]
    match = re.match(r'([A-Za-z_]\w*)@', body)
    return (match.group(1), body[match.end():]) if match else None


def plain_scopes(rest):
    """The scopes (innermost first) of the rest of a symbol, or None when one is a template."""
    scopes = []
    while not rest.startswith('@'):
        if rest.startswith('?$'):
            return None
        match = re.match(r'([A-Za-z_]\w*)@', rest)
        if not match:
            return None
        scopes.append(match.group(1))
        rest = rest[match.end():]
    return scopes


def new_name(row, in_class_folder):
    """(file stem, None) or (None, reason) for one row's file: <Method> in a class folder,
    <Class>_<Method> (or <Function>) elsewhere."""
    named = function_name(row['symbol'])
    if named is None:
        return None, 'seed name'
    name, rest = named
    if re.match(r'^(?:FUN|LAB|SUB|thunk_FUN)_', name) or name.startswith('$'):
        return None, 'seed name'
    method = strip_address(name)
    if not method or method in ('unknown', 'Unknown', 'FUN', 'get', 'target', 'cleanup'):
        return None, 'seed name'
    view = re.match(r'(?:Leaf|Call|Cleanup|Target|Append|Storage|Thunk|View)_[0-9a-fA-F]{6,8}(?:_\d+)?@', rest)
    if view:
        return None, 'seed name'
    if in_class_folder:
        return method, None
    scopes = plain_scopes(rest)
    if scopes is None:
        return None, 'no spelling'
    scopes = [x for x in scopes if not SEED_NAMESPACE.match(x)]
    if any(ADDRESS_TOKEN.search(x) for x in scopes):
        return None, 'seed name'
    if (method in SPECIAL.values() or method in OPERATORS.values()) and not scopes:
        return None, 'seed name'
    return '_'.join(list(reversed(scopes)) + [method]), None


def plan(catalog=None):
    """{'moves': {old: new}, 'kept': {path: reason}}."""
    import refile_unit
    catalog = catalog or catalog_io.load(catalog_io.locate(REPOSITORY), references=False)
    by_source = collections.defaultdict(list)
    for row in catalog['regions']:
        if row.get('source') and row.get('symbol'):
            by_source[row['source']].append(row)
    unit_of = {e: u for u, entries in refile_unit.load_unit_map().items() for e in entries}
    tracked = subprocess.run(['git', 'ls-files', 'src/recovered'], cwd=REPOSITORY, capture_output=True, text=True,
                             check=True).stdout.split()
    existing = collections.defaultdict(set)
    for path in tracked:
        existing[Path(path).parent.as_posix()].add(Path(path).name.lower())
    proposals, kept = {}, {}
    for path in sorted(p for p in tracked if p.endswith('.cpp') and address_named(p)):
        rows = by_source.get(path, [])
        if len(rows) != 1:
            kept[path] = 'not one accepted row (%d)' % len(rows)
            continue
        row = rows[0]
        parts = Path(path).parts
        in_class = len(parts) > 4 and parts[2] == 'classes'
        stem, reason = new_name(row, in_class)
        if stem is None:
            kept[path] = reason
            continue
        if in_class:
            folder = Path(path).parent.as_posix()
        else:
            unit = unit_of.get(row['entry'])
            current = Path(path).parent.as_posix()
            if unit is not None:
                folder = 'src/recovered/%s' % unit
            elif current not in ('src/recovered', 'src/recovered/unowned') and not ADDRESS_TOKEN.search(current):
                folder = current                          # no unit: renamed in its named folder
            else:
                kept[path] = 'no spelling'
                continue
        proposals[path] = '%s/%s.cpp' % (folder, stem)
    # unique per folder, case-insensitive, against the files that stay too
    taken = collections.Counter(new.lower() for new in proposals.values())
    moves = {}
    for old, new in proposals.items():
        folder, name = new.rsplit('/', 1)
        stays = existing[folder] - ({Path(old).name.lower()} if Path(old).parent.as_posix() == folder else set())
        if taken[new.lower()] > 1 or name.lower() in stays:
            kept[old] = 'collision'
        else:
            moves[old] = new
    return dict(moves=moves, kept=kept)


def remaining_counts(catalog=None):
    """{reason: files} of the address-named files that stay (the STATUS line)."""
    result = plan(catalog)
    counts = collections.Counter(result['kept'].values())
    counts['renameable'] = len(result['moves'])
    return counts


def tracked_texts():
    out = subprocess.run(['git', 'ls-files'], cwd=REPOSITORY, capture_output=True, text=True, check=True).stdout.split('\n')
    for path in out:
        if not path or path.startswith(('src/', 'docs/history/')) or Path(path).suffix not in TEXT_SUFFIXES:
            continue
        if re.search(r'_20\d{6}\.', Path(path).name):          # dated records stay as written
            continue
        yield path


def path_pattern(moves):
    return re.compile(r'(?<![\w/.-])(%s)(?![\w/.-])' % '|'.join(map(re.escape, sorted(moves, key=len, reverse=True))))


def rekey_texts(moves, paths, root=REPOSITORY):
    pattern = path_pattern(moves)
    changed = 0
    for path in paths:
        file = root / path
        try:
            text = file.read_text()
        except (UnicodeDecodeError, OSError):
            continue
        new = pattern.sub(lambda m: moves[m.group(1)], text)
        if new != text:
            file.write_text(new)
            changed += 1
    return changed


def apply(count, config_path):
    started = time.monotonic()
    result = plan()
    batch = dict(sorted(result['moves'].items())[:count])
    if not batch:
        raise SystemExit('nothing to rename')
    folder = REPOSITORY / 'build/rename-files' / ('batch-%d' % time.time_ns())
    folder.mkdir(parents=True)
    # src texts that name a moved path (comments) are re-keyed inside the same verified batch
    rules = [dict(pattern=r'(?<![\w/.-])%s(?![\w/.-])' % re.escape(old), replace=new) for old, new in batch.items()]
    named = subprocess.run(['git', 'grep', '-l', '-F'] + sum((['-e', old] for old in batch), []) + ['--', 'src'],
                           cwd=REPOSITORY, capture_output=True, text=True).stdout.split()
    spec = dict(name=folder.name, renames={}, moves=batch, rewrite=rules if named else [])
    catalog_io.write_text_lf(folder / 'spec.json', json.dumps(spec, indent=1) + '\n')
    run = subprocess.run([sys.executable, '-B', str(Path(__file__).with_name('apply_rename.py')), str(folder / 'spec.json'),
                          '--config', str(config_path), '--output', str(folder / 'out')], cwd=REPOSITORY,
                         capture_output=True, text=True)
    catalog_io.write_text_lf(folder / 'apply.log', run.stdout + run.stderr)
    if run.returncode:
        raise SystemExit('apply_rename refused (%s):\n%s' % (folder / 'apply.log', (run.stdout + run.stderr)[-2000:]))
    changed = rekey_texts(batch, list(tracked_texts()))
    subprocess.run([sys.executable, '-B', str(REPOSITORY / 'native/tools/initterm_manifest.py')], cwd=REPOSITORY,
                   capture_output=True, text=True)
    catalog_io.write_text_lf(folder / 'moves.json', json.dumps(batch, indent=1) + '\n')
    print('renamed %d files; %d tracked texts re-keyed; %d src texts named a moved path; moves %s (%.0fs)' % (
        len(batch), changed, len(named), folder / 'moves.json', time.monotonic() - started))


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = parser.add_subparsers(dest='command', required=True)
    p = sub.add_parser('plan'); p.add_argument('--list', action='store_true')
    p = sub.add_parser('apply'); p.add_argument('--count', type=int, default=500)
    p.add_argument('--config', type=Path, default=catalog_io.config_path(REPOSITORY))
    p = sub.add_parser('rekey-text'); p.add_argument('moves', type=Path); p.add_argument('directory', type=Path)
    p.add_argument('--scope', action='append', required=True, metavar='SUBFOLDER',
                   help='only these subfolders of DIRECTORY (e.g. assignments, status); deliveries, messages and '
                        'other dated records stay as written')
    args = parser.parse_args()
    if args.command == 'plan':
        result = plan()
        counts = collections.Counter(result['kept'].values())
        print(json.dumps(dict(renameable=len(result['moves']), keep=dict(counts)), indent=1))
        if args.list:
            for old, new in sorted(result['moves'].items()):
                print('%s -> %s' % (old, new))
            for old, reason in sorted(result['kept'].items()):
                print('keep %s (%s)' % (old, reason))
    elif args.command == 'apply':
        apply(args.count, args.config)
    elif args.command == 'rekey-text':
        moves = json.loads(args.moves.read_text())
        root = args.directory.resolve()
        paths = [p.relative_to(root).as_posix() for scope in args.scope for p in (root / scope).rglob('*')
                 if p.is_file() and p.suffix in TEXT_SUFFIXES and not re.search(r'20\d{6}', p.name)]
        print('%d files re-keyed' % rekey_texts(moves, paths, root))


if __name__ == '__main__':
    main()
