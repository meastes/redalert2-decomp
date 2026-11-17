#!/usr/bin/env python3
"""yr_dis.py <address> <size>: disassemble gamemd.exe (read only) with the YR tree's names (functions.csv,
manifest.csv, native/yr/data/globals.tsv), import names and short strings annotated. Needs capstone
(the matching-pilot Python, $RA2_PYTHON). gamemd.exe: $YR_GAMEMD (default: the user's install).
Lane Y3 research helper for native drafts; reads the executable only, writes nothing."""
import capstone, struct, csv, sys, re, os
from pathlib import Path
W = str(Path(__file__).resolve().parents[3]) + '/'
d = open(os.environ.get('YR_GAMEMD', '/Volumes/Lesalia/Command and Conquer Red Alert II/gamemd.exe'), 'rb').read()
pe=struct.unpack_from('<I',d,0x3c)[0]; n=struct.unpack_from('<H',d,pe+6)[0]; opt=struct.unpack_from('<H',d,pe+20)[0]
base=0x400000; s=pe+24+opt; secs=[]
for i in range(n):
    vs,va,rs,rp=struct.unpack_from('<IIII',d,s+8); secs.append((base+va,rs,rp)); s+=40
def read(a,n):
    for va,rs,rp in secs:
        if va<=a<va+rs: return d[rp+a-va:rp+a-va+n]
names={}
# imports (IAT slot -> name)
def u32(a): return struct.unpack('<I', read(a,4))[0]
imp_rva=struct.unpack_from('<I',d,pe+24+104)[0]
a=base+imp_rva
while True:
    olt,ts,fc,nm,iat=struct.unpack('<5I',read(a,20))
    if nm==0: break
    k=0
    while True:
        th=u32(base+(olt or iat)+4*k)
        if th==0: break
        if not th&0x80000000:
            b=read(base+th+2,64); names[base+iat+4*k]='IMP:'+b[:b.find(b'\0')].decode()
        k+=1
    a+=20

for r in csv.DictReader(open(W+'yr/ra2-decomp/data/functions.csv')):
    if r['name']: names[int(r['address'],16)]=r['name']
for r in csv.DictReader(open(W+'yr/ra2-decomp/data/manifest.csv')):
    names.setdefault(int(r['address'],16), r['symbol'])
for l in open(W+'native/yr/data/globals.tsv'):
    if l.startswith(('#','name\t')): continue
    nm,a,_=l.rstrip('\n').split('\t'); names.setdefault(int(a,16),nm)
def strat(a):
    b=read(a,80)
    if not b: return None
    e=b.find(b'\0')
    if e>2 and all(32<=c<127 for c in b[:e]): return b[:e].decode()
md=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32)
a0=int(sys.argv[1],16); ln=int(sys.argv[2])
for i in md.disasm(read(a0,ln),a0):
    txt=i.op_str
    for m in re.findall(r'0x[0-9a-f]{6,8}',txt):
        v=int(m,16)
        if v in names: txt+='   ; '+names[v]
        else:
            st=strat(v)
            if st: txt+='   ; "%s"'%st[:60]
    if True:
        print(hex(i.address), i.mnemonic, txt)
