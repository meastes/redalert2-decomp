"""Recover omitted recursive operands after a proved constructor branch."""

from pathlib import Path
import re

from seed_guarded_cfg import X, guarded_graph, require


NUMBER = r"(?:0[xX][0-9a-fA-F]+|0|[1-9][0-9]*)"
FIELD = re.compile(
    r"\*\s*\(\s*(?:undefined4|uint|int)\s*\*\s*\)\s*\(\s*"
    r"(DAT_[0-9a-fA-F]{8})\s*\+\s*(" + NUMBER + r")\s*\)"
)
SAVED = {X.X86_REG_EBX, X.X86_REG_EBP, X.X86_REG_ESI, X.X86_REG_EDI}


def word(expression):
    text = expression.strip()
    if re.fullmatch(NUMBER, text):
        return ("imm", int(text, 0))
    match = FIELD.fullmatch(text)
    require(match, "Unbound source operand")
    return ("field", int(match[1][4:], 16), int(match[2], 0))


def source_binding(body, name, params, seed):
    """Read the original self-call and adjacent conditional constructor."""
    view = seed.code_view(body)
    require(not re.search(r"#|\b(?:asm|__asm|typedef|using)\b", view),
            "Unsupported source scope")
    formals = list(map(seed.param_parts, params))
    require(len(formals) == 5 and formals[0][0] == "int *" and formals[1][0] == "int",
            "Unsupported recursive register signature")
    sites = list(seed.call_sites(body, name))
    require(len(sites) == 1, "Expected one source self-call")
    start, end, args, used = sites[0]
    require(not used and view.startswith(name, start) and len(args) == 3 and
            view[start:end] == body[start:end], "Unsupported recursive source call")
    stack = [word(arg) for arg in args]
    require(stack[0] == ("imm", 0) and stack[1][0] == "field" and
            stack[2][0] == "imm", "Unsupported recursive stack operands")
    terminator = re.match(r"\s*;", view[end:])
    require(terminator, "Self-call is not a statement")
    anchors = list(re.finditer(
        r"\bif\s*\(\s*[A-Za-z_]\w*\s*!=\s*\(\s*void\s*\*\s*\)\s*0x0\s*\)"
        r"\s*\{\s*(FUN_[0-9a-f]{8})\s*\(", view[:start]
    ))
    require(anchors, "Missing adjacent conditional constructor")
    anchor = anchors[-1]
    constructor_name = anchor[1]
    ctor_sites = list(seed.call_sites(body, constructor_name))
    require(len(ctor_sites) == 1, "Ambiguous source constructor")
    ctor_start, ctor_end, ctor_args, ctor_used = ctor_sites[0]
    require(not ctor_used and ctor_start == anchor.start(1) and
            re.fullmatch(r"\s*;\s*\}\s*", view[ctor_end:start]) and
            view[anchor.start():start] == body[anchor.start():start],
            "Constructor branch contains other source effects")
    require(len(ctor_args) >= 2, "Missing source coordinate")
    coordinate = ctor_args[1].strip()
    declarations, comma = seed.scalar_declarations(view)
    require(not comma and sum(row[1] == coordinate for row in declarations) == 1 and
            re.fullmatch(r"[A-Za-z_]\w*", coordinate) and
            seed.variable_types(body, params).get(coordinate) == "int *",
            "Coordinate is not a unique existing pointer")
    require(not re.search(r"&\s*\(*\s*\b" + re.escape(coordinate) + r"\b", view),
            "Coordinate pointer storage escapes")
    global_name = FIELD.fullmatch(args[1].strip())[1]
    require(global_name not in seed.variable_types(body, params) and
            not ({name, constructor_name} & {row[1] for row in declarations}),
            "Shadowed source operand")
    ctor_stack = [("coordinate",) if i == 1 else word(arg)
                  for i, arg in enumerate(ctor_args)]
    return dict(start=start, end=end + terminator.end(), coordinate=coordinate,
                constructor=int(constructor_name[4:], 16), ctor_stack=ctor_stack,
                stack=stack, global_name=global_name)


def path_from_branch(graph, end):
    """Walk to the split before one incoming edge of the join."""
    nodes = graph["nodes"]
    at = end
    path = []
    for _ in range(24):
        path.append(at)
        parents = graph["pred"][at]
        require(len(parents) == 1, "Alternate ingress into constructor branch")
        parent = next(iter(parents))
        if len(graph["edges"][parent]) == 2:
            return parent, list(reversed(path))
        require(graph["edges"][parent] == [at] and nodes[parent].address + nodes[parent].size == at,
                "Nonlinear constructor branch")
        at = parent
    require(False, "Constructor branch exceeds bound")


def evaluate(graph, path, registers, cleanup=None):
    """Evaluate only DWORD loads, copies, and pushes in the local proof."""
    regs = registers.copy()
    stack = []
    reads = []
    seen_call = False

    def value(operand):
        require(operand.size == 4, "Non-DWORD operand")
        if operand.type == X.X86_OP_REG:
            require(operand.reg != X.X86_REG_ESP, "Escaping stack address")
            return regs.get(operand.reg)
        if operand.type == X.X86_OP_IMM:
            return ("imm", operand.imm & 0xffffffff)
        require(operand.type == X.X86_OP_MEM and not operand.mem.index and not operand.mem.segment,
                "Unsupported memory expression")
        mem = operand.mem
        if not mem.base:
            result = ("global", mem.disp & 0xffffffff)
        elif mem.base == X.X86_REG_ESP:
            offset = mem.disp - 4 * len(stack)
            require(offset >= 0 and offset % 4 == 0 and not seen_call,
                    "Unproved stack home read")
            result = ("frame", offset)
        else:
            base = regs.get(mem.base)
            require(base and base[0] == "global" and mem.disp >= 0 and mem.disp % 4 == 0,
                    "Unsupported indirect load")
            result = ("field", base[1], mem.disp)
        reads.append(result)
        return result

    for at in path:
        ins = graph["nodes"][at]
        ops = ins.operands
        if ins.mnemonic == "mov":
            dest, source = ops
            require(dest.type == X.X86_OP_REG and dest.size == 4 and dest.reg != X.X86_REG_ESP,
                    "Unexpected write in local path")
            regs[dest.reg] = value(source)
        elif ins.mnemonic == "push":
            stack.append(value(ops[0]))
        elif ins.mnemonic == "call":
            require(cleanup and not seen_call and ops[0].type == X.X86_OP_IMM and
                    ops[0].imm == cleanup[0] and len(stack) == cleanup[1] and
                    regs.get(X.X86_REG_ECX) == ("allocation",), "Unproved constructor call")
            seen_call = True
            actual = list(reversed(stack))
            coordinate = actual[1]
            require(coordinate and coordinate[0] == "frame", "Unproved coordinate home")
            actual[1] = ("coordinate",)
            require(actual == cleanup[2], "Constructor operand mismatch")
            stack.clear()
            regs = {reg: origin for reg, origin in regs.items() if reg in SAVED}
        else:
            require(ins.mnemonic == "jmp" and at == path[-1], "Unsupported local instruction")
    return regs, list(reversed(stack)), reads, seen_call


def native_binding(graph, binding):
    """Prove the shared pointer home and both recursive register values."""
    entry = graph["entry"]
    nodes = graph["nodes"]
    recursive = [at for at, ins in nodes.items() if ins.mnemonic in ("call", "jmp")
                 and ins.operands[0].type == X.X86_OP_IMM and ins.operands[0].imm == entry]
    require(len(recursive) == 1 and nodes[recursive[0]].mnemonic == "call",
            "No unique direct native recursion")
    site = recursive[0]
    returns = [ins for ins in nodes.values() if ins.mnemonic == "ret"]
    require(returns and all(len(ins.operands) == 1 and ins.operands[0].imm == 12 for ins in returns),
            "Recursive cleanup differs from source")
    constructor = binding["constructor"]
    ctor_calls = [at for at, ins in nodes.items() if ins.mnemonic == "call"
                  and ins.operands[0].type == X.X86_OP_IMM and ins.operands[0].imm == constructor]
    require(len(ctor_calls) == 1, "No unique native constructor")
    at = site
    tail = []
    for _ in range(16):
        parents = graph["pred"][at]
        if len(parents) == 2:
            break
        require(len(parents) == 1, "Unknown recursive call ingress")
        previous = next(iter(parents))
        require(nodes[previous].address + nodes[previous].size == at and
                graph["edges"][previous] == [at], "Nonlinear recursive tail")
        tail.append(previous)
        at = previous
    else:
        require(False, "No nearby constructor join")
    join = at
    paths = [path_from_branch(graph, end) for end in graph["pred"][join]]
    split = paths[0][0]
    require(paths[1][0] == split and nodes[split].mnemonic in ("je", "jne") and
            set(graph["edges"][split]) == {path[0] for _, path in paths},
            "Not a closed two-path constructor branch")
    parents = graph["pred"][split]
    require(len(parents) == 1, "Ambiguous constructor condition")
    test = nodes[next(iter(parents))]
    require(test.mnemonic == "test" and test.op_str == "eax, eax" and
            test.address + test.size == split, "Unknown constructor predicate")
    ctor_path = next((path for _, path in paths if ctor_calls[0] in path), None)
    require(ctor_path is not None, "Constructor is outside the branch")
    taken = nodes[split].operands[0].imm
    require((ctor_path[0] == taken) == (nodes[split].mnemonic == "jne"),
            "Constructor predicate disagrees with source")
    other_path = next(path for _, path in paths if path != ctor_path)
    arg_count = len(binding["ctor_stack"])
    ctor_regs, ctor_remaining, ctor_reads, called = evaluate(
        graph, ctor_path, {X.X86_REG_EAX: ("allocation",)},
        (constructor, arg_count, binding["ctor_stack"])
    )
    other_regs, other_remaining, other_reads, _ = evaluate(graph, other_path, {})
    require(called and not ctor_remaining and not other_remaining and
            len(other_reads) == 1 and other_reads[0][0] == "frame" and
            [read for read in ctor_reads if read[0] == "frame"] == other_reads,
            "Constructor paths do not read the same coordinate home")
    home = other_reads[0]
    common = {reg: origin for reg, origin in ctor_regs.items()
              if reg in SAVED and origin == home and other_regs.get(reg) == home}
    regs, stack, reads, _ = evaluate(graph, list(reversed(tail)), common)
    require(stack == binding["stack"] and regs.get(X.X86_REG_ECX) == home,
            "Recursive coordinate or stack arguments differ")
    damage = regs.get(X.X86_REG_EDX)
    warhead = stack[1]
    require(damage and damage[0] == "field" and damage[1] == warhead[1] and
            reads == [("global", warhead[1]), warhead, damage],
            "Unproved ordered recursive field loads")
    return dict(site=site, constructor_site=ctor_calls[0], join=join,
                coordinate_frame_offset=home[1], damage_offset=damage[2],
                warhead_offset=warhead[2], reads=reads)


def constructor_pin(repository, audit, inventory, binding):
    """Verify the accepted constructor interface and its native cleanup."""
    import catalog_io
    address = binding["constructor"]
    catalog = catalog_io.load(catalog_io.locate(repository), references=False)
    matches = [row for row in catalog["regions"] if row["entry"] == address]
    require(len(matches) == 1, "Constructor is not uniquely accepted")
    row = matches[0]
    name = re.fullmatch(r"\?\?0([A-Za-z_]\w*)@@QAE@.+@Z", row["symbol"])
    require(name, "Not an accepted i386 thiscall constructor")
    source = audit.read(row["source"], row["source_sha256"]).decode("utf-8")
    owner = re.escape(name[1])
    signatures = list(re.finditer(
        r"(?m)^" + owner + r"::" + owner + r"\s*\(([^()]*)\)", source
    ))
    require(len(signatures) == 1, "Constructor source interface is ambiguous")
    args = signatures[0][1].split(",")
    require(len(args) == len(binding["ctor_stack"]) and
            re.fullmatch(r"\s*CoordStruct\s+const\s*&\s*[A-Za-z_]\w*\s*", args[1]),
            "Constructor coordinate reference is unproved")
    native = guarded_graph(audit, inventory, address)
    require(native["body_sha256"] == row["body_sha256"] and
            len(native["raw"]) == row["body_bytes"], "Constructor native bound differs")
    returns = [ins for ins in native["nodes"].values() if ins.mnemonic == "ret"]
    require(returns and all(len(ins.operands) == 1 and ins.operands[0].imm == 4 * len(args)
                            for ins in returns), "Constructor cleanup differs")
    return row["source_sha256"], row["body_sha256"]


def render(body, name, binding, proof):
    """Keep the shared base load and the two field reads in native order."""
    identifiers = set(re.findall(r"\b[A-Za-z_]\w*\b", body))

    def fresh(stem):
        candidate = stem
        while candidate in identifiers:
            candidate += "_"
        identifiers.add(candidate)
        return candidate

    base = fresh("recursive_base")
    stack_word = fresh("recursive_stack_word")
    start, end = binding["start"], binding["end"]
    flag = binding["stack"][2][1]
    replacement = (
        "{\n        unsigned int %s = (unsigned int)%s;\n"
        "        unsigned int %s = *(unsigned int *)(%s + 0x%x);\n"
        "        %s(%s, *(int *)(%s + 0x%x), 0, %s, %d);\n    }"
        % (base, binding["global_name"], stack_word, base, proof["warhead_offset"],
           name, binding["coordinate"], base, proof["damage_offset"], stack_word, flag)
    )
    return body[:start] + replacement + body[end:]


def optional_recursive_diamond(body, name, params, conv, entry, inventory, repository, seed):
    if conv != "__fastcall" or body.count(name) != 1 or len(params) != 5:
        return body, []
    try:
        binding = source_binding(body, name, params, seed)
        repository = Path(repository)
        evidence = (repository / "private/original-target-20261020/inventory").resolve()
        directory = next((p for p in inventory.passes
                          if (p / "decompiled" / ("%08x.c" % entry)).is_file()), None)
        require(directory is not None and directory.resolve().is_relative_to(evidence),
                "Missing local frozen source")
        original = inventory.decompiled(entry)
        _, original_conv, original_name, original_params, span = seed.parse_header(original)
        require((original_conv, original_name, original_params) == (conv, name, params) and
                body == seed.nonnegative_tests(seed.reindent(original[span[1]:].strip())),
                "Source differs from frozen export")
        from verify_manifest import Audit
        audit = Audit(repository, "private/original-target-20261020/inputs/game.exe")
        proof = native_binding(guarded_graph(audit, inventory, entry), binding)
        constructor_pin(repository, audit, inventory, binding)
    except (ValueError, TypeError, KeyError, OSError):
        return body, []
    return render(body, name, binding, proof), [
        "//   Scratch hypothesis: constructor-branch coordinate and ordered recursive field loads verified in native code."
    ]
