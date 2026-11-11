"""Discover and apply bounded native effects at frame-bearing direct calls."""
from seed_fp_native import X,ptr,pointer,require
from seed_fp_alias import GP,SCALAR,frames,shifted

def linear_call(machine,proofs,callee):
    """Infer from actual pointer-bearing inputs; no address/record-role table.

    The full helper must be an independently bounded, accepted, straight-line
    native body. Its exact numeric writes and pointer-input read footprints are
    symbolically established before any effect is applied to the caller frame.
    """
    require(machine.at in machine.frame['direct_pops'],'No verified direct cleanup')
    ins=machine.frame['nodes'][machine.at]
    require(ins.mnemonic=='call' and len(ins.operands)==1 and ins.operands[0].type==X.X86_OP_IMM and ins.operands[0].imm&0xffffffff==callee,'Effect callee differs from native direct call')
    pop=machine.frame['direct_pops'][machine.at]
    registers={};arguments={};bindings={}
    old={reg:machine.rv(reg) for reg in GP}
    for reg,value in old.items():
        if frames(value):
            require(None not in frames(value),'Unknown incoming register pointer offset')
            space='reg_%d'%reg;registers[reg]=ptr(space);bindings[space]=value
    for offset in range(0,pop,4):
        value=machine.readmem({machine.height+offset},4)
        if frames(value):
            require(None not in frames(value),'Unknown incoming argument pointer offset')
            space='arg_%d'%offset;arguments[offset+4]=ptr(space);bindings[space]=value
    require(bindings,'No frame-bearing helper input')
    # ECX is also a possible external object address. Symbolizing it does not
    # assume a layout or permit frame writes; arithmetic/value uses must still
    # pass the native interpreter and external numeric writes publish no alias.
    if X.X86_REG_ECX not in registers:
        space='external_ecx';registers[X.X86_REG_ECX]=ptr(space);bindings[space]=old[X.X86_REG_ECX]
    contract=proofs.linear_effects(callee,registers,arguments)
    require(contract['pop']==pop,'Inferred cleanup changed')
    for read in contract['reads']:
        require(read['space'] in bindings,'Unknown inferred read root')
        machine.numeric_buffer(shifted(bindings[read['space']],read['offset']),read['size'])
    # Resolve every write and output alias before committing any caller effect.
    resolved=[]
    for write in contract['writes']:
        require(write['space'] in bindings,'Unknown inferred write root')
        destination=frames(shifted(bindings[write['space']],write['offset']))
        require(None not in destination and len(destination)<=1,'Ambiguous inferred write destination')
        require(not destination or all(kind=='frame' for kind,offset in bindings[write['space']]),'Mixed frame/non-frame write root')
        require(all(not (dest<machine.protected[1] and dest+write['size']>machine.protected[0]) for dest in destination),'Inferred helper clobbers saved capture')
        resolved.append((destination,write['size']))
    def bind(value):
        if pointer(value):
            space,offset=value[1:]
            if space=='stack':return frozenset({('frame',machine.height-4+offset)})
            require(space in bindings,'Unknown inferred return root')
            return shifted(bindings[space],offset)
        if isinstance(value,tuple):
            require(value[0]=='saved' and value[1] in old,'Unknown inferred return token')
            return old[value[1]]
        return frozenset({('constant',value&0xffffffff)}) if isinstance(value,int) else SCALAR
    endregs={int(reg):bind(value) for reg,value in contract['end_register_values'].items()}
    for destination,size in resolved:
        if destination:machine.store(destination,size,SCALAR,'inferred native helper')
    machine.regs.update(endregs)
    return dict(callee='%08x'%callee,kind='inferred native effects',writes=contract['writes'],reads=contract['reads'],pointer_input_count=len(bindings),roles_or_record_sizes_guessed=False)


def prove_killed_inputs(proofs,entry,registers):
    """Prove unmodeled incoming GP aliases are killed before any use/return.

    A readonly summary that models stack input/ECX/saved registers cannot silently
    discard a frame pointer arriving in EAX or EDX. This check closes that gap
    when those actual incoming values are overwritten before native use.
    """
    from collections import deque
    import capstone
    from seed_fp_native import graph,PARTS
    proofs.provenance(entry)
    nodes,edges,pred,raw=graph(proofs.audit,proofs.inventory,entry)
    targets=set(registers);require(targets<={X.X86_REG_EAX,X.X86_REG_EDX},'Unsupported unmodeled input set')
    indegree={at:len(pred[at]) for at in nodes};ready=deque(at for at,n in indegree.items() if not n);order=[]
    while ready:
        at=ready.popleft();order.append(at)
        for child in edges[at]:
            indegree[child]-=1
            if not indegree[child]:ready.append(child)
    require(len(order)==len(nodes) and order[0]==entry,'Unknown cyclic input-kill proof')
    states={}
    for at in order:
        alive=set(targets) if at==entry else set().union(*(states[parent] for parent in pred[at]))
        ins=nodes[at];ops=ins.operands;reads,writes=ins.regs_access()
        zero=ins.mnemonic=='xor' and len(ops)==2 and ops[0].type==ops[1].type==X.X86_OP_REG and ops[0].reg==ops[1].reg and ops[0].size==4
        require(zero or not alive.intersection(PARTS.get(reg,reg) for reg in reads),'Incoming unmodeled frame register read before overwrite')
        if ins.group(capstone.CS_GRP_CALL) or ins.group(capstone.CS_GRP_RET):require(not alive,'Incoming unmodeled frame register reaches call/return')
        alive.difference_update(reg for reg in writes if reg not in PARTS)
        states[at]=alive
    return dict(entry=entry,incoming_registers=sorted(targets),nodes=len(nodes),killed_before_any_use=True)


def readonly_tree(proofs,entry,ecx_symbol=False):
    """Discover a transitive readonly helper DAG and exact numeric read spans.

    Current supported native argument domain is zero or one DWORD stack slot.
    Unknown indirect/recursive calls, stack effects, read spans and writes refuse.
    Every child is proved from current accepted source/native provenance.
    """
    from seed_fp_native import graph
    cache={};active=set()
    def visit(target,ecx,depth):
        require(depth<=16,'Readonly helper depth exceeded')
        require(target not in active,'Recursive readonly helper graph')
        key=(target,ecx)
        if key in cache:return cache[key]
        proofs.provenance(target)
        nodes,edges,pred,raw=graph(proofs.audit,proofs.inventory,target)
        pops={ins.operands[0].imm if ins.operands else 0 for ins in nodes.values() if ins.mnemonic=='ret'}
        require(len(pops)==1 and next(iter(pops)) in (0,4),'Unsupported readonly argument extent')
        pop=next(iter(pops));active.add(target)
        nested={}
        for ins in nodes.values():
            if ins.mnemonic!='call':continue
            require(len(ins.operands)==1 and ins.operands[0].type==X.X86_OP_IMM,'Unknown nested readonly call')
            child=ins.operands[0].imm&0xffffffff
            # Zero-argument children must account for a live ECX (e.g. ordinary
            # compiler conversions preserve it). Stack-input children require
            # a frame-free ECX at their actual call site in the parent model.
            child_nodes=graph(proofs.audit,proofs.inventory,child)[0]
            child_pops={i.operands[0].imm if i.operands else 0 for i in child_nodes.values() if i.mnemonic=='ret'}
            require(len(child_pops)==1,'Unknown nested readonly cleanup')
            nested[child]=visit(child,next(iter(child_pops))==0,depth+1)
        contract=proofs.all_alignments(target,stack_input=pop==4,ecx_symbol=ecx,nested=nested,infer_reads=True)
        active.remove(target);cache[key]=contract
        return contract
    return visit(entry,ecx_symbol,0)


def readonly_call(machine,proofs,callee):
    """Apply automatically derived readonly footprints at a native direct call."""
    from seed_fp_native import taint
    from seed_fp_alias import lost_offset
    require(machine.at in machine.frame['direct_pops'],'Missing direct readonly cleanup')
    ins=machine.frame['nodes'][machine.at]
    require(ins.mnemonic=='call' and len(ins.operands)==1 and ins.operands[0].type==X.X86_OP_IMM and ins.operands[0].imm&0xffffffff==callee,'Readonly callee differs from direct call')
    old={reg:machine.rv(reg) for reg in GP}
    prove_killed_inputs(proofs,callee,{reg for reg in (X.X86_REG_EAX,X.X86_REG_EDX) if frames(old[reg])})
    contract=readonly_tree(proofs,callee,bool(frames(old[X.X86_REG_ECX])))
    require(contract['pop']==machine.frame['direct_pops'][machine.at],'Readonly cleanup changed')
    arg=machine.readmem({machine.height},4) if contract['stack_input'] else SCALAR
    def bind(tags):
        result=frozenset()
        for space,offset in tags:
            if space=='input':value=arg
            elif space=='ecx':value=old[X.X86_REG_ECX]
            elif space=='local':
                result|=frozenset({('frame',None if offset is None else machine.height-4+offset)});continue
            elif space=='unknown_scalar':
                result|=SCALAR;continue
            elif space.startswith('saved_'):
                reg=int(space[6:]);require(reg in old,'Unknown saved readonly origin');value=old[reg]
            else:raise ValueError('Unknown readonly pointer space')
            result|=shifted(value,offset) if offset is not None else lost_offset(value)
        return result or SCALAR
    for read in contract['reads']:
        machine.numeric_buffer(bind(taint(read['space'],read['offset'])),read['size'])
    endregs={int(reg):bind(tags) for reg,tags in contract['_endregs'].items()}
    argument_writes=[]
    for at,kind,offset,size in contract['writes']:
        if kind!='owned_argument':continue
        dest=machine.height-4+offset
        require(not (dest<machine.protected[1] and dest+size>machine.protected[0]),'Readonly argument reuse clobbers saved capture')
        argument_writes.append((dest,size))
    for dest,size in argument_writes:
        # Native return proofs permit only alias removal from owned arguments.
        # Preserve old possible aliases plus a scalar alternative; do not invent
        # a definite write destination if this slot is read again after reuse.
        for byte in range(size):machine.memory[dest+byte]=machine.memory.get(dest+byte,SCALAR)|SCALAR
        machine.write_events.append(dict(address='%08x'%machine.at,frame=dest,size=size,reason='readonly argument reuse'))
    machine.regs.update(endregs)
    return dict(callee='%08x'%callee,kind='inferred read-only',input_frames=sorted(frames(arg),key=str),reads=contract['reads'],nested_calls=contract['nested_calls'],roles_or_record_sizes_guessed=False)


def direct_call(machine,proofs,seed):
    """Discover a direct call effect without a helper-address binding table."""
    from seed_fp_native import NativeRefusal
    from seed_fp_abi import member_stack_arguments
    ins=machine.frame['nodes'][machine.at]
    require(ins.mnemonic=='call' and len(ins.operands)==1 and ins.operands[0].type==X.X86_OP_IMM,'Not a direct call')
    callee=ins.operands[0].imm&0xffffffff
    require(machine.at in machine.frame['direct_pops'],'No checked direct cleanup')
    pop=machine.frame['direct_pops'][machine.at]
    has_frame=any(frames(machine.rv(reg)) for reg in GP) or any(frames(machine.readmem({machine.height+off},4)) for off in range(0,pop,4))
    if not has_frame:
        try:
            contract=member_stack_arguments(proofs,callee,seed)
            require(contract['stack_argument_bytes']==pop,'Source/native ordinary argument extent differs')
        except NativeRefusal:pass
        else:
            machine.opaque(contract['stack_argument_bytes'])
            return dict(callee='%08x'%callee,kind='opaque normal C++ call, no frame aliases')
    if has_frame:
        try:return linear_call(machine,proofs,callee)
        except NativeRefusal:pass
    return readonly_call(machine,proofs,callee)
