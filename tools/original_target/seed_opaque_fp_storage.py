"""Provisional FP representation views for Ghidra's opaque eight-byte locals.

Only undefined8 storage with observed nonconstant FP producers qualifies. The
same object/address is retained. Concrete integer64 types and numeric64 native
conversions refuse. Native FLD/FSTP corroboration is not source correspondence
or body acceptance; strict original matching remains mandatory.
"""
import re
from seed_wide_partial_stores import _integer_value
FP={'float','double','float10'}

def _slots(body,params,seed):
    view=seed.code_view(body);rows,comma=seed.scalar_declarations(view)
    if comma or re.search(r'#|\b(?:typedef|using|namespace|class|struct|union|enum)\b',view):return None,None,None
    if re.search(r'(?:^|[;{}\n])\s*(?:extern\s+)?[A-Za-z_]\w*(?:\s+[A-Za-z_]\w*)*\s+\**CONCAT44\s*\(',view):return None,None,None
    slots={};locals=set()
    for kind,name in map(seed.param_parts,params):
        if name:slots.setdefault(name,[]).append((kind,'parameter'))
    for kind,name,array in rows:slots.setdefault(name,[]).append((kind,array));locals.add(name)
    if 'CONCAT44' in slots:return None,None,None
    carriers={name for name in locals if slots[name]==[('undefined8',None)]}
    return view,slots,carriers


def _assignments(body,view,seed):
    result=[]
    for match in re.finditer(r'(?m)^[\t ]*(?P<name>[A-Za-z_]\w*)[\t ]*=(?!=)\s*',view):
        before=view[:match.start()].rstrip()
        if before and before[-1] not in '{;}':continue
        end=seed.expression_end(view,match.end())
        if end>=len(view) or view[end]!=';':continue
        if body[match.start('name'):end]!=view[match.start('name'):end]:continue
        result.append((match,end,body[match.end():end].strip()))
    return result


def _fp_producer(rhs,slots,seed):
    text=seed.code_view(rhs).strip()
    if re.search(r'\bCONCAT[0-9]+\b',text):return False
    # A floating subexpression does not make a boolean, integer or conditional
    # result floating. Nested operations inside an explicit cast remain inside
    # that cast; member arrows are not comparison operators.
    depth=0;i=0
    while i<len(text):
        char=text[i]
        if char in '([':depth+=1
        elif char in ')]':depth-=1
        elif depth==0:
            if text[i:i+2]=='->':i+=2;continue
            if char in '<>=!&|^%?:,':return False
        if depth<0:return False
        i+=1
    if depth:return False
    if not any(name in slots and len(slots[name])==1 and not re.search(r'\b(?:const|volatile)\b',slots[name][0][0]) for name in re.findall(r'\b[A-Za-z_]\w*\b',text)):return False
    if re.match(r'^\(\s*double\s*\)',text) or re.match(r'^\*\s*\(\s*(?:float|double|float10)\s*\*\s*\)',text):return True
    match=re.fullmatch(r'(\*\s*)?([A-Za-z_]\w*)',text)
    if not match:return False
    rows=slots.get(match[2],[])
    if len(rows)!=1 or rows[0][1] not in (None,'parameter'):return False
    kind=seed.clean_type(rows[0][0])
    if re.search(r'\b(?:const|volatile)\b',kind):return False
    return kind in FP if not match[1] else kind.endswith('*') and seed.clean_type(kind.rstrip('*').strip()) in FP


def source_fp_carriers(body,params,seed):
    view,slots,carriers=_slots(body,params,seed)
    if view is None:return set()
    assignments=_assignments(body,view,seed)
    return {match['name'] for match,end,rhs in assignments if match['name'] in carriers and not re.search(r'\b'+re.escape(match['name'])+r'\b',rhs) and _fp_producer(rhs,slots,seed)}


def fp_storage_hypotheses(body,params,seed,facts):
    if not facts.get('original_ranges_verified') or not facts.get('fp64_loads') or not facts.get('fp64_stores') or facts.get('integer64_conversions')!=0:return body,[]
    view,slots,carriers=_slots(body,params,seed)
    if view is None:return body,[]
    active=source_fp_carriers(body,params,seed)
    if not active:return body,[]
    assignments=_assignments(body,view,seed);edits=[];whole=[]
    def fp_kind(name):
        rows=slots.get(name,[])
        return len(rows)==1 and rows[0][1] in (None,'parameter') and seed.clean_type(rows[0][0]) in FP
    # A complete preserved-low-DWORD store, not a numeric conversion of uint64.
    for match,end,rhs in assignments:
        slot=match['name'];rows=slots.get(slot,[])
        if slot not in active and rows!=[('double',None)]:continue
        concat=re.match(r'^\(\s*double\s*\)\s*CONCAT44\s*\(',rhs)
        if not concat:continue
        opening=rhs.index('(',rhs.index('CONCAT44'));closing=seed.matching_paren(rhs,opening)
        if rhs[closing+1:].strip():continue
        args=seed.split_args(rhs[opening+1:closing])
        if len(args)!=2:continue
        offset=0
        if re.fullmatch(re.escape(slot)+r'\s*\.\s*_4_4_',args[0].strip()):value=args[1].strip()
        elif slot in active and re.fullmatch(r'\(\s*(?:int|uint|undefined4|long|ulong)\s*\)\s*'+re.escape(slot),args[1].strip()):offset=4;value=args[0].strip()
        else:continue
        pointer=re.fullmatch(r'\(\s*([A-Za-z_]\w*(?:\s+[A-Za-z_]\w*)*\s*\*)\s*\)\s*([A-Za-z_]\w*)',value)
        pointer_ok=False
        if pointer and not re.search(r'\b(?:const|volatile|__ptr64|far|near)\b',pointer[1]):
            declared=slots.get(pointer[2],[]);pointer_ok=len(declared)==1 and not declared[0][1] and seed.clean_type(declared[0][0])=='undefined8'
        if not pointer_ok and not _integer_value(value,slots,seed):continue
        first=match.start('name');last=end+1;whole.append((first,last));edits.append((first,last,('*((uint *)&('+slot+'))' if offset==0 else '*((uint *)((char *)&('+slot+') + 4))')+' = (uint)('+value+');'))
    def covered(a,b):return any(first<=a and b<=last for first,last in whole)
    for match,end,rhs in assignments:
        first,last=match.start('name'),match.end('name');slot=match['name']
        if covered(first,last):continue
        if slot in active and _fp_producer(rhs,slots,seed):edits.append((first,last,'*((double *)&('+slot+'))'))
        if fp_kind(slot) and rhs.strip() in active:
            name=rhs.strip();a=match.end()+len(body[match.end():end])-len(body[match.end():end].lstrip());edits.append((a,a+len(name),'*((double const *)&('+name+'))'))
    pattern=r'\(\s*(float|double|float10)\s*\)\s*('+('|'.join(map(re.escape,sorted(active))))+r')\b'
    for match in re.finditer(pattern,view):
        if covered(match.start(),match.end()) or body[match.start():match.end()]!=view[match.start():match.end()]:continue
        rest=view[match.end():].lstrip()
        if rest.startswith(('.','[','->')):continue
        edits.append((match.start(),match.end(),'('+match[1]+')(*((double const *)&('+match[2]+')))'))
    def add_read(a,b,name):
        if covered(a,b) or any(a<last and b>first for first,last,replacement in edits):return
        edits.append((a,b,'*((double const *)&('+name+'))'))
    # A norm built from three scalar values has FP arithmetic when its peers
    # are FP scalars. Opaque inputs retain the same object, read as its FP view.
    norm=r'(?<![\w.>])(?P<a>[A-Za-z_]\w*)\s*\*\s*(?P=a)\s*\+\s*(?P<b>[A-Za-z_]\w*)\s*\*\s*(?P=b)\s*\+\s*(?P<c>[A-Za-z_]\w*)\s*\*\s*(?P=c)\b'
    for match in re.finditer(norm,view):
        names=[match[name] for name in ('a','b','c')]
        if len(set(names))!=3 or not any(fp_kind(name) for name in names) or not all(name in active or fp_kind(name) for name in names):continue
        for token in re.finditer(r'\b[A-Za-z_]\w*\b',view[match.start():match.end()]):
            if token[0] in active:add_read(match.start()+token.start(),match.start()+token.end(),token[0])
    # Only a plain opaque comparison operand is inferred from a floating peer.
    # Integer casts, bitwise/boolean results and neighboring clauses are barriers.
    for match in re.finditer(r'(?<![<>=!])(?P<op><=|>=|==|!=|<|>)(?![<>=])\s*(?P<name>'+('|'.join(map(re.escape,sorted(active))))+r')\b',view):
        if match.start()>0 and view[match.start()-1]=='-':continue
        tail=view[match.end():].lstrip()
        if tail and not tail.startswith((')','&&','||','?',':',',',';','}')):continue
        start=_operand_start(view,match.start())
        if _floating_side(view[start:match.start()],slots,seed):add_read(match.start('name'),match.end('name'),match['name'])
    ordered=sorted(edits)
    if any(a[1]>b[0] for a,b in zip(ordered,ordered[1:])):return body,[]
    for a,b,text in reversed(ordered):body=body[:a]+text+body[b:]
    notes=['//   Scratch hypothesis: opaque eight-byte FP stores/views; native FLD/FSTP corroboration, source correspondence unproved.'] if edits else []
    return body,notes


def _operand_start(text,end):
    depth=0;i=end-1
    while i>=0:
        char=text[i]
        if char in ')]':depth+=1
        elif char in '([':
            if depth:depth-=1
            else:break
        if not depth and (char in ';{},?:' or text[max(0,i-1):i+1] in ('&&','||')):break
        i-=1
    return i+1


def _floating_side(text,slots,seed):
    text=text.strip()
    while text.startswith('('):
        try:end=seed.matching_paren(text,0)
        except ValueError:return False
        if end!=len(text)-1:break
        text=text[1:-1].strip()
    depth=0;last_op=0;found=False;i=0
    while i<len(text):
        char=text[i]
        if depth==0:
            if char in '<>=!&|^%?:,':return False
            if char=='(' and re.match(r'\(\s*(?:float|double|float10)\s*\)',text[i:]):
                if not text[last_op:i].strip(' +-\t\r\n'):found=True
            token=re.match(r'[A-Za-z_]\w*',text[i:])
            if token:
                rows=slots.get(token[0],[])
                if len(rows)==1 and rows[0][1] in (None,'parameter') and seed.clean_type(rows[0][0]) in FP and not text[last_op:i].strip(' +-\t\r\n'):found=True
                i+=len(token[0]);continue
            if char in '+-*/':last_op=i+1
        if char in '([':depth+=1
        elif char in ')]':depth-=1
        if depth<0:return False
        i+=1
    return found and depth==0


def native_fp64_facts(audit,inventory,entry):
    from capstone import x86_const as X
    unused,row=inventory.function(entry);result=dict(original_ranges_verified=False,fp64_loads=0,fp64_stores=0,integer64_conversions=0)
    if not row or not row.get('ranges'):return result
    for span in row['ranges']:
        at=int(span['start'],16);raw=audit.original(at,int(span['end_inclusive'],16)+1-at)
        if raw!=bytes.fromhex(span['bytes_hex']):return result
        cursor=at
        for ins in audit.decoder.disasm(raw,at):
            if ins.address!=cursor:return result
            cursor+=ins.size
            if not any(op.type==X.X86_OP_MEM and op.size==8 for op in ins.operands):continue
            if ins.mnemonic=='fld':result['fp64_loads']+=1
            elif ins.mnemonic in ('fst','fstp'):result['fp64_stores']+=1
            elif ins.mnemonic in ('fild','fist','fistp','fisttp'):result['integer64_conversions']+=1
        if cursor!=at+len(raw):return result
    result['original_ranges_verified']=True;return result


def optional_fp_storage(body,params,entry,inventory,repository,seed):
    if not source_fp_carriers(body,params,seed):return body,[]
    from pathlib import Path
    repository=Path(repository);evidence=(repository/'private/original-target-20261020/inventory').resolve()
    directory=next((p for p in getattr(inventory,'passes',()) if (p/'decompiled'/('%08x.c'%entry)).is_file()),None)
    if directory is None or not directory.resolve().is_relative_to(evidence):return body,[]
    try:
        from verify_manifest import Audit
        audit=Audit(repository,'private/original-target-20261020/inputs/game.exe');facts=native_fp64_facts(audit,inventory,entry)
        return fp_storage_hypotheses(body,params,seed,facts)
    except (ValueError,KeyError,OSError):return body,[]
