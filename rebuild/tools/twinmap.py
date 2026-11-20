#!/usr/bin/env python3
"""twinmap.py [--write]: Yuri's Revenge twins of the rebuild's parked tail (lane L2, 2026-11-19).

For every row of rebuild/manifest/tail_buckets.csv (tailmap.py) this looks for a twin body in the retail
gamemd.exe 1.001 (SHA-256 7e839d3b...6077) and says whether gibbo101/ra2-decomp has matched it, without
compiling anything. Measurement only: nothing is adopted.

Bodies are compared as the 2026-11-06 and Scout808 (2026-11-14) measurements did: each body is decoded on
its own (capstone, linear); the rel32 field of call/jmp/jcc and every 4-byte immediate or displacement whose
value lies inside that image are masked. Identical = same length, same bytes outside both masks (a second
pass lets a byte differ inside a dword that is in-image in both bodies: switch tables decoded as code).
Near twins: (layout) same length and instruction boundaries, only immediate/displacement fields differ; or
(fuzzy) length within 10%, mnemonic-sequence ratio >= 0.90. Their score is a token similarity over the
masked instruction text (1.0 for identical). A row with several identical YR candidates takes the one that
lies between the YR twins of its neighbouring anchors (identical twins of every units.json row with one
candidate, kept as a monotonic chain), else the first.

Matched = the YR address is in ra2-decomp's data/manifest.csv; candidates come from data/functions.csv
(every YR function with its size). Classes: identical_matched, near_matched, identical_unmatched,
near_unmatched, ra2_only.

Text: their text is read at the `// FUNCTION|SYNTHETIC|TEMPLATE: GAMEMD 0x...` annotation of their_path
(a body when code follows; "header" when the annotation only names a function defined elsewhere). Ours is
the definition of the row's name in rebuild/src/<unit>.cpp (any guard state; the closest overload by text).
text_sim is a token ratio over both bodies with comments and spaces removed; text_same says "same" when the
token lists are equal.

--write writes rebuild/manifest/tail_twins.csv (one line per tail row) and rebuild/manifest/twin_queue.csv
(rows with a matched twin, by bytes) and prints the split. The YR executable is looked up in $YR_GAMEMD,
the user's installation and ra2-decomp work clones; it is read only.
"""
import bisect
import collections
import csv
import difflib
import hashlib
import json
import os
import re
import struct
import sys
from pathlib import Path

import capstone
from capstone import x86_const as X

TOOLS = Path(__file__).resolve().parent
REBUILD = TOOLS.parent
ROOT = REBUILD.parent
MANIFEST = REBUILD / 'manifest'
sys.path.insert(0, str(TOOLS))
from guard import demangle  # noqa: E402

RA2 = ROOT / 'private/original-target-20261020/inputs/game.exe'
RA2_SHA = 'e6418111c2c36081a1795fb000e0927beea1630170b334c5013d12102602e70f'
YR_SHA = '7e839d3bb42f2989cea5d01884fb340d26a09f9184d47453943d9552852b6077'
YR_PLACES = [os.environ.get('YR_GAMEMD', ''), '/Volumes/Lesalia/Command and Conquer Red Alert II/gamemd.exe',
             str(Path.home() / 'Code/ra2-decomp-work/orig/gamemd.exe'),
             str(ROOT / 'private/reference/ra2-decomp/orig/gamemd.exe')]
THEIRS = ROOT / 'private/reference/ra2-decomp'


class Image:
    def __init__(self, path, sha):
        data = Path(path).read_bytes()
        if hashlib.sha256(data).hexdigest() != sha:
            raise SystemExit('hash mismatch ' + str(path))
        self.data = data
        pe = struct.unpack_from('<I', data, 0x3c)[0]
        n = struct.unpack_from('<H', data, pe + 6)[0]
        opt = struct.unpack_from('<H', data, pe + 20)[0]
        self.base = struct.unpack_from('<I', data, pe + 24 + 28)[0]
        self.size = struct.unpack_from('<I', data, pe + 24 + 56)[0]
        self.text = None
        for i in range(n):
            at = pe + 24 + opt + 40 * i
            vs, va, rs, ra = struct.unpack_from('<IIII', data, at + 8)
            if data[at:at + 8].rstrip(b'\0') == b'.text':
                self.text = (self.base + va, vs, rs, ra)

    def inrange(self, v):
        return self.base <= v < self.base + self.size

    def read(self, a, n):
        va, vs, rs, ra = self.text
        if n > 0 and va <= a and a + n <= va + min(vs, rs):
            return self.data[ra + a - va: ra + a - va + n]
        return None


MD = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
MD.detail = True
MD.skipdata = True
BRANCH = {X.X86_INS_CALL, X.X86_INS_JMP} | {getattr(X, 'X86_INS_' + j) for j in (
    'JA JAE JB JBE JE JG JGE JL JLE JNE JNO JNP JNS JO JP JS'.split())}
SHORT = {getattr(X, 'X86_INS_' + j) for j in ('JECXZ', 'LOOP', 'LOOPE', 'LOOPNE')}


class Body:
    __slots__ = ('addr', 'data', 'mask', 'mn', 'tok', 'key', 'ninsn', 'opf', 'starts', 'cmn')

    def __init__(self, img, addr, data):
        self.addr, self.data = addr, data
        mask = bytearray(len(data))
        opf = bytearray(len(data))
        starts, mn, tok = [], [], []
        for ins in MD.disasm(data, addr):
            off = ins.address - addr
            if ins.id == 0:
                mn.append('.byte'); tok.append('.byte'); continue
            text = ins.op_str
            imm_off, imm_sz = getattr(ins, 'imm_offset', 0), getattr(ins, 'imm_size', 0)
            disp_off, disp_sz = getattr(ins, 'disp_offset', 0), getattr(ins, 'disp_size', 0)
            if (ins.id in BRANCH or ins.id in SHORT) and ins.operands and ins.operands[0].type == X.X86_OP_IMM:
                if ins.size >= 5:
                    for k in range(ins.size - 4, ins.size):
                        mask[off + k] = 1
                text = 'REL'
            else:
                for fo, fs in ((imm_off, imm_sz), (disp_off, disp_sz)):
                    if fs == 4 and fo:
                        v = int.from_bytes(data[off + fo: off + fo + 4], 'little')
                        if img.inrange(v):
                            for k in range(4):
                                mask[off + fo + k] = 1
                            text = text.replace(hex(v), 'ADDR')
            starts.append((off, ins.size, ins.bytes[0]))
            for fo, fs in ((imm_off, imm_sz), (disp_off, disp_sz)):
                if fo and fs:
                    for k in range(fs):
                        if off + fo + k < len(data):
                            opf[off + fo + k] = 1
            mn.append(ins.mnemonic)
            tok.append(ins.mnemonic + ' ' + text)
        self.mask, self.mn, self.tok, self.ninsn = mask, mn, tok, len(mn)
        self.opf, self.starts = opf, starts
        self.key = bytes(0 if m else b for b, m in zip(data, mask))
        self.cmn = collections.Counter(mn)


def equiv(a, b, ia, ib):
    if len(a.data) != len(b.data):
        return None
    if a.key == b.key and a.mask == b.mask:
        return 'insn'
    da, db = a.data, b.data
    how = 'insn'
    for p in range(len(da)):
        if da[p] == db[p] or (a.mask[p] and b.mask[p]):
            continue
        for k in range(max(0, p - 3), min(p, len(da) - 4) + 1):
            if ia.inrange(int.from_bytes(da[k:k + 4], 'little')) and ib.inrange(int.from_bytes(db[k:k + 4], 'little')):
                break
        else:
            return None
        how = 'table'
    return how


def layout_equiv(a, b):
    if len(a.data) != len(b.data) or a.starts != b.starts:
        return None
    fields = set()
    for p in range(len(a.data)):
        if a.data[p] == b.data[p] or (a.mask[p] and b.mask[p]):
            continue
        if not a.opf[p]:
            return None
        fields.add(p >> 2)
    return fields


def tok_sim(a, b):
    return difflib.SequenceMatcher(None, a.tok, b.tok, autojunk=False).ratio()


# ---------------------------------------------------------------- source text

COMMENT = re.compile(r'//[^\n]*|/\*.*?\*/', re.S)
TOKEN = re.compile(r'[A-Za-z_]\w*|0[xX][0-9a-fA-F]+|\d+\.?\d*[fFlLuU]*|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|\S')
ANNOT = re.compile(r'^// (FUNCTION|SYNTHETIC|TEMPLATE|LIBRARY): GAMEMD (0x[0-9A-Fa-f]+)', re.M)


def braced(text, i):
    """End offset of the {...} block starting at the first '{' at or after i (strings/comments skipped)."""
    j = text.find('{', i)
    if j < 0:
        return None
    depth, k = 0, j
    while k < len(text):
        c = text[k]
        if c in '"\'':
            q = c; k += 1
            while k < len(text) and text[k] != q:
                k += 2 if text[k] == '\\' else 1
        elif text.startswith('//', k):
            k = text.find('\n', k)
            if k < 0:
                return None
        elif text.startswith('/*', k):
            k = text.find('*/', k) + 1
        elif c == '{':
            depth += 1
        elif c == '}':
            depth -= 1
            if depth == 0:
                return k + 1
        k += 1
    return None


def tokens(text):
    return TOKEN.findall(COMMENT.sub(' ', text))


_their_cache = {}


def their_annotations(path):
    if path not in _their_cache:
        p = THEIRS / path
        text = p.read_text(errors='replace') if p.exists() else ''
        _their_cache[path] = (text, {int(m.group(2), 16): m for m in ANNOT.finditer(text)})
    return _their_cache[path]


def their_text(path, addr):
    """(line, text or None) of their definition at the annotation for addr."""
    text, ann = their_annotations(path)
    m = ann.get(addr)
    if not m:
        return None, None
    line = text.count('\n', 0, m.start()) + 1
    k = text.find('\n', m.start()) + 1
    # skip the comment lines that follow the annotation (symbol / name)
    while text.startswith('//', k):
        k = text.find('\n', k) + 1
    nxt = ANNOT.search(text, k)
    rest = text[k: nxt.start() if nxt else len(text)]
    if m.group(1) in ('SYNTHETIC', 'TEMPLATE') or '{' not in rest:
        return line, None
    head = rest.split('{', 1)[0]
    if ';' in head.replace('::', ''):    # a declaration-only gap: the body is elsewhere
        return line, None
    end = braced(text, k)
    return line, text[k:end] if end else None


_our_cache = {}


def our_texts(unit, name):
    """Every definition of Class::Name (or Name) at file scope in rebuild/src/<unit>.cpp."""
    p = REBUILD / 'src' / (unit + '.cpp')
    if p not in _our_cache:
        _our_cache[p] = p.read_text(errors='replace') if p.exists() else ''
    text = _our_cache[p]
    if not name:
        return []
    out = []
    pat = re.compile(r'^[^\s#/][^;{}\n]*?(?<![\w:])' + re.escape(name) + r'\s*\(', re.M)
    for m in pat.finditer(text):
        end = braced(text, m.start())
        head = text[m.start(): text.find('{', m.start())] if end else ''
        if not end or ';' in COMMENT.sub(' ', head):
            continue
        line = text.count('\n', 0, m.start()) + 1
        out.append((line, text[m.start():end]))
    return out


def text_compare(ours, theirs):
    a, b = tokens(ours), tokens(theirs)
    if a == b:
        return 'same', 1.0
    return 'differs', difflib.SequenceMatcher(None, a, b, autojunk=False).ratio()


# ---------------------------------------------------------------- main

def read_csv(path):
    with open(path, newline='') as f:
        return list(csv.DictReader(f))


def find_yr():
    for p in YR_PLACES:
        if p and Path(p).exists():
            return p
    raise SystemExit('gamemd.exe 1.001 not found; set YR_GAMEMD')


def main(write):
    ra2, yr = Image(RA2, RA2_SHA), Image(find_yr(), YR_SHA)
    man = {int(r['address'], 16): (int(r['size']), r['symbol'], r['source'])
           for r in read_csv(THEIRS / 'data/manifest.csv')}
    fl = {int(r['address'], 16): (int(r['size']), r['name']) for r in read_csv(THEIRS / 'data/functions.csv')}
    for a, (n, sym, src) in man.items():
        fl.setdefault(a, (n, sym))
    yb = {}
    for a, (n, _) in fl.items():
        d = yr.read(a, n)
        if d:
            yb[a] = Body(yr, a, d)
    by_len = collections.defaultdict(list)
    by_key = collections.defaultdict(list)
    for a, b in yb.items():
        by_len[len(b.data)].append(a)
        by_key[b.key].append(a)
    print('YR bodies', len(yb), 'matched', sum(1 for a in yb if a in man), flush=True)

    def identical(b):
        hits = [a for a in by_key.get(b.key, ()) if equiv(b, yb[a], ra2, yr)]
        if not hits:
            hits = [a for a in by_len.get(len(b.data), ()) if equiv(b, yb[a], ra2, yr)]
        return hits

    # anchors: identical twins with a single candidate over every units.json row
    units = json.load(open(MANIFEST / 'units.json'))
    anchors = []
    for u, info in units.items():
        for r in info.get('rows', ()):
            e, n = int(r['entry'], 16), int(r.get('body') or 0)
            if n < 16:
                continue
            d = ra2.read(e, n)
            if d:
                h = identical(Body(ra2, e, d))
                if len(h) == 1:
                    anchors.append((e, h[0]))
    anchors.sort()
    tails, ty, prev = [], [], [None] * len(anchors)
    for i, (_, y) in enumerate(anchors):
        j = bisect.bisect_left(ty, y)
        if j == len(tails):
            tails.append(i); ty.append(y)
        else:
            tails[j] = i; ty[j] = y
        prev[i] = tails[j - 1] if j else None
    chain, i = [], tails[-1] if tails else None
    while i is not None:
        chain.append(anchors[i]); i = prev[i]
    chain.reverse()
    ax = [c[0] for c in chain]
    print('anchors', len(chain), 'of', len(anchors), flush=True)

    def window(e):
        i = bisect.bisect_left(ax, e)
        return (chain[i - 1][1] if i else 0), (chain[i][1] if i < len(chain) else 1 << 32)

    hist = {(r['unit'], r['entry']): r for r in read_csv(MANIFEST / 'tail_history.csv')}
    out = []
    for r in read_csv(MANIFEST / 'tail_buckets.csv'):
        e, n = int(r['entry'], 16), int(r['bytes'])
        h = hist.get((r['unit'], r['entry']), {})
        row = dict(unit=r['unit'], entry=r['entry'], function=r['function'], bytes=n, bucket=r['bucket'],
                   difficulty=h.get('difficulty', ''), twin_class='ra2_only', yr_entry='', yr_bytes='',
                   how='', score='', candidates=0, in_order='', matched='', their_symbol='', their_path='',
                   their_line='', their_text='', our_line='', text_same='', text_sim='')
        d = ra2.read(e, n)
        out.append(row)
        if not d:
            row['how'] = 'unreadable'
            continue
        b = Body(ra2, e, d)
        lo, hi = window(e)
        hits = identical(b)
        pick = None
        if hits:
            hits.sort(key=lambda a: (not (lo <= a <= hi), a not in man))
            pick, how, score, ncand = hits[0], 'identical', 1.0, len(hits)
        else:
            lay = []
            for a in by_len.get(n, ()):
                f = layout_equiv(b, yb[a])
                if f is not None:
                    lay.append((len(f), a))
            cands = []
            if lay:
                for nf, a in lay:
                    cands.append((tok_sim(b, yb[a]), a, 'layout (%d field%s)' % (nf, '' if nf == 1 else 's')))
            elif n >= 16:
                lo_n, hi_n = int(n * 0.9), int(n * 1.1) + 1
                for L in range(lo_n, hi_n + 1):
                    for a in by_len.get(L, ()):
                        y = yb[a]
                        if abs(y.ninsn - b.ninsn) > max(2, b.ninsn // 10):
                            continue
                        if sum((b.cmn & y.cmn).values()) / max(b.ninsn, y.ninsn) < 0.88:
                            continue
                        mr = difflib.SequenceMatcher(None, b.mn, y.mn, autojunk=False).ratio()
                        if mr >= 0.90:
                            cands.append((tok_sim(b, y), a, 'fuzzy (mnemonics %.3f)' % mr))
            if cands:
                cands.sort(key=lambda c: (lo <= c[1] <= hi, c[0], c[1] in man), reverse=True)
                score, pick, how = cands[0]
                ncand = len(cands)
        if pick is None:
            continue
        matched = pick in man
        row.update(yr_entry='%08x' % pick, yr_bytes=len(yb[pick].data), how=how, score='%.4f' % score,
                   candidates=ncand, in_order='yes' if lo <= pick <= hi else 'no', matched='yes' if matched else 'no',
                   twin_class=('identical_' if how == 'identical' else 'near_') + ('matched' if matched else 'unmatched'),
                   their_symbol=man[pick][1] if matched else fl[pick][1])
        if matched:
            path = man[pick][2]
            line, tt = their_text(path, pick)
            row.update(their_path=path, their_line=line or '', their_text='body' if tt else 'header')
            name = demangle(r['function']) or r['function']
            mine = our_texts(r['unit'], name)
            if tt and mine:
                best = max(((text_compare(t, tt), ln) for ln, t in mine), key=lambda x: x[0][1])
                (same, sim), ln = best
                row.update(our_line=ln, text_same=same, text_sim='%.3f' % sim)
            elif mine:
                row.update(our_line=mine[0][0], text_same='no their body')
            else:
                row.update(text_same='not in unit source')
        if len(out) % 100 == 0:
            print('rows', len(out), flush=True)

    split(out)
    if write:
        cols = list(out[0])
        with open(MANIFEST / 'tail_twins.csv', 'w', newline='') as f:
            w = csv.DictWriter(f, cols, lineterminator='\n')
            w.writeheader()
            w.writerows(out)
        q = [r for r in out if r['matched'] == 'yes']
        q.sort(key=lambda r: (-r['bytes'], r['unit'], r['entry']))
        with open(MANIFEST / 'twin_queue.csv', 'w', newline='') as f:
            w = csv.writer(f, lineterminator='\n')
            w.writerow(['unit', 'function', 'entry', 'yr_entry', 'their_path', 'their_line', 'bytes', 'twin_class',
                        'score', 'bucket', 'difficulty', 'text_same', 'text_sim', 'action'])
            for r in q:
                if r['text_same'] == 'same':
                    act = 'text is already the twin\'s: adopt its header forms (twin_header_diffs.csv)'
                elif r['their_text'] == 'header':
                    act = 'twin defined in their headers or compiler-generated: adopt the header form'
                else:
                    act = 'adopt the twin\'s text and its header forms'
                w.writerow([r['unit'], r['function'], r['entry'], r['yr_entry'], r['their_path'], r['their_line'],
                            r['bytes'], r['twin_class'], r['score'], r['bucket'], r['difficulty'], r['text_same'],
                            r['text_sim'], act])
        print('wrote tail_twins.csv and twin_queue.csv (%d queued)' % len(q))


def split(out):
    classes = ['identical_matched', 'near_matched', 'identical_unmatched', 'near_unmatched', 'ra2_only']
    for title, rows in (('all parked', out), ('hard on main', [r for r in out if r['difficulty'] == 'hard on main'])):
        print('%s: %d rows, %d B' % (title, len(rows), sum(r['bytes'] for r in rows)))
        for c in classes:
            sel = [r for r in rows if r['twin_class'] == c]
            print('  %-20s %5d rows %8d B' % (c, len(sel), sum(r['bytes'] for r in sel)))
    sel = [r for r in out if r['twin_class'] == 'identical_matched']
    tc = collections.Counter(r['text_same'] or '-' for r in sel)
    print('identical_matched text:', dict(tc))



# ---------------------------------------------------------------- headers (--headers)

TIBSUN = ROOT / 'private/reference/TibSun/code'   # ra2-decomp's pinned archive (dbfbfbf): their base header set
CLASSDEF = re.compile(r'(?<![\w<])(?:class|struct)\s+(?:__declspec\([^)]*\)\s*)?(\w+)\s*(?::[^{;()]*)?\{')
ACCESS = re.compile(r'^(?:public|private|protected)\s*:')


def header_files(trees):
    """basename -> path, later trees overriding earlier ones (their tools copy TibSun, then include/)."""
    files = {}
    for base in trees:
        for h in sorted(base.rglob('*')):
            if h.is_file() and h.suffix.lower() in ('.h', '.hh', '.hpp', '.inl'):
                files[str(h.relative_to(base))] = h
    return files


def class_index(files):
    idx = {}
    for rel, h in files.items():
        text = h.read_text(errors='replace')
        clean = COMMENT.sub(lambda m: ' ' * len(m.group(0)), text)
        for m in CLASSDEF.finditer(clean):
            name = m.group(1)
            end = braced(clean, m.end() - 1)
            if not end:
                continue
            names = [name]
            alias = re.match(r'\s*(\w+)\s*[;,]', clean[end:])
            if alias and 'typedef' in clean[max(0, m.start() - 20):m.start()]:
                names.append(alias.group(1))
            for n in names:
                if n not in idx or len(clean[m.end():end]) > len(idx[n][1]):
                    idx[n] = (rel, clean[m.end():end - 1], m.group(0))
    return idx


def members(body):
    """Top-level member declarations of a class body (inline bodies and nested types kept whole)."""
    out, depth, cur, k = [], 0, [], 0
    while k < len(body):
        c = body[k]
        cur.append(c)
        if c in '{(':
            depth += 1
        elif c in '})':
            depth -= 1
            if depth == 0 and c == '}':
                rest = body[k + 1:].lstrip()
                if not rest.startswith(';') and not re.match(r'\w', rest):
                    out.append(''.join(cur)); cur = []
        elif c == ';' and depth == 0:
            out.append(''.join(cur)); cur = []
        k += 1
    res = []
    for m in out:
        m = ' '.join(m.split())
        while True:
            a = ACCESS.match(m)
            if not a:
                break
            m = m[a.end():].strip()
        if m and m != ';':
            res.append(m)
    return res


def classify(m):
    head = m.split('{', 1)[0]
    t = tokens(head)
    if not t or t[0] in ('typedef', 'enum', 'friend', 'class', 'struct', 'union', 'using'):
        return 'type', t[1] if len(t) > 1 else '', m
    fm = re.search(r'(~?\w+|operator\s*[^\s(]+)\s*\((?!\s*\*)', head)
    if fm and not re.search(r'\(\s*\*', head.split(fm.group(0))[0]):
        kind = 'inline' if '{' in m else 'method'
        return kind, fm.group(1).replace(' ', ''), m
    kind = 'static' if t[0] == 'static' else 'data'
    return kind, (re.findall(r'\w+', head.split('[')[0].split(':')[0]) or [''])[-1], m


def data_sig(m):
    """Type of a data member: its tokens without the declarator name (arrays and bit widths kept)."""
    t = tokens(m.rstrip(';'))
    names = [i for i, x in enumerate(t) if re.match(r'[A-Za-z_]\w*$', x)]
    for i in reversed(names):
        if i + 1 == len(t) or t[i + 1] in ('[', ':', '=', ','):
            del t[i]
            break
    return ' '.join(t)


def class_diff(ours, theirs):
    om, tm = [classify(m) for m in members(ours)], [classify(m) for m in members(theirs)]
    od = [data_sig(m) for k, _, m in om if k == 'data']
    td = [data_sig(m) for k, _, m in tm if k == 'data']
    layout = []
    for op, i1, i2, j1, j2 in difflib.SequenceMatcher(None, od, td, autojunk=False).get_opcodes():
        if op != 'equal':
            layout.append('%s ours[%d:%d]=%s theirs[%d:%d]=%s' % (op, i1, i2, '|'.join(od[i1:i2])[:80], j1, j2,
                                                                 '|'.join(td[j1:j2])[:80]))
    ofn = [n for k, n, _ in om if k in ('method', 'inline')]
    tfn = [n for k, n, _ in tm if k in ('method', 'inline')]
    only_o = sorted(set(ofn) - set(tfn))
    only_t = sorted(set(tfn) - set(ofn))
    ov = [n for k, n, m in om if k in ('method', 'inline') and m.startswith('virtual')]
    tv = [n for k, n, m in tm if k in ('method', 'inline') and m.startswith('virtual')]
    oi = {n: tokens(m) for k, n, m in om if k == 'inline'}
    ti = {n: tokens(m) for k, n, m in tm if k == 'inline'}
    inline_o = sorted(n for n in oi if n not in ti and n in tfn)       # inline in ours, out of line in theirs
    inline_t = sorted(n for n in ti if n not in oi and n in ofn)
    inline_d = sorted(n for n in oi if n in ti and oi[n] != ti[n])
    return dict(data_ours=len(od), data_theirs=len(td), layout='same' if not layout else '; '.join(layout[:4]),
                decl_ours=len(om), decl_theirs=len(tm), methods_ours=len(ofn), methods_theirs=len(tfn),
                only_ours=' '.join(only_o[:12]) + (' ...' if len(only_o) > 12 else ''), n_only_ours=len(only_o),
                only_theirs=' '.join(only_t[:12]) + (' ...' if len(only_t) > 12 else ''), n_only_theirs=len(only_t),
                virtual_order='same' if ov == tv else '%d/%d, %d by position' % (
                    len(ov), len(tv), sum(1 for x, y in zip(ov, tv) if x != y) + abs(len(ov) - len(tv))),
                inline_ours_only=' '.join(inline_o[:8]), inline_theirs_only=' '.join(inline_t[:8]),
                inline_bodies_differ=' '.join(inline_d[:8]))


def used_as_type(toks, c):
    """False when every use of c in the token list is an object (c.x, c->x): the global Map, Scen, Options."""
    uses = [i for i, t in enumerate(toks) if t == c]
    return any(i + 1 >= len(toks) or toks[i + 1] not in ('.', '-') for i in uses)


def header_decl_diff(a, b):
    ta, tb = tokens(a), tokens(b)
    if ta == tb:
        return None
    da, db = count_decls(a), count_decls(b)
    oa = sorted(set(da) - set(db))
    ob = sorted(set(db) - set(da))
    return dict(decl_ours=len(da), decl_theirs=len(db), n_only_ours=len(oa), n_only_theirs=len(ob),
                only_ours=' '.join(oa[:12]) + (' ...' if len(oa) > 12 else ''),
                only_theirs=' '.join(ob[:12]) + (' ...' if len(ob) > 12 else ''),
                token_sim='%.3f' % difflib.SequenceMatcher(None, ta, tb, autojunk=False).ratio())


def headers(min_sim=0.95):
    """Rows with an identical matched twin whose text is already (nearly) the twin's: what differs between our
    header forms and theirs, for the classes the text names and the headers the unit includes directly."""
    rows = [r for r in read_csv(MANIFEST / 'tail_twins.csv') if r['twin_class'] == 'identical_matched'
            and r['text_sim'] and float(r['text_sim']) >= min_sim]
    ofiles = header_files([REBUILD / 'include'])
    tfiles = header_files([TIBSUN, THEIRS / 'include'])
    obase = {Path(k).name: v for k, v in ofiles.items()}
    tbase = {Path(k).name: v for k, v in tfiles.items()}
    ours, theirs = class_index(ofiles), class_index(tfiles)
    involved = collections.defaultdict(list)
    included = collections.defaultdict(list)
    for r in rows:
        name = demangle(r['function']) or ''
        toks = []
        for ln, t in our_texts(r['unit'], name):
            if str(ln) == r['our_line']:
                toks = tokens(t)
        cls = name.split('::')[-2] if '::' in name else None
        tag = '%s %s' % (r['unit'], r['entry'])
        for c in sorted(set(toks) | ({cls} if cls else set())):
            if c in ours and c in theirs and (c == cls or used_as_type(toks, c)):
                involved[c].append(tag)
        src = (REBUILD / 'src' / (r['unit'] + '.cpp')).read_text(errors='replace')
        for h in re.findall(r'^\s*#\s*include\s*"([^"]+)"', src, re.M):
            included[Path(h).name].append(tag)
    out = []
    for c, users in sorted(involved.items()):
        d = class_diff(ours[c][1], theirs[c][1])
        same = (d['layout'] == 'same' and not d['n_only_ours'] and not d['n_only_theirs'] and d['virtual_order'] == 'same'
                and not d['inline_ours_only'] and not d['inline_theirs_only'] and not d['inline_bodies_differ'])
        out.append(dict(kind='class', name=c, rows=' '.join(sorted(set(users))), our_header=ours[c][0],
                        their_header=theirs[c][0],
                        their_base='ra2-decomp' if (THEIRS / 'include' / theirs[c][0]).exists() else 'TibSun',
                        status='same' if same else 'differs', **d))
    for h, users in sorted(included.items()):
        if h not in obase or h not in tbase:
            continue
        d = header_decl_diff(obase[h].read_text(errors='replace'), tbase[h].read_text(errors='replace'))
        rel = str(tbase[h].relative_to(THEIRS / 'include' if str(tbase[h]).startswith(str(THEIRS)) else TIBSUN))
        out.append(dict(kind='header', name=h, rows=' '.join(sorted(set(users))),
                        our_header=str(obase[h].relative_to(REBUILD / 'include')), their_header=rel,
                        their_base='ra2-decomp' if str(tbase[h]).startswith(str(THEIRS)) else 'TibSun',
                        status='same' if d is None else 'differs', **(d or {})))
    cols = []
    for o in out:
        cols += [k for k in o if k not in cols]
    with open(MANIFEST / 'twin_header_diffs.csv', 'w', newline='') as f:
        w = csv.DictWriter(f, cols, lineterminator='\n')
        w.writeheader()
        w.writerows(out)
    print('rows %d (identical matched twin, text sim >= %.2f)' % (len(rows), min_sim))
    for kind in ('class', 'header'):
        sel = [o for o in out if o['kind'] == kind]
        print(kind, len(sel), 'differing', sum(1 for o in sel if o['status'] == 'differs'))
        for o in sel:
            if o['status'] != 'differs':
                continue
            if kind == 'class':
                print('  %-14s data %s/%s layout %s | methods only ours %s, theirs %s | virtuals %s | rows %s' % (
                    o['name'], o['data_ours'], o['data_theirs'], o['layout'][:50], o['n_only_ours'],
                    o['n_only_theirs'], o['virtual_order'], o['rows']))
            else:
                print('  %-14s decls %s/%s only ours %s [%s] theirs %s [%s] sim %s | rows %s' % (
                    o['name'], o['decl_ours'], o['decl_theirs'], o['n_only_ours'], o['only_ours'][:60],
                    o['n_only_theirs'], o['only_theirs'][:60], o['token_sim'], o['rows'][:60]))


# ---------------------------------------------------------------- count-sensitive rows (--countsens)

PROTO = re.compile(r'^[^\s#/{}][^;{}=]*?\b(~?\w+)\s*\([^;{}]*\)\s*(?:const\s*)?;', re.M)


def count_decls(text):
    """Function declarations (prototypes and member declarations) in a header or file, in order."""
    return [m.group(1) for m in PROTO.finditer(COMMENT.sub(' ', text))]


def countsens():
    units = json.load(open(MANIFEST / 'units.json'))
    ours = header_files([REBUILD / 'include'])
    theirs = header_files([TIBSUN, THEIRS / 'include'])
    for u, info in sorted(units.items()):
        for e, cs in sorted((info.get('count_sensitive') or {}).items()):
            print('%s %s: %s' % (u, e, cs['header']))
            for h in dict.fromkeys(re.findall(r'(\w+\.(?:h|cpp))', cs['header'])):
                if h.endswith('.cpp'):
                    a, b = REBUILD / 'src' / h, THEIRS / 'src' / h
                else:
                    a, b = ours.get(h), theirs.get(h)
                if not a or not b or not Path(a).exists() or not Path(b).exists():
                    print('   %s: missing (%s / %s)' % (h, a, b))
                    continue
                da, db = count_decls(Path(a).read_text(errors='replace')), count_decls(Path(b).read_text(errors='replace'))
                print('   %s: ours %d, theirs %d (delta %+d); only ours %s; only theirs %s' % (
                    h, len(da), len(db), len(db) - len(da), sorted(set(da) - set(db))[:10],
                    sorted(set(db) - set(da))[:10]))


if __name__ == '__main__':
    if '--headers' in sys.argv:
        headers()
    elif '--countsens' in sys.argv:
        countsens()
    else:
        main('--write' in sys.argv)
