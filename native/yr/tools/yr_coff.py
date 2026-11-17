#!/usr/bin/env python3
"""COFF spellings of the YR units' assembler labels (the x86_64 Windows build; lane Y13).

The generated YR units (native/yr/fixups.json "append" texts among them) name symbols in
assembler labels in the Mach-O spelling, as the RA2 tree's units do; native/tools/coff_asm_labels.py
drops the one leading underscore for COFF. This writes a copy in <gen>/coff/units of each unit that
holds a label (when its text changes) and <out>, the units.cmake to compile (YR_NATIVE_UNITS).

    python3 native/yr/tools/yr_coff.py --gen <gen-yr> --out <gen-yr>/coff_units.cmake
"""
import argparse
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / 'tools'))
import coff_asm_labels  # noqa: E402


# File-scope assembly written as several string literals (the glue's `.globl` + `=` aliases, one
# literal per line), which coff_asm_labels.py's one-literal pattern does not take.
MULTI_ASM = re.compile(r'^__asm__\(((?:\s*"(?:[^"\\]|\\.)*")+)\s*\);', re.M)
LITERAL = re.compile(r'"((?:[^"\\]|\\.)*)"')


def coff_text(text):
    def multi(m):
        body = LITERAL.sub(lambda l: '"%s"' % coff_asm_labels.SYMBOL.sub('', l.group(1)), m.group(1))
        return '__asm__(%s);' % body
    return coff_asm_labels.coff_text(MULTI_ASM.sub(multi, text))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--gen', type=Path, required=True)
    ap.add_argument('--out', type=Path, required=True)
    a = ap.parse_args()
    units = re.findall(r'"([^"]+)"', (a.gen / 'units.cmake').read_text())
    names, copies = [], 0
    for unit in units:
        path = Path(unit)
        text = path.read_text(errors='replace')
        if not (coff_asm_labels.LABEL.search(text) or MULTI_ASM.search(text)):
            names.append(unit)
            continue
        copy = a.gen / 'coff' / 'units' / path.name
        copy.parent.mkdir(parents=True, exist_ok=True)
        new = '#line 1 "%s"\n' % path + coff_text(text)
        if not copy.exists() or copy.read_text(errors='replace') != new:
            copy.write_text(new)
        names.append(str(copy))
        copies += 1
    a.out.write_text('set(YR_NATIVE_UNITS\n%s)\n' % ''.join('  "%s"\n' % n for n in names))
    print('yr_coff: %d of %d units with COFF labels' % (copies, len(names)))


if __name__ == '__main__':
    main()
