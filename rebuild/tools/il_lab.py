"""il_lab.py UNIT SPEC.json [XLO XHI STEP] [--source PATH]: the /Ob2 IL size of callees, measured inside the
unit's own compile environment (lane FT1, the Fable trial, 2026-11-20).

The inline rule (VC6_MATCHING_NOTES "/Ob2 inline decision rule") turns on the IL size s of each callee, which
depends on the unit's headers and spellings; this reads s for any callee of a unit without a separate lab.
A scratch copy of the unit's text (never the tree's file; nothing in rebuild/manifest is written) gets, for
each callee statement f of the spec and each x in XLO..XHI by STEP,

    void Lab_<name>_<x>() { Lab_Spend_<x>(); f }

where Lab_Spend_<x> is an inline function of size 1000 - x (constant stores: 12 for the empty body, 6 per
array-element store, 4 per scalar store, so every even size). The root is small, so its budget is the floor
1000; the spend is charged first and f is decided with x left: f expands iff s(f) <= x (or s(f) <= 40, free).
The first x whose call list changes is s(f) to STEP units. Past it, the list keeps changing as f's own nested
sites are decided with (x - s(f)) // 1: the further transitions are the nested thresholds of f's body, in
budget units of that body, which is how to read which nested site flips first and what a body's N is.

Each printed line is 'x=<first x of a state>: <the root's direct calls in that state>'. A root whose last
statement is refused may show no call (a tail jump is not a call): read "(none)" at the low end as refused.

SPEC.json:
  "f":     {"name": "statement;", ...}   the callee statements, in the unit's own names
  "decls": text placed before the roots (pointers and keys the statements use)
  "pre":   text placed before the whole unit (for example "#define private public\n" to reach private members;
           it changes access letters in decorated names, which the lab does not bind, and no layout)
  "open":  substrings; a RA2_NON_MATCHING guard whose first 600 characters hold one is opened

Environment: ROOTPAD=<n> appends n out-of-line `GetFocus();` statements to every root (raises the root budget
past the 1000 floor, 8 per statement, to reach thresholds above 1000); LABSHORT=1 prints short callee names.

Measured with it (ownrdraw, 2026-11-20): Dictionary<unsigned, WinData>::keyHash 48, the node's ??_G 50,
~WinData 50, ??_GWideWstring 50, getPointer 132, WinData::operator= 132, Set_Wide_Text 116, Remove_Node 140,
shrink 248, remove(key, value) 378, remove(key) 62, Restore_Child 106, Copy_List about 280."""
import json
import os
import re
import sys
from pathlib import Path

import rebuild as R
from coff import Coff
from tailmap import guards, open_text, compile_text


def spend(x):
    """An inline function of IL size 1000 - x."""
    n = (1000 - x - 12) // 2
    j = {0: 0, 1: 2, 2: 1}[n % 3]
    k = (n - 2 * j) // 3
    if k < 0 or 3 * k + 2 * j != n:
        sys.exit('no spend of size %d (x=%d): use an even x of at most 984' % (1000 - x, x))
    body = ''.join('Lab_Arr[%d]=0;' % i for i in range(k)) + ''.join('Lab_S%d=0;' % i for i in range(j))
    return 'inline void Lab_Spend_%d() {%s}\n' % (x, body)


def direct_calls(coff, symbol):
    sym, section, start, end = coff.function(symbol)
    data = section['data']
    return [coff.symbols[index]['name'] for offset, index, kind in sorted(section['relocs'])
            if kind == 20 and start <= offset < end and data[offset - 1] == 0xE8]


def main():
    args = [a for a in sys.argv[1:] if not a.startswith('--')]
    source = next((sys.argv[i + 1] for i, a in enumerate(sys.argv) if a == '--source'), None)
    if source in args:
        args.remove(source)
    unit, spec = args[0], json.load(open(args[1]))
    xlo, xhi, step = (int(v) for v in args[2:5]) if len(args) > 4 else (42, 402, 8)
    info = R.load_json(R.UNITS, {})[unit]
    text = Path(source or R.ROOT / info['source']).read_text(encoding='latin-1')
    text = open_text(text, {s for s, e, label in guards(text) if any(o in text[s:s + 600] for o in spec.get('open', []))})
    xs = list(range(xlo, xhi + 1, step))
    pad = 'GetFocus();' * int(os.environ.get('ROOTPAD', '0'))
    out = ['\n// ---- il_lab ----\n', spec.get('decls', ''), '\nint Lab_Arr[400]; int Lab_S0, Lab_S1;\n']
    out += [spend(x) for x in xs]
    for name, stmt in spec['f'].items():
        out += ['void Lab_%s_%d() { Lab_Spend_%d(); %s %s }\n' % (name, x, x, stmt, pad) for x in xs]
    data, log = compile_text(unit, info, spec.get('pre', '') + text + ''.join(out), 'il_lab')
    if data is None:
        print('\n'.join(line for line in log.splitlines() if 'error' in line)[:3000])
        sys.exit(2)
    coff = Coff(data)
    functions = {s['name'] for s in coff._functions}
    short = (lambda n: n.split('@')[0].lstrip('?')[:14] + ('' if '@' not in n else '@' + n.split('@')[1][:10])) \
        if os.environ.get('LABSHORT') else (lambda n: n[:60])
    for name in spec['f']:
        previous, lines = None, []
        for x in xs:
            symbol = next((n for n in functions if re.match(r'\?Lab_%s_%d@' % (re.escape(name), x), n)), None)
            if symbol is None:
                lines.append('x=%d: (root not emitted)' % x)
                continue
            calls = tuple(short(n) for n in direct_calls(coff, symbol))
            if calls != previous:
                lines.append('x=%d: %s' % (x, ' '.join(calls) or '(none)'))
                previous = calls
        print('%-8s %s' % (name, '\n         '.join(lines)))


if __name__ == '__main__':
    main()
