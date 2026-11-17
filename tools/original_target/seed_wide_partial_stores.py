"""Complete self-preserving WORD store hypotheses in known eight-byte locals.

No six-byte load/lvalue, high-part reread, invented storage, call or numeric
uint64 helper. This follows the seed's existing floating-word storage repair;
source-to-original correspondence and whole-body acceptance remain unproved.
"""
import re
WIDE={'double','undefined8','ulonglong','unsigned __int64','unsigned long long','uint64_t'}
INT={'char','signed char','unsigned char','short','unsigned short','ushort','uchar','byte','undefined1','undefined2','int','uint','long','ulong','signed int','unsigned int','signed long','unsigned long','undefined4'}
CAST_WORDS=set('char signed unsigned short ushort uchar byte undefined1 undefined2 int uint long ulong undefined4'.split())

def _integer_value(value,slots,seed):
    view=seed.code_view(value)
    if not seed.byte_update_value_safe(value) or re.search(r'->|\.|(?<![\w.])[0-9]+[eE][+-]?[0-9]+\b',view):return False
    # Validate the pointee and scalar index before treating each complete load
    # as an integer value. The source operand itself is emitted unchanged.
    loads=list(re.finditer(r'(?<![\w.>])([A-Za-z_]\w*)\s*\[([^\[\]]+)\]',view))
    for load in reversed(loads):
        rows=slots.get(load[1],[])
        if len(rows)!=1 or rows[0][1] not in (None,'parameter'):return False
        raw_kind=rows[0][0]
        if re.search(r'\b(?:const|volatile)\b',raw_kind):return False
        kind=seed.clean_type(raw_kind)
        if kind.count('*')!=1 or seed.clean_type(kind[:-1].strip()) not in INT:return False
        index=load[2]
        if not _integer_value(index,slots,seed):return False
        for token in re.findall(r'\b[A-Za-z_]\w*\b',index):
            if token in CAST_WORDS:continue
            row=slots.get(token,[])
            if len(row)!=1 or row[0][1] not in (None,'parameter') or seed.clean_type(row[0][0]) not in INT:return False
        view=view[:load.start()]+'0'+view[load.end():]
    if '[' in view or ']' in view:return False
    for token in re.findall(r'\b[A-Za-z_]\w*\b',view):
        if token in CAST_WORDS:continue
        rows=slots.get(token,[])
        if len(rows)!=1:return False
        kind,array=rows[0]
        if array not in (None,'parameter') or re.search(r'\b(?:const|volatile)\b',kind):return False
        kind=seed.clean_type(kind)
        if kind in INT:continue
        if kind.endswith('*') and seed.clean_type(kind.rstrip('*').strip()) in INT:continue
        return False
    return True


def low_word_store_hypotheses(body,params,seed):
    if 'CONCAT62' not in body:return body,[]
    view=seed.code_view(body);declarations,comma=seed.scalar_declarations(view)
    if comma or '#' in view or re.search(r'\b(?:typedef|using|namespace|class|struct|union|enum)\b',view):return body,[]
    slots={};locals=set()
    for kind,name in map(seed.param_parts,params):
        if name:slots.setdefault(name,[]).append((kind,None))
    for kind,name,array in declarations:slots.setdefault(name,[]).append((kind,array));locals.add(name)
    if 'CONCAT62' in slots or re.search(r'(?:^|[;{}\n])\s*(?:extern\s+)?[A-Za-z_]\w*(?:\s+[A-Za-z_]\w*)*\s+\**CONCAT62\s*\(',view):return body,[]
    pattern=re.compile(r'(?m)^[\t ]*(?P<slot>[A-Za-z_]\w*)[\t ]*=(?!=)\s*\(\s*double\s*\)\s*CONCAT62\s*\(')
    edits=[]
    for match in pattern.finditer(view):
        before=view[:match.start()].rstrip()
        if before and before[-1] not in '{;}':continue
        slot=match['slot'];declared=slots.get(slot,[])
        if slot not in locals or len(declared)!=1:continue
        kind,array=declared[0]
        if array or '&' in kind or '*' in kind or re.search(r'\b(?:const|volatile)\b',kind) or seed.clean_type(kind) not in WIDE:continue
        opening=match.end()-1
        try:closing=seed.matching_paren(view,opening)
        except ValueError:continue
        end=closing+1
        while end<len(view) and view[end].isspace():end+=1
        if end>=len(view) or view[end]!=';' or body[match.start('slot'):closing+1]!=view[match.start('slot'):closing+1]:continue
        args=seed.split_args(body[opening+1:closing])
        if len(args)!=2 or not re.fullmatch(re.escape(slot)+r'\s*\.\s*_2_6_',seed.code_view(args[0]).strip()):continue
        value=args[1].strip()
        if not value or not _integer_value(value,slots,seed):continue
        edits.append((match.start('slot'),end+1,'*((ushort *)&('+slot+')) = (ushort)('+value+');'))
    for first,last,replacement in reversed(edits):body=body[:first]+replacement+body[last:]
    notes=['//   Scratch hypothesis: %d self-preserving low WORD stores in known eight-byte locals; original correspondence requires exact audit.'%len(edits)] if edits else []
    return body,notes
