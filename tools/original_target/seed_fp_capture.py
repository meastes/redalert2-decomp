"""Discover a raw two-word scalar capture and its native FP argument witness.

Discovery is deliberately separate from alias closure and source emission.
A result is not permission to rewrite a seed until the caller's alias proof is
established. No original function address or local identifier is hardcoded.
"""
import hashlib
import re
import capstone
from seed_fp_native import X, NativeRefusal, graph, require
from seed_fp_source import bind_capture

def single_float_member(proofs, entry, seed):
    proofs.provenance(entry)
    require(entry in proofs.rows, 'Scalar member lacks accepted C++ source')
    row = proofs.rows[entry]
    source = proofs._source_bytes(row['source'], row['source_sha256']).decode('utf-8')
    view = seed.code_view(source)
    matches = list(re.finditer(r'^([A-Za-z_]\w*)\s*&\s*([A-Za-z_]\w*)::([A-Za-z_]\w*)\(float\s+[A-Za-z_]\w*\)', view, re.M))
    require(len(matches) == 1, 'Unsupported single-float member definition')
    match = matches[0]
    require(match[1] == match[2], 'Scalar member does not return its owner reference')
    prefix = '?' + match[3] + '@' + match[2] + '@@'
    symbol = row['symbol']
    require(symbol.startswith(prefix) and re.match(r'[QU][AB]EAA[UV]', symbol[len(prefix):]) and symbol.endswith('M@Z'), 'Scalar member symbol ABI differs')
    return dict(entry=entry, source=row['source'], source_sha256=row['source_sha256'], symbol=symbol)

def cleanup(proofs, entry):
    proofs.provenance(entry)
    nodes, edges, pred, raw = graph(proofs.audit, proofs.inventory, entry)
    returns = [i for i in nodes.values() if i.mnemonic == 'ret']
    pops = {i.operands[0].imm if i.operands else 0 for i in returns}
    require(len(pops) == 1, 'Unknown or nonunique direct callee cleanup')
    return next(iter(pops))

def caller_frame(proofs, entry):
    nodes, edges, pred, raw = graph(proofs.audit, proofs.inventory, entry)
    direct = {}
    unknown = []
    for at, ins in nodes.items():
        if ins.mnemonic != 'call':
            continue
        if len(ins.operands) == 1 and ins.operands[0].type == X.X86_OP_IMM:
            direct[at] = cleanup(proofs, ins.operands[0].imm & 0xffffffff)
        else:
            unknown.append(at)
    require(len(unknown) <= 1, 'Multiple unbound indirect cleanups')
    states = {entry: (0, 0)}
    pending = [entry]
    equations = []
    while pending:
        at = pending.pop()
        ins = nodes[at]
        op = ins.operands
        c, k = states[at]
        if ins.mnemonic == 'push':
            require(len(op) == 1 and op[0].size == 4, 'Non-DWORD push')
            c -= 4
        elif ins.mnemonic == 'pop':
            require(len(op) == 1 and op[0].type == X.X86_OP_REG and op[0].size == 4 and op[0].reg not in (X.X86_REG_ESP, X.X86_REG_SP), 'Unmodeled pop')
            c += 4
        elif ins.mnemonic in ('add', 'sub') and op[0].type == X.X86_OP_REG and op[0].reg == X.X86_REG_ESP:
            require(op[1].type == X.X86_OP_IMM, 'Dynamic stack allocation')
            c += op[1].imm * (1 if ins.mnemonic == 'add' else -1)
        elif ins.mnemonic == 'call':
            if at in direct:
                c += direct[at]
            else:
                require(at in unknown, 'Unmodeled call')
                k += 1
        elif ins.mnemonic == 'ret':
            require(not op or op[0].imm == 0, 'Root has stack arguments not represented by source witness')
            equations.append((c, k))
        else:
            require(not {X.X86_REG_ESP, X.X86_REG_SP}.intersection(ins.regs_access()[1]), 'Unknown root ESP effect')
        for target in edges[at]:
            if target in states:
                equations.append((c-states[target][0], k-states[target][1]))
            else:
                states[target] = (c, k)
                pending.append(target)
    if unknown:
        values = {-c//k for c, k in equations if k and (-c) % k == 0}
        require(len(values) == 1, 'Indirect cleanup is not uniquely constrained')
        pop = next(iter(values))
        require(0 <= pop <= 0x1000 and pop % 4 == 0, 'Unsupported indirect cleanup')
    else:
        pop = 0
    require(all(c+k*pop == 0 for c, k in equations), 'Caller frame constraints disagree')
    heights = {at: c+k*pop for at, (c, k) in states.items()}
    require(len(heights) == len(nodes), 'Incomplete frame reachability')
    return dict(nodes=nodes, edges=edges, pred=pred, raw=raw, heights=heights, direct_pops=direct, indirect_pops={at:pop for at in unknown}, equation_count=len(equations))

def initial_word_capture(frame, entry):
    nodes, pred, height = frame['nodes'], frame['pred'], frame['heights']
    values = {X.X86_REG_ECX: ('owner', 0)}
    stores = {0: [], 4: []}
    at = entry
    while True:
        ins = nodes[at]
        op = ins.operands
        if ins.group(capstone.CS_GRP_CALL) or ins.group(capstone.CS_GRP_JUMP) or ins.group(capstone.CS_GRP_RET):
            break
        if ins.mnemonic == 'mov' and op[0].size == op[1].size == 4:
            if op[0].type == X.X86_OP_REG:
                if op[1].type == X.X86_OP_REG:
                    value = values.get(op[1].reg)
                elif op[1].type == X.X86_OP_MEM and not op[1].mem.index and not op[1].mem.segment:
                    base = values.get(op[1].mem.base)
                    value = ('word', base[1]+op[1].mem.disp, at) if base and base[0] == 'owner' else None
                else:
                    value = None
                values[op[0].reg] = value
            elif op[0].type == X.X86_OP_MEM and op[0].mem.base == X.X86_REG_ESP and not op[0].mem.index and not op[0].mem.segment and op[1].type == X.X86_OP_REG:
                value = values.get(op[1].reg)
                if value and value[0] == 'word' and value[1] in stores:
                    stores[value[1]].append(dict(load=value[2], store=at, frame=height[at]+op[0].mem.disp))
        elif ins.mnemonic == 'lea' and op[0].type == X.X86_OP_REG:
            address = op[1]
            base = values.get(address.mem.base)
            values[op[0].reg] = ('owner', base[1]+address.mem.disp) if base and base[0] == 'owner' and not address.mem.index and not address.mem.segment else None
        else:
            for reg in ins.regs_access()[1]:
                values.pop(reg, None)
                for full, parts in ((X.X86_REG_EAX, {X.X86_REG_AX,X.X86_REG_AL,X.X86_REG_AH}), (X.X86_REG_ECX,{X.X86_REG_CX,X.X86_REG_CL,X.X86_REG_CH}), (X.X86_REG_EDX,{X.X86_REG_DX,X.X86_REG_DL,X.X86_REG_DH}), (X.X86_REG_EBX,{X.X86_REG_BX,X.X86_REG_BL,X.X86_REG_BH}), (X.X86_REG_ESI,{X.X86_REG_SI}), (X.X86_REG_EDI,{X.X86_REG_DI}), (X.X86_REG_EBP,{X.X86_REG_BP})):
                    if reg in parts:values.pop(full,None)
        targets = frame['edges'][at]
        require(len(targets) == 1 and pred[targets[0]] == {at}, 'Nonlinear initial capture prefix')
        at = targets[0]
    require(len(stores[0]) == len(stores[4]) == 1, 'Missing or ambiguous incoming-root DWORD capture')
    low, high = stores[0][0], stores[4][0]
    require(low['frame']+4 == high['frame'] and low['load'] < high['load'] and low['store'] < high['store'], 'Initial word pair is not ordered/adjacent')
    return dict(frame=low['frame'], low=low, high=high)

def argument_witness(frame, call, capture_frame):
    nodes, pred, height = frame['nodes'], frame['pred'], frame['heights']
    at = call
    store = None
    load = None
    interval = []
    for _ in range(32):
        require(len(pred[at]) == 1, 'Ambiguous floating-argument ingress')
        before = next(iter(pred[at]))
        ins = nodes[before]
        require(before+ins.size == at and not ins.group(capstone.CS_GRP_CALL) and not ins.group(capstone.CS_GRP_JUMP) and not ins.group(capstone.CS_GRP_RET), 'Nonlinear floating argument')
        if store is None and not ins.mnemonic.startswith('f'):
            # Once the outgoing float has been stored, only these non-memory-
            # writing setup operations may precede the call. Alias closure
            # protects the saved capture before the earlier FLD separately.
            safe = ins.mnemonic == 'nop' or (ins.mnemonic in ('mov','lea') and len(ins.operands)==2 and ins.operands[0].type==X.X86_OP_REG and ins.operands[0].reg not in (X.X86_REG_ESP,X.X86_REG_SP))
            if ins.mnemonic == 'mov' and len(ins.operands)==2 and ins.operands[0].type==X.X86_OP_MEM:
                dest=ins.operands[0]
                safe=dest.mem.base==X.X86_REG_ESP and not dest.mem.index and dest.mem.segment in (0,X.X86_REG_DS,X.X86_REG_SS) and (dest.mem.disp>=4 or dest.mem.disp+dest.size<=0)
            require(safe, 'Outgoing float may be overwritten before call')
        if ins.mnemonic.startswith('f'):
            require(len(ins.operands) == 1 and ins.operands[0].type == X.X86_OP_MEM, 'Unmodeled floating operation')
            operand = ins.operands[0]
            require(operand.mem.base == X.X86_REG_ESP and not operand.mem.index and not operand.mem.segment, 'Unknown FP frame operand')
            if store is None:
                require(ins.mnemonic == 'fstp' and operand.size == 4 and operand.mem.disp == 0 and height[before] == height[call], 'Not a single outgoing float argument')
                store = before
            else:
                require(ins.mnemonic == 'fld' and operand.size == 8 and height[before]+operand.mem.disp == capture_frame, 'Argument is not the raw captured double')
                load = before
                break
        interval.append(before)
        at = before
    require(store is not None and load is not None, 'Missing bounded FP argument chain')
    return dict(load=load, store=store, call=call, interval=sorted(interval))

def discover_capture(proofs, caller, source, seed):
    candidates = []
    for name in sorted(set(seed.CALLEE.findall(seed.code_view(source)))):
        entry = int(name[4:],16)
        try:
            member = single_float_member(proofs,entry,seed)
            binding = bind_capture(source,name,seed)
        except (NativeRefusal,KeyError,ValueError,OSError):
            continue
        candidates.append((entry,member,binding))
    require(len(candidates) == 1, 'Missing or ambiguous scalar capture candidate')
    callee, member, binding = candidates[0]
    frame = caller_frame(proofs,caller)
    require(cleanup(proofs,callee) == 4, 'Scalar member native cleanup differs')
    calls = sorted(at for at,ins in frame['nodes'].items() if ins.mnemonic=='call' and ins.operands[0].type==X.X86_OP_IMM and ins.operands[0].imm & 0xffffffff==callee)
    require(len(calls) == binding['source_call_count'], 'Source/native scalar call count differs')
    capture = initial_word_capture(frame,caller)
    witness = argument_witness(frame,calls[binding['source_call_ordinal']],capture['frame'])
    return dict(caller=caller,callee=callee,member=member,source_binding=binding,capture=capture,witness=witness,original_extent=len(frame['raw']),original_sha256=hashlib.sha256(frame['raw']).hexdigest(),frame_equations=frame['equation_count'],indirect_pops=frame['indirect_pops'],alias_closure_verified=False,source_emission_allowed=False)
