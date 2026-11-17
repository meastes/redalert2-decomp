"""Native guards for a dropped final double in a raw member-call seed.

Keep the original DWORD-word prefix and all source expressions. The accepted
interface establishes stack locations, not invented layouts or bit conversions.
Unknown control, receiver, payload, nested call or argument types decline repair.
"""
import hashlib
import re
from collections import deque
import capstone
from seed_callee_abi import AbiRefusal, require, graph, REGISTER_PARTS, ECX_PARTS

X = capstone.x86_const
REGS = (X.X86_REG_EAX,X.X86_REG_ECX,X.X86_REG_EDX,X.X86_REG_EBX,
        X.X86_REG_ESP,X.X86_REG_EBP,X.X86_REG_ESI,X.X86_REG_EDI)
ALIASES = dict(REGISTER_PARTS)
ALIASES.update({X.X86_REG_EAX:{X.X86_REG_EAX,X.X86_REG_AX,X.X86_REG_AL,X.X86_REG_AH},
                X.X86_REG_ECX:ECX_PARTS,
                X.X86_REG_EDX:{X.X86_REG_EDX,X.X86_REG_DX,X.X86_REG_DL,X.X86_REG_DH},
                X.X86_REG_ESP:{X.X86_REG_ESP,X.X86_REG_SP}})
SAVED = set(REGISTER_PARTS)
VOLATILE = {X.X86_REG_EAX,X.X86_REG_ECX,X.X86_REG_EDX}

def writes(ins):
    changed=set(ins.regs_access()[1])
    return {reg for reg,aliases in ALIASES.items() if changed & aliases}

def receiver_offsets(nodes, edges, entry, call):
    """Meet over every reachable path; only copies and plain LEA retain this.

    Calls use the existing Microsoft i386 callee-saved-register contract.
    Unknown/partial writes kill that register's value, including on loop paths.
    """
    initial={reg:None for reg in REGS};initial[X.X86_REG_ECX]=0
    incoming={entry:initial};pending=deque([entry]);iterations=0
    while pending:
        at=pending.popleft();iterations+=1
        require(iterations<100000, 'Receiver dataflow did not converge')
        before=incoming[at];after=dict(before);ins=nodes[at]
        for reg in writes(ins):after[reg]=None
        op=ins.operands
        if ins.mnemonic=='mov' and len(op)==2 and op[0].type==X.X86_OP_REG and op[0].size==4 and op[0].reg in REGS and op[1].type==X.X86_OP_REG and op[1].size==4:
            after[op[0].reg]=before.get(op[1].reg)
        elif ins.mnemonic=='lea' and len(op)==2 and op[0].type==X.X86_OP_REG and op[0].size==4 and op[0].reg in REGS and op[1].type==X.X86_OP_MEM:
            mem=op[1].mem;value=before.get(mem.base)
            if value is not None and not mem.index and not mem.segment:
                offset=value+mem.disp
                if 0<=offset<=0x10000:after[op[0].reg]=offset
        if ins.group(capstone.CS_GRP_CALL):
            for reg in VOLATILE:after[reg]=None
        for target in edges[at]:
            prior=incoming.get(target)
            merged=after if prior is None else {reg:prior[reg] if prior[reg]==after[reg] else None for reg in REGS}
            if prior is None or merged!=prior:
                incoming[target]=dict(merged);pending.append(target)
    require(call in incoming and incoming[call][X.X86_REG_ECX] is not None,
            'Unknown or conflicting reaching receiver')
    return incoming[call][X.X86_REG_ECX]

def cleanup_and_body(audit,inventory,entry,row,expected):
    nodes,edges,pred,raw=graph(audit,inventory,entry)
    require(row and hashlib.sha256(raw).hexdigest()==row.get('body_sha256'), 'Unaccepted or changed native callee')
    returns=[i for i in nodes.values() if i.group(capstone.CS_GRP_RET)]
    require(returns and all(len(i.operands)==1 and i.operands[0].imm==expected for i in returns),
            'Native return cleanup disagrees')
    return nodes,edges,pred

def sret_copy(audit,inventory,entry,row):
    """Prove a small straight member copier writes only its hidden result.

    Returns the exact output span from original DWORD stores. No class size,
    sret convention or returned pointer is inferred from a source type name.
    """
    nodes,edges,pred=cleanup_and_body(audit,inventory,entry,row,4)
    require(len(nodes)<=32 and all(len(x)<=1 for x in edges.values()), 'Nonlinear nested copier')
    values={reg:('saved',reg) for reg in SAVED};values.update({X.X86_REG_ECX:('object',0),X.X86_REG_EAX:None,X.X86_REG_EDX:None})
    sp=0;stack=[];stores={};at=entry;seen=set()
    while True:
        require(at not in seen, 'Loop in nested copier');seen.add(at);ins=nodes[at];op=ins.operands
        require(not ins.group(capstone.CS_GRP_CALL) and not ins.group(capstone.CS_GRP_JUMP), 'Nested copier has transfer')
        if ins.mnemonic=='ret':
            require(sp==0 and not stack and values.get(X.X86_REG_EAX)==('result',0) and
                    all(values.get(reg)==('saved',reg) for reg in SAVED), 'Nested copier return contract differs')
            break
        if ins.mnemonic=='push':
            require(len(op)==1 and op[0].type==X.X86_OP_REG and op[0].size==4 and op[0].reg in SAVED, 'Unknown copier push')
            stack.append(values[op[0].reg]);sp-=4
        elif ins.mnemonic=='pop':
            require(len(op)==1 and op[0].type==X.X86_OP_REG and op[0].size==4 and op[0].reg in SAVED and stack, 'Unknown copier pop')
            values[op[0].reg]=stack.pop();sp+=4
        elif ins.mnemonic=='add':
            require(len(op)==2 and op[0].type==X.X86_OP_REG and op[0].size==4 and op[1].type==X.X86_OP_IMM and
                    values.get(op[0].reg) is not None and values[op[0].reg][0]=='object', 'Unknown copier arithmetic')
            values[op[0].reg]=('object',values[op[0].reg][1]+op[1].imm)
        elif ins.mnemonic=='mov' and len(op)==2 and op[0].size==4 and op[1].size==4:
            dest,source=op
            if dest.type==X.X86_OP_REG:
                require(dest.reg in values, 'Unknown copier destination')
                if source.type==X.X86_OP_REG:values[dest.reg]=values.get(source.reg)
                else:
                    require(source.type==X.X86_OP_MEM and not source.mem.index and not source.mem.segment, 'Unknown copier load')
                    mem=source.mem
                    if mem.base==X.X86_REG_ESP:
                        require(sp+mem.disp==4, 'Unknown copier stack load');values[dest.reg]=('result',0)
                    else:
                        base=values.get(mem.base)
                        require(base is not None and base[0]=='object', 'Copier load is not from object')
                        values[dest.reg]=('data',base[1]+mem.disp)
            else:
                require(dest.type==X.X86_OP_MEM and not dest.mem.index and not dest.mem.segment and source.type==X.X86_OP_REG,
                        'Unknown copier memory store')
                base=values.get(dest.mem.base);value=values.get(source.reg)
                require(base is not None and base[0]=='result' and value is not None and value[0]=='data', 'Copier writes outside result')
                offset=base[1]+dest.mem.disp
                require(0<=offset<64 and offset%4==0 and offset not in stores, 'Overlapping/unknown copier result')
                stores[offset]=value[1]
        else:raise AbiRefusal('Unsupported nested copier instruction')
        require(len(edges[at])==1, 'Unknown copier continuation');at=edges[at][0]
    require(len(seen)==len(nodes) and stores and set(stores)==set(range(0,4*len(stores),4)), 'Noncontiguous copier result')
    require(len(set(stores.values()))==len(stores) and all(stores[i]-i==stores[0] for i in stores), 'Copier source mapping differs')
    return 4*len(stores)

def argument_paths(nodes,pred,call):
    require(1<=len(pred[call])<=4, 'Unknown native call ingress')
    result=[]
    for end in sorted(pred[call]):
        path=[];at=end
        while True:
            require(len(path)<80 and at not in path, 'Unknown/looping argument prefix')
            ins=nodes[at];path.append(at);op=ins.operands
            if ins.mnemonic=='sub' and len(op)==2 and op[0].type==X.X86_OP_REG and op[0].reg==X.X86_REG_ESP and op[0].size==4 and op[1].type==X.X86_OP_IMM and op[1].imm==8:break
            require(not ins.group(capstone.CS_GRP_RET), 'Argument prefix crosses return')
            if ins.group(capstone.CS_GRP_JUMP):
                require(ins.mnemonic=='jmp' and op[0].type==X.X86_OP_IMM and op[0].imm==call and at==end, 'Unknown argument branch')
            require(len(pred[at])==1, 'Argument prefix has unproved merge')
            before=next(iter(pred[at]));require(before+nodes[before].size==at,'Nonadjacent argument predecessor');at=before
        result.append(list(reversed(path)))
    return result

def inspect_wide_call(audit,inventory,caller,callee,rows,word_count):
    """Prove every ingress supplies identical DWORD-prefix/final-double layout."""
    require(callee in rows, 'Missing accepted callee')
    cleanup_and_body(audit,inventory,callee,rows[callee],word_count*4+8)
    nodes,edges,pred,_=graph(audit,inventory,caller)
    sites=[i for i in nodes.values() if i.mnemonic=='call' and len(i.operands)==1 and
           i.operands[0].type==X.X86_OP_IMM and i.operands[0].imm&0xffffffff==callee]
    require(len(sites)==1 and sites[0].bytes[0]==0xe8 and sites[0].size==5,'Ambiguous native direct call')
    call=sites[0].address;receiver=receiver_offsets(nodes,edges,caller,call)
    require(0<receiver<=0x10000, 'Not a proved subobject receiver')
    proofs=[]
    for path in argument_paths(nodes,pred,call):
        sp=0;slots={};values={};nested=[]
        for index,at in enumerate(path):
            ins=nodes[at];op=ins.operands;before=dict(values)
            require(ins.mnemonic in ('mov','lea','push','sub','call','jmp','fild','fadd','fstp'), 'Unknown argument-prefix instruction')
            if index==0:
                sp=-8;continue
            if ins.mnemonic=='push':
                require(len(op)==1 and op[0].size==4,'Non-DWORD argument push')
                sp-=4;require(not any(start<sp+4 and sp<start+length for start,(length,_,_) in slots.items()),'Overlapping argument push')
                slots[sp]=(4,'word',before.get(op[0].reg) if op[0].type==X.X86_OP_REG else None)
            elif ins.group(capstone.CS_GRP_CALL):
                require(ins.bytes[0]==0xe8 and ins.size==5 and len(op)==1 and op[0].type==X.X86_OP_IMM,'Unknown nested argument call')
                target=op[0].imm&0xffffffff;slot=slots.get(sp)
                require(target in rows and slot is not None and slot[0:2]==(4,'word') and slot[2] is not None and slot[2]>=0,
                        'Nested result pointer overlaps arguments or is unknown')
                size=sret_copy(audit,inventory,target,rows[target]);output=slot[2]
                del slots[sp];sp+=4
                nested.append(dict(call=at,target=target,result_offset=output,result_bytes=size))
                for reg in VOLATILE:values[reg]=None
                values[X.X86_REG_EAX]=output
                continue
            elif writes(ins)&{X.X86_REG_ESP}:
                raise AbiRefusal('Unmodeled argument stack adjustment')
            # Capstone labels x87 FSTP operands as READ in some versions.
            # Use the explicit whitelisted opcode semantics for this store.
            memory_writes=[o for i,o in enumerate(op) if o.type==X.X86_OP_MEM and
                           (ins.mnemonic=='fstp' or ins.mnemonic=='mov' and i==0 or o.access&capstone.CS_AC_WRITE)]
            for operand in memory_writes:
                mem=operand.mem
                require(mem.base==X.X86_REG_ESP and not mem.index and not mem.segment, 'Non-stack write in argument prefix')
                start=sp+mem.disp
                if start<0:
                    require(ins.mnemonic=='fstp' and operand.size==8 and start==-8 and sp==-8 and mem.disp==0 and -8 not in slots,
                            'Unexpected/overwriting outgoing payload store')
                    slots[-8]=(8,'double',None)
                else:
                    require(ins.mnemonic=='mov' and operand.size==4,'Unknown local store in argument prefix')
            for reg in writes(ins):values[reg]=None
            if ins.mnemonic=='mov' and len(op)==2 and op[0].type==X.X86_OP_REG and op[0].size==4 and op[1].type==X.X86_OP_REG and op[1].size==4:
                values[op[0].reg]=before.get(op[1].reg)
            elif ins.mnemonic=='lea' and len(op)==2 and op[0].type==X.X86_OP_REG and op[0].size==4 and op[1].type==X.X86_OP_MEM:
                mem=op[1].mem
                if mem.base==X.X86_REG_ESP and not mem.index and not mem.segment:values[op[0].reg]=sp+mem.disp
            require(not ins.group(capstone.CS_GRP_JUMP) or at==path[-1],'Unexpected argument transfer')
        expected=word_count*4+8
        require(sp==-expected and slots.get(-8,(None,None,None))[:2]==(8,'double'), 'Outgoing argument size/double differs')
        require(set(slots)==set(range(-expected,-8,4))|{-8} and all(slots[x][:2]==(4,'word') for x in range(-expected,-8,4)), 'Outgoing payload has holes or overlaps')
        # The source's first and last DWORD parameters are record references;
        # prove both addresses name frame locals, not the receiver or a scalar.
        require(slots[sp][2] is not None and slots[sp][2]>=0 and slots[-12][2] is not None and slots[-12][2]>=0,
                'Record-reference argument is not a known frame address')
        proofs.append(dict(start=path[0],instructions=path,nested_copies=nested,argument_bytes=expected))
    return dict(call=call,receiver_offset=receiver,paths=proofs,word_prefix=word_count,trailing_double_bytes=8)


def wide_signature(source,symbol,old_params,seed):
    """Match real ABI boundaries against an unchanged DWORD-prefix export."""
    view=seed.code_view(source)
    matches=list(re.finditer(r'^void[ \t]+([A-Za-z_]\w*)::([A-Za-z_]\w*)[ \t]*\(([^()]*)\)\s*\{',view,re.M))
    require(len(matches)==1,'Unknown/ambiguous accepted void member')
    match=matches[0];require(symbol.startswith('?'+match[2]+'@'+match[1]+'@@QAEX'),'Accepted member symbol differs')
    params=seed.split_args(match[3]);parts=list(map(seed.param_parts,params))
    require(parts and parts[-1][0]=='double' and len(parts)>2,'No real trailing double')
    prefix=old_params[1:];cursor=0;references=[];doubles=0
    for kind,name in parts[:-1]:
        if kind=='double':
            require(cursor+2<=len(prefix) and all(seed.param_parts(x)[0]=='undefined4' for x in prefix[cursor:cursor+2]),
                    'Raw prefix does not preserve both double words')
            cursor+=2;doubles+=1
        else:
            require(re.fullmatch(r'(?:const\s+)?[A-Za-z_]\w*(?:\s+const)?\s*&',kind) is not None,
                    'Unsupported accepted parameter representation')
            require(cursor<len(prefix) and re.fullmatch(r'[A-Za-z_]\w*\s*\*+',seed.param_parts(prefix[cursor])[0]) is not None,
                    'Reference does not map to raw pointer')
            references.append(cursor);cursor+=1
    require(cursor==len(prefix) and doubles and len(references)==2 and references==[0,len(prefix)-1],
            'Unsupported raw-prefix parameter boundaries')
    return len(prefix),references

def raw_source_arguments(body,params,args,old_params,seed):
    view=seed.code_view(body);decls,comma=seed.scalar_declarations(view)
    require(not comma and not re.search(r'#|\b(?:typedef|using|class|struct|union|enum)\b',view), 'Ambiguous source argument scope')
    names=[row[1] for row in decls]+[seed.param_parts(p)[1] for p in params]
    require(len(names)==len(set(names)),'Duplicate source declarations')
    types=seed.variable_types(body,params)
    require(len(args)==len(old_params),'Raw source count differs')
    for argument,parameter in zip(args[:-1],old_params[1:]):
        require(re.fullmatch(r'[A-Za-z_]\w*',argument.strip()) is not None,'Nontrivial raw source argument')
        actual=types.get(argument.strip());kind=seed.param_parts(parameter)[0]
        require(actual is not None,'Unknown raw argument type')
        if '*' in kind:require('*' in actual,'Raw reference argument is not pointer-valued')
        else:require(actual in seed.INTEGRAL32,'Raw double word is not integral DWORD')
    require(re.fullmatch(r'[A-Za-z_]\w*',args[-1].strip()) is not None and types.get(args[-1].strip())=='double',
            'Missing final source double or unsupported conversion')

def accepted_wide_member_view(repository,inventory,caller,callee,body,root_params,
                              old_prototype,this_name,accepted,seed):
    old_ret,old_conv,old_params=old_prototype
    if not this_name or old_ret.strip()!='void' or old_conv!='__thiscall' or callee not in accepted:return None
    sites=list(seed.call_sites(body,'FUN_%08x'%callee))
    if len(sites)!=1 or len(sites[0][2])!=len(old_params) or len(old_params)<5:return None
    # Source feasibility precedes any expensive catalog/native work.
    try:raw_source_arguments(body,root_params,sites[0][2],old_params,seed)
    except AbiRefusal:return None
    evidence_root=(repository/'private/original-target-20261020/inventory').resolve()
    directory=next((p for p in inventory.passes if (p/'decompiled'/('%08x.c'%caller)).is_file()),None)
    if directory is None or not directory.resolve().is_relative_to(evidence_root):return None
    try:
        from seed_callee_abi import _CONTEXTS
        key=repository.resolve()
        if key not in _CONTEXTS:
            import catalog_io
            from verify_manifest import Audit
            from collections import defaultdict
            catalog=catalog_io.load(catalog_io.locate(repository),references=False);mapped=defaultdict(list)
            for row in catalog.get('regions',[]):mapped[row['entry']].append(row)
            _CONTEXTS[key]=mapped,Audit(repository,'private/original-target-20261020/inputs/game.exe')
        mapped,audit=_CONTEXTS[key]
        require(len(mapped.get(callee,[]))==1,'Missing/ambiguous accepted callee')
        row=mapped[callee][0]
        require(row.get('source')==accepted[callee] and row['source'].startswith('src/recovered/'),'Accepted source map differs')
        path=repository/row['source'];require(path.resolve().is_relative_to((repository/'src/recovered').resolve()),'Noncanonical source path')
        count,refs=wide_signature(path.read_text(encoding='utf-8'),row['symbol'],old_params,seed)
        rows={entry:values[0] for entry,values in mapped.items() if len(values)==1}
        proof=inspect_wide_call(audit,inventory,caller,callee,rows,count)
        require(not any(seed.param_parts(p)[1]=='seed_trailing_double' for p in old_params),'Generated parameter name conflict')
    except (AbiRefusal,OSError,KeyError,ValueError):return None
    return dict(prototype=(old_ret,old_conv,list(old_params)+['double seed_trailing_double']),
                forwards=[],owner='((char*)%s + 0x%x)'%(this_name,proof['receiver_offset']),proof=proof,
                note='// Accepted callee %08x: native subobject receiver and all outgoing DWORD/final-double stack slots verified; raw prefix retained.'%callee)
