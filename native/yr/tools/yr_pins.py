#!/usr/bin/env python3
"""Regenerate native/yr/data/globals.tsv (lane Y2): the gamemd.exe 1.001 address of every YR global that
matched code references, by name.

Runs ra2-decomp's own exelink tooling (MIT) in a local working clone of the pinned commit whose
toolchain is set up (VC6 SP3 under Wine; tools/setup_toolchain.sh there) and whose objects are built
(`uv run tools/exelink/build.py`). For every relocation site in a function whose compiled bytes equal
the retail ones, retail dword minus addend is the referenced symbol's address (their pindata.py);
names that derive to one address in .rdata/.data are written. Nothing is published or pushed; the
output holds names and addresses only.

    cd <ra2-decomp working clone> && uv run --offline python <repo>/native/yr/tools/yr_pins.py \\
        --exe <unpatched gamemd.exe 1.001> --out <repo>/native/yr/data/globals.tsv
"""
import argparse
import hashlib
import json
import os
import re
import sys
from pathlib import Path


def key(msvc):
    """C++ qualified or C name of an MSVC data symbol."""
    if msvc.startswith('?') and not msvc.startswith('??'):
        m = re.match(r'\?([A-Za-z_]\w*)((?:@[A-Za-z_][\w$]*)*)@@[0-6]', msvc)
        if m:
            return '::'.join(reversed([m.group(1)] + [p for p in m.group(2).split('@') if p]))
        return None
    if msvc.startswith('_') and '@' not in msvc:
        return msvc[1:]
    return None


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--exe', type=Path, required=True)
    ap.add_argument('--out', type=Path, required=True)
    a = ap.parse_args()
    root = Path.cwd()
    sys.path.insert(0, str(root / 'tools/exelink'))
    sys.path.insert(0, str(root / 'tools'))
    import paths
    paths.RETAIL.parent.mkdir(parents=True, exist_ok=True)
    paths.RETAIL.write_bytes(a.exe.read_bytes())
    os.environ['GAMEMD_BINARY'] = str(paths.RETAIL)
    import prepare
    prepare.write_json(paths.TOTALS, {'_meta': {'sha256': hashlib.sha256(paths.RETAIL.read_bytes()).hexdigest(), 'build': 'retail'}})
    prepare.write_json(paths.MANIFEST, prepare.manifest_rows())
    prepare.write_json(paths.RTTI_CLASSES, prepare.rtti_classes(paths.RETAIL.read_bytes()))
    prepare.write_json(paths.BINDINGS, prepare.bindings())
    import hybrid
    import link
    import pindata
    index = json.loads((paths.OBJDIR / 'objects.json').read_text())
    retail = hybrid.Retail(pindata.retail_path())
    pe, _, secs = pindata.load_retail()
    rows = link.equal_rows(paths.OBJDIR, index, link.load_manifest(), retail)
    plan = pindata.derive(paths.OBJDIR, index, rows, pe, secs)
    out, ambiguous = {}, set()
    for msvc, va in plan['global'].items():
        name = key(msvc)
        if va < 0x7E1000 or not name:
            continue
        if name in out and out[name][0] != va:
            ambiguous.add(name)
        out.setdefault(name, (va, msvc))
    for name in ambiguous:
        out.pop(name)
    with a.out.open('w') as f:
        f.write("# YR globals (gamemd.exe 1.001) by name: the retail address matched YR code references (lane Y2).\n"
                "# Derived from ra2-decomp's pindata.py (MIT tooling) over their VC6 objects of the pinned tree: for each\n"
                "# relocation site in a function whose compiled bytes equal retail, retail dword - addend. Names only\n"
                "# that derive to one address. Addresses and names only, no bytes. Regenerate: native/yr/tools/yr_pins.py.\n"
                "name\taddress\tmsvc\n")
        for name, (va, msvc) in sorted(out.items(), key=lambda r: r[1][0]):
            f.write('%s\t0x%08X\t%s\n' % (name, va, msvc))
    print('yr_pins: %d globals (%d equal rows, %d ambiguous left out)' % (len(out), len(rows), len(ambiguous)))


if __name__ == '__main__':
    main()
