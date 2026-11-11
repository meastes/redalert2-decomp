"""Conservative native-fact guard for merged eight-byte seed carriers.

No storage, ABI or address is inferred from a Ghidra variable name. This is
scratch syntax recovery; original bounds, references and bodies still require
normal strict acceptance. Unsupported source/control/stack shapes decline it.
"""
import re
import capstone
from seed_switches import inspect_index_switch, UnsupportedSwitch

X = capstone.x86_const
class CarrierRefusal(ValueError):
    pass

def require(condition, reason):
    if not condition:
        raise CarrierRefusal(reason)

def source_groups(body, params, seed):
    view = seed.code_view(body)
    if re.search(r'#|\b(?:typedef|using|class|struct|union|enum)\b', view): return None
    rows, comma = seed.scalar_declarations(view)
    if comma: return None
    slots = {}
    for kind, name, array in rows: slots.setdefault(name, []).append((kind, array))
    for kind, name in map(seed.param_parts, params): slots.setdefault(name, []).append((kind, 'parameter'))
    if 'CONCAT44' in slots: return None
    carriers = {name for name, decls in slots.items() if decls == [('undefined8', None)]}
    if not carriers or len(re.findall(r'\bswitch\s*\(', view)) != 1: return None
    labels = list(re.finditer(r'\bcase\s+(\d+)\s*:', view))
    if not labels or len({int(m[1]) for m in labels}) != len(labels): return None
    result = {int(m[1]): [] for m in labels}
    pattern = re.compile(r'(?m)^[\t ]*(?P<value>\w+)\s*=\s*FUN_(?P<target>[0-9a-f]{8})'
                         r'\((?P<args>[^;]*)\);\s*(?P<slot>\w+)\s*=\s*\(double\)CONCAT44'
                         r'\(\s*(?P=slot)\._4_4_,\s*(?P=value)\s*\);')
    eligible = set()
    for i, label in enumerate(labels):
        start, end = label.end(), labels[i+1].start() if i+1<len(labels) else len(view)
        chunk = view[start:end]
        for match in pattern.finditer(chunk):
            if match['slot'] not in carriers: continue
            value_decls = slots.get(match['value'], [])
            if len(value_decls)!=1 or value_decls[0][1] is not None or value_decls[0][0] not in seed.INTEGRAL32: return None
            args = seed.split_args(match['args']) if match['args'].strip() else []
            for arg in args:
                if re.fullmatch(r'-?(?:0[xX][0-9a-fA-F]+|[0-9]+)', arg.strip()): continue
                decl = slots.get(arg.strip(), [])
                if len(decl)!=1 or decl[0][1] not in (None, 'parameter') or decl[0][0] not in seed.INTEGRAL32: return None
            prefix = chunk[:match.end()]
            if re.search(r'\b(?:if|for|while|do|switch|goto|return)\b|[{}?:]', prefix): return None
            position = start+match.start('slot')
            eligible.add(position)
            result[int(label[1])].append(dict(start=position, slot=match['slot'], target=int(match['target'],16)))
    all_updates = {m.start(1) for m in re.finditer(r'(?m)^[\t ]*(\w+)\s*=\s*\(double\)CONCAT44\s*\(',view) if m[1] in carriers}
    if not eligible or eligible != all_updates: return None
    return result

def inspect_carrier_stores(audit, entry, inventory, groups, accepted=()):
    """Match straight case prefixes and actual call-result DWORD stores."""
    def row_span(address):
        _, row = inventory.function(address)
        require(row is not None and row.get('contiguous') and len(row['ranges'])==1, 'Unknown contiguous native function')
        span = row['ranges'][0]; lo, hi = int(span['start'],16), int(span['end_inclusive'],16)+1
        require(lo==address and 0<hi-lo<=0x10000, 'Unsupported native function extent')
        require(bytes.fromhex(span['bytes_hex'])==audit.original(lo,hi-lo), 'Native inventory bytes differ')
        return lo,hi
    lo,hi = row_span(entry)
    def decode(address):
        ins = next(audit.decoder.disasm(audit.original(address,16),address,1),None)
        require(ins is not None, 'Undecoded native instruction')
        return ins
    pending=[entry]; visited=set(); dispatches=[]
    while pending:
        at=pending.pop()
        if at in visited: continue
        require(lo<=at<hi and len(visited)<4096, 'Native dispatch window escaped')
        visited.add(at); ins=decode(at)
        require(at+ins.size<=hi, 'Native decode crosses inventory extent')
        if ins.group(capstone.CS_GRP_RET): continue
        if ins.group(capstone.CS_GRP_JUMP):
            op=ins.operands[0]
            if op.type==X.X86_OP_IMM: pending.append(op.imm&0xffffffff)
            else: dispatches.append(ins); continue
            if ins.mnemonic=='jmp': continue
        pending.append(at+ins.size)
    require(len(dispatches)==1,'Expected one original indexed dispatch')
    dispatch=dispatches[0]; op=dispatch.operands[0]
    require(op.type==X.X86_OP_MEM, 'Unknown dispatch operand')
    try: switch=inspect_index_switch(audit,entry,dispatch.address,op.mem.disp,accepted)
    except UnsupportedSwitch as error: raise CarrierRefusal(str(error))
    require(set(groups)==set(range(len(switch['targets']))), 'Source/native case sets differ')
    targets={address:index for index,address in enumerate(switch['targets'])}
    require(len(targets)==len(switch['targets']), 'Shared case targets are ambiguous')
    # Every repaired case prefix must have only its dispatch entry and
    # straight predecessor edges. A default/bypass/cross-case edge could
    # reach the same store with a different frame or without its call.
    graph_pending=[entry]; graph_nodes={}; predecessors={}
    while graph_pending:
        at=graph_pending.pop()
        if at in graph_nodes: continue
        require(entry<=at<switch['table'] and len(graph_nodes)<32768,'Native graph escaped')
        ins=decode(at);graph_nodes[at]=ins
        if at==dispatch.address: edges=list(switch['targets'])
        elif ins.group(capstone.CS_GRP_RET): edges=[]
        elif ins.group(capstone.CS_GRP_JUMP):
            require(ins.operands[0].type==X.X86_OP_IMM,'Additional indirect branch')
            target=ins.operands[0].imm&0xffffffff
            if entry<=target<switch['table']:edges=[target]
            else:
                require(ins.mnemonic=='jmp' and target in accepted,'Unknown native tail');edges=[]
            if ins.mnemonic!='jmp':edges.append(at+ins.size)
        else: edges=[at+ins.size]
        for target in edges:
            predecessors.setdefault(target,set()).add(at);graph_pending.append(target)
    require(all(all(p<=dispatch.address for p in predecessors.get(at,()))
                for at in graph_nodes if at<=dispatch.address),'Case reentry into native prefix')
    cleanup_cache={}; proving=set()
    def stack_effect(ins, delta):
        if ins.mnemonic=='push':
            require(ins.operands and ins.operands[0].size==4,'Unsupported PUSH width')
            return delta-4
        if ins.mnemonic=='pop':
            require(ins.operands and ins.operands[0].size==4,'Unsupported POP width')
            require(ins.operands[0].type!=X.X86_OP_REG or ins.operands[0].reg!=X.X86_REG_ESP,'POP ESP is unknown')
            return delta+4
        if ins.mnemonic in ('add','sub') and ins.operands[0].type==X.X86_OP_REG and ins.operands[0].reg==X.X86_REG_ESP:
            require(ins.operands[1].type==X.X86_OP_IMM,'Unknown ESP arithmetic')
            return delta+(ins.operands[1].imm if ins.mnemonic=='add' else -ins.operands[1].imm)
        if ins.mnemonic=='call':
            require(ins.operands[0].type==X.X86_OP_IMM and ins.operands[0].size==4 and 0x66 not in ins.prefix,'Unknown indirect/16-bit callee cleanup')
            return delta+cleanup(ins.operands[0].imm&0xffffffff)
        require(not {X.X86_REG_ESP,X.X86_REG_SP}.intersection(ins.regs_access()[1]), 'Unsupported native ESP write')
        return delta
    def cleanup(address):
        if address in cleanup_cache: return cleanup_cache[address]
        require(address not in proving and len(proving)<16,'Recursive/unknown callee cleanup')
        proving.add(address); lower,upper=row_span(address); todo=[(address,0)]; done={}; pops=set()
        while todo:
            at,delta=todo.pop()
            if at in done:
                require(done[at]==delta,'Callee stack states disagree'); continue
            require(lower<=at<upper and len(done)<2048,'Callee graph escaped')
            done[at]=delta; ins=decode(at)
            require(at+ins.size<=upper, 'Callee decode crosses extent')
            if ins.group(capstone.CS_GRP_RET):
                require(ins.mnemonic=='ret' and 0x66 not in ins.prefix,'Unsupported far/16-bit callee return')
                require(delta==0,'Callee ESP is not restored'); pops.add(ins.operands[0].imm if ins.operands else 0); continue
            if ins.group(capstone.CS_GRP_JUMP):
                require(ins.operands[0].type==X.X86_OP_IMM,'Unknown callee branch')
                todo.append((ins.operands[0].imm&0xffffffff,delta))
                if ins.mnemonic=='jmp':continue
            todo.append((at+ins.size,stack_effect(ins,delta)))
        require(len(pops)==1,'Callee returns disagree'); value=pops.pop()
        require(0<=value<=256 and value%4==0,'Unsupported callee pop')
        proving.remove(address); cleanup_cache[address]=value; return value
    # Resolve the frame anchor through the pre-dispatch graph. Case paths
    # below are deliberately linear until every requested store is matched.
    todo=[(entry,0,None)]; done={}; anchor_state=None
    while todo:
        at,delta,anchor=todo.pop()
        if at in done:
            require(done[at]==(delta,anchor),'Pre-dispatch frame states disagree'); continue
        require(lo<=at<=dispatch.address and len(done)<4096,'Pre-dispatch path escaped')
        done[at]=(delta,anchor); ins=decode(at)
        if at==dispatch.address: anchor_state=(delta,anchor); continue
        if ins.group(capstone.CS_GRP_RET): continue
        if (ins.mnemonic=='and' and ins.operands[0].type==X.X86_OP_REG and ins.operands[0].reg==X.X86_REG_ESP):
            require(anchor is None and ins.operands[1].type==X.X86_OP_IMM and ins.operands[1].imm&0xffffffff==0xfffffff8,'Unsupported stack alignment')
            todo.append((at+ins.size,0,at));continue
        if ins.group(capstone.CS_GRP_JUMP):
            require(ins.operands[0].type==X.X86_OP_IMM,'Unknown prefix branch'); target=ins.operands[0].imm&0xffffffff
            # The guard's default path does not participate in case stores.
            if target<=dispatch.address: todo.append((target,delta,anchor))
            if ins.mnemonic=='jmp':continue
        todo.append((at+ins.size,stack_effect(ins,delta),anchor))
    require(anchor_state is not None,'Missing native dispatch frame')
    matched=[]; locations={}
    for case, requested in groups.items():
        if not requested:continue
        at=switch['targets'][case]; delta,anchor=anchor_state; active=None; steps=0; cursor=0; case_seen=set(); previous=dispatch.address
        while cursor<len(requested):
            require(entry<=at<switch['table'] and at not in case_seen and len(case_seen)<2048,'Nonlinear native case prefix')
            require(predecessors.get(at,set())=={previous},'Additional incoming case-prefix edge')
            case_seen.add(at); ins=decode(at)
            require(not ins.group(capstone.CS_GRP_JUMP) and not ins.group(capstone.CS_GRP_RET),'Native case ends before matched stores')
            if ins.mnemonic=='call':
                require(ins.operands[0].type==X.X86_OP_IMM and ins.operands[0].size==4 and 0x66 not in ins.prefix,'Unknown case callee')
                active=dict(target=ins.operands[0].imm&0xffffffff,call=at,writes=[]);steps=0
            elif active:
                steps+=1
                if (ins.mnemonic=='mov' and len(ins.operands)==2 and ins.operands[0].type==X.X86_OP_MEM and ins.operands[1].type==X.X86_OP_REG and ins.operands[1].reg==X.X86_REG_EAX):
                    dst=ins.operands[0]
                    require(dst.size==4 and dst.mem.base==X.X86_REG_ESP and not dst.mem.index,'Call result is not a direct stack DWORD store')
                    offset=delta+dst.mem.disp; wanted=requested[cursor]
                    require(active['target']==wanted['target'],'Source/native call order differs')
                    require(all(not (start<offset+8 and start+width>offset) for start,width in active['writes']),'Carrier is overwritten before its result store')
                    location=(anchor,offset); old=locations.setdefault(wanted['slot'],location)
                    require(old==location,'Carrier native offsets disagree')
                    matched.append(dict(start=wanted['start'],slot=wanted['slot'],case=case,store=at,call=active['call'],target=active['target'],anchor=anchor,offset=offset));cursor+=1;active=None
                else:
                    writes=[]
                    if ins.mnemonic=='push':writes.append((delta-4,4))
                    for operand in ins.operands:
                        if operand.type==X.X86_OP_MEM and operand.access&capstone.CS_AC_WRITE:
                            require(operand.mem.base==X.X86_REG_ESP and not operand.mem.index,'Unknown intervening memory write')
                            writes.append((delta+operand.mem.disp,operand.size))
                    active['writes'].extend(writes)
                    if {X.X86_REG_EAX,X.X86_REG_AX,X.X86_REG_AL,X.X86_REG_AH}.intersection(ins.regs_access()[1]) or steps>=5:active=None
            delta=stack_effect(ins,delta);previous=at;at+=ins.size
    values=list(locations.items())
    for i,(name,(anchor,offset)) in enumerate(values):
        for other,(other_anchor,other_offset) in values[i+1:]:
            require(anchor==other_anchor and abs(offset-other_offset)>=8,'Ambiguous carrier storage overlap')
    require(len(matched)==sum(map(len,groups.values())),'Not every source store was matched')
    return dict(stores=matched,switch=switch,callee_cleanup={format(k,'08x'):v for k,v in cleanup_cache.items()},scope='Scratch carrier store facts only; no accepted body or layout inference.')
