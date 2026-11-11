"""Fold complete packed SSA values without widening narrow parameter objects.

Scratch hypothesis only: source expressions are retained and evaluated once.
The independent native guard must prove two WORD stores into a reused incoming
home and a complete DWORD output copy. Full source/alias equivalence is unproved.
"""
import hashlib
import re
from seed_guarded_cfg import X,PARTS,require,guarded_graph
from seed_frame_equations import AffineFrames
from seed_wide_partial_stores import _integer_value,INT

NARROW={'char':1,'signed char':1,'unsigned char':1,'byte':1,'uchar':1,'undefined1':1,'short':2,'signed short':2,'unsigned short':2,'ushort':2,'undefined2':2}
DWORD={'int','uint','long','ulong','undefined4','signed int','unsigned int','signed long','unsigned long'}


def bind_packed_copy(body,params,conv,seed):
    require(conv=='__thiscall' and len(params)>1,'Not an ordinary member parameter view')
    view=seed.code_view(body);rows,comma=seed.scalar_declarations(view)
    require(not comma and '#' not in view and not re.search(r'\b(?:typedef|using|class|struct|union|enum|asm|__asm|goto|switch|for|while|do)\b',view),'Unsupported source scope')
    slots={};formals={};parts=list(map(seed.param_parts,params))
    for kind,name,array in rows:slots.setdefault(name,[]).append((kind,array))
    for index,(kind,name) in enumerate(parts):slots.setdefault(name,[]).append((kind,'parameter'));formals[name]=(index,kind)
    require(all(len(items)==1 for items in slots.values()),'Shadowed declaration')
    require('CONCAT22' not in slots,'CONCAT22 is locally bound')
    for kind,name in parts:
        require(name and not re.search(r'\b(?:volatile|const|__ptr64|far|near)\b|[&()[\]]',kind) and (seed.clean_type(kind) in DWORD|set(NARROW) or '*' in kind),'Unknown physical formal ABI')
    bindings=[]
    pattern=re.compile(r'(?m)^[\t ]*(?P<alias>_[A-Za-z_]\w*)\s*=(?!=)\s*CONCAT22\s*\(')
    for match in pattern.finditer(view):
        alias=match['alias'];base=alias[1:];before=view[:match.start()].rstrip()
        if base not in formals or alias in slots or before and before[-1] not in '{;}':continue
        index,kind=formals[base];narrow=NARROW.get(seed.clean_type(kind))
        if not narrow or index==0 or len(re.findall(r'\b'+re.escape(alias)+r'\b',view))!=2:continue
        if re.search(r'&\s*'+re.escape(base)+r'\b',view):continue
        opening=match.end()-1;closing=seed.matching_paren(view,opening);args=seed.split_args(body[opening+1:closing])
        if len(args)!=2 or not all(_integer_value(arg,slots,seed) for arg in args):continue
        tokens=set(re.findall(r'\b[A-Za-z_]\w*\b',' '.join(seed.code_view(a) for a in args)))
        if any(token in slots and (slots[token][0][1] not in (None,'parameter') or seed.clean_type(slots[token][0][0]) not in INT or re.search(r'\b(?:const|volatile)\b',slots[token][0][0])) for token in tokens):continue
        # A packed half must read an existing input or a previously assigned
        # scalar. Do not turn an uninitialized Ghidra register artifact into a
        # supposedly complete computed output. With loops/gotos excluded, a
        # enclosing-scope assignment outside unbraced control dominates the use.
        def scope_at(at):
            stack=[]
            for offset,ch in enumerate(view[:at]):
                if ch=='{':stack.append(offset)
                elif ch=='}':
                    if not stack:return None
                    stack.pop()
            return tuple(stack)
        scope=scope_at(match.start());require(scope is not None,'Malformed source scope')
        initialized=True
        for token in tokens & slots.keys():
            if token in formals:continue
            definitions=list(re.finditer(r'(?m)^[\t ]*'+re.escape(token)+r'\s*=(?!=)',view[:match.start()]))
            dominating=[d for d in definitions if scope_at(d.start()) is not None and scope[:len(scope_at(d.start()))]==scope_at(d.start()) and (not view[:d.start()].rstrip() or view[:d.start()].rstrip()[-1] in '{;}')]
            if not dominating:initialized=False;break
        if not initialized:continue
        tail=re.match(r'\s*;\s*\*\s*(?P<output>[A-Za-z_]\w*)\s*=\s*'+re.escape(alias)+r'\s*;',view[closing+1:])
        if tail is None:continue
        end=closing+1+tail.end();output=tail['output']
        if output not in formals or output==base or re.search(r'\b'+re.escape(base)+r'\b',view[end:]):continue
        out_index,out_kind=formals[output];out_kind=seed.clean_type(out_kind)
        if out_index==0 or out_kind.count('*')!=1 or seed.clean_type(out_kind[:-1].strip()) not in DWORD:continue
        if body[match.start():end]!=view[match.start():end]:continue
        bindings.append(dict(source_body_sha256=hashlib.sha256(body.encode()).hexdigest(),alias=alias,narrow_parameter=base,narrow_bytes=narrow,parameter_index=index,parameter_frame=index*4,output=output,output_frame=out_index*4,stack_bytes=(len(params)-1)*4,span=[match.start(),end],expression=body[view.rfind('CONCAT22',match.start(),opening):closing+1]))
    require(len(bindings)==1,'No unique complete packed producer/output copy');return bindings[0]


def graph_linear(graph,start,stop=None,limit=128):
    result=[];at=start
    for unused in range(limit):
        result.append(at)
        if at==stop or graph['nodes'][at].mnemonic=='ret':return result
        ins=graph['nodes'][at];targets=graph['edges'][at]
        require(len(targets)==1 and targets[0]==at+ins.size and graph['pred'][targets[0]]=={at},'Packed tail has branch or alternate ingress');at=targets[0]
    require(False,'Packed tail exceeds bound')


def register_definition(graph,at,reg,active=()):
    key=(at,reg);require(key not in active and len(active)<8,'Cyclic register copies');cursor=at
    for unused in range(32):
        parents=graph['pred'][cursor];require(len(parents)==1,'Ambiguous output register ingress');parent=next(iter(parents));ins=graph['nodes'][parent]
        require(parent+ins.size==cursor and graph['edges'][parent]==[cursor] and ins.mnemonic!='call','Nonlinear/call-clobbered output register')
        if reg in {PARTS.get(r,r) for r in ins.regs_access()[1]}:
            require(ins.mnemonic=='mov' and len(ins.operands)==2 and ins.operands[0].type==X.X86_OP_REG and ins.operands[0].reg==reg and ins.operands[0].size==4,'Unknown/partial output register definition')
            if ins.operands[1].type==X.X86_OP_REG and ins.operands[1].size==4:return register_definition(graph,parent,ins.operands[1].reg,active+(key,))
            return parent,ins.operands[1]
        cursor=parent
    require(False,'No bounded output register definition')


def native_packed_copy(graph,binding):
    require(not any(i.mnemonic=='call' for i in graph['nodes'].values()),'Packed view has calls/unknown aliases')
    returns=[i for i in graph['nodes'].values() if i.mnemonic=='ret'];require(len(returns)==1 and len(returns[0].operands)==1 and returns[0].operands[0].imm==binding['stack_bytes'],'Member cleanup contradicts source formal slots')
    frames=AffineFrames(graph,{});home=binding['parameter_frame'];accesses=[];other_writes=[]
    for at,ins in graph['nodes'].items():
        if ins.mnemonic=='push':
            address=frames.address(at,-4);require(address['proved'],'Unknown implicit stack write');offset=int(address['constant']);require(offset>=home+4 or offset+4<=home,'Implicit stack write overlaps packed home')
        for op in ins.operands:
            if op.type!=X.X86_OP_MEM:continue
            if op.mem.base==X.X86_REG_ESP:
                require(not op.mem.index and not op.mem.segment,'Unknown native frame access');a=frames.address(at,op.mem.disp);require(a['proved'],'Underdetermined native frame');offset=int(a['constant'])
                if offset<home+4 and offset+op.size>home:accesses.append((at,ins,op,offset))
            elif op.access&2:other_writes.append((at,ins,op))
    writes=[x for x in accesses if x[2].access&2];require(len(writes)==2 and all(x[2].size==2 and x[1].mnemonic=='mov' for x in writes),'No complete two-WORD packed definition');require({x[3] for x in writes}=={home,home+2},'WORD stores do not cover one complete DWORD')
    first=min(x[0] for x in writes);tail=graph_linear(graph,first);tail_set=set(tail);require(all(x[0] in tail_set for x in writes),'Packed definitions are not one linear tail')
    ordinary={'mov','movsx','movzx','lea','add','sub','sar','shr','shl','sal','and','or','xor','inc','dec','neg','not','cdq','imul','mul','div','idiv','adc','sbb','cmp','test','pop','push','ret'}
    require(all(graph['nodes'][at].mnemonic in ordinary for at in tail),'Unknown operation in packed copy tail')
    reads=[x for x in accesses if x[2].access&1];wide=[x for x in reads if x[2].size==4 and x[3]==home];require(len(wide)==1,'No unique complete packed DWORD read');read_at,read_ins,read_op,unused=wide[0]
    require(read_at in tail_set and all(tail.index(x[0])<tail.index(read_at) for x in writes),'Packed DWORD is read before complete definition')
    narrow=[x for x in reads if x not in wide];require(narrow and all(x[2].size<=binding['narrow_bytes'] and x[3]==home and x[0] not in tail_set for x in narrow),'Narrow input lifetime overlaps packed output')
    require(len(other_writes)==1,'Unknown additional nonframe write');out_at,out_ins,out_op=other_writes[0];require(out_at in tail_set and tail.index(out_at)>tail.index(read_at),'Output precedes complete packed value')
    require(out_ins.mnemonic=='mov' and len(out_ins.operands)==2 and out_op==out_ins.operands[0] and out_op.size==4 and out_op.mem.base and not out_op.mem.index and not out_op.mem.segment and out_op.mem.disp==0,'Unknown DWORD output store')
    data=out_ins.operands[1];require(data.type==X.X86_OP_REG and data.size==4,'Output data is not one DWORD register')
    data_at,data_op=register_definition(graph,out_at,data.reg);require(data_at==read_at and data_op.type==X.X86_OP_MEM,'Output value is not the verified complete packed read')
    ptr_at,ptr_op=register_definition(graph,out_at,out_op.mem.base);require(ptr_op.type==X.X86_OP_MEM and ptr_op.size==4 and ptr_op.mem.base==X.X86_REG_ESP and not ptr_op.mem.index and not ptr_op.mem.segment,'Output pointer is not an incoming formal load');address=frames.address(ptr_at,ptr_op.mem.disp);require(address['proved'] and address['constant']==str(binding['output_frame']),'Native/source output parameter differs')
    return dict(narrow_input_reads=[x[0] for x in narrow],word_stores=[dict(at=x[0],frame=x[3]) for x in writes],packed_frame=home,packed_read=read_at,output_store=out_at,output_pointer_load=ptr_at,tail=tail,normal_MS_i386_input_contract_qualified=True,original_parameter_object_widened=False,full_source_alias_correspondence_claimed=False)


def optional_packed_copy(body,params,conv,entry,inventory,repository,seed):
    try:binding=bind_packed_copy(body,params,conv,seed)
    except ValueError:return body,[]
    from pathlib import Path
    repository=Path(repository);evidence=(repository/'private/original-target-20261020/inventory').resolve();directory=next((p for p in inventory.passes if (p/'decompiled'/('%08x.c'%entry)).is_file()),None)
    if directory is None or not directory.resolve().is_relative_to(evidence):return body,[]
    try:
        from verify_manifest import Audit
        audit=Audit(repository,'private/original-target-20261020/inputs/game.exe');graph=guarded_graph(audit,inventory,entry);proof=native_packed_copy(graph,binding)
        source=inventory.decompiled(entry);ret,old_conv,name,old_params,span=seed.parse_header(source);old_conv,old_params=seed.fit_convention(old_conv,old_params,seed.stack_cleanup(inventory.function(entry)[1]),old_conv=='__thiscall');frozen=seed.nonnegative_tests(seed.reindent(source[span[1]:].strip()));require(body==frozen and params==old_params and conv==old_conv,'Packed source is not the current frozen export')
    except (ValueError,KeyError,TypeError,OSError):return body,[]
    start,end=binding['span'];replacement='*'+binding['output']+' = '+binding['expression']+';'
    return body[:start]+replacement+body[end:],['//   Scratch hypothesis: complete packed SSA value folded into its native-verified DWORD output; narrow parameter unchanged, full source/alias correspondence unproved.']
