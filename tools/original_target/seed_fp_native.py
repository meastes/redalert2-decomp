"""Native pointer-effect proof primitives for original-target seed preparation.

The caller supplies an already hash-pinned original Audit and a frozen Inventory.
Every helper rechecks its current canonical source and native-body provenance.
This module performs no import-time file reads, script execution or proof runs.
Unknown instructions, extents, aliases and ABI/frame effects refuse explicitly,
including when Python assertions are disabled.
"""
from collections import defaultdict, deque
from collections.abc import Mapping
from pathlib import Path
import hashlib
import json
import capstone
X = capstone.x86_const

class NativeRefusal(ValueError):
    """The available native/source evidence does not establish the effect."""

def require(condition, reason):
    if not condition:
        raise NativeRefusal(reason)

def bounded_original(audit, inventory, entry):
    (_, row) = inventory.function(entry)
    require(row and row.get('contiguous') and (len(row.get('ranges', [])) == 1), 'Unknown contiguous function')
    span = row['ranges'][0]
    (lo, hi) = (int(span['start'], 16), int(span['end_inclusive'], 16) + 1)
    require(lo == entry and 0 < hi - lo <= 65536, 'Unsupported function extent')
    raw = audit.original(lo, hi - lo)
    require(raw == bytes.fromhex(span['bytes_hex']), 'Original inventory bytes differ')
    return raw

def graph(audit, inventory, entry):
    raw=bounded_original(audit,inventory,entry)
    lo,hi=entry,entry+len(raw)
    (nodes, edges, pred, occupied, pending) = ({}, {}, defaultdict(set), {}, [entry])
    while pending:
        at = pending.pop()
        if at in nodes:
            continue
        require(lo <= at < hi and len(nodes) < 8192, 'Function graph escaped')
        ins = next(audit.decoder.disasm(raw[at - lo:], at, 1), None)
        require(ins is not None and at + ins.size <= hi, 'Undecoded function instruction')
        require(all((b not in occupied or occupied[b] == at for b in range(at, at + ins.size))), 'Overlapping original decode')
        for b in range(at, at + ins.size):
            occupied[b] = at
        nodes[at] = ins
        require(ins.mnemonic not in ('retf', 'retfq', 'iret', 'iretd', 'int', 'int1', 'int3', 'ud2', 'hlt') and (not (ins.group(capstone.CS_GRP_CALL) and ins.mnemonic != 'call')), 'Unknown control transfer')
        if ins.group(capstone.CS_GRP_RET):
            require(ins.mnemonic == 'ret' and ins.bytes[0] in (194, 195), 'Non-DWORD return')
            targets = []
        elif ins.group(capstone.CS_GRP_JUMP):
            require(len(ins.operands) == 1 and ins.operands[0].type == X.X86_OP_IMM, 'Unknown indirect branch')
            require(ins.mnemonic == 'jmp' or ins.mnemonic.startswith('j'), 'Unmodeled loop branch')
            targets = [ins.operands[0].imm & 4294967295]
            if ins.mnemonic != 'jmp':
                targets.append(at + ins.size)
        else:
            targets = [at + ins.size]
        edges[at] = targets
        for target in targets:
            pred[target].add(at)
        pending.extend(targets)
    return (nodes, edges, pred, raw)

def ptr(space, offset=0):
    return ('pointer', space, offset)

def pointer(value):
    return isinstance(value, tuple) and value[:1] == ('pointer',)

def plus(value, amount):
    return ptr(value[1], value[2] + amount) if pointer(value) else value + amount if isinstance(value, int) else ('fragment',) if isinstance(value,tuple) else None
EMPTY = frozenset()
SAVED = [X.X86_REG_EBX, X.X86_REG_EBP, X.X86_REG_ESI, X.X86_REG_EDI]
PARTS = {X.X86_REG_AX: X.X86_REG_EAX, X.X86_REG_AL: X.X86_REG_EAX, X.X86_REG_AH: X.X86_REG_EAX, X.X86_REG_CX: X.X86_REG_ECX, X.X86_REG_CL: X.X86_REG_ECX, X.X86_REG_CH: X.X86_REG_ECX, X.X86_REG_DX: X.X86_REG_EDX, X.X86_REG_DL: X.X86_REG_EDX, X.X86_REG_DH: X.X86_REG_EDX, X.X86_REG_BX: X.X86_REG_EBX, X.X86_REG_BL: X.X86_REG_EBX, X.X86_REG_BH: X.X86_REG_EBX, X.X86_REG_SI: X.X86_REG_ESI, X.X86_REG_DI: X.X86_REG_EDI, X.X86_REG_BP: X.X86_REG_EBP}

def taint(space, offset=0):
    return frozenset({(space, offset)})

def shifted(tags, delta):
    return frozenset(((space, None if offset is None else offset + delta) for (space, offset) in tags))

def frame_tags(tags):
    return frozenset((t for t in tags if t[0] in ('input', 'local', 'ecx') or t[0].startswith('saved_')))

def undefined_offset(tags):
    return frozenset(((space, None) for (space, offset) in tags))

def combine(states):
    regs = {}
    memory = {}
    for (a, b) in states:
        for (reg, t) in a.items():
            regs[reg] = regs.get(reg, EMPTY) | t
        for (off, t) in b.items():
            memory[off] = memory.get(off, EMPTY) | t
    return (regs, memory)

class NativeProofs:
    """Explicit proof environment; no global repository or mutable module state."""

    def __init__(self, repository, inventory, audit, accepted_rows):
        self.repository = Path(repository).resolve()
        self.inventory = inventory
        self.audit = audit
        self.ambiguous_entries = set()
        if isinstance(accepted_rows, Mapping):
            self.rows = dict(accepted_rows)
        else:
            grouped = defaultdict(list)
            for row in accepted_rows:
                grouped[row['entry']].append(row)
            self.ambiguous_entries = {entry for entry, rows in grouped.items() if len(rows) != 1}
            self.rows = {entry: rows[0] for (entry, rows) in grouped.items() if len(rows) == 1}

    def _source_bytes(self, source, expected_hash):
        require(isinstance(source, str) and source.startswith('src/recovered/'), 'Noncanonical helper source')
        path = (self.repository / source).resolve()
        require(path.is_relative_to((self.repository / 'src/recovered').resolve()), 'Helper source escaped recovered tree')
        try:
            data = path.read_bytes()
        except OSError as error:
            raise NativeRefusal('Accepted helper source is unavailable') from error
        require(hashlib.sha256(data).hexdigest() == expected_hash, 'Accepted helper source hash differs')
        return data

    def provenance(self, entry):
        require(entry not in self.ambiguous_entries, 'Ambiguous accepted helper entry')
        if entry in self.rows:
            row = self.rows[entry]
            require(isinstance(row, Mapping) and {'source', 'source_sha256', 'body_bytes', 'body_sha256'} <= row.keys(), 'Incomplete accepted helper metadata')
            self._source_bytes(row['source'], row['source_sha256'])
            raw=bounded_original(self.audit,self.inventory,entry)
            require(len(raw)==row['body_bytes'] and hashlib.sha256(raw).hexdigest()==row['body_sha256'], 'Accepted helper extent differs from complete inventory body')
            require(hashlib.sha256(self.audit.original(entry, row['body_bytes'])).hexdigest() == row['body_sha256'], 'Accepted helper native body hash differs')
            return row['body_sha256']
        ledger_path = self.repository / 'manifests/original_target/assembly.json'
        require(ledger_path.is_file(), 'Missing accepted helper provenance')
        ledger = json.loads(ledger_path.read_text())
        rows = [row for row in ledger['regions'] if int(row['entry'], 16) == entry]
        require(len(rows) == 1 and rows[0]['symbol'] == '__ftol', 'Missing or ambiguous compiler conversion helper')
        row = rows[0]
        self._source_bytes(row['source'], row['source_sha256'])
        raw=bounded_original(self.audit,self.inventory,entry)
        require(len(raw)==row['bytes'] and hashlib.sha256(raw).hexdigest()==row['sha256'], 'Assembly helper extent differs from complete inventory body')
        require(hashlib.sha256(self.audit.original(entry, row['bytes'])).hexdigest() == row['sha256'], 'Accepted assembly helper native body hash differs')
        return row['sha256']

    def linear_effects(self,entry,registers,arguments):
        """Infer exact native writes/read footprints; no record-size/role guess.

        All actual frame-bearing incoming registers/stack words must be supplied
        as distinct symbolic pointer roots by the caller. The native body must
        be straight-line, call-free and restore stack/nonvolatile registers.
        """
        return self.output_contract(entry,registers,arguments,None)

    def output_contract(self, entry, registers, arguments, extent):
        v = self.repository
        rows = self.rows
        audit = self.audit
        inv = self.inventory
        self.provenance(entry)
        inferred=extent is None
        if not (inferred or (0 < extent <= 4096 and extent % 4 == 0)):
            raise NativeRefusal('Unsupported output extent')
        row = rows[entry]
        if not hashlib.sha256((v / row['source']).read_bytes()).hexdigest() == row['source_sha256']:
            raise NativeRefusal('Unsupported native evidence')
        if not hashlib.sha256(audit.original(entry, row['body_bytes'])).hexdigest() == row['body_sha256']:
            raise NativeRefusal('Unsupported native evidence')
        (nodes, edges, pred, raw) = graph(audit, inv, entry)
        ordered = sorted(nodes.values(), key=lambda i: i.address)
        if not (ordered[0].address == entry and all((not i.group(capstone.CS_GRP_CALL) and (not i.group(capstone.CS_GRP_JUMP)) for i in ordered))):
            raise NativeRefusal('Unsupported native evidence')
        if not (all((edges[i.address] == [ordered[n + 1].address] for (n, i) in enumerate(ordered[:-1]))) and ordered[-1].mnemonic == 'ret'):
            raise NativeRefusal('Unsupported native evidence')
        saved = {reg: ('saved', reg) for reg in [X.X86_REG_EBX, X.X86_REG_EBP, X.X86_REG_ESI, X.X86_REG_EDI]}
        regs = {**saved, X.X86_REG_ESP: ptr('stack'), **registers}
        saved={reg:regs[reg] for reg in saved}
        roots={value[1] for value in [*registers.values(),*arguments.values()] if pointer(value)}
        require(not inferred or not roots.intersection({'stack','image'}),'Reserved inferred pointer space')
        native_pop=ordered[-1].operands[0].imm if ordered[-1].operands else 0
        reads=[]
        memory = {offset+b: ((value >> (8*b)) & 255) if isinstance(value,int) else value for offset,value in arguments.items() for b in range(4)}
        writes = []

        def address(o):
            if not o.type == X.X86_OP_MEM:
                raise NativeRefusal('Unsupported native evidence')
            if not o.mem.segment in (0, X.X86_REG_DS, X.X86_REG_SS):
                raise NativeRefusal('Unsupported native evidence')
            if o.mem.base == 0 and o.mem.index == 0:
                return ptr('image',o.mem.disp)
            base = regs.get(o.mem.base)
            if o.mem.index:
                index = regs.get(o.mem.index)
                if isinstance(index, int):
                    return plus(base, index * o.mem.scale + o.mem.disp)
                if pointer(index) and o.mem.scale == 1 and isinstance(base, int):
                    return plus(index, base + o.mem.disp)
                require(not inferred or not (pointer(base) or pointer(index)), 'Unbounded symbolic input index')
                return None
            return plus(base, o.mem.disp)

        def stack_read(at, size):
            if inferred:
                require((regs[X.X86_REG_ESP][2]<=at and at+size<=0) or (4<=at and at+size<=4+native_pop),'Read outside private stack/native argument extent')
            values = [memory.get(at+b) for b in range(size)]
            tokens = [value for value in values if isinstance(value, tuple)]
            if not tokens:
                return int.from_bytes(bytes(values),'little') if values and all(isinstance(value,int) for value in values) else None
            return tokens[0] if size == 4 and all(value == tokens[0] for value in values) else ('fragment',)

        def read(o):
            if o.type == X.X86_OP_REG:
                value = regs.get(PARTS.get(o.reg,o.reg))
                return (('fragment',) if isinstance(value,tuple) else None) if o.size != 4 else value
            if o.type == X.X86_OP_IMM:
                return o.imm
            if o.type == X.X86_OP_MEM:
                at = address(o)
                if pointer(at) and at[1]=='stack':return stack_read(at[2],o.size)
                if inferred and pointer(at) and at[1] in roots:
                    require(0<=at[2] and at[2]+o.size<=4096,'Unbounded inferred input read')
                    reads.append(dict(address='%08x'%ins.address,space=at[1],offset=at[2],size=o.size))
                return None
            raise NativeRefusal('Unknown operand')

        def store(at, size, value, ins):
            if not pointer(at):
                raise NativeRefusal(('unbounded write', hex(ins.address), ins.op_str, at))
            if at[1] == 'stack':
                if not (regs[X.X86_REG_ESP][2] <= at[2] and at[2] + size <= 0):
                    raise NativeRefusal(('outside private helper stack', hex(ins.address), at, size))
                for byte in range(size):memory[at[2]+byte] = ((value >> (8*byte)) & 255) if isinstance(value,int) else value
            else:
                if not ((at[1] in roots if inferred else at[1]=='out') and 0<=at[2] and at[2]+size<=(4096 if inferred else extent)):
                    raise NativeRefusal(('outside output', hex(ins.address), at, size))
                if not not isinstance(value, tuple):
                    raise NativeRefusal(('Pointer/saved-register token leaked into output data', hex(ins.address), value))
                event=dict(address='%08x' % ins.address, offset=at[2], size=size)
                if inferred:event['space']=at[1]
                writes.append(event)
        for ins in ordered:
            op = ins.operands
            m = ins.mnemonic
            if m == 'push':
                if not op[0].size == 4:
                    raise NativeRefusal('Unsupported native evidence')
                value=read(op[0])
                regs[X.X86_REG_ESP] = plus(regs[X.X86_REG_ESP], -4)
                for byte in range(4):memory[regs[X.X86_REG_ESP][2]+byte] = ((value >> (8*byte)) & 255) if isinstance(value,int) else value
            elif m == 'pop':
                if not (op[0].type == X.X86_OP_REG and op[0].reg != X.X86_REG_ESP):
                    raise NativeRefusal('Unsupported native evidence')
                regs[op[0].reg] = stack_read(regs[X.X86_REG_ESP][2],4)
                regs[X.X86_REG_ESP] = plus(regs[X.X86_REG_ESP], 4)
            elif m == 'mov':
                value = read(op[1])
                if op[0].type == X.X86_OP_REG:
                    if not (op[0].size == 4 and op[0].reg != X.X86_REG_ESP):
                        raise NativeRefusal('Unsupported native evidence')
                    regs[op[0].reg] = value
                else:
                    store(address(op[0]), op[0].size, value, ins)
            elif m == 'lea':
                if not op[0].reg != X.X86_REG_ESP:
                    raise NativeRefusal('Unsupported native evidence')
                regs[op[0].reg] = address(op[1])
            elif m in ('add', 'sub'):
                if not (op[0].type == X.X86_OP_REG and op[1].type == X.X86_OP_IMM):
                    raise NativeRefusal('Unsupported native evidence')
                regs[op[0].reg] = plus(regs.get(op[0].reg), op[1].imm * (1 if m == 'add' else -1))
            elif m == 'imul':
                if not (op[0].type == X.X86_OP_REG and (not any(isinstance(read(o),tuple) for o in op if o.access & capstone.CS_AC_READ))):
                    raise NativeRefusal('Unsupported native evidence')
                regs[op[0].reg] = None
            elif m in ('shl', 'shr', 'sar'):
                if not (op[0].type == X.X86_OP_REG and op[0].reg != X.X86_REG_ESP and (not isinstance(regs.get(op[0].reg),tuple))):
                    raise NativeRefusal('Unsupported native evidence')
                regs[op[0].reg] = None
            elif m == 'rep movsd':
                count = regs[X.X86_REG_ECX]
                if not (isinstance(count, int) and 0 < count <= 1024):
                    raise NativeRefusal('Unsupported native evidence')
                source = regs.get(X.X86_REG_ESI)
                dest = regs.get(X.X86_REG_EDI)
                for k in range(count):
                    source_at=plus(source,4*k)
                    if inferred and pointer(source_at) and source_at[1] in roots:
                        require(0<=source_at[2] and source_at[2]+4<=4096,'Unbounded inferred REP input')
                        reads.append(dict(address='%08x'%ins.address,space=source_at[1],offset=source_at[2],size=4))
                    require(not inferred or pointer(source_at) or not pointer(source),'Unbounded inferred REP source')
                    store(plus(dest,4*k),4,stack_read(source_at[2],4) if pointer(source_at) and source_at[1]=='stack' else None,ins)
                regs[X.X86_REG_ESI] = plus(source, count * 4)
                regs[X.X86_REG_EDI] = plus(dest, count * 4)
                regs[X.X86_REG_ECX] = 0
            elif m in ('fld', 'fmul', 'fadd', 'faddp', 'fmulp', 'fsub', 'fsubp', 'fsubr', 'fsubrp', 'fdiv', 'fdivp', 'fdivr', 'fdivrp', 'fchs'):
                require(not any(o.type == X.X86_OP_MEM and isinstance(read(o),tuple) for o in op), 'Pointer bits consumed by floating operation')
                if not not {X.X86_REG_EAX, X.X86_REG_ECX, X.X86_REG_EDX, X.X86_REG_ESP, X.X86_REG_EBX, X.X86_REG_ESI, X.X86_REG_EDI, X.X86_REG_EBP}.intersection(ins.regs_access()[1]):
                    raise NativeRefusal('Unsupported native evidence')
            elif m in ('fstp', 'fst'):
                if op[0].type == X.X86_OP_MEM:
                    store(address(op[0]), op[0].size, None, ins)
            elif m == 'ret':
                if not (regs[X.X86_REG_ESP] == ptr('stack') and (inferred or regs.get(X.X86_REG_EAX) == ptr('out')) and all((regs.get(reg) == value for (reg, value) in saved.items()))):
                    raise NativeRefusal('Unsupported native evidence')
            else:
                raise NativeRefusal((hex(ins.address), m, ins.op_str))
        if inferred:
            require(writes,'No bounded numeric writes discovered')
            return dict(entry='%08x'%entry,source=row['source'],source_sha256=row['source_sha256'],body_sha256=row['body_sha256'],nodes=len(nodes),writes=writes,reads=reads,end_register_values={str(reg):regs.get(reg) for reg in [X.X86_REG_EAX,X.X86_REG_ECX,X.X86_REG_EDX]},callee_saved_and_stack_restored=True,pop=native_pop,roles_or_record_sizes_guessed=False)
        if not set((b for w in writes for b in range(w['offset'], w['offset'] + w['size']))) == set(range(extent)):
            raise NativeRefusal('Unsupported native evidence')
        return dict(entry='%08x' % entry, source=row['source'], source_sha256=row['source_sha256'], body_sha256=row['body_sha256'], nodes=len(nodes), output_extent=extent, writes=writes, return_alias='out+0', end_register_values={str(reg): regs.get(reg) for reg in [X.X86_REG_EAX, X.X86_REG_ECX, X.X86_REG_EDX]}, callee_saved_and_stack_restored=True, pop=ordered[-1].operands[0].imm if ordered[-1].operands else 0)

    def readonly_contract(self, entry, stack_input=False, ecx_symbol=False, nested=None, entry_mod8=0, infer_reads=False):
        audit = self.audit
        inv = self.inventory
        digest = self.provenance(entry)
        (nodes, edges, pred, raw) = graph(audit, inv, entry)
        nested = nested or {}
        calls = {}
        for (at, i) in nodes.items():
            if i.mnemonic == 'call':
                if not i.operands[0].type == X.X86_OP_IMM:
                    raise NativeRefusal('Unsupported native evidence')
                target = i.operands[0].imm
                if not target in nested:
                    raise NativeRefusal('Unsupported native evidence')
                calls[at] = nested[target]
        indegree = {at: len(pred[at]) for at in nodes}
        ready = deque((at for (at, d) in indegree.items() if d == 0))
        order = []
        while ready:
            at = ready.popleft()
            order.append(at)
            for target in edges[at]:
                indegree[target] -= 1
                if indegree[target] == 0:
                    ready.append(target)
        if not (len(order) == len(nodes) and order[0] == entry):
            raise NativeRefusal('Unknown cyclic helper graph')
        anchor = None
        capture = [i for i in nodes.values() if i.mnemonic == 'mov' and i.op_str == 'ebp, esp']
        if capture:
            if not len(capture) == 1:
                raise NativeRefusal('Unsupported native evidence')
            at = entry
            level = 0
            while at != capture[0].address:
                i = nodes[at]
                if not (not i.group(capstone.CS_GRP_CALL) and (not i.group(capstone.CS_GRP_JUMP)) and (len(edges[at]) == 1)):
                    raise NativeRefusal('Unsupported native evidence')
                if i.mnemonic == 'push':
                    level -= 4
                elif not not {X.X86_REG_ESP, X.X86_REG_EBP}.intersection(i.regs_access()[1]):
                    raise NativeRefusal('Unsupported native evidence')
                at = edges[at][0]
            anchor = level
            if not all((i.address == capture[0].address or i.mnemonic == 'pop' for i in nodes.values() if X.X86_REG_EBP in i.regs_access()[1])):
                raise NativeRefusal('Unknown frame-pointer reassignment')
        heights = {entry: 0}

        def after_height(at):
            i = nodes[at]
            op = i.operands
            h = heights[at]
            if i.mnemonic == 'push':
                if not op[0].size == 4:
                    raise NativeRefusal('Unsupported native evidence')
                return h - 4
            if i.mnemonic == 'pop':
                if not op[0].reg != X.X86_REG_ESP:
                    raise NativeRefusal('Unsupported native evidence')
                return h + 4
            if i.mnemonic in ('add', 'sub') and op[0].type == X.X86_OP_REG and (op[0].reg == X.X86_REG_ESP):
                if not op[1].type == X.X86_OP_IMM:
                    raise NativeRefusal('Unsupported native evidence')
                return h + op[1].imm * (1 if i.mnemonic == 'add' else -1)
            if i.mnemonic == 'call':
                return h + calls[at]['pop']
            if i.mnemonic == 'and' and op[0].type == X.X86_OP_REG and (op[0].reg == X.X86_REG_ESP):
                if not op[1].imm & 4294967295 == 4294967288:
                    raise NativeRefusal('Unsupported native evidence')
                return h - (entry_mod8 + h) % 8
            if i.mnemonic == 'mov' and i.op_str == 'esp, ebp':
                if not anchor is not None:
                    raise NativeRefusal('Unsupported native evidence')
                return anchor
            if not (not {X.X86_REG_ESP, X.X86_REG_SP}.intersection(i.regs_access()[1]) or i.mnemonic == 'ret'):
                raise NativeRefusal((hex(at), i.mnemonic, i.op_str))
            return h
        for at in order:
            h = after_height(at)
            for target in edges[at]:
                if target in heights:
                    if not heights[target] == h:
                        raise NativeRefusal('Conflicting helper frame heights')
                else:
                    heights[target] = h
        initial_regs = {reg: taint('saved_%d' % reg) for reg in SAVED}
        if ecx_symbol:
            initial_regs[X.X86_REG_ECX] = taint('ecx')
        pops = {i.operands[0].imm if i.operands else 0 for i in nodes.values() if i.mnemonic == 'ret'}
        if not len(pops) == 1:
            raise NativeRefusal('Unsupported native evidence')
        owned_argument_bytes = next(iter(pops))
        initial_mem = {4 + b: taint('input') for b in range(4)} if stack_input else {}
        outputs = {}
        returns = []
        writes = []
        nested_calls = []
        input_reads=[]
        for at in order:
            (regs, memory) = ({**initial_regs}, {**initial_mem}) if at == entry else combine([outputs[p] for p in pred[at]])
            i = nodes[at]
            op = i.operands
            m = i.mnemonic
            h = heights[at]

            def reg_value(reg):
                return taint('local', h) if reg == X.X86_REG_ESP else regs.get(PARTS.get(reg, reg), EMPTY)

            def address(o):
                if not (o.type == X.X86_OP_MEM and o.mem.segment in (0, X.X86_REG_DS, X.X86_REG_SS)):
                    raise NativeRefusal('Unsupported native evidence')
                if o.mem.base == 0 and o.mem.index == 0:
                    return ('image', o.mem.disp)
                base = reg_value(o.mem.base)
                if o.mem.index:
                    return ('tainted', undefined_offset(base | reg_value(o.mem.index)))
                tags = shifted(base, o.mem.disp)
                if len(tags) == 1:
                    (space, offset) = next(iter(tags))
                    if space == 'local' and offset is not None:
                        return ('stack', offset)
                return ('tainted', tags)

            def observe_numeric(tags,size,address_):
                for space,offset in frame_tags(tags):
                    require(offset is not None,'Unbounded transitive readonly input')
                    if space=='local':
                        require((h<=offset and offset+size<=0) or (4<=offset and offset+size<=4+owned_argument_bytes),'Nested read outside private/owned argument stack')
                        payload=frozenset().union(*(memory.get(offset+b,EMPTY) for b in range(size)))
                        require(not frame_tags(payload),'Pointer bits in nested numeric input')
                    else:
                        require(0<=offset and offset+size<=4096,'Unbounded readonly input footprint')
                        input_reads.append(dict(address='%08x'%address_,space=space,offset=offset,size=size))

            def read(o):
                if o.type == X.X86_OP_REG:
                    value=reg_value(o.reg)
                    return value if o.size==4 else undefined_offset(value)
                if o.type == X.X86_OP_IMM:
                    return EMPTY
                if o.type == X.X86_OP_MEM:
                    (kind, value) = address(o)
                    if kind=='stack':
                        if infer_reads:require((h<=value and value+o.size<=0) or (4<=value and value+o.size<=4+owned_argument_bytes),'Read outside private/owned readonly stack')
                        tags=frozenset().union(*(memory.get(value+b,EMPTY) for b in range(o.size)))
                        return tags if o.size==4 else undefined_offset(tags)
                    if infer_reads and kind=='tainted':
                        require(not any(space=='local' for space,offset in value),'Mixed/unknown local read address')
                        observe_numeric(value,o.size,at)
                    return EMPTY
                raise NativeRefusal('Unknown operand')

            def write(o, value):
                if o.type == X.X86_OP_REG:
                    reg = PARTS.get(o.reg, o.reg)
                    if not reg != X.X86_REG_ESP:
                        raise NativeRefusal('Unsupported native evidence')
                    regs[reg] = value if o.size == 4 else undefined_offset(regs.get(reg, EMPTY) | value)
                else:
                    (kind, where) = address(o)
                    if kind == 'stack':
                        private = h <= where and where + o.size <= 0
                        argument = 4 <= where and where + o.size <= 4 + owned_argument_bytes
                        if not (private or argument):
                            raise NativeRefusal(('write outside private/owned argument stack', hex(at), where, o.size))
                        for word in range(where // 4 * 4, (where + o.size - 1) // 4 * 4 + 1, 4):
                            if where <= word and word + 4 <= where + o.size:
                                continue
                            for byte in range(word, word + 4):
                                if not where <= byte < where + o.size:
                                    memory[byte] = undefined_offset(memory.get(byte, EMPTY))
                        for byte in range(o.size):
                            memory[where + byte] = value
                        writes.append((at, 'stack' if private else 'owned_argument', where, o.size))
                    elif kind == 'image':
                        if not not frame_tags(value):
                            raise NativeRefusal('Frame pointer published to image')
                        writes.append((at, 'image', where, o.size))
                    else:
                        raise NativeRefusal(('Unknown/input-pointer write', hex(at), i.op_str, where))
            if m == 'mov' and i.op_str == 'esp, ebp':
                pass
            elif m == 'and' and op[0].type == X.X86_OP_REG and (op[0].reg == X.X86_REG_ESP):
                pass
            elif m == 'mov':
                write(op[0], read(op[1]))
            elif m in ('movsx', 'movzx'):
                write(op[0], undefined_offset(read(op[1])))
            elif m == 'lea':
                (kind, value) = address(op[1])
                write(op[0], taint('local', value) if kind == 'stack' else EMPTY if kind == 'image' else value)
            elif m == 'push':
                value = read(op[0])
                for byte in range(4):
                    memory[h - 4 + byte] = value
            elif m == 'pop':
                write(op[0], frozenset().union(*(memory.get(h + b, EMPTY) for b in range(4))))
            elif m in ('add', 'sub') and op[0].type == X.X86_OP_REG and (op[0].reg == X.X86_REG_ESP):
                pass
            elif m in ('add', 'sub') and op[0].type == X.X86_OP_REG and (op[1].type == X.X86_OP_IMM):
                write(op[0], shifted(read(op[0]), op[1].imm * (1 if m == 'add' else -1)))
            elif m in ('add', 'sub', 'xor', 'and', 'or', 'shl', 'shr', 'sar', 'imul', 'neg', 'inc', 'dec', 'sbb', 'adc', 'cdq', 'cwde'):
                if m == 'cdq':
                    regs[X.X86_REG_EDX] = undefined_offset(reg_value(X.X86_REG_EAX))
                elif m == 'cwde':
                    regs[X.X86_REG_EAX] = undefined_offset(reg_value(X.X86_REG_EAX))
                elif m == 'xor' and len(op) == 2 and (op[0].type == op[1].type == X.X86_OP_REG) and (op[0].reg == op[1].reg):
                    write(op[0], EMPTY)
                else:
                    if not op[0].type == X.X86_OP_REG:
                        raise NativeRefusal('Unsupported native evidence')
                    value = EMPTY
                    for operand in op:
                        value |= read(operand)
                    write(op[0], undefined_offset(value))
            elif m == 'call':
                contract = calls[at]
                arg = frozenset().union(*(memory.get(h + b, EMPTY) for b in range(4))) if contract['stack_input'] else EMPTY
                if contract['stack_input']:
                    if not not frame_tags(reg_value(X.X86_REG_ECX)):
                        raise NativeRefusal('Unexpected frame receiver')

                def bind(tags):
                    out = EMPTY
                    for (space, offset) in tags:
                        if space == 'input':
                            out |= shifted(arg, offset) if offset is not None else undefined_offset(arg)
                        elif space == 'local':
                            out |= taint('local', h - 4 + offset) if offset is not None else taint('local', None)
                        elif space == 'ecx':
                            out |= shifted(reg_value(X.X86_REG_ECX), offset) if offset is not None else undefined_offset(reg_value(X.X86_REG_ECX))
                        elif space=='unknown_scalar':
                            out|=taint('unknown_scalar')
                        elif space.startswith('saved_'):
                            out |= shifted(reg_value(int(space[6:])), offset) if offset is not None else undefined_offset(reg_value(int(space[6:])))
                        else:
                            raise NativeRefusal('Unknown nested alias')
                    return out
                if infer_reads:
                    require('reads' in contract,'Nested readonly footprint is unavailable')
                    from seed_fp_effects import prove_killed_inputs
                    prove_killed_inputs(self,int(contract['entry'],16),{reg for reg in (X.X86_REG_EAX,X.X86_REG_EDX) if frame_tags(reg_value(reg))})
                    for footprint in contract['reads']:
                        observe_numeric(bind(taint(footprint['space'],footprint['offset'])),footprint['size'],at)
                    for write_at,kind,offset,size in contract['writes']:
                        if kind!='owned_argument':continue
                        where=h-4+offset
                        private=h<=where and where+size<=0
                        argument=4<=where and where+size<=4+owned_argument_bytes
                        require(private or argument,'Nested argument write escapes parent stack ownership')
                        for byte in range(size):memory[where+byte]=memory.get(where+byte,EMPTY)|taint('unknown_scalar')
                        writes.append((at,'stack' if private else 'owned_argument',where,size))
                new = {reg: bind(tags) for (reg, tags) in contract['_endregs'].items()}
                regs.update(new)
                nested_calls.append(dict(address='%08x' % at, callee=contract['entry'], input_aliases=sorted(arg)))
            elif m in ('fst', 'fstp', 'fistp', 'fnstcw', 'fnstsw'):
                if op[0].type == X.X86_OP_MEM:
                    write(op[0], EMPTY)
                else:
                    write(op[0], EMPTY)
            elif m in ('fld', 'fild', 'fldcw', 'fwait', 'wait', 'fcom', 'fcomp', 'fcompp', 'fadd', 'faddp', 'fmul', 'fmulp', 'fsub', 'fsubp', 'fsubr', 'fsubrp', 'fdiv', 'fdivp', 'fdivr', 'fdivrp', 'fchs', 'fcomi', 'fucom', 'fucomp', 'ftst'):
                require(not any(o.type == X.X86_OP_MEM and frame_tags(read(o)) for o in op), 'Pointer bits consumed by floating operation')
                if not not {X.X86_REG_EAX, X.X86_REG_ECX, X.X86_REG_EDX, X.X86_REG_ESP, X.X86_REG_EBX, X.X86_REG_ESI, X.X86_REG_EDI, X.X86_REG_EBP}.intersection(i.regs_access()[1]):
                    raise NativeRefusal('Unsupported native evidence')
            elif m in ('cmp', 'test', 'nop') or i.group(capstone.CS_GRP_JUMP):
                if infer_reads and m in ('cmp','test'):
                    for operand in op:
                        if operand.type==X.X86_OP_MEM:read(operand)
            elif m.startswith('set'):
                write(op[0], EMPTY)
            elif m == 'ret':
                if not (h == 0 and all((reg_value(reg) == taint('saved_%d' % reg) for reg in SAVED))):
                    raise NativeRefusal('Unrestored helper frame/nonvolatile')
                if not not frame_tags(reg_value(X.X86_REG_EAX)):
                    raise NativeRefusal('Input/local pointer returned')
                if not all((frame_tags(memory.get(byte,EMPTY)) <= frame_tags(initial_mem.get(byte,EMPTY)) for byte in range(4, 4 + owned_argument_bytes))):
                    raise NativeRefusal('New alias written into an owned argument slot')
                returns.append((at, op[0].imm if op else 0, {reg: reg_value(reg) for reg in [X.X86_REG_EAX, X.X86_REG_ECX, X.X86_REG_EDX]}))
            else:
                raise NativeRefusal((hex(at), m, i.op_str))
            outputs[at] = (regs, memory)
        if not (returns and len({pop for (at, pop, regs) in returns}) == 1):
            raise NativeRefusal('Unsupported native evidence')
        end = {reg: EMPTY for reg in [X.X86_REG_EAX, X.X86_REG_ECX, X.X86_REG_EDX]}
        for (at, pop, regs) in returns:
            for (reg, tags) in regs.items():
                end[reg] |= tags
        result=dict(entry='%08x' % entry, entry_esp_mod8=entry_mod8, body_sha256=digest, nodes=len(nodes), edges=sum(map(len, edges.values())), stack_input=stack_input, argument_aliases_only_removed=True, pop=returns[0][1], writes=writes, nested_calls=nested_calls, return_pointer_aliases=sorted(end[X.X86_REG_EAX]), _endregs=end)
        if infer_reads:result['reads']=input_reads
        return result

    def all_alignments(self, entry, **kwargs):
        variants = [self.readonly_contract(entry, entry_mod8=mod, **kwargs) for mod in (0, 4)]
        if not variants[0]['pop'] == variants[1]['pop']:
            raise NativeRefusal('Unsupported native evidence')
        result = variants[0]
        result['verified_entry_alignments'] = [0, 4]
        if kwargs.get('infer_reads'):
            result['reads']=[dict(items) for items in sorted({tuple(sorted(read.items())) for variant in variants for read in variant['reads']})]
            result['writes']=sorted({tuple(write) for variant in variants for write in variant['writes']})
        for (reg, tags) in variants[1]['_endregs'].items():
            result['_endregs'][reg] |= tags
        return result
