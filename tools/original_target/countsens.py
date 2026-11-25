"""countsens.py [UNIT ...] [--write]: try every row parked as "exact form, count-sensitive" again (review
ruling 2026-11-18, lane R20). Such a row's text is exact under one header state and flips with the
declaration count before it alone; it is guarded in its unit's source with the label
"exact form, count-sensitive" and listed in the unit's `count_sensitive` in units.json
(entry -> {windows, header, note}).

For each listed row (all units, or those named): the guard is taken off and the unit checked. When the row
is exact again and no other row of the unit is lost, the guard stays off with --write and the entry leaves
`count_sensitive`; otherwise the guard goes back. Without --write every trial is undone and only reported.
Run it at a landing whenever a header changes (the declaration count before the row may have moved back
into an exact window); it never edits a header. Unguarding a function can renumber the compiler's `_$E`
initializer names after it, so each trial first renames the unit's initializer rows by their evidence
(xcu.py: the .CRT$XCU table, then the calls and atexit registrations it makes) and checks again; a refused
trial restores units.json as it was.

Rows that are exact but sit at the edge of a declaration-count window are listed in the unit's
`count_watch` (entry -> {windows, note}); each run checks those units and reports each watched row as
exact or FLIPPED (then park it: guard it with the count-sensitive label and move its entry to
`count_sensitive`)."""
import hashlib
import json
import re
import subprocess
import sys

import rebuild as R
from guard import body_end, demangle

LABEL = '#ifdef RA2_NON_MATCHING // exact form, count-sensitive'


def rebind(unit, src):
    """Check, rename the unit's initializer rows by their evidence, and check again."""
    check(unit, src)
    subprocess.run([sys.executable, '-B', str(R.ROOT / 'tools/original_target/xcu.py'), unit], capture_output=True,
                   cwd=str(R.ROOT / 'tools/original_target'))
    return check(unit, src)


def check(unit, src):
    subprocess.run([sys.executable, '-B', str(R.ROOT / 'tools/original_target/rebuild.py'), 'check', unit, '--learn'],
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
    for unit, info in R.load_json(R.UNITS, {}).items():
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
            saved = R.UNITS.read_bytes()
            s, e, bs, be = span
            src.write_text(text[:s] + text[bs:be] + text[e:], encoding='latin-1')
            now = rebind(unit, src)
            if now is not None and entry in now and base <= now:
                print('%s %s: exact again (%d rows)' % (unit, entry, len(now)))
                if write:
                    units = R.load_json(R.UNITS, {})
                    del units[unit]['count_sensitive'][entry]
                    if not units[unit]['count_sensitive']:
                        del units[unit]['count_sensitive']
                    R.save_units(units, [unit])
                    continue
            else:
                print('%s %s: still flipped' % (unit, entry))
            src.write_text(text, encoding='latin-1')
            R.UNITS.write_bytes(saved)
            check(unit, src)

    for unit, info in R.load_json(R.UNITS, {}).items():
        if names and unit not in names or not info.get('count_watch'):
            continue
        now = check(unit, R.ROOT / info['source'])
        for entry in info['count_watch']:
            state = 'does not compile' if now is None else 'exact' if entry in now else \
                'FLIPPED: park it count-sensitive (guard it, move the entry to count_sensitive)'
            print('%s %s: watched, %s' % (unit, entry, state))


if __name__ == '__main__':
    main(sys.argv[1:])
