"""Native value/control witness for an integer-first NaN-labelled seed local.

No source emission: call/pointee alias effects must be proved separately.
Unknown values, stack locations, controls and competing candidates refuse.
"""
import capstone
from fractions import Fraction
from seed_guarded_cfg import X,PARTS,require,CFGRefusal


def components(graph):
    nodes,edges,pred=graph['nodes'],graph['edges'],graph['pred'];seen=set();order=[]
    for root in nodes:
        if root in seen:continue
        pending=[(root,False)]
        while pending:
            at,done=pending.pop()
            if done:order.append(at);continue
            if at in seen:continue
            seen.add(at);pending.append((at,True));pending.extend((child,False) for child in edges[at] if child not in seen)
    assigned={};groups=[]
    for root in reversed(order):
        if root in assigned:continue
        group=set();pending=[root];index=len(groups)
        while pending:
            at=pending.pop()
            if at in assigned:continue
            assigned[at]=index;group.add(at);pending.extend(pred[at])
        groups.append(group)
    return assigned,groups


def dominates(graph,first,last):
    if first==last:return True
    pending=[graph['entry']];seen={first}
    while pending:
        at=pending.pop()
        if at in seen:continue
        if at==last:return False
        seen.add(at);pending.extend(graph['edges'][at])
    return True


class Origins:
    def __init__(self,graph,frames,anchor,anchor_disp,normal_calls):
        self.graph=graph;self.frames=frames;self.anchor=anchor;self.anchor_disp=anchor_disp;self.normal_calls=set(normal_calls);self.cache={};self.active=set();self.crossed_calls=set()
    def address(self,at,operand):
        require(operand.type==X.X86_OP_MEM and operand.mem.base==X.X86_REG_ESP and not operand.mem.index and operand.mem.segment in (0,X.X86_REG_DS,X.X86_REG_SS),'Unknown integer stack operand')
        result=self.frames.address(at,operand.mem.disp,relative_to=self.anchor,relative_displacement=self.anchor_disp)
        require(result['proved'],'Integer field stack address is underdetermined');value=Fraction(result['constant']);require(value.denominator==1,'Nonintegral integer field address');return int(value)
    def operand(self,at,operand):
        require(operand.size==4,'Partial integer value')
        if operand.type==X.X86_OP_REG:return self.reg(at,operand.reg)
        if operand.type==X.X86_OP_IMM:return ('constant',operand.imm&0xffffffff)
        if operand.type==X.X86_OP_MEM:return ('stack',self.address(at,operand),at)
        raise CFGRefusal('Unknown integer operand')
    def reg(self,at,reg):
        key=(at,reg)
        if key in self.cache:return self.cache[key]
        require(key not in self.active and len(self.active)<128,'Cyclic/long integer value provenance');self.active.add(key)
        try:
            parents=self.graph['pred'][at];require(parents,'Unknown incoming integer register')
            values={self.step(parent,reg) for parent in parents};require(len(values)==1,'Ambiguous integer register provenance');result=next(iter(values));self.cache[key]=result;return result
        finally:self.active.remove(key)
    def step(self,at,reg):
        ins=self.graph['nodes'][at];ops=ins.operands
        if ins.mnemonic=='call':
            require(reg not in (X.X86_REG_EAX,X.X86_REG_ECX,X.X86_REG_EDX) and at in self.normal_calls,'Unverified register value across call');self.crossed_calls.add(at)
        if reg in {PARTS.get(value,value) for value in ins.regs_access()[1]}:
            require(ops and ops[0].type==X.X86_OP_REG and ops[0].reg==reg and ops[0].size==4,'Partial/implicit integer definition')
            if ins.mnemonic=='mov' and len(ops)==2:return self.operand(at,ops[1])
            if ins.mnemonic=='xor' and len(ops)==2 and ops[1].type==X.X86_OP_REG and ops[1].reg==reg:return ('constant',0)
            if ins.mnemonic=='inc':return ('increment',self.reg(at,reg),1,at)
            if ins.mnemonic=='add' and len(ops)==2 and ops[1].type==X.X86_OP_IMM:return ('increment',self.reg(at,reg),ops[1].imm,at)
            raise CFGRefusal('Unmodeled integer register definition')
        return self.reg(at,reg)
    def interval(self,start,stop,protected):
        nodes,pred=self.graph['nodes'],self.graph['pred'];at=stop;seen=set()
        while at!=start:
            require(len(pred[at])==1,'Nonlinear integer def/use interval');before=next(iter(pred[at]));require(before not in seen,'Cyclic integer interval');seen.add(before);ins=nodes[before]
            require(before+ins.size==at and ins.mnemonic!='call' and not ins.group(capstone.CS_GRP_JUMP),'Integer value crosses call/control')
            for operand in ins.operands:
                if operand.type==X.X86_OP_MEM and operand.access&capstone.CS_AC_WRITE:
                    where=self.address(before,operand);require(all(not (where<field+4 and where+operand.size>field) for field in protected),'Integer source field written after load')
            if ins.mnemonic=='push':
                result=self.frames.address(before,-4,relative_to=self.anchor,relative_displacement=self.anchor_disp);require(result['proved'],'Unknown PUSH field overlap');where=int(Fraction(result['constant']));require(all(not (where<field+4 and where+4>field) for field in protected),'PUSH overwrites integer source field')
            at=before
        return sorted(seen)


def bind_native_integer(graph,frames,binding,normal_calls,diagnostics=None):
    nodes,edges,pred=graph['nodes'],graph['edges'],graph['pred'];membership,groups=components(graph);candidates=[]
    for init,store in nodes.items():
        ops=store.operands
        if not (store.mnemonic=='mov' and len(ops)==2 and ops[0].type==X.X86_OP_MEM and ops[0].size==4 and ops[0].mem.base==X.X86_REG_ESP and not ops[0].mem.index and ops[1].type==X.X86_OP_IMM):continue
        word=ops[1].imm&0xffffffff
        if word&0x7f800000!=0x7f800000 or not word&0x7fffff or word>>31!=binding['sign_bit']:continue
        origin=Origins(graph,frames,init,ops[0].mem.disp,normal_calls)
        for compare,ins in nodes.items():
            operands=ins.operands
            if not (ins.mnemonic=='cmp' and len(operands)==2 and operands[0].type==X.X86_OP_REG and operands[0].size==4 and operands[1].type==X.X86_OP_IMM):continue
            limit=binding['comparison_limit'];immediate=operands[1].imm&0xffffffff;immediate=immediate-(1<<32) if immediate>>31 else immediate
            branch_at=compare+ins.size
            if branch_at not in nodes or pred[branch_at]!={compare}:continue
            branch=nodes[branch_at]
            if not ((immediate==limit-1 and branch.mnemonic in ('jle','jg')) or (immediate==limit and branch.mnemonic in ('jl','jge'))):continue
            if len(pred[compare])!=1:continue
            sub_at=next(iter(pred[compare]));sub=nodes[sub_at];s=sub.operands
            if not (sub.mnemonic=='sub' and sub_at+sub.size==compare and len(s)==2 and s[0].type==X.X86_OP_REG and s[0].size==s[1].size==4 and s[0].reg==operands[0].reg):continue
            try:
                left,right=origin.operand(sub_at,s[0]),origin.operand(sub_at,s[1])
                require(left[0]==right[0]=='stack' and right[1]==0 and left[1]!=0,'Subtraction is not counter minus sentinel cache');counter=left[1]
                origin.interval(left[2],sub_at,{counter,0});origin.interval(right[2],sub_at,{counter,0})
                group=groups[membership[sub_at]];require(len(group)>1 or sub_at in edges[sub_at],'Integer cache is not in a loop')
                require(init not in group and dominates(graph,init,sub_at),'Sentinel initializer does not dominate integer read')
                entries={child for at in nodes if at not in group for child in edges[at] if child in group};exits={child for at in group for child in edges[at] if child not in group}
                require(len(entries)==len(exits)==1 and all(dominates(graph,init,entry) for entry in entries),'Unknown first-lifetime loop boundary')
                updates=[];increments=[];touches=[]
                for at in sorted(group):
                    current=nodes[at];c=current.operands
                    for operand in c:
                        if operand.type!=X.X86_OP_MEM or operand.mem.base!=X.X86_REG_ESP:continue
                        where=origin.address(at,operand)
                        if where<4 and where+operand.size>0:touches.append((at,current.mnemonic,operand.access,where,operand.size))
                    if current.mnemonic!='mov' or len(c)!=2 or c[0].type!=X.X86_OP_MEM or c[0].mem.base!=X.X86_REG_ESP:continue
                    where=origin.address(at,c[0])
                    if where not in (0,counter):continue
                    require(c[0].size==4,'Partial integer cache/counter write');value=origin.operand(at,c[1])
                    if where==0:
                        require(value[0]=='stack' and value[1]==counter,'Cache update comes from another value');origin.interval(value[2],at,{counter,0});updates.append(dict(store=at,load=value[2]))
                    else:
                        require(value[0]=='increment' and value[2]==1 and value[1][0]=='stack' and value[1][1]==counter,'Counter update is not a wrapping DWORD increment');origin.interval(value[1][2],at,{counter,0});increments.append(at)
                require(len(updates)==1 and len(increments)==binding['counter_increment_count'],'Source/native integer update count differs')
                require({at for at,m,access,where,size in touches}=={right[2],updates[0]['store']} and all(m=='mov' and where==0 and size==4 for at,m,access,where,size in touches),'Extra/partial/FP/address use of integer cache in loop')
                zeros=[]
                for at,current in nodes.items():
                    c=current.operands
                    if not (current.mnemonic=='mov' and len(c)==2 and c[0].type==X.X86_OP_MEM and c[0].mem.base==X.X86_REG_ESP and c[0].size==4 and dominates(graph,at,init)):continue
                    try:
                        if origin.address(at,c[0])==counter and origin.operand(at,c[1])==('constant',0):zeros.append(at)
                    except CFGRefusal:continue
                require(len(zeros)==1,'No unique dominating zero-initialized native counter')
                def before_loop(start,field):
                    pending=list(edges[start]);seen=set()
                    while pending:
                        at=pending.pop()
                        if at in group or at in seen:continue
                        seen.add(at);current=nodes[at]
                        for operand in current.operands:
                            if operand.type==X.X86_OP_MEM and operand.access&capstone.CS_AC_WRITE and operand.mem.base==X.X86_REG_ESP:
                                where=origin.address(at,operand);require(not (where<field+4 and where+operand.size>field),'Direct integer field overwrite before first loop entry')
                        if current.mnemonic=='push':
                            height=frames.address(at,-4,relative_to=init,relative_displacement=ops[0].mem.disp);require(height['proved'],'Unknown pre-loop PUSH alias');where=int(Fraction(height['constant']));require(not (where<field+4 and where+4>field),'Pre-loop PUSH overwrites integer field')
                        pending.extend(edges[at])
                before_loop(init,0);before_loop(zeros[0],counter)
                candidates.append(dict(initializer=init,word=word,signed_value=word-(1<<32) if word>>31 else word,counter_zero=zeros[0],counter_offset_relative_cache=counter,cache_load=right[2],counter_load=left[2],subtraction=sub_at,comparison=compare,branch=branch_at,branch_taken_for_true=branch.mnemonic in ('jl','jle'),update=updates[0],counter_increments=increments,loop_entry=next(iter(entries)),loop_exit=next(iter(exits)),loop_nodes=len(group),normal_nonvolatile_call_sites=sorted(origin.crossed_calls)))
            except CFGRefusal as error:
                if diagnostics is not None:diagnostics.append(dict(initializer=init,comparison=compare,reason=str(error)))
                continue
    require(len(candidates)==1,'Missing/ambiguous native integer lifetime witness')
    return dict(candidates[0],native_source_roles_verified=True,alias_effect_closure_complete=False,source_emission_allowed=False,body_acceptance_claimed=False)
