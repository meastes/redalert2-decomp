"""progress.py (written only at a gated landing on rebuild-trial; worker branches do not run it): append one line to rebuild/manifest/progress.csv from the rebuild catalog (rows the
whole-file build accepts) and provenance.csv (class a/b/c). Bytes are main's frozen extents (body
and padding). Lane-hours count from the rebuild's start, 2026-11-17 18:17Z (lane R)."""
import csv, json, time, datetime
import rebuild as R, catalog_io
START = datetime.datetime(2026, 11, 17, 18, 17, tzinfo=datetime.timezone.utc)
TEXT = 3769805                       # .text bytes (docs/STATUS.md denominator)
LIBRARY, ASSEMBLY = 89127, 16999     # linked library and assembled bytes the build reproduces (receipt)
cat = catalog_io.load(R.MANIFEST / 'catalog')
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
line = dict(time_utc=now.strftime('%Y-%m-%dT%H:%MZ'), units=units, functions_exact=len(cat['regions']),
            bytes_exact=all_b, a_functions=split['a'][0], a_bytes=split['a'][1], b_functions=split['b'][0],
            b_bytes=split['b'][1], c_functions=split['c'][0], c_bytes=split['c'][1],
            workers_functions=workers[0], workers_bytes=workers[1],
            lane_hours='%.2f' % ((now - START).total_seconds() / 3600),
            source_share_of_text='%.3f%%' % (100.0 * all_b / TEXT),
            reproduced_text='%.3f%%' % (100.0 * (all_b + LIBRARY + ASSEMBLY) / TEXT))
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
            count_sensitive_rows=m['count_sensitive_rows'], count_sensitive_bytes=m['count_sensitive_bytes'],
            # Function-level beside the whole-unit count (review session, 2026-11-20; a separate column, no credit
            # change): each function checked on its own, as ra2-decomp reports (rebuild_census.function_level).
            function_level_functions=m['function_level_rows'], function_level_bytes=m['function_level_bytes'],
            function_level_share_of_text='%.3f%%' % (100.0 * m['function_level_bytes'] / TEXT),
            tree='rebuild-trial')
path = R.MANIFEST / 'progress.csv'
rows = list(csv.DictReader(open(path))) if path.exists() else []
rows.append({k: str(v) for k, v in line.items()})
with open(path, 'w', newline='') as f:
    w = csv.DictWriter(f, fieldnames=list(line), restval=''); w.writeheader(); w.writerows(rows)
print(line)
