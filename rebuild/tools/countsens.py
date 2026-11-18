"""countsens.py [UNIT ...] [--write]: try every row parked as "exact form, count-sensitive" again (review
ruling 2026-11-18, lane R20). Such a row's text is exact under one header state and flips with the
declaration count before it alone; it is guarded in its unit's source with the label
"exact form, count-sensitive" and listed in the unit's `count_sensitive` in units.json
(entry -> {windows, header, note}).

For each listed row (all units, or those named): the guard is taken off and the unit checked. When the row
is exact again and no other row of the unit is lost, the guard stays off with --write and the entry leaves
`count_sensitive`; otherwise the guard goes back. Without --write every trial is undone and only reported.
Run it at a landing whenever a header changes (the declaration count before the row may have moved back
into an exact window); it never edits a header."""
import hashlib
import json
import re
import subprocess
import sys

import rebuild as R
from guard import body_end, demangle

LABEL = '#ifdef RA2_NON_MATCHING // exact form, count-sensitive'


def check(unit, src):
    subprocess.run([sys.executable, '-B', str(R.REBUILD / 'tools/rebuild.py'), 'check', unit, '--learn'],
                   capture_output=True)
    last = json.load(open(R.WORK / 'last' / (unit + '.json')))
    if last['source_sha256'] != hashlib.sha256(src.read_bytes()).hexdigest():
        return None    # compile failed: the last check is of an older text
    return {r['entry'] for r in last['results'] if r['status'] == 'exact'}


def guard_span(text, name):
    """(start, end, body start, body end) of the count-sensitive guard around the definition of name."""
    short = name.split('::')[-1]
    for m in re.finditer(re.escape(LABEL) + r'[^\n]*\n', text):
        head = text[m.end():text.find('{', m.end())]
        if not re.search(r'(?<![\w:])%s\s*\(' % re.escape(name if '::' in head else short), head):
            continue
        close = body_end(text, text.find('{', m.end()))
        nl = text.find('\n', close)
        if close > 0 and text.startswith('\n#endif', nl):
            return m.start(), nl + len('\n#endif'), m.end(), nl
    return None


def main(argv):
    write = '--write' in argv
    names = [a for a in argv if not a.startswith('-')]
    units = R.load_json(R.UNITS, {})
    changed = False
    for unit, info in units.items():
        if names and unit not in names or not info.get('count_sensitive'):
            continue
        src = R.ROOT / info['source']
        symbols = {r['entry']: r['symbol'] for r in info['rows']}
        for entry in list(info['count_sensitive']):
            base = check(unit, src)
            text = src.read_text(encoding='latin-1')
            span = guard_span(text, demangle(symbols[entry]) or '')
            if base is None or span is None:
                print('%s %s: %s' % (unit, entry, 'does not compile' if base is None else 'guard not found'))
                continue
            s, e, bs, be = span
            src.write_text(text[:s] + text[bs:be] + text[e:], encoding='latin-1')
            now = check(unit, src)
            if now is not None and entry in now and base <= now:
                print('%s %s: exact again (%d rows)' % (unit, entry, len(now)))
                if write:
                    del info['count_sensitive'][entry]
                    changed = True
                    continue
            else:
                print('%s %s: still flipped' % (unit, entry))
            src.write_text(text, encoding='latin-1')
            check(unit, src)
        if write and not info['count_sensitive']:
            del info['count_sensitive']
    if changed:
        R.save_units(units, [u for u in units if not names or u in names])


if __name__ == '__main__':
    main(sys.argv[1:])
