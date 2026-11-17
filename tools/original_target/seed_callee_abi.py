"""Narrow scratch ABI view for an accepted member with a dropped trailing arg.

The frozen decompile is never changed. Native bodies establish physical stack
arity and incoming-this identity; the accepted definition supplies real pointer
and scalar types. This is seed preparation, not body acceptance.
"""
import re
import hashlib
from collections import defaultdict
import capstone

X = capstone.x86_const
class AbiRefusal(ValueError):
    pass

def require(condition, reason):
    if not condition:
        raise AbiRefusal(reason)

def graph(audit, inventory, entry):
    _, row = inventory.function(entry)
    require(row and row.get('contiguous') and len(row.get('ranges', [])) == 1,
            'Unknown contiguous function')
    span = row['ranges'][0]
    lo, hi = int(span['start'], 16), int(span['end_inclusive'], 16)+1
    require(lo == entry and 0 < hi-lo <= 0x10000, 'Unsupported function extent')
    raw = audit.original(lo, hi-lo)
    require(raw == bytes.fromhex(span['bytes_hex']), 'Original inventory bytes differ')
    nodes, edges, pred, occupied, pending = {}, {}, defaultdict(set), {}, [entry]
    while pending:
        at = pending.pop()
        if at in nodes: continue
        require(lo <= at < hi and len(nodes) < 8192, 'Function graph escaped')
        ins = next(audit.decoder.disasm(raw[at-lo:], at, 1), None)
        require(ins is not None and at+ins.size <= hi, 'Undecoded function instruction')
        require(all(b not in occupied or occupied[b] == at for b in range(at, at+ins.size)),
                'Overlapping original decode')
        for b in range(at, at+ins.size): occupied[b] = at
        nodes[at] = ins
        require(ins.mnemonic not in ('retf','retfq','iret','iretd','int','int1','int3','ud2','hlt') and
                not (ins.group(capstone.CS_GRP_CALL) and ins.mnemonic != 'call'),
                'Unknown control transfer')
        if ins.group(capstone.CS_GRP_RET):
            require(ins.mnemonic == 'ret' and ins.bytes[0] in (0xc2, 0xc3), 'Non-DWORD return')
            targets = []
        elif ins.group(capstone.CS_GRP_JUMP):
            require(len(ins.operands) == 1 and ins.operands[0].type == X.X86_OP_IMM,
                    'Unknown indirect branch')
            require(ins.mnemonic == 'jmp' or ins.mnemonic.startswith('j'), 'Unmodeled loop branch')
            targets = [ins.operands[0].imm & 0xffffffff]
            if ins.mnemonic != 'jmp': targets.append(at+ins.size)
        else:
            targets = [at+ins.size]
        edges[at] = targets
        for target in targets: pred[target].add(at)
        pending.extend(targets)
    return nodes, edges, pred, raw

def reachable(edges, starts, blocked=()):
    pending, seen = list(starts), set()
    while pending:
        at = pending.pop()
        if at in seen or at in blocked: continue
        seen.add(at); pending.extend(edges[at])
    return seen

REGISTER_PARTS = {
    X.X86_REG_EBX: {X.X86_REG_EBX, X.X86_REG_BX, X.X86_REG_BL, X.X86_REG_BH},
    X.X86_REG_ESI: {X.X86_REG_ESI, X.X86_REG_SI},
    X.X86_REG_EDI: {X.X86_REG_EDI, X.X86_REG_DI},
    X.X86_REG_EBP: {X.X86_REG_EBP, X.X86_REG_BP},
}
ECX_PARTS = {X.X86_REG_ECX, X.X86_REG_CX, X.X86_REG_CL, X.X86_REG_CH}

def native_member_call(audit, inventory, caller, callee, argc, body_sha256):
    """One direct site, exact DWORD pushes, closed graphs, saved incoming ECX.

    Existing Microsoft i386 callee-saved EBX/ESI/EDI/EBP is the only inter-call
    register contract used. No ECX identity is inferred merely from a register
    name or one nearby MOV. Every reaching definition is checked.
    """
    cn, ce, cp, raw = graph(audit, inventory, callee)
    require(hashlib.sha256(raw).hexdigest() == body_sha256, 'Accepted native body differs')
    returns = [i for i in cn.values() if i.group(capstone.CS_GRP_RET)]
    require(returns and all(len(i.operands) == 1 and i.operands[0].imm == argc*4 for i in returns),
            'Native cleanup disagrees with accepted arguments')
    nodes, edges, pred, _ = graph(audit, inventory, caller)
    sites = [i for i in nodes.values() if i.group(capstone.CS_GRP_CALL) and
             len(i.operands) == 1 and i.operands[0].type == X.X86_OP_IMM and
             i.operands[0].imm & 0xffffffff == callee]
    require(len(sites) == 1, 'Ambiguous direct call correspondence')
    call = sites[0].address
    require(len(pred[call]) == 1, 'Ambiguous receiver setup')
    setup = nodes[next(iter(pred[call]))]
    require(setup.address+setup.size == call and setup.mnemonic == 'mov' and
            len(setup.operands) == 2 and setup.operands[0].type == X.X86_OP_REG and
            setup.operands[0].reg == X.X86_REG_ECX and setup.operands[1].type == X.X86_OP_REG and
            setup.operands[1].reg in REGISTER_PARTS, 'Unknown saved receiver')
    saved = setup.operands[1].reg
    captures = [i for i in nodes.values() if i.mnemonic == 'mov' and len(i.operands) == 2 and
                i.operands[0].type == X.X86_OP_REG and i.operands[0].reg == saved and
                i.operands[1].type == X.X86_OP_REG and i.operands[1].reg == X.X86_REG_ECX]
    require(len(captures) == 1, 'Ambiguous incoming receiver capture')
    capture = captures[0].address
    at, visited = caller, set()
    while at != capture:
        require(at not in visited, 'Receiver capture preceded by cycle')
        visited.add(at); ins = nodes[at]
        require(not ins.group(capstone.CS_GRP_CALL) and not ins.group(capstone.CS_GRP_JUMP) and
                not ECX_PARTS.intersection(ins.regs_access()[1]) and len(edges[at]) == 1,
                'Incoming receiver changed before capture')
        at = edges[at][0]
    require(call not in reachable(edges, [caller], {capture}), 'Receiver capture bypassed')
    writes = [at for at, ins in nodes.items() if at != capture and
              REGISTER_PARTS[saved].intersection(ins.regs_access()[1])]
    require(all(call not in reachable(edges, edges[at], {capture}) for at in writes),
            'Saved receiver overwritten on reaching path')
    block, at = [], call
    while len(pred[at]) == 1:
        before = next(iter(pred[at])); ins = nodes[before]
        if ins.group(capstone.CS_GRP_CALL) or ins.group(capstone.CS_GRP_JUMP) or ins.group(capstone.CS_GRP_RET): break
        require(before+ins.size == at, 'Nonlinear call argument prefix')
        block.append(ins); at = before
        if at == caller: break
    pushes = [i for i in reversed(block) if i.mnemonic == 'push']
    require(len(pushes) == argc and all(len(i.operands) == 1 and i.operands[0].size == 4 for i in pushes),
            'Native/source argument count or width differs')
    require(all(i.mnemonic == 'push' or not {X.X86_REG_ESP, X.X86_REG_SP}.intersection(i.regs_access()[1])
                for i in block), 'Unmodeled argument stack modification')
    # Explicit writes through ESP would overwrite a supplied argument without
    # changing ESP itself. Decline all memory stores in this small prefix.
    require(all(not any(o.type == X.X86_OP_MEM and o.access & capstone.CS_AC_WRITE for o in i.operands)
                for i in block), 'Memory write in argument prefix')
    return dict(call=call, capture=capture, setup=setup.address,
                pushes=[i.address for i in pushes], returns=[i.address for i in returns])

SCALARS = {'bool', 'char', 'signed char', 'unsigned char', 'short', 'unsigned short',
           'int', 'unsigned int', 'long', 'unsigned long'}

def accepted_signature(source, symbol, seed):
    """Only ordinary public non-const double-returning member definitions.

    Exactly one trailing pointer must be unused in the accepted body. No
    templates, function pointers, references, records by value or hidden sret.
    """
    view = seed.code_view(source)
    matches = list(re.finditer(r'^double[ \t]+([A-Za-z_]\w*)::([A-Za-z_]\w*)[ \t]*\(([^()]*)\)\s*\{', view, re.M))
    require(len(matches) == 1, 'Unknown or ambiguous accepted member definition')
    m = matches[0]; owner, method = m[1], m[2]
    require(symbol.startswith('?' + method + '@' + owner + '@@QAEN'), 'Accepted symbol/definition ABI differs')
    raw = seed.split_args(m[3]); require(raw and all(raw), 'Empty accepted parameters')
    params, forwards, names = [], set(), []
    for param in raw:
        kind, name = seed.param_parts(param)
        require(re.fullmatch(r'[A-Za-z_]\w*', name or '') and name not in names, 'Unknown parameter name')
        names.append(name)
        kind = re.sub(r'\s+', ' ', kind.strip())
        if '*' in kind:
            pointer = re.fullmatch(r'(?:const )?([A-Za-z_]\w*)[ \t]*(\*+)', kind)
            require(pointer is not None, 'Unsupported pointer parameter')
            base, stars = pointer.groups()
            require(base not in {'float','double','void'} and base not in SCALARS, 'Unsupported scalar pointer view')
            forwards.add(base); kind = ('const ' if kind.startswith('const ') else '') + '::' + base + ' ' + stars
        else: require(kind in SCALARS, 'Unsupported parameter width or value layout')
        params.append(kind + ' ' + name)
    require('*' in seed.param_parts(params[-1])[0], 'Trailing argument is not a pointer')
    opening = m.end()-1; depth, closing = 1, opening+1
    while depth and closing < len(view):
        depth += (view[closing] == '{') - (view[closing] == '}'); closing += 1
    require(depth == 0 and not re.search(r'\b'+re.escape(names[-1])+r'\b', view[opening+1:closing-1]),
            'Trailing accepted pointer is not unused')
    return params, sorted(forwards)


def source_arguments(body, params, args, accepted_params, seed):
    """Preserve positional source expressions; refuse numeric FP conversions."""
    types = seed.variable_types(body, params)
    require(len(args) == len(accepted_params), 'Source/accepted argument count differs')
    for expression, parameter in zip(args, accepted_params):
        expected = seed.param_parts(parameter)[0]
        actual = seed.seed_expression_type(expression, types)
        if expected == 'bool':
            if actual == 'bool' or expression.strip() in ('0','1'): continue
            # A side-effect-free scalar equality still has C++ bool result.
            match = re.fullmatch(r"\((char|int|unsigned int)\)([A-Za-z_]\w*)\s*(==|!=)\s*('(?:\\.|[^'\\])+'|0)", expression.strip())
            require(match is not None and match[2] in types and
                    ('*' in types[match[2]] or types[match[2]] in seed.INTEGRAL32),
                    'Unknown boolean argument representation')
            continue
        require(actual is not None, 'Unknown source argument representation')
        if '*' in expected:
            require('*' in actual or actual in seed.INTEGRAL32, 'Non-DWORD pointer argument')
        else:
            require(actual in SCALARS or actual in seed.INTEGRAL32, 'Non-integral scalar argument')

_CONTEXTS = {}

def accepted_member_view(repository, inventory, caller, callee, body, root_params,
                         old_prototype, this_name, accepted, seed):
    """Decline unless the one omitted-unused-pointer pattern is fully evidenced.

    No global prototype replacement or new overload: only one positional call
    with a proved implicit root receiver is eligible. All catalog/source/target
    inputs are read from the same repository. Unknown evidence keeps old output.
    """
    old_ret, old_conv, old_params = old_prototype
    if not this_name or old_conv != '__thiscall' or old_ret.strip() not in ('float10','double'):
        return None
    name = 'FUN_%08x' % callee
    sites = list(seed.call_sites(body, name))
    if len(sites) != 1 or len(sites[0][2]) != len(old_params) or callee not in accepted:
        return None
    evidence_root = (repository/'private/original-target-20261020/inventory').resolve()
    directory = next((p for p in inventory.passes if (p/'decompiled'/('%08x.c'%caller)).is_file()), None)
    if directory is None or not directory.resolve().is_relative_to(evidence_root): return None
    try:
        key = repository.resolve()
        if key not in _CONTEXTS:
            import catalog_io
            from verify_manifest import Audit
            catalog = catalog_io.load(catalog_io.locate(repository), references=False)
            rows = defaultdict(list)
            for row in catalog.get('regions', []): rows[row['entry']].append(row)
            _CONTEXTS[key] = rows, Audit(repository, 'private/original-target-20261020/inputs/game.exe')
        rows, audit = _CONTEXTS[key]
        require(len(rows.get(callee, [])) == 1, 'Missing or ambiguous accepted callee')
        row = rows[callee][0]
        require(row.get('source') == accepted[callee] and row['source'].startswith('src/recovered/'),
                'Accepted source map differs from canonical catalog')
        path = repository/row['source']
        require(path.resolve().is_relative_to((repository/'src/recovered').resolve()), 'Noncanonical source path')
        source = path.read_text(encoding='utf-8')
        c_params, forwards = accepted_signature(source, row['symbol'], seed)
        require(len(c_params) == len(old_params) and len(c_params) > 1,
                'Not one omitted stack parameter')
        # Every old explicit parameter must itself occupy exactly one DWORD;
        # otherwise this could be a width/sret issue instead of a dropped arg.
        require(all(seed.param_parts(p)[0] in SCALARS | {'float','uint','undefined4'} or
                    '*' in seed.param_parts(p)[0] for p in old_params[1:]), 'Unknown old argument width')
        source_arguments(body, root_params, sites[0][2], c_params, seed)
        proof = native_member_call(audit, inventory, caller, callee, len(c_params), row['body_sha256'])
    except (AbiRefusal, OSError, KeyError, ValueError):
        return None
    return dict(prototype=('double', '__thiscall', ['void *this_']+c_params),
                forwards=['class '+name+';' for name in forwards], proof=proof,
                note='// Accepted callee %08x: native DWORD stack arguments and incoming this verified; trailing unused pointer omitted by Ghidra.' % callee)
