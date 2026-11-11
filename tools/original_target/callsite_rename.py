#!/usr/bin/env python3
"""Type-first member rename for a class family: the compiler finds the call sites.

    callsite_rename.py CFG.json --output DIR [--repository R] [--config C] [--max-iter N]
    callsite_rename.py native SPEC.json CALLSITES.txt [--repository R] [--build-dir B]

CFG may also hold parts [{root|family, methods}, ...] (several families in one spec, e.g. slots that sibling branches
introduce under different names); headers shared by stacked specs conflict in apply_rename's merge, so such slots go in one spec.

CFG keys: name; root (the family is the root and every class deriving from it, transitively, by the src/shared headers) or family
[classes]; methods [[old, new], ...]; optional dirs {class: directory} and prefixes {class: file prefix} for class files that live in
src/recovered/classes/<dir>/<prefix>_<method>_<addr>.cpp.

Source pass (default):
1. the old names inside every family class body in src/shared (brace-matched) become the new names;
2. in the family's class files, `Fam::old` and calls without a receiver become the new names;
3. every compile group (population + options, sources concatenated in source order as build.py compiles them) whose sources spell
   an old name is compiled; each `error C2039: 'old' : is not a member of 'Fam'` names a call site, and the old names on that
   source or header line become the new names; a line that then reports `'new' : is not a member of 'X'` (X outside the family)
   is put back. Repeat until clean or --max-iter.
4. DIR receives <name>.json (an apply_rename spec: family-limited mangling rules, staged copies of every changed file with their
   @ref manglings renamed, file moves), stage/ and callsites.txt (the family, then every call site the compiler found); the
   repository's files are restored. DIR must be inside the repository (apply_rename reads the staged copies by repository path).
   Compile folders go to <repository>/build/callsite-loop/<name>/ (the remote compiler maps only repository paths).

Native pass (`native`, run after the spec has been applied): the spec's file moves, family-limited Itanium (<n>Fam<n>old), MSVC (old@Fam@) and qualified
(Fam::old) spellings under native/, then reconfigure + `cmake --build -k 0` and rename each clang `no member named 'old' in 'Fam'` at
its line and column, repeating until no such error (at most 6 rounds). Prints `total leftovers in native/: N`.
"""
import argparse
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys

TOOLS = Path(__file__).resolve().parent
CLASS_HEAD = r'\b(?:class|struct)\s+(\w+)\s*:\s*([^{;]+)\{'


def read(path):
    with open(path, encoding='latin-1', newline='') as f:
        return f.read()


def write(path, text):
    with open(path, 'w', encoding='latin-1', newline='') as f:
        f.write(text)


def base_map(texts):
    """{class: {direct bases}} from header texts."""
    bases = {}
    for t in texts:
        for m in re.finditer(CLASS_HEAD, t):
            bases.setdefault(m.group(1), set()).update(
                re.findall(r'(?:public|protected|private)?\s*(\w+)\s*(?:,|$)', m.group(2).strip()))
    return bases


def family_of(root, bases):
    family = {root}
    while True:
        more = {c for c, bs in bases.items() if bs & family} - family
        if not more:
            return family
        family |= more


def family_pattern(family):
    return '|'.join(sorted(family, key=len, reverse=True))


def rename_names(text, methods):
    for old, new in methods:
        text = re.sub(r'\b%s\b' % re.escape(old), new, text)
    return text


def class_bodies(text, family):
    """(start, end) of every family class body in text, brace-matched."""
    for m in re.finditer(r'\b(?:class|struct)\s+(%s)\b[^;{()]*\{' % family_pattern(family), text):
        depth, j = 1, m.end()
        while depth and j < len(text):
            depth += {'{': 1, '}': -1}.get(text[j], 0)
            j += 1
        yield m.start(), j


def rename_in_bodies(text, family, methods):
    out, last = '', 0
    for i, j in class_bodies(text, family):
        out += text[last:i] + rename_names(text[i:j], methods)
        last = j
    return out + text[last:]


def rename_definitions(text, family, methods):
    pattern = family_pattern(family)
    for old, new in methods:
        text = re.sub(r'\b(%s)::%s\b' % (pattern, re.escape(old)), r'\1::' + new, text)
        text = re.sub(r'(?<![\w.~])(?<!->)(?<!::)%s(?=\s*\()' % re.escape(old), new, text)
    return text


C2039 = re.compile(r"^(?P<file>.+?)\((?P<line>\d+)\) : error C2039: '(?P<name>\w+)' : is not a member of '(?P<cls>[\w:]+)'", re.M)
C2065 = re.compile(r"^(?P<file>.+?)\((?P<line>\d+)\) : error C2065: '(?P<name>\w+)' : undeclared identifier", re.M)


def unit_starts(texts):
    """[(first line, source)] of sources concatenated with b'\\n' (build.py's group text)."""
    starts, line = [], 1
    for source, data in texts:
        starts.append((line, source))
        line += data.count(b'\n') + 1
    return starts


def locate(path, line, starts):
    """(repository path, line) of a compiler message: accepted.cpp lines map through starts, headers by their src/ path."""
    if path.endswith('accepted.cpp'):
        first, source = [s for s in starts if s[0] <= line][-1]
        return source, line - first + 1
    path = path.replace('\\', '/')
    return path[path.rfind('/src/') + 1:], line


def classify(log, starts, parts):
    """parts [(family, methods)]. fixes {path: {line: {(old, new)}}} (an old name on a class of the part that renames it),
    undo {path: {line: {(new, old)}}} (a new name reported on a class outside its part's family), other [messages]."""
    fixes, undo, other = {}, {}, []
    for m in C2039.finditer(log):
        path, line = locate(m.group('file'), int(m.group('line')), starts)
        name, cls = m.group('name'), m.group('cls').split('::')[-1]
        fix = [(o, n) for family, methods in parts for o, n in methods if o == name and cls in family]
        back = [(n, o) for family, methods in parts for o, n in methods if n == name and cls not in family]
        if fix:
            fixes.setdefault(path, {}).setdefault(line, set()).update(fix[:1])
        elif back:
            undo.setdefault(path, {}).setdefault(line, set()).update(back)
        else:
            other.append(m.group(0))
    for m in C2065.finditer(log):
        # an unqualified call of an old name inside a family member defined outside the family's class directories
        path, line = locate(m.group('file'), int(m.group('line')), starts)
        news = {n for _, methods in parts for o, n in methods if o == m.group('name')}
        if len(news) == 1:
            fixes.setdefault(path, {}).setdefault(line, set()).add((m.group('name'), news.pop()))
        else:
            other.append(m.group(0))
    return fixes, undo, other


def mangling_rules(methods, family):
    # the class may end the string: catalog symbol prefixes are `?old@Class` without the signature
    return [{'pattern': r'(?<![\w])%s@(?=(?:%s)(?![\w]))' % (old, family_pattern(family)), 'replace': new + '@'} for old, new in methods]


def source_pass(args):
    repo = Path(os.path.abspath(args.repository))
    output = Path(os.path.abspath(args.output))   # not resolved: a symlinked folder inside the repository keeps its repository path
    if repo not in output.parents:
        raise SystemExit('--output must be inside the repository (apply_rename reads staged copies by repository path)')
    sys.path.insert(0, str(repo / 'tools/original_target'))
    import build, catalog_io, canonical
    cfg = json.loads(args.cfg.read_text())
    name = cfg['name']
    headers = sorted(str(p.relative_to(repo)) for p in (repo / 'src/shared').rglob('*.hpp'))
    bases = base_map(read(repo / h) for h in headers)
    parts = [(set(p['family']) if 'family' in p else family_of(p['root'], bases), [tuple(m) for m in p['methods']])
             for p in cfg.get('parts', [cfg])]
    all_methods = [m for _, methods in parts for m in methods]
    backup, changed, moves, log = {}, set(), {}, []

    def change(rel, text):
        if rel not in backup:
            backup[rel] = read(repo / rel)
        changed.add(rel)
        write(repo / rel, text)

    for h in headers:
        t = read(repo / h)
        n = t
        for family, methods in parts:
            n = rename_in_bodies(n, family, methods)
        if n != t:
            change(h, n)
    for cls, family, methods in sorted((c, f, m) for f, m in parts for c in f):
        d = repo / 'src/recovered/classes' / cfg.get('dirs', {}).get(cls, cls)
        prefix = cfg.get('prefixes', {}).get(cls, cls)
        for f in sorted(d.glob('*.cpp')) if d.is_dir() else []:
            rel = str(f.relative_to(repo))
            t = read(f)
            n = rename_definitions(t, family, methods)
            if n != t:
                change(rel, n)
            for old, new in methods:
                m = re.fullmatch(r'%s_%s_([0-9a-f]{8})\.cpp' % (re.escape(prefix), re.escape(old)), f.name)
                if m:
                    moves[rel] = str(f.parent.relative_to(repo) / ('%s_%s_%s.cpp' % (prefix, new, m.group(1))))
    try:
        config = json.loads(args.config.read_bytes())
        catalog = catalog_io.load(catalog_io.locate(str(repo)))
        separate = canonical.separate_unit_populations(catalog)
        groups = {}
        for row in sorted(catalog['regions'], key=lambda r: r['source_order']):
            key = canonical.compile_group_key(row, separate)
            if row['source'] not in groups.setdefault(key, []):
                groups[key].append(row['source'])
        names = re.compile(r'\b(%s)\b' % '|'.join(sorted({x for m in all_methods for x in m}, key=len, reverse=True)))
        clean = False
        for iteration in range(args.max_iter):
            todo = [k for k, sources in groups.items() if any(names.search(read(repo / s)) for s in sources)]
            work = repo / 'build/callsite-loop' / name / ('loop-%d' % iteration)
            shutil.rmtree(work, ignore_errors=True)
            work.mkdir(parents=True)
            units, maps = [], []
            for index, key in enumerate(todo):
                d = work / ('g%03d' % index)
                d.mkdir()
                texts = [(s, (repo / s).read_bytes()) for s in groups[key]]
                (d / 'accepted.cpp').write_bytes(b'\n'.join(data for _, data in texts))
                units.append(dict(cwd=d, source=d / 'accepted.cpp', obj=d / 'accepted.obj', log=d / 'compile.log',
                                  returncode=d / 'returncode.txt', args=build.compiler_arguments(key[1])))
                maps.append(unit_starts(texts))
            print('iteration %d: %d groups' % (iteration, len(units)), flush=True)
            if not units:
                clean = True
                break
            build.compile_units(config, work, units, 'loop')
            fixes, undo, other = {}, {}, []
            for unit, starts in zip(units, maps):
                text = unit['log'].read_text(encoding='latin-1', errors='replace') if unit['log'].exists() else ''
                f, u, o = classify(text, starts, parts)
                for k, v in f.items():
                    for line, pairs in v.items():
                        fixes.setdefault(k, {}).setdefault(line, set()).update(pairs)
                for k, v in u.items():
                    for line, pairs in v.items():
                        undo.setdefault(k, {}).setdefault(line, set()).update(pairs)
                other += o
                code = unit['returncode'].read_text().strip() if unit['returncode'].exists() else '1'
                if code != '0' and not C2039.search(text) and not C2065.search(text):
                    other.append('%s: %s' % (unit['cwd'].name, ' | '.join(l for l in text.splitlines() if 'error' in l)[:300]))
            for rel, lines in fixes.items():
                rows = read(repo / rel).split('\n')
                for line, pairs in sorted(lines.items()):
                    rows[line - 1] = rename_names(rows[line - 1], sorted(pairs))
                    log.append('%s:%d %s' % (rel, line, rows[line - 1].strip()[:160]))
                change(rel, '\n'.join(rows))
            for rel, lines in undo.items():
                rows = read(repo / rel).split('\n')
                for line, pairs in sorted(lines.items()):
                    rows[line - 1] = rename_names(rows[line - 1], sorted(pairs))
                    log.append('UNDO %s:%d' % (rel, line))
                change(rel, '\n'.join(rows))
            print('  %d call-site lines, %d undone, %d other errors' % (
                sum(map(len, fixes.values())), sum(map(len, undo.values())), len(other)), flush=True)
            for o in other[:10]:
                print('   ', o[:240])
            if not fixes and not undo:
                clean = not other
                break
        spec = dict(name=name, header_base=subprocess.run(['git', '-C', str(repo), 'rev-parse', 'HEAD'], capture_output=True,
                                                          text=True).stdout.strip(),
                    mangling_rewrite=[r for family, methods in parts for r in mangling_rules(methods, family)], rewrite=[], headers={}, sources={}, moves=moves)

        def mangle(s):
            for r in spec['mangling_rewrite']:
                s = re.sub(r['pattern'], r['replace'], s)
            return s
        output.mkdir(parents=True, exist_ok=True)
        for rel in sorted(changed):
            t = read(repo / rel)
            if t == backup[rel]:
                continue
            t = '\n'.join(re.sub(r'(?<=@ref )(\S+)', lambda m: mangle(m.group(1)), l) if '@ref ' in l else l for l in t.split('\n'))
            dst = output / 'stage' / rel
            dst.parent.mkdir(parents=True, exist_ok=True)
            write(dst, t)
            (spec['headers'] if rel.startswith('src/shared/') else spec['sources'])[rel] = os.path.relpath(dst, repo)
    finally:
        for rel, t in backup.items():
            write(repo / rel, t)
    (output / (name + '.json')).write_text(json.dumps(spec, indent=1))
    (output / 'callsites.txt').write_text(''.join('family: %s | %s\n' % (' '.join(sorted(f)), ' '.join('%s=%s' % m for m in ms))
                                                  for f, ms in parts) + '\n'.join(log) + '\n')
    print('%s: families %s, %d sources, %d headers, %d moves, %d call-site lines%s' % (
        name, '+'.join(str(len(f)) for f, _ in parts), len(spec['sources']), len(spec['headers']), len(moves), len(log), '' if clean else ' (NOT CLEAN)'))
    return 0 if clean else 1


CLANG_UNDECLARED = re.compile(r"^(?P<f>[^:\n]+):(?P<l>\d+):(?P<c>\d+): error: use of undeclared identifier '(?P<n>\w+)'", re.M)
CLANG = re.compile(r"^(?P<f>[^:\n]+):(?P<l>\d+):(?P<c>\d+): error: no member named '(?P<n>\w+)' in '(?:[\w:]+::)?(?P<k>\w+)'", re.M)


def native_symbol_text(text, methods, family):
    for old, new in methods:
        for c in family:
            text = re.sub(r'(?<![0-9])%d%s%d%s' % (len(c), c, len(old), old), '%d%s%d%s' % (len(c), c, len(new), new), text)
            text = re.sub(r'(?<![\w])%s@%s(?![\w])' % (old, c), '%s@%s' % (new, c), text)
        text = re.sub(r'\b(%s)::%s\b' % (family_pattern(family), old), r'\1::' + new, text)
    return text


def rename_at(row, column, old, new):
    i = column - 1
    if row[i:i + len(old)] == old:
        return row[:i] + new + row[i + len(old):]
    return re.sub(r'\b%s\b' % old, new, row)


def read_parts(callsites_text):
    """[(family, [(old, new)])] from callsites.txt's leading `family: A B | old=new ...` lines."""
    parts = []
    for line in callsites_text.split('\n'):
        if not line.startswith('family:'):
            break
        classes, _, pairs = line.split(':', 1)[1].partition('|')
        parts.append((set(classes.split()), [tuple(p.split('=', 1)) for p in pairs.split()]))
    return parts


def move_rules(moves):
    """(pattern, replacement) for moved sources as native/ spells them: repository paths, classes/... tails, bare file names."""
    rules = []
    for old, new in sorted(moves.items(), key=lambda kv: -len(kv[0])):
        rules.append((re.escape(old), new))
        ot, nt = old.split('src/recovered/', 1)[-1], new.split('src/recovered/', 1)[-1]
        if ot != nt:
            rules.append((r'(?<![\w/])' + re.escape(ot), nt))
        ob, nb = old.rsplit('/', 1)[-1], new.rsplit('/', 1)[-1]
        if ob != nb:
            rules.append((r'(?<![\w])' + re.escape(ob), nb))
    return rules


def native_pass(args):
    repo = args.repository.resolve()
    parts = read_parts(args.callsites.read_text())
    moves = json.loads(args.spec.read_text()).get('moves', {})
    def new_for(old, cls):
        return next((n for family, methods in parts for o, n in methods if o == old and cls in family), None)
    allnames = {x for _, methods in parts for m in methods for x in m}
    bdir = args.build_dir or ('build/native-play' if (repo / 'build/native-play').is_dir() else 'build/native')
    if not (repo / bdir / 'CMakeCache.txt').is_file():
        raise SystemExit('%s is not a configured native build directory' % bdir)
    files = [f for f in subprocess.run(['git', '-C', str(repo), 'ls-files', 'native'], capture_output=True, text=True).stdout.split()
             if f.endswith(('.cpp', '.hpp', '.h', '.json', '.tsv', '.txt', '.py'))]
    for f in files:
        t = read(repo / f)
        n = t
        for pattern, replacement in move_rules(moves):
            n = re.sub(pattern, replacement, n)
        for family, methods in parts:
            n = native_symbol_text(n, methods, family)
        if n != t:
            write(repo / f, n)
            print('rewrote', f)
    unresolved = []
    for round_ in range(6):
        subprocess.run(['cmake', bdir], cwd=repo, capture_output=True)
        out = subprocess.run(['cmake', '--build', bdir, '--', '-k', '0'], cwd=repo, capture_output=True, text=True)
        text = out.stdout + out.stderr
        hits = {(m.group('f'), int(m.group('l')), int(m.group('c')), m.group('n'), m.group('k')) for m in CLANG.finditer(text)}
        # an unqualified call inside a family member (drafts define members outside the class files): the old name's only new name
        for m in CLANG_UNDECLARED.finditer(text):
            news = {n for _, methods in parts for o, n in methods if o == m.group('n')}
            if len(news) == 1:
                cls = next(iter(f for f, methods in parts for o, n in methods if o == m.group('n')))
                hits.add((m.group('f'), int(m.group('l')), int(m.group('c')), m.group('n'), next(iter(cls))))
        todo = [h for h in hits if new_for(h[3], h[4])]
        unresolved = [h for h in hits if h not in todo and h[3] in allnames]
        print('native round %d: build exit %d, %d call sites' % (round_, out.returncode, len(todo)), flush=True)
        if not todo:
            break
        for f in sorted({h[0] for h in todo}):
            path = Path(f) if os.path.isabs(f) else repo / f
            rows = read(path).split('\n')
            for _, line, column, old, cls in sorted((h for h in todo if h[0] == f), key=lambda h: -h[2]):
                rows[line - 1] = rename_at(rows[line - 1], column, old, new_for(old, cls))
                print('  call site %s:%d' % (os.path.relpath(path, repo), line))
            write(path, '\n'.join(rows))
    left = []
    for family, methods in parts:
        for old, _ in methods:
            left += subprocess.run(['git', '-C', str(repo), 'grep', '-n', '-P', r'\b(%s)::%s\b' % (family_pattern(family), old), '--', 'native'],
                                   capture_output=True, text=True).stdout.splitlines()
    left += ['compiler: %s:%d %s in %s' % (u[0], u[1], u[3], u[4]) for u in unresolved]
    for l in left:
        print('  ', l[:200])
    print('total leftovers in native/: %d' % len(left))
    return 0 if not left else 1


def main(argv=None):
    argv = sys.argv[1:] if argv is None else argv
    repository = TOOLS.parents[1]
    if argv and argv[0] == 'native':
        p = argparse.ArgumentParser(prog='callsite_rename.py native')
        p.add_argument('spec', type=Path)
        p.add_argument('callsites', type=Path)
        p.add_argument('--repository', type=Path, default=repository)
        p.add_argument('--build-dir')
        return native_pass(p.parse_args(argv[1:]))
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument('cfg', type=Path)
    p.add_argument('--output', type=Path, required=True)
    p.add_argument('--repository', type=Path, default=repository)
    p.add_argument('--config', type=Path, default=repository / 'private/original-target.local.json')
    p.add_argument('--max-iter', type=int, default=6)
    return source_pass(p.parse_args(argv))


if __name__ == '__main__':
    sys.exit(main())
