"""progress.py (written only at a gated landing on rebuild-trial; worker branches do not run it): append one line to manifests/progress.csv from the rebuild catalog (rows the
whole-file build accepts) and provenance.csv (class a/b/c). Bytes are main's frozen extents (body
and padding). Lane-hours count from the rebuild's start, 2026-11-17 18:17Z (lane R)."""
import csv, json, time, datetime
import rebuild as R, catalog_io
START = datetime.datetime(2026, 11, 17, 18, 17, tzinfo=datetime.timezone.utc)
TEXT = 3769805                       # .text bytes (docs/STATUS.md denominator)
LIBRARY = 89127                      # linked library bytes the build reproduces (receipt)
# Assembled handwritten asm, read from the rebuild's ledger (manifests/assembly.json, a copy of main's with
# asm paths since the 2026-11-24 switch; the same procedures and bytes):
# ASSEMBLY the archive text adopted unchanged (16,999 B), REVISED the revised, binary-derived lines (user decision
# 2026-11-24, lane O73: six ra2-decomp src/ra2asm procedures, 1,553 B), on its own column; both in reproduced.
_asm_rows = json.load(open(R.ROOT / 'manifests/assembly.json'))['regions']
REVISED = sum(r['bytes'] for r in _asm_rows if r.get('revised'))
ASSEMBLY = sum(r['bytes'] for r in _asm_rows) - REVISED
cat = catalog_io.load(R.MANIFEST / 'catalog')


def _fuzzy():
    import bytematch_report
    value = bytematch_report.fuzzy_figure(R.ROOT)
    return '' if value is None else '%.3f%%' % value


prov = {r['entry']: r['class'] for r in csv.DictReader(open(R.MANIFEST / 'provenance.csv'))}
split = {k: [0, 0] for k in 'abc'}
workers_path = R.MANIFEST / 'workers.csv'
worker_units = {r['unit']: r['worker'] for r in csv.DictReader(open(workers_path))} if workers_path.exists() else {}
# Worker rows are counted per row (owner, 2026-11-19): a unit listed in worker_rows.csv (a unit a lane
# landed, into which a worker later delivered functions) counts only its listed rows as the worker's; a unit
# only in workers.csv (the worker delivered the unit's source) counts every row.
rows_path = R.MANIFEST / 'worker_rows.csv'
worker_rows = {}
for r in (csv.DictReader(open(rows_path)) if rows_path.exists() else []):
    worker_rows.setdefault(r['unit'], {})[r['entry']] = r['worker']
def worker_of(unit, entry):
    return worker_rows[unit].get(entry) if unit in worker_rows else worker_units.get(unit)
workers = [0, 0]
for row in cat['regions']:
    if worker_of(row['source'].split('/')[-1].rsplit('.', 1)[0], '%08x' % row['entry']):
        workers[0] += 1; workers[1] += row['full_extent_bytes']; continue      # reported apart
    cls = prov.get('%08x' % row['entry'], 'a')
    split[cls][0] += 1; split[cls][1] += row['full_extent_bytes']
units = len({r['source'] for r in cat['regions']})
now = datetime.datetime.now(datetime.timezone.utc)
total_b = sum(v[1] for v in split.values())    # this lane's rows; worker rows are in workers_*
all_b = total_b + workers[1]                   # every catalogued row (as functions_exact counts them)
# Evidenced inline assembly is its own ledger category, not C/C++ credit (AGENTS.md, user 2026-11-07; reached
# through a unit's file-static helper too, review session and user 2026-11-20): its body bytes leave the
# source share and stay in the reproduced share. Series break at the first row with the asm columns: before
# it the source share also held the evidenced rows' bytes (conquer Main_Game, lcw LCW_Comp, 1,187 B).
asm_b = sum(r['body_bytes'] for r in cat['regions'] if r.get('inline_asm') == 'evidenced')
# C/C++ is counted in body bytes, padding on its own line, as main's "Matched C/C++ body bytes" (lane O41,
# 2026-11-22; series break marked on the first row with c_body_bytes): before it source_share_of_text and the
# function-level and clean source shares held the accepted rows' frozen padding too (140,377 B at 21:23Z).
# Reproduced is unchanged: body + padding + linked library + assembled handwritten asm, main's sum.
body_b = sum(r['body_bytes'] for r in cat['regions'])
pad_b = sum(r['padding_bytes'] for r in cat['regions'])
assert body_b + pad_b == all_b
line = dict(time_utc=now.strftime('%Y-%m-%dT%H:%MZ'), units=units, functions_exact=len(cat['regions']),
            bytes_exact=all_b, a_functions=split['a'][0], a_bytes=split['a'][1], b_functions=split['b'][0],
            b_bytes=split['b'][1], c_functions=split['c'][0], c_bytes=split['c'][1],
            workers_functions=workers[0], workers_bytes=workers[1],
            lane_hours='%.2f' % ((now - START).total_seconds() / 3600),
            source_share_of_text='%.3f%%' % (100.0 * (body_b - asm_b) / TEXT),
            reproduced_text='%.3f%%' % (100.0 * (all_b + LIBRARY + ASSEMBLY + REVISED) / TEXT),
            # The categories of Reproduced, each on its own (main's STATUS lines): C/C++ body, evidenced inline
            # asm (inline_asm_bytes below), frozen padding, linked library members, assembled handwritten asm.
            c_body_bytes=body_b - asm_b, padding_bytes=pad_b, library_bytes=LIBRARY, assembly_bytes=ASSEMBLY,
            revised_assembly_bytes=REVISED)
# Structure beside the percent (user, 2026-11-17): the census gate's measures, the units still held
# (pending), and the functions not exact in any unit with their bytes (1 KB and over apart: the trend).
import rebuild_census
m = rebuild_census.main(quiet=True)
units_info = R.load_json(R.UNITS, {})
landed = {r['source'] for r in cat['regions']}
import cleanup_census as C
ext_landed = sum(len(C.extern_names(C.code_only((R.ROOT / i['source']).read_text(errors='replace'))))
                 for i in units_info.values() if i['source'] in landed)
# Parked: rebuild_census.parked_tail, the one definition the census prints (owner, 2026-11-18).
parked_f, parked_b, large_f, large_b = rebuild_census.parked_tail()
# External functions the audit would omit as kept copies (review ruling 2026-11-18; budgeted, lane R7).
import rebuild_catalog
from coff import Coff
ext_omitted = 0
for u, i in units_info.items():
    last = R.load_json(R.WORK / 'last' / (u + '.json'), None)
    if not last or i['source'] not in landed:
        continue
    exact_names = {r['symbol'] for r in last['results'] if r['status'] == 'exact'}
    exact_entries = {int(r['entry'], 16) for r in last['results'] if r['status'] == 'exact'}
    resolve = R.resolver(u, i)
    ext_omitted += sum(resolve(n)[0] not in exact_entries for n in rebuild_catalog.external_omitted(
        Coff(open(last['object'], 'rb').read()), exact_names, resolve))
line.update(external_omitted=ext_omitted)
line.update(duplicates=m['duplicates'], externs_landed=ext_landed, externs_all=m['externs'],
            address_suffixed=m['address_suffixed'], parked_units=sum(i['source'] not in landed for i in units_info.values()),
            parked_functions=parked_f, parked_bytes=parked_b, parked_large_functions=large_f, parked_large_bytes=large_b,
            judgment_rows=m['judgment_rows'], judgment_bytes=m['judgment_bytes'],
            unplaced_symbols=m['unplaced_symbols'], unplaced_bytes=m['unplaced_bytes'],
            content_symbols=m['content_symbols'], content_bytes=m['content_bytes'],
            inline_asm_rows=m['inline_asm_rows'], inline_asm_bytes=m['inline_asm_bytes'],
            helper_asm_rows=m['helper_asm_rows'], helper_asm_bytes=m['helper_asm_bytes'],
            asm_unflagged_rows=m['asm_unflagged_rows'], asm_unflagged_bytes=m['asm_unflagged_bytes'],
            count_sensitive_rows=m['count_sensitive_rows'], count_sensitive_bytes=m['count_sensitive_bytes'],
            # Function-level beside the whole-unit count (review session, 2026-11-20; a separate column, no credit
            # change): each function checked on its own, as ra2-decomp reports (rebuild_census.function_level).
            function_level_functions=m['function_level_rows'], function_level_bytes=m['function_level_bytes'],
            function_level_share_of_text='%.3f%%' % (100.0 * m['function_level_bytes'] / TEXT),
            # The rebuild's headline (owner, 2026-11-21): function-level from source (evidenced inline asm left out,
            # as in source_share_of_text) and reproduced (with the linked library and assembled bytes), beside the
            # whole-unit source_share_of_text / reproduced_text; the whole-file build stays the gate.
            function_level_source_share_of_text='%.3f%%' % (100.0 * (m['function_level_body_bytes'] - asm_b) / TEXT),
            function_level_c_body_bytes=m['function_level_body_bytes'] - asm_b,
            function_level_padding_bytes=m['function_level_bytes'] - m['function_level_body_bytes'],
            function_level_reproduced_text='%.3f%%' % (100.0 * (m['function_level_bytes'] + LIBRARY + ASSEMBLY + REVISED)
                                                     / TEXT),
            # Exact with no judgment call (user-approved process change, 2026-11-22, lane O37): the two headline
            # figures from source with every flagged row left out (judgment notes in provenance.csv, rows listed as
            # dependents of a flagged header form in header_judgments.csv, and count-sensitive rows), bytes body
            # plus padding, source shares C/C++ body only (lane O41); judgment_any_* are the flagged rows of the function-level set (body bytes), of which
            # header_dependent_* come from header forms only (rebuild_census.clean_exact).
            clean_functions=m['clean_whole_rows'], clean_bytes=m['clean_whole_bytes'],
            clean_source_share_of_text='%.3f%%' % (100.0 * m['clean_whole_source_bytes'] / TEXT),
            clean_c_body_bytes=m['clean_whole_source_bytes'],
            clean_function_level_functions=m['clean_function_rows'], clean_function_level_bytes=m['clean_function_bytes'],
            clean_function_level_source_share_of_text='%.3f%%' % (100.0 * m['clean_function_source_bytes'] / TEXT),
            clean_function_level_c_body_bytes=m['clean_function_source_bytes'],
            judgment_any_rows=m['judgment_any_rows'], judgment_any_bytes=m['judgment_any_bytes'],
            header_dependent_rows=m['header_dependent_rows'], header_dependent_bytes=m['header_dependent_bytes'],
            # Judgment by kind and exact with no soft judgment (review session request, 2026-11-23, lane O63): the
            # function-level judgment rows per kind ('kind rows/body bytes; ...', kinds in rebuild_census.py), and the
            # headline sets with only the soft kinds (iii, iv-b, vii, header-form dependents of a soft form) and the
            # count-sensitive rows left out (rebuild_census.clean_measures).
            judgment_by_kind=rebuild_census.format_kinds(m['judgment_kinds']),
            soft_judgment_rows=m['soft_judgment_rows'], soft_judgment_bytes=m['soft_judgment_bytes'],
            soft_free_functions=m['soft_free_whole_rows'], soft_free_c_body_bytes=m['soft_free_whole_source_bytes'],
            soft_free_source_share_of_text='%.3f%%' % (100.0 * m['soft_free_whole_source_bytes'] / TEXT),
            soft_free_function_level_functions=m['soft_free_function_rows'],
            soft_free_function_level_c_body_bytes=m['soft_free_function_source_bytes'],
            soft_free_function_level_source_share_of_text='%.3f%%' % (100.0 * m['soft_free_function_source_bytes'] / TEXT),
            # Fuzzy, best draft scores (lane O90, user 2026-11-25): ByteMatch's whole-project fuzzy_match_percent, every
            # in-scope function not exact at its best measured draft score (bytematch_report.py), its own series.
            fuzzy_best_draft=_fuzzy(),
            tree='rebuild-trial')
# Data reproduced from source (user, 2026-11-25, lane O89): its own series, two new columns, no break in the others;
# a report beside the code figure, never acceptance or credit (data_measure.py, docs/DATA_AS_SOURCE.md). Read from
# the gated build's receipt; inserted before `tree`, so the columns read with the measures.
import data_measure
tail = {k: line.pop(k) for k in ('tree',)}
line.update(data_measure.progress_columns(R.ROOT), **tail)
path = R.MANIFEST / 'progress.csv'
rows = list(csv.DictReader(open(path))) if path.exists() else []
# Series breaks are marked on the row where they start (the 2026-11-18T16:07Z one is in the README).
line['note'] = '' if any(r.get('inline_asm_rows') for r in rows) else (
    'series break: source_share_of_text leaves out evidenced inline asm (conquer, lcw: 1,187 B were in it before)')
if not any(r.get('c_body_bytes') for r in rows):
    line['note'] = ('series break: source_share_of_text, function_level_source_share_of_text and the clean source '
                    'shares count C/C++ body bytes only (padding in padding_bytes, as on main); rows before this held '
                    'the accepted rows\' padding too')
# Lane O72 (2026-11-24): status.py HANDWRITTEN gains the cell range; marked once, on the first row after it.
if not any('Draw_Shroud_Or_Fog_Shape' in r.get('note', '') for r in rows):
    line['note'] = '; '.join(n for n in (line['note'], (
        'series break: status.py HANDWRITTEN gains CellClass::Draw_Shroud_Or_Fog_Shape 0x474260-0x474503 (675 B, '
        'the author\'s __asm block, lane O72): the in-scope denominator (main STATUS) and both trees\' ByteMatch '
        'scope drop by 675 B; the columns of this file are shares of all .text and do not move')) if n)
# Lane O73 (2026-11-24, user decision): the revised, binary-derived assembly category; marked once.
if not any(r.get('revised_assembly_bytes') for r in rows):
    line['note'] = '; '.join(n for n in (line['note'], (
        'series break: Reproduced .text gains the revised, binary-derived assembly category (user decision 2026-11-24, '
        'lane O73): six ra2-decomp src/ra2asm procedures, %s B, in revised_assembly_bytes and in reproduced_text and '
        'function_level_reproduced_text (both trees); assembly_bytes stays the archive text; the C/C++ columns do not '
        'move' % format(REVISED, ','))) if n)
# Lane O90 (2026-11-25): the fuzzy (best draft scores) column starts; a new series, no other column moves.
if not any(r.get('fuzzy_best_draft') for r in rows):
    line['note'] = '; '.join(n for n in (line['note'], (
        'new series fuzzy_best_draft (lane O90): ByteMatch\'s fuzzy figure, each in-scope function not exact at its '
        'best measured draft score against its image-derived bound; empty on earlier rows; no other column moves')) if n)
rows.append({k: str(v) for k, v in line.items()})
with open(path, 'w', newline='') as f:
    w = csv.DictWriter(f, fieldnames=list(line), restval=''); w.writeheader(); w.writerows(rows)
print(line)
