#!/usr/bin/env python3
"""Generate the numeric part of docs/STATUS.md from the canonical manifest.

Coverage comes from the catalog (manifests/original_target/); the .text size from the
supplied executable's section header. Throughput history comes from every
commit that changed the catalog (or the former monolithic
manifests/original_target.json), so the matched-bytes-per-hour trend is
measured, not narrated. The block between the generated markers in
docs/STATUS.md is replaced; everything else in that file is hand-written.
"""
import argparse
from collections import Counter
from datetime import datetime
import json
from pathlib import Path
import subprocess
import sys

import catalog_io

REPOSITORY = Path(__file__).resolve().parents[2]
BEGIN, END = '<!-- generated:status -->', '<!-- /generated:status -->'


def evidenced_inline_asm(region):
    """Regions whose source holds cited TibSun `__asm` (canonical.validate_inline_asm):
    their own ledger category beside library code, not C/C++ source credit."""
    return region.get('inline_asm') == 'evidenced'


TEXT_START = 0x401000
# C runtime and linked libraries: everything from 0x781000 to the end of .text (the boundary the
# treemap uses for its 'C runtime' group). Library ledger, not C/C++ source credit; the handwritten
# ranges below take precedence where they overlap it (sosCODEC and ASM_UnVQ run to 0x781571).
CRT_START = 0x781000
# Handwritten blitters/codecs: the owner's evidenced handwritten-assembly ledger (Rich header: 48
# Masm613 build-7299 objects). Byte-free [start, end) ranges; each entry cites the OPEN_MISSES ruling
# that classed it and the bound used (Ghidra inventory body, a hypothesis like every unaccepted bound).
# A range that later lands as an accepted `inline_asm: evidenced` region is counted once (union).
HANDWRITTEN = (
    # LCW_Comp: TibSun ships it as `__asm` (OPEN_MISSES Scout805, Scout848 ruling, Scout922); inventory body 407 B.
    (0x534D40, 0x534ED7, 'LCW_Comp'),
    # RDTSC and Get_RDTSC_CPU_Speed `__asm` (OPEN_MISSES Scout805, Scout848 ruling, Scout922); inventory 14 B and 450 B.
    (0x5ABF60, 0x5ABF6E, 'RDTSC'),
    (0x5ABF70, 0x5AC132, 'Get_RDTSC_CPU_Speed'),
    # XSurface::Fill_Rect (Westwood `_asm` surface_quick_fill; OPEN_MISSES Surf66 list, Scout848 ruling); 748 B.
    (0x7743C0, 0x7746AC, 'XSurface::Fill_Rect'),
    # BlitTrans BlitForward specialisations (TibSun blitblit.h `__asm`; Surf66 list, Scout848 ruling); 45 B and 70 B.
    (0x775980, 0x7759AD, 'BlitTrans<uchar>::BlitForward'),
    (0x4861C0, 0x486206, 'BlitTrans BlitForward'),
    # BlitForward with a C prologue and an `__asm` 50% translucent blend (owner, 2026-11-08, Scout1030): TibSun's
    # BlitTransLucent50 is plain C, so the evidenced inline-asm exception does not apply; 90 B.
    (0x487080, 0x4870DA, 'BlitTransLucent50 BlitForward'),
    # VQA32 MASM tail: AudioUnzap, VQA_LCW_Uncompress, sosCODEC, ASM_UnVQ_*, cpuid/rdtsc/FPU helpers
    # (OPEN_MISSES Abuf490, Scout845, Scout848 ruling, Scout922: ra2-decomp .asm twins). Extended to 0x781C8C (owner,
    # 2026-11-07): library_identity.py finds no archive member before typinfo.obj at 0x781C8C and the run jumps back
    # into 0x780xxx, so it is the tail of the same handwritten family.
    (0x77EDD0, 0x781C8C, 'VQA32 assembly tail'),
    # Westwood MMX lighting code inside the CRT band (owner, 2026-11-07, library_identity.py): push-ebp prologues,
    # 256-byte-aligned entries, called from LightConvertClass::Set_Tint 0x538D54 and SpotLightClass 0x5DDE06; no
    # archive member matches. 25 inventory rows.
    (0x797440, 0x7995CC, 'MMX lighting assembly'),
)


def merge(intervals):
    out = []
    for a, b in sorted(i for i in intervals if i[0] < i[1]):
        if out and a <= out[-1][1]:
            out[-1][1] = max(out[-1][1], b)
        else:
            out.append([a, b])
    return out


def measure(intervals):
    return sum(b - a for a, b in merge(intervals))


def clip(intervals, start, end):
    return [(max(a, start), min(b, end)) for a, b in intervals if min(b, end) > max(a, start)]


def inside(entry, intervals):
    return any(a <= entry < b for a, b in intervals)


def ledger(catalog, text, text_start=TEXT_START):
    """Split .text into CRT/library, handwritten assembly and in-scope game code.

    Categories are disjoint: handwritten ranges (plus accepted evidenced inline-asm regions) first,
    then CRT for the rest of [CRT_START, end of .text). In-scope matched bytes are accepted C/C++
    bodies outside both; the in-scope total is .text minus both categories minus the frozen padding
    of in-scope regions (padding of unaccepted code is not yet known)."""
    end = text_start + text
    regions = catalog['regions']
    located = [r for r in regions if 'entry' in r]
    asm = merge(clip([(a, b) for a, b, _ in HANDWRITTEN] +
                     [(r['entry'], r['entry'] + r['body_bytes']) for r in located if evidenced_inline_asm(r)],
                     text_start, end))
    crt_span = clip([(CRT_START, end)], text_start, end)
    asm_bytes, crt_total = measure(asm), measure(crt_span)
    crt_bytes = crt_total - measure(clip(asm, CRT_START, end))
    crt = [tuple(i) for i in crt_span]

    def scope(r):
        if 'entry' not in r:
            return 'game'
        if inside(r['entry'], asm):
            return 'asm'
        return 'crt' if inside(r['entry'], crt) else 'game'
    in_scope = [r for r in regions if scope(r) == 'game']
    matched = sum(r['body_bytes'] for r in in_scope if not evidenced_inline_asm(r))
    padding = sum(r['padding_bytes'] for r in in_scope)
    return dict(crt=crt_bytes, crt_matched=sum(r['body_bytes'] for r in regions
                                               if scope(r) == 'crt' and not evidenced_inline_asm(r)),
                asm=asm_bytes, asm_matched=sum(r['body_bytes'] for r in regions if scope(r) == 'asm'
                                               and not evidenced_inline_asm(r)),
                in_scope_matched=matched, in_scope_total=text - crt_bytes - asm_bytes - padding)


def best_scores(index_path, sweep_path):
    """Best recorded score per unaccepted entry: the per-row miss index (docs/MISSES_INDEX.md) first,
    then the seed sweep's results.jsonl (private build output, optional)."""
    best = {}
    if sweep_path and sweep_path.exists():
        for line in sweep_path.read_text().splitlines():
            try:
                row = json.loads(line)
            except ValueError:
                continue
            if row.get('score') is None or row.get('entry') is None:
                continue
            entry = int(row['entry'], 16)
            best[entry] = max(best.get(entry, 0.0), float(row['score']))
    if index_path and index_path.exists():
        for line in index_path.read_text().splitlines():
            cells = [c.strip() for c in line.strip().strip('|').split('|')]
            if len(cells) > 2 and cells[0].startswith('0x'):
                try:
                    best[int(cells[0], 16)] = float(cells[2])
                except ValueError:
                    pass
    return best


def functions(treemap, false_rows, best):
    """In-scope function counts and the partial-match average. In scope: entry below CRT_START, outside
    the HANDWRITTEN ranges, not a known false row. Each unaccepted function counts at its best recorded
    score (0 when none is recorded), weighted by bytes and per function."""
    asm = [(a, b) for a, b, _ in HANDWRITTEN]
    rows = [r for r in treemap if r['entry'] < CRT_START and not inside(r['entry'], asm)
            and r['entry'] not in false_rows]
    done = [r for r in rows if r['status'] == 'exact']
    open_rows = [r for r in rows if r['status'] != 'exact']
    scored = [r for r in open_rows if r['entry'] in best]
    total = sum(r['bytes'] for r in rows)
    weighted = sum(r['bytes'] for r in done) + sum(r['bytes'] * best.get(r['entry'], 0.0) / 100 for r in open_rows)
    per_function = (len(done) * 100 + sum(best.get(r['entry'], 0.0) for r in open_rows)) / len(rows) if rows else 0.0
    return dict(total=len(rows), done=len(done), scored=len(scored), open=len(open_rows),
                fuzzy_bytes=100.0 * weighted / total if total else 0.0, fuzzy_functions=per_function)


def load_assembly(repository=REPOSITORY):
    """Handwritten assembly reproduced from source (assembly_link.py --write): procedures assembled from the
    adopted TibSun .asm files and proved against .text. None when the manifest is absent."""
    path = repository / 'manifests/original_target/assembly.json'
    if not path.exists():
        return None
    regions = json.loads(path.read_text())['regions']
    return dict(bytes=sum(r['bytes'] for r in regions), regions=len(regions))


def load_library(repository=REPOSITORY):
    """Linked library ledger from manifests/original_target/library.json (library_link.py --write): the
    archive sections the build links in place of scaffold. None when the manifest is absent."""
    path = repository / 'manifests/original_target/library.json'
    if not path.exists():
        return None
    regions = json.loads(path.read_text())['regions']
    return dict(bytes=sum(r['bytes'] for r in regions), sections=len(regions))


def load_functions(repository=REPOSITORY):
    """Function stats from build/treemap.json (treemap.py --json, regenerated at each landing). None when
    the treemap is absent, so STATUS still renders."""
    path = repository / 'build/treemap.json'
    if not path.exists():
        return None
    false_path = repository / 'manifests/original_target/false_rows.json'
    false_rows = {int(r['entry'], 16) for r in json.loads(false_path.read_text())['rows']} if false_path.exists() else set()
    best = best_scores(repository / 'docs/MISSES_INDEX.md', repository / 'build/sweep-6b/results.jsonl')
    return functions(json.loads(path.read_text()), false_rows, best)


def coverage(catalog):
    regions = catalog['regions']
    return dict(regions=len(regions), body=sum(r['body_bytes'] for r in regions if not evidenced_inline_asm(r)),
                padding=sum(r['padding_bytes'] for r in regions),
                evidenced_inline_asm=sum(r['body_bytes'] for r in regions if evidenced_inline_asm(r)))


def history(repository=REPOSITORY, cache_path=None):
    """Coverage of every commit that changed the catalog. Each commit's coverage is
    computed once and kept in an ignored cache keyed by the full commit hash (commits
    are immutable, so the cache is append-only); only new commits are parsed."""
    cache_path = cache_path or repository / 'build/status-history-cache.json'
    try:
        cache = json.loads(cache_path.read_text())
    except (OSError, ValueError):
        cache = {}
    log = subprocess.run(['git', 'log', '--reverse', '--format=%H %h %cI %s', '--', catalog_io.LEGACY, catalog_io.SHARDED],
                         cwd=repository, capture_output=True, text=True, check=True).stdout
    rows, changed = [], False
    for line in log.splitlines():
        full, commit, stamp, subject = line.split(' ', 3)
        if full not in cache:
            catalog = catalog_io.load_git(repository, full, references=False)
            cache[full] = None if catalog is None else coverage(catalog)   # None: the commit removed the catalog
            changed = True
        if cache[full] is None:
            continue
        rows.append(dict(commit=commit, time=datetime.fromisoformat(stamp), subject=subject, **cache[full]))
    if changed:
        cache_path.parent.mkdir(parents=True, exist_ok=True)
        cache_path.write_text(json.dumps(cache, sort_keys=True))
    return rows


def text_bytes(executable):
    import pefile
    pe = pefile.PE(executable, fast_load=True)
    return next(s.Misc_VirtualSize for s in pe.sections if s.Characteristics & 0x20)


def render(catalog, text, rows, funcs=None, lib=None, asm=None):
    now = coverage(catalog)
    scaffold = text - now['body'] - now['padding'] - now['evidenced_inline_asm'] - (lib['bytes'] if lib else 0) - (asm['bytes'] if asm else 0)
    populations = Counter(r['population'] for r in catalog['regions'])
    split = ledger(catalog, text)
    lib_bytes = lib['bytes'] if lib else 0
    asm_bytes = asm['bytes'] if asm else 0
    reproduced = now['body'] + now['evidenced_inline_asm'] + now['padding'] + lib_bytes + asm_bytes
    lines = [BEGIN, '',
             '| Measure | Value |', '| --- | ---: |',
             '| **Reproduced .text** (built from real inputs, not copied from the original: C/C++ source + '
             'evidenced inline asm + frozen padding + linked library members + assembled handwritten asm; 100% when done) '
             '| **{:,} of {:,} ({:.2f}%)** |'.format(reproduced, text, 100.0 * reproduced / text),
             '| Accepted regions | {:,} |'.format(now['regions']),
             '| Matched C/C++ body bytes | {:,} ({:.2f}% of .text) |'.format(now['body'], 100.0 * now['body'] / text),
             '| Evidenced inline assembly body bytes (cited TibSun `__asm`; own category beside library code, '
             'not C/C++ credit) | {:,} ({:.2f}% of .text) |'.format(now['evidenced_inline_asm'],
                                                            100.0 * now['evidenced_inline_asm'] / text),
             '| Frozen padding bytes | {:,} |'.format(now['padding']),
             '| C runtime / library bytes (.text >= 0x{:X}; library ledger, not source credit) '
             '| {:,} ({:.2f}% of .text; {:,} matched) |'.format(CRT_START, split['crt'], 100.0 * split['crt'] / text,
                                                              split['crt_matched']),
             ] + ([
             '| Linked library bytes (identified archive members built from the pinned SP3 LIBCMT/COMSUPP and VC98 import libraries; '
             'manifests/original_target/library.json; not source credit) | {:,} in {:,} sections ({:.2f}% of .text) |'
             .format(lib['bytes'], lib['sections'], 100.0 * lib['bytes'] / text),
             ] if lib else []) + [
             ] + ([
             '| Handwritten assembly reproduced from adopted TibSun .asm sources (manifests/original_target/assembly.json; '
             'own ledger) | {:,} in {:,} procedures |'.format(asm['bytes'], asm['regions']),
             ] if asm else []) + [
             '| Handwritten blitter/codec assembly bytes ({} owner-ruled ranges plus evidenced inline asm; '
             'own ledger) | {:,} ({:.2f}% of .text) |'.format(len(HANDWRITTEN), split['asm'], 100.0 * split['asm'] / text),
             '| In-scope matched C/C++ bytes (of .text minus CRT, handwritten assembly and in-scope padding) '
             '| {:,} of {:,} ({:.2f}%) |'.format(split['in_scope_matched'], split['in_scope_total'],
                                               100.0 * split['in_scope_matched'] / split['in_scope_total']
                                               if split['in_scope_total'] else 0.0),
             ] + ([
             '| In-scope functions matched (game code below 0x{:X}, handwritten ranges and known false rows '
             'excluded) | {:,} of {:,} ({:.2f}%) |'.format(CRT_START, funcs['done'], funcs['total'],
                                                       100.0 * funcs['done'] / funcs['total']),
             '| Partial-match average (unmatched functions at best recorded score, 0 if none, sized by inventory bounds; {:,} of {:,} '
             'scored) | {:.2f}% by bytes, {:.2f}% per function |'.format(funcs['scored'], funcs['open'],
                                                                       funcs['fuzzy_bytes'], funcs['fuzzy_functions']),
             ] if funcs else []) + [
             '| Original .text scaffold bytes | {:,} |'.format(scaffold),
             '| Logical .text bytes | {:,} |'.format(text),
             '| Compiler populations | {} |'.format(len(populations)), '']
    if rows:
        lines += ['Manifest history (body bytes added per commit, and per hour since the previous commit):', '',
                  '| Commit | Time | Regions | Body bytes | +Body | +Body/hour |',
                  '| --- | --- | ---: | ---: | ---: | ---: |']
        previous = None
        for row in rows:
            added = row['body'] - previous['body'] if previous else 0
            hours = (row['time'] - previous['time']).total_seconds() / 3600 if previous else 0
            rate = '{:,.0f}'.format(added / hours) if hours > 0 else ''
            lines.append('| `{}` | {} | {:,} | {:,} | {:+,} | {} |'.format(
                row['commit'], row['time'].strftime('%Y-%m-%d %H:%M %z'), row['regions'], row['body'],
                added, rate))
            previous = row
        lines.append('')
    lines.append(END)
    return '\n'.join(lines)


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('--config', type=Path, default=REPOSITORY / 'private/original-target.local.json')
    parser.add_argument('--text-bytes', type=int, help='override instead of reading the executable')
    parser.add_argument('--write', action='store_true', help='replace the generated block in docs/STATUS.md')
    args = parser.parse_args()
    catalog = catalog_io.load(catalog_io.locate(REPOSITORY), references=False)
    text = args.text_bytes or text_bytes(json.loads(args.config.read_bytes())['executable'])
    block = render(catalog, text, history(), load_functions(), load_library(), load_assembly())
    if not args.write:
        print(block); return
    path = REPOSITORY / 'docs/STATUS.md'
    current = path.read_text()
    if BEGIN not in current or END not in current:
        sys.exit('docs/STATUS.md has no generated markers')
    head, rest = current.split(BEGIN, 1)
    path.write_text(head + block + rest.split(END, 1)[1])
    print('updated', path)


if __name__ == '__main__':
    main()
