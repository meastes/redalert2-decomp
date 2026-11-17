"""Restore omitted fastcall operands with native forwarding evidence."""

from collections import deque
from pathlib import Path
import re

from seed_guarded_cfg import PARTS, X, guarded_graph, require


REGISTERS = (
    X.X86_REG_EAX, X.X86_REG_ECX, X.X86_REG_EDX, X.X86_REG_EBX,
    X.X86_REG_EBP, X.X86_REG_ESI, X.X86_REG_EDI,
)


def source_site(body, name, params, seed):
    """Return the sole direct self-call and its explicitly supplied words."""
    view = seed.code_view(body)
    require(not re.search(r"#|\b(?:asm|__asm|typedef|using)\b", view),
            "Unsupported source scope")
    parts = list(map(seed.param_parts, params))
    require(len(parts) >= 3 and all(part[1] for part in parts),
            "Missing fastcall formal")
    for kind, name_part in parts:
        require(kind in seed.INTEGRAL32 | seed.POINTER_ALIASES |
                {"char", "short", "bool"} or re.fullmatch(r"[\w ]+\s*\*", kind),
                "Unknown fastcall formal width")
    require(len({part[1] for part in parts}) == len(parts), "Duplicate formal")
    rows, comma = seed.scalar_declarations(view)
    declared = {row[1] for row in rows}
    require(not comma and not ({name, parts[0][1], parts[1][1]} & declared),
            "Shadowed forwarding name")
    for _, formal in parts[:2]:
        token = r"\b" + re.escape(formal) + r"\b"
        require(not re.search(r"\b_" + re.escape(formal) + r"\b|" + token +
                              r"\s*\.\s*_", view), "Partial forwarding alias")
        require(not re.search(
            r"(?:&|\+\+|--)\s*" + token + "|" + token +
            r"\s*(?:\+\+|--|(?:[+*/%&|^<>-]|<<|>>)?=(?!=))", view),
            "Forwarded formal can change or escape")
    sites = list(seed.call_sites(body, name))
    require(len(sites) == 1, "No unique source self-call")
    start, end, args, used = sites[0]
    prefix = view[:start].rstrip()
    require(view.startswith(name, start) and not used and prefix.endswith(("{", ";", "}")) and
            re.match(r"\s*;", view[end:]) and view[start:end] == body[start:end],
            "Unsupported self-call context")
    require(len(args) == len(parts) - 2, "Source stack argument count differs")
    values = []
    for arg in args:
        arg = arg.strip()
        if re.fullmatch(r"(?:0[xX][\da-fA-F]+|0|[1-9]\d*)", arg):
            values.append(("imm", int(arg, 16 if arg.lower().startswith("0x") else 10)))
        else:
            require(re.fullmatch(r"DAT_[\da-fA-F]{8}", arg) and arg not in declared
                    and arg not in {part[1] for part in parts}, "Unbound stack argument")
            values.append(("global", int(arg[4:], 16)))
    opening = view.index("(", start, end)
    return opening + 1, [part[1] for part in parts[:2]], values


def incoming_registers(graph, stop, anchor=None):
    """Track copies of ECX and EDX through a closed ordinary i386 CFG."""
    entry = graph["entry"]
    incoming = {entry: {X.X86_REG_ECX: 0, X.X86_REG_EDX: 1}}
    todo = deque([entry])
    nodes = graph["nodes"]
    while todo:
        at = todo.popleft()
        before = incoming[at]
        after = before.copy()
        ins = nodes[at]
        for reg in ins.regs_access()[1]:
            after.pop(PARTS.get(reg, reg), None)
        ops = ins.operands
        if ins.mnemonic == "call":
            require(not (ops[0].type == X.X86_OP_IMM and
                         ops[0].imm in nodes and ops[0].imm != entry),
                    "Intra-body call has no external ABI contract")
            for reg in (X.X86_REG_EAX, X.X86_REG_ECX, X.X86_REG_EDX):
                after.pop(reg, None)
        if ins.mnemonic == "mov" and len(ops) == 2 and all(
                op.type == X.X86_OP_REG and op.size == 4 for op in ops):
            if ops[1].reg in before and ops[0].reg in REGISTERS:
                after[ops[0].reg] = before[ops[1].reg]
        if anchor is not None and at == anchor[0]:
            after[anchor[1]] = 2
        for dest in graph["edges"][at]:
            previous = incoming.get(dest)
            merged = after if previous is None else {
                reg: value for reg, value in previous.items() if after.get(reg) == value
            }
            if previous != merged:
                incoming[dest] = merged
                todo.append(dest)
    return incoming[stop]


def native_site(graph, stack_values, anchor=None):
    nodes = graph["nodes"]
    entry = graph["entry"]
    sites = [at for at, ins in nodes.items() if ins.mnemonic in ("call", "jmp")
             and ins.operands[0].type == X.X86_OP_IMM and ins.operands[0].imm == entry]
    require(len(sites) == 1 and nodes[sites[0]].mnemonic == "call",
            "No unique direct native self-call")
    site = sites[0]
    returns = [ins for ins in nodes.values() if ins.mnemonic == "ret"]
    require(returns and all(len(ins.operands) == 1 and
                            ins.operands[0].imm == 4 * len(stack_values) for ins in returns),
            "Native cleanup contradicts stack arguments")
    origins = incoming_registers(graph, site, anchor)
    require(origins.get(X.X86_REG_ECX) == 0 and origins.get(X.X86_REG_EDX) == (2 if anchor else 1),
            "Register operands are not proven incoming forwards")

    def predecessor(at):
        parents = graph["pred"][at]
        require(len(parents) == 1, "Alternate ingress in self-call setup")
        ins = nodes[next(iter(parents))]
        require(ins.address + ins.size == at, "Nonlinear self-call setup")
        return ins

    # Work backward over the contiguous push sequence. The nearest push is
    # the first stack operand in the source signature.
    at = site
    for wanted in stack_values:
        for _ in range(8):
            ins = predecessor(at)
            at = ins.address
            if ins.mnemonic == "push":
                break
            require(ins.mnemonic == "mov" and ins.operands[0].type == X.X86_OP_REG
                    and ins.operands[0].reg in REGISTERS and ins.operands[0].size == 4,
                    "Unknown operation in stack setup")
        else:
            require(False, "No nearby stack push")
        op = ins.operands[0]
        require(op.size == 4, "Unexpected pushed width")
        if op.type == X.X86_OP_REG:
            reg = op.reg
            cursor = at
            for _ in range(12):
                load = predecessor(cursor)
                cursor = load.address
                if reg in {PARTS.get(x, x) for x in load.regs_access()[1]}:
                    require(load.mnemonic == "mov" and load.operands[0].type == X.X86_OP_REG
                            and load.operands[0].reg == reg and load.operands[0].size == 4,
                            "Unknown pushed register definition")
                    op = load.operands[1]
                    break
                require(load.mnemonic in ("push", "mov") and
                        (load.mnemonic == "push" or
                         load.operands[0].type == X.X86_OP_REG),
                        "Pushed value crosses unknown memory effect")
            else:
                require(False, "No nearby pushed register definition")
        if op.type == X.X86_OP_IMM:
            actual = ("imm", op.imm & 0xffffffff)
        elif op.type == X.X86_OP_MEM and op.size == 4 and not (
                op.mem.base or op.mem.index or op.mem.segment):
            actual = ("global", op.mem.disp & 0xffffffff)
        else:
            require(False, "Unsupported stack operand")
        require(actual == wanted, "Native stack operand differs")
    return site


def source_control_id(body, name, dialog, params, seed):
    """Bind the ID already used for GetDlgItem in the recursive source block."""
    view = seed.code_view(body)
    rows, comma = seed.scalar_declarations(view)
    require(not comma and all(row[1] != "GetDlgItem" for row in rows)
            and all(seed.param_parts(p)[1] != "GetDlgItem" for p in params),
            "Shadowed control lookup")
    call_start = next(seed.call_sites(body, name))[0]

    def scope(position):
        ancestors = []
        for match in re.finditer(r"[{}]", view[:position]):
            if match[0] == "{":
                ancestors.append(match.start())
            else:
                require(ancestors, "Unbalanced source scope")
                ancestors.pop()
        return ancestors

    anchors = []
    for start, end, args, used in seed.call_sites(body, "GetDlgItem"):
        if start >= call_start or scope(start) != scope(call_start):
            continue
        if len(args) == 2 and args[0].strip() == dialog and used:
            anchors.append((start, end, args[1].strip()))
    require(len(anchors) == 1,
            "No unique control lookup in the self-call block")
    start, end, control = anchors[0]
    require(re.fullmatch(r"[A-Za-z_]\w*", control) and
            seed.variable_types(body, params).get(control) == "int" and
            sum(row[1] == control for row in rows) == 1,
            "Control ID is not an existing int")
    token = r"\b" + re.escape(control) + r"\b"
    require(not re.search(r"\b_" + re.escape(control) + r"\b|" + token +
                          r"\s*\.\s*_", view), "Partial control ID alias")
    require(not re.search(r"&\s*" + token, view) and not re.search(
        r"(?:\+\+|--)\s*" + token + "|" + token +
        r"\s*(?:\+\+|--|(?:[+*/%&|^<>-]|<<|>>)?=(?!=))", view[end:call_start]),
        "Control ID can change or escape")
    assignment = re.search(r"(?m)^[ \t]*[A-Za-z_]\w*\s*=\s*$", view[:start])
    require(assignment and view[:assignment.start()].rstrip().endswith(("{", ";", "}"))
            and re.match(r"\s*;", view[end:]), "Control lookup is not a standalone assignment")
    require(not re.search(r"\bgoto\b|(?<!:):(?!:)", view[end:call_start]),
            "Jump or label between the control lookup and self-call")
    return control


def native_control_lookup(graph, iat):
    """Find the dominating lookup whose saved ID reaches the recursive call."""
    nodes = graph["nodes"]
    entry = graph["entry"]
    self_calls = [at for at, ins in nodes.items() if ins.mnemonic == "call"
                  and ins.operands[0].type == X.X86_OP_IMM and ins.operands[0].imm == entry]
    require(len(self_calls) == 1, "No unique native recursion")
    target = self_calls[0]
    candidates = []
    for at, ins in nodes.items():
        if ins.mnemonic != "call":
            continue
        op = ins.operands[0]
        if op.type != X.X86_OP_MEM or op.mem.base or op.mem.index or op.mem.segment or op.mem.disp != iat:
            continue
        cursor = at
        pushes = []
        for _ in range(5):
            parents = graph["pred"][cursor]
            if len(parents) != 1:
                break
            parent = nodes[next(iter(parents))]
            if parent.address + parent.size != cursor:
                break
            cursor = parent.address
            if (parent.mnemonic == "push" and parent.operands[0].type == X.X86_OP_REG
                    and parent.operands[0].size == 4):
                pushes.append(parent.operands[0].reg)
                if len(pushes) == 2:
                    break
                continue
            if parent.mnemonic != "mov" or len(parent.operands) != 2:
                break
            dest = parent.operands[0]
            if not (dest.type == X.X86_OP_MEM and dest.mem.base == X.X86_REG_ESP
                    and not dest.mem.index and not dest.mem.segment and dest.mem.disp >= 8):
                break
        if len(pushes) != 2:
            continue
        dialog, control = pushes
        if control not in (X.X86_REG_EBX, X.X86_REG_EBP, X.X86_REG_ESI, X.X86_REG_EDI):
            continue
        before = incoming_registers(graph, at)
        if before.get(dialog) != 0:
            continue
        origin = incoming_registers(graph, target, (at, control))
        if origin.get(X.X86_REG_EDX) == 2:
            candidates.append((at, control))
    require(len(candidates) == 1, "No unique native lookup provides recursive ID")
    return candidates[0]


def optional_recursive_forwarding(body, name, params, conv, entry, inventory, repository, seed):
    if conv != "__fastcall" or name not in body:
        return body, []
    try:
        at, forwards, values = source_site(body, name, params, seed)
        repository = Path(repository)
        evidence = (repository / "private/original-target-20261020/inventory").resolve()
        directory = next((p for p in inventory.passes
                          if (p / "decompiled" / ("%08x.c" % entry)).is_file()), None)
        require(directory is not None and directory.resolve().is_relative_to(evidence),
                "Missing local frozen source")
        original = inventory.decompiled(entry)
        _, source_conv, source_name, source_params, span = seed.parse_header(original)
        require((source_conv, source_name, source_params) == (conv, name, params)
                and body == seed.nonnegative_tests(seed.reindent(original[span[1]:].strip())),
                "Source differs from frozen export")
        from verify_manifest import Audit
        audit = Audit(repository, "private/original-target-20261020/inputs/game.exe")
        graph = guarded_graph(audit, inventory, entry)
        try:
            native_site(graph, values)
        except ValueError:
            control = source_control_id(body, name, forwards[0], params, seed)
            imports = [item.address for dll in audit.pe.DIRECTORY_ENTRY_IMPORT
                       if dll.dll.lower() == b"user32.dll"
                       for item in dll.imports if item.name == b"GetDlgItem"]
            require(len(imports) == 1, "Missing unique USER32 GetDlgItem")
            anchor = native_control_lookup(graph, imports[0])
            native_site(graph, values, anchor)
            forwards[1] = control
    except (ValueError, TypeError, KeyError, OSError):
        return body, []
    return (body[:at] + ", ".join(forwards) + ", " + body[at:],
            ["//   Scratch hypothesis: native recursive fastcall register values bound to existing source operands; original stack operands retained."])
