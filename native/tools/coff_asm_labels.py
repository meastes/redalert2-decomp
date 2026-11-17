#!/usr/bin/env python3
"""COFF spellings of the Mach-O symbol names in native sources (the x86_64 Windows build).

The native data and link sources and the generated units name symbols directly
in assembler labels (`__asm__("_name")` on a declaration) and in file-scope
assembly (`.globl _a` / `.set _a, _b + 16`), in the Mach-O spelling: the C name
with a leading underscore. Windows x86_64 COFF adds no underscore, so the Windows
build compiles each such source with labels that drop that one underscore
(`_name` -> `name`, `__ZN...` -> `_ZN...`); nothing else changes.

    python3 native/tools/coff_asm_labels.py <source.cpp> <copy.cpp>
        a copy for a tracked source (a #line directive keeps diagnostics on it;
        the build reruns it when the source changes)
    python3 native/tools/coff_asm_labels.py --units <gen> <units.cmake>
        the generated units (configure time): a copy in <gen>/coff/units of each
        unit that holds a label, written when its text changes, and <units.cmake>
        naming the units to compile (RA2_NATIVE_UNITS)

A `.set` alias whose target another object defines has no COFF form: the
assembler leaves the alias undefined, and link_closure.py adds it to the
target's object (coff_aliases.py).
"""
import re
import sys
from pathlib import Path

LABEL = re.compile(r'__asm__\("_')
TOP_ASM = re.compile(r'^(__asm__\(")((?:[^"\\]|\\.)*)("\);)', re.M)
SYMBOL = re.compile(r'(?<![\w.$])_(?=[A-Za-z_])')


def coff_text(text):
    def top(m):
        body = m.group(2).replace('\\n', '\n')
        body = SYMBOL.sub('', body)
        return m.group(1) + body.replace('\n', '\\n') + m.group(3)
    return LABEL.sub('__asm__("', TOP_ASM.sub(top, text))


def main():
    args = sys.argv[1:]
    if args[:1] == ['--units'] and len(args) == 3:
        gen, listing = Path(args[1]), Path(args[2])
        units = re.findall(r'"\$\{RA2_NATIVE_GEN\}/units/([^"]+)"', (gen / 'units.cmake').read_text())
        names = []
        for unit in units:
            text = (gen / 'units' / unit).read_text(errors='replace')
            if not (LABEL.search(text) or TOP_ASM.search(text)):
                names.append('units/' + unit)
                continue
            copy = gen / 'coff' / 'units' / unit
            copy.parent.mkdir(parents=True, exist_ok=True)
            new = coff_text(text)
            if not copy.exists() or copy.read_text(errors='replace') != new:
                copy.write_text(new)
            names.append('coff/units/' + unit)
        listing.write_text('set(RA2_NATIVE_UNITS\n%s)\n' % ''.join('  "${RA2_NATIVE_GEN}/%s"\n' % n for n in names))
        return
    if len(args) != 2:
        sys.exit(__doc__)
    source, copy = Path(args[0]).resolve(), Path(args[1])
    copy.parent.mkdir(parents=True, exist_ok=True)
    copy.write_text('#line 1 "%s"\n' % source + coff_text(source.read_text()))


if __name__ == '__main__':
    main()
