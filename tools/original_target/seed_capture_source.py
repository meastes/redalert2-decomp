"""Frozen-source witness for a computed DWORD delta stored as a double.

No native fact is inferred from a Ghidra variable name. This module binds a
specific complete expression and preserves all other statements. Native proof
is supplied separately; a binding by itself grants no rewrite permission.
"""
import hashlib
import re

WORD={'int','uint','undefined4','long','ulong','unsigned int','signed int','unsigned long','signed long'}
class CaptureRefusal(ValueError):pass

def need(condition,message):
    if not condition:raise CaptureRefusal(message)

def bind_source(body,params,seed):
    view=seed.code_view(body)
    need(not re.search(r'#|\b(?:typedef|using|class|struct|union|enum|__asm|_asm|asm)\b',view),'Unsupported source declarations')
    rows,comma=seed.scalar_declarations(view);need(not comma,'Ambiguous comma declaration');slots={};locals=set()
    for kind,name,array in rows:slots.setdefault(name,[]).append((kind,array));locals.add(name)
    for kind,name in map(seed.param_parts,params):
        if name:slots.setdefault(name,[]).append((kind,'parameter'))
    need(all(len(items)==1 for items in slots.values()),'Shadowed source declarations')
    def scalar(name):
        kind,array=slots.get(name,[('',None)])[0]
        return array in (None,'parameter') and not re.search(r'\b(?:const|volatile)\b',kind) and seed.clean_type(kind) in WORD
    def pointer(name,pointee=False):
        kind,array=slots.get(name,[('',None)])[0]
        if array not in (None,'parameter') or re.search(r'\b(?:const|volatile|__ptr64|far|near)\b',kind) or '&' in kind:return False
        kind=seed.clean_type(kind)
        return kind.endswith('*') and (not pointee or kind.count('*')==1 and seed.clean_type(kind[:-1].strip()) in WORD)
    leaf=r'\(\s*double\s*\)\s*CONCAT44\s*\(\s*(?P<high>[A-Za-z_]\w*)\s*,\s*(?P<low>[A-Za-z_]\w*)\s*\)'
    matches=list(re.finditer(leaf,view));need(len(matches)==2 and len(re.findall(r'\bCONCAT44\b',view))==2,'Missing or ambiguous packed square')
    a,b=matches;high,low=a['high'],a['low'];need((high,low)==(b['high'],b['low']) and high!=low,'Packed operands differ')
    need(high in locals and low in locals and scalar(high) and (scalar(low) or pointer(low)),'Packed operands are not local DWORD values')
    need(view[a.end():b.start()].strip()=='*','Packed pair is not one squared term')
    need(body[a.start():b.end()]==view[a.start():b.end()],'Comment or literal in selected square')
    containers=[]
    for name in sorted(set(re.findall(r'\bFUN_[0-9a-fA-F]{8}\b',view))):
        for start,end,args,unused in seed.call_sites(body,name):
            if start<a.start()<b.end()<end and len(args)==1:containers.append((name,start,end,args[0]))
    need(len(containers)==1,'Packed square is not a unique scalar call argument');callee,call_start,call_end,argument=containers[0]
    # Complete zero-offset DWORD subtraction following two explicit virtual
    # result-pointer assignments. No calls/field reads are duplicated.
    differences=[]
    pattern=r'(?m)^[\t ]*(?P<dst>[A-Za-z_]\w*)\s*=\s*\*\s*(?P<left>[A-Za-z_]\w*)\s*-\s*\*\s*(?P<right>[A-Za-z_]\w*)\s*;'
    getter=r'(?m)^[\t ]*(?P<dst>[A-Za-z_]\w*)\s*=\s*\(\s*(?:int|uint|undefined4|long|ulong)\s*\*\s*\)\s*\(\s*\*\s*\*\s*\(\s*code\s*\*\s*\*\s*\)\s*\(\s*\*\s*(?P<root>[A-Za-z_]\w*)\s*\+\s*(?P<slot>0x[0-9a-fA-F]+|[0-9]+)\s*\)\s*\)\s*\('
    getters=[]
    for match in re.finditer(getter,view):
        opening=match.end()-1;closing=seed.matching_paren(view,opening);end=closing+1
        while end<len(view) and view[end].isspace():end+=1
        if end>=len(view) or view[end]!=';':continue
        args=seed.split_args(body[opening+1:closing])
        if any(arg.strip() for arg in args) and not (len(args)==1 and re.fullmatch(r'\s*&\s*[A-Za-z_]\w*\s*',args[0])):continue
        if body[match.start():end+1]!=view[match.start():end+1]:continue
        getters.append(dict(dst=match['dst'],root=match['root'],slot=int(match['slot'],0),start=match.start(),end=end+1))
    for match in re.finditer(pattern,view):
        if match.end()>=call_start or match['dst'] not in locals or not scalar(match['dst']) or not all(pointer(match[name],True) for name in ('left','right')):continue
        if body[match.start():match.end()]!=view[match.start():match.end()]:continue
        pairs=[(first,second) for first in getters for second in getters if first['dst']==match['left'] and second['dst']==match['right'] and first['end']<=second['start'] and second['end']<=match.start() and not view[first['end']:second['start']].strip() and not view[second['end']:match.start()].strip()]
        if len(pairs)==1:differences.append((match,pairs[0]))
    need(len(differences)==1,'No unique two-getter DWORD difference');difference,(first,second)=differences[0];delta=difference['dst']
    need(delta not in (high,low) and first['dst']!=second['dst'],'Overlapping source roles')
    need(not re.search(r'\b'+re.escape(delta)+r'\s*(?:=(?!=)|(?:<<|>>|[+*/%&|^~-])=|\+\+|--)|(?:\+\+|--)\s*\b'+re.escape(delta)+r'\b|&\s*'+re.escape(delta)+r'\b',view[difference.end():call_start]),'Difference is rewritten or escapes before use')
    need(not re.search(r'\b(?:if|else|for|while|do|switch|goto|return|break|continue|throw)\b|[?:]',view[difference.end():call_start]),'Capture/use source path is not straight-line')
    need(view[difference.end():call_start].count('{')==view[difference.end():call_start].count('}')==0,'Capture/use source scope differs')
    # Aliases must be plain, unique source assignments; no name/offset guess.
    parameter_names=[seed.param_parts(p)[1] for p in params]
    def origin(name,at,active=()):
        need(name not in active and len(active)<16,'Cyclic source root alias')
        need(not re.search(r'&\s*'+re.escape(name)+r'\b|\b'+re.escape(name)+r'\s*(?:\+\+|--|(?:<<|>>|[+*/%&|^~-])=)|(?:\+\+|--)\s*'+re.escape(name)+r'\b',view[:at]),'Source root address/mutation escapes')
        assignments=list(re.finditer(r'(?m)^\s*'+re.escape(name)+r'\s*=(?!=)\s*([A-Za-z_]\w*)\s*;',view[:at]));all_writes=list(re.finditer(r'\b'+re.escape(name)+r'\s*=(?!=)',view[:at]))
        if name in parameter_names:need(not all_writes,'Source parameter reassigned');return parameter_names.index(name)
        need(pointer(name) and len(assignments)==len(all_writes)==1,'Source root is not a stable parameter alias');return origin(assignments[0][1],assignments[0].start(),active+(name,))
    need(all(item['slot'] % 4 == 0 and 0 <= item['slot'] <= 4096 for item in (first,second)), 'Unsupported virtual slot')
    origins=[origin(item['root'],item['start']) for item in (first,second)]
    return dict(source_body_sha256=hashlib.sha256(body.encode()).hexdigest(),delta=delta,left=difference['left'],right=difference['right'],high=high,low=low,source_getters=[dict(item,parameter_index=index) for item,index in zip((first,second),origins)],writer_span=[difference.start(),difference.end()],square_spans=[[a.start(),a.end()],[b.start(),b.end()]],scalar_callee=int(callee[4:],16),scalar_call_span=[call_start,call_end],source_emission_allowed=False,alias_closure_claimed=False,body_acceptance_claimed=False)

def rewrite_capture(body,binding,native_proof,seed):
    need(hashlib.sha256(body.encode()).hexdigest()==binding['source_body_sha256'],'Source binding is stale')
    need(native_proof.get('signed_dword_capture_verified') and native_proof.get('same_eight_byte_reload_verified') and native_proof.get('source_roots_correspond') and native_proof.get('frozen_source_verified'),'Missing native/source capture qualification')
    need(native_proof.get('source_body_sha256')==binding['source_body_sha256'],'Native proof binds a different source')
    identifiers=set(re.findall(r'\b[A-Za-z_]\w*\b',seed.code_view(body)));name='captured_delta';ordinal=0
    while name in identifiers:ordinal+=1;name='captured_delta_%d'%ordinal
    opening=seed.code_view(body).find('{');need(opening>=0,'No function body')
    edits=[(opening+1,opening+1,'\n    double '+name+';'),(binding['writer_span'][1],binding['writer_span'][1],'\n    '+name+' = (double)(int)'+binding['delta']+';')]
    edits.extend((a,b,name) for a,b in binding['square_spans'])
    # The obsolete high-word artifact is removed only when these two reads
    # were its entire lifetime. Other locals/uses remain untouched.
    high=binding['high'];view=seed.code_view(body)
    if len(re.findall(r'\b'+re.escape(high)+r'\b',view))==3:
        declaration=re.search(r'(?m)^[\t ]*(?:undefined4|uint|int|long|ulong|unsigned\s+int|signed\s+int|unsigned\s+long|signed\s+long)\s+'+re.escape(high)+r'\s*;[\t ]*(?:\n|$)',view)
        if declaration and body[declaration.start():declaration.end()]==view[declaration.start():declaration.end()]:edits.append((declaration.start(),declaration.end(),''))
    edits.sort();need(all(a[1]<=b[0] for a,b in zip(edits,edits[1:])),'Overlapping source edits')
    for first,last,text in reversed(edits):body=body[:first]+text+body[last:]
    return body,['//   Scratch hypothesis: native signed-DWORD-to-double capture; frozen source binding, complete alias closure/source correspondence unproved.']
