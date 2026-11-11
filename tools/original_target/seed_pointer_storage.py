"""Refuse undeclared widened aliases of named Ghidra stack pointers.

A declared pointer is four bytes on this target. A second, underscored name
used for a double value does not establish another eight-byte local/overlap.
This module only refuses: it never invents an extent or replacement storage.
"""
import re

def validate_pointer_storage(body,params,seed):
    view=seed.code_view(body);rows,comma=seed.scalar_declarations(view)
    if comma or re.search(r'#|\b(?:typedef|using|namespace|class|struct|union|enum)\b',view):return
    slots={}
    for kind,name,array in rows:slots.setdefault(name,[]).append((kind,array))
    for kind,name in map(seed.param_parts,params):
        if name:slots.setdefault(name,[]).append((kind,None))
    pattern=r'\b_([A-Za-z_]\w*Stack_[0-9a-fA-F]+)\b'
    for match in re.finditer(pattern,view):
        alias=match[0];base=match[1];decls=slots.get(base,[])
        if alias in slots or len(decls)!=1 or decls[0][1] or '*' not in decls[0][0]:continue
        before,after=view[:match.start()].rstrip(),view[match.end():].lstrip()
        if before.endswith(('.','->','::')) or after.startswith(('(',':')) or re.search(r'\bgoto\s*$',before):continue
        assignments=list(re.finditer(r'\b'+re.escape(alias)+r'\s*=(?!=)\s*([^;]+);',view))
        if not any(re.match(r'\(\s*double\s*\)|-?(?:[0-9]+\.[0-9]*|[0-9]+[eE][+-]?[0-9]+)',assignment[1]) for assignment in assignments):continue
        raise ValueError('Undeclared widened Ghidra pointer storage alias %s for %s %s: eight-byte extent and overlap are unverified'%(alias,decls[0][0],base))
