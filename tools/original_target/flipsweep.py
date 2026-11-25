"""flipsweep.py UNIT ENTRY --func TEXT [--kind KIND] [--at POS] [--max K] [--body N --pad N] [--symbol NAME]:
find the count of one diagnostic statement at which a near miss's differing lines change (lane O47, 2026-11-22).

Idea and kinds adopted from ra2-decomp's tools/flipsweep.py (gibbo101, GPL-3.0-or-later, commit 3f4505e;
THIRD_PARTY.md), rewritten for this tree's compile and compare (rebuild.py). Some ties are thresholds on a
count (named locals, instructions in a block before scheduling, inline sites, the inline pool): the score stays
flat until the flip, so one run finds it. A flip is a target, not a fix: the probes never land; find natural
source that changes the count the same way.

The unit's text is copied to build/r/flipsweep/ (never the tree's file), every RA2_NON_MATCHING guard whose
text holds --func is opened, and for k = 0..K, k copies of the statement go at one place in that function's
body. Per k: status, differing bytes, body size and a letter per distinct set of differing lines.

kinds (each copy's index replaces {k}):
  local  a dead named, initialised local            int flip_local_{k} = {k};
  store  a one-instruction store to an extern int    Flip_Probe_Store_[{k}] = 0x7F00 + {k};
  call   a call of a trivial inline that folds       Inline_Probe_({k});
  expr   a dead distinct expression term             (void)(Flip_Probe_Value_ * ({k} + 3));
  stmt   --stmt TEXT
positions: start, end (before the body's last return), before:REGEX, after:REGEX (first matching line of the
body). Declarations a kind needs go just above the function at every k, k = 0 included.
"""
import argparse
import json
import re
import shutil
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import rebuild as R                                   # noqa: E402
from coff import Coff                                 # noqa: E402
from verify_manifest import Audit                     # noqa: E402
from tailmap import guards, open_text                 # noqa: E402

KINDS = {
    'local': ('int flip_local_{k} = {k};', ''),
    'store': ('Flip_Probe_Store_[{k}] = 0x7F00 + {k};', 'extern int Flip_Probe_Store_[];'),
    'call': ('Inline_Probe_({k});', 'static inline int Inline_Probe_(int a) { return a & 0; }'),
    'expr': ('(void)(Flip_Probe_Value_ * ({k} + 3));', 'extern int Flip_Probe_Value_;'),
}


def body_span(text, start):
    """(open, close) offsets of the first brace body at or after `start` (comments and literals skipped)."""
    depth, i, n = 0, start, len(text)
    first = None
    while i < n:
        c = text[i]
        if text.startswith('//', i):
            i = text.index('\n', i)
            continue
        if c in '"\'':
            j = i + 1
            while text[j] != c:
                j += 2 if text[j] == '\\' else 1
            i = j + 1
            continue
        if c == '{':
            depth += 1
            first = i if first is None else first
        elif c == '}':
            depth -= 1
            if first is not None and depth == 0:
                return first, i
        i += 1
    raise SystemExit('unbalanced braces')


def position(text, span, pos):
    o, c = span
    lines_start = [m.start() for m in re.finditer(r'^', text[o:c], re.M)]
    starts = [o + s for s in lines_start][1:]
    if pos == 'start':
        return starts[0]
    if pos == 'end':
        for s in reversed(starts):
            line = text[s:text.index('\n', s)]
            if re.match(r'\s*return\b', line):
                return s
            if line.strip() and line.strip() != '}':
                break
        return text.rindex('\n', o, c) + 1
    where, _, pattern = pos.partition(':')
    for s in starts:
        if re.search(pattern, text[s:text.index('\n', s)]):
            return s if where == 'before' else text.index('\n', s) + 1
    raise SystemExit('no line of the body matches %r' % pattern)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('unit'); ap.add_argument('entry')
    ap.add_argument('--func', required=True, help='text of the function header line (first match is used)')
    ap.add_argument('--kind', default='local', choices=[*KINDS, 'stmt'])
    ap.add_argument('--stmt'); ap.add_argument('--decl', default='')
    ap.add_argument('--at', default='start'); ap.add_argument('--max', type=int, default=8)
    ap.add_argument('--body', type=int); ap.add_argument('--pad', type=int, default=0)
    ap.add_argument('--symbol')
    a = ap.parse_args()
    info = dict(R.load_json(R.UNITS, {})[a.unit])
    entry = int(a.entry, 16)
    row = next((r for r in info['rows'] if r['entry'] == a.entry.lower().rjust(8, '0')), None)
    body, pad = (a.body, a.pad) if a.body else ((row['body'], row.get('padding', 0)) if row else (None, 0))
    symbol = a.symbol or (row['symbol'] if row else None)
    if body is None or symbol is None:
        raise SystemExit('no row for %s: pass --body and --symbol' % a.entry)
    stmt, decl = (a.stmt, a.decl) if a.kind == 'stmt' else KINDS[a.kind]
    text = (R.ROOT / info['source']).read_text(encoding='latin-1')
    fpos = text.index(a.func)
    text = open_text(text, [g[0] for g in guards(text) if g[0] < fpos < g[1]])
    fpos = text.index(a.func)
    head = text.rfind('\n', 0, fpos) + 1
    if decl:
        text = text[:head] + decl + '\n' + text[head:]
    span = body_span(text, text.index(a.func))
    at = position(text, span, a.at)
    audit = Audit(R.ROOT, json.loads(R.CONFIG.read_text())['executable'])
    resolve = R.resolver(a.unit, info)
    out = R.WORK / 'flipsweep'
    out.mkdir(parents=True, exist_ok=True)
    sets, rows = {}, []
    for k in range(a.max + 1):
        probe = text[:at] + ''.join('\t' + stmt.replace('{k}', str(q)) + '\n' for q in range(k)) + text[at:]
        path = out / ('%s.cpp' % a.unit)
        path.write_text(probe, encoding='latin-1')
        obj, log, _, folder = R.compile_unit(a.unit + '-flip', dict(info, source=str(path.relative_to(R.ROOT))))
        if obj is None:
            rows.append((k, 'build failed', None)); print('k=%-3d build failed' % k, flush=True)
            continue
        coff = Coff(obj.read_bytes())
        st, diff, size, _, _ = R.compare(audit, coff, symbol, entry, body, pad, resolve)
        lines = tuple(sorted(l for l in R.disasm_diff(audit, R.compare.actual, entry, body).splitlines()
                             if l[:1] in '+-' and not l.startswith(('+++', '---')) and '0x7f' not in l))
        tag = sets.setdefault(lines, chr(ord('A') + len(sets)))
        rows.append((k, st, tag))
        print('k=%-3d %-8s %5s B differ  body %5d  set %s' % (k, st, diff, len(R.compare.actual), tag), flush=True)
        shutil.rmtree(folder, ignore_errors=True)
    base = rows[0][2]
    flip = next((k for k, _, t in rows if t != base), None)
    print('flip point: k=%s' % flip if flip is not None else 'no flip: every k gives k=0\'s set')


if __name__ == '__main__':
    main()
