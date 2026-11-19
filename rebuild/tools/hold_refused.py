"""hold_refused.py BUILD_LOG: mark every unit the whole-file build refused (its source named in the
log's refusal lines) as build_refused for its current text, with the first reason (lane R2)."""
import json, re, sys
import rebuild as R
log = open(sys.argv[1]).read()
units = R.load_json(R.UNITS, {})
by_source = {i['source']: u for u, i in units.items()}
seen = {}
for line in log.splitlines():
    m = re.search(r'source (rebuild/src/[\w]+\.cpp)', line)
    if m and m.group(1) in by_source and m.group(1) not in seen:
        seen[m.group(1)] = re.sub(r'(/[^ /]+)+/', '', line.split(' [row')[0])[:200]    # no local paths
for src, reason in seen.items():
    u = by_source[src]
    last = R.load_json(R.WORK / 'last' / (u + '.json'), {})
    units[u]['build_refused'] = dict(source_sha256=last.get('source_sha256'), reason=reason)
    print('%s: %s' % (u, reason[:120]))
R.write_json(R.UNITS, units)
