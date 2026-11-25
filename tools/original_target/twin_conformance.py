#!/usr/bin/env python3
"""twin_conformance.py [--write] [--top N]: mechanical conformance of the rebuild against ra2-decomp (lane C1,
2026-11-20). No compile; measurement only.

ra2-decomp (gibbo101's Yuri's Revenge decompilation, private/reference/ra2-decomp, their include/ over the pinned
TibSun archive) is compared with include and src in four sections, one CSV
(manifests/twin_conformance.csv, column `section`):

  helper    every inline function, wrapper, accessor and file-local helper either tree defines: in-class bodies and
            out-of-class definitions in headers; in a unit source, definitions that are `static`, `inline` or
            declared `_STATIC`, and in-class bodies. Keyed by owner class and name, overloads by parameter types.
            status same (tokens equal outside comments), different (placement in-class/out-of-class, const,
            parameter types, specifiers, body), absent (theirs only). For a different or absent entry: the
            parked rows (manifests/tail_buckets.csv) whose matched twin (tail_twins.csv: the YR address in
            ra2-decomp's data/manifest.csv) calls it, directly or through other twin helpers, and their bytes.
  ctor      for every class with a parked row: our constructors' member-initialiser lists and base inits against
            the twin's, and explicit constructors one side defines and the other does not.
  spellout  parked rows with a matched twin body (twin_queue.csv "adopt the twin's text"): statement-level diff of
            our draft against the twin's text; every replaced span where theirs calls a helper (inventory entry or
            any function the twin tree defines) that our span does not call, with whether our tree defines it.
  include   per unit with twin rows: our #include list against the twin unit's (set and order).

A call is attributed to an entry when the entry is a free function, or its owner is the parked row's class, one of
that class's bases, or a class (or base of a class) named in the twin's body. Rows: ranked by parked bytes.
"""
import argparse
import collections
import csv
import difflib
import re
import sys
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
ROOT = TOOLS.parents[1]
MANIFEST = ROOT / 'manifests'
THEIRS = ROOT / 'private/reference/ra2-decomp'
TIBSUN = ROOT / 'private/reference/TibSun/code'
OUT = MANIFEST / 'twin_conformance.csv'

HDR = ('.h', '.hh', '.hpp', '.inl')
SRC = ('.cpp', '.c')
COMMENT = re.compile(r'//[^\n]*|/\*.*?\*/', re.S)
TOKEN = re.compile(r'[A-Za-z_]\w*|0[xX][0-9a-fA-F]+[uUlL]*|\d+\.?\d*(?:[eE][-+]?\d+)?[fFlLuU]*|"(?:\\.|[^"\\\n])*"'
                   r'|\'(?:\\.|[^\'\\\n])*\'|::|->|\+\+|--|<<=|>>=|<<|>>|[<>!=+\-*/%&|^]=|&&|\|\||\S')
KEYWORDS = {'if', 'for', 'while', 'switch', 'return', 'sizeof', 'catch', 'do', 'else', 'case', 'new', 'delete',
            '__declspec', 'defined', 'throw', '__asm', 'typeid', 'static_cast', 'dynamic_cast', 'reinterpret_cast',
            'const_cast', 'operator'}
SPECS = {'inline', 'static', '_STATIC', 'virtual', 'explicit', '__inline', '__forceinline', 'extern', 'friend'}
TYPEWORDS = {'const', 'volatile', 'unsigned', 'signed', 'struct', 'class', 'enum', 'union', 'typename'}


# ---------------------------------------------------------------- preprocessing

def live_text(text):
    """Text with comments blanked, `#if 0` (and `#else` of `#if 1`) regions blanked and directive lines blanked;
    returns (text, guarded) where guarded[i] is True for lines under an RA2_NON_MATCHING guard. Line count kept."""
    text = COMMENT.sub(lambda m: re.sub(r'[^\n]', ' ', m.group(0)), text)
    lines = text.split('\n')
    stack = []            # (dead, guard)
    out, guarded = [], []
    cont = False
    for ln in lines:
        s = ln.strip()
        if cont or s.startswith('#'):
            cont = s.endswith('\\')
            d = re.match(r'#\s*(if|ifdef|ifndef|elif|else|endif)\b(.*)', s)
            if d and not (cont and False):
                kind, arg = d.group(1), d.group(2).strip()
                if kind in ('if', 'ifdef', 'ifndef'):
                    dead = kind == 'if' and re.fullmatch(r'0|\(0\)|defined\s*\(?\s*RA2_UNUSED\s*\)?', arg) is not None
                    guard = kind == 'ifdef' and arg.split()[:1] == ['RA2_NON_MATCHING']
                    if kind == 'ifndef' and arg.split()[:1] == ['RA2_NON_MATCHING']:
                        dead = True
                    stack.append([dead, guard, kind == 'if' and arg == '1'])
                elif kind == 'else' and stack:
                    top = stack[-1]
                    if top[0] or top[2]:
                        top[0] = not top[0]
                    if top[1]:
                        top[1] = False; top[0] = True       # the guard's #else: the exact build's text, not the draft
                elif kind == 'elif' and stack:
                    pass
                elif kind == 'endif' and stack:
                    stack.pop()
            out.append(''); guarded.append(False)
            continue
        dead = any(t[0] for t in stack)
        out.append('' if dead else ln)
        guarded.append(any(t[1] for t in stack))
    return '\n'.join(out), guarded


def toks(text):
    """[(token, line)]"""
    res = []
    line = 1
    pos = 0
    for m in TOKEN.finditer(text):
        line += text.count('\n', pos, m.start())
        pos = m.start()
        res.append((m.group(0), line))
    return res


# ---------------------------------------------------------------- definitions

class Def:
    __slots__ = ('file', 'line', 'owner', 'name', 'params', 'const', 'specs', 'in_class', 'init', 'body', 'guarded',
                 'unit', 'header', 'template', 'nreq')

    def key(self):
        return (self.owner, self.name)

    def where(self):
        return '%s:%d' % (self.file, self.line)


def match_close(t, i, o, c):
    d = 0
    for k in range(i, len(t)):
        if t[k][0] == o:
            d += 1
        elif t[k][0] == c:
            d -= 1
            if d == 0:
                return k
    return len(t) - 1


def strip_targs(seq):
    out, d = [], 0
    for x in seq:
        if x == '<':
            d += 1
        elif x == '>':
            d -= 1
        elif d == 0:
            out.append(x)
    return out


def count_defaults(ptoks):
    d, n = 0, 0
    for x in ptoks:
        if x in '(<[':
            d += 1
        elif x in ')>]':
            d -= 1
        elif x == '=' and d == 0:
            n += 1
    return n


def norm_params(ptoks):
    """Parameter types only: names, defaults dropped; `void` alone is empty."""
    parts, cur, d = [], [], 0
    for x in ptoks:
        if x in '(<[':
            d += 1
        elif x in ')>]':
            d -= 1
        if x == ',' and d == 0:
            parts.append(cur); cur = []
        else:
            cur.append(x)
    if cur:
        parts.append(cur)
    res = []
    for p in parts:
        if '=' in p:
            p = p[:p.index('=')]
        # drop a trailing declarator name (identifier after a type word / * / &), keep array brackets out
        while p and p[-1] == ']':
            p = p[:p.index('[')] + ['[]'] if '[' in p else p[:-1]
        if len(p) >= 2 and re.match(r'[A-Za-z_]\w*$', p[-1]) and p[-1] not in TYPEWORDS and (
                p[-2] in ('*', '&', '>', ',') or re.match(r'[A-Za-z_]\w*$', p[-2])):
            if not (len(p) == 2 and p[0] in ('unsigned', 'signed', 'long', 'short') and p[1] in ('int', 'long', 'char', 'short')):
                p = p[:-1]
        res.append(' '.join(p))
    if res == ['void']:
        res = []
    return tuple(res)


def scan(path, rel, unit=None, header=False):
    """Every function definition in a file: Def list, plus class bases {class: [bases]} and _STATIC prototypes."""
    raw = path.read_text(errors='replace')
    text, guarded = live_text(raw)
    t = toks(text)
    defs, bases, protos = [], {}, set()

    def scope(i, end, cls):
        stmt = []
        while i < end:
            x = t[i][0]
            if x == '}':
                return i + 1
            if x in ('public', 'private', 'protected') and i + 1 < end and t[i + 1][0] == ':' and not stmt:
                i += 2; continue
            if x == '(':
                k = match_close(t, i, '(', ')')
                stmt.extend(range(i, k + 1)); i = k + 1; continue
            if x == ';':
                words = [t[j][0] for j in stmt]
                if words and words[0] == '_STATIC' and '(' in words:
                    n = words[words.index('(') - 1]
                    protos.add(n)
                stmt = []; i += 1; continue
            if x != '{':
                stmt.append(i); i += 1; continue
            # a block opens
            words = [t[j][0] for j in stmt]
            close = match_close(t, i, '{', '}')
            if not words:
                i = close + 1; continue
            if words[0] in ('namespace',) or (words[0] == 'extern' and len(words) <= 2):
                scope(i + 1, close, cls); i = close + 1; stmt = []; continue
            if 'enum' in words and '(' not in words:
                i = close + 1; continue
            ck = [k for k, w in enumerate(words) if w in ('class', 'struct', 'union')]
            if ck and '(' not in words[:ck[0]] and (')' not in words or words.index(')') < ck[0]) and '=' not in words:
                k = ck[0] + 1
                while k < len(words) and words[k] == '__declspec':
                    k += 4
                name = words[k] if k < len(words) and re.match(r'[A-Za-z_]\w*$', words[k]) else ''
                if ':' in words[k:]:
                    b = [w for w in strip_targs(words[words.index(':', k) + 1:])
                         if re.match(r'[A-Za-z_]\w*$', w) and w not in ('public', 'private', 'protected', 'virtual')]
                    bases.setdefault(name, []).extend(b)
                scope(i + 1, close, name or cls)
                i = close + 1
                stmt = []
                continue
            if '=' in words and ('(' not in words or words.index('=') < words.index('(')):
                i = close + 1; continue          # an aggregate initialiser: the statement goes on to its ';'
            if '(' not in words:
                i = close + 1; stmt = []; continue
            d = make_def(stmt, i, close, cls)
            if d:
                defs.append(d)
            i = close + 1
            stmt = []
        return i

    def make_def(stmt, ob, close, cls):
        words = [t[j][0] for j in stmt]
        # the declarator's parameter list: the first '(' after a name that is not a specifier macro
        p = None
        for k, w in enumerate(words):
            if w == '(' and k > 0:
                prev = words[k - 1]
                if prev == 'operator' or (k >= 2 and words[k - 2] == 'operator'):
                    if prev == 'operator':
                        continue            # operator ( ) ( ... ): the call operator's own parens
                if re.match(r'[A-Za-z_~]\w*$', prev) and prev not in KEYWORDS - {'operator'} and prev not in (
                        '__declspec', 'CALLBACK', 'WINAPI', '__cdecl', '__stdcall', '__fastcall') or prev in (')',) and 'operator' in words[:k]:
                    p = k
                    break
                if 'operator' in words[:k]:
                    p = k
                    break
        if p is None:
            return None
        if 'operator' in words[:p]:
            o = words.index('operator')
            name = 'operator' + ''.join(words[o + 1:p])
            q = o
        else:
            name = words[p - 1]
            q = p - 1
            if q >= 1 and words[q - 1] == '~':
                name = '~' + name; q -= 1
        qual = []
        while q >= 2 and words[q - 1] == '::':
            j = q - 2
            if words[j] == '>':
                dd = 0
                while j >= 0:
                    if words[j] == '>':
                        dd += 1
                    elif words[j] == '<':
                        dd -= 1
                        if dd == 0:
                            break
                    j -= 1
                j -= 1
            qual.insert(0, words[j])
            q = j
        if name in KEYWORDS:
            return None
        pc = match_close([(w, 0) for w in words], p, '(', ')')
        after = words[pc + 1:]
        d = Def()
        d.file, d.line = rel, t[stmt[0]][1]
        d.owner = qual[-1] if qual else (cls or '')
        d.name = name
        d.params = norm_params(words[p + 1:pc])
        d.nreq = len(d.params) - count_defaults(words[p + 1:pc])
        d.const = bool(after) and after[0] == 'const'
        head = words[:q]
        d.template = 'template' in head
        d.specs = tuple(sorted({w for w in head if w in SPECS}))
        d.in_class = bool(cls) and not qual
        d.init = tuple(after[after.index(':') + 1:]) if ':' in after else ()
        d.body = tuple(x for x, _ in t[ob + 1:close])
        d.guarded = guarded[t[ob][1] - 1] if t[ob][1] - 1 < len(guarded) else False
        d.unit = unit
        d.header = header
        return d

    scope(0, len(t), None)
    for d in defs:
        if d.name in protos and not d.owner:
            d.specs = tuple(sorted(set(d.specs) | {'_STATIC'}))
    return defs, bases


def is_helper(d):
    if d.header:
        return True
    if d.in_class:
        return True
    if not d.owner and ({'static', '_STATIC', 'inline', '__inline', '__forceinline'} & set(d.specs)):
        return True
    if d.owner and 'inline' in d.specs:
        return True
    return False


def collect(trees, header_only=False):
    """trees: [(base, rel_prefix)] in override order for headers (later wins by relative path)."""
    defs, bases = [], collections.defaultdict(list)
    files = {}
    for base, prefix in trees:
        for p in sorted(base.rglob('*')):
            if p.is_file() and p.suffix.lower() in HDR + SRC:
                files[str(p.relative_to(base))] = (p, prefix)
    for rel, (p, prefix) in files.items():
        hdr = p.suffix.lower() in HDR
        if header_only and not hdr:
            continue
        unit = None if hdr else Path(rel).with_suffix('').as_posix()
        dd, bb = scan(p, prefix + rel, unit, hdr)
        defs.extend(dd)
        for k, v in bb.items():
            bases[k].extend(v)
    return defs, bases


# ---------------------------------------------------------------- compare

def compare(o, t):
    diffs = []
    if o.in_class != t.in_class:
        diffs.append('ours in-class' if o.in_class else 'theirs in-class')
    if o.const != t.const:
        diffs.append('const ours' if o.const else 'const theirs')
    if o.params != t.params:
        diffs.append('params')
    so = set(o.specs) - {'virtual', 'friend', 'extern'}
    st = set(t.specs) - {'virtual', 'friend', 'extern'}
    if {'_STATIC'} & st:
        st = (st - {'_STATIC'}) | {'static'}
    if o.in_class or t.in_class:
        so -= {'inline'}; st -= {'inline'}
    if so != st:
        diffs.append('specs ours=%s theirs=%s' % ('+'.join(sorted(so)) or '-', '+'.join(sorted(st)) or '-'))
    if o.init != t.init:
        diffs.append('init')
    if o.body != t.body:
        diffs.append('body')
    return diffs


def sim(a, b):
    return difflib.SequenceMatcher(None, a, b, autojunk=False).ratio()


def read_csv(p):
    with open(p, newline='') as f:
        return list(csv.DictReader(f))


def row_class(fn):
    m = re.match(r'\?[^@]+@(\w+)@@', fn)
    if m and not fn.startswith('??'):
        return m.group(1)
    m = re.match(r'\?\?[0-9A-Z_]\w*?@?(\w+)@@', fn)
    if fn.startswith('??0') or fn.startswith('??1'):
        m = re.match(r'\?\?[01](\w+)@@', fn)
        return m.group(1) if m else ''
    if '::' in fn:
        return fn.split('::')[0]
    return ''


def ancestors(cls, bases, seen=None):
    seen = set() if seen is None else seen
    for c in [cls]:
        if c in seen or not c:
            continue
        seen.add(c)
        for b in bases.get(c, ()):
            ancestors(b, bases, seen)
    return seen


def calls(body):
    """names called in a token list: identifier followed by '(' (not a declaration keyword)."""
    return set(n for n, _ in call_args(body))


def call_args(body):
    """{(name, argument count)} for every identifier followed by '('."""
    out = set()
    for k in range(len(body) - 1):
        if body[k + 1] == '(' and re.match(r'[A-Za-z_~]\w*$', body[k]) and body[k] not in KEYWORDS:
            d, n, any_tok = 0, 0, False
            for j in range(k + 1, len(body)):
                x = body[j]
                if x in '([{':
                    d += 1
                    if d == 1:
                        continue
                elif x in ')]}':
                    d -= 1
                    if d == 0:
                        break
                if d == 1 and x == ',':
                    n += 1
                elif d >= 1:
                    any_tok = True
            out.add((body[k], n + 1 if any_tok else 0))
    return out


def arity_ok(t, n):
    if n is None:
        return True
    return t.nreq <= n <= len(t.params)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--write', action='store_true')
    ap.add_argument('--top', type=int, default=25)
    args = ap.parse_args()

    sys.path.insert(0, str(TOOLS))
    import twinmap as TM

    # theirs: TibSun headers, then their include/ (by relative path, as their build copies them), and their src/
    th_defs, th_bases = collect([(TIBSUN, 'TibSun/'), (THEIRS / 'include', 'include/')], header_only=True)
    ts_defs, ts_bases = collect([(THEIRS / 'src', 'src/')])
    o_defs, o_bases = collect([(ROOT / 'include', 'include/'), (ROOT / 'src', 'src/')])
    bases = collections.defaultdict(list)
    for b in (th_bases, ts_bases, o_bases):
        for k, v in b.items():
            bases[k].extend(v)
    theirs = [d for d in th_defs + ts_defs if is_helper(d)]
    ours = [d for d in o_defs if is_helper(d) and not d.guarded]
    ours_all = collections.defaultdict(list)
    for d in o_defs:
        if not d.guarded:
            ours_all[d.key()].append(d)
    their_all = collections.defaultdict(list)
    for d in th_defs + ts_defs:
        their_all[d.name].append(d)

    # their helpers by key; a src helper is keyed with its unit (file-local)
    tkeys = collections.OrderedDict()
    for d in theirs:
        k = (d.owner, d.name, d.params, d.unit if (not d.header and not d.owner) else None)
        tkeys.setdefault(k, d)
    okeys = collections.defaultdict(list)
    for d in ours:
        okeys[(d.owner, d.name)].append(d)

    entries = []
    for (owner, name, params, unit), t in tkeys.items():
        cand = okeys.get((owner, name), []) or [d for d in ours_all.get((owner, name), [])]
        if unit:
            cand = [d for d in cand if d.unit == Path(unit).name or d.unit == unit] or cand
        if not cand:
            entries.append(dict(t=t, o=None, status='absent', diffs='', sim=''))
            continue
        best = max(cand, key=lambda o: (o.params == t.params, sim(o.body, t.body)))
        df = compare(best, t)
        if not is_helper(best):
            df.insert(0, 'ours out-of-line (not inline)')
        entries.append(dict(t=t, o=best, status='same' if not df else 'different', diffs='; '.join(df),
                            sim='%.3f' % sim(best.body, t.body)))
    by_name = collections.defaultdict(list)
    for e in entries:
        by_name[e['t'].name].append(e)

    # parked rows with a matched twin body
    tail = read_csv(MANIFEST / 'tail_buckets.csv')
    parked = {(r['unit'], r['entry']): r for r in tail}
    twins = {(r['unit'], r['entry']): r for r in read_csv(MANIFEST / 'tail_twins.csv')}
    rows = []
    for k, r in parked.items():
        tw = twins.get(k)
        if not tw or tw.get('matched') != 'yes' or not tw.get('their_path'):
            continue
        body = None
        if tw.get('their_text') == 'body':
            _, txt = TM.their_text(tw['their_path'], int(tw['yr_entry'], 16))
            body = TM.tokens(txt) if txt else None
        rows.append(dict(r=r, tw=tw, body=body))

    tdefs_by_key = collections.defaultdict(list)
    for d in theirs:
        tdefs_by_key[d.name].append(d)

    for row in rows:
        r, tw, body = row['r'], row['tw'], row['body']
        if body is None:
            # a header-defined twin: the row's own entry
            nm = re.sub(r'^.*::', '', r['function']) if '::' in r['function'] else None
            row['direct'] = set()
            continue
        cls = row_class(r['function'])
        named = {x for x in body if re.match(r'[A-Z]\w*$', x) and x in bases or x.endswith('Class') or x.endswith('Struct')}
        owners = set()
        for c in {cls} | named:
            owners |= ancestors(c, bases)
        owners |= {'Coord', 'Cell', 'Point2D', 'Point3D', 'Rect', 'TPoint2D', 'TPoint3D', 'TRect', 'TVelocity3D'} & set(body)
        seen, todo, hits = set(), list(call_args(body)), set()
        depth = 0
        while todo and depth < 4:
            nxt = []
            for n, na in todo:
                if (n, na) in seen:
                    continue
                seen.add((n, na))
                for e in by_name.get(n, ()):
                    t = e['t']
                    if t.owner and t.owner not in owners and t.owner != n:
                        continue
                    if not arity_ok(t, na):
                        continue
                    if not t.header and not t.owner and t.unit and Path(tw['their_path']).with_suffix('').as_posix() != 'src/' + t.unit \
                            and Path(tw['their_path']).with_suffix('').as_posix().split('/', 1)[-1] != t.unit:
                        continue
                    hits.add(id(e))
                    nxt.extend(call_args(t.body))
                    nxt.extend(call_args(t.init))
            todo = nxt
            depth += 1
        row['direct'] = hits

    eid = {id(e): e for e in entries}
    for e in entries:
        e['rows'] = []
    for row in rows:
        for h in row.get('direct', ()):
            eid[h]['rows'].append(row)
    # the row's own entry when the twin is header-defined: match by name
    for row in rows:
        if row['body'] is None:
            fn = row['r']['function']
            m = re.match(r'\?(\w+)@(\w+)@@', fn)
            if m:
                for e in by_name.get(m.group(1), ()):
                    if e['t'].owner == m.group(2) or not e['t'].owner:
                        e['rows'].append(row)

    out = []
    for e in entries:
        t, o = e['t'], e['o']
        rs = sorted({id(x): x for x in e['rows']}.values(), key=lambda x: -int(x['r']['bytes']))
        pb = sum(int(x['r']['bytes']) for x in rs)
        ours_call = 0
        for x in rs:
            src = ROOT / 'src' / (x['r']['unit'] + '.cpp')
        out.append(dict(section='helper', owner=t.owner, name=t.name, params=', '.join(t.params),
                        status=e['status'], diffs=e['diffs'], body_sim=e['sim'],
                        theirs=t.where(), ours=o.where() if o else '',
                        their_kind='file-local' if (not t.header and not t.owner) else ('in-class' if t.in_class else 'header'),
                        parked_rows=len(rs), parked_bytes=pb,
                        rows=' '.join('%s:%s' % (x['r']['unit'], x['r']['entry']) for x in rs[:8])))

    # ---- per row: the twin's helpers that ours lacks or spells differently
    for row in rows:
        r = row['r']
        miss = [e for e in entries if row in e['rows'] and e['status'] != 'same']
        if not miss:
            continue
        out.append(dict(section='row', owner=r['unit'], name=short_name(r['function']), params='', status=r['bucket'],
                        diffs=' '.join('%s%s%s[%s]' % (e['t'].owner, '::' if e['t'].owner else '', e['t'].name, e['status'][0])
                                       for e in miss),
                        body_sim='', their_kind=row['tw']['twin_class'],
                        theirs='%s:%s' % (row['tw']['their_path'], row['tw']['their_line']), ours='',
                        parked_rows=1, parked_bytes=int(r['bytes']), rows='%s:%s' % (r['unit'], r['entry'])))

    # ---- ctor forms
    parked_classes = collections.defaultdict(list)
    for r in tail:
        c = row_class(r['function'])
        if c:
            parked_classes[c].append(r)
    octors = collections.defaultdict(list)
    tctors = collections.defaultdict(list)
    for d in o_defs:
        if not d.guarded and d.name == d.owner and d.owner:
            octors[d.owner].append(d)
    for d in th_defs + ts_defs:
        if d.name == d.owner and d.owner:
            tctors[d.owner].append(d)
    for c in sorted(parked_classes):
        pb = sum(int(r['bytes']) for r in parked_classes[c])
        tl = {d.params: d for d in tctors.get(c, ())}
        ol = {d.params: d for d in octors.get(c, ())}
        for p in sorted(set(tl) | set(ol)):
            a, b = ol.get(p), tl.get(p)
            if a and b:
                df = []
                if a.init != b.init:
                    df.append('init ours=[%s] theirs=[%s]' % (' '.join(a.init), ' '.join(b.init)))
                if a.body != b.body:
                    df.append('body')
                if a.in_class != b.in_class:
                    df.append('ours in-class' if a.in_class else 'theirs in-class')
                st = 'same' if not df else 'different'
            elif a:
                st, df = 'ours only', ['explicit ctor ours only (%s)' % ('empty' if not a.body and not a.init else 'with body')]
            else:
                st, df = 'absent', ['theirs only (%s)' % ('empty' if not b.body and not b.init else 'with body')]
            out.append(dict(section='ctor', owner=c, name=c, params=', '.join(p), status=st, diffs='; '.join(df),
                            body_sim='', theirs=b.where() if b else '', ours=a.where() if a else '', their_kind='',
                            parked_rows=len(parked_classes[c]), parked_bytes=pb,
                            rows=' '.join('%s:%s' % (r['unit'], r['entry']) for r in parked_classes[c][:8])))

    # ---- spell-outs: statement-level
    known_theirs = {d.name for d in th_defs + ts_defs}
    ours_names = {d.name for d in o_defs if not d.guarded}
    for row in rows:
        if row['body'] is None:
            continue
        r = row['r']
        name = row['tw'].get('their_symbol') or ''
        our = TM.our_texts(r['unit'], short_name(r['function']))
        if not our:
            continue
        line, otext = max(our, key=lambda x: sim(TM.tokens(x[1]), row['body']))
        a = statements(TM.tokens(otext))
        b = statements(row['body'])
        sm = difflib.SequenceMatcher(None, [tuple(s) for s in a], [tuple(s) for s in b], autojunk=False)
        for op, i1, i2, j1, j2 in sm.get_opcodes():
            if op == 'equal':
                continue
            oc = set().union(*[calls(s) for s in a[i1:i2]]) if i2 > i1 else set()
            tc = set().union(*[calls(s) for s in b[j1:j2]]) if j2 > j1 else set()
            extra = sorted(n for n in tc - oc if n in known_theirs and n not in KEYWORDS)
            if not extra:
                continue
            for n in extra:
                out.append(dict(section='spellout', owner=r['unit'], name=n, params='',
                                status='callee in ours' if n in ours_names else 'callee absent',
                                diffs='ours: %s | theirs: %s' % (clip(a[i1:i2]), clip(b[j1:j2])), body_sim='',
                                theirs='%s:%s' % (row['tw']['their_path'], row['tw']['their_line']),
                                ours='src/%s.cpp:%d' % (r['unit'], line), their_kind=op,
                                parked_rows=1, parked_bytes=int(r['bytes']),
                                rows='%s:%s %s' % (r['unit'], r['entry'], short_name(r['function']))))

    # ---- include lists
    paths = collections.Counter()
    for k, tw in twins.items():
        if tw.get('their_path'):
            paths[(k[0], tw['their_path'])] += 1
    unit_path = {}
    for (u, p), n in paths.most_common():
        unit_path.setdefault(u, p)
    for u in sorted({k[0] for k in parked}):
        p = unit_path.get(u)
        ours_p = ROOT / 'src' / (u + '.cpp')
        if not p or not (THEIRS / p).exists() or not ours_p.exists():
            continue
        oi = includes(ours_p.read_text(errors='replace'))
        ti = includes((THEIRS / p).read_text(errors='replace'))
        pb = sum(int(r['bytes']) for r in tail if r['unit'] == u)
        if [x.lower() for x in oi] == [x.lower() for x in ti]:
            st, df = 'same', ''
        else:
            so, stt = {x.lower() for x in oi}, {x.lower() for x in ti}
            st = 'same set, order differs' if so == stt else 'different'
            df = 'ours only: %s; theirs only: %s' % (' '.join(sorted(so - stt)) or '-', ' '.join(sorted(stt - so)) or '-')
        out.append(dict(section='include', owner=u, name=p, params='', status=st, diffs=df, body_sim='',
                        theirs=' '.join(ti), ours=' '.join(oi), their_kind='', parked_rows=sum(1 for r in tail if r['unit'] == u),
                        parked_bytes=pb, rows=''))

    order = {'helper': 0, 'row': 1, 'ctor': 2, 'spellout': 3, 'include': 4}
    out.sort(key=lambda x: (order[x['section']], x['status'] == 'same', -x['parked_bytes'], x['owner'], x['name']))
    cols = ['section', 'owner', 'name', 'params', 'status', 'diffs', 'body_sim', 'their_kind', 'theirs', 'ours',
            'parked_rows', 'parked_bytes', 'rows']
    summary(out, args.top)
    if args.write:
        with open(OUT, 'w', newline='') as f:
            w = csv.DictWriter(f, fieldnames=cols, lineterminator='\n')
            w.writeheader()
            for x in out:
                if x['section'] == 'helper' and x['status'] == 'absent' and not x['parked_rows']:
                    continue        # YR-only helpers no parked row's twin calls: counted, not listed
                w.writerow(x)
        print('wrote', OUT.relative_to(ROOT))


def short_name(fn):
    m = re.match(r'\?(\w+)@(\w+)@@', fn)
    if fn.startswith('??0'):
        m2 = re.match(r'\?\?0(\w+)@@', fn)
        return '%s::%s' % (m2.group(1), m2.group(1)) if m2 else fn
    if m:
        return '%s::%s' % (m.group(2), m.group(1))
    m = re.match(r'\?(\w+)@@', fn)
    return m.group(1) if m else fn


def statements(tk):
    out, cur, d = [], [], 0
    for x in tk:
        if x == '(':
            d += 1
        elif x == ')':
            d -= 1
        cur.append(x)
        if d == 0 and x in (';', '{', '}'):
            out.append(cur); cur = []
    if cur:
        out.append(cur)
    return out


def clip(sts, n=160):
    s = ' '.join(' '.join(x) for x in sts)
    return s if len(s) <= n else s[:n] + ' ...'


INCLUDE = re.compile(r'^\s*#\s*include\s*[<"]([^>"]+)[>"]', re.M)


def includes(text):
    return INCLUDE.findall(COMMENT.sub('', text))


def summary(out, top):
    h = [x for x in out if x['section'] == 'helper']
    c = collections.Counter(x['status'] for x in h)
    print('helpers (theirs, keyed by owner/name/params): same %d, different %d, absent %d' % (
        c['same'], c['different'], c['absent']))
    for st in ('different', 'absent'):
        sel = [x for x in h if x['status'] == st and x['parked_rows']]
        print('  %s with parked callers: %d entries, rows %d' % (st, len(sel), len({r for x in sel for r in x['rows'].split()})))
    ct = [x for x in out if x['section'] == 'ctor']
    print('ctor forms:', dict(collections.Counter(x['status'] for x in ct)))
    sp = [x for x in out if x['section'] == 'spellout']
    print('spell-outs: %d (callee in ours %d) over %d rows' % (
        len(sp), sum(1 for x in sp if x['status'] == 'callee in ours'), len({x['rows'] for x in sp})))
    inc = [x for x in out if x['section'] == 'include']
    print('include lists:', dict(collections.Counter(x['status'] for x in inc)))
    print('top adoption list (helpers different/absent by parked bytes):')
    sel = sorted([x for x in h if x['status'] != 'same' and x['parked_rows']], key=lambda x: -x['parked_bytes'])
    for x in sel[:top]:
        print('  %7d B %3d rows  %-9s %s::%s(%s)  %s  [%s]' % (
            x['parked_bytes'], x['parked_rows'], x['status'], x['owner'], x['name'], x['params'][:40], x['diffs'][:70], x['theirs']))


if __name__ == '__main__':
    main()
