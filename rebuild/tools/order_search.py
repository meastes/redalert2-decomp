"""order_search.py UNIT ENTRY [--alt ALTS.json] [--split] [--pairs N] [--max N] [--batch N] [--keep N]
[--only KINDS]: a mechanical search over ordinary matching iteration for one function (lane O12, 2026-11-21).

The function's text in rebuild/src/UNIT.cpp (its RA2_NON_MATCHING guard opened in a scratch copy; the tree's
file is never written) is varied by these transforms only, each of which keeps the same variables and the same
operations:

  perm    the order of a run of adjacent sibling statements (declarations included) that are pairwise
          independent: no calls, no statement reads or writes what another writes, no two writes through
          pointers that may alias, no read through a pointer beside a write through one (all orders of a run
          of up to 5, adjacent swaps in a longer run)
  split   (--split) a run of initialised pointer or builtin declarations declared first in one order and
          assigned in another (the same reads, in an order the perm rule allows)
  incr    `x++` <-> `++x` as a statement or a for step
  loop    `for (A; B; C) {...}` <-> `A; while (B) {... C;}` (no continue in the body)
  alt     a statement's text from the twin, the archive or main (--alt: a JSON list of
          {"old": text, "new": text, "source": citation}; old must occur once in the function)

It never adds a statement, a local, an alias, a cast or a declaration placed for a count. Each variant is
compiled with the real compiler through the rebuild's compile path (build.compile_units: the compiler lock,
up to `parallel_sessions` Wine sessions per batch), the row is compared with the original (references bound
as rebuild.py check binds them), and every variant gets the row's differing bytes and the unit's exact-row
count. Singles come first; then pairs of the singles that did not get worse (--pairs, default 40). The ranked
table and the best variants' texts are written to build/r/order_search/UNIT-ENTRY/ (table.tsv, best-N.cpp,
best-N.diff). A variant that is exact still lands under the normal rules only (a twin statement flagged as
a judgment call); anything outside them goes to manifest/variation_candidates.csv as a finding.
"""
import difflib
import hashlib
import itertools
import json
import re
import shutil
import sys
import time
from pathlib import Path

import rebuild as R
from coff import Coff
from guard import body_end
from tailmap import guards, open_text
from verify_manifest import Audit

KEYWORDS = {'return', 'break', 'continue', 'goto', 'if', 'else', 'for', 'while', 'do', 'switch', 'case',
            'default', 'delete', 'throw', 'new', 'sizeof', 'operator', 'this', 'typedef', 'using'}
NOT_CALLS = {'sizeof', 'if', 'while', 'for', 'switch', 'return'}
BUILTIN = {'int', 'unsigned', 'signed', 'char', 'short', 'long', 'bool', 'float', 'double', 'wchar_t', 'BYTE',
           'WORD', 'DWORD', 'UINT', 'HWND', 'HANDLE', 'size_t', 'uint', 'ushort', 'uchar', '__int64'}

def pod_typedefs():
    """Names the rebuild headers typedef to a builtin or a builtin array (no constructor)."""
    names = set()
    for path in (R.REBUILD / 'include').rglob('*.h'):
        for m in re.finditer(r'^\s*typedef\s+((?:unsigned|signed|const)\s+)*(\w+)\s+\**\s*(\w+)\s*(\[[^\]]*\])?\s*;',
                             path.read_text(encoding='latin-1'), re.M):
            if m.group(2) in BUILTIN or m.group(2) in ('void',) and '*' in m.group(0):
                names.add(m.group(3))
    return names


BUILTIN |= pod_typedefs()
DECL = re.compile(r'^(?:(?:const|unsigned|signed|struct|class|enum|static)\s+)*([A-Za-z_]\w*(?:::\w+)*(?:<[^;=()]*>)?)'
                  r'((?:\s*(?:\*|&|const\b))*\s+|(?:\s*(?:\*|&))+\s*)([A-Za-z_]\w*)\s*(\[[^\]]*\])?\s*(?:=\s*(.+))?;$', re.S)
IDENT = re.compile(r'[A-Za-z_]\w*')


def code_of(line):
    """The line without its // comment (string literals kept whole)."""
    out, i, quote = [], 0, None
    while i < len(line):
        c = line[i]
        if quote:
            out.append(c)
            if c == '\\':
                out.append(line[i + 1:i + 2]); i += 1
            elif c == quote:
                quote = None
        elif c in '"\'':
            quote = c; out.append(c)
        elif line.startswith('//', i):
            break
        else:
            out.append(c)
        i += 1
    return ''.join(out).rstrip()


def strip_literals(code):
    return re.sub(r'L?"(?:\\.|[^"\\])*"|L?\'(?:\\.|[^\'\\])*\'', '0', code)


class Unit:
    """One statement unit of the body: lines [a, b) with its brace depth at the start."""
    def __init__(self, lines, a, b, depth):
        self.a, self.b, self.depth = a, b, depth
        self.code = strip_literals(' '.join(code_of(l).strip() for l in lines[a:b]))
        self.analyse()

    def analyse(self):
        c = self.code
        self.simple = c.endswith(';') and c.count('{') == c.count('}') == 0 and c.count('(') == c.count(')')
        first = IDENT.match(c)
        self.keyword = first.group(0) in KEYWORDS if first else not c.startswith(('++', '--', '*', '('))
        self.decl = None
        m = DECL.match(c)
        if self.simple and m and m.group(1) not in KEYWORDS and not c.startswith(('return', 'delete')):
            self.decl = dict(type=m.group(1), ptr=m.group(2), name=m.group(3), array=m.group(4), init=m.group(5))
        self.calls = [n for n in re.findall(r'([A-Za-z_]\w*)\s*\(', c) if n not in NOT_CALLS]
        if self.decl:
            # A declaration's own type spelling is not a call (TRect<int> r(...) is, and is refused below).
            pass
        self.reads = set(IDENT.findall(c)) - KEYWORDS
        self.writes, self.memwrites = set(), []
        if self.decl:
            self.writes.add(self.decl['name'])
            self.reads -= {self.decl['type'].split('::')[-1], self.decl['name']}
            if self.decl['init'] is None and self.decl['type'] not in BUILTIN and '*' not in self.decl['ptr']:
                self.calls.append(self.decl['type'])    # a class local runs its constructor
        else:
            for lhs in re.findall(r'^\s*([^=;]*?)\s*(?:[-+*/%&|^]|<<|>>)?=(?!=)', c):
                self.lvalue(lhs)
            for x in re.findall(r'(?:\+\+|--)\s*([\w.\->\[\]]+)|([\w.\->\[\]]+)\s*(?:\+\+|--)', c):
                self.lvalue(x[0] or x[1])
        self.deref = '->' in c or '[' in c or re.search(r'(^|[=(,!&|+\-*/<>?:]\s*)\*\s*[A-Za-z_(]', c) is not None

    def lvalue(self, text):
        text = text.strip()
        ids = IDENT.findall(text)
        if not ids:
            return
        if '->' in text or '[' in text or text.startswith('*'):
            constant_index = re.search(r'\[\s*\d+\s*\]$', text)
            field = re.sub(r'\s+', '', text) if constant_index else ids[-1]
            self.memwrites.append((ids[0], field, text))
            self.writes.add(field)
        else:
            self.writes.add(ids[0])
            self.writes.add(ids[-1])

    def movable(self):
        return self.simple and not self.calls and (self.decl or not self.keyword)


def pod_declaration(u):
    d = u.decl
    return d is not None and d['init'] is None and (d['type'] in BUILTIN or '*' in d['ptr'])


def independent(x, y):
    for p, q in ((x, y), (y, x)):
        if pod_declaration(p) and q.simple and p.decl['name'] not in q.reads | q.writes and \
                (not q.decl or q.decl['name'] not in p.reads):
            return True    # declaration order alone: a local with no initialiser and no constructor
    if not (x.movable() and y.movable()):
        return False
    if x.writes & (y.reads | y.writes) or y.writes & (x.reads | x.writes):
        return False
    for p, q in ((x, y), (y, x)):
        for base, field, _ in p.memwrites:
            if q.deref:
                same_base_only = all(b == base and f != field for b, f, _ in q.memwrites) and \
                    field not in q.reads and not re.search(r'\[|(^|[^\w])\*\s*[A-Za-z_(]', q.code.split('=', 1)[-1])
                if not same_base_only:
                    return False
    return True


def split_units(lines, start, end):
    """Statement units of lines [start, end) (the body between the function's braces)."""
    units, depth, i = [], 0, start
    while i < end:
        code = code_of(lines[i]).strip()
        if not code or code.startswith('#'):
            i += 1
            continue
        j, text = i, code
        while not text.endswith((';', '{', '}')) and j + 1 < end:
            j += 1
            text += ' ' + code_of(lines[j]).strip()
        units.append(Unit(lines, i, j + 1, depth))
        depth += text.count('{') - text.count('}')
        i = j + 1
    return units


def sibling_runs(units):
    """Maximal runs of adjacent units at one depth whose members are pairwise independent."""
    runs, k = [], 0
    while k < len(units):
        run = [units[k]]
        m = k + 1
        while m < len(units) and units[m].depth == run[0].depth and units[m].a == run[-1].b and \
                all(independent(u, units[m]) for u in run):
            run.append(units[m]); m += 1
        if len(run) > 1 and not any(run[-1] is r[-1] for r in runs):
            runs.append(run)
        k += 1
    return runs


def permute_lines(lines, run, order):
    out = list(lines[:run[0].a])
    for idx in order:
        out += lines[run[idx].a:run[idx].b]
    return out + lines[run[-1].b:]


def transforms(lines, start, end, alts, split):
    """[(kind, description, set of touched line indexes, function(lines) -> lines)]."""
    units = split_units(lines, start, end)
    out = []
    for run in sibling_runs(units):
        n = len(run)
        names = [u.decl['name'] if u.decl else code_of(lines[u.a]).strip()[:24] for u in run]
        touched = set(range(run[0].a, run[-1].b))
        orders = [p for p in itertools.permutations(range(n)) if list(p) != list(range(n))] if n <= 5 else \
            [tuple(range(i)) + (i + 1, i) + tuple(range(i + 2, n)) for i in range(n - 1)]
        for p in orders:
            out.append(('perm', 'line %d: %s' % (run[0].a + 1, ' '.join(names[i] for i in p)), touched,
                        lambda ls, run=run, p=p: permute_lines(ls, run, p)))
        ok = [bool(u.decl and u.decl['init'] and (u.decl['type'] in BUILTIN or '*' in u.decl['ptr'])
                   and not u.decl['array']) for u in run]
        for i0 in range(n):
            i1 = i0
            while i1 < n and ok[i1]:
                i1 += 1
            sub = run[i0:i1]
            if not split or not 2 <= len(sub) <= 4 or (i0 and ok[i0 - 1]):
                continue
            subnames = names[i0:i1]
            for p in itertools.permutations(range(len(sub))):
                for q in itertools.permutations(range(len(sub))):
                    out.append(('split', 'line %d: declare %s, assign %s' % (
                        sub[0].a + 1, ' '.join(subnames[i] for i in p), ' '.join(subnames[i] for i in q)), touched,
                        lambda ls, sub=sub, p=p, q=q: split_lines(ls, sub, p, q)))
    for i in range(start, end):
        code = code_of(lines[i])
        for m in re.finditer(r'(?<![\w+\-.>])([A-Za-z_](?:[\w.]|->)*)(\+\+|--)(?=\s*[;)])|(\+\+|--)([A-Za-z_](?:[\w.]|->)*)(?=\s*[;)])', code):
            if m.group(1):
                new = m.group(2) + m.group(1)
            else:
                new = m.group(4) + m.group(3)
            if not (re.match(r'\s*for\s*\(', code) or code.strip() == m.group(0) + ';'):
                continue
            out.append(('incr', 'line %d: %s -> %s' % (i + 1, m.group(0), new), {i},
                        lambda ls, i=i, s=m.start(), e=m.end(), new=new: ls[:i] + [ls[i][:s] + new + ls[i][e:]] + ls[i + 1:]))
    for u in units:
        m = re.match(r'(\s*)for\s*\((.*?);(.*?);(.*)\)\s*\{\s*$', code_of(lines[u.a]))
        if m and u.b == u.a + 1:
            close = closing_line(lines, u.a, end)
            body = '\n'.join(code_of(l) for l in lines[u.a + 1:close])
            if close is not None and not re.search(r'\bcontinue\b', body):
                out.append(('loop', 'line %d: for -> while' % (u.a + 1), set(range(u.a, close + 1)),
                            lambda ls, a=u.a, c=close, m=m: for_to_while(ls, a, c, m)))
        m = re.match(r'(\s*)while\s*\((.*)\)\s*\{\s*$', code_of(lines[u.a]))
        if m and u.b == u.a + 1:
            close = closing_line(lines, u.a, end)
            if close is not None and close - 1 > u.a:
                last = code_of(lines[close - 1]).strip()
                body = '\n'.join(code_of(l) for l in lines[u.a + 1:close - 1])
                if re.match(r'^[\w.\->\[\]+\-=*\s]+;$', last) and not re.search(r'\bcontinue\b', body) and \
                        not re.search(r'\b(if|for|while|else)\b', code_of(lines[close - 2])):
                    out.append(('loop', 'line %d: while -> for' % (u.a + 1), set(range(u.a, close + 1)),
                                lambda ls, a=u.a, c=close, m=m, last=last: while_to_for(ls, a, c, m, last)))
    text = '\n'.join(lines[start:end])
    for alt in alts:
        if text.count(alt['old']) != 1:
            print('  alt not applied (occurs %d times): %s' % (text.count(alt['old']), alt['old'][:60]))
            continue
        offset = text.index(alt['old'])
        first = start + text.count('\n', 0, offset)
        last = start + text.count('\n', 0, offset + len(alt['old']))
        out.append(('alt', 'line %d: %s (%s)' % (first + 1, alt['new'][:50], alt.get('source', '')),
                    set(range(first, last + 1)), lambda ls, alt=alt: replace_once(ls, start, end, alt)))
    return out


def replace_once(lines, start, end, alt):
    text = '\n'.join(lines[start:end]).replace(alt['old'], alt['new'], 1)
    return lines[:start] + text.split('\n') + lines[end:]


def closing_line(lines, a, end):
    depth = 0
    for i in range(a, end):
        code = strip_literals(code_of(lines[i]))
        depth += code.count('{') - code.count('}')
        if depth == 0:
            return i if code.strip() == '}' else None
    return None


def for_to_while(lines, a, close, m):
    indent, init, cond, step = m.group(1), m.group(2).strip(), m.group(3).strip(), m.group(4).strip()
    comment = lines[a][len(code_of(lines[a])):]
    head = ([indent + init + ';'] if init else []) + [indent + 'while (%s) {%s' % (cond or '1', comment)]
    tail = [indent + '\t' + step + ';'] if step else []
    return lines[:a] + head + lines[a + 1:close] + tail + lines[close:]


def while_to_for(lines, a, close, m, last):
    indent, cond = m.group(1), m.group(2)
    comment = lines[a][len(code_of(lines[a])):]
    return lines[:a] + [indent + 'for (; %s; %s) {%s' % (cond, last.rstrip(';'), comment)] + \
        lines[a + 1:close - 1] + lines[close:]


def split_lines(lines, run, p, q):
    decl_lines, assign_lines = [], []
    for u in run:
        line = lines[u.a]
        code = code_of(line)
        comment = line[len(code):]
        d = u.decl
        indent = re.match(r'\s*', line).group(0)
        decl_lines.append(indent + code.strip().split('=')[0].rstrip() + ';' + comment)
        assign_lines.append(indent + '%s = %s;' % (d['name'], d['init'].rstrip(';').strip()) + comment)
    out = list(lines[:run[0].a])
    out += [decl_lines[i] for i in p] + [assign_lines[i] for i in q]
    return out + lines[run[-1].b:]


def locate(text, row, names):
    """(definition start, body open brace, body end) of the row's function in the unit text."""
    symbol = row['symbol']
    name = names.get(int(row['entry'], 16)) or symbol
    m = re.match(r'\?(\w+)@(?:(\w+)@)?', symbol)
    func, owner = (m.group(1), m.group(2)) if m else (symbol, None)
    if symbol.startswith('??0'):
        func = owner = re.match(r'\?\?0(\w+)@', symbol).group(1)
    pattern = (r'%s::%s\s*\(' % (owner, func)) if owner and not symbol.endswith('@@YIXXZ') else r'(?<![\w:~])%s\s*\(' % func
    pattern = r'(?:%s|STDMETHOD_?\((?:\w+,\s*)?%s\)\s*\()' % (pattern, func)
    hits = []
    for hit in re.finditer(r'^[^\n;{}#/]*' + pattern + r'(?:[^;{/]|//[^\n]*)*\{', text, re.M):
        if 'RA2: 0x%s' % row['entry'].upper() in text[hit.start():hit.end() + 200].upper() or True:
            hits.append(hit)
    for hit in hits:
        head = text[hit.start():hit.end()]
        address = row['entry'].lstrip('0').lower()
        if address in text[hit.start():text.find('\n', hit.start())].lower() or \
                address in text[max(0, hit.start() - 3000):hit.start()].lower().split('\n}')[-1]:
            return hit.start(), hit.end() - 1, body_end(text, hit.end() - 1)
    if len(hits) == 1:
        return hits[0].start(), hits[0].end() - 1, body_end(text, hits[0].end() - 1)
    sys.exit('cannot locate %s (%d candidates)' % (symbol, len(hits)))


def main(argv):
    import build
    from canonical import compiler_arguments
    args = [a for a in argv if not a.startswith('--')]
    opt = lambda k, d=None: next((argv[i + 1] for i, a in enumerate(argv) if a == k), d)
    unit, entry = args[0], args[1].lower().replace('0x', '').zfill(8)
    info = R.load_json(R.UNITS, {})[unit]
    row = next(r for r in info['rows'] if r['entry'] == entry)
    text = (R.ROOT / info['source']).read_text(encoding='latin-1')
    names = {a: s for s, a in R.mischa_names().items()}
    d0, brace, d1 = locate(text, row, names)
    opened = {s for s, e, label in guards(text) if s < d0 < e}
    text = open_text(text, opened)
    lines = text.split('\n')
    first = text.count('\n', 0, brace) + 1
    last = text.count('\n', 0, d1 - 1)
    alts = json.load(open(opt('--alt'))) if opt('--alt') else []
    kinds = set(opt('--only', 'perm,split,incr,loop,alt').split(','))
    singles = [t for t in transforms(lines, first, last, alts, '--split' in argv) if t[0] in kinds]
    if '--list' in argv:
        for t in singles:
            print('  %-5s %s' % (t[0], t[1]))
        return
    maximum, batch = int(opt('--max', '400')), int(opt('--batch', '8'))
    out_dir = R.WORK / 'order_search' / ('%s-%s' % (unit, entry))
    shutil.rmtree(out_dir, ignore_errors=True); out_dir.mkdir(parents=True)
    config = json.loads(R.CONFIG.read_text())
    audit = Audit(R.ROOT, config['executable'])
    resolve = R.resolver(unit, info)
    print('%s %s %s: %d single variants (%s)' % (unit, entry, row['symbol'][:60], len(singles),
                                                  ', '.join('%s %d' % (k, sum(t[0] == k for t in singles)) for k in sorted(kinds))))

    def score(data):
        coff = Coff(data)
        functions = {s['name'] for s in coff._functions}
        result, exact = None, 0
        for r in info['rows']:
            if r['symbol'] not in functions:
                continue
            st, diff, size, b, probs = R.compare(audit, coff, r['symbol'], int(r['entry'], 16), r['body'],
                                                 r.get('gap_padding', r['padding']), resolve)
            exact += st == 'exact'
            if r is row:
                result = (diff, st, hashlib.sha1(R.compare.actual).hexdigest()[:10], len(probs))
                if '--asm' in argv and not getattr(score, 'shown', False):
                    score.shown = True
                    print('\n'.join(probs)[:2000])
                    print(R.disasm_diff(audit, R.compare.actual, int(r['entry'], 16), r['body']))
        return result, exact

    def run(variants):
        results = []
        for k in range(0, len(variants), batch):
            chunk = variants[k:k + batch]
            run.calls = getattr(run, 'calls', 0) + 1
            folder = out_dir / ('c%04d' % run.calls)
            folder.mkdir()
            units = []
            for j, (desc, vlines) in enumerate(chunk):
                src = folder / ('v%02d.cpp' % j)
                src.write_text('\n'.join(vlines), encoding='latin-1')
                units.append(dict(cwd=folder, source=src, obj=folder / ('v%02d.obj' % j), log=folder / ('v%02d.log' % j),
                                  returncode=folder / ('v%02d.rc' % j), args=compiler_arguments(info['options'])))
            build.compile_units(config, folder, units, 'order-search', parallel=True, timeout=1800)
            for (desc, vlines), u in zip(chunk, units):
                ok = Path(u['obj']).exists()
                res = score(Path(u['obj']).read_bytes()) if ok else (None, None)
                results.append((desc, vlines, res))
            for j, u in enumerate(units):
                failed = not Path(u['obj']).exists()
                for p in (u['obj'], u['returncode'], u['source']) + (() if failed else (u['log'],)):
                    Path(p).unlink(missing_ok=True)
            print('  %d/%d compiled' % (min(k + batch, len(variants)), len(variants)), flush=True)
            if all(Path(u['obj']).exists() for u in units):
                shutil.rmtree(folder, ignore_errors=True)
        return results

    started = time.time()
    base = run([('base', lines)])[0][2]
    print('base: %s, unit exact rows %s' % (base[0], base[1]))
    if base[0] is None:
        sys.exit('the base does not compile or the row is not emitted')
    single_results = run([((t[0], t[1]), t[3](lines)) for t in singles[:maximum]])
    table = [(('base', 'tree text'), lines, base)] + single_results
    pair_limit = int(opt('--pairs', '40'))
    keep = [(t, r) for t, r in zip(singles, single_results) if r[2][0] and r[2][0][0] <= base[0][0] and
            r[2][0][2] != base[0][2]]
    keep.sort(key=lambda tr: tr[1][2][0][0])
    pairs = []
    for (t1, r1), (t2, r2) in itertools.combinations(keep, 2):
        if t1[2] & t2[2] or r1[2][0][2] == r2[2][0][2]:
            continue
        pairs.append((('pair', t1[1] + ' + ' + t2[1]), t2[3](t1[3](lines))))
        if len(pairs) >= pair_limit:
            break
    if pairs:
        print('%d pairs of the %d singles that did not get worse' % (len(pairs), len(keep)))
        table += run(pairs)
    rows = []
    for (kind, desc), vlines, res in table:
        (diff, st, digest, probs), exact = res if res[0] else ((None, 'error', '', 0), None)
        rows.append(dict(kind=kind, desc=desc, diff=diff, status=st, digest=digest, unit_exact=exact, lines=vlines))
    rows.sort(key=lambda r: (r['diff'] is None, r['diff'] if r['diff'] is not None else 0, r['kind'] != 'base'))
    with open(out_dir / 'table.tsv', 'w') as f:
        f.write('rank\tdiff\tstatus\tunit_exact\tobject\tkind\tvariant\n')
        for i, r in enumerate(rows):
            f.write('%d\t%s\t%s\t%s\t%s\t%s\t%s\n' % (i, r['diff'], r['status'], r['unit_exact'], r['digest'],
                                                     r['kind'], r['desc']))
    for i, r in enumerate(rows[:int(opt('--keep', '3'))]):
        (out_dir / ('best-%d.cpp' % i)).write_text('\n'.join(r['lines']), encoding='latin-1')
        (out_dir / ('best-%d.diff' % i)).write_text(''.join(difflib.unified_diff(
            [l + '\n' for l in lines], [l + '\n' for l in r['lines']], 'tree', r['desc'][:60], n=1)))
    for c in out_dir.glob('c*'):
        shutil.rmtree(c, ignore_errors=True)
    outcomes = {}
    for r in rows:
        if r['diff'] is not None:
            outcomes.setdefault((r['diff'], r['digest']), []).append(r)
    print('%d variants in %.0fs; %d distinct objects for the row; base %s' % (
        len(rows), time.time() - started, len(outcomes), base[0][0]))
    for (diff, digest), rs in sorted(outcomes.items())[:12]:
        print('  %5s %s x%-3d unit %s  e.g. %s %s' % (diff, digest, len(rs), rs[0]['unit_exact'], rs[0]['kind'],
                                                     rs[0]['desc'][:110]))
    print('table: %s' % (out_dir / 'table.tsv'))


if __name__ == '__main__':
    main(sys.argv[1:])
