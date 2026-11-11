"""Generic, qualified input/receiver provenance for a scratch capture hypothesis.

This proves register identity and ordinary-call cleanup only, never object
memory effects, alias closure, source correspondence, or body acceptance.
Unknown facts remain unknown. No target addresses or class names are built in.
"""
from collections import deque
import hashlib
import re

import capstone
from seed_guarded_cfg import X,PARTS,require,guarded_graph,CFGRefusal
from seed_frame_equations import AffineFrames
from seed_fp_virtual import physical_family
from seed_fp_abi import value_bytes

REGISTERS=(X.X86_REG_EAX,X.X86_REG_ECX,X.X86_REG_EDX,X.X86_REG_EBX,X.X86_REG_ESI,X.X86_REG_EDI,X.X86_REG_EBP)
VOLATILE=(X.X86_REG_EAX,X.X86_REG_ECX,X.X86_REG_EDX)
SEGMENTS=(0,X.X86_REG_DS,X.X86_REG_SS)

def input_contract(repository,catalog,audit,inventory,entry,params,seed):
    schema=seed.typed_schema();choice=schema.method_at(entry)
    require(choice is not None,'No unique canonical method ABI');owner,method=choice
    require(not method.static and not method.name.startswith('~'),'Unsupported static/destructor ABI')
    require(not re.search(r'\b(?:__cdecl|__stdcall|__fastcall|__declspec|__asm)\b',method.declaration),'Nonordinary member ABI')
    result=method.declaration.split(method.name,1)[0].replace('virtual','').strip()
    require(result=='void' or value_bytes(result)==4,'Unsupported member result width')
    arguments=[] if not method.parameters.strip() or method.parameters.strip()=='void' else seed.split_args(method.parameters)
    require(len(params)==len(arguments)+1,'Source/canonical physical parameter counts differ')
    roots={0:('input',0,owner.name)};offsets={};types=[]
    for index,argument in enumerate(arguments,1):
        kind,name=seed.param_parts(argument);require(name and value_bytes(kind)==4,'Unknown/non-DWORD formal ABI');types.append(kind)
        match=re.fullmatch(r'(?:const\s+)?([A-Za-z_]\w*)(?:\s+const)?\s*\*',kind.strip())
        if match and match[1] in schema.records and schema.records[match[1]].polymorphic:
            root=('input',index,match[1]);roots[index]=root;offsets[index*4]=root
    require(all(seed.clean_type(seed.param_parts(p)[0]).count('*') or seed.clean_type(seed.param_parts(p)[0]) in seed_capture_words() for p in params),'Unknown source physical parameter width')
    symbol_prefix='?'+method.name+'@'+owner.name+'@@'
    populations={pop for pop,refs in catalog.get('population_references',{}).items() if any(name.startswith(symbol_prefix) and address==entry for name,address in refs.items())}
    pins=[]
    for row in catalog['regions']:
        if row.get('population') not in populations:continue
        raw=(repository/row['source']).read_bytes()
        matches=re.findall(r'@ref\s+('+re.escape(symbol_prefix)+r'\S+)=([0-9a-fA-F]{8})',raw.decode('utf-8'))
        matches=[symbol for symbol,address in matches if int(address,16)==entry]
        if not matches:continue
        require(hashlib.sha256(raw).hexdigest()==row['source_sha256'],'Accepted interface source pin differs')
        try:caller=guarded_graph(audit,inventory,row['entry'])
        except ValueError:continue
        require(caller['body_sha256']==row['body_sha256'],'Accepted caller native pin differs')
        if not any(ins.mnemonic=='call' and len(ins.operands)==1 and ins.operands[0].type==X.X86_OP_IMM and ins.operands[0].imm&0xffffffff==entry for ins in caller['nodes'].values()):continue
        require(len(set(matches))==1 and re.match(re.escape(symbol_prefix)+r'[QU][AB]E',matches[0]),'Unsupported interface symbol')
        pins.append(dict(entry=row['entry'],source=row['source'],source_sha256=row['source_sha256'],body_sha256=row['body_sha256'],symbol=matches[0]));break
    require(pins,'No pinned accepted caller corroborates canonical method ABI')
    header=repository/'src'/owner.header;require(header.is_file(),'Canonical method header is missing')
    pins.append(dict(header='src/'+owner.header,sha256=hashlib.sha256(header.read_bytes()).hexdigest()))
    return dict(owner=owner.name,method=method.name,roots=roots,offsets=offsets,pins=pins,stack_bytes=len(arguments)*4,qualified_by_canonical_static_input_types=True,qualified_by_normal_MS_i386_CPP_call_contracts=True)

def seed_capture_words():
    return {'int','uint','undefined4','long','ulong','unsigned int','signed int','unsigned long','signed long'}

def restored_tail_target(decoder, entry, raw, cleanup):
    """A linear i386 body can forward cleanup after restoring incoming ESP."""
    instructions = list(decoder.disasm(raw, entry))
    require(instructions and sum(ins.size for ins in instructions) == len(raw),
            "Incomplete native tail decode")
    tail = instructions[-1]
    require(tail.mnemonic == "jmp" and len(tail.operands) == 1 and
            tail.operands[0].type == X.X86_OP_IMM and 0x66 not in tail.prefix,
            "No terminal direct DWORD tail jump")
    target = tail.operands[0].imm & 0xffffffff
    require(not entry <= target < entry + len(raw), "Tail target is inside the body")
    height = 0
    for ins in instructions[:-1]:
        operands = ins.operands
        if ins.mnemonic == "call":
            require(len(operands) == 1 and operands[0].type == X.X86_OP_IMM and
                    0x66 not in ins.prefix, "Unknown nested tail cleanup")
            callee = operands[0].imm & 0xffffffff
            require(not entry <= callee < entry + len(raw), "Nested call enters the tail body")
            height += cleanup(callee)
        else:
            require(not any(ins.group(group) for group in
                            (capstone.CS_GRP_JUMP, capstone.CS_GRP_CALL,
                             capstone.CS_GRP_RET, capstone.CS_GRP_INT,
                             capstone.CS_GRP_IRET)) and ins.mnemonic not in ("ud2", "hlt"),
                    "Nonlinear native tail body")
            writes_stack = {X.X86_REG_SP, X.X86_REG_ESP}.intersection(ins.regs_access()[1])
            if writes_stack:
                require(ins.mnemonic in ("add", "sub") and len(operands) == 2 and
                        operands[0].type == X.X86_OP_REG and operands[0].reg == X.X86_REG_ESP and
                        operands[0].size == 4 and operands[1].type == X.X86_OP_IMM,
                        "Unmodeled tail stack adjustment")
                height += operands[1].imm if ins.mnemonic == "add" else -operands[1].imm
            for operand in operands:
                if operand.type == X.X86_OP_MEM and operand.mem.base == X.X86_REG_ESP:
                    require(not operand.mem.index and operand.mem.segment in SEGMENTS and
                            height + operand.mem.disp < 0 and
                            height + operand.mem.disp + operand.size <= 0,
                            "Tail accesses beyond its local stack frame")
        require(height <= 0 and height % 4 == 0, "Invalid tail stack height")
    require(height == 0, "Tail jump does not restore incoming ESP")
    return target


class Receivers:
    def __init__(self,repository,catalog,audit,inventory,graph,contract):
        self.repository=repository;self.catalog=catalog;self.audit=audit;self.inventory=inventory;self.graph=graph;self.contract=contract
        self.rows={};self.cleanup_cache={};self.family_cache={};self.known={};self.families={};self.refused={}
        for row in catalog['regions']:self.rows.setdefault(row['entry'],[]).append(row)
    def accepted(self,entry):
        rows=self.rows.get(entry,[]);require(len(rows)==1,'Native virtual target is not uniquely accepted');row=rows[0]
        raw=(self.repository/row['source']).read_bytes();require(hashlib.sha256(raw).hexdigest()==row['source_sha256'],'Accepted target source differs')
        native=self.audit.original(entry,row['body_bytes']);require(hashlib.sha256(native).hexdigest()==row['body_sha256'],'Accepted target native differs');return row
    def cleanup(self,entry,active=()):
        require(entry not in active and len(active)<8,'Cyclic/deep cleanup thunk')
        if entry in self.cleanup_cache:return self.cleanup_cache[entry]
        _, row = self.inventory.function(entry)
        require(row and row.get("contiguous") and len(row.get("ranges", [])) == 1,
                "Unknown cleanup extent")
        span = row["ranges"][0]
        raw = bytes.fromhex(span["bytes_hex"])
        require(int(span["start"],16) == entry and
                int(span["end_inclusive"],16) + 1 == entry + len(raw) and
                self.audit.original(entry,len(raw)) == raw, "Cleanup original bytes differ")
        first = next(self.audit.decoder.disasm(raw,entry,1), None)
        nested = lambda address: self.cleanup(address, active + (entry,))
        if (first and first.mnemonic == "jmp" and first.size == len(raw) and
                len(first.operands) == 1 and first.operands[0].type == X.X86_OP_IMM):
            target = first.operands[0].imm & 0xffffffff
            self.accepted(target)
            result = nested(target)
        else:
            try:
                child = guarded_graph(self.audit, self.inventory, entry)
            except CFGRefusal:
                accepted = self.accepted(entry)
                require(accepted["body_bytes"] == len(raw) and 0 < len(raw) <= 65536,
                        "Accepted tail extent differs from the frozen body")
                target = restored_tail_target(self.audit.decoder, entry, raw, nested)
                result = nested(target)
            else:
                returns = [ins for ins in child["nodes"].values() if ins.mnemonic == "ret"]
                require(returns, "No independently known cleanup")
                widths = {ins.operands[0].imm if ins.operands else 0 for ins in returns}
                require(len(widths) == 1, "Nonunique native return cleanup")
                result = next(iter(widths))
        require(0<=result<=4096 and result%4==0,'Unsupported native cleanup');self.cleanup_cache[entry]=result;return result
    def family(self,root,slot):
        key=(root[2],slot)
        if key in self.family_cache:return self.family_cache[key]
        tables=physical_family(self.audit,*key);targets=[];pops=set();records=set()
        for target in sorted({row['target'] for row in tables}):
            row=self.accepted(target);pop=self.cleanup(target);pops.add(pop);symbol=row.get('symbol','')
            record=re.fullmatch(r'\?[A-Za-z_]\w*@[A-Za-z_]\w*@@[QU][AB]E\?AU([A-Za-z_]\w*)@@XZ',symbol)
            records.add(record[1] if record else None)
            targets.append(dict(entry=target,source=row['source'],source_sha256=row['source_sha256'],body_sha256=row['body_sha256'],symbol=symbol,pop=pop))
        require(len(pops)==1,'Virtual family cleanup differs');pop=next(iter(pops));record=next(iter(records)) if len(records)==1 else None
        if record:require(pop==4,'Record getter lacks one hidden result pointer')
        result=dict(class_name=key[0],slot=slot,physical_tables=tables,targets=targets,pop=pop,record_result=record);self.family_cache[key]=result;return result
    def flow(self):
        frames=AffineFrames(self.graph,self.known);nodes=self.graph['nodes'];initial={reg:None for reg in REGISTERS};initial[X.X86_REG_ECX]=self.contract['roots'][0]
        incoming={self.graph['entry']:initial};pending=deque([self.graph['entry']]);steps=0
        while pending:
            at=pending.popleft();steps+=1;require(steps<100000,'Receiver dataflow did not converge');ins=nodes[at];ops=ins.operands;before=incoming[at];after=dict(before)
            for reg in ins.regs_access()[1]:
                full=PARTS.get(reg,reg)
                if full in after:after[full]=None
            if ins.mnemonic=='call':
                for reg in VOLATILE:after[reg]=None
                family=self.families.get(at)
                if family and family['record_result']:
                    root=before.get(X.X86_REG_ECX)
                    if root and root[0]=='input':after[X.X86_REG_EAX]=('return',at,root,family['slot'],family['record_result'])
            elif ins.mnemonic=='mov' and len(ops)==2 and ops[0].type==X.X86_OP_REG and ops[0].size==4 and ops[0].reg in after:
                src=ops[1];value=None
                if src.type==X.X86_OP_REG and src.size==4:value=before.get(src.reg)
                elif src.type==X.X86_OP_MEM and src.size==4 and not src.mem.index and src.mem.segment in SEGMENTS:
                    if src.mem.base==X.X86_REG_ESP:
                        address=frames.address(at,src.mem.disp)
                        if address['proved'] and address['constant'].isdigit() and self.input_safe(at,src,frames):value=self.contract['offsets'].get(int(address['constant']))
                    elif src.mem.disp==0:
                        base=before.get(src.mem.base)
                        if base and base[0]=='input':value=('vptr',base)
                        elif base and base[0]=='return':value=('word',base,0)
                after[ops[0].reg]=value
            elif ins.mnemonic=='sub' and len(ops)==2 and ops[0].type==X.X86_OP_REG and ops[0].size==4 and ops[0].reg in after:
                left=before.get(ops[0].reg);right=None;src=ops[1]
                if src.type==X.X86_OP_REG and src.size==4:right=before.get(src.reg)
                elif src.type==X.X86_OP_MEM and src.size==4 and not src.mem.index and src.mem.segment in SEGMENTS and src.mem.disp==0:
                    base=before.get(src.mem.base)
                    if base and base[0]=='return':right=('word',base,0)
                if left and right and left[0]==right[0]=='word':after[ops[0].reg]=('difference',at,left,right)
            for target in self.graph['edges'][at]:
                if target not in incoming:incoming[target]=dict(after);pending.append(target)
                else:
                    merged={reg:after[reg] if after[reg]==incoming[target][reg] else None for reg in REGISTERS}
                    if merged!=incoming[target]:incoming[target]=merged;pending.append(target)
        return incoming,frames,steps
    def solve(self):
        for at,ins in self.graph['nodes'].items():
            if ins.mnemonic=='call' and len(ins.operands)==1 and ins.operands[0].type==X.X86_OP_IMM:
                try:self.known[at]=self.cleanup(ins.operands[0].imm&0xffffffff)
                except ValueError as error:self.refused[at]=str(error)
        for unused in range(16):
            incoming,frames,steps=self.flow();added=0
            for at,ins in self.graph['nodes'].items():
                if at in self.families or ins.mnemonic!='call' or len(ins.operands)!=1:continue
                op=ins.operands[0];before=incoming[at];root=before.get(X.X86_REG_ECX)
                if not (op.type==X.X86_OP_MEM and op.size==4 and not op.mem.index and op.mem.segment in SEGMENTS and 0<=op.mem.disp<=4096 and op.mem.disp%4==0 and root and root[0]=='input' and before.get(op.mem.base)==('vptr',root)):continue
                try:family=self.family(root,op.mem.disp//4)
                except ValueError as error:self.refused[at]=str(error);continue
                self.families[at]=family;self.known[at]=family['pop'];added+=1
            if not added:
                return incoming,frames,steps
        raise CFGRefusal('Virtual cleanup fixed point did not converge')

    def input_safe(self,at,operand,frames):
        # An argument copy may legitimately be reused after its initial load.
        # Check only predecessor paths reaching this particular native read.
        pending=list(self.graph['pred'][at]);seen=set()
        while pending:
            parent=pending.pop()
            if parent in seen:continue
            seen.add(parent);ins=self.graph['nodes'][parent]
            for op in ins.operands:
                if op.type!=X.X86_OP_MEM or op.mem.base!=X.X86_REG_ESP:continue
                if not (op.access&2 or ins.mnemonic=='lea'):continue
                if op.mem.index or op.mem.segment not in SEGMENTS:return False
                address=frames.address(parent,op.mem.disp,relative_to=at,relative_displacement=operand.mem.disp)
                if not address['proved']:return False
                offset=int(address['constant']);size=op.size if ins.mnemonic!='lea' else 1
                if offset<4 and offset+size>0:return False
            pending.extend(self.graph['pred'][parent])
        return True
