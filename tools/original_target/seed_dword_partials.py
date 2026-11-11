"""Bounded low-three-byte store hypotheses in known DWORD local objects.

The uint3 cast denotes a real24-bit normalized value; no C++ primitive lvalue
has that width. Evaluate the original operand once, then write exactly2+1
bytes, leaving byte3 alone. Native/source correspondence and body acceptance
remain unproved. Unknown storage, wider values and arbitrary slices refuse.
"""
import re

DWORD={'int','uint','undefined4','long','ulong','unsigned int','signed int','unsigned long','signed long'}


def low24_store_hypotheses(body,params,seed):
    if '._0_3_' not in body:return body,[]
    view=seed.code_view(body);rows,comma=seed.scalar_declarations(view)
    if comma or '#' in view or re.search(r'\b(?:typedef|using|class|struct|union|enum|asm|__asm)\b',view):return body,[]
    slots={};locals=set()
    for kind,name,array in rows:slots.setdefault(name,[]).append((kind,array));locals.add(name)
    for kind,name in map(seed.param_parts,params):
        if name:slots.setdefault(name,[]).append((kind,'parameter'))
    if any(name in slots for name in ('uint3','uint','ushort','uchar')):return body,[]
    def dword(name,local=False):
        items=slots.get(name,[])
        if len(items)!=1 or local and name not in locals:return False
        kind,array=items[0]
        return array in (None,'parameter') and not re.search(r'\b(?:const|volatile)\b',kind) and seed.clean_type(kind) in DWORD
    pattern=re.compile(r'(?m)^[\t ]*(?P<slot>[A-Za-z_]\w*)\._0_3_[\t ]*=(?!=)\s*\(\s*uint3\s*\)\s*(?P<value>[A-Za-z_]\w*)\s*;')
    names=set(re.findall(r'\b[A-Za-z_]\w*\b',view+' '+' '.join(params)));edits=[]
    for match in pattern.finditer(view):
        slot,value=match['slot'],match['value'];before=view[:match.start()].rstrip()
        if before and before[-1] not in '{;}':continue
        if not dword(slot,True) or not dword(value) or body[match.start():match.end()]!=view[match.start():match.end()]:continue
        name='low24_value';ordinal=0
        while name in names:ordinal+=1;name='low24_value_%d'%ordinal
        names.add(name)
        # The mask computes the24-bit value the original cast specifies. This
        # is a real value used by both stores, never a dummy/pass-through alias.
        text='{\n    uint '+name+' = (uint)('+value+') & 0x00ffffffU;\n    *((ushort *)&('+slot+')) = (ushort)'+name+';\n    ((uchar *)&('+slot+'))[2] = (uchar)('+name+' >> 16);\n}'
        edits.append((match.start(),match.end(),text))
    for start,end,text in reversed(edits):body=body[:start]+text+body[end:]
    notes=['//   Scratch hypothesis: %d bounded24-bit stores in declaredDWORD locals; normalized operand evaluated once, exact2+1byte writes preserve byte3; native correspondence unproved.'%len(edits)] if edits else []
    return body,notes
