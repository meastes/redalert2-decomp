"""progress.py: append one line to rebuild/manifest/progress.csv from the rebuild catalog (rows the
whole-file build accepts) and provenance.csv (class a/b/c). Bytes are main's frozen extents (body
and padding). Lane-hours count from the trial's start, 2026-11-17 18:17Z (lane R)."""
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
workers = [0, 0]
for row in cat['regions']:
    if row['source'].split('/')[-1].rsplit('.', 1)[0] in worker_units:
        workers[0] += 1; workers[1] += row['full_extent_bytes']; continue      # reported apart
    cls = prov.get('%08x' % row['entry'], 'a')
    split[cls][0] += 1; split[cls][1] += row['full_extent_bytes']
units = len({r['source'] for r in cat['regions']})
now = datetime.datetime.now(datetime.timezone.utc)
total_b = sum(v[1] for v in split.values())    # this lane's rows; worker rows are in workers_*
line = dict(time_utc=now.strftime('%Y-%m-%dT%H:%MZ'), units=units, functions_exact=len(cat['regions']),
            bytes_exact=total_b, a_functions=split['a'][0], a_bytes=split['a'][1], b_functions=split['b'][0],
            b_bytes=split['b'][1], c_functions=split['c'][0], c_bytes=split['c'][1],
            workers_functions=workers[0], workers_bytes=workers[1],
            lane_hours='%.2f' % ((now - START).total_seconds() / 3600),
            source_share_of_text='%.3f%%' % (100.0 * (total_b + workers[1]) / TEXT),
            reproduced_text='%.3f%%' % (100.0 * (total_b + workers[1] + LIBRARY + ASSEMBLY) / TEXT))
# Structure beside the percent (user, 2026-11-17): the census gate's measures, the units still held
# (pending), and the functions not exact in any unit with their bytes (1 KB and over apart: the trend).
import rebuild_census
m = rebuild_census.main(quiet=True)
units_info = R.load_json(R.UNITS, {})
landed = {r['source'] for r in cat['regions']}
import cleanup_census as C
ext_landed = sum(len(C.extern_names(C.code_only((R.ROOT / i['source']).read_text(errors='replace'))))
                 for i in units_info.values() if i['source'] in landed)
parked_f = parked_b = large_b = large_f = 0
for u, i in units_info.items():
    last = R.load_json(R.WORK / 'last' / (u + '.json'), None)
    res = {r['entry']: r['status'] for r in last['results']} if last else {}
    for r in i['rows']:
        if res.get(r['entry']) != 'exact':
            parked_f += 1; parked_b += r['body']
            if r['body'] >= 1024: large_f += 1; large_b += r['body']
line.update(duplicates=m['duplicates'], externs_landed=ext_landed, externs_all=m['externs'],
            address_suffixed=m['address_suffixed'], parked_units=sum(i['source'] not in landed for i in units_info.values()),
            parked_functions=parked_f, parked_bytes=parked_b, parked_large_functions=large_f, parked_large_bytes=large_b)
path = R.MANIFEST / 'progress.csv'
rows = list(csv.DictReader(open(path))) if path.exists() else []
rows.append({k: str(v) for k, v in line.items()})
with open(path, 'w', newline='') as f:
    w = csv.DictWriter(f, fieldnames=list(line), restval=''); w.writeheader(); w.writerows(rows)
print(line)
