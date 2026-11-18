"""guard.py UNIT [--dry]: put the unit's functions that are not exact under RA2_NON_MATCHING (the exact
build then keeps the original bytes for them), each labelled with its last score. A function is guarded
when its row's last check differs, or when the object emits it under a name that binds to one of the
unit's rows that is not exact. Overloads the tool cannot tell apart are reported and left (lane R2)."""
import json, re, sys
import rebuild as R
from coff import Coff

OPEN = '#ifdef RA2_NON_MATCHING // not exact: %s\n'


def demangle(sym):
    """'Class::Name' (or 'NS::Class::Name', 'Name') of a simple MSVC symbol, or None."""
    m = re.match(r'\?\?([01])([A-Za-z_]\w*)@((?:[A-Za-z_]\w*@)*)@', sym)
    if m:
        scope = [m.group(2)] + [s for s in m.group(3).split('@') if s]
        cls = '::'.join(reversed(scope))
        return cls + '::' + ('~' if m.group(1) == '1' else '') + m.group(2)
    m = re.match(r'\?([A-Za-z_]\w*)@((?:[A-Za-z_]\w*@)*)@', sym)
    if m:
        scope = [s for s in m.group(2).split('@') if s]
        return '::'.join(list(reversed(scope)) + [m.group(1)])
    m = re.match(r'_([A-Za-z_]\w*)$', sym)
    return m.group(1) if m else None


def body_end(text, i):
    depth = 0
    while i < len(text):
        c = text[i]
        if c == '{': depth += 1
        elif c == '}':
            depth -= 1
            if depth == 0: return i + 1
        elif text.startswith('//', i): i = text.find('\n', i) - 1
        elif text.startswith('/*', i): i = text.find('*/', i) + 1
        elif c in '"\'':
            j = i + 1
            while text[j] != c: j += 2 if text[j] == '\\' else 1
            i = j
        i += 1
    return -1


def namespace_spans(text, scope):
    """(start, end) of every `namespace A { namespace B {` block that opens scope 'A::B' (one level per
    block; nested blocks are matched by their innermost name only when scope has one part)."""
    spans = []
    for m in re.finditer(r'^namespace\s+%s\s*\{' % re.escape(scope.split('::')[-1]), text, re.M):
        end = body_end(text, m.end() - 1)
        if end > 0:
            spans.append((m.end(), end))
    return spans


def main(unit, dry, callers=False):
    units = R.load_json(R.UNITS, {})
    info = units[unit]
    last = json.load(open(R.WORK / 'last' / (unit + '.json')))
    results = {r['entry']: r for r in last['results']}
    coff = Coff(open(last['object'], 'rb').read())
    resolve = R.resolver(unit, info)
    lo, hi = int(info['range'][0], 16), int(info['range'][1], 16)
    rows = {int(r['entry'], 16): r for r in info['rows']}
    targets = {}
    for r in info['rows']:
        res = results.get(r['entry'], {})
        if res.get('status') == 'differs':
            targets[r['symbol']] = '%d bytes differ (adopted text)' % res.get('differing', 0)
    exact = {r['symbol'] for r in info['rows'] if results.get(r['entry'], {}).get('status') == 'exact'}
    for f in coff._functions:
        n = f['name']
        if n in exact or n in targets or n.startswith(('??_G', '??_E', '_$E')):
            continue
        a, label = resolve(n)
        if a is not None and lo <= a < hi and a in rows and results.get('%08x' % a, {}).get('status') != 'exact':
            targets[n] = 'no exact match yet (row %08x)' % a
        elif a is None and '?$' not in n and not n.startswith('??_'):
            targets[n] = 'no row of the original matches it yet (unplaced)'
        elif a is not None and lo <= a < hi and a not in rows and '?$' not in n and not n.startswith('??_'):
            # Its address lies in the unit but main has no row there: no independent bound, so it
            # cannot be accepted, and its statics would reach the image unbound (lane R3).
            targets[n] = 'no independent bound yet (%08x has no row)' % a
    if callers:
        # Only the functions that are not exact and call an unbound function out of line: an inline
        # helper the exact rows expand is emitted only for such callers, so guarding them leaves the
        # helper unemitted (lane R6).
        unbound = {f['name'] for f in coff._functions if resolve(f['name'])[0] is None and
                   not f['name'].startswith(('??_G', '??_E', '_$E'))}
        targets = {}
        for f in coff._functions:
            n = f['name']
            if n in exact or n in unbound or n.startswith(('??_G', '??_E', '_$E', '??_')) or '?$' in n.split('@@')[0]:
                continue
            section = coff.sections[f['section'] - 1]
            called = {coff.symbols[i]['name'] for _, i, _ in section['relocs'] if i in coff.symbols} & unbound
            if called:
                targets[n] = 'calls %s out of line, which no row of the original has' % ', '.join(
                    sorted(demangle(c) or c for c in called))[:120]
    src = R.ROOT / info['source']
    text = src.read_text(encoding='latin-1')
    edits, left = [], []
    for sym, why in sorted(targets.items()):
        name = demangle(sym)
        if not name:
            left.append((sym, 'not demangled')); continue
        pat = re.compile(r'^(?![ \t#/])[^\n;{}]*?(?<![\w:])%s\s*\(' % re.escape(name), re.M)
        def no_semicolon(m):
            seg = text[m.start():text.find('{', m.start())]
            return ';' not in re.sub(r'//[^\n]*', '', seg)
        hits = [m for m in pat.finditer(text) if body_end(text, text.find('{', m.start())) > 0 and no_semicolon(m)]
        if not hits and '::' in name:
            # A namespace member defined inside a `namespace NS {` block names itself unqualified (lane R6).
            # A member of a class inside the namespace names itself by the rest ('WinData::Copy').
            parts = name.split('::')
            for k in range(1, len(parts)):
                spans = namespace_spans(text, '::'.join(parts[:k]))
                rest = '::'.join(parts[k:])
                pat = re.compile(r'^(?![ \t#/])[^\n;{}]*?(?<![\w:~])%s\s*\(' % re.escape(rest), re.M)
                hits = [m for m in pat.finditer(text) if any(s < m.start() < e for s, e in spans) and
                        body_end(text, text.find('{', m.start())) > 0 and no_semicolon(m)]
                if hits:
                    break
        hits = [m for m in hits if not text[:m.start()].rstrip().endswith('#ifdef RA2_NON_MATCHING') and
                not re.search(r'#ifdef RA2_NON_MATCHING[^\n]*\n$', text[:m.start()])]
        if len(hits) > 1:
            # Overloads: the definition whose parameter list names every class the symbol's arguments do.
            args = sym.split('@@', 1)[1] if '@@' in sym else ''
            wanted = set(re.findall(r'[VU]([A-Za-z_]\w*)@', args[3:])) - {name.split('::')[0]}
            def params(m):
                i = text.index('(', m.end() - 1); depth = 0
                for j in range(i, len(text)):
                    depth += text[j] == '('; depth -= text[j] == ')'
                    if depth == 0: return text[i:j + 1]
                return ''
            fit = [m for m in hits if wanted and all(re.search(r'\b%s\b' % w, params(m)) for w in wanted)]
            hits = fit if len(fit) == 1 else hits
        if len(hits) != 1:
            left.append((sym, '%d definitions of %s' % (len(hits), name))); continue
        m = hits[0]
        short = name.split('::')[-1]
        if text[m.start():m.end()].lstrip().startswith('static') or re.search(
                r'^static\b[^\n;{]*\b%s\s*\([^;{]*\)\s*;' % re.escape(short), text, re.M):
            left.append((sym, 'file-static: callers need its definition')); continue
        end = body_end(text, text.find('{', m.start()))
        # A definition that is also the function's only declaration leaves a prototype in front of the
        # guard, so the callers still compile (lane R6).
        proto = ''
        if '::' not in text[m.start():m.end()]:
            decl = re.compile(r'(?<![\w:~])%s\s*\([^;{}]*\)\s*;' % re.escape(short))
            if not decl.search(text[:m.start()]):
                head = re.sub(r'\s*//[^\n]*', '', text[m.start():text.find('{', m.start())]).strip()
                proto = re.sub(r'^inline\s+', '', head) + '; // RA2: the definition is not exact yet\n'
        edits.append((m.start(), end, why, sym, proto))
    for start, end, why, sym, proto in sorted(edits, reverse=True):
        nl = text.find('\n', end)
        nl = len(text) if nl < 0 else nl
        text = text[:start] + proto + OPEN % why + text[start:nl] + '\n#endif' + text[nl:]
    print('%s: %d guarded, %d left' % (unit, len(edits), len(left)))
    for sym, why in left:
        print('    left %s: %s' % (sym[:80], why))
    if not dry and edits:
        src.write_text(text, encoding='latin-1')
        if not info.get('partial_ok'):
            info['partial_ok'] = True
            R.save_units(units, [unit])
    elif not dry and not info.get('partial_ok') and not targets:
        # Rows the object does not emit at all (the unit has no text for them yet) keep the original
        # bytes as a guarded draft would: the unit may land with them parked (lane R4). An emitted
        # function that binds nowhere still refuses the unit in the catalog.
        missing = [r for r in info['rows'] if results.get(r['entry'], {}).get('status') == 'missing']
        if missing and all(results.get(r['entry'], {}).get('status') in ('exact', 'missing') for r in info['rows']):
            info['partial_ok'] = True
            R.save_units(units, [unit])
            print('%s: %d rows not emitted are parked (partial_ok)' % (unit, len(missing)))


def refine(unit, before_path):
    """Unguard the drafts that exact rows need: a row exact before guarding and not after names a guarded
    function in its body (the compiler expanded the draft inline there). Returns how many were unguarded."""
    units = R.load_json(R.UNITS, {})
    info = units[unit]
    before = {r['entry']: r['status'] for r in json.load(open(before_path))['results']}
    after = {r['entry']: r['status'] for r in json.load(open(R.WORK / 'last' / (unit + '.json')))['results']}
    lost = [r for r in info['rows'] if before.get(r['entry']) == 'exact' and after.get(r['entry']) != 'exact']
    src = R.ROOT / info['source']
    text = src.read_text(encoding='latin-1')
    class G:
        def __init__(self, s, e, inner): self.s, self.e, self.inner = s, e, inner
        def start(self): return self.s
        def end(self): return self.e
        def group(self, i): return self.inner if i == 1 else text[self.s:self.e]
    guards = []
    for m in re.finditer(r'#ifdef RA2_NON_MATCHING // not exact: [^\n]*\n', text):
        close = body_end(text, text.find('{', m.end()))
        nl = text.find('\n', close)
        if close > 0 and text.startswith('\n#endif', nl):
            guards.append(G(m.start(), nl + len('\n#endif'), text[m.end():nl]))
    def short(block):
        m = re.search(r'([A-Za-z_~][\w]*)\s*\(', block.split('{')[0][::-1][::-1].split('\n')[0] if False else block.split('(')[0] + '(')
        return m.group(1) if m else None
    names = {}
    for g in guards:
        head = g.group(1).split('(')[0]
        n = re.findall(r'([A-Za-z_~]\w*)\s*$', head)
        if n: names[g] = n[0]
    drop = set()
    for r in lost:
        name = demangle(r['symbol']) or ''
        body = ''
        m = re.search(r'^(?![ \t#/])[^\n;{}]*?(?<![\w:])%s\s*\(' % re.escape(name), text, re.M) if name else None
        if m:
            end = body_end(text, text.find('{', m.start()))
            body = text[m.start():end]
        else:
            body = text        # the lost row is a template copy or a renamed row: unguard what any row may call
        for g, n in names.items():
            if re.search(r'(?<![\w~])%s\s*\(' % re.escape(n), body.replace(g.group(1), '')):
                drop.add(g)
    for g in sorted(drop, key=lambda g: -g.start()):
        text = text[:g.start()] + g.group(1) + text[g.end():]
    if drop:
        src.write_text(text, encoding='latin-1')
    print('%s: %d rows lost to guards, %d drafts unguarded' % (unit, len(lost), len(drop)))
    return len(drop)


if __name__ == '__main__':
    if '--refine' in sys.argv:
        refine(sys.argv[1], sys.argv[sys.argv.index('--refine') + 1])
    else:
        main(sys.argv[1], '--dry' in sys.argv, '--callers' in sys.argv)
