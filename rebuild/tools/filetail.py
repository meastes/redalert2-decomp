"""filetail.py UNIT...: file the main-exact rows right after a unit's range that its object already emits
(lane R23; radsite precedent, rule accepted by the owner 2026-11-18). A row R after unit U's range and
before the next unit's range joins U when (1) main's module map (manifests/original_target/modules)
puts R in U and (2) U's last checked object defines R's exact main symbol. U's range is widened over
the run of rows main files under U (or under no module) that starts at its end, up to the last row U's
object defines: rows in between lie between U's range and U's kept copy (link-order evidence) and join
as rows to write. Run
`rebuild.py check UNIT --learn` afterwards; `--trim` then drops the added rows after the last one that is exact."""
import glob, json, sys
import rebuild as R
from coff import Coff

catalog = R.main_catalog()
ordered = sorted(catalog['regions'], key=lambda r: r['entry'])
mainmod = {}
for p in glob.glob(str(R.ROOT / 'manifests/original_target/modules/*.json')):
    d = json.load(open(p))
    for key in ('code', 'joined', 'residuals'):
        for c in d.get(key) or []:
            if 'entry' in c:
                mainmod.setdefault(int(c['entry'], 16), d['module'])
units = R.load_json(R.UNITS, {})
trim = '--trim' in sys.argv
done = []
for unit in [a for a in sys.argv[1:] if not a.startswith('--')]:
    info = units[unit]
    lo, hi = int(info['range'][0], 16), int(info['range'][1], 16)
    if trim:
        last = json.load(open(R.WORK / 'last' / (unit + '.json')))
        status = {r['entry']: r['status'] for r in last['results']}
        state = json.load(open(R.WORK / ('filetail-%s.json' % unit)))
        added = state['added']
        while added and status.get(added[-1]) != 'exact':      # back to the last exact (kept) copy
            e = added.pop()
            info['rows'] = [r for r in info['rows'] if r['entry'] != e]
        end = max([int(r['entry'], 16) + r['body'] + r['padding'] for r in info['rows'] if r['entry'] in added] or
                  [int(state['from'], 16)])
        info['range'][1] = '%08x' % end
        state['added'] = added
        R.write_json(R.WORK / ('filetail-%s.json' % unit), state)
        print('%s: trimmed, range end %s, %d filed rows' % (unit, info['range'][1], len(added)))
        done.append(unit); continue
    nxt = min([int(i['range'][0], 16) for u, i in units.items() if u != unit and int(i['range'][0], 16) >= hi] or [1 << 32])
    last = json.load(open(R.WORK / 'last' / (unit + '.json')))
    defined = {s['name'] for s in Coff(open(last['object'], 'rb').read()).symbols.values() if s['section'] > 0}
    have = {r['entry'] for r in info['rows']}
    run, end, last_emitted = [], hi, 0
    for r in ordered:
        a = r['entry']
        if a < hi or '%08x' % a in have:
            continue
        if a >= nxt or a != end or mainmod.get(a, unit) != unit:
            break
        run.append(r)
        end = a + r['full_extent_bytes']
        if r['symbol'] in defined:
            last_emitted = len(run)
    added = []
    for r in run[:last_emitted]:
        a = r['entry']
        info['rows'].append(dict(entry='%08x' % a, body=r['body_bytes'], padding=r['padding_bytes'],
                                 symbol=r['symbol'], main_symbol=r['symbol'], main_source=r['source']))
        added.append('%08x' % a)
    end = run[last_emitted - 1]['entry'] + run[last_emitted - 1]['full_extent_bytes'] if last_emitted else hi
    if not added:
        print('%s: nothing to file' % unit); continue
    info['rows'].sort(key=lambda r: r['entry'])
    R.write_json(R.WORK / ('filetail-%s.json' % unit), {'from': info['range'][1], 'added': added})
    info['range'][1] = '%08x' % end
    print('%s: range end %08x -> %08x, %d rows filed' % (unit, hi, end, len(added)))
    done.append(unit)
R.save_units(units, done)
