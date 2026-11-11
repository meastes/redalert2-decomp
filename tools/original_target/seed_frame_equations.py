"""Affine native ESP equations, retaining unresolved cleanup variables.

Known call cleanups must be independently evidenced by the caller. Unknown
normal-call cleanups remain symbolic: only uniquely established heights or
relative stack addresses may be consumed. No call memory effects are implied.
"""
from fractions import Fraction
from seed_guarded_cfg import X,require,CFGRefusal

def zero_cleanup_columns(rows):
    """Find zero variables implied by nonnegative RET immediate widths.

    A zero right-hand side and same-sign coefficients force every variable
    in that row to zero. Removing those variables may expose another row.
    Mixed-sign or nonzero equations are otherwise left to affine reduction.
    """
    fixed = set()
    while True:
        before = len(fixed)
        for _, row in rows:
            if row[-1] != 0:
                continue
            active = {
                column: value for column, value in enumerate(row[:-1])
                if value and column not in fixed
            }
            if all(value > 0 for value in active.values()) or all(
                value < 0 for value in active.values()
            ):
                fixed.update(active)
        if len(fixed) == before:
            return fixed


class AffineFrames:
    def __init__(self,graph,known_pops):
        self.graph=graph;nodes,edges=graph['nodes'],graph['edges'];entry=graph['entry']
        calls={at for at,ins in nodes.items() if ins.mnemonic=='call'}
        require(set(known_pops)<=calls,'Cleanup supplied for a non-call')
        require(all(isinstance(value,int) and 0<=value<=4096 and value%4==0 for value in known_pops.values()),'Unknown/non-DWORD known cleanup')
        self.variables=sorted(calls-set(known_pops));columns={at:index for index,at in enumerate(self.variables)};n=len(columns)
        self.states={entry:(0,(0,)*n)};pending=[entry];equations=[]
        while pending:
            at=pending.pop();ins=nodes[at];ops=ins.operands;c,original=self.states[at];coeff=list(original)
            if ins.mnemonic=='push':
                require(len(ops)==1 and ops[0].size==4,'Unknown PUSH width');c-=4
            elif ins.mnemonic=='pop':
                require(len(ops)==1 and ops[0].size==4 and not (ops[0].type==X.X86_OP_REG and ops[0].reg==X.X86_REG_ESP),'Unknown POP effect');c+=4
            elif ins.mnemonic in ('add','sub') and ops[0].type==X.X86_OP_REG and ops[0].reg==X.X86_REG_ESP:
                require(len(ops)==2 and ops[0].size==4 and ops[1].type==X.X86_OP_IMM,'Dynamic stack adjustment');c+=ops[1].imm*(1 if ins.mnemonic=='add' else -1)
            elif ins.mnemonic=='call':
                if at in known_pops:c+=known_pops[at]
                else:coeff[columns[at]]+=1
            elif ins.mnemonic=='ret':equations.append((c,tuple(coeff)))
            else:require(not {X.X86_REG_ESP,X.X86_REG_SP}.intersection(ins.regs_access()[1]),'Unmodeled native ESP effect')
            for target in edges[at]:
                if target in self.states:
                    other_c,other_coeff=self.states[target];equations.append((c-other_c,tuple(value-other for value,other in zip(coeff,other_coeff))))
                else:self.states[target]=(c,tuple(coeff));pending.append(target)
        require(len(self.states)==len(nodes),'Incomplete native stack reachability')
        matrix=[[Fraction(value) for value in coeff]+[Fraction(-c)] for c,coeff in equations];pivot=0;rows=[]
        for column in range(n):
            found=next((index for index in range(pivot,len(matrix)) if matrix[index][column]),None)
            if found is None:continue
            matrix[pivot],matrix[found]=matrix[found],matrix[pivot];row=matrix[pivot];factor=row[column];row=[value/factor for value in row];matrix[pivot]=row
            for index,other in enumerate(matrix):
                if index!=pivot and other[column]:
                    factor=other[column];matrix[index]=[value-factor*basis for value,basis in zip(other,row)]
            rows.append((column,row));pivot+=1
        require(all(any(row[:-1]) or not row[-1] for row in matrix),'Native stack constraints disagree')
        self.rows=rows;self.equation_count=len(equations);self.rank=len(rows);self.known_pops=dict(known_pops);self.solved_pops={}
        self.zero_columns = zero_cleanup_columns(rows)
        for site,column in columns.items():
            coeff=[0]*n;coeff[column]=1;constant,free=self.reduce((0,tuple(coeff)))
            if not any(free):
                require(constant.denominator==1 and 0<=constant<=4096 and constant%4==0,'Solved cleanup is not a valid normal DWORD ABI')
                self.solved_pops[site]=int(constant)

    def reduce(self,expression):
        c,values=expression;c=Fraction(c);values=list(map(Fraction,values))
        for column,row in self.rows:
            factor=values[column]
            if factor:
                c+=factor*row[-1];values=[value-factor*basis for value,basis in zip(values,row[:-1])]
        for column in self.zero_columns:
            values[column] = Fraction(0)
        return c,tuple(values)

    def address_expression(self,at,displacement=0,relative_to=None,relative_displacement=0):
        require(at in self.states,'Unknown stack query instruction')
        c,coeff=self.states[at];c+=displacement
        if relative_to is not None:
            require(relative_to in self.states,'Unknown stack origin instruction')
            other_c,other_coeff=self.states[relative_to];c-=other_c+relative_displacement;coeff=tuple(value-other for value,other in zip(coeff,other_coeff))
        return c, coeff

    def address(self,at,displacement=0,relative_to=None,relative_displacement=0):
        expression = self.address_expression(at, displacement, relative_to, relative_displacement)
        constant,free=self.reduce(expression)
        return dict(constant=str(constant),unresolved={str(site):str(value) for site,value in zip(self.variables,free) if value},proved=not any(free))

    def disjoint_addresses(self, at, displacement, size, other_at, other_displacement, other_size):
        """Prove two ESP-relative accesses disjoint for all nonnegative cleanups."""
        require(isinstance(size, int) and size > 0 and
                isinstance(other_size, int) and other_size > 0, "Unknown stack access width")
        expression = self.address_expression(at, displacement, other_at, other_displacement)
        for constant, coefficients in (expression, self.reduce(expression)):
            if ((constant + size <= 0 and all(value <= 0 for value in coefficients)) or
                    (constant >= other_size and all(value >= 0 for value in coefficients))):
                return True
        return False

    def height(self,at):
        result=self.address(at);require(result['proved'],'Native frame height remains underdetermined');value=Fraction(result['constant']);require(value.denominator==1,'Nonintegral native stack height');return int(value)



def aligned_frames(graph, known_pops):
    """Solve offsets relative to ESP immediately after a proved alignment.

    This is opt-in. The resulting origin is the aligned frame, so it cannot
    locate incoming arguments relative to the caller's original ESP.
    """
    entry = graph["entry"]
    nodes, edges, predecessors = (graph[key] for key in ("nodes", "edges", "pred"))

    def next_instruction(at):
        following = at + nodes[at].size
        require(edges[at] == [following] and following in nodes and
                set(predecessors[following]) == {at}, "Nonlinear aligned frame boundary")
        return following

    def register_operands(at, opcode, registers):
        ins = nodes[at]
        return ins.mnemonic == opcode and len(ins.operands) == len(registers) and all(
            op.type == X.X86_OP_REG and op.size == 4 and op.reg == register
            for op, register in zip(ins.operands, registers)
        )

    require(not predecessors.get(entry) and register_operands(entry, "push", (X.X86_REG_EBP,)),
            "Aligned frame lacks a single saved EBP entry")
    establish = next_instruction(entry)
    require(register_operands(establish, "mov", (X.X86_REG_EBP, X.X86_REG_ESP)),
            "Aligned frame lacks an incoming EBP anchor")
    mask_at = next_instruction(establish)
    mask = nodes[mask_at]
    require(mask.mnemonic == "and" and len(mask.operands) == 2 and
            mask.operands[0].type == X.X86_OP_REG and mask.operands[0].reg == X.X86_REG_ESP and
            mask.operands[0].size == 4 and mask.operands[1].type == X.X86_OP_IMM and
            (mask.operands[1].imm & 0xffffffff) in (0xfffffff8, 0xfffffff0),
            "Unsupported ESP alignment")
    origin = next_instruction(mask_at)

    # The saved registers immediately follow the fixed allocation. Their
    # inverse sequence must appear before every restore of original ESP.
    allocation = nodes[origin]
    require(allocation.mnemonic == "sub" and len(allocation.operands) == 2 and
            allocation.operands[0].type == X.X86_OP_REG and
            allocation.operands[0].reg == X.X86_REG_ESP and allocation.operands[0].size == 4 and
            allocation.operands[1].type == X.X86_OP_IMM and
            0 < allocation.operands[1].imm <= 65536 and allocation.operands[1].imm % 4 == 0,
            "Unknown aligned frame allocation")
    saved = []
    at = next_instruction(origin)
    nonvolatile = {X.X86_REG_EBX, X.X86_REG_EBP, X.X86_REG_ESI, X.X86_REG_EDI}
    while nodes[at].mnemonic == "push":
        operands = nodes[at].operands
        require(len(operands) == 1 and operands[0].type == X.X86_OP_REG and operands[0].size == 4,
                "Unknown aligned save")
        reg = operands[0].reg
        if reg not in nonvolatile:
            break
        require(all(reg != previous for _, previous in saved), "Duplicate aligned save")
        saved.append((at, reg))
        at = next_instruction(at)
    require(any(reg == X.X86_REG_EBP for _, reg in saved), "Aligned body lacks a saved EBP slot")

    restores = [at for at in nodes if register_operands(at, "mov", (X.X86_REG_ESP, X.X86_REG_EBP))]
    require(restores, "No aligned frame restore")
    tails = set()
    pairs = []
    for restore in restores:
        pop = next_instruction(restore)
        require(register_operands(pop, "pop", (X.X86_REG_EBP,)), "Aligned restore lacks caller EBP pop")
        ret = next_instruction(pop)
        require(nodes[ret].mnemonic == "ret" and not edges[ret], "Aligned restore does not return")
        tails.update((restore, pop, ret))
        cursor = restore
        for push, register in saved:
            require(len(predecessors[cursor]) == 1, "Aligned epilogue can bypass a register pop")
            before, = predecessors[cursor]
            require(next_instruction(before) == cursor and register_operands(before, "pop", (register,)),
                    "Aligned register restore order differs")
            pairs.append((push, before))
            cursor = before

    # A proved restore reintroduces the caller's unknown alignment bias.
    # Cut that transition rather than equating it with an ordinary return.
    kept = set()
    pending = [origin]
    while pending:
        at = pending.pop()
        if at in kept or at in tails:
            continue
        kept.add(at)
        pending.extend(edges[at])
    require(set(nodes) == kept | tails | {entry, establish, mask_at} and
            kept.isdisjoint({entry, establish, mask_at}), "Aligned frame has another entry or exit")
    require(all(edges[at] and nodes[at].mnemonic != "ret" for at in kept),
            "Aligned body bypasses a restore")
    require(all(push in kept and pop in kept for push, pop in pairs), "Unreachable aligned save or restore")
    kept_edges = {at: [target for target in edges[at] if target in kept] for at in kept}
    kept_predecessors = {at: set(predecessors[at]) & kept for at in kept}
    relative_graph = dict(entry=origin, nodes={at: nodes[at] for at in kept},
                          edges=kept_edges, pred=kept_predecessors)
    frames = AffineFrames(relative_graph, known_pops)
    for push, pop in pairs:
        displacement = frames.address(pop, relative_to=push, relative_displacement=-4)
        require(displacement["proved"] and displacement["constant"] == "0",
                "Aligned restore slot is not proved")
    return frames
