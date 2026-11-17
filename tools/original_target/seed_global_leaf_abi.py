"""Native leaf virtual cleanup from a pinned addressed global class pointer.

This proves stack cleanup only. It never grants a pointee-write/alias summary.
Unrecognized or conflicting family targets remain unresolved, not guessed.
"""
import hashlib
import re
from seed_guarded_cfg import X,PARTS,require,guarded_graph,CFGRefusal
from seed_frame_equations import AffineFrames


def global_receiver(graph,at):
    nodes,pred=graph['nodes'],graph['pred'];cache={};active=set()
    def value(site,reg):
        key=(site,reg)
        if key in cache:return cache[key]
        require(key not in active and len(active)<48,'Unknown/cyclic global receiver flow');active.add(key)
        require(len(pred[site])==1,'Ambiguous global receiver ingress')
        before=next(iter(pred[site]));ins=nodes[before];ops=ins.operands
        require(before+ins.size==site and ins.mnemonic!='call','Nonlocal global receiver setup')
        if reg in {PARTS.get(r,r) for r in ins.regs_access()[1]}:
            require(ins.mnemonic=='mov' and len(ops)==2 and ops[0].type==X.X86_OP_REG and ops[0].reg==reg and ops[0].size==ops[1].size==4,'Unknown global receiver definition')
            source=ops[1]
            if source.type==X.X86_OP_REG:result=value(before,source.reg)
            else:
                require(source.type==X.X86_OP_MEM and not source.mem.index and source.mem.segment in (0,X.X86_REG_DS),'Unsupported global receiver operand')
                if not source.mem.base:result=('global',source.mem.disp&0xffffffff,before)
                else:
                    require(source.mem.disp==0,'Not a primary vptr read');origin=value(before,source.mem.base)
                    require(origin[0]=='global','Vptr does not come from a global class pointer');result=('vptr',origin)
        else:result=value(before,reg)
        active.remove(key);cache[key]=result;return result
    ins=nodes[at];ops=ins.operands
    require(ins.mnemonic=='call' and len(ops)==1,'Not a virtual call')
    operand=ops[0]
    require(operand.type==X.X86_OP_MEM and operand.size==4 and operand.mem.base and not operand.mem.index and operand.mem.segment in (0,X.X86_REG_DS) and 0<=operand.mem.disp<=4096 and operand.mem.disp%4==0,'Unsupported primary virtual operand')
    vptr=value(at,operand.mem.base);receiver=value(at,X.X86_REG_ECX)
    require(vptr==('vptr',receiver) and receiver[0]=='global','Global this/vptr are not the same original load')
    return dict(global_address=receiver[1],global_load=receiver[2],slot=operand.mem.disp//4)


def global_leaf_cleanup(proofs,graph,at,seed,header_pins):
    from seed_fp_virtual import physical_family
    flow=global_receiver(graph,at)
    registry=seed.shared_pointer_types(proofs.repository/'src/shared');known=registry.get(flow['global_address'])
    require(known is not None,'No canonical addressed global pointer type')
    match=re.fullmatch(r'([A-Za-z_]\w*)\s*\*',known['kind'])
    require(match is not None and known['forward'],'Global pointee is not an existing class/struct')
    header='src/'+known['header'];path=(proofs.repository/header).resolve()
    require(path.is_relative_to((proofs.repository/'src/shared').resolve()) and header in header_pins,'Global declaration header is not canonically pinned')
    data=path.read_bytes();require(hashlib.sha256(data).hexdigest()==header_pins[header],'Global pointer declaration header hash differs')
    # Rebind the exact declaration on the current pinned text, not just a cached
    # registry result. No record layout or argument extent is synthesized.
    pattern=r'^\s*extern\s+'+re.escape(match[1])+r'\s*\*\s*[A-Za-z_]\w*\s*;[^\n]*//\s*0x'+format(flow['global_address'],'08x')+r'\b'
    declarations=re.findall(pattern,data.decode('utf-8'),re.M|re.I)
    require(declarations,'Addressed global declaration is absent/changed')
    family=physical_family(proofs.audit,match[1],flow['slot']);targets=[];pops=set()
    for target in sorted({table['target'] for table in family}):
        digest=proofs.provenance(target);child=guarded_graph(proofs.audit,proofs.inventory,target)
        require(not any(ins.mnemonic=='call' for ins in child['nodes'].values()),'Virtual cleanup target is not native call-free')
        frames=AffineFrames(child,{})
        returns=[(site,ins) for site,ins in child['nodes'].items() if ins.mnemonic=='ret']
        require(returns,'Leaf virtual target has no normal return')
        for site,ins in returns:require(frames.height(site)==0,'Leaf virtual target does not restore its entry stack')
        cleanup={ins.operands[0].imm if ins.operands else 0 for site,ins in returns}
        require(len(cleanup)==1,'Leaf virtual target has nonunique cleanup');pop=next(iter(cleanup));require(0<=pop<=4096 and pop%4==0,'Non-DWORD virtual cleanup');pops.add(pop)
        targets.append(dict(entry=target,native_body_sha256=digest,pop=pop,call_free=True))
    require(len(pops)==1,'Physical global virtual cleanup set disagrees')
    return dict(flow,header=header,header_sha256=header_pins[header],static_class=match[1],pop=next(iter(pops)),physical_tables=family,targets=targets,memory_effects_proved=False)
