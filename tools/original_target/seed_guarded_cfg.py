"""Closed original CFG with guarded one/two-level readonly switch tables.

Bounds come from the frozen independent inventory. This is analysis evidence,
not a compiler candidate, a function acceptance rule, or an inferred code extent.
"""
from collections import defaultdict
import hashlib
import struct
import capstone
X=capstone.x86_const
PARTS={X.X86_REG_AX:X.X86_REG_EAX,X.X86_REG_AL:X.X86_REG_EAX,X.X86_REG_AH:X.X86_REG_EAX,X.X86_REG_BX:X.X86_REG_EBX,X.X86_REG_BL:X.X86_REG_EBX,X.X86_REG_BH:X.X86_REG_EBX,X.X86_REG_CX:X.X86_REG_ECX,X.X86_REG_CL:X.X86_REG_ECX,X.X86_REG_CH:X.X86_REG_ECX,X.X86_REG_DX:X.X86_REG_EDX,X.X86_REG_DL:X.X86_REG_EDX,X.X86_REG_DH:X.X86_REG_EDX,X.X86_REG_SI:X.X86_REG_ESI,X.X86_REG_DI:X.X86_REG_EDI,X.X86_REG_BP:X.X86_REG_EBP}

class CFGRefusal(ValueError):
    pass

def require(condition,reason):
    if not condition:raise CFGRefusal(reason)


def guarded_graph(audit,inventory,entry):
    unused,row=inventory.function(entry)
    require(row and row.get('contiguous') and len(row.get('ranges',[]))==1,'Unknown independent contiguous extent')
    span=row['ranges'][0];lo,hi=int(span['start'],16),int(span['end_inclusive'],16)+1
    require(lo==entry and 0<hi-lo<=65536,'Unsupported independent function extent')
    raw=audit.original(lo,hi-lo);require(raw==bytes.fromhex(span['bytes_hex']),'Original inventory bytes differ')
    resolved={};proofs={}
    def walk():
        nodes={};edges={};occupied={};pending=[entry];unknown=set();pred=defaultdict(set)
        while pending:
            at=pending.pop()
            if at in nodes:continue
            require(lo<=at<hi and len(nodes)<8192,'Function control escaped independent bound')
            ins=next(audit.decoder.disasm(raw[at-lo:],at,1),None)
            require(ins is not None and at+ins.size<=hi,'Undecoded original instruction')
            require(all(byte not in occupied or occupied[byte]==at for byte in range(at,at+ins.size)),'Overlapping original decode')
            for byte in range(at,at+ins.size):occupied[byte]=at
            require(ins.mnemonic not in ('retf','retfq','iret','iretd','int','int1','int3','ud2','hlt'),'Unknown control/trap')
            nodes[at]=ins
            if ins.group(capstone.CS_GRP_RET):
                require(ins.mnemonic=='ret' and ins.bytes[0] in (0xc2,0xc3),'Unknown return width');targets=[]
            elif ins.group(capstone.CS_GRP_JUMP):
                require(len(ins.operands)==1,'Unknown branch operand')
                if ins.operands[0].type==X.X86_OP_IMM:
                    require(ins.mnemonic=='jmp' or ins.mnemonic.startswith('j'),'Unsupported loop branch')
                    targets=[ins.operands[0].imm&0xffffffff]
                    if ins.mnemonic!='jmp':targets.append(at+ins.size)
                else:
                    require(ins.mnemonic=='jmp','Unknown indirect conditional branch')
                    targets=resolved.get(at,[])
                    if at not in resolved:unknown.add(at)
            else:
                require(not ins.group(capstone.CS_GRP_CALL) or ins.mnemonic=='call','Unknown far call')
                targets=[at+ins.size]
            edges[at]=targets
            for target in targets:pred[target].add(at)
            pending.extend(targets)
        return nodes,edges,pred,unknown,occupied

    def readonly(address,size):
        in_body = lo <= address and address + size <= hi
        require((address >= hi or in_body) and 0 < size <= 1024,
                'Unknown switch data extent')
        section=audit.pe.get_section_by_rva(address-audit.base)
        require(section is not None and section.Characteristics&0x40000000 and not section.Characteristics&0x80000000,'Writable/unknown switch table')
        end=section.VirtualAddress+min(section.SizeOfRawData,section.Misc_VirtualSize)
        require(address-audit.base+size<=end,'Switch table escapes initialized data')
        data=audit.original(address,size);require(len(data)==size,'Truncated original switch table');return data

    def inspect(at,nodes,edges,pred):
        dispatch=nodes[at];operand=dispatch.operands[0]
        require(operand.type==X.X86_OP_MEM and operand.size==4 and not operand.mem.base and operand.mem.index and operand.mem.scale==4 and operand.mem.segment in (0,X.X86_REG_DS),'Unsupported indexed switch operand')
        index=operand.mem.index;table=operand.mem.disp&0xffffffff;require(table%4==0,'Unaligned switch pointer table')
        chain=[at]
        def previous(cursor):
            require(len(pred[cursor])==1,'Unproved switch ingress/bypass')
            parent=next(iter(pred[cursor]));ins=nodes[parent]
            require(parent+ins.size==cursor,'Nonlinear switch guard chain')
            chain.append(parent);return ins
        setup=previous(at);mapping=None;input_index=index;level=1;cursor=at
        low={X.X86_REG_EAX:X.X86_REG_AL,X.X86_REG_EBX:X.X86_REG_BL,X.X86_REG_ECX:X.X86_REG_CL,X.X86_REG_EDX:X.X86_REG_DL}
        op=setup.operands
        if setup.mnemonic in ('mov','movzx') and len(op)==2 and op[0].type==X.X86_OP_REG and op[1].type==X.X86_OP_MEM and op[1].size==1:
            source=op[1]
            require(source.mem.base and not source.mem.index and source.mem.segment in (0,X.X86_REG_DS),'Unknown byte-index map address')
            input_index=source.mem.base;mapping=source.mem.disp&0xffffffff;require(input_index!=index,'Switch map destroys its bounded index')
            if setup.mnemonic=='mov':
                require(op[0].size==1 and op[0].reg==low.get(index),'Not a zero-based low-byte index load')
                zero=previous(setup.address);z=zero.operands
                require(zero.mnemonic=='xor' and len(z)==2 and z[0].type==z[1].type==X.X86_OP_REG and z[0].size==z[1].size==4 and z[0].reg==z[1].reg==index,'Byte index has unknown upper bits')
                cursor=zero.address
            else:
                require(op[0].size==4 and op[0].reg==index,'Unknown zero-extension switch index');cursor=setup.address
            branch=previous(cursor);level=2
        else:branch=setup
        require(branch.mnemonic=='ja' and branch.address+branch.size==(cursor if level==2 else at),'Switch has no unsigned fallthrough upper bound')
        require(branch.operands[0].type==X.X86_OP_IMM,'Unknown switch range branch')
        compare=previous(branch.address)
        while compare.mnemonic in ('mov','lea','nop'):
            require(input_index not in {PARTS.get(reg,reg) for reg in compare.regs_access()[1]} and X.X86_REG_EFLAGS not in compare.regs_access()[1],'Switch index/flags changed after comparison')
            compare=previous(compare.address)
        c=compare.operands
        require(compare.mnemonic=='cmp' and len(c)==2 and c[0].type==X.X86_OP_REG and c[0].size==4 and c[0].reg==input_index and c[1].type==X.X86_OP_IMM and 0<=c[1].imm<256,'Unknown original switch bound')
        count=c[1].imm+1
        if mapping is None:indices=list(range(count));map_bytes=None
        else:map_bytes=readonly(mapping,count);indices=list(map_bytes)
        slot_count=max(indices)+1;table_bytes=readonly(table,4*slot_count)
        if mapping is not None:require(table+len(table_bytes)<=mapping or mapping+len(map_bytes)<=table,'Overlapping switch index/pointer tables')
        targets=sorted({struct.unpack_from('<I',table_bytes,4*slot)[0] for slot in indices})
        require(targets and all(lo<=target<hi for target in targets),'Switch target escapes independent code body')
        literals=[dict(address=table,size=len(table_bytes),sha256=hashlib.sha256(table_bytes).hexdigest(),kind='pointer table')]
        if mapping is not None:literals.append(dict(address=mapping,size=len(map_bytes),sha256=hashlib.sha256(map_bytes).hexdigest(),kind='byte index'))
        return dict(dispatch=at,level=level,input_index=input_index,index=index,bound=count,guard=compare.address,range_branch=branch.address,targets=targets,literals=literals,guard_chain=chain)

    while True:
        nodes,edges,pred,unknown,occupied=walk()
        require(len(unknown)<=32,'Excessive unresolved switches')
        if not unknown:break
        for at in sorted(unknown):
            proof=inspect(at,nodes,edges,pred);resolved[at]=proof['targets'];proofs[at]=proof
    for at,proof in proofs.items():
        final=inspect(at,nodes,edges,pred)
        require(final==proof,'Expanded switch graph changed guard evidence')
    # An embedded table is safe only after expanding every switch edge.
    # Reject an instruction that enters any byte of a proved literal.
    for proof in proofs.values():
        for literal in proof['literals']:
            address = literal['address']
            if address < hi:
                require(occupied.keys().isdisjoint(range(address, address + literal['size'])),
                        'Embedded switch data overlaps reachable instructions')
    return dict(entry=entry,nodes=nodes,edges=edges,pred=pred,raw=raw,body_sha256=hashlib.sha256(raw).hexdigest(),switches=[proofs[at] for at in sorted(proofs)],code_end=hi,calls_assumed_returning=True,body_acceptance_claimed=False)
