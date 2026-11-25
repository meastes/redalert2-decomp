"""mainhist.py [--refresh]: main's history for every parked row of tail_buckets.csv (lane L1, 2026-11-19).

For each row tailmap.py wrote (manifests/tail_buckets.csv) this attaches what main's record says about the
same function, without compiling anything:

  main_state        exact (a region of main's catalog), non_exact (main's 96 never-matched drafts, non_exact.json)
                    or no_row
  accepted_commit,  the first commit of main whose catalog lists the entry (manifests/original_target.json until
  accepted_date     2026-10-27, the population shards after), from one pass over the catalog's history; cached
                    under build/r/mainhist/ by main's head and extended incrementally. A row already in the
                    catalog's first commit (2026-10-20) was accepted before the catalog existed: its date reads
                    "by 2026-10-20" 
  population,       main's population today, its source, and the region's flags (count_calibrated,
  main_source,      split_declaration, declarations_only, inline_asm, data_bound_by) with notes cut short
  flags
  best, tries,      docs/MISSES_INDEX.md (best score, tries, last lane, causes) and docs/MISSES_RESCORE.tsv; for
  causes            non_exact rows the score main's draft holds
  sessions          distinct lane tags (Scout603, Inf65, codex2 ...) in main's commit messages that name the
                    address (rebuild landings and merges excluded), and the highest round (r2, r3 ...) they give
  open_misses,      lines of docs/OPEN_MISSES.md and files of docs/history/ naming the address
  history
  notes             VC6_MATCHING_NOTES.md sections naming the address (their study tags: Study37, Big49, Scout1114 ...)
  difficulty        first-try on main: exact on main, no miss record, at most one session, no round past r1 and no
                    judgment flag; hard on main: exact on main after a miss record, several sessions, a later round
                    or a flag; never matched on main: non_exact; no main row

Writes manifests/tail_history.csv and prints the counts and bytes per difficulty and bucket. --refresh
rebuilds the catalog-history cache from the first commit (about 7 minutes)."""
import collections
import csv
import json
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
MANIFEST = ROOT / 'manifests'
TAIL = MANIFEST / 'tail_buckets.csv'
OUT = MANIFEST / 'tail_history.csv'
CACHE = ROOT / 'build/r/mainhist/first_seen.json'
CATALOG_PATHS = ['manifests/original_target.json', 'manifests/original_target/populations']
FLAGS = ('count_calibrated', 'split_declaration', 'declarations_only', 'inline_asm', 'data_bound_by')
HEX = re.compile(r'(?<![0-9A-Za-z_])(?:0x)?0{0,2}([4-7][0-9A-Fa-f]{5})(?![0-9A-Za-z_])')
TAG = re.compile(r'(?<![\w-])((?:codex|[A-Z][a-z]+)\d+)(?![\w-])(?:\s+r(\d+))?')


def git(*args):
    return subprocess.run(['git', '-C', str(ROOT)] + list(args), capture_output=True, text=True, check=True).stdout.strip()


def first_seen(refresh):
    """{entry: (commit, date)}: the first commit of main whose catalog has a region with that entry."""
    head = git('rev-parse', 'main')
    cache = {} if refresh or not CACHE.exists() else json.loads(CACHE.read_text())
    seen = {int(k): v for k, v in cache.get('first', {}).items()}
    base = cache.get('head')
    if base == head:
        return seen
    span = ['main'] if not base else ['%s..main' % base]
    if base and subprocess.run(['git', '-C', str(ROOT), 'merge-base', '--is-ancestor', base, 'main']).returncode:
        span, seen = ['main'], {}
    proc = subprocess.Popen(['git', '-C', str(ROOT), 'log'] + span + ['--reverse', '-p', '--no-renames', '--unified=0',
                            '--format=@@C %h %ad', '--date=short', '--'] + CATALOG_PATHS,
                            stdout=subprocess.PIPE, text=True, errors='replace')
    commit = None
    method = re.compile(r'^\+\s*"method": "([0-9a-fA-F]+)"')
    for line in proc.stdout:
        if line.startswith('@@C '):
            commit = line[4:].split()
            continue
        m = method.match(line)
        if m and commit:
            seen.setdefault(int(m.group(1), 16), commit)
    proc.wait()
    CACHE.parent.mkdir(parents=True, exist_ok=True)
    CACHE.write_text(json.dumps(dict(head=head, first={str(k): v for k, v in seen.items()})))
    return seen


def catalog():
    """{entry: region} of main's catalog today."""
    out = {}
    for p in sorted((ROOT / 'manifests/original_target/populations').glob('*.json')):
        for r in json.loads(p.read_text()).get('regions', []):
            out[int(r['method'], 16)] = r
    return out


def misses():
    out = {}
    for line in (ROOT / 'docs/MISSES_INDEX.md').read_text().splitlines():
        c = [x.strip() for x in line.strip().strip('|').split('|')]
        if len(c) >= 9 and c[0].startswith('0x'):
            out[int(c[0], 16)] = dict(best=c[2], tries=c[3], last=c[4], causes=' '.join(c[6].split()))
    p = ROOT / 'docs/MISSES_RESCORE.tsv'
    if p.exists():
        for r in csv.DictReader(open(p), delimiter='\t'):
            try:
                a = int(r['entry'], 16)
            except (KeyError, ValueError):
                continue
            out.setdefault(a, dict(best=r.get('index_best', ''), tries='', last='', causes=''))['rescore'] = r.get('head_score', '')
    return out


def mentions(text):
    """{entry: count} of the addresses a text names."""
    c = collections.Counter()
    for m in HEX.finditer(text):
        c[int(m.group(1), 16)] += 1
    return c


def commit_sessions():
    """{entry: (tags, highest round, commits)} from main's commit messages (rebuild landings and merges excluded)."""
    log = git('log', 'main', '--format=%x00%h%n%B')
    out = collections.defaultdict(lambda: [set(), 0, 0])
    for chunk in log.split('\x00')[1:]:
        lines = chunk.split('\n', 1)
        body = lines[1] if len(lines) > 1 else ''
        subject = body.lstrip().split('\n', 1)[0]
        if subject.startswith(('Rebuild', 'Merge')):
            continue
        body = re.sub(r'(?m)^(Co-Authored-By|Claude-Session):.*$', '', body)
        found = mentions(body)
        if not found:
            continue
        tags = TAG.findall(body)
        for a in found:
            rec = out[a]
            rec[2] += 1
            for t, r in tags:
                rec[0].add(t)
                if r and not t.startswith('codex') and int(r) <= 50:   # codex rN counts runs, not rounds of a row
                    rec[1] = max(rec[1], int(r))
    return out


def notes_sections():
    """{entry: [section titles]} of VC6_MATCHING_NOTES.md."""
    sys.path.insert(0, str(ROOT / 'tools/original_target'))
    import notes_index
    lines = (ROOT / 'docs/VC6_MATCHING_NOTES.md').read_text(errors='replace').splitlines()
    out = collections.defaultdict(list)
    for title, first, last, _ in notes_index.parse():
        for a in mentions('\n'.join(lines[first - 1:last])):
            out[a].append(title)
    return out


def study_tags(titles):
    tags = []
    for t in titles:
        for m in re.finditer(r'\(([^()]*)\)\s*$', t):
            tags += [x.strip() for x in re.split(r'[;,]', m.group(1)) if re.match(r'[A-Za-z]+\d', x.strip())]
        if not re.search(r'\([^()]*\)\s*$', t):
            tags.append(t[:40])
    return list(dict.fromkeys(x.split(' ')[0] if re.match(r'[A-Za-z]+\d+\b', x) else x for x in tags))


def main():
    rows = list(csv.DictReader(open(TAIL)))
    seen = first_seen('--refresh' in sys.argv)
    first_commit = git('log', 'main', '--reverse', '--format=%h', '--', CATALOG_PATHS[0]).split('\n')[0]
    cat = catalog()
    nonexact = {int(r['entry'], 16): r for r in json.loads((ROOT / 'manifests/original_target/non_exact.json').read_text())['rows']}
    miss = misses()
    sess = commit_sessions()
    open_m = mentions((ROOT / 'docs/OPEN_MISSES.md').read_text(errors='replace'))
    hist = collections.defaultdict(set)
    for p in sorted((ROOT / 'docs/history').glob('*')):
        for a in mentions(p.read_text(errors='replace')):
            hist[a].add(p.stem)
    notes = notes_sections()
    out = []
    for r in rows:
        a = int(r['entry'], 16)
        reg = cat.get(a)
        rec = dict(unit=r['unit'], entry=r['entry'], function=r['function'], bytes=int(r['bytes']), bucket=r['bucket'])
        state = 'exact' if reg else 'non_exact' if a in nonexact else 'no_row'
        commit, date = seen.get(a, ['', ''])
        if commit and commit == first_commit:
            date = 'by ' + date          # in the catalog's first commit: accepted before the catalog existed
        flags = []
        if reg:
            for f in FLAGS:
                if reg.get(f):
                    note = reg.get(f + '_note') or ''
                    flags.append(f + (': ' + note[:80] if note else ''))
        m = miss.get(a, {})
        best = m.get('best', '')
        if state == 'non_exact':
            best = best if best not in ('', '-') and float(best) > nonexact[a]['non_exact']['score'] else nonexact[a]['non_exact']['score']
        tags, rounds, ncommits = sess.get(a, [set(), 0, 0])
        titles = notes.get(a, [])
        if state == 'exact':
            hard = bool(m) or open_m.get(a) or len(tags) > 1 or rounds > 1 or flags
            difficulty = 'hard on main' if hard else 'first-try on main'
        else:
            difficulty = 'never matched on main' if state == 'non_exact' else 'no main row'
        rec.update(main_state=state, accepted_commit=commit if reg else '', accepted_date=date if reg else '',
                   population=reg.get('population', '') if reg else '', main_source=reg.get('source', '') if reg else '',
                   flags='; '.join(flags), best=best, tries=m.get('tries', ''), last=m.get('last', ''),
                   causes=m.get('causes', ''), sessions=' '.join(sorted(tags)), max_round=rounds or '',
                   commit_mentions=ncommits, open_misses=open_m.get(a, 0), history=' '.join(sorted(hist.get(a, ()))),
                   notes='; '.join(study_tags(titles)), difficulty=difficulty)
        out.append(rec)
    with open(OUT, 'w', newline='') as f:
        w = csv.DictWriter(f, list(out[0]))
        w.writeheader()
        for rec in out:
            w.writerow(rec)
    summary(out)


def summary(out):
    by = collections.defaultdict(lambda: [0, 0])
    cross = collections.defaultdict(lambda: [0, 0])
    for r in out:
        by[r['difficulty']][0] += 1; by[r['difficulty']][1] += r['bytes']
        cross[(r['bucket'], r['difficulty'])][0] += 1; cross[(r['bucket'], r['difficulty'])][1] += r['bytes']
    print('%-24s %6s %9s' % ('difficulty', 'rows', 'bytes'))
    for k, (n, b) in sorted(by.items(), key=lambda kv: -kv[1][1]):
        print('%-24s %6d %9d' % (k, n, b))
    print('\n%-16s %s' % ('bucket', '  '.join('%s rows/bytes' % k for k in sorted(by))))
    for bucket in sorted({b for b, _ in cross}):
        print('%-16s %s' % (bucket, '  '.join('%5d/%-8d' % tuple(cross[(bucket, k)]) for k in sorted(by))))
    print('wrote %s (%d rows)' % (OUT.relative_to(ROOT), len(out)))


if __name__ == '__main__':
    main()
