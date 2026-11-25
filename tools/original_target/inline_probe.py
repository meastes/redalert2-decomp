"""inline_probe.py UNIT ENTRY [--symbol NAME] [--callee NAME] [--pad K --stmt TEXT]: where VC6's /Ob2 inline
budget runs out in one function (lane R13).

Without --pad: compiles src/UNIT.cpp once and lists the direct calls of the row's function, in address
order, in our object and in the original, aligned by callee. A callee the original calls where our object has no
call was expanded by us and not by the original (the original's budget ran out first); the reverse means the
original expanded a site that we call.

--source PATH probes a scratch text of the unit (a hypothesis: another function defined, a form changed).

With --pad K --stmt TEXT: compiles K+1 scratch copies of the unit (never the tree's file) with TEXT inserted
0..K times at the top of the function's body, and reports for each k the number of calls left to CALLEE (or to
every callee). The k at which a site flips from expanded to called measures the budget left at that site in
units of TEXT's expansion; when the original calls a site that k=0 expands, the k that first calls it is how
much cheaper our text is than the original's before that site.

The compile is the check's own (options, include path); nothing in manifests is written."""
import argparse
import difflib
import json
import re
import struct
import sys
from pathlib import Path

import rebuild as R
from coff import Coff
from verify_manifest import Audit


def our_calls(coff, symbol, resolve):
    """[(offset, callee name, resolved address or None)] of the REL32 calls in SYMBOL's body."""
    sym, section, start, end = coff.function(symbol)
    data = section['data']
    out = []
    for offset, index, kind in sorted(section['relocs']):
        if kind != 20 or not start <= offset < end or offset < 1 or data[offset - 1] != 0xE8:
            continue
        name = coff.symbols[index]['name']
        if name.startswith('$'):
            continue
        out.append((offset - start, name, resolve(name)[0]))
    return out


def original_calls(audit, entry, body):
    """[(offset, target address)] of the direct calls in the original body."""
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    out = []
    for insn in md.disasm(audit.original(entry, body), entry):
        if insn.mnemonic == 'call' and insn.op_str.startswith('0x'):
            out.append((insn.address - entry, int(insn.op_str, 16)))
    return out


def names_by_address(coff, resolve):
    names = {}
    for s in coff.symbols.values():
        if s['name'].startswith(('?', '_')):
            address = resolve(s['name'])[0]
            if address is not None:
                names.setdefault(address, s['name'])
    return names


def compile_variant(unit, info, text, tag):
    path = R.WORK / 'inline_probe' / ('%s-%s.cpp' % (unit, tag))
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(text, encoding='latin-1')
    variant = dict(info, source=str(path.relative_to(R.ROOT)))
    obj, log, _, _ = R.compile_unit(unit + '-iprobe', variant, tag)
    if obj is None:
        sys.exit('compile failed (%s):\n%s' % (tag, '\n'.join(l for l in log.splitlines() if 'error' in l)[:2000]))
    return Coff(obj.read_bytes())


def body_start(text, symbol):
    """Index just after the opening brace of SYMBOL's definition in TEXT."""
    import guard
    name = guard.demangle(symbol)
    short = name.split('::')[-1] if name else symbol
    for m in re.finditer(r'^(?![ \t#/])[^\n;{}]*?(?<![\w~])%s\s*\(' % re.escape(short), text, re.M):
        brace = text.find('{', m.start())
        if brace >= 0 and text.find(';', m.start(), brace) < 0:
            return brace + 1
    sys.exit('definition of %s not found' % (name or symbol))


def open_guard(text, symbol):
    """TEXT with the RA2_NON_MATCHING guard around SYMBOL's draft opened (a parked function is probed as
    written)."""
    try:
        at = body_start(text, symbol)
    except SystemExit:
        return text         # a name the definition finder cannot spell (an operator): probed as written
    stack = []              # open conditionals before the body, nested ones (#ifndef X ... #endif) included
    for m in re.finditer(r'^[ \t]*#[ \t]*(if|ifdef|ifndef|endif)\b.*$', text[:at], re.M):
        if m.group(1) == 'endif':
            if stack:
                stack.pop()
        else:
            stack.append(m)
    for m in reversed(stack):
        if re.match(r'[ \t]*#[ \t]*ifdef[ \t]+RA2_NON_MATCHING\b', m.group(0)):
            text = text[:m.start()] + '#if 1' + text[m.end():]
            break
    return text


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('unit'); ap.add_argument('entry')
    ap.add_argument('--symbol'); ap.add_argument('--callee', help='NAME or NAME=ADDRESS')
    ap.add_argument('--pad', type=int, default=0); ap.add_argument('--stmt'); ap.add_argument('--step', type=int, default=1)
    ap.add_argument('--source', help='a scratch text of the unit to probe instead of the tree\'s file')
    a = ap.parse_args()
    info = R.load_json(R.UNITS, {})[a.unit]
    row = next(r for r in info['rows'] if r['entry'] == a.entry.lower().zfill(8))
    symbol = a.symbol or row['symbol']
    entry = int(row['entry'], 16)
    audit = Audit(R.ROOT, json.loads(R.CONFIG.read_text())['executable'])
    resolve = R.resolver(a.unit, info)
    source = open_guard(Path(a.source or R.ROOT / info['source']).read_text(encoding='latin-1'), symbol)
    original = original_calls(audit, entry, row['body'])

    if not a.pad:
        coff = compile_variant(a.unit, info, source, 'k0')
        ours = our_calls(coff, symbol, resolve)
        names = names_by_address(coff, resolve)
        orig_names = [names.get(t, '%08x' % t) for _, t in original]
        our_names = [n for _, n, _ in ours]
        print('%s %s: original %d calls, ours %d' % (a.unit, symbol, len(original), len(ours)))
        sm = difflib.SequenceMatcher(None, orig_names, our_names, autojunk=False)
        for op, i1, i2, j1, j2 in sm.get_opcodes():
            if op == 'equal':
                continue
            if op == 'replace' and i2 - i1 == j2 - j1 and all(our_names[j].startswith('?') and orig_names[i][0] in '0123456789abcdef'
                                                               for i, j in zip(range(i1, i2), range(j1, j2))):
                continue        # names this check has not bound yet, paired by position
            for i in range(i1, i2):
                print('  original only  +%04x  %s' % (original[i][0], orig_names[i]))
            for j in range(j1, j2):
                print('  ours only      +%04x  %s' % (ours[j][0], our_names[j]))
        if sm.ratio() == 1.0:
            print('  same calls in the same order')
        return

    if not a.stmt:
        sys.exit('--pad needs --stmt')
    at = body_start(source, symbol)
    callee_address = None
    if a.callee and '=' in a.callee:
        a.callee, address = a.callee.split('=')
        callee_address = int(address, 16)
    elif a.callee:
        callee_address = resolve(a.callee)[0]
    want = sum(1 for _, t in original if t == callee_address) if a.callee else len(original)
    print('%s %s: the original makes %d call(s) to %s' % (a.unit, symbol, want, a.callee or 'any callee'))
    for k in range(0, a.pad + 1, a.step):
        text = source[:at] + ('\n\t' + a.stmt) * k + source[at:]
        coff = compile_variant(a.unit, info, text, 'k%d' % k)
        ours = our_calls(coff, symbol, resolve)
        hits = [o for o, n, _ in ours if not a.callee or n == a.callee]
        print('  k=%-3d %d call(s)%s' % (k, len(hits), (' at ' + ' '.join('+%04x' % o for o in hits)) if hits else ''))


if __name__ == '__main__':
    main()
