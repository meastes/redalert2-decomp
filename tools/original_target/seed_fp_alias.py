"""Qualified caller-frame alias interpreter.

Call effects are supplied by a trusted, independently validated contract adapter.
This module neither discovers those contracts nor authorizes source emission.
It performs no import-time I/O and contains no game addresses or ABI-role table.
"""
from collections import deque
import capstone
from seed_fp_native import X, PARTS, require, NativeRefusal

EMPTY=frozenset()
SCALAR=frozenset({('scalar',None)})
GP=(X.X86_REG_EAX,X.X86_REG_ECX,X.X86_REG_EDX,X.X86_REG_EBX,X.X86_REG_ESI,X.X86_REG_EDI,X.X86_REG_EBP)

def frames(value):
    return {offset for kind,offset in value if kind=='frame'}

def shifted(value,delta):
    return frozenset((kind, (None if offset is None else ((offset+delta)&0xffffffff if kind=='constant' else offset+delta))) for kind,offset in value)

def lost_offset(value):
    return SCALAR | frozenset(('frame',None) for kind,offset in value if kind=='frame')

def constant(value):
    require(len(value)==1 and next(iter(value))[0]=='constant','Unknown or path-dependent integer constant')
    return next(iter(value))[1]

def merge(states):
    states=list(states)
    regs={key:frozenset().union(*(state[0].get(key,SCALAR) for state in states)) for key in GP}
    keys=set().union(*(state[1].keys() for state in states))
    memory={key:frozenset().union(*(state[1].get(key,SCALAR) for state in states)) for key in keys}
    return regs,memory

class PrefixMachine:
    """One hash-checked caller graph and one saved scalar interval.

    The effect adapter receives this machine at each call. It must establish
    source/native provenance, ABI, pointer inputs and all writes/volatile output
    aliases; it applies writes through store(). Unknown effects must refuse.
    A successful run is only a qualified alias result, never rewrite permission.
    """
    def __init__(self,frame,entry,target,capture):
        self.frame=frame;self.entry=entry;self.target=target
        self.protected=(capture['frame'],capture['frame']+8)
        self.initial={capture['low']['store']:(capture['low']['frame'],4),capture['high']['store']:(capture['high']['frame'],4)}
        require(len(self.initial)==2 and sorted(self.initial.values())==[(capture['frame'],4),(capture['frame']+4,4)],'Invalid two-word capture stores')
        self.write_events=[];self.call_events=[];self.rep_events=[];self.formed=[];self.completed=set()

    def rv(self,reg):
        if reg==X.X86_REG_ESP:return frozenset({('frame',self.height)})
        value=self.regs.get(PARTS.get(reg,reg),SCALAR)
        return lost_offset(value) if reg in PARTS else value

    def addresses(self,operand):
        require(operand.type==X.X86_OP_MEM and operand.mem.segment in (0,X.X86_REG_DS,X.X86_REG_SS),'Unknown address segment')
        base=self.rv(operand.mem.base);index=self.rv(operand.mem.index) if operand.mem.index else EMPTY
        offsets=frames(base)
        require(None not in offsets and not frames(index),'Unknown frame-derived index/address')
        require(not offsets or all(kind=='frame' for kind,offset in base),'Mixed frame/non-frame base is not a bounded address')
        if not offsets:return set()
        extra=constant(index)*operand.mem.scale if operand.mem.index else 0
        return {offset+operand.mem.disp+extra for offset in offsets}

    def readmem(self,addresses,size):
        return frozenset().union(*(self.memory.get(address+byte,SCALAR) for address in addresses for byte in range(size))) if addresses else SCALAR

    def read(self,operand):
        if operand.type==X.X86_OP_REG:return self.rv(operand.reg)
        if operand.type==X.X86_OP_IMM:return frozenset({('constant',operand.imm&0xffffffff)})
        if operand.type==X.X86_OP_MEM:return self.readmem(self.addresses(operand),operand.size)
        raise NativeRefusal('Unknown operand')

    def store(self,addresses,size,value,reason):
        require(size>0,'Unknown write width')
        require(len(addresses)<=1,'Ambiguous write destination requires a weak memory update')
        require(addresses or not frames(value),'Caller frame pointer published outside frame')
        # Memory carries possible frame aliases, never a falsely reconstructed
        # integer constant from byte fragments or a partially initialized word.
        aliases=frozenset(tag for tag in value if tag[0]=='frame')
        value=aliases | (SCALAR if not value or any(tag[0]!='frame' for tag in value) else EMPTY)
        for dest in addresses:
            require(dest is not None,'Unknown frame write offset')
            if dest<self.protected[1] and dest+size>self.protected[0]:
                require(self.initial.get(self.at)==(dest,size) and reason=='direct','Saved capture clobbered')
            for word in range(dest//4*4,(dest+size-1)//4*4+1,4):
                if dest<=word and word+4<=dest+size:continue
                for byte in range(word,word+4):
                    if not dest<=byte<dest+size:self.memory[byte]=lost_offset(self.memory.get(byte,SCALAR))
            for byte in range(size):self.memory[dest+byte]=value
            self.write_events.append(dict(address='%08x'%self.at,frame=dest,size=size,reason=reason))

    def write(self,operand,value):
        if operand.type==X.X86_OP_REG:
            reg=PARTS.get(operand.reg,operand.reg)
            require(reg in GP,'Unsupported general register write')
            self.regs[reg]=value if operand.size==4 else lost_offset(self.rv(reg)|value)
        else:self.store(self.addresses(operand),operand.size,value,'direct')

    def numeric_buffer(self,value,size):
        require(size>0,'Unknown numeric input extent')
        for start in frames(value):
            require(start is not None and not frames(self.readmem({start},size)),'Frame alias stored in numeric input')

    def opaque(self,argument_bytes):
        """Apply *already provenance/ABI-verified* normal C++ opaque effects.

        argument_bytes is the proved supplied stack-argument extent, not merely
        a RET cleanup. Cdecl/tailcall contracts must establish it independently."""
        require(argument_bytes>=0 and argument_bytes%4==0,'Unknown opaque argument extent')
        require(not any(frames(self.rv(reg)) for reg in GP),'Frame register supplied to opaque call')
        require(not any(frames(self.readmem({self.height+off},4)) for off in range(0,argument_bytes,4)),'Frame stack argument supplied to opaque call')
        for reg in (X.X86_REG_EAX,X.X86_REG_ECX,X.X86_REG_EDX):self.regs[reg]=SCALAR

    def run(self,effect_handler):
        nodes,edges,pred=self.frame['nodes'],self.frame['edges'],self.frame['pred']
        require(self.target in nodes,'Missing protected-load target')
        need={self.target};pending=[self.target]
        while pending:
            for parent in pred[pending.pop()]:
                if parent not in need:need.add(parent);pending.append(parent)
        indegree={at:len(pred[at]&need) for at in need};ready=deque(at for at,n in indegree.items() if n==0);order=[]
        while ready:
            at=ready.popleft();order.append(at)
            for child in edges[at]:
                if child in need:
                    indegree[child]-=1
                    if not indegree[child]:ready.append(child)
        require(len(order)==len(need) and order[0]==self.entry and order[-1]==self.target,'Unknown cyclic or disconnected prefix')
        states={}
        for at in order:
            self.at=at;self.height=self.frame['heights'][at]
            self.regs,self.memory=({reg:SCALAR for reg in GP},{}) if at==self.entry else merge(states[p] for p in pred[at]&need)
            ins=nodes[at];op=ins.operands;m=ins.mnemonic
            if m=='mov':self.write(op[0],self.read(op[1]))
            elif m in ('movsx','movzx'):self.write(op[0],lost_offset(self.read(op[1])))
            elif m=='lea':
                addresses=self.addresses(op[1]);self.write(op[0],frozenset(('frame',n) for n in addresses) if addresses else SCALAR)
                if addresses:self.formed.append(dict(address='%08x'%at,frames=sorted(addresses)))
            elif m=='push':self.store({self.height-4},4,self.read(op[0]),'push')
            elif m=='pop':self.write(op[0],self.readmem({self.height},4))
            elif m in ('add','sub') and op[0].type==X.X86_OP_REG and op[0].reg==X.X86_REG_ESP:pass
            elif m in ('add','sub') and op[0].type==X.X86_OP_REG and op[1].type==X.X86_OP_IMM:self.write(op[0],shifted(self.read(op[0]),op[1].imm*(1 if m=='add' else -1)))
            elif m=='rep movsd':
                count=constant(self.rv(X.X86_REG_ECX));require(0<count<=1024,'Unbounded REP count')
                src,dest=frames(self.rv(X.X86_REG_ESI)),frames(self.rv(X.X86_REG_EDI))
                require(None not in src|dest and len(src)==len(dest)==1 and all(kind=='frame' for kind,offset in self.rv(X.X86_REG_ESI)|self.rv(X.X86_REG_EDI)),'Unknown REP input/output frame')
                source,output=next(iter(src)),next(iter(dest))
                for word in range(count):self.store({output+4*word},4,self.readmem({source+4*word},4),'rep movsd')
                self.regs[X.X86_REG_ESI]=shifted(self.rv(X.X86_REG_ESI),4*count);self.regs[X.X86_REG_EDI]=shifted(self.rv(X.X86_REG_EDI),4*count);self.regs[X.X86_REG_ECX]=frozenset({('constant',0)})
                self.rep_events.append(dict(address='%08x'%at,source=source,output=output,bytes=count*4))
            elif m=='call':
                require(callable(effect_handler),'Missing verified call effect provider')
                event=effect_handler(self,ins)
                require(isinstance(event,dict) and event.get('kind'),'Call effect provider did not establish a contract')
                self.call_events.append(dict(address='%08x'%at,**event))
            elif m in ('fst','fstp','fistp','fnstcw','fnstsw'):
                if op[0].type==X.X86_OP_MEM:self.store(self.addresses(op[0]),op[0].size,SCALAR,'floating store')
                elif op[0].type==X.X86_OP_REG and op[0].reg in tuple(getattr(X,'X86_REG_ST%d'%n) for n in range(8)):pass
                else:self.write(op[0],SCALAR)
            elif m in ('fld','fild','fldz','fld1','fcom','fcomp','fcompp','fadd','faddp','fmul','fmulp','fsub','fsubp','fsubr','fsubrp','fdiv','fdivp','fdivr','fdivrp','fchs','fabs','ftst','fucom','fucomp'):
                require(not any(o.type==X.X86_OP_MEM and frames(self.read(o)) for o in op),'Pointer bits consumed by FP operation')
                require(not set(GP+(X.X86_REG_ESP,)).intersection(ins.regs_access()[1]),'Unknown FP general-register effect')
            elif m in ('xor','and','or','shl','shr','sar','imul','neg','inc','dec','add','sub','sbb','adc','cdq'):
                if m=='cdq':self.regs[X.X86_REG_EDX]=lost_offset(self.rv(X.X86_REG_EAX))
                elif m=='xor' and len(op)==2 and op[0].type==op[1].type==X.X86_OP_REG and op[0].reg==op[1].reg:self.write(op[0],frozenset({('constant',0)}))
                else:
                    require(op[0].type==X.X86_OP_REG,'Unmodeled integer memory effect')
                    self.write(op[0],lost_offset(frozenset().union(*(self.read(o) for o in op))))
            elif m.startswith('set'):self.write(op[0],SCALAR)
            elif m in ('cmp','test','nop') or ins.group(capstone.CS_GRP_JUMP):pass
            else:raise NativeRefusal(('Unmodeled caller instruction',hex(at),m,ins.op_str))
            states[at]=(dict(self.regs),dict(self.memory))
            self.completed.add(at)
        require(not frames(self.readmem({self.protected[0]},8)),'Captured scalar contains a frame alias')
        touched=[event for event in self.write_events if event['frame']<self.protected[1] and event['frame']+event['size']>self.protected[0]]
        require(sorted((int(event['address'],16),event['frame'],event['size']) for event in touched)==sorted((at,where,size) for at,(where,size) in self.initial.items()),'Missing or duplicate capture initialization')
        return dict(prefix_nodes=len(need),prefix_edges=sum(child in need for at in need for child in edges[at]),capture_interval=list(self.protected),writes_touching_capture=touched,formed_frame_addresses=self.formed,calls=self.call_events,rep_copies=self.rep_events,native_alias_closure_complete=True,verified_call_effect_provider_required=True,source_emission_allowed=False)
