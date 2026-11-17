#!/usr/bin/env python3
"""Rescore parked drafts in their Route A unit (lane M, 2026-11-12).

    rescore_joined.py LIST.json OUTDIR [--rename-map MAP.json]...

Lane bs.py-style harnesses join a draft into its population by concatenating one file per row; since Route A
a population's rows share one module source, so they duplicate it and no longer compile. This scores a draft
the way the build would compile it today. Nothing here installs or accepts anything; acceptance stays with the
strict audit.

Item fields: entry (hex), length (original body bytes; use the true bound), draft, symbol (prefix, optional),
mode J or S, pre (text prepended to the unit), inc (extra include directory searched before src/), x
({addr: path}), before (regex: insert before the first module line matching it, for modules whose functions
carry no address header), rename (true: apply the --rename-map identifier maps to draft, pre and x texts),
base (directory relative draft paths resolve against), tag. Relative paths resolve against base, then
RA2_CLASS_WORK (default build/class-work of the repository), then the repository.

LIST: JSON list of {entry, length, draft, symbol?, pre?, inc?, x?: {addr: path}, mode: J|S, tag}.
J: the draft is inserted into its original unit's module source (unit_map.json -> modules/<unit>.json)
before the header comment of the module's next function by address, inside that population's
compile group (sources joined in source order, as build.compile_sources does). Units without a
module fall back to the population of the nearest lower accepted row (draft placed in source
order). X rows: an accepted row's block in the module is replaced by the file, an unaccepted
one is inserted. PRE text goes first. INC is an extra include directory before src/.
S: the draft compiled alone (PRE first, INC as given).
All TUs compile in parallel Wine sessions; each object is scored with diff_candidate's scorer
(population references added). Without a symbol every function symbol not belonging to an
accepted row is scored and the best is kept."""
import sys, os, json, re, bisect
from collections import OrderedDict
from pathlib import Path
R = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(R / 'tools/original_target'))
os.chdir(R)
import catalog_io, build, diff_candidate as dc, canonical
from coff import Coff
import capstone
from verify_manifest import Audit

import argparse
_parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
_parser.add_argument('list', type=Path)
_parser.add_argument('output', type=Path)
_parser.add_argument('--rename-map', type=Path, action='append', default=[],
                     help='JSON {old: new} identifier map applied to items with rename (later maps win)')
_parser.add_argument('--config', type=Path, default=R / 'private/original-target.local.json')
_args = _parser.parse_args()
lst, out = _args.list, _args.output.resolve()
out.mkdir(parents=True, exist_ok=True)
cfg = json.loads(_args.config.read_bytes())
cat = catalog_io.load(catalog_io.locate(R), references=False)
umap = json.load(open(R / 'manifests/original_target/unit_map.json'))['units']
unit_of = {int(a, 16): u for u, addrs in umap.items() for a in addrs}
ukeys = sorted(unit_of)
accepted = {r['entry']: r for r in cat['regions']}
accepted_symbols = {r['symbol'] for r in cat['regions'] if r.get('symbol')}
rows_by_entry = sorted((r['entry'], r) for r in cat['regions'])
entries = [e for e, _ in rows_by_entry]
BEFORE = {}
HDR = re.compile(r'^//.*\(0x([0-9A-Fa-f]{8})\)\s*$')
CW = Path(os.environ.get('RA2_CLASS_WORK', R / 'build/class-work'))


def unit_for(entry):
    if entry in unit_of:
        return unit_of[entry]
    i = bisect.bisect_left(ukeys, entry)
    return unit_of[ukeys[i - 1]] if i else None


def group_for(entry):
    unit = unit_for(entry)
    mod = R / 'manifests/original_target/modules' / ('%s.json' % unit) if unit else None
    if mod and mod.exists():
        m = json.load(open(mod))
        return unit, m['population'], tuple(m['options']), m['source']
    i = bisect.bisect_left(entries, entry) - 1
    r = rows_by_entry[i][1]
    return unit, r['population'], tuple(r['options']), None


def insert_module(text, entry, block, replace=False):
    lines = text.split('\n')
    hdrs = [(n, int(HDR.match(l).group(1), 16)) for n, l in enumerate(lines) if HDR.match(l)]
    if replace:
        for k, (n, a) in enumerate(hdrs):
            if a == entry:
                end = hdrs[k + 1][0] if k + 1 < len(hdrs) else len(lines)
                return '\n'.join(lines[:n] + [block] + lines[end:]), 'replaced'
        return None, 'not-found'
    at = next((n for n, a in hdrs if a > entry), None)
    if BEFORE.get(entry):
        k = next((n for n, l in enumerate(lines) if re.search(BEFORE[entry], l)), None)
        if k is not None:
            return '\n'.join(lines[:k] + [block] + lines[k:]), 'before /%s/' % BEFORE[entry]
    if at is None:
        return text + '\n' + block, 'module-end'
    return '\n'.join(lines[:at] + [block] + lines[at:]), 'before %x' % hdrs[[n for n, _ in hdrs].index(at)][1]


def compose(entry, inserts, population, options, module_source):
    """inserts: list of (entry, text, replace)."""
    rows = sorted((r for r in cat['regions'] if r['population'] == population and tuple(r['options']) == options),
                  key=lambda r: r['source_order'])
    sources = list(OrderedDict.fromkeys(r['source'] for r in rows))
    texts = OrderedDict((s, build.source_path(R, s).read_text(errors='replace')) for s in sources)
    notes = []
    for e, block, replace in inserts:
        r = accepted.get(e)
        if replace and r and r['source'] in texts:
            src = r['source']
            if src == module_source:
                t, w = insert_module(texts[src], e, block, True)
                if t is None:
                    notes.append('X %x not found in module' % e); continue
                texts[src] = t; notes.append('X %x %s' % (e, w)); continue
            texts[src] = block; notes.append('X %x file' % e); continue
        if replace and r:
            notes.append('X %x accepted elsewhere (%s), skipped' % (e, r['population'])); continue
        if module_source in texts:
            t, w = insert_module(texts[module_source], e, block)
            texts[module_source] = t; notes.append('%x %s' % (e, w))
        else:
            keys = list(texts)
            k = next((i for i, s in enumerate(keys)
                      if min(rr['entry'] for rr in rows if rr['source'] == s) > e), len(keys))
            items = list(texts.items()); items.insert(k, ('__ins_%x' % e, block))
            texts = OrderedDict(items); notes.append('%x pop-order %d/%d' % (e, k, len(keys)))
    return '\n'.join(texts.values()), notes


def strip_dupes(draft_text, context):
    """Drop draft top-level static/inline helper blocks and #define lines already present verbatim."""
    out, lines, i = [], draft_text.split('\n'), 0
    while i < len(lines):
        line = lines[i]
        if re.match(r'^\s*#\s*define\b', line) and line.strip() in context:
            i += 1; continue
        if re.match(r'^(static\s+)?inline\b|^static\b', line) and '(' in line:
            j, depth, seen = i, 0, False
            while j < len(lines):
                depth += lines[j].count('{') - lines[j].count('}')
                seen = seen or '{' in lines[j]
                if (seen and depth == 0) or (not seen and lines[j].rstrip().endswith(';')):
                    break
                j += 1
            block = '\n'.join(lines[i:j + 1])
            if block.strip() in context:
                i = j + 1; continue
        out.append(line); i += 1
    return '\n'.join(out)


def path_of(p, base=None):
    p = Path(p)
    if p.is_absolute():
        return p
    for b in ([base] if base else []) + [CW, R]:
        if (b / p).exists():
            return b / p
    return CW / p


items = json.load(open(lst))
_rn = {}
for _m in _args.rename_map:
    _rn.update(json.load(open(_m)))
_IDW = re.compile(r'[A-Za-z_]\w*')
def rebase(text, on):
    return _IDW.sub(lambda m: _rn.get(m.group(0), m.group(0)), text) if on else text
units, plan = [], []
for n, it in enumerate(items):
    entry, length = int(str(it['entry']), 16), int(it['length'])
    base = path_of(it['base']) if it.get('base') else None
    draft = path_of(it['draft'], base)
    d = out / ('%03d_%x_%s' % (n, entry, it.get('mode', 'J'))); d.mkdir(exist_ok=True)
    for p in ('joined.obj', 'returncode.txt', 'compile.log'):
        (d / p).unlink(missing_ok=True)
    rec = dict(it, entry='%x' % entry, draft=str(draft), dir=str(d))
    if not draft.exists():
        plan.append((rec, None)); rec['status'] = 'missing-draft'; continue
    raw = rebase(draft.read_text(errors='replace'), it.get('rename'))
    pre = rebase(path_of(it['pre'], base).read_text(errors='replace'), it.get('rename')) + '\n' if it.get('pre') else ''
    inc = [path_of(it['inc'], base)] if it.get('inc') else []
    if it.get('mode', 'J') == 'S':
        unit, pop, opts, mod = group_for(entry)
        text = pre + raw; rec.update(unit=unit, population=pop, where='standalone')
    else:
        unit, pop, opts, mod = group_for(entry)
        ctx, _ = compose(entry, [], pop, opts, mod)
        if it.get('before'):
            BEFORE[entry] = it['before']
        inserts = [(entry, strip_dupes(raw, ctx), False)]
        for a, p in (it.get('x') or {}).items():
            inserts.append((int(a, 16), rebase(path_of(p, base).read_text(errors='replace'), it.get('rename')), True))
        text, notes = compose(entry, inserts, pop, opts, mod)
        text = pre + text
        rec.update(unit=unit, population=pop, where='; '.join(notes))
    if it.get('opt') is not None:
        opts = tuple(dc.OPTIONS[int(it['opt'])])
    src = d / 'joined.cpp'; src.write_text(text)
    args = canonical.compiler_arguments(list(opts)) + ['/I' + build.win(i) for i in inc]
    units.append(dict(cwd=d, source=src, obj=d / 'joined.obj', log=d / 'compile.log',
                      returncode=d / 'returncode.txt', args=args))
    plan.append((rec, d))
if units:
    build.compile_units(cfg, out, units, 'rs-compile')

audit = Audit(R, cfg['executable'])
decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
refs_base = dc.known_references(cat)
for name, value in audit.original_type_descriptors().items():
    refs_base.setdefault(name, value)
results = []
for rec, d in plan:
    if d is None:
        results.append(rec); print(json.dumps(rec), flush=True); continue
    entry, length = int(rec['entry'], 16), int(rec['length'])
    obj = d / 'joined.obj'
    if not obj.exists():
        log = (d / 'compile.log').read_text(errors='replace') if (d / 'compile.log').exists() else ''
        rec['status'] = 'compile-failed'
        rec['errors'] = [l.strip()[-200:] for l in log.splitlines() if ' error ' in l or 'fatal error' in l][:5]
        results.append(rec); print(json.dumps(rec), flush=True); continue
    refs = dict(refs_base)
    pj = R / 'manifests/original_target/populations' / ('%s.json' % rec['population'])
    if pj.exists():
        for k, v in json.load(open(pj)).get('references', {}).items():
            refs[k] = v if isinstance(v, int) else int(v, 16)
    for k, v in dc.SOURCE_REF.findall(Path(rec['draft']).read_text(errors='replace')):
        refs[k] = int(v, 16)
    coff = Coff(obj.read_bytes())
    sym = rec.get('symbol')
    fn = [s['name'] for s in coff.symbols.values() if s['section'] > 0 and s['type'] & 0x20 and
          s['storage'] == 2 and coff.sections[s['section'] - 1]['flags'] & 0x20]
    if sym:
        sym = rebase(sym, rec.get('rename'))
        cands = [n for n in fn if n.startswith(sym)] or [n for n in fn if n not in accepted_symbols]
    else:
        cands = [n for n in fn if n not in accepted_symbols] if rec.get('mode', 'J') == 'J' else fn
    original_bytes = audit.original(entry, length)
    original = dc.disassemble(decoder, original_bytes, entry)
    best = None
    for c in cands:
        try:
            symbol, section, start, end = coff.function(c)
        except Exception:
            continue
        relocs = [(o - start, s, k) for o, s, k in section['relocs'] if start <= o < end]
        data, unresolved = dc.resolve(section['data'][start:end], relocs, coff.symbols, entry, refs)
        body, padding = dc.split_padding(data, length)
        cand = dc.disassemble(decoder, body, entry, unresolved)
        summary, pairs = dc.score(original, cand)
        exact = not unresolved and body == original_bytes
        r = dict(symbol=symbol['name'], exact=exact, unresolved=sorted(set(unresolved.values())), **summary)
        if best is None or (r['exact'], r['score']) > (best[0]['exact'], best[0]['score']):
            best = (r, pairs)
    if best is None:
        rec['status'] = 'no-symbol'; rec['candidates'] = cands[:8]
    else:
        r, pairs = best
        rec.update(status='EXACT' if r['exact'] else 'miss',
                   **{k: r[k] for k in ('symbol', 'score', 'loose_score', 'shape_score', 'original_instructions',
                                        'candidate_instructions', 'unresolved')})
        (d / 'diff.txt').write_text(dc.render(pairs, context=None))
    results.append(rec); print(json.dumps({k: v for k, v in rec.items() if k not in ('x',)}), flush=True)
json.dump(results, open(out / 'results.json', 'w'), indent=1)
