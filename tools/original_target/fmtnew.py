#!/usr/bin/env python3
"""Format only the lines a change adds or modifies in src and include (lane F1, 2026-11-19).

    fmtnew.py [--base REV] [--dry-run] [--no-check] [PATH...]

The changed lines are those of `git diff -U0 BASE` (BASE against the working tree; default the merge-base of
HEAD and main). Whole files are never formatted, so the Westwood layout of the adopted text stays as
it is: a changed line whose text occurs in the file's adopted origin (manifests/adopted.csv; the reference
checkouts under private/reference) counts as adopted and is left alone. clang-format runs with .clang-format on the changed ranges only,
and only whitespace edits that keep the line count are applied (64 places use __LINE__; moving a line can
change compiled bytes), so the file keeps every line where it was. Kept as written: `return(x)` (clang-format
would put a space after `return`).

After formatting, every touched unit is checked again (`rebuild.py check UNIT`, for a header every unit whose
include closure holds it). A unit whose set of exact rows changes has its files restored unformatted; the
restored lines are listed in manifests/format_exempt.csv so the census does not count them, and the run
reports them. Meant for the owner's landing; workers need not run it.

The census item "new lines not in house format" (rebuild_census.py) uses `unformatted()` over the same diff.
"""
import argparse
import csv
import os
import re
import shutil
import subprocess
import sys
import xml.etree.ElementTree as ET
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
MANIFEST = ROOT / 'manifests'
EXEMPT = MANIFEST / 'format_exempt.csv'
STYLE = ROOT / '.clang-format'
REFERENCES = {'mischa85': ROOT / 'private/reference/mischa85-RedAlert2',
              'tibsun': ROOT / 'private/reference/TibSun',
              'ra2-decomp': ROOT / 'private/reference/ra2-decomp'}
SUFFIXES = ('.cpp', '.c', '.h', '.hh', '.hpp', '.inl')


def git(*args, check=True):
    r = subprocess.run(['git', '-C', str(ROOT)] + list(args), capture_output=True, text=True, errors='replace')
    if check and r.returncode:
        raise SystemExit('git %s: %s' % (' '.join(args), r.stderr.strip()))
    return r.stdout


def clang_format():
    path = shutil.which('clang-format')
    if path:
        return path
    r = subprocess.run(['xcrun', '--find', 'clang-format'], capture_output=True, text=True) if shutil.which('xcrun') else None
    return r.stdout.strip() if r is not None and r.returncode == 0 and r.stdout.strip() else None


def default_base():
    """RA2_FMT_BASE, else the merge-base of HEAD and main (None when neither exists)."""
    base = os.environ.get('RA2_FMT_BASE')
    if base:
        return base
    r = subprocess.run(['git', '-C', str(ROOT), 'merge-base', 'HEAD', 'main'], capture_output=True, text=True)
    return r.stdout.strip() or None


def changed_lines(base, paths=None):
    """{path: sorted line numbers (1-based, working tree)} added or modified since BASE."""
    # A base from before the layout move (2026-11-25, lane O88: rebuild/src and rebuild/include became src and
    # include) is diffed with rename detection over both spellings, so a moved file counts only its changed lines.
    before_move = subprocess.run(['git', '-C', str(ROOT), 'cat-file', '-e', '%s:rebuild/src' % base],
                                 capture_output=True).returncode == 0
    if before_move:
        out = git('diff', '-U0', '--no-color', '--no-ext-diff', '-M', base, '--',
                  *(paths or ['src', 'include']), 'rebuild/src', 'rebuild/include')
    else:
        out = git('diff', '-U0', '--no-color', '--no-ext-diff', '--no-renames', base, '--',
                  *(paths or ['src', 'include']))
    untracked = git('ls-files', '--others', '--exclude-standard', '--', *(paths or ['src', 'include']))
    found, current = {}, None
    for line in out.splitlines():
        if line.startswith('+++ '):
            current = line[6:] if line.startswith('+++ b/') else None
        elif line.startswith('@@') and current:
            m = re.match(r'@@ -\S+ \+(\d+)(?:,(\d+))? @@', line)
            start, count = int(m.group(1)), int(m.group(2) or 1)
            found.setdefault(current, set()).update(range(start, start + count))
    for p in untracked.split():
        n = len((ROOT / p).read_text(errors='replace').splitlines())
        found.setdefault(p, set()).update(range(1, n + 1))
    return {p: sorted(v) for p, v in found.items() if p.endswith(SUFFIXES) and v and (ROOT / p).is_file()}


_ORIGINS = None


def origin_texts(path):
    """The texts (bytes) of PATH's adopted origin files (empty when not adopted or the reference is absent)."""
    global _ORIGINS
    if _ORIGINS is None:
        _ORIGINS = {}
        ledger = MANIFEST / 'adopted.csv'
        if ledger.exists():
            for r in csv.DictReader(open(ledger)):
                _ORIGINS[r['file']] = r
    row = _ORIGINS.get(path)
    if row and row['origin'] in REFERENCES:
        sources = [(REFERENCES[row['origin']], row['commit'], row['origin_path'])]
    else:   # not in the ledger: whatever the references hold under the same name counts as adopted
        name = path.split('/', 1)[1]
        sources = [(REFERENCES['mischa85'], 'HEAD', 'src/' + name), (REFERENCES['mischa85'], 'HEAD', 'include/' + name),
                   (REFERENCES['tibsun'], 'HEAD', 'code/' + name), (REFERENCES['ra2-decomp'], 'HEAD', 'src/' + name),
                   (REFERENCES['ra2-decomp'], 'HEAD', 'include/' + name)]
    found = []
    for repo, commit, origin_path in sources:
        if repo.exists():
            r = subprocess.run(['git', '-C', str(repo), 'show', '%s:%s' % (commit, origin_path)],
                               capture_output=True)
            if r.returncode == 0 and r.stdout:
                found.append(r.stdout)
    return found


def adopted_lines(path):
    """Line numbers of PATH (working tree) that a line diff aligns with an adopted origin text: unchanged adopted
    lines. A lone `}` that only happens to equal an origin line elsewhere is not aligned, so it stays ours."""
    import tempfile
    found = set()
    for origin in origin_texts(path):
        with tempfile.NamedTemporaryFile(suffix='.txt') as f:
            f.write(origin); f.flush()
            out = subprocess.run(['git', 'diff', '--no-index', '-U0', '--no-color', '--diff-algorithm=histogram',
                                  f.name, str(ROOT / path)], capture_output=True, text=True, errors='replace').stdout
        n_lines = len((ROOT / path).read_bytes().split(b'\n'))
        added = set()
        for line in out.splitlines():
            m = re.match(r'@@ -\S+ \+(\d+)(?:,(\d+))? @@', line)
            if m:
                start, count = int(m.group(1)), int(m.group(2) or 1)
                added.update(range(start, start + count))
        found |= set(range(1, n_lines + 1)) - added
    return found


def exemptions():
    """{path: set of exempt line texts} (lines restored unformatted because formatting changed a unit's rows)."""
    found = {}
    if EXEMPT.exists():
        for r in csv.DictReader(open(EXEMPT, newline='')):
            found.setdefault(r['file'], set()).add(r['line'])
    return found


def own_lines(path, lines, text_lines, exempt):
    """The lines of LINES that are this project's own text: not in the adopted origin, not exempt, not blank."""
    adopted = adopted_lines(path)
    skip = exempt.get(path, set())
    return [n for n in lines if n <= len(text_lines) and text_lines[n - 1].strip()
            and n not in adopted and text_lines[n - 1] not in skip]


def ranges(lines):
    """Consecutive runs of LINES as (first, last)."""
    out = []
    for n in lines:
        if out and n == out[-1][1] + 1:
            out[-1][1] = n
        else:
            out.append([n, n])
    return [tuple(r) for r in out]


OPERATOR = re.compile(r'\boperator\s*[^\w\s(]*\s*$|\boperator\s*\(\)\s*$')


def replacements(cf, text, path, lines):
    """clang-format's replacements for LINES of TEXT, as (offset, length, new) in character offsets, filtered to
    whitespace edits that keep the line count and leave `return(` alone."""
    if not lines:
        return []
    wanted = set(lines)
    data = text.encode('utf-8', 'surrogateescape')
    args = [cf, '--style=file:%s' % STYLE, '--assume-filename=%s' % (ROOT / path).with_suffix(
        '.cpp' if path.endswith(('.cpp', '.c')) else '.hpp'), '--output-replacements-xml']
    args += ['--lines=%d:%d' % r for r in ranges(lines)]
    r = subprocess.run(args, input=data, capture_output=True)
    if r.returncode:
        raise RuntimeError('clang-format failed on %s: %s' % (path, r.stderr.decode(errors='replace')[:300]))
    kept = []
    if not r.stdout.strip():
        return kept
    for e in ET.fromstring(r.stdout).iter('replacement'):
        off, length, new = int(e.get('offset')), int(e.get('length')), e.text or ''
        old = data[off:off + length].decode('utf-8', 'surrogateescape')
        if old.strip() or new.strip():
            continue                                    # not whitespace only (never expected)
        if old.count('\n') != new.count('\n'):
            continue                                    # would add or remove a line
        if old == '' and new == ' ' and data[:off].endswith(b'return') and data[off:off + 1] == b'(':
            continue                                    # return(x) is the house form
        if '\n' not in old and data[off + length:off + length + 2] in (b'//', b'/*'):
            continue                                    # the space before a trailing comment keeps its alignment
        if old and not new and OPERATOR.search(data[max(0, off - 24):off].decode('utf-8', 'surrogateescape')):
            continue                                    # `operator == (` keeps its spaces (TibSun form)
        # Trim what old and new share, so the edit covers only the whitespace that changes; it must lie on
        # the requested lines (never the trailing whitespace of an adopted line before them).
        while old and new and old[0] == new[0]:
            old, new, off, length = old[1:], new[1:], off + 1, length - 1
        while old and new and old[-1] == new[-1]:
            old, new, length = old[:-1], new[:-1], length - 1
        if not old and not new:
            continue
        first, last = line_of(data, off), line_of(data, off + length)
        if not all(n in wanted for n in range(first, last + 1)):
            continue
        kept.append((off, length, new))
    return kept


def apply(text, edits):
    data = text.encode('utf-8', 'surrogateescape')
    for off, length, new in sorted(edits, reverse=True):
        data = data[:off] + new.encode() + data[off + length:]
    return data.decode('utf-8', 'surrogateescape')


def settle(cf, text, path, lines, passes=4):
    """TEXT with LINES formatted to a fixed point: a pass over lines whose neighbours keep an older layout can
    leave them where a second pass, seeing its own result as context, moves them again."""
    for _ in range(passes):
        edits = replacements(cf, text, path, lines)
        if not edits:
            break
        text = apply(text, edits)
    return text


def line_of(data, offset):
    return data.count(b'\n', 0, offset) + 1


def unformatted(base=None, paths=None, detail=False):
    """Census item: the number of the project's own changed lines (BASE..working tree) that the formatter would
    change. Returns (count, note); note explains a skipped measurement."""
    base = base or default_base()
    cf = clang_format()
    if not base:
        return 0, 'no base (set RA2_FMT_BASE or have a main branch)'
    if not cf or not STYLE.exists():
        return 0, 'clang-format or .clang-format not found: not measured'
    exempt = exemptions()
    count, listed = 0, []
    for path, lines in sorted(changed_lines(base, paths).items()):
        text = (ROOT / path).read_text(errors='surrogateescape')
        text_lines = text.split('\n')
        mine = own_lines(path, lines, text_lines, exempt)
        new = settle(cf, text, path, mine)
        touched = [i for i, (a, b) in enumerate(zip(text_lines, new.split('\n'))) if a != b]
        if not touched:
            continue
        count += len(touched)
        listed.append('%s %d' % (path, len(touched)))
    return count, ', '.join(listed) if detail else ''


# --- the landing tool -------------------------------------------------------------------------------------------

def units_by_source():
    import json
    units = json.loads((MANIFEST / 'units.json').read_text())
    return units, {i['source']: u for u, i in units.items()}


def includers(header, units):
    """Units whose include closure holds HEADER (a path under include)."""
    sys.path.insert(0, str(ROOT / 'tools/original_target'))
    import rebuild as R
    name = Path(header).relative_to('include').as_posix()
    roots = [ROOT / 'include']
    found = []
    for u, info in sorted(units.items()):
        src = ROOT / info['source']
        if not src.exists():
            continue
        closure = R.include_closure(src.read_text(errors='replace'), roots)
        if any(p is not None and Path(p).resolve() == (ROOT / header).resolve() for p in closure.values()) or name in closure:
            found.append(u)
    return found


PYTHON = sys.executable


def exact_rows(unit):
    """UNIT's rows from a fresh check as (entry, status, differing bytes); None when the unit fails to compile."""
    import json
    env = dict(os.environ)
    r = subprocess.run([PYTHON, '-B', str(ROOT / 'tools/original_target/rebuild.py'), 'check', unit], cwd=ROOT, env=env,
                       capture_output=True, text=True)
    last = ROOT / 'build/r/last' / (unit + '.json')
    if r.returncode not in (0, 1) or not last.exists():
        return None
    data = json.loads(last.read_text())
    return frozenset((x['entry'], x['status'], x.get('differing')) for x in data['results'])


def record_exempt(path, text_lines, lines):
    rows = list(csv.DictReader(open(EXEMPT, newline=''))) if EXEMPT.exists() else []
    have = {(r['file'], r['line']) for r in rows}
    for n in lines:
        key = (path, text_lines[n - 1])
        if key not in have:
            rows.append(dict(file=path, line=text_lines[n - 1])); have.add(key)
    with open(EXEMPT, 'w', newline='') as f:
        w = csv.DictWriter(f, fieldnames=['file', 'line'], lineterminator='\n')
        w.writeheader(); w.writerows(sorted(rows, key=lambda r: (r['file'], r['line'])))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--base', help='diff base (default RA2_FMT_BASE or merge-base of HEAD and main)')
    ap.add_argument('--dry-run', action='store_true', help='report what would change, write nothing')
    ap.add_argument('--no-check', action='store_true', help='skip the per-unit checks (never at a landing)')
    ap.add_argument('--skip', action='append', default=[], help='unit to leave alone (e.g. one a lane holds)')
    ap.add_argument('--own', action='store_true', help='every line of PATHS (default all of src and include) '
                    'that is not in its adopted origin, not only the diff: the cleanup pass over our own text')
    ap.add_argument('--range', help='FIRST-LAST: only these lines (with one PATH)')
    ap.add_argument('paths', nargs='*')
    args = ap.parse_args()
    base = args.base or default_base()
    cf = clang_format()
    if not base or not cf:
        sys.exit('fmtnew: no base or no clang-format')
    units, by_source = units_by_source()
    skip_sources = {units[u]['source'] for u in args.skip if u in units}
    exempt = exemptions()
    originals, plans = {}, {}
    if args.own:
        files = git('ls-files', '--', *(args.paths or ['src', 'include'])).split()
        changes = {p: list(range(1, len((ROOT / p).read_text(errors='surrogateescape').split('\n')) + 1))
                   for p in files if p.endswith(SUFFIXES)}
    else:
        changes = changed_lines(base, args.paths or None)
    if args.range:
        first, last = map(int, args.range.split('-'))
        changes = {p: [n for n in v if first <= n <= last] for p, v in changes.items()}
    for path, lines in sorted(changes.items()):
        if path in skip_sources:
            continue
        text = (ROOT / path).read_text(errors='surrogateescape')
        text_lines = text.split('\n')
        mine = own_lines(path, lines, text_lines, exempt)
        new = settle(cf, text, path, mine)
        if new == text:
            continue
        assert new.count('\n') == text.count('\n'), path
        changed = [i + 1 for i, (a, b) in enumerate(zip(text_lines, new.split('\n'))) if a != b]
        originals[path], plans[path] = text, (new, changed)
        print('%s: %d lines' % (path, len(changed)))
    if args.dry_run or not plans:
        print('fmtnew: %d files, %d lines%s' % (len(plans), sum(len(c) for _, c in plans.values()),
                                               ' (dry run)' if args.dry_run else ''))
        return 0
    # Units to recheck: a source's own unit; a header's includers.
    groups = {}     # unit -> files of the plan it depends on
    for path in plans:
        if path in by_source:
            groups.setdefault(by_source[path], set()).add(path)
        elif path.startswith('include/'):
            for u in includers(path, units):
                groups.setdefault(u, set()).add(path)
    if args.no_check:
        for path, (new, _) in plans.items():
            (ROOT / path).write_text(new, errors='surrogateescape')
        print('fmtnew: written without checks')
        return 0
    print('fmtnew: checking %d units before formatting' % len(groups))
    before = {u: exact_rows(u) for u in sorted(groups)}
    for path, (new, _) in plans.items():
        (ROOT / path).write_text(new, errors='surrogateescape')
    restored = set()
    while True:
        bad = []
        for u in sorted(groups):
            if groups[u] <= restored:
                continue
            after = exact_rows(u)
            if after != before[u]:
                bad.append((u, before[u], after))
        if not bad:
            break
        for u, b, a in bad:
            lost = sorted(e for e, st, _ in (b or set()) - (a or set()))
            print('fmtnew: %s rows change under formatting (%s): restoring %s' % (
                u, 'does not compile' if a is None else ','.join(lost) or '-', ', '.join(sorted(groups[u] - restored))))
            for path in groups[u] - restored:
                (ROOT / path).write_text(originals[path], errors='surrogateescape')
                record_exempt(path, originals[path].split('\n'), plans[path][1])
                restored.add(path)
        # A restored header changes its other includers back: check them again in the next pass.
    done = [p for p in plans if p not in restored]
    print('fmtnew: formatted %d files (%d lines); restored unformatted: %s' % (
        len(done), sum(len(plans[p][1]) for p in done), ', '.join(sorted(restored)) or 'none'))
    return 0


if __name__ == '__main__':
    sys.exit(main())
