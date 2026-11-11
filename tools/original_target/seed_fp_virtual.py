"""Source-backed virtual ABI closure from physical original-image RTTI.
No fixed lookup address, call address, slot index or class-family table.
"""
import re
import struct
from seed_fp_native import X,PARTS,require,NativeRefusal
from seed_fp_abi import member_stack_arguments


def pointer_result(proofs,entry,seed):
    result=member_stack_arguments(proofs,entry,seed)
    match=re.fullmatch(r'(?:const\s+)?([A-Za-z_]\w*)\s*\*',result['return_type'])
    require(match is not None,'Lookup result is not a simple class pointer')
    name=match[1]
    require(re.search(r'@@[QU][AB]EP[AB][UV]'+re.escape(name)+r'@@',result['symbol']),'Lookup pointer result symbol differs')
    return dict(result,class_name=name)


class ReceiverFlow:
    """Backward exact-value trace through an already verified caller prefix.

    Completed prefix effects account for every possible frame write and call;
    this trace uses them to refuse partial/alias/helper overwrites of a spill.
    It does not assume that a stack slot is unescaped merely from its spelling.
    """
    def __init__(self,machine):
        self.machine=machine;self.frame=machine.frame;self.cache={};self.active=set()
        self.writes={}
        for event in machine.write_events:self.writes.setdefault(int(event['address'],16),[]).append(event)
        self.calls={int(event['address'],16) for event in machine.call_events}
        self.trace=[]

    def query(self,at,kind,value):
        key=(at,kind,value)
        if key in self.cache:return self.cache[key]
        require(key not in self.active,'Cyclic receiver provenance')
        require(len(self.active)<128,'Receiver provenance depth exceeded')
        self.active.add(key)
        parents=self.frame['pred'][at]
        require(parents,'Receiver provenance reached an unknown incoming value')
        values={self.step(parent,kind,value) for parent in parents}
        require(len(values)==1,'Ambiguous receiver/spill provenance')
        result=next(iter(values));self.active.remove(key);self.cache[key]=result
        return result

    def operand(self,at,operand):
        require(operand.size==4,'Partial receiver value')
        if operand.type==X.X86_OP_REG:return self.query(at,'reg',operand.reg)
        require(operand.type==X.X86_OP_MEM and not operand.mem.index and operand.mem.segment in (0,X.X86_REG_DS,X.X86_REG_SS),'Unsupported receiver load')
        if operand.mem.base==X.X86_REG_ESP:return self.query(at,'stack',self.frame['heights'][at]+operand.mem.disp)
        require(operand.mem.base and operand.mem.disp==0,'Not a primary vptr load')
        owner=self.query(at,'reg',operand.mem.base)
        require(owner[0]=='return','Vptr load lacks a unique typed return origin')
        return ('vptr',owner)

    def step(self,at,kind,value):
        require(at in self.machine.completed,'Receiver trace crossed an unverified prefix instruction')
        ins=self.frame['nodes'][at];ops=ins.operands
        if ins.mnemonic=='call':require(at in self.calls,'Receiver spill trace crossed an unverified call')
        if kind=='stack':
            overlap=[event for event in self.writes.get(at,[]) if event['frame']<value+4 and event['frame']+event['size']>value]
            if overlap:
                require(len(overlap)==1 and overlap[0]['frame']==value and overlap[0]['size']==4,'Partial/ambiguous receiver spill overwrite')
                require(ins.mnemonic in ('mov','push'),'Helper/FP/REP receiver spill overwrite')
                source=ops[1] if ins.mnemonic=='mov' else ops[0]
                self.trace.append(dict(address=at,kind='spill',frame=value))
                return self.operand(at,source)
        else:
            if ins.mnemonic=='call':
                require(at in self.calls,'Receiver trace crossed an unverified call')
                if value in (X.X86_REG_EAX,X.X86_REG_ECX,X.X86_REG_EDX):
                    require(value==X.X86_REG_EAX and len(ops)==1 and ops[0].type==X.X86_OP_IMM,'Unknown volatile receiver value across call')
                    return ('return',at,ops[0].imm&0xffffffff)
            writes={PARTS.get(reg,reg) for reg in ins.regs_access()[1]}
            if value in writes:
                require(ins.mnemonic=='mov' and len(ops)==2 and ops[0].type==X.X86_OP_REG and ops[0].reg==value and ops[0].size==4,'Unmodeled receiver register definition')
                self.trace.append(dict(address=at,kind='register',register=value))
                return self.operand(at,ops[1])
        return self.query(at,kind,value)

    def virtual(self,at):
        ins=self.frame['nodes'][at]
        require(ins.mnemonic=='call' and len(ins.operands)==1,'Not a virtual call')
        operand=ins.operands[0]
        require(operand.type==X.X86_OP_MEM and operand.size==4 and operand.mem.base and not operand.mem.index and operand.mem.segment in (0,X.X86_REG_DS,X.X86_REG_SS),'Unknown virtual dispatch operand')
        require(0<=operand.mem.disp<=4096 and operand.mem.disp%4==0,'Unknown virtual slot displacement')
        vptr=self.query(at,'reg',operand.mem.base);receiver=self.query(at,'reg',X.X86_REG_ECX)
        require(vptr[0]=='vptr' and vptr[1]==receiver and receiver[0]=='return','Vptr and this have different/unknown object origins')
        return dict(lookup_call=receiver[1],lookup_entry=receiver[2],slot=operand.mem.disp//4,trace=self.trace)


def physical_family(audit,class_name,slot):
    """Enumerate RTTI/COL-backed original tables directly from initialized data.

    Class-family membership comes from original CHD/base descriptors, not the
    class inventory's hypothesis list. Nonzero/virtual base adjustment refuses.
    Only a primary offset-zero family is currently supported.
    """
    require(re.fullmatch(r'[A-Za-z_]\w*',class_name) and 0<=slot<=1024,'Unsupported static receiver type/slot')
    metadata=[];code=[]
    for section in audit.pe.sections:
        start=audit.base+section.VirtualAddress;size=min(section.SizeOfRawData,section.Misc_VirtualSize)
        if size<=0:continue
        if section.Characteristics & 0x20000020:code.append((start,start+size))
        elif section.Characteristics & 0x40:metadata.append((start,start+size,audit.original(start,size)))
    require(metadata and code,'Missing original image section bounds')
    def inside(address,size=1):return next(((lo,hi,data) for lo,hi,data in metadata if lo<=address and address+size<=hi),None)
    def raw(address,size):
        span=inside(address,size);require(span is not None,'RTTI structure escaped initialized data')
        lo,hi,data=span;return data[address-lo:address-lo+size]
    def word(address):return struct.unpack('<I',raw(address,4))[0]
    def executable(address):return any(lo<=address<hi for lo,hi in code)
    def typename(descriptor):
        span=inside(descriptor,12);require(span is not None,'Unknown RTTI type descriptor')
        lo,hi,data=span;head=raw(descriptor,8);vtable,spare=struct.unpack('<II',head)
        require(spare==0 and inside(vtable,4) and executable(word(vtable)),'Unknown RTTI type_info header')
        value=raw(descriptor+8,min(512,hi-descriptor-8)).split(b'\0',1)[0]
        require(value.startswith((b'.?AV',b'.?AU')) and value.endswith(b'@@') and all(32<c<127 for c in value),'Unknown RTTI decorated type name')
        return value.decode('ascii'),vtable
    wanted={'.?AV'+class_name+'@@','.?AU'+class_name+'@@'}
    found=[];type_info=set();required_descriptors=set();primary_descriptors=set()
    for lo,hi,data in metadata:
        for offset in range(0,len(data)-7,4):
            locator,first=struct.unpack_from('<II',data,offset)
            if not executable(first) or not inside(locator,20):continue
            signature,object_offset,cd,descriptor,hierarchy=struct.unpack('<IIIII',raw(locator,20))
            if signature or object_offset>65536 or not inside(descriptor,12):continue
            # Not every data word before a code pointer is a locator. A real
            # type_info header/name establishes the RTTI candidate first.
            descriptor_span=inside(descriptor,12)
            _,descriptor_end,_=descriptor_span
            possible=raw(descriptor+8,min(512,descriptor_end-descriptor-8)).split(b'\0',1)[0]
            if not (possible.startswith((b'.?AV',b'.?AU')) and possible.endswith(b'@@')):continue
            name,tinfo=typename(descriptor)
            sig,attributes,count,array=struct.unpack('<IIII',raw(hierarchy,16))
            require(sig==0 and 0<count<=256 and inside(array,count*4),'Unknown RTTI class hierarchy')
            bases=[]
            for index in range(count):
                base=word(array+4*index);td,contained,mdisp,pdisp,vdisp,flags=struct.unpack('<IIiiiI',raw(base,24));base_name,base_tinfo=typename(td)
                require(base_tinfo==tinfo and contained<count,'Inconsistent RTTI base hierarchy')
                bases.append((base_name,mdisp,pdisp,vdisp,flags))
            require(bases[0][:4]==(name,0,-1,0),'RTTI self descriptor differs')
            matching=[base for base in bases if base[0] in wanted]
            if not matching:continue
            require(len(matching)==1 and matching[0][1:]==(0,-1,0,0),'Unsupported virtual/nonzero/ambiguous receiver base')
            require(cd==0,'Unknown construction displacement for receiver family')
            required_descriptors.add(descriptor)
            if object_offset:continue
            primary_descriptors.add(descriptor)
            table=lo+offset+4
            for index in range(slot+1):require(executable(word(table+4*index)),'Virtual slot exceeds original code-pointer run')
            target=word(table+4*slot);match=re.fullmatch(r'\.\?A[UV]([A-Za-z_]\w*)@@',name)
            found.append(dict(vtable=table,col=locator,class_name=match[1] if match else name,type_descriptor_name=name,target=target))
            type_info.add(tinfo)
    require(found and len(type_info)==1,'Missing or inconsistent physical receiver family')
    require(required_descriptors<=primary_descriptors,'Receiver family has no verified primary table for a dynamic type')
    require(len({row['vtable'] for row in found})==len(found),'Duplicate physical family tables')
    return sorted(found,key=lambda row:row['vtable'])


def virtual_call(machine,proofs,seed):
    """Prove virtual ABI closure, then apply the normal no-frame opaque effect."""
    flow=ReceiverFlow(machine).virtual(machine.at)
    lookup=pointer_result(proofs,flow['lookup_entry'],seed)
    require(flow['lookup_call'] in machine.frame['direct_pops'],'Missing lookup native ABI evidence')
    require(lookup['stack_argument_bytes']==machine.frame['direct_pops'][flow['lookup_call']],'Lookup source/native ABI differs')
    family=physical_family(proofs.audit,lookup['class_name'],flow['slot'])
    contracts={target:member_stack_arguments(proofs,target,seed) for target in sorted({row['target'] for row in family})}
    signatures={(contract['return_type'],tuple(contract['parameter_types']),contract['stack_argument_bytes']) for contract in contracts.values()}
    require(len(signatures)==1,'Virtual target ABI set disagrees')
    result,parameters,argument_bytes=next(iter(signatures))
    require(argument_bytes==machine.frame['indirect_pops'].get(machine.at),'Virtual ABI and independent frame cleanup disagree')
    machine.opaque(argument_bytes)
    return dict(kind='inferred virtual ABI',callee=lookup['class_name']+' slot%d'%flow['slot'],static_class=lookup['class_name'],slot=flow['slot'],lookup_entry=flow['lookup_entry'],lookup_call=flow['lookup_call'],flow_trace=flow['trace'],physical_tables=family,target_symbols={str(target):contract['symbol'] for target,contract in contracts.items()},return_type=result,parameter_types=list(parameters),stack_argument_bytes=argument_bytes,physical_family_complete=True)
