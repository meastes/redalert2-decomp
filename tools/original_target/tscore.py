#!/usr/bin/env python3
"""tscore.py OBJ SYMBOL_PREFIX ENTRY LENGTH [--src DRAFT.cpp] [--config CFG] [--out FILE] [--context N]
Table-aware score of one compiled object against the original: VC6 switch tables ($L-labelled DD jump tables and
their byte index tables) that lie INSIDE the function's extent are stripped from the candidate bytes before decoding,
so a candidate whose code is longer than the original is still aligned on instructions only. Everything else is
diff_candidate's own resolve/disassemble/score/render. (Scout1217, 2026-11-10.)"""
import sys, re, argparse, struct
from pathlib import Path
R = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(R / 'tools/original_target'))
import diff_candidate as dc, catalog_io
from coff import Coff
from verify_manifest import Audit
import json

def table_ranges(coff, section, start, end):
    """VC6 places a function's switch tables after its last instruction: DD jump tables (whose entries are DIR32
    relocations to code labels) and the byte index tables between them. The first DD table is the smallest
    relocation target inside the function that itself carries a relocation; everything from there to the end is table."""
    reloc_at = {o for o, s, k in section['relocs'] if start <= o < end}
    targets = sorted({coff.symbols[s]['value'] for o, s, k in section['relocs']
                      if start <= o < end and k == 6 and coff.symbols[s]['name'].startswith('$L')
                      and start <= coff.symbols[s]['value'] < end and coff.symbols[s]['section'] == coff.sections.index(section) + 1})
    tables = [t for t in targets if t in reloc_at]
    return [(tables[0], end)] if tables else []

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('obj'); ap.add_argument('symbol'); ap.add_argument('entry'); ap.add_argument('length', type=int)
    ap.add_argument('--src', action='append', default=[]); ap.add_argument('--config', default=str(R / 'private/original-target.w3.json'))
    ap.add_argument('--out'); ap.add_argument('--context', type=int, default=2)
    a = ap.parse_args()
    import capstone
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    config = json.load(open(a.config)); audit = Audit(R, config['executable'])
    entry = int(a.entry, 16)
    cat = catalog_io.load(catalog_io.locate(R), references=False)
    references = dict(dc.known_references(cat))
    for src in a.src:
        for name, value in dc.SOURCE_REF.findall(Path(src).read_text(errors='replace')): references[name] = int(value, 16)
    coff = Coff(Path(a.obj).read_bytes())
    symbol, section, start, end = coff.function(a.symbol)
    tables = table_ranges(coff, section, start, end)
    keep = []; cut = 0; pos = start
    for lo, hi in tables:
        keep.append((pos, lo)); pos = hi
    keep.append((pos, end))
    # rebuild the body without the tables, re-offsetting relocations
    body = bytearray(); relocs = []; shift = {}
    for lo, hi in keep:
        base = len(body)
        for o, s, k in section['relocs']:
            if lo <= o < hi and o + 4 <= hi: relocs.append((o - lo + base, s, k))
        body += section['data'][lo:hi]
    data, unresolved = dc.resolve(bytes(body), relocs, coff.symbols, entry, references)
    data, padding = dc.split_padding(data, a.length)
    # the dispatch instructions' table operands reference stripped labels: leave them unresolved (printed as ?$L)
    original = dc.disassemble(decoder, audit.original(entry, a.length), entry)
    candidate = dc.disassemble(decoder, data, entry, unresolved)
    summary, pairs = dc.score(original, candidate)
    stripped = sum(hi - lo for lo, hi in tables)
    print('%s tables %d (%d bytes stripped) body %d/%d bytes  score %6.2f loose %6.2f shape %6.2f insns %d/%d unresolved %s' % (
        Path(a.obj).stem, len(tables), stripped, len(data), a.length, summary['score'], summary['loose_score'], summary['shape_score'],
        summary['original_instructions'], summary['candidate_instructions'], sorted(set(unresolved.values()))[:4]))
    if a.out: Path(a.out).write_text(dc.render(pairs, context=a.context) + '\n')
if __name__ == '__main__': main()
