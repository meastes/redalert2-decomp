#!/usr/bin/env python3
"""twinmap.py [--write] | --new-since A..B|A|YYYY-MM-DD [--out FILE]: Yuri's Revenge twins of every RA2 function
the rebuild has not made exact (lane L2, 2026-11-19; candidates and --new-since lane O75, 2026-11-24).

Candidates (lane O75): every RA2 function the tools know, from main's manifests/function_map.csv, the rebuild's
units.json rows and rebuild/manifest/image_bounds.csv, less those the rebuild catalog holds or whose unit's
current check (build/r/last) finds exact. Each carries the rebuild's state (class Ours: catalogued rows are
left out; guarded draft with its score, count-sensitive, active, row with no draft, image bound or main row
with no rebuild row). Rows that landed drop out by themselves; tail_buckets.csv only adds its bucket.

For each candidate a twin body is looked up in the retail gamemd.exe 1.001 (SHA-256 7e839d3b...6077) and
ra2-decomp's matched state is read, without compiling anything. Measurement only: nothing is adopted.

Bodies are compared as the 2026-11-06 and Scout808 (2026-11-14) measurements did: each body is decoded on
its own (capstone, linear); the rel32 field of call/jmp/jcc and every 4-byte immediate or displacement whose
value lies inside that image are masked. Identical = same length, same bytes outside both masks (a second
pass lets a byte differ inside a dword that is in-image in both bodies: switch tables decoded as code).
Near twins: (layout) same length and instruction boundaries, only immediate/displacement fields differ; or
(fuzzy) length within 10%, mnemonic-sequence ratio >= 0.90; or (window, lane O75) a body between the YR twins
of the two neighbouring anchors, length within 30%, mnemonic ratio >= 0.60 (rows that grew or shrank between
the games: Techno_Render_Voxel_Object, Send_Statistics_Packet). Name (lane O75): a matched YR function whose
Class::Member equals one of the row's names (rebuild, main, image_bounds, mischa85 names.csv, symbols.csv;
address suffixes dropped, placeholders ignored) and is unique on both sides; it wins over an unmatched byte
twin or a window guess that disagrees. Anchors are identical twins with a single candidate over every
units.json row, kept as a monotonic chain. Score: a token similarity over the masked instruction text.

Matched = the YR address is in ra2-decomp's data/manifest.csv; candidates come from data/functions.csv
(every YR function with its size). Classes: identical_matched, near_matched, identical_unmatched,
near_unmatched, ra2_only.

Text: their text is read at the `// FUNCTION|SYNTHETIC|TEMPLATE: GAMEMD 0x...` annotation of their_path
(a body when code follows; "header" when the annotation only names a function defined elsewhere). Ours is
the definition of the row's name in rebuild/src/<unit>.cpp (any guard state; the closest overload by text).
text_sim is a token ratio over both bodies with comments and spaces removed; text_same says "same" when the
token lists are equal.

--write writes rebuild/manifest/tail_twins.csv (one line per candidate) and rebuild/manifest/twin_queue.csv
(rows with a matched twin, by bytes) and prints the split. Without it nothing is written.

--new-since (the daily pull's first step, lane O75): the YR functions in ra2-decomp's data/manifest.csv at B
(default HEAD) that were not there at A (a commit, or the last commit before a date), each with the commit
that added it, its RA2 twin (same searches in the other direction: identical, layout, fuzzy, name, window),
the rebuild's state and LEAD when that state is not exact. --out also writes the list to FILE.

The YR executable is looked up in $YR_GAMEMD, the user's installation and ra2-decomp work clones; it is read
only.
"""
import bisect
import collections
import csv
import difflib
import hashlib
import io
import json
import os
import re
import struct
import subprocess
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


def definitions(unit, name):
    """[(line, offset, text)] of every definition of Class::Name (or Name) at file scope in rebuild/src/<unit>.cpp."""
    p = REBUILD / 'src' / (unit + '.cpp')
    if p not in _our_cache:
        _our_cache[p] = p.read_text(errors='replace') if p.exists() else ''
    text = _our_cache[p]
    if not name:
        return []
    out = []
    pat = re.compile(r'^(?:[^\s#/][^;{}\n]*?)?(?<![\w:~])' + re.escape(name) + r'\s*\(', re.M)
    for m in pat.finditer(text):
        end = braced(text, m.start())
        head = text[m.start(): text.find('{', m.start())] if end else ''
        if not end or ';' in COMMENT.sub(' ', head):
            continue
        line = text.count('\n', 0, m.start()) + 1
        out.append((line, m.start(), text[m.start():end]))
    return out


def our_texts(unit, name):
    """Every definition of Class::Name (or Name) at file scope in rebuild/src/<unit>.cpp: [(line, text)]."""
    return [(line, text) for line, _, text in definitions(unit, name)]


def text_compare(ours, theirs):
    a, b = tokens(ours), tokens(theirs)
    if a == b:
        return 'same', 1.0
    return 'differs', difflib.SequenceMatcher(None, a, b, autojunk=False).ratio()


# ---------------------------------------------------------------- our side: every RA2 function and its state

FUNCTION_MAP = ROOT / 'manifests/function_map.csv'
IMAGE_BOUNDS = MANIFEST / 'image_bounds.csv'
NAMES = ROOT / 'private/reference/mischa85-RedAlert2/config/ra2-1.006/names.csv'
GUARD = re.compile(r'^#ifdef RA2_NON_MATCHING\b[^\n]*', re.M)
DIRECTIVE = re.compile(r'^[ \t]*#[ \t]*(if|ifdef|ifndef|endif)\b', re.M)
PLACEHOLDER = re.compile(r'(?:^|::)(?:Unknown|FUN|sub|thunk|Leaf|Func)_[0-9A-Fa-f]+|_[0-9A-Fa-f]{6,8}::')
DONE = ('catalogued', 'exact in its unit check')


def read_csv(path):
    with open(path, newline='') as f:
        return list(csv.DictReader(f))


def find_yr():
    for p in YR_PLACES:
        if p and Path(p).exists():
            return p
    raise SystemExit('gamemd.exe 1.001 not found; set YR_GAMEMD')


def name_key(sym):
    """'Class::Member' of a symbol for matching across the two games, or None for a placeholder name
    (Unknown_..., FUN_..., an address-suffixed class); a member's address suffix is dropped
    (Render_Voxel_Object_006d24e0 -> Render_Voxel_Object)."""
    if not sym:
        return None
    d = demangle(sym) if sym[0] in '?_' else sym
    if not d:
        return None
    if PLACEHOLDER.search(d):
        return None
    return re.sub(r'_[0-9A-Fa-f]{6,8}$', '', d)


def guard_spans(text):
    """[(start, end, label)] of every RA2_NON_MATCHING guard in text."""
    out = []
    for m in GUARD.finditer(text):
        depth = 0
        for d in DIRECTIVE.finditer(text, m.start()):
            depth += 1 if d.group(1).startswith('if') else -1
            if depth == 0:
                out.append((m.start(), d.start(), m.group(0)))
                break
    return out


def guard_score(label):
    m = re.search(r'(\d[\d,]*) of (\d[\d,]*) bytes differ', label) or re.search(r'(\d[\d,]*) bytes differ', label)
    if m:
        return '%s B off' % m.group(1)
    m = re.search(r'best ([\d.]+)', label)
    return 'main best %s%%' % m.group(1) if m else ''


class Ours:
    """Every RA2 function the tools know (main's function map, the rebuild's unit rows, image-derived bounds)
    with its names and the rebuild's state of it. States, first that applies:
      catalogued                the rebuild catalog holds it (whole-file exact)
      exact in its unit check   its unit's current check (build/r/last) finds it exact
      count-sensitive           parked exact form (units.json count_sensitive)
      header-form parked        units.json header_form_parked
      guarded draft[, no row]   its definition is under RA2_NON_MATCHING (the guard's score follows)
      active, not exact         an active definition the unit's check does not find exact
      row, no definition in the unit file   a unit row whose name is not defined in rebuild/src/<unit>.cpp
                                (a header inline or template)
    A row's tailmap bucket (tail_buckets.csv, as of its last --write) follows in brackets.
      image bound, no row       image_bounds.csv only
      no row                    main's function map only (a unit not yet holding it)"""

    def __init__(self):
        self.units = json.load(open(MANIFEST / 'units.json'))
        self.fn = {}
        ranges = []
        for u, info in self.units.items():
            lo, hi = (int(x, 16) for x in info.get('range') or ('0', '0'))
            ranges.append((hi - lo, lo, hi, u))
        ranges.sort()

        def get(e):
            return self.fn.setdefault(e, dict(entry=e, unit='', bytes=0, names=[], row=None, image=False))
        for r in read_csv(FUNCTION_MAP):
            f = get(int(r['address'], 16))
            f['bytes'], f['main_unit'] = int(r['size']), Path(r['source']).stem
            f['names'].append(r['symbol'])
        for r in read_csv(IMAGE_BOUNDS):
            if r['status'] == 'bound':
                f = get(int(r['entry'], 16))
                f['bytes'], f['image'] = int(r['body_bytes']), True
                f['names'].append(r['catalog_symbol'])
        for u, info in self.units.items():
            for r in info['rows']:
                f = get(int(r['entry'], 16))
                f['unit'], f['bytes'], f['row'] = u, int(r['body']), r
                f['names'] = [r['symbol'], r.get('main_symbol') or ''] + f['names']
        for path in (NAMES, MANIFEST / 'symbols.csv'):
            if path.exists():
                for r in read_csv(path):
                    e = int(r['address'], 16)
                    if e in self.fn:
                        self.fn[e]['names'].append(r['symbol'])
        for e, f in self.fn.items():
            f['names'] = [n for n in dict.fromkeys(f['names']) if n]
            if not f['unit']:      # inside a unit's range, else no rebuild unit (main's file stays in main_unit)
                f['unit'] = next((u for _, lo, hi, u in ranges if lo <= e < hi), '')
        self.by_name = collections.defaultdict(set)
        for e, f in self.fn.items():
            for n in f['names']:
                k = name_key(n)
                if k:
                    self.by_name[k].add(e)
        self.done = {}
        try:
            sys.path.insert(0, str(ROOT / 'tools/original_target'))
            import catalog_io
            for r in catalog_io.load(MANIFEST / 'catalog')['regions']:
                self.done[r['entry']] = 'catalogued'
        except Exception as err:   # a tree without a catalog: every row reads as not exact
            print('catalog not read:', err)
        work = ROOT / 'build/r/last'
        for u, info in self.units.items():
            p = work / (u + '.json')
            if not p.exists():
                continue
            last = json.load(open(p))
            src = ROOT / info['source']
            if not src.exists() or last.get('source_sha256') != hashlib.sha256(src.read_bytes()).hexdigest():
                continue
            if [(r['entry'], r['symbol']) for r in last['results']] != [(r['entry'], r['symbol']) for r in info['rows']]:
                continue
            for r in last['results']:
                if r['status'] == 'exact':
                    self.done.setdefault(int(r['entry'], 16), 'exact in its unit check')
        self._spans = {}
        path = MANIFEST / 'tail_buckets.csv'
        self.buckets = {r['entry']: r['bucket'] for r in read_csv(path)} if path.exists() else {}

    def name(self, e):
        """The function's best name: the rebuild row's symbol, else the first non-placeholder one."""
        f = self.fn[e]
        named = [n for n in f['names'] if name_key(n)]
        return (named or f['names'] or [''])[0]

    def state(self, e):
        if e in self.done:
            return self.done[e]
        f = self.fn.get(e)
        if not f:
            return 'unknown to the tools'
        info = self.units.get(f['unit'], {})
        h = '%08x' % e
        if h in (info.get('count_sensitive') or {}):
            return 'count-sensitive'
        if h in (info.get('header_form_parked') or {}):
            return 'header-form parked'
        defs = []
        for n in f['names']:
            k = name_key(n)
            parts = k.split('::') if k else []
            for i in range(max(1, len(parts) - 1)):   # NS::Class::Member, then Class::Member
                defs = definitions(f['unit'], '::'.join(parts[i:]))
                if defs:
                    break
            if defs:
                break
        if f['unit'] not in self._spans:
            p = REBUILD / 'src' / (f['unit'] + '.cpp')
            self._spans[f['unit']] = guard_spans(p.read_text(errors='replace')) if p.exists() else []
        guarded = [label for _, at, _ in defs for a, b, label in self._spans[f['unit']] if a <= at < b]
        bucket = self.buckets.get(h)
        tail = ' [tailmap %s]' % bucket if bucket and f['row'] else ''
        if guarded:      # an overload of the name may be an exact row of its own
            s = guard_score(guarded[0])
            return 'guarded draft' + ('' if f['row'] else ', no row') + (' (%s)' % s if s else '') + tail
        if defs:
            return ('active, not exact' if f['row'] else 'active definition, no row') + tail
        if f['row']:
            return 'row, no definition in the unit file' + tail
        state = 'image bound, no row' if f['image'] else 'no row'
        return state if f['unit'] else '%s (no unit; main %s)' % (state, f.get('main_unit', '?'))


# ---------------------------------------------------------------- main

def their_manifest(ref=None):
    """{YR address: (size, symbol, source)} of ra2-decomp's data/manifest.csv (at ref, else the checkout)."""
    if ref:
        text = subprocess.run(['git', '-C', str(THEIRS), 'show', ref + ':data/manifest.csv'], capture_output=True,
                              text=True, check=True).stdout
        rows = csv.DictReader(io.StringIO(text))
    else:
        rows = read_csv(THEIRS / 'data/manifest.csv')
    return {int(r['address'], 16): (int(r['size']), r['symbol'], r['source']) for r in rows}


class Twins:
    """Both images' bodies, the anchor chain of single identical twins and the searches in both directions."""

    def __init__(self, ours):
        self.ours = ours
        self.ra2, self.yr = Image(RA2, RA2_SHA), Image(find_yr(), YR_SHA)
        self.man = their_manifest()
        self.fl = {int(r['address'], 16): (int(r['size']), r['name']) for r in read_csv(THEIRS / 'data/functions.csv')}
        for a, (n, sym, src) in self.man.items():
            self.fl.setdefault(a, (n, sym))
        self.yb = self._bodies(self.yr, ((a, n) for a, (n, _) in self.fl.items()))
        self.rb = self._bodies(self.ra2, ((e, f['bytes']) for e, f in ours.fn.items()))
        self.y_len, self.y_key = self._index(self.yb)
        self.r_len, self.r_key = self._index(self.rb)
        self.ry = sorted(self.yb)
        self.rr = sorted(self.rb)
        self.y_name = collections.defaultdict(set)
        for a, (n, sym, src) in self.man.items():
            k = name_key(sym)
            if k:
                self.y_name[k].add(a)
        print('YR bodies', len(self.yb), 'matched', sum(1 for a in self.yb if a in self.man), '| RA2 bodies',
              len(self.rb), flush=True)
        anchors = []
        for e, f in ours.fn.items():
            b = self.rb.get(e)
            if b is not None and f['row'] and len(b.data) >= 16:
                h = self.identical(b, self.yb, self.y_key, self.y_len, self.ra2, self.yr)
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
        self.chain = chain
        self.cx = [c[0] for c in chain]
        self.cy = [c[1] for c in chain]
        print('anchors', len(chain), 'of', len(anchors), flush=True)

    @staticmethod
    def _bodies(img, items):
        out = {}
        for a, n in items:
            d = img.read(a, n)
            if d:
                out[a] = Body(img, a, d)
        return out

    @staticmethod
    def _index(bodies):
        by_len, by_key = collections.defaultdict(list), collections.defaultdict(list)
        for a, b in bodies.items():
            by_len[len(b.data)].append(a)
            by_key[b.key].append(a)
        return by_len, by_key

    @staticmethod
    def identical(b, pool, by_key, by_len, ib, ip):
        hits = [a for a in by_key.get(b.key, ()) if equiv(b, pool[a], ib, ip)]
        if not hits:
            hits = [a for a in by_len.get(len(b.data), ()) if equiv(b, pool[a], ib, ip)]
        return hits

    def yr_window(self, e):
        """YR addresses between the twins of the anchors around RA2 entry e."""
        i = bisect.bisect_left(self.cx, e)
        return (self.cy[i - 1] if i else 0), (self.cy[i] if i < len(self.cy) else 1 << 32)

    def ra2_window(self, y):
        i = bisect.bisect_left(self.cy, y)
        return (self.cx[i - 1] if i else 0), (self.cx[i] if i < len(self.cx) else 1 << 32)

    @staticmethod
    def near(b, pool, by_len, among=None):
        """[(score, address, how)] of near twins of b in pool: same layout, or a close mnemonic sequence
        (length within 10%); with `among` (addresses between two anchors) also any body there whose length is
        within 30% and whose mnemonic sequence is >= 0.60 alike (rows that grew or shrank between the games)."""
        n = len(b.data)
        lay = []
        for a in by_len.get(n, ()):
            f = layout_equiv(b, pool[a])
            if f is not None:
                lay.append((tok_sim(b, pool[a]), a, 'layout (%d field%s)' % (len(f), '' if len(f) == 1 else 's')))
        if lay:
            return lay
        cands = []
        if n >= 16:
            for L in range(int(n * 0.9), int(n * 1.1) + 2):
                for a in by_len.get(L, ()):
                    y = pool[a]
                    if abs(y.ninsn - b.ninsn) > max(2, b.ninsn // 10):
                        continue
                    if sum((b.cmn & y.cmn).values()) / max(b.ninsn, y.ninsn) < 0.88:
                        continue
                    mr = difflib.SequenceMatcher(None, b.mn, y.mn, autojunk=False).ratio()
                    if mr >= 0.90:
                        cands.append((tok_sim(b, y), a, 'fuzzy (mnemonics %.3f)' % mr))
        if not cands and among and n >= 32:
            for a in among:
                y = pool[a]
                if not 0.7 <= len(y.data) / n <= 1.43:
                    continue
                if sum((b.cmn & y.cmn).values()) / max(b.ninsn, y.ninsn) < 0.6:
                    continue
                mr = difflib.SequenceMatcher(None, b.mn, y.mn, autojunk=False).ratio()
                if mr >= 0.60:
                    cands.append((tok_sim(b, y), a, 'window (mnemonics %.3f)' % mr))
        return cands

    def between(self, ordered, lo, hi, limit=400):
        i, j = bisect.bisect_left(ordered, lo), bisect.bisect_right(ordered, hi)
        return ordered[i:j] if j - i <= limit else ()

    def to_yr(self, e, names):
        """(yr address, how, score, candidates, in_order, name_hit) of RA2 entry e's YR twin, or None.
        Order: identical bytes; same layout or close mnemonics; a matched YR function of the same name; a body
        between the two anchors (window). A byte twin that is unmatched gives way to a matched name twin."""
        b = self.rb.get(e)
        lo, hi = self.yr_window(e)
        name_hit = None
        for n in names:
            k = name_key(n)
            hits = self.y_name.get(k, ()) if k else ()
            if len(hits) == 1:
                name_hit = next(iter(hits))
                break
        pick = None
        if b is not None:
            hits = self.identical(b, self.yb, self.y_key, self.y_len, self.ra2, self.yr)
            if hits:
                hits.sort(key=lambda a: (not (lo <= a <= hi), a not in self.man, a != name_hit))
                pick = (hits[0], 'identical', 1.0, len(hits))
            else:
                cands = self.near(b, self.yb, self.y_len, self.between(self.ry, lo, hi))
                if cands:
                    cands.sort(key=lambda c: (c[1] == name_hit, lo <= c[1] <= hi, c[0], c[1] in self.man), reverse=True)
                    score, a, how = cands[0]
                    if not how.startswith('window') or name_hit is None or a == name_hit:
                        pick = (a, how, score, len(cands))
        if name_hit is not None and (pick is None or (pick[0] != name_hit and pick[0] not in self.man)):
            score = tok_sim(b, self.yb[name_hit]) if b is not None and name_hit in self.yb else 0.0
            pick = (name_hit, 'name', score, 1)
        if pick is None:
            return None
        a, how, score, ncand = pick
        return a, how, score, ncand, 'yes' if lo <= a <= hi else 'no', name_hit

    def to_ra2(self, y):
        """(ra2 entry, how, score, name_hit) of YR function y's RA2 twin, or None (the same order as to_yr)."""
        b = self.yb.get(y)
        lo, hi = self.ra2_window(y)
        name_hit = None
        k = name_key(self.man[y][1]) if y in self.man else None
        hits = self.ours.by_name.get(k, ()) if k else ()
        if len(hits) == 1:
            name_hit = next(iter(hits))
        if b is not None:
            hits = self.identical(b, self.rb, self.r_key, self.r_len, self.yr, self.ra2)
            if hits:
                hits.sort(key=lambda a: (a != name_hit, not (lo <= a <= hi)))
                a = hits[0]
                if len(hits) > 1 and len(b.data) < 16 and a != name_hit and not lo <= a <= hi:
                    return None          # a tiny body (ret, a thunk) identical to many: no evidence
                return a, 'identical', 1.0, name_hit
            cands = self.near(b, self.rb, self.r_len, self.between(self.rr, lo, hi))
            if cands:
                cands.sort(key=lambda c: (c[1] == name_hit, lo <= c[1] <= hi, c[0]), reverse=True)
                score, a, how = cands[0]
                if name_hit is None or a == name_hit or not how.startswith('window'):
                    return a, how, score, name_hit
        if name_hit is not None:
            score = tok_sim(b, self.rb[name_hit]) if b is not None and name_hit in self.rb else 0.0
            return name_hit, 'name', score, name_hit
        return None


def main(write):
    ours = Ours()
    tw = Twins(ours)
    man, fl, yb = tw.man, tw.fl, tw.yb
    hist = {(r['unit'], r['entry']): r for r in read_csv(MANIFEST / 'tail_history.csv')}
    buckets = ours.buckets
    out = []
    for e in sorted(ours.fn):
        state = ours.state(e)
        if state in DONE:
            continue
        f = ours.fn[e]
        h = '%08x' % e
        n = f['bytes']
        hr = hist.get((f['unit'], h), {})
        row = dict(unit=f['unit'], entry=h, function=ours.name(e), bytes=n, bucket=buckets.get(h, ''),
                   difficulty=hr.get('difficulty', ''), twin_class='ra2_only', yr_entry='', yr_bytes='',
                   how='', score='', candidates=0, in_order='', matched='', their_symbol='', their_path='',
                   their_line='', their_text='', our_line='', text_same='', text_sim='', state=state, name_twin='')
        out.append(row)
        if e not in tw.rb:
            row['how'] = 'unreadable'
        t = tw.to_yr(e, f['names'])
        if t is None:
            continue
        pick, how, score, ncand, in_order, name_hit = t
        matched = pick in man
        row.update(yr_entry='%08x' % pick, yr_bytes=len(yb[pick].data) if pick in yb else fl.get(pick, ('',))[0],
                   how=how, score='%.4f' % score, candidates=ncand, in_order=in_order,
                   matched='yes' if matched else 'no',
                   twin_class=('identical_' if how == 'identical' else 'near_') + ('matched' if matched else 'unmatched'),
                   their_symbol=man[pick][1] if matched else fl.get(pick, ('', ''))[1],
                   name_twin='%08x' % name_hit if name_hit is not None else '')
        if matched:
            path = man[pick][2]
            line, tt = their_text(path, pick)
            row.update(their_path=path, their_line=line or '', their_text='body' if tt else 'header')
            name = name_key(row['function']) or ''
            mine = our_texts(f['unit'], name) if name else []
            if tt and mine:
                best = max(((text_compare(t, tt), ln) for ln, t in mine), key=lambda x: x[0][1])
                (same, sim), ln = best
                row.update(our_line=ln, text_same=same, text_sim='%.3f' % sim)
            elif mine:
                row.update(our_line=mine[0][0], text_same='no their body')
            else:
                row.update(text_same='not in unit source')

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
                        'score', 'bucket', 'difficulty', 'text_same', 'text_sim', 'action', 'state'])
            for r in q:
                if r['text_same'] == 'same':
                    act = 'text is already the twin\'s: adopt its header forms (twin_header_diffs.csv)'
                elif r['their_text'] == 'header':
                    act = 'twin defined in their headers or compiler-generated: adopt the header form'
                else:
                    act = 'adopt the twin\'s text and its header forms'
                w.writerow([r['unit'], r['function'], r['entry'], r['yr_entry'], r['their_path'], r['their_line'],
                            r['bytes'], r['twin_class'], r['score'], r['bucket'], r['difficulty'], r['text_same'],
                            r['text_sim'], act, r['state']])
        print('wrote tail_twins.csv and twin_queue.csv (%d queued)' % len(q))


def split(out):
    classes = ['identical_matched', 'near_matched', 'identical_unmatched', 'near_unmatched', 'ra2_only']
    for title, rows in (('all not exact', out), ('hard on main', [r for r in out if r['difficulty'] == 'hard on main'])):
        print('%s: %d rows, %d B' % (title, len(rows), sum(r['bytes'] for r in rows)))
        for c in classes:
            sel = [r for r in rows if r['twin_class'] == c]
            print('  %-20s %5d rows %8d B' % (c, len(sel), sum(r['bytes'] for r in sel)))
    sel = [r for r in out if r['twin_class'] == 'identical_matched']
    tc = collections.Counter(r['text_same'] or '-' for r in sel)
    print('identical_matched text:', dict(tc))
    sc = collections.Counter(re.sub(r' \([^)]*\)| \[.*', '', r['state']) for r in out)
    print('states:', ', '.join('%s %d' % kv for kv in sc.most_common()))


# ---------------------------------------------------------------- new since (--new-since)

def resolve_range(spec):
    """(old, new) commits of ra2-decomp for 'A..B', 'A' (to HEAD) or a date 'YYYY-MM-DD' (last commit before it)."""
    def rev(r):
        return subprocess.run(['git', '-C', str(THEIRS), 'rev-parse', '--short', r], capture_output=True, text=True,
                              check=True).stdout.strip()
    old, _, new = spec.partition('..')
    if re.match(r'\d{4}-\d\d-\d\d', old):
        old = subprocess.run(['git', '-C', str(THEIRS), 'rev-list', '-1', '--before=' + old, new or 'HEAD'],
                             capture_output=True, text=True, check=True).stdout.strip()
    return rev(old), rev(new or 'HEAD')


def new_since(spec, out_path=None):
    """ra2-decomp functions matched at B but not at A, each with its RA2 twin and the rebuild's state of it."""
    old, new = resolve_range(spec)
    log = subprocess.run(['git', '-C', str(THEIRS), 'log', '--reverse', '--format=%h %s', old + '..' + new, '--',
                          'data/manifest.csv'], capture_output=True, text=True, check=True).stdout.splitlines()
    seen = set(their_manifest(old))
    added = {}
    for line in log:
        c, _, subject = line.partition(' ')
        for a in set(their_manifest(c)) - seen:
            added[a] = (c, subject)
        seen |= set(their_manifest(c))
    final = their_manifest(new)
    added = {a: v for a, v in added.items() if a in final}
    for a, (c, subject) in list(added.items()):   # the commit that brought the source (Restore ...), when another
        src = subprocess.run(['git', '-C', str(THEIRS), 'log', '--reverse', '--format=%h %s', '-S',
                              'GAMEMD 0x%08X' % a, old + '..' + new, '--', 'src', 'include'],
                             capture_output=True, text=True).stdout.splitlines()
        if src and not src[0].startswith(c + ' '):
            added[a] = tuple(src[0].split(' ', 1)) if ' ' in src[0] else (src[0], '')
    ours = Ours()
    tw = Twins(ours)
    for a in added:
        if a not in tw.man:          # the checkout is behind B: take B's rows
            tw.man[a] = final[a]
    rows = []
    for a in sorted(added):
        size, sym, src = final[a]
        c, subject = added[a]
        r = dict(commit=c, subject=subject, yr_entry='%08x' % a, yr_bytes=size, their_symbol=sym, their_path=src,
                 ra2_entry='', unit='', function='', bytes='', how='', score='', state='', note='')
        t = tw.to_ra2(a)
        if t is None:
            r['note'] = 'no RA2 twin found (YR only?)'
        else:
            e, how, score, name_hit = t
            f = ours.fn.get(e, {})
            r.update(ra2_entry='%08x' % e, unit=f.get('unit', ''), function=ours.name(e) if f else '',
                     bytes=f.get('bytes', ''), how=how, score='%.4f' % score, state=ours.state(e))
            if name_hit is not None and name_hit != e:
                r['note'] = 'name twin %08x differs' % name_hit
        rows.append(r)
    lead = [r for r in rows if r['state'] and r['state'] not in DONE]
    lines = ['# ra2-decomp functions newly matched %s..%s (%d commits touching data/manifest.csv): %d, '
             'RA2 twin not exact here: %d' % (old, new, len(log), len(rows), len(lead))]
    for r in sorted(rows, key=lambda r: (r['state'] in DONE or not r['state'], -(r['bytes'] or 0))):
        lines.append('%s %s %s YR %s (%s B) -> %s %s %s (%s B) %s %s | %s%s' % (
            'LEAD' if r in lead else 'done' if r['state'] else '----', r['commit'], demangle(r['their_symbol'])
            or r['their_symbol'], r['yr_entry'], r['yr_bytes'], r['unit'] or '-', r['ra2_entry'] or '-',
            r['function'] and (demangle(r['function']) or r['function']), r['bytes'], r['how'], r['score'],
            r['state'] or '-', (' | ' + r['note']) if r['note'] else ''))
    text = '\n'.join(lines) + '\n'
    print(text, end='')
    if out_path:
        Path(out_path).write_text(text)
    return rows


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
    elif '--new-since' in sys.argv:
        i = sys.argv.index('--new-since')
        out = sys.argv[sys.argv.index('--out') + 1] if '--out' in sys.argv else None
        new_since(sys.argv[i + 1], out)
    else:
        main('--write' in sys.argv)
