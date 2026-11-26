#!/usr/bin/env python3
"""Generate the numeric part of docs/STATUS.md (one tree since the layout move, lane O88, 2026-11-25).

The tree's figures come from its catalog (manifests/catalog) and the gated-landing rows of
manifests/progress.csv; the .text size from the supplied executable's section header. The reference
column is src-final: the earlier tree's catalog (manifests/original_target/, kept as read-only evidence;
the source itself is the tag src-final), whose figures cannot change. Its full frozen table and commit
history are in docs/history/SRC_FINAL_STATUS.md. The block between the generated markers in
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
# The rebuild became the exact-match tree of record on this day (user, 2026-11-24, in the owner session) and moved
# to the repository root on 2026-11-25 (lane O88); the earlier tree is the tag src-final, shown as the reference column.
FROZEN_ON = '2026-11-24'


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
    # CellClass::Draw_Shroud_Or_Fog_Shape: a C function whose contiguous-raster branch is the author's `__asm`
    # block 0x4744CB-0x4744FA (owner, lane O72, 2026-11-24, after lanes F58 and O71). Image evidence: an ebp frame
    # in a frame-pointer-omitting unit (its fog twin 0x474510 and the function ending at 0x47425F have none); the
    # compiler spills ecx/eax to [ebp-24h]/[ebp+8] and the block reloads them at once into edx/ecx, then loads
    # esi, edi, ebx from named slots; it compares with the .rdata dword 0x79DAD0 (0xFE, read by no other site)
    # where the wrapped C branch beside it uses the immediate 0FEh; `dec / jg` latches with no zero-trip guard.
    # The archive's cell.cpp:2025 is plain C with two for-loop pairs: RA2 replaced the non-wrapping pair with the
    # block. No archive `__asm`, so no evidenced inline asm (BlitTransLucent50 precedent). ra2-decomp files the
    # YR twin 0x47EFE0 as MASM (f707224 src/ra2asm/cell.asm, not adopted: its prologue and frame are compiler
    # output written as assembly, test (a)). Image bound 0x474260-0x474503 (rebuild image_bounds.csv); 675 B.
    (0x474260, 0x474503, 'CellClass::Draw_Shroud_Or_Fog_Shape'),
    # BlitForward with a C prologue and an `__asm` 50% translucent blend (owner, 2026-11-08, Scout1030): TibSun's
    # BlitTransLucent50 is plain C, so the evidenced inline-asm exception does not apply; 90 B. ra2-decomp's
    # f707224 src/ra2asm/convert.asm (YR twin 0x492830) reproduces these 90 B (lane O71); not adopted: its C
    # prologue written out as assembly fails test (a). No credit change (owner, lane O72, 2026-11-24).
    (0x487080, 0x4870DA, 'BlitTransLucent50 BlitForward'),
    # VQA32 MASM tail: AudioUnzap, VQA_LCW_Uncompress, sosCODEC, ASM_UnVQ_*, cpuid/rdtsc/FPU helpers
    # (OPEN_MISSES Abuf490, Scout845, Scout848 ruling, Scout922: ra2-decomp .asm twins). Extended to 0x781C8C (owner,
    # 2026-11-07): library_identity.py finds no archive member before typinfo.obj at 0x781C8C and the run jumps back
    # into 0x780xxx, so it is the tail of the same handwritten family.
    # The range classifies .text (it stays out of the in-scope denominator); the procedures assembly.json builds inside
    # it are reproduced, not scaffolding: the archive rows, and since 2026-11-24 (user decision, lane O73) the revised,
    # binary-derived Detect_MMX_Availability 0x77F14D and Iso_Blit_Asm1 0x77F2D0 (ra2-decomp src/ra2asm).
    (0x77EDD0, 0x781C8C, 'VQA32 assembly tail'),
    # Westwood MMX lighting code inside the CRT band (owner, 2026-11-07, library_identity.py): push-ebp prologues,
    # 256-byte-aligned entries, called from LightConvertClass::Set_Tint 0x538D54 and SpotLightClass 0x5DDE06; no
    # archive member matches. 25 inventory rows. Reproduced inside it (lane O73, 2026-11-24): the archive's
    # winasm.asm rows and the revised, binary-derived *_Normals_ASM 0x798A00/0x798B00/0x798C00/0x798D20.
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
    path = repository / 'manifests/assembly.json'          # the tree's ledger (tree.ASSEMBLY)
    if not path.exists():
        return None
    regions = json.loads(path.read_text())['regions']
    # Revised, binary-derived lines (user decision 2026-11-24): their own sub-line; Reproduced .text only.
    revised = [r for r in regions if r.get('revised')]
    return dict(bytes=sum(r['bytes'] for r in regions), regions=len(regions),
                revised_bytes=sum(r['bytes'] for r in revised), revised_regions=len(revised))


def load_library(repository=REPOSITORY, frozen=False):
    """Linked library ledger (library_link.py --write): the archive sections the build links in place of
    scaffold. The tree's ledger is manifests/library.json; FROZEN reads src-final's manifests/original_target/
    library.json (its column: src-final credited 12 libcmt rows as C that the tree's ledger links, lane O96 menu
    R1). None when the manifest is absent."""
    path = repository / ('manifests/original_target/library.json' if frozen else 'manifests/library.json')
    if not path.exists():
        return None
    doc = json.loads(path.read_text())
    regions = doc['regions']
    # Linker import thunks in the library band that no real input reproduces (no import library on the VC98 disc
    # names them the way the original does): listed, never credited (lane O96, owner 2026-11-26).
    unlinked = [x for x in doc.get('skipped_other', []) if int(x['entry'], 16) >= 0x781000]
    return dict(bytes=sum(r['bytes'] for r in regions), sections=len(regions),
                entries={int(r['entry'], 16) for r in regions}, unlinked=unlinked)


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


# Tracked manifests carry no absolute local path (tests/test_original_target_refile_unit.py checks the tree).
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


def main_only(main_catalog, rebuild_catalog, library_entries=()):
    """(functions, body bytes) of main's accepted rows whose entry has no row in the rebuild catalog: matched in
    src-final (main), not yet in the rebuild (lane O50, 2026-11-22; the line stays until it reaches zero). Rows the
    tree's library ledger links (LIBRARY_ENTRIES: libcmt code src-final took as C, lane O96 menu R1) are reproduced
    here as library code and leave the line."""
    have = {r['entry'] for r in rebuild_catalog['regions']} | set(library_entries)
    rows = [r for r in main_catalog['regions'] if r.get('entry') is not None and r['entry'] not in have]
    return len(rows), sum(r['body_bytes'] for r in rows)


def rebuild_categories(repository=REPOSITORY, main_catalog=None):
    """Whole-unit category bytes of the rebuild catalog (manifests/catalog: the rows the whole-file build
    accepts), counted as main's coverage() counts main's: C/C++ body (no padding, no evidenced inline asm),
    evidenced inline asm body, frozen padding. None when the rebuild has no catalog."""
    path = Path(repository) / 'manifests/catalog'
    if not path.exists():
        return None
    catalog = catalog_io.load(path, references=False)
    return dict(coverage(catalog), inline_asm_rows=sum(1 for r in catalog['regions'] if evidenced_inline_asm(r)),
                flagged_judgment=rebuild_flagged_judgment(catalog, repository),
                main_only=main_only(main_catalog, catalog, (load_library(repository) or {}).get('entries', ()))
                if main_catalog is not None else None)


# A rebuild judgment note under main's definition (owner ruling, 2026-11-22, lane O46): the note names one of main's
# JUDGMENT_FLAGS as its first word (`count_calibrated: ...`). The rebuild's notes are free text with no flag field,
# so only a note that names the flag is counted; one that describes the same kind of choice in other words is not.
# A rebuild judgment note may start with its bracketed kind (lane O63, 2026-11-23: "[iv-b] count_calibrated: ...").
FLAGGED_NOTE = re.compile(r'\s*(?:\[[a-z-]+\]\s*)?(?:%s)\b' % '|'.join(JUDGMENT_FLAGS))
HEX_ENTRY = re.compile(r'\b(?:0x)?(?=[0-9A-Fa-f]*[0-9])([0-9A-Fa-f]{6,8})\b')


def rebuild_flagged_judgment(catalog, repository=REPOSITORY):
    """(rows, body bytes) of the rebuild catalog's rows under main's judgment definition (JUDGMENT_FLAGS): a row
    whose provenance.csv judgment note names a flag (FLAGGED_NOTE), or that a header_judgments.csv form whose
    judgment note names a flag lists among its dependents (an address there that is a catalog entry). Counted over
    the catalog (whole-unit accepted rows), as main counts its accepted regions."""
    import csv
    manifest = Path(repository) / 'manifests'
    body = {'%08x' % r['entry']: r['body_bytes'] for r in catalog['regions']}
    flagged = set()
    path = manifest / 'provenance.csv'
    for r in (csv.DictReader(path.open()) if path.exists() else []):
        if FLAGGED_NOTE.match(r.get('judgment') or '') and r.get('entry') in body:
            flagged.add(r['entry'])
    path = manifest / 'header_judgments.csv'
    for r in (csv.DictReader(path.open()) if path.exists() else []):
        if FLAGGED_NOTE.match(r.get('judgment') or ''):
            flagged |= {m.group(1).lower().zfill(8) for m in HEX_ENTRY.finditer(r.get('dependents') or '')} & set(body)
    return len(flagged), sum(body[e] for e in flagged)


def kb(value):
    return '{:,.1f} KB'.format(value / 1000.0)


def rebuild_lines(text, main, lib_bytes, asm_bytes, judgment, main_clean, repository=REPOSITORY, categories=None,
                  main_catalog=None, revised_bytes=0, main_lib_bytes=None, unlinked=()):
    """Main's tree and the rebuild tree (rebuild/, on main since 2026-11-19) side by side, never added, on the same
    category lines (lane O41, 2026-11-22; AGENTS.md status measures): Reproduced .text is the sum of C/C++ body,
    evidenced inline asm, frozen padding, linked library members and assembled handwritten asm in both trees. The
    rebuild's whole-unit categories come from its catalog; its function-level figures (each function checked on its
    own, as ra2-decomp reports) and structure from the last gated-landing row of manifests/progress.csv. Both
    builds link the same library members and assemble the same procedures (manifests/original_target/library.json,
    assembly.json). MAIN is coverage() of main's catalog; JUDGMENT main's (regions, body bytes); MAIN_CLEAN main's
    C/C++ body bytes outside its judgment-flagged rows. ASM_BYTES is all assembled bytes; REVISED_BYTES the part of
    it in the revised, binary-derived category (user decision 2026-11-24), shown on its own line."""
    import csv
    path = Path(repository) / 'manifests/progress.csv'
    if not path.exists():
        return []
    rows = [r for r in csv.DictReader(path.open()) if r.get('tree') in ('rebuild-trial', 'main')]
    if not rows:
        return []
    r = rows[-1]
    cat = categories if categories is not None else rebuild_categories(repository, main_catalog)
    if cat is None:
        return []
    pct = lambda value: '{:.2f}%'.format(100.0 * value / text)
    main_lib_bytes = lib_bytes if main_lib_bytes is None else main_lib_bytes    # src-final's frozen ledger
    import collections
    dlls = collections.Counter(re.search(r'(\w+)\.(?:LIB|DLL)', x['reason']).group(1) if re.search(
        r'(\w+)\.(?:LIB|DLL)', x['reason']) else 'other' for x in unlinked)
    unlinked_text = '{} import thunks, {} B ({})'.format(len(unlinked), 6 * len(unlinked), ', '.join(
        '{} {}'.format(n, d) for d, n in sorted(dlls.items()))) if unlinked else 'none'
    reproduced = main['body'] + main['evidenced_inline_asm'] + main['padding'] + main_lib_bytes + asm_bytes
    whole_repro = cat['body'] + cat['evidenced_inline_asm'] + cat['padding'] + lib_bytes + asm_bytes
    # Function-level body and padding exist from the first progress row with c_body_bytes (series break).
    level = r.get('function_level_c_body_bytes')
    if level:
        fl_body, fl_pad = int(level), int(r['function_level_padding_bytes'])
        fl_repro = fl_body + cat['evidenced_inline_asm'] + fl_pad + lib_bytes + asm_bytes
        # Headline whole-unit (the tree of record's build: every row of a catalogued unit exact in one compile),
        # function-level beside it, labelled (each function checked on its own, as ra2-decomp reports).
        repro_cell = '**{} whole-unit** ({:,} B) ({} function-level, {:,} B)'.format(pct(whole_repro), whole_repro,
                                                                                    pct(fl_repro), fl_repro)
        body_cell = '**{:,} B, {} whole-unit** ({:,} B, {} function-level); gap to src-final {} ({} function-level)'.format(
            cat['body'], pct(cat['body']), fl_body, pct(fl_body), kb(main['body'] - cat['body']), kb(main['body'] - fl_body))
        pad_cell = '{:,} B whole-unit ({:,} B function-level)'.format(cat['padding'], fl_pad)
    else:
        repro_cell = '{} whole-unit ({:,} B); function-level from the next gated landing'.format(pct(whole_repro),
                                                                                                 whole_repro)
        body_cell = '{:,} B, {} whole-unit; gap to src-final {}; function-level from the next gated landing'.format(
            cat['body'], pct(cat['body']), kb(main['body'] - cat['body']))
        pad_cell = '{:,} B whole-unit'.format(cat['padding'])
    if r.get('clean_function_level_source_share_of_text') and level:
        fl_b, w_b = (('{:,} B, '.format(int(r['clean_function_level_c_body_bytes'])),
                      '{:,} B, '.format(int(r['clean_c_body_bytes']))) if r.get('clean_c_body_bytes') else ('', ''))
        clean_cell = ('**{}{} whole-unit** ({}{} function-level) of .text; left out: judgment calls {:,} rows / {:,} B (of '
                      'which header-form dependents {:,} / {:,} B), count-sensitive {:,} rows / {:,} B').format(
            w_b, r['clean_source_share_of_text'], fl_b, r['clean_function_level_source_share_of_text'],
            int(r['judgment_any_rows']),
            int(r['judgment_any_bytes']), int(r['header_dependent_rows']), int(r['header_dependent_bytes']),
            int(r['count_sensitive_rows']), int(r['count_sensitive_bytes']))
    else:
        clean_cell = 'from the next gated landing'
    # Exact with no soft judgment and judgment by kind (review session request, 2026-11-23, lane O63).
    if r.get('soft_free_function_level_c_body_bytes'):
        soft_cell = ('**{:,} B, {} whole-unit** ({:,} B, {} function-level); left out: soft judgment rows {:,} / {:,} B '
                     '(kinds iii, iv-b, vii, the variation forms, reference-borne and header-form dependents of such a form), count-sensitive {:,} rows / '
                     '{:,} B').format(
            int(r['soft_free_c_body_bytes']), r['soft_free_source_share_of_text'],
            int(r['soft_free_function_level_c_body_bytes']), r['soft_free_function_level_source_share_of_text'],
            int(r['soft_judgment_rows']),
            int(r['soft_judgment_bytes']), int(r['count_sensitive_rows']), int(r['count_sensitive_bytes']))
        kind_cell = r.get('judgment_by_kind') or 'none'
    else:
        soft_cell = kind_cell = 'from the next gated landing'
    lines = ['| Tree (numbers side by side, never added) | src-final (the earlier tree, frozen {}; tag src-final) '
             '| this tree (`src/`, `include/`; last gated landing {}) |'.format(FROZEN_ON, r['time_utc']),
             '| --- | ---: | ---: |',
             '| Reproduced .text (both trees: C/C++ body + evidenced inline asm + frozen padding + linked library '
             'members + assembled handwritten asm, the six lines below) | {} ({:,} B) | {} |'.format(
                 pct(reproduced), reproduced, repro_cell),
             '| Fuzzy (best draft scores): ByteMatch\'s fuzzy match, in-scope C/C++ bytes with each function not exact '
             'counted at its best measured draft score against its image-derived bound, (body - differing) / body; no '
             'exact-match credit (bytematch_report.py; progress.csv fuzzy_best_draft) | | {} |'.format(
                 fuzzy_cell(r, repository)),
             *data_line(rows, repository),
             '| C/C++ bodies matched (body bytes: no padding, no evidenced inline asm; the parity line) | {:,} B, {} '
             '| {} |'.format(main['body'], pct(main['body']), body_cell),
             '| Evidenced inline assembly (cited TibSun `__asm`; not C/C++ credit) | {:,} B | {:,} B in {:,} rows '
             '(whole-unit) |'.format(main['evidenced_inline_asm'], cat['evidenced_inline_asm'], cat['inline_asm_rows']),
             '| Frozen padding of accepted rows | {:,} B | {} |'.format(main['padding'], pad_cell),
             '| Linked library members (each tree\'s ledger: the tree\'s adds the 12 libcmt rows src-final took as C, '
             'lane O96 menu R1, and 4 CRT fragments; not linked, no real input reproduces them: {}) | {:,} B | {:,} B |'.format(
                 unlinked_text, main_lib_bytes, lib_bytes),
             '| Assembled handwritten assembly (the same adopted TibSun .asm procedures in both builds) | {:,} B '
             '| {:,} B |'.format(asm_bytes - revised_bytes, asm_bytes - revised_bytes),
             '| Assembled handwritten assembly, revised (binary-derived lines: ra2-decomp src/ra2asm text revising a '
             'cited TibSun archive procedure, allowlisted and flagged; Reproduced .text only, never C/C++; user '
             '2026-11-24) | {:,} B | {:,} B |'.format(revised_bytes, revised_bytes),
             '| C/C++ bodies, exact with no judgment call (each tree\'s judgment rows left out) | {:,} B, {} | {} |'.format(
                 main_clean, pct(main_clean), clean_cell),
             '| C/C++ bodies, exact with no soft judgment (only the judgment rows resting on a choice left out: '
             'in the rebuild kinds iii author idiom unreferenced at the line, iv-b looked-up local no reference has, vii '
             'other, the flagged variation forms and reference-borne forms; main\'s flags are all choices, so its figure is the line above) | {:,} B, {} | {} |'.format(
                 main_clean, pct(main_clean), soft_cell),
             '| Rebuild judgment calls by kind (function-level, rows/body bytes; i reference form chosen by caller '
             'evidence, ii sibling-image helper form, iii author idiom unreferenced at the line, iv-a twin or reference '
             'text at this or a sibling site, iv-b looked-up local no reference has, v image-derived or frozen bound, vi '
             'header-form dependent, joined two references joined at one site, vii other; helper, spelling, compiler-local, '
             'readability-alias flagged variation forms; reference-borne a refused-if-coined form in adopted reference '
             'text; kinds tagged in provenance.csv and header_judgments.csv) | | {} |'.format(
                 kind_cell),
             '| Judgment calls, one definition for both trees (main\'s: the manifest flags {}; in the rebuild, the '
             'accepted rows whose provenance.csv judgment note, or a header_judgments.csv form they depend on, names '
             'one of those flags; whole-unit, body bytes) | {:,} rows / {:,} B | {} |'.format(
                 ', '.join(JUDGMENT_FLAGS), judgment[0], judgment[1],
                 '{:,} rows / {:,} B'.format(*cat['flagged_judgment']) if cat.get('flagged_judgment') else 'n/a'),
             '| Judgment calls, each tree\'s own count (not comparable row for row: main counts the manifest flags '
             'above; the rebuild counts every provenance.csv judgment note, which includes twin-text forms and bounds, '
             'plus the rows listed as dependents of a flagged header form; body bytes) | {:,} rows / {:,} B | {} |'.format(
                 judgment[0], judgment[1],
                 '{:,} rows / {:,} B'.format(int(r['judgment_any_rows']), int(r['judgment_any_bytes']))
                 if r.get('judgment_any_rows') else '{:,} rows / {:,} B (provenance notes only)'.format(
                     int(r['judgment_rows']), int(r['judgment_bytes']))),
             '| Matched in src-final (main), not yet in the rebuild (main\'s accepted entries with no rebuild catalog '
             'row; body bytes; stays until it reaches zero) | | {} |'.format(
                 '{:,} functions / {:,} B'.format(*cat['main_only']) if cat.get('main_only') else 'n/a'),
             '| Accepted | {:,} regions | {:,} functions in {:,} units{} |'.format(main['regions'],
                 int(r['functions_exact']), int(r['units']), '; function-level {:,} functions'.format(
                     int(r['function_level_functions'])) if r.get('function_level_functions') else ''),
             '| Provenance (docs/REBUILD.md; body plus padding) | | adopted {:,} fn / {:,} B; reshaped from '
             'src/ {:,} / {:,} B; fresh {:,} / {:,} B; Windows workers {:,} / {:,} B |'.format(
                 int(r['a_functions']), int(r['a_bytes']), int(r['b_functions']), int(r['b_bytes']),
                 int(r['c_functions']), int(r['c_bytes']), int(r['workers_functions']), int(r['workers_bytes'])),
             '| Rebuild structure | | duplicates {}, externs {} landed / {} all, address-suffixed {}; parked {:,} functions '
             '/ {:,} B (>=1 KB: {} / {:,} B); judgment {} rows / {:,} B; count-sensitive {} rows / {:,} B |'.format(
                 r['duplicates'], r['externs_landed'], r['externs_all'], r['address_suffixed'], int(r['parked_functions']),
                 int(r['parked_bytes']), r['parked_large_functions'], int(r['parked_large_bytes']), r['judgment_rows'],
                 int(r['judgment_bytes']), r['count_sensitive_rows'], int(r['count_sensitive_bytes']))]
    if r.get('inline_asm_rows'):
        # Rows reached through a unit's file-static helper on their own line (review session and user, 2026-11-20).
        lines += ['| Rebuild evidenced inline assembly reached through a file-static helper of its unit | | {:,} rows '
                  '/ {:,} B |'.format(int(r['helper_asm_rows']), int(r['helper_asm_bytes'])),
                  '| Rebuild asm bytes not yet flagged (main flags them; header-template attribution to fix) | | {:,} '
                  'rows / {:,} B |'.format(int(r.get('asm_unflagged_rows') or 0), int(r.get('asm_unflagged_bytes') or 0))]
    return [rebuild_first(line) for line in lines] + ['']


def fuzzy_cell(row, repository=REPOSITORY):
    """STATUS's fuzzy (best draft scores) cell: the last gated landing's progress.csv fuzzy_best_draft, else the
    figure measured now from the checkout (bytematch_report.fuzzy_figure), else a placeholder."""
    if row.get('fuzzy_best_draft'):
        return 'fuzzy (best draft scores): **{}**'.format(row['fuzzy_best_draft'])
    try:
        import bytematch_report
        value = bytematch_report.fuzzy_figure(Path(repository))
    except (OSError, ValueError, KeyError, SystemExit):
        value = None
    if value is None:
        return 'from the next gated landing'
    return 'fuzzy (best draft scores): **{:.2f}%** (measured from the checkout; progress.csv from the next gated ' \
           'landing)'.format(value)

# Lane D's sizing of the earlier tree (docs/DATA_AS_SOURCE.md, 2026-11-13): proven by module maps and other objects.
SRC_FINAL_DATA = (151518, 711662)


def generated_constants(repository=REPOSITORY):
    """The flagged constants of the tree's generated data modules (manifests/generated_data.json): every constant a
    generator reads from the image rather than derives, named on the "with generated modules" sub-line."""
    path = Path(repository) / 'manifests/generated_data.json'
    if not path.exists():
        return []
    return ['{} = 0x{} ({}, derivation unknown)'.format(c['name'], c['value'].upper(), m['name'])
            for m in json.loads(path.read_text())['modules'] for c in m.get('flagged_constants', [])]


def data_line(rows, repository=REPOSITORY):
    """Data reproduced from source (user, 2026-11-25; data_measure.py names the measure): the file-backed non-code
    bytes a compiled unit's object reproduces at their original addresses. A report beside the code figure: not in
    Reproduced .text and not acceptance. From the newest progress row of the series, else the newest measured build.
    Two rows, always together (review-session ruling 2026-11-26, lane O104): the headline "authored" (unit sources
    and tracked assembly) and the sub-line "with generated modules" (plus the blocks generated data modules prove)."""
    row = next((r for r in reversed(rows) if r.get('data_reproduced_bytes')), None)
    if row is not None:
        value, total, when = int(row['data_reproduced_bytes']), SRC_FINAL_DATA[1], 'gated landing ' + row['time_utc']
        generated = int(row['data_with_generated_bytes']) if row.get('data_with_generated_bytes') else None
    else:
        import data_measure
        figure = data_measure.latest_build_figure(repository)
        if figure is None:
            return ['| Data reproduced from source, authored | n/a | from the next gated landing |',
                    '| Data reproduced from source, with generated modules | n/a | from the next gated landing |']
        value, total, when, generated = figure[0], figure[1], 'whole-file build ' + figure[2], figure[3]
    constants = generated_constants(repository)
    return ('| Data reproduced from source, authored (file-backed non-code bytes, 711,662 B: headers, .rdata, raw '
            '.data, .rsrc; equal to the original where a compiled unit\'s object or tracked assembly lays them; a '
            'report, not in Reproduced .text or acceptance; by kind in docs/DATA_AS_SOURCE.md) | {:,} B, {:.1f}% '
            '(lane D sizing, 2026-11-13) | **{:,} B, {:.1f}%** ({}) |\n'  # split below: two rows, always together
            '| Data reproduced from source, with generated modules (authored plus the blocks a tracked generator '
            'computes entry by entry from formulas, proven whole at their original addresses: the fastmath tables, '
            '.data 0x801DC4-0x827E04, 155,712 B; flagged constants: {}) | n/a | {} |').format(
                SRC_FINAL_DATA[0], 100.0 * SRC_FINAL_DATA[0] / SRC_FINAL_DATA[1], value, 100.0 * value / total, when,
                '; '.join(constants) or 'none',
                'from the next gated landing' if generated is None else '{:,} B, {:.1f}% ({})'.format(
                    generated, 100.0 * generated / total, when)).split('\n')


def rebuild_first(line):
    """One table row with the tree of record's column first: '| label | src-final | rebuild |' becomes
    '| label | rebuild | src-final |' (rebuild/ is the tree of record from 2026-11-24)."""
    cells = line.strip().strip('|').split('|')
    if len(cells) != 3 or not line.startswith('|') or not line.endswith('|'):
        raise ValueError('status row with other than three cells: ' + line[:80])
    label, frozen, rebuild = (c.strip() for c in cells)
    return '| {} | {} | {} |'.format(label, rebuild, frozen).replace('|  |', '| |')


def rebuild_history_lines(repository=REPOSITORY, limit=12):
    """Matched C/C++ body bytes per gated landing of the tree of record (manifests/progress.csv, whole-unit),
    with bytes per hour since the previous row that changed them; the last LIMIT such rows."""
    import csv
    from datetime import datetime
    path = Path(repository) / 'manifests/progress.csv'
    if not path.exists():
        return []
    rows = [r for r in csv.DictReader(path.open()) if r.get('tree') in ('rebuild-trial', 'main') and r.get('c_body_bytes')]
    kept = []
    for r in rows:
        if not kept or int(r['c_body_bytes']) != int(kept[-1]['c_body_bytes']):
            kept.append(r)
    if len(kept) < 2:
        return []
    when = lambda r: datetime.strptime(r['time_utc'], '%Y-%m-%dT%H:%MZ')
    lines = ['Rebuild history (manifests/progress.csv; C/C++ body bytes whole-unit per gated landing that changed '
             'them, and per hour since the previous one):', '',
             '| Landing (UTC) | Functions | C/C++ body bytes | +Body | +Body/hour |', '| --- | ---: | ---: | ---: | ---: |']
    window = kept[-(limit + 1):]
    for previous, row in zip(window, window[1:]):
        added = int(row['c_body_bytes']) - int(previous['c_body_bytes'])
        hours = (when(row) - when(previous)).total_seconds() / 3600
        lines.append('| {} | {:,} | {:,} | {:+,} | {} |'.format(row['time_utc'], int(row['functions_exact']),
                     int(row['c_body_bytes']), added, '{:,.0f}'.format(added / hours) if hours > 0 else ''))
    return lines + ['']


def source_split(repository=REPOSITORY):
    """provenance_rollup.rollup over the tree's catalog (None without one)."""
    import provenance_rollup
    path = Path(repository) / 'manifests/catalog'
    if not path.exists():
        return None
    return provenance_rollup.rollup(catalog_io.load(path, references=False)['regions'], Path(repository) / 'manifests')


def source_lines(split=None):
    """The by-source split of the accepted text (provenance_rollup.py), for STATUS."""
    import provenance_rollup
    split = split if split is not None else source_split()
    if split is None:
        return []
    return ['Where the accepted text came from (tools/original_target/provenance_rollup.py over manifests/catalog; '
            'body bytes include evidenced inline asm):', ''] + provenance_rollup.table_lines(split) + ['', provenance_rollup.CAVEAT, '']


def render(catalog, text, lib=None, asm=None, main_lib=None):
    """The generated block: the tree's figures beside src-final's (CATALOG: src-final's frozen catalog)."""
    now = coverage(catalog)
    judgment = judgment_coverage(catalog)
    lib_bytes = lib['bytes'] if lib else 0
    asm_bytes = asm['bytes'] if asm else 0
    main_clean = sum(r['body_bytes'] for r in catalog['regions']
                     if not evidenced_inline_asm(r) and not any(r.get(flag) is True for flag in JUDGMENT_FLAGS))
    revised_bytes = asm.get('revised_bytes', 0) if asm else 0
    lines = [BEGIN, ''] + rebuild_lines(text, now, lib_bytes, asm_bytes, judgment, main_clean, main_catalog=catalog,
                                        revised_bytes=revised_bytes,
                                        main_lib_bytes=main_lib['bytes'] if main_lib else None,
                                        unlinked=lib.get('unlinked', ()) if lib else ()) + rebuild_history_lines() + [
             'src-final (the earlier tree: `src/recovered`, `src/shared` and the catalog manifests/original_target/, '
             'frozen on {}): its full table and commit history, which cannot change, are in '
             '[history/SRC_FINAL_STATUS.md](history/SRC_FINAL_STATUS.md).'.format(FROZEN_ON), ''] + source_lines()
    lines.append(END)
    return '\n'.join(lines)


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('--config', type=Path, default=REPOSITORY / 'private/original-target.local.json')
    parser.add_argument('--text-bytes', type=int, help='override instead of reading the executable')
    parser.add_argument('--write', action='store_true', help='replace the generated block in docs/STATUS.md')
    args = parser.parse_args()
    # src-final's catalog (frozen evidence): the reference column; each tree's library ledger, the shared assembly ledger.
    catalog = catalog_io.load(catalog_io.locate(REPOSITORY), references=False)
    text = args.text_bytes or text_bytes(json.loads(args.config.read_bytes())['executable'])
    block = render(catalog, text, lib=load_library(), asm=load_assembly(), main_lib=load_library(frozen=True))
    if not args.write:
        print(block); return
    path = REPOSITORY / 'docs/STATUS.md'
    current = path.read_text()
    if BEGIN not in current or END not in current:
        sys.exit('docs/STATUS.md has no generated markers')
    head, rest = current.split(BEGIN, 1)
    path.write_text(head + block + rest.split(END, 1)[1])
    print('updated', path)
    # The README carries no figures (user, 2026-11-25): the split by source is printed in STATUS only.


if __name__ == '__main__':
    main()
