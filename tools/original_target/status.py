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
import re
from pathlib import Path
import subprocess
import sys

import catalog_io
from build import FIXED_LAYOUT

REPOSITORY = Path(__file__).resolve().parents[2]
BEGIN, END = '<!-- generated:status -->', '<!-- /generated:status -->'


def evidenced_inline_asm(region):
    """Regions whose source holds cited TibSun `__asm` (canonical.validate_inline_asm):
    their own ledger category beside library code, not C/C++ source credit."""
    return region.get('inline_asm') == 'evidenced'


def count_calibrated(region):
    """Regions whose named-local or helper forms were chosen by count calibration (user decision
    2026-11-09; canonical.validate_count_calibrated): matched C/C++, reported as their own category."""
    return region.get('count_calibrated') is True and not evidenced_inline_asm(region)


# Manifest flags that mark an accepted row resting on a judgment call (decision policy, user 2026-11-09):
# plausible but unprovable source forms. Twin-evidenced categories (evidenced inline asm) are not here.
# split_declaration (review 2026-11-11, docs/DECISIONS.md): a row whose bytes depend on a header keeping a
# second declaration of a module's class (canonical.validate_split_declaration).
# declarations_only (review 2026-11-12, docs/DECISIONS.md "Container member joins on their own frozen bound"):
# a row of a source that keeps every shared/vector_class.hpp member undefined (canonical.validate_declarations_only).
JUDGMENT_FLAGS = ('count_calibrated', 'split_declaration', 'declarations_only')


def judgment_coverage(catalog):
    """(regions, body bytes) of accepted rows carrying any JUDGMENT_FLAGS flag."""
    rows = [r for r in catalog['regions'] if any(r.get(flag) is True for flag in JUDGMENT_FLAGS)]
    return len(rows), sum(r['body_bytes'] for r in rows)


def own_bound_coverage(catalog):
    """(rows, body bytes, donor populations) of rows that compile in a unit's population and
    take their bound, gap padding and provenance from the frozen pass of `bound_population`
    (container member joins, docs/DECISIONS.md)."""
    rows = [r for r in catalog['regions'] if 'bound_population' in r]
    return len(rows), sum(r['body_bytes'] for r in rows), len({r['bound_population'] for r in rows})


def own_bound_line(catalog):
    rows, body, donors = own_bound_coverage(catalog)
    return ('| Rows bound by their own frozen population (joined into a unit\'s translation unit; bound, gap '
            'padding and provenance from `bound_population`\'s pass) | {:,} rows, {:,} bytes, {:,} donor populations |'
            .format(rows, body, donors))


JOINED_UNDER_GR = "joined under the unit's /GR"
JOINED_UNDER_OPTIONS = "joined under the unit's options"


def joined_under(prefix, repository=None):
    """(rows, bytes, units) of joined rows whose manifest note starts with `prefix`."""
    path = (repository or Path(__file__).resolve().parents[2]) / 'manifests/original_target/modules.json'
    modules = json.loads(path.read_text()) if path.exists() else {}
    rows = [(u, j) for u, item in modules.items() for j in item.get('joined', [])
            if str(j.get('options', '')).startswith(prefix)]
    return len(rows), sum(j.get('bytes', 0) for _, j in rows), len({u for u, _ in rows})


def joined_under_gr(repository=None):
    """(rows, bytes, units) of rows that joined their unit only by taking the unit's /GR
    (docs/DECISIONS.md "Own-bound joins widened"; the manifest note on each joined entry)."""
    return joined_under(JOINED_UNDER_GR, repository)


def joined_under_options(repository=None):
    """(rows, bytes, units) of rows frozen under another option set than their unit's (more than
    /GR) that joined under the unit's (docs/DECISIONS.md "Stand-ins frozen under another
    optimisation option than their unit"; note "joined under the unit's options (frozen under X)")."""
    return joined_under(JOINED_UNDER_OPTIONS, repository)


def joined_under_gr_line():
    rows, body, units = joined_under_gr()
    other = joined_under_options()
    return ('| Rows joined under the unit\'s /GR (frozen without /GR; every RTTI structure the unit emits still '
            'proven at its original address; manifest note "joined under the unit\'s /GR") / under the unit\'s '
            'options (frozen under another option set; the unit\'s set evidenced by its own rows; manifest note '
            '"joined under the unit\'s options (frozen under X)") | /GR: {:,} rows, {:,} bytes, {:,} units; '
            'options: {:,} rows, {:,} bytes, {:,} units |'.format(rows, body, units, *other))


def unit_evidence_lines(catalog):
    """`$E` rows whose unit rests on the original .CRT$XCU table (docs/DECISIONS.md "$E initialiser/atexit
    rows placed by the original .CRT$XCU table"): one line per flagged tier (row field unit_evidence,
    canonical.validate_unit_evidence), plus the rows the table places in one evidenced unit (no flag)."""
    from canonical import UNIT_EVIDENCE, unit_evidence_counts
    counts = unit_evidence_counts(catalog)
    about = {'xcu-exclusion': 'the previous unit already owns that header static',
             'xcu-prefix+map': 'prefix rule and unit map agree',
             'xcu-prefix': 'prefix rule alone'}
    single = joined_unit_evidence('xcu single')
    lines = ['| `$E` rows joined to the one unit the .CRT$XCU table evidences (no flag; manifest note "xcu single") '
             '| {:,} rows, {:,} bytes, {:,} units |'.format(*single)]
    for tier in UNIT_EVIDENCE:
        lines.append('| `$E` rows joined to a .CRT$XCU boundary pick, flag `unit_evidence: {}` ({}; bytes cannot tell '
                     'the two candidates apart) | {:,} rows, {:,} bytes |'.format(tier, about[tier], *counts[tier]))
    return lines


def joined_unit_evidence(note, repository=None):
    """(rows, bytes, units) of joined rows whose module-item record carries `unit_evidence` == note."""
    path = (repository or Path(__file__).resolve().parents[2]) / 'manifests/original_target/modules.json'
    modules = json.loads(path.read_text()) if path.exists() else {}
    rows = [(u, j) for u, item in modules.items() for j in item.get('joined', []) if j.get('unit_evidence') == note]
    return len(rows), sum(j.get('bytes', 0) for _, j in rows), len({u for u, _ in rows})


def address_named_line(catalog):
    """Address-free source file names (rename_files.py): the per-function files that still
    carry an address, by why they keep it."""
    import rename_files
    counts = rename_files.remaining_counts(catalog)
    total = sum(n for k, n in counts.items())
    other = total - counts['collision'] - counts['seed name'] - counts['renameable']
    return ('| Address-named source files remaining (collision / seed name; rename_files.py) | {:,} files: '
            'collision {:,}, seed name {:,}, not yet renamed {:,}, other {:,} (no unit or folder spelling, not one '
            'accepted row) |'.format(total, counts['collision'], counts['seed name'], counts['renameable'], other))


def generated_data_line():
    """Generated data modules (docs/DECISIONS.md "fastmath tables as a generated data module"):
    tracked generators whose assembled output build.py proves as one whole block."""
    import json
    path = REPOSITORY / 'manifests/original_target/generated_data.json'
    modules = json.loads(path.read_text())['modules'] if path.exists() else []
    size = sum(int(m['range'][1], 16) - int(m['range'][0], 16) for m in modules)
    flagged = sum(len(m.get('flagged_constants', [])) for m in modules)
    return ('| Generated data modules (tracked generator, no original bytes in the repository; each proven at '
            'build time as one whole block at its original addresses; not C/C++ credit) | {:,} module(s), {:,} '
            'bytes; {:,} flagged constant(s) ({}) |'.format(len(modules), size, flagged,
                                                          ', '.join(m['name'] for m in modules) or 'none'))


def data_bound_by_lines(catalog):
    """Data-only bindings (docs/DECISIONS.md "Data-only symbols bound by section layout / bss by
    elimination"): symbols each source declares (row field data_bound_by), and the bytes of the
    proven module-map sections that hold a layout-bound symbol. Elimination earns nothing."""
    import json
    kinds = {'layout': set(), 'elimination': set()}
    sources = set()
    for row in catalog['regions']:
        for name, kind in (row.get('data_bound_by') or {}).items():
            kinds[kind].add((row['source'], name)); sources.add(row['source'])
    layout_bytes = 0
    for path in sorted((REPOSITORY / 'manifests/original_target/modules').glob('*.json')):
        entry = json.loads(path.read_text())
        layout_bytes += sum(c['bytes'] for c in entry.get('data', []) if 'layout' in (c.get('bound_by') or ()))
    return ['| Data bound by enclosed section layout (flag data_bound_by: layout; the split into declarations '
            'is a hypothesis) | {:,} symbols in {:,} source(s); {:,} bytes in the proven sections that hold them |'
            .format(len(kinds['layout']), len({s for s, _ in kinds['layout']}), layout_bytes),
            '| Data bound by bss elimination (flag data_bound_by: elimination; earns nothing by itself) '
            '| {:,} symbols in {:,} source(s) |'.format(len(kinds['elimination']), len({s for s, _ in kinds['elimination']}))]


DECLARATIONS_ONLY_DEFINE = re.compile(rb'^[ \t]*#[ \t]*define[ \t]+VECTOR_DECLARATIONS_ONLY\b', re.M)


def declarations_only_files(catalog, read=None):
    """Accepted sources that define VECTOR_DECLARATIONS_ONLY (they depend on the switch whether or
    not a row of theirs carries the declarations_only flag; owner ruling 2026-11-12)."""
    def disk(source):
        path = REPOSITORY / source
        return path.read_bytes() if path.is_file() else b''
    read = read or disk
    return sorted(s for s in {r['source'] for r in catalog['regions'] if r.get('source')}
                  if DECLARATIONS_ONLY_DEFINE.search(read(s)))


def declarations_only_line(catalog, read=None):
    files = declarations_only_files(catalog, read)
    return ('| Files depending on VECTOR_DECLARATIONS_ONLY (container members left undefined; per file, no bytes '
            'counted) | {:,} ({}) |'.format(len(files), ', '.join(Path(f).name for f in files) or 'none'))


def calibrated_coverage(catalog):
    """(regions, body bytes) of count-calibrated rows; a subset of the matched C/C++ body bytes."""
    rows = [r for r in catalog['regions'] if count_calibrated(r)]
    return len(rows), sum(r['body_bytes'] for r in rows)


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


def functions(treemap, false_rows, best, drafts=()):
    """In-scope function counts and the partial-match average. In scope: entry below CRT_START, outside
    the HANDWRITTEN ranges, not a known false row. Each unaccepted function counts at its best recorded
    score (0 when none is recorded), weighted by bytes and per function. `drafts`: entries of non-exact
    drafts placed in source (non_exact.json); `with_source` counts in-scope functions that are exact or
    have such a draft."""
    asm = [(a, b) for a, b, _ in HANDWRITTEN]
    rows = [r for r in treemap if r['entry'] < CRT_START and not inside(r['entry'], asm)
            and r['entry'] not in false_rows]
    done = [r for r in rows if r['status'] == 'exact']
    open_rows = [r for r in rows if r['status'] != 'exact']
    scored = [r for r in open_rows if r['entry'] in best]
    total = sum(r['bytes'] for r in rows)
    weighted = sum(r['bytes'] for r in done) + sum(r['bytes'] * best.get(r['entry'], 0.0) / 100 for r in open_rows)
    per_function = (len(done) * 100 + sum(best.get(r['entry'], 0.0) for r in open_rows)) / len(rows) if rows else 0.0
    drafts = set(drafts)
    return dict(total=len(rows), done=len(done), scored=len(scored), open=len(open_rows),
                with_source=len(done) + sum(1 for r in open_rows if r['entry'] in drafts),
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
    import non_exact
    drafts = {int(r['entry'], 16) for r in non_exact.load_manifest(repository).get('rows', [])}
    return functions(json.loads(path.read_text()), false_rows, best, drafts)


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


FILE_BACKED_NON_CODE = 711662   # headers + .rdata + raw .data + .rsrc of the target (docs/DATA_AS_SOURCE.md)


def proven_data(entries):
    """Bytes the module maps prove at their original addresses, deduplicated by address
    (several units prove one COMDAT copy), by kind. A class hierarchy descriptor that differs
    only in the ambiguous bit (DECISIONS, user-confirmed exception) counts without its
    attributes word: no credit for the bit."""
    copies, known = {}, set()
    for e in entries:
        for c in e['data'] + e['kept_elsewhere']:
            a = int(c['address'], 16)
            if c.get('known_difference'):
                known.add(a)
                if c['bytes'] > 8:
                    copies.setdefault(a + 8, (c['bytes'] - 8, c['kind']))
                copies.setdefault(a, (4, c['kind']))
                continue
            if copies.get(a, (0, ''))[0] < c['bytes']:
                copies[a] = (c['bytes'], c['kind'])
    covered, by_kind, end = 0, Counter(), 0
    for a in sorted(copies):
        n, kind = copies[a]
        start = max(a, end)
        if a + n > start:
            covered += a + n - start
            by_kind[kind] += a + n - start
            end = a + n
    return covered, by_kind, len(known)


def module_form(repository=REPOSITORY, catalog=None):
    """Modules in original-translation-unit form (manifests/original_target/modules/*.json, written by
    module_ledger.py from a kept-intermediates build): functions, code bytes and compiler-emitted data
    bytes proven at their original addresses. The data line is its own ledger, not .text credit."""
    maps = sorted((repository / 'manifests/original_target/modules').glob('*.json'))
    entries = [json.loads(m.read_text()) for m in maps]
    # Withheld template details (docs/DECISIONS.md header_gaps): recorded per module, no bytes.
    declared = repository / 'manifests/original_target/modules.json'
    declared = json.loads(declared.read_text()) if declared.exists() else {}
    gaps = {name: len(item.get('header_gaps', [])) for name, item in declared.items()}
    return dict(modules=len(entries), header_gaps=sum(gaps.values()),
                header_gap_modules=sum(1 for n in gaps.values() if n), functions=sum(len(e['code']) for e in entries),
                code_bytes=sum(e['code_bytes'] for e in entries),
                data_bytes=sum(e['data_bytes_proven'] for e in entries),
                data_sections=sum(len(e['data']) - len(e['kept_elsewhere']) for e in entries),
                kept_elsewhere_bytes=sum(c['bytes'] for e in entries for c in e['kept_elsewhere']),
                names=[e['module'] for e in entries], proven=proven_data(entries),
                **route_a_form(entries, source_reader(repository), catalog))


LOCAL_PATH_MARK = re.compile(r'(?<![\w.\-])(?:[A-Za-z]:)?[\\/]{1,2}(?:Users|home|private|var|tmp|Volumes)[\\/]'
                             r'|\.claude[\\/]+worktrees')


def manifest_local_paths(repository=REPOSITORY, folder='manifests'):
    """Tracked manifest files that carry an absolute local path (a home directory, a worktree,
    a temporary folder): there must be none; a recorded cause or note names files
    repo-relative. Returns [(relative path, first offending text)]."""
    found = []
    for path in sorted((repository / folder).rglob('*.json')):
        match = LOCAL_PATH_MARK.search(path.read_text(errors='replace'))
        if match:
            found.append((path.relative_to(repository).as_posix(), match.group(0)))
    return found


def local_path_lines(repository=REPOSITORY):
    """A status row only when a tracked manifest carries an absolute local path (there must be none)."""
    found = manifest_local_paths(repository)
    if not found:
        return []
    return ['| **Tracked manifests with an absolute local path (must be 0)** | **{:,}**: {} |'.format(
        len(found), ', '.join(name for name, _ in found[:8]))]


def source_reader(repository):
    def read(path):
        try:
            return (repository / path).read_text(errors='replace')
        except OSError:
            return None
    return read


def route_a_form(entries, read=None, catalog=None):
    """Route A units among the module maps (refile_unit.py): rows of those units that stay in
    their own files, by kind. 'split' rows are held by another frozen population (their bytes
    are the known ceiling of what joining can still move into unit files); 'flipped' rows were
    moved and were not exact; 'kept' rows stay by rule. Stand-in references are other units'
    spellings of a unit's functions (the declaration-cleanup work list). With `read`
    (path -> text or None) the split rows are also broken down by cause, read statically
    from each row's file against its unit's header state (refile_unit.split_class); with `catalog`
    too, the rows the text does not decide are classed by their holding population."""
    units = [e for e in entries if e.get('route') == 'A']
    out = dict(route_a_units=len(units), route_a_functions=sum(len(e['code']) for e in units),
               route_a_code_bytes=sum(e['code_bytes'] for e in units),
               route_a_switches=sum(len(e.get('switches', [])) for e in units),
               route_a_stand_in_references=sum(len(e.get('stand_in_references', [])) for e in units))
    for kind in ('split', 'flipped', 'kept'):
        rows = [r for e in units for r in e.get('residuals', []) if r.get('kind') == kind]
        out['route_a_%s_rows' % kind] = len(rows)
        out['route_a_%s_bytes' % kind] = sum(r['bytes'] for r in rows)
    if read is not None:
        import refile_unit
        classes = refile_unit.split_classes(units, read, catalog)
        for key, _ in refile_unit.SPLIT_CLASSES:
            out['route_a_split_%s_rows' % key], out['route_a_split_%s_bytes' % key] = classes[key]
    return out


def route_a_split_causes(modules):
    """' Split rows by cause: ...' for the Route A line, or '' when the breakdown was not read."""
    import refile_unit
    if any('route_a_split_%s_rows' % key not in modules for key, _ in refile_unit.SPLIT_CLASSES):
        return ''
    notes = dict(template='the file sets a container implementation switch such as VECTOR_IMPLEMENT_* or '
                          'instantiates a template',
                 header_state='another switch, another include or ghidra_types.hpp',
                 stand_in='placeholder-form rows',
                 emitted='comment-only files: an inline constructor, dynamic initialiser or compiler-generated '
                         'destructor another source emits; moves only with its emitter',
                 separate_units='the holding population compiles each source alone',
                 option_set='compiled under another option set than the unit\'s group',
                 own_population='every row of the holding population is this unit\'s: a second compile group a '
                                'join would empty',
                 frozen_bound='a mixed population; the unit\'s frozen inventory pass gives no equal bound',
                 unclassified='nothing in the text or the holding population decides')
    return ' Split rows by cause (read from each row\'s file and its holding population, no compile): ' + '; '.join(
        '{} {:,} rows, {:,} bytes ({})'.format(label, modules['route_a_split_%s_rows' % key],
                                               modules['route_a_split_%s_bytes' % key], notes[key])
        for key, label in refile_unit.SPLIT_CLASSES) + '.'


def route_a_line(modules):
    return ('| Route A units (one source per original translation unit under its own header state, refile_unit.py; '
            'part of the Module form line) | {:,} unit(s): {:,} functions, {:,} code bytes; residual rows in their own '
            'files: **split {:,} rows, {:,} bytes** (held by another frozen population; the known ceiling of what '
            'joining can still move), **flipped {:,} rows, {:,} bytes** (not exact when moved), kept by rule {:,} rows, '
            '{:,} bytes; {:,} per-unit switches kept; {:,} stand-in references from other units.{} |'.format(
                modules['route_a_units'], modules['route_a_functions'], modules['route_a_code_bytes'],
                modules['route_a_split_rows'], modules['route_a_split_bytes'],
                modules['route_a_flipped_rows'], modules['route_a_flipped_bytes'],
                modules['route_a_kept_rows'], modules['route_a_kept_bytes'],
                modules['route_a_switches'], modules['route_a_stand_in_references'], route_a_split_causes(modules)))


def non_exact_line(drafts):
    """STATUS line for non-exact drafts in source: (functions, bytes, mean score); no .text credit."""
    count, size, mean = drafts
    return ('| Non-exact drafts in source (#ifdef RA2_NON_MATCHING, manifests/original_target/non_exact.json; '
            'never compiled by the exact build; no .text credit) | {:,} functions, {:,} bytes, mean score {:.2f} |'
            .format(count, size, mean))


def render(catalog, text, rows, funcs=None, lib=None, asm=None, drafts=(0, 0, 0.0)):
    now = coverage(catalog)
    modules = module_form(catalog=catalog)
    calibrated = calibrated_coverage(catalog)
    judgment = judgment_coverage(catalog)
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
             '| Judgment-call bytes (accepted rows resting on a flagged, unprovable choice: {}; read from the manifest) '
             '| {:,} in {:,} regions ({:.2f}% of .text) |'.format(', '.join(JUDGMENT_FLAGS), judgment[1], judgment[0],
                                                                 100.0 * judgment[1] / text),
             '| Accepted regions | {:,} |'.format(now['regions']),
             '| Matched C/C++ body bytes | {:,} ({:.2f}% of .text) |'.format(now['body'], 100.0 * now['body'] / text),
             '| Count-calibrated C/C++ bytes (rows whose named locals were chosen by count calibration under the user\'s '
             'three conditions, 2026-11-09; own category, included in the matched C/C++ bytes above) '
             '| {:,} in {:,} regions ({:.2f}% of .text) |'.format(calibrated[1], calibrated[0], 100.0 * calibrated[1] / text),
             '| Evidenced inline assembly body bytes (cited TibSun `__asm`; own category beside library code, '
             'not C/C++ credit) | {:,} ({:.2f}% of .text) |'.format(now['evidenced_inline_asm'],
                                                            100.0 * now['evidenced_inline_asm'] / text),
             '| Frozen padding bytes | {:,} |'.format(now['padding']),
             '| Module form (sources in original translation-unit form, no addresses; manifests/original_target/modules/) '
             '| {:,} module(s): {:,} functions, {:,} code bytes; **{:,} data bytes compiler-emitted and proven at their '
             'original addresses** in {:,} sections, {:,} bytes of vtable/RTTI copies kept elsewhere; {:,} header gaps '
             '(withheld template details, modules.json header_gaps; no bytes) in {:,} module(s); own ledger, no .text '
             'credit ({}) |'.format(modules['modules'], modules['functions'], modules['code_bytes'], modules['data_bytes'],
                                     modules['data_sections'], modules['kept_elsewhere_bytes'], modules['header_gaps'],
                                     modules['header_gap_modules'], ', '.join(modules['names']) or 'none'),
             ] + ([route_a_line(modules)] if modules['route_a_units'] else []) + [own_bound_line(catalog), joined_under_gr_line()] + unit_evidence_lines(catalog) + [address_named_line(catalog), declarations_only_line(catalog)] + data_bound_by_lines(catalog) + [generated_data_line()] + local_path_lines() + [
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
             '| In-scope functions with source (exact or draft: accepted rows plus non-exact drafts in source) '
             '| {:,} of {:,} ({:.2f}%) |'.format(funcs.get('with_source', funcs['done']), funcs['total'],
                                               100.0 * funcs.get('with_source', funcs['done']) / funcs['total']),
             ] if funcs else []) + [non_exact_line(drafts)] + [
             '| Original .text scaffold bytes | {:,} |'.format(scaffold),
             '| Logical .text bytes | {:,} |'.format(text),
             '| Compiler populations | {} |'.format(len(populations)),
             '| Compile path (build environment, with the pinned VC6 SP3 8447 compiler; docs/ORIGINAL_TARGET_BUILD.md '
             '"Fixed compile folder") | `{}` |'.format(FIXED_LAYOUT),
             '| Populations under /GR (docs/DECISIONS.md "/GR per population") | {} of {} |'.format(
                 len({r['population'] for r in catalog['regions'] if '/GR' in r.get('options', ())}), len(populations)),
             '| Non-code bytes proven at their original addresses (module maps, deduplicated by address; not .text '
             'credit) | {:,} of {:,} file-backed ({:.1f}%); RTTI {:,}, vtables {:,}, strings {:,}; {} class hierarchy '
             'descriptor(s) differ from the original only in the ambiguous bit (known difference; '
             'the attributes word is not counted) |'.format(
                 modules['proven'][0], FILE_BACKED_NON_CODE, 100.0 * modules['proven'][0] / FILE_BACKED_NON_CODE,
                 modules['proven'][1]['rtti'], modules['proven'][1]['vtable'], modules['proven'][1]['string'],
                 modules['proven'][2]), '']
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
    import non_exact
    block = render(catalog, text, history(), load_functions(), load_library(), load_assembly(),
                   non_exact.summary(REPOSITORY))
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
