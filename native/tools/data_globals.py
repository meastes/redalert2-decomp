#!/usr/bin/env python3
"""Native definitions for the globals the recovered code reads but no source defines.

Input: the undefined symbols of a full native build (`nm -u`-style list, one
symbol per line, e.g. from tools/link_closure.py --list-undefined). Each data
symbol is bound to its original address through the catalog's reference tables
(MSVC name -> address) or its address-suffixed name, and to its section through
the pinned target's PE section headers (hash-checked; only the headers are read).

  * Zero-initialised (.bss: past the section's file-backed part): defined in
    data/bss_globals.cpp as zero storage of the original extent (distance to the
    next known symbol) doubled, 16-byte aligned. Doubling covers LP64 growth
    (pointers and pointer-aligned members at most double a layout). Objects of
    class type are constructed by their recovered initialisers when those are in
    the tree (_initterm order, native/initterm.json); otherwise they are listed.
  * Initialised (.data/.rdata file-backed): never defined from the executable.
    Listed in data/unsourced_initialised.tsv with size and users; each needs
    adopted source text, a generator, or a run-time read of game files.

    python3 native/tools/data_globals.py --undefined build/.../undefined.txt
"""
import argparse
import bisect
import hashlib
import json
import re
import struct
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'tools/original_target'))
import catalog_io  # noqa: E402

SHA256 = 'e6418111c2c36081a1795fb000e0927beea1630170b334c5013d12102602e70f'
DEFAULT_EXE = ROOT / 'private/original-target-20261020/inputs/game.exe'


def sections(path):
    data = path.read_bytes()
    if hashlib.sha256(data).hexdigest() != SHA256:
        raise SystemExit('not the pinned target')
    pe = struct.unpack_from('<I', data, 0x3C)[0]
    count = struct.unpack_from('<H', data, pe + 6)[0]
    optional = struct.unpack_from('<H', data, pe + 20)[0]
    base = struct.unpack_from('<I', data, pe + 24 + 28)[0]
    out = []
    for i in range(count):
        o = pe + 24 + optional + 40 * i
        name = data[o:o + 8].rstrip(b'\0').decode()
        vsize, va, rsize, _raw = struct.unpack_from('<IIII', data, o + 8)
        out.append(dict(name=name, start=base + va, end=base + va + max(vsize, rsize), raw_end=base + va + rsize))
    return out


def msvc_key(msvc):
    """(qualified name, kind) of an MSVC/C symbol."""
    if msvc.startswith('?'):
        m = re.match(r'\?([A-Za-z_]\w*)((?:@[A-Za-z_]\w*)*)@@([0-9A-Z])', msvc)
        if not m:
            return None
        parts = [m.group(1)] + [p for p in m.group(2).split('@') if p]
        return '::'.join(reversed(parts)), 'data' if m.group(3) in '0123456' else 'func'
    if msvc.startswith('_'):
        return re.sub(r'@\d+$', '', msvc[1:]), 'c'
    return None


def demangle(symbols):
    out = subprocess.run(['c++filt'], input='\n'.join(symbols), capture_output=True, text=True, check=True).stdout
    return out.split('\n')[:len(symbols)]


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--undefined', type=Path, required=True)
    ap.add_argument('--exe', type=Path, default=DEFAULT_EXE)
    ap.add_argument('--out-dir', type=Path, default=ROOT / 'native/data')
    ap.add_argument('--defined', type=Path, help='symbols the other link inputs define (one per line)')
    args = ap.parse_args()
    secs = sections(args.exe)
    catalog = catalog_io.load(catalog_io.locate(ROOT), references=True)
    refs, users = {}, {}
    for pop, table in catalog['population_references'].items():
        for name, addr in table.items():
            refs.setdefault(name, addr)
    pop_source = {}
    for r in catalog['regions']:
        refs.setdefault(r['symbol'], int(r['entry']))
        pop_source.setdefault(r['population'], r['source'])
    for pop, table in catalog['population_references'].items():
        for name in table:
            users.setdefault(name, pop_source.get(pop))
    by_key = {}
    for name, addr in refs.items():
        k = msvc_key(name)
        if k:
            by_key.setdefault(k[0], []).append((name, addr, k[1]))
    known = sorted(set(refs.values()))

    def section(addr):
        return next((s for s in secs if s['start'] <= addr < s['end']), None)

    symbols = [s.strip() for s in args.undefined.read_text().split('\n') if s.strip()]
    names = demangle(symbols)
    bss, unsourced, skipped = [], [], []
    for sym, dem in zip(symbols, names):
        if sym.startswith('__Z'):
            if '(' in dem or dem.startswith(('vtable for', 'typeinfo', 'VTT for', 'guard variable')):
                continue
            name = dem
        else:
            name = sym[1:]
        cands = by_key.get(name, [])
        cands = [c for c in cands if c[2] != 'func']
        addr, msvc = (cands[0][1], cands[0][0]) if cands else (None, None)
        if addr is None:
            m = re.search(r'_(?:0x)?([0-9a-fA-F]{6,8})$', name)
            if m and 0x79A000 <= int(m.group(1), 16) < 0xB2D000:
                addr = int(m.group(1), 16)
        if addr is None:
            continue
        s = section(addr)
        if not s or s['name'] not in ('.data', '.rdata'):
            continue
        i = bisect.bisect_right(known, addr)
        nxt = known[i] if i < len(known) else s['end']
        extent = max(1, min(nxt, s['end']) - addr)
        row = dict(symbol=sym, name=name, address='0x%08X' % addr, section=s['name'], extent=extent,
                   msvc=msvc, user=users.get(msvc) if msvc else None)
        if addr >= s['raw_end']:
            bss.append(row)
        else:
            unsourced.append(row)

    # One storage per original address: the other names of an address are
    # assembler aliases of the storage, or (when another link input defines
    # the object) ld64 aliases listed in data/bss_aliases.tsv.
    defined_at = {}
    if args.defined:
        dsyms = [s.strip() for s in args.defined.read_text().split('\n') if s.strip()]
        for sym, dem in zip(dsyms, demangle(dsyms)):
            name = dem if sym.startswith('__Z') else sym[1:]
            if '(' in name:
                continue
            for c in by_key.get(name, []):
                if c[2] != 'func':
                    defined_at.setdefault(c[1], sym)
    external, groups = [], {}
    for row in bss:
        a = int(row['address'], 16)
        if a in defined_at and defined_at[a] != row['symbol']:
            external.append((row['symbol'], defined_at[a], row['address']))
        else:
            groups.setdefault(row['address'], []).append(row)
    lines = ['// generated by native/tools/data_globals.py: zero-initialised globals (.bss of the',
             '// original) that the recovered code reads but no recovered source defines yet.',
             '// Storage only: original extent (to the next known symbol) doubled for LP64,',
             '// 16-byte aligned, zero as the loader gives .bss. No bytes from the executable.',
             '// A source that later defines one of these names takes over (remove it here',
             '// by regenerating from that build\'s undefined list).',
             '']
    def rank(row):
        # The storage takes the most descriptive name; placeholders alias it.
        generic = re.match(r'^(DAT_|_?DAT_|data_|g\d_|unknown_)', row['name']) is not None
        return (generic, row['name'])
    for k, (address, rows) in enumerate(sorted(groups.items())):
        rows = sorted(rows, key=rank)
        owner = rows[0]
        size = max(16, 2 * max(r['extent'] for r in rows))
        lines.append('alignas(16) unsigned char ra2_bss_%04d[%d] __asm__("%s");  // %s %s%s'
                     % (k, size, owner['symbol'], owner['address'], owner['name'][:80],
                        ' (%s)' % owner['msvc'] if owner['msvc'] and owner['msvc'] != '_' + owner['name'] else ''))
        for other in rows[1:]:
            lines.append('__asm__(".globl %s\\n.set %s, %s\\n");  // same address: %s'
                         % (other['symbol'], other['symbol'], owner['symbol'], other['name'][:80]))
    (args.out_dir / 'bss_globals.cpp').write_text('\n'.join(lines) + '\n')
    alias_lines = ['# generated by native/tools/data_globals.py: zero-initialised names whose address another '
                   'link input defines (one storage per address; ld64 -alias_list via link_closure.py).',
                   'alias\ttarget\taddress\tkind\tevidence']
    alias_lines += ['%s\t%s\t%s\tdata\tsame address' % e for e in sorted(external, key=lambda e: e[2])]
    (args.out_dir / 'bss_aliases.tsv').write_text('\n'.join(alias_lines) + '\n')
    tsv = ['# generated by native/tools/data_globals.py: initialised .data/.rdata globals the recovered code '
           'reads that have no permitted source yet (adopted text, generator, or game files at run time). '
           'Not defined natively; the link closure gives them zero placeholders. extent = bytes to the next '
           'known symbol (an upper bound); user = a recovered source that references it.',
           'address\tsection\textent\tsymbol\tname\tmsvc\tuser']
    for row in sorted(unsourced, key=lambda r: r['address']):
        tsv.append('\t'.join(str(row[k] or '') for k in ('address', 'section', 'extent', 'symbol', 'name', 'msvc', 'user')))
    (args.out_dir / 'unsourced_initialised.tsv').write_text('\n'.join(tsv) + '\n')
    print('%d zero-initialised names: %d storages, %d same-address aliases, %d aliases of other inputs; '
          '%d initialised listed as unsourced' % (
              len(bss), len(groups), sum(len(r) - 1 for r in groups.values()), len(external), len(unsourced)))


if __name__ == '__main__':
    main()
