"""assess.py: the rebuild's measures by provenance class, from manifests/provenance.csv (rows the
catalog accepts), timelog.csv (lane time per class) and the catalog. Bytes are main's frozen extents
(body and padding) (lane R)."""
import csv, datetime
from collections import defaultdict
import rebuild as R
import catalog_io

cat = catalog_io.load(R.MANIFEST / 'catalog')
extent = {'%08x' % r['entry']: r['full_extent_bytes'] for r in cat['regions']}
prov = [r for r in csv.DictReader(open(R.MANIFEST / 'provenance.csv')) if r['entry'] in extent]
by = defaultdict(lambda: dict(functions=0, bytes=0, yes=0, edit=0, rematch=0, units=set()))
for r in prov:
    b = by[r['class']]
    b['functions'] += 1; b['bytes'] += extent[r['entry']]; b[r['first_try']] += 1; b['units'].add(r['unit'])

minutes = defaultdict(float)
now = datetime.datetime.utcnow().strftime('%H:%M')
for row in csv.DictReader(open(R.MANIFEST / 'timelog.csv')):
    s, e = row['start_utc'], row['end_utc'] or now
    m = (int(e[:2]) * 60 + int(e[3:])) - (int(s[:2]) * 60 + int(s[3:]))
    minutes[row['class']] += max(m, 0)

print('class  units  functions  bytes   first-try  small-edit  rematch  lane-min  bytes/lane-h  functions/lane-h')
for k in ('a', 'b', 'c'):
    b = by[k]; m = minutes.get(k, 0) or 1
    print('%-5s  %5d  %9d  %7d  %9d  %10d  %7d  %8.0f  %12.0f  %16.1f' % (
        k, len(b['units']), b['functions'], b['bytes'], b['yes'], b['edit'], b['rematch'], minutes.get(k, 0),
        b['bytes'] * 60.0 / m, b['functions'] * 60.0 / m))
print('tooling/support minutes: %.0f' % (minutes.get('tooling', 0) + minutes.get('support', 0)))
