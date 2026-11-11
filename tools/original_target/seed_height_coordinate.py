"""Recover a constructed integer coordinate and a forwarded height input."""

from pathlib import Path
import re

from seed_guarded_cfg import X, guarded_graph, require
from seed_recursive_forwarding import incoming_registers


def source_binding(body, params, seed):
    """Find the two fake FP stores and both missing coordinate operands."""
    view = seed.code_view(body)
    require(not re.search(r"#|\b(?:asm|__asm|typedef|using)\b", view),
            "Unsupported source scope")
    if not params or seed.param_parts(params[0])[0] != "int *":
        require(False, "No incoming coordinate pointer")
    incoming = seed.param_parts(params[0])[1]
    names = seed.variable_types(body, params)
    declarations, comma = seed.scalar_declarations(view)
    require(not comma, "Ambiguous local declarations")
    shape = re.compile(
        r"(?m)^[ \t]*(?P<x>\w+)\s*=\s*\(float\)\s*"
        r"\((?P<low>\w+)\s*\*\s*0x100\s*\+\s*0x80\)\s*;\s*"
        r"(?P<y>\w+)\s*=\s*\(double\)\s*\(ulonglong\)\s*"
        r"\((?P<high>\w+)\s*\*\s*0x100\s*\+\s*0x80\)\s*;\s*"
        r"(?P<bridge>\w+\s*=\s*\w+\s*;)\s*"
        r"(?P<result>\w+)\s*=\s*(?P<callee>FUN_[0-9a-f]{8})\s*\(\s*\)\s*;"
    )
    matches = list(shape.finditer(view))
    require(len(matches) == 1, "No unique coordinate storage artifact")
    match = matches[0]
    require(view[match.start():match.end()] == body[match.start():match.end()] and
            view[:match.start()].rstrip().endswith((";", "{", "}")),
            "Unsupported coordinate assignment context")
    fields = (match["x"], match["y"], match["low"], match["high"])
    require(len(set(fields)) == 4 and all(sum(row[1] == f for row in declarations) == 1
                                         for f in fields), "Unbound coordinate locals")
    require(tuple(names.get(f) for f in fields) == ("float", "undefined8", "short", "short"),
            "Coordinate local types differ")
    token = r"\b" + re.escape(incoming) + r"\b"
    require(not re.search(r"&\s*\(*\s*" + token + "|" + token +
                          r"\s*(?:\+\+|--|[+*/%&|^<>-]?=(?!=)|\.\s*_)", view),
            "Incoming coordinate is rebound or escapes as a local")
    compact = re.sub(r"\s+", "", view[:match.start()])
    for local, operand in ((match["low"], "*" + incoming),
                           (match["high"], incoming + "[1]")):
        expression = local + "=(short)(" + operand + "+(" + operand + ">>0x1f&0xffU)>>8);"
        require(compact.count(expression) == 1 and
                len(re.findall(r"\b"+local+r"\s*=(?!=)", view[:match.start()])) == 1,
                "Signed cell component expression differs")

    def depth(at):
        return view[:at].count("{") - view[:at].count("}")

    # The first real double assignment must dominate all later reads of
    # the reused carrier. Before it, both old names occur only as LHSs.
    writes = re.finditer(r"(?m)^[ \t]*" + match["y"] + r"\s*=\s*\(double\)",
                         view[match.end():])
    offsets = [match.end() + m.start() for m in writes]
    overwrites = [at for at in offsets if depth(at) == 1 and
                  view[:at].rstrip().endswith((";", "{", "}"))]
    require(overwrites, "No independent floating lifetime")
    overwrite = overwrites[0]
    assignment = re.match(r"[ \t]*" + match["y"] + r"\s*=([^;]*);", view[overwrite:])
    require(assignment and not re.search(r"\b" + match["y"] + r"\b", assignment[1]),
            "Floating overwrite reads the old carrier")
    require(all(len(re.findall(r"\b"+local+r"\b", view[:match.start()])) == 1
                for local in fields[:2]), "Initial carrier has earlier uses")
    for local, stop in ((match["x"], len(view)), (match["y"], overwrite)):
        for use in re.finditer(r"\b"+local+r"\b", view[match.end():stop]):
            at = match.end() + use.end()
            require(re.match(r"\s*=(?!=)", view[at:]),
                    "Reused storage remains live")
    labels = {label[1]: label.start() for label in re.finditer(
        r"(?m)^[ \t]*([A-Za-z_]\w*)\s*:(?!:)", view)}
    for jump in re.finditer(r"\bgoto\s+([A-Za-z_]\w*)\s*;", view[:overwrite]):
        require(jump[1] in labels and labels[jump[1]] < overwrite,
                "A jump bypasses the floating overwrite")
    sites = list(seed.call_sites(body, match["callee"]))
    require(len(sites) == 2 and all(not site[2] and site[3] for site in sites),
            "Height call count or operands differ")
    first, second = sites
    require(match.start() < first[0] < match.end() <= second[0] and
            depth(match.start()) == 1, "Unexpected source height call order")
    return dict(start=match.start(), end=match.end(), second=list(second[:2]),
                incoming=incoming, low=match["low"], high=match["high"],
                bridge=match["bridge"], result=match["result"], name=match["callee"],
                callee=int(match["callee"][4:],16))


def predecessor(graph, at):
    parents = graph["pred"][at]
    require(len(parents) == 1, "Alternate call setup ingress")
    ins = graph["nodes"][next(iter(parents))]
    require(ins.address + ins.size == at and graph["edges"][ins.address] == [at],
            "Nonlinear call setup")
    return ins


def first_coordinate(graph, call):
    """Evaluate the bounded integer stores and stack pointer passed to the call."""
    path = []
    at = call
    for _ in range(16):
        ins = predecessor(graph, at)
        path.append(ins)
        at = ins.address
        if sum(op.mnemonic == "movsx" for op in path) == 2:
            break
    require(sum(op.mnemonic == "movsx" for op in path) == 2,
            "Missing signed coordinate loads")
    regs = {}
    writes = []
    sp = 0
    pointer = None

    def frame(op, width):
        require(op.type == X.X86_OP_MEM and op.size == width and
                op.mem.base == X.X86_REG_ESP and not op.mem.index and not op.mem.segment,
                "Unproved frame operand")
        return sp + op.mem.disp

    for ins in reversed(path):
        op, *rest = ins.operands
        require(op.type != X.X86_OP_REG or op.reg != X.X86_REG_ESP,
                "Explicit stack pointer change in coordinate setup")
        if ins.mnemonic == "movsx":
            require(sp == 0 and op.type == X.X86_OP_REG and op.size == 4,
                    "Invalid signed word destination")
            regs[op.reg] = ("word", frame(rest[0],2))
        elif ins.mnemonic in ("shl", "add"):
            require(op.type == X.X86_OP_REG and op.size == 4 and rest[0].type == X.X86_OP_IMM,
                    "Unknown coordinate arithmetic")
            value = regs.get(op.reg)
            if ins.mnemonic == "shl":
                require(value and value[0] == "word" and rest[0].imm == 8,
                        "Coordinate scale differs")
                regs[op.reg] = ("scaled", value[1])
            else:
                require(value and value[0] == "scaled" and rest[0].imm == 128,
                        "Coordinate bias differs")
                regs[op.reg] = ("center", value[1])
        elif ins.mnemonic == "lea":
            require(op.type == X.X86_OP_REG and op.size == 4, "Unknown frame address")
            regs[op.reg] = ("frame", frame(rest[0],4))
        elif ins.mnemonic == "push":
            require(pointer is None and op.type == X.X86_OP_REG and op.size == 4,
                    "Unexpected coordinate argument count")
            pointer = regs.get(op.reg)
            require(pointer and pointer[0] == "frame" and pointer[1] >= 0,
                    "Missing coordinate frame pointer")
            sp -= 4
        elif ins.mnemonic == "mov":
            value = rest[0]
            require(value.size == 4 and value.type in (X.X86_OP_IMM, X.X86_OP_REG),
                    "Unsupported coordinate store value")
            origin = ("imm", value.imm) if value.type == X.X86_OP_IMM else regs.get(value.reg)
            require(origin is not None, "Unbound coordinate value")
            if op.type == X.X86_OP_MEM:
                writes.append((frame(op,4), origin))
            else:
                require(op.type == X.X86_OP_REG and op.size == 4 and
                        op.reg != X.X86_REG_ESP, "Unexpected coordinate register write")
                regs[op.reg] = origin
        else:
            require(False, "Unexpected coordinate setup instruction")
    require(pointer is not None and sp == -4 and len(writes) == 3,
            "Incomplete coordinate packet")
    base = pointer[1]
    require([at for at, _ in writes] == [base+4,base,base+8] and
            writes[0][1][0] == writes[1][1][0] == "center" and
            writes[0][1][1] == writes[1][1][1] + 2 and writes[2][1] == ("imm",0) and
            writes[1][1][1] + 4 <= base, "Coordinate order, width or value differs")
    receiver = regs.get(X.X86_REG_ECX)
    require(receiver and receiver[0] == "imm" and receiver[1] > 0,
            "Unknown height receiver")
    return receiver[1], writes


def native_binding(graph, callee):
    calls = [at for at, ins in graph["nodes"].items() if ins.mnemonic in ("call", "jmp")
             and ins.operands[0].type == X.X86_OP_IMM and ins.operands[0].imm == callee]
    require(len(calls) == 2 and all(graph["nodes"][at].mnemonic == "call" for at in calls),
            "Expected two direct native height calls")
    first, second = sorted(calls)
    receiver, writes = first_coordinate(graph, first)
    setup = predecessor(graph, second)
    require(setup.mnemonic == "mov" and setup.operands[0].type == X.X86_OP_REG and
            setup.operands[0].reg == X.X86_REG_ECX and setup.operands[1].type == X.X86_OP_IMM and
            setup.operands[1].imm == receiver, "Height receivers differ")
    pushed = predecessor(graph, setup.address)
    require(pushed.mnemonic == "push" and pushed.operands[0].type == X.X86_OP_REG and
            pushed.operands[0].size == 4 and
            incoming_registers(graph, pushed.address).get(pushed.operands[0].reg) == 0,
            "Second height argument is not the original incoming coordinate")
    return dict(receiver=receiver, calls=[first,second], writes=writes,
                second_origin="incoming ECX")


def accepted_height(repository, audit, inventory, callee):
    import catalog_io
    catalog = catalog_io.load(catalog_io.locate(repository), references=False)
    rows = [row for row in catalog["regions"] if row["entry"] == callee]
    require(len(rows) == 1, "Height routine is not uniquely accepted")
    row = rows[0]
    require(row["symbol"] == "?Get_Height_GL@MapClass@@QAEHABUCoordStruct@@@Z",
            "Height interface differs")
    source = audit.read(row["source"], row["source_sha256"]).decode("utf-8")
    require(re.search(r"(?m)^int MapClass::Get_Height_GL\(CoordStruct const &\w+\)",source),
            "Height source signature differs")
    native = guarded_graph(audit,inventory,callee)
    returns = [ins for ins in native["nodes"].values() if ins.mnemonic == "ret"]
    require(native["body_sha256"] == row["body_sha256"] and
            len(native["raw"]) == row["body_bytes"] and returns and
            all(len(i.operands) == 1 and i.operands[0].imm == 4 for i in returns),
            "Unproved height body or cleanup")
    return row["source_sha256"], row["body_sha256"]


def render(body, binding, proof):
    name = "height_coord"
    while re.search(r"\b"+name+r"\b",body):
        name += "_"
    receiver = "&DAT_%08x" % proof["receiver"]
    start, end = binding["second"]
    body = (body[:start] + "%s(%s, %s)" %
            (binding["name"],receiver,binding["incoming"]) + body[end:])
    replacement = (
        "{\n    CoordStruct %s;\n    %s.Y = %s * 0x100 + 0x80;\n"
        "    %s.X = %s * 0x100 + 0x80;\n    %s.Z = 0;\n"
        "    %s\n    %s = %s(%s, &%s);\n}"
        % (name,name,binding["high"],name,binding["low"],name,
           binding["bridge"],binding["result"],binding["name"],receiver,name)
    )
    return body[:binding["start"]] + replacement + body[binding["end"]:]


def optional_height_coordinate(body, frozen, params, conv, entry, inventory, repository, seed):
    if conv != "__fastcall" or "(double)(ulonglong)" not in frozen:
        return body, {}, [], []
    try:
        binding = source_binding(frozen, params, seed)
        stop = binding["second"][1]
        require(body[:stop] == frozen[:stop], "Another repair changed the height source span")
        repository = Path(repository)
        evidence = (repository / "private/original-target-20261020/inventory").resolve()
        directory = next((p for p in inventory.passes
                          if (p / "decompiled" / ("%08x.c" % entry)).is_file()),None)
        require(directory is not None and directory.resolve().is_relative_to(evidence),
                "Missing local frozen source")
        original = inventory.decompiled(entry)
        _, original_conv, _, original_params, span = seed.parse_header(original)
        require((original_conv, original_params) == (conv, params) and
                frozen == seed.nonnegative_tests(seed.reindent(original[span[1]:].strip())),
                "Source differs from frozen export")
        from verify_manifest import Audit
        audit = Audit(repository,"private/original-target-20261020/inputs/game.exe")
        proof = native_binding(guarded_graph(audit,inventory,entry),binding["callee"])
        accepted_height(repository,audit,inventory,binding["callee"])
    except (ValueError, TypeError, KeyError, OSError):
        return body, {}, [], []
    return (render(body,binding,proof),
            {binding["callee"]: ("int","__thiscall",["void *this_", "CoordStruct const *coord"])},
            ['#include "shared/coord.hpp"'],
            ["//   Scratch hypothesis: native integer XYZ and forwarded incoming coordinate at the two height calls."])
