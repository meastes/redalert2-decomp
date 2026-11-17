#!/usr/bin/env python3
"""yr_ctor_stores.py <layout.tsv> <address> <size> [this-reg]: list a constructor's stores through
`this` (esi by default) with the field each one writes and the value, register constants
tracked; doubles joined from their two dwords. Output: one line per store, C++-like (lane Y3;
used for the TechnoTypeClass constructor draft). Layout from yr_layout.py."""
import sys, re, struct, subprocess
lay, addr, size = sys.argv[1], sys.argv[2], sys.argv[3]
this = sys.argv[4] if len(sys.argv) > 4 else 'esi'
dis = subprocess.run([sys.executable, str(__import__('pathlib').Path(__file__).with_name('yr_dis.py')), addr, size], capture_output=True, text=True).stdout.splitlines()
fields = []
for l in open(lay):
    p = l.rstrip('\n').split('\t')
    if len(p) == 4 and p[0].startswith('0x') and p[2] == 'field':
        fields.append((int(p[0], 16), p[1], p[3]))
def tsize(decl):
    t = decl.rsplit(' ', 1)[0]
    m = re.search(r'\[(\d+)\]', t)
    base = re.sub(r'\[\d+\]', '', t).strip()
    n = int(m.group(1)) if m else 1
    sz = {'_Bool': 1, 'char': 1, 'unsigned char': 1, 'signed char': 1, 'short': 2, 'unsigned short': 2, 'wchar_t': 2,
          'double': 8, 'float': 4, '__int64': 8, 'long long': 8, 'unsigned long long': 8}.get(base, 4)
    return base, n, sz
def field_at(off):
    best = None
    for o, path, decl in fields:
        base, n, sz = tsize(decl)
        if o <= off < o + n * sz and (best is None or o >= best[0]):
            if not (decl.startswith(('class ', 'struct ', 'union ')) and '[' not in decl):
                best = (o, path, decl)
    return best
R32 = {'al': 'eax', 'ax': 'eax', 'bl': 'ebx', 'bx': 'ebx', 'cl': 'ecx', 'cx': 'ecx', 'dl': 'edx', 'dx': 'edx'}
regs, base_of = {}, {'ecx': 0}
stores = []
for line in dis:
    parts = line.split(' ', 2)
    if len(parts) < 2:
        continue
    mn, ops = parts[1], parts[2] if len(parts) > 2 else ''
    ops = ops.split('   ;')[0]
    if mn == 'call':
        for r in ('eax', 'ecx', 'edx'):
            regs.pop(r, None); base_of.pop(r, None)
        stores.append((None, 'call', ops + '   ;' + line.split('   ;')[1] if '   ;' in line else ops))
        continue
    m = re.match(r'(byte|word|dword|qword) ptr \[(\w+)(?: \+ (0x[0-9a-f]+|\d+))?\], (.+)$', ops)
    if mn == 'mov' and m and m.group(2) in base_of:
        w = {'byte': 1, 'word': 2, 'dword': 4}[m.group(1)]
        off = base_of[m.group(2)] + (int(m.group(3), 0) if m.group(3) else 0)
        src = m.group(4)
        val = int(src, 0) if re.match(r'^-?(0x[0-9a-f]+|\d+)$', src) else regs.get(R32.get(src, src))
        stores.append((off, w, val if val is not None else src))
        continue
    if mn in ('fstp', 'fst') and m is None:
        m2 = re.match(r'(dword|qword) ptr \[(\w+)(?: \+ (0x[0-9a-f]+|\d+))?\]', ops)
        if m2 and m2.group(2) in base_of:
            off = base_of[m2.group(2)] + (int(m2.group(3), 0) if m2.group(3) else 0)
            stores.append((off, 8 if m2.group(1) == 'qword' else 4, 'fpu:' + last_fld))
        continue
    if mn.startswith('fld'):
        last_fld = ops
    dst = ops.split(',')[0].strip()
    d32 = R32.get(dst, dst)
    if mn == 'xor' and ops == '%s, %s' % (dst, dst):
        regs[d32] = 0; base_of.pop(d32, None); continue
    mm = re.match(r'(\w+), (-?0x[0-9a-f]+|\d+)$', ops)
    if mn == 'mov' and mm:
        regs[d32] = int(mm.group(2), 0); base_of.pop(d32, None); continue
    if mn == 'or' and ops == dst + ', 0xffffffff':
        regs[d32] = 0xffffffff; base_of.pop(d32, None); continue
    ml = re.match(r'(\w+), \[(\w+)(?: \+ (0x[0-9a-f]+|\d+))?\]$', ops)
    if mn == 'lea' and ml and ml.group(2) in base_of:
        base_of[d32] = base_of[ml.group(2)] + (int(ml.group(3), 0) if ml.group(3) else 0); regs.pop(d32, None); continue
    if mn == 'mov' and re.match(r'(\w+), (\w+)$', ops) and ops.split(', ')[1] in base_of:
        base_of[d32] = base_of[ops.split(', ')[1]]; continue
    if mn == 'mov' and re.match(r'(\w+), (\w+)$', ops) and ops.split(', ')[1] in regs:
        regs[d32] = regs[ops.split(', ')[1]]; base_of.pop(d32, None); continue
    if mn in ('mov', 'lea', 'xor', 'or', 'and', 'add', 'sub', 'inc', 'dec', 'pop', 'movzx', 'movsx', 'imul', 'shl', 'shr', 'sar', 'neg'):
        if d32 in regs or d32 in base_of:
            if not (mn in ('add', 'sub') and d32 in base_of):
                regs.pop(d32, None); base_of.pop(d32, None)
            else:
                k = re.match(r'\w+, (0x[0-9a-f]+|\d+)$', ops)
                if k: base_of[d32] += int(k.group(1), 0) * (1 if mn == 'add' else -1)
                else: base_of.pop(d32, None)
i = 0
pending = {}
for off, w, val in stores:
    if off is None:
        print('    // call', val)
        continue
    f = field_at(off)
    if f is None:
        print('    // +0x%X (%d) = %s   ?' % (off, w, val)); continue
    o, path, decl = f
    base, n, sz = tsize(decl)
    if base == 'double' and isinstance(val, int):
        key = o
        pending.setdefault(key, {})[off - o] = val
        if len(pending[key]) == 2 and n == 1:
            v = struct.unpack('<d', struct.pack('<II', pending[key][0] & 0xffffffff, pending[key][4] & 0xffffffff))[0]
            print('    %s = %r;   // +0x%X' % (path, v, o)); del pending[key]
        continue
    if base == 'float' and isinstance(val, int) and w == 4:
        v = struct.unpack('<f', struct.pack('<I', val & 0xffffffff))[0]
        print('    %s = %rf;   // +0x%X' % (path, v, o)); continue
    idx = '' if n == 1 else '[%d]' % ((off - o) // sz)
    note = '' if w == sz else '   // %d-byte store over %s' % (w, decl)
    if isinstance(val, int):
        if val & 0x80000000 and w == 4: val -= 1 << 32
        sval = str(val) if -4096 < val < 4096 else hex(val & 0xffffffff)
    else:
        sval = str(val)
    print('    %s%s = %s;   // +0x%X%s' % (path, idx, sval, off, note))
for k, v in pending.items():
    print('    // double at +0x%X partial %s' % (k, v))
