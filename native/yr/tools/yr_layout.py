#!/usr/bin/env python3
"""yr_layout.py <Class> [header ...]: the class's 32-bit (VC6-equivalent i686 MS layout) field offsets
from the YR tree's headers, via the local llvm-mingw clang -fdump-record-layouts.
First copies the pinned headers (lower-case names, TibSun under ra2-decomp) to build/yr/layout/hdr. Prints
'offset<TAB>size?<TAB>path' for every leaf field (nested members flattened)."""
import subprocess, sys, re, os
from pathlib import Path
W = Path(__file__).resolve().parents[3]
TOOLCHAIN = 'private/toolchains/llvm-mingw-20261020/llvm-mingw-20261020-ucrt-macos-universal'
M = os.environ.get('YR_LLVM_MINGW') or next((str(r / TOOLCHAIN) for r in [W] + list(W.parents) if (r / TOOLCHAIN).exists()), str(W / TOOLCHAIN))
cls = sys.argv[1]
headers = sys.argv[2:] or ['always.h', cls.lower().replace('class', '') + '.h']
hdr = W / 'build/yr/layout/hdr'
if not hdr.exists():
    for base in (W / 'yr/tibsun/code', W / 'yr/ra2-decomp/include'):
        for h in base.rglob('*'):
            if h.is_file() and h.suffix.lower() in ('.h', '.hh', '.hpp', '.inl'):
                t = hdr / h.relative_to(base).as_posix().lower(); t.parent.mkdir(parents=True, exist_ok=True); t.write_bytes(h.read_bytes())
probe = W / 'build/yr/layout' / ('probe_%s.cpp' % cls)
probe.write_text(''.join('#include "%s"\n' % h for h in ['always.h'] + headers) + 'int probe_size = sizeof(%s);\n' % cls)
out = subprocess.run([M + '/bin/i686-w64-mingw32-clang++', '-fms-extensions', '-fsyntax-only', '-w', '-fdelayed-template-parsing',
                      '-mms-bitfields', '-I', str(W / 'build/yr/layout/hdr'), '-D_WIN32', '-DWIN32', '-Xclang', '-fdump-record-layouts',
                      str(probe)], capture_output=True, text=True)
text = out.stdout
errs = [l for l in out.stderr.splitlines() if 'error' in l]
if errs:
    print('\n'.join(errs[:10]), file=sys.stderr)
start = text.find('\n         0 | class %s\n' % cls)
if start < 0:
    start = text.find('\n         0 | struct %s\n' % cls)
block = text[start:text.find('sizeof=', start)]
stack = []
for line in block.splitlines()[1:]:
    m = re.match(r'\s*(\d+)(?::(\d+)-(\d+))? \|( +)(.*)$', line)
    if not m:
        continue
    off, depth, decl = int(m.group(1)), (len(m.group(4)) - 1) // 2, m.group(5)
    if decl.startswith('('):
        name, kind = '<vptr>', 'vptr'
    elif decl.endswith('base)'):
        name, kind = '<' + decl.split()[1] + '>', 'base'
    else:
        name, kind = decl.split()[-1], 'field'
    stack = stack[:depth - 1] + [name]
    print('0x%X\t%s\t%s\t%s' % (off, '.'.join(x for x in stack if not x.startswith('<') or x == '<vptr>'), kind, decl))
print('sizeof', text[text.find('sizeof=', start):text.find(',', text.find('sizeof=', start))], file=sys.stderr)
