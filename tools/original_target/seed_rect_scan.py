"""Split a rectangle argument home reused by a packed cell scan."""

import re
from pathlib import Path

from seed_callee_abi import reachable
from seed_capture_receivers import Receivers
from seed_frame_equations import AffineFrames
from seed_guarded_cfg import CFGRefusal, X, guarded_graph, require


SETUP = re.compile(
    r"(?m)^(?P<indent> +)(?P<base>\w+)\._2_2_ = (?P<y>\(short\)[^;]+);\n"
    r" +(?P=base)\._0_2_ = (?P<x>\w+) \+ -2;\n"
    r" +(?P<rows>\w+) = (?P<carrier>\w+)\[3\] / 0xf \+ 0x11;\n"
    r" +(?P<index>\w+) = 0;\n"
    r" +(?P<columns>\w+) = (?P=carrier)\[2\] / 0x3c \+ 4;\n"
    r" +(?P=carrier)\._0_2_ = (?P=x);\n"
    r" +(?P=carrier)\._2_2_ = (?P=base)\._2_2_;"
)
ROW = re.compile(
    r"(?m)^(?P<indent> +)(?P<carrier>\w+)\._0_2_ = "
    r"\(short\)(?P<step>\w+) \+ \(short\)(?P<base>\w+);\n"
    r" +(?P=carrier)\._2_2_ = (?P=base)\._2_2_ \+ (?P=step)\._2_2_;\n"
    r" +(?P<count>\w+) = (?P<columns>\w+);"
)
MAP_CALL = re.compile(r"\b(FUN_[0-9a-f]{8})\((&stack0x[0-9a-f]{8})\)")


def source_binding(body, params, seed):
    view = seed.code_view(body)
    require(not re.search(r"#|\b(?:goto|break|continue|return\s+\w+)\b", view),
            "Unsupported rectangle-scan control or source scope")
    require(params[1:] == ["int unused_fastcall_register_1", "int unused_0", "int unused_1"],
            "The expected two omitted stack arguments are absent")
    setups, rows, calls = list(SETUP.finditer(view)), list(ROW.finditer(view)), list(MAP_CALL.finditer(view))
    require(len(setups) == len(rows) == 1 and len(calls) == 2, "Ambiguous cell scan source")
    setup, row = setups[0], rows[0]
    first, second = calls
    require(setup.end() < row.start() < first.start() < second.start() and
            all(setup[k] == row[k] for k in ("carrier", "base", "columns")) and
            first[2] == second[2] and first[1] != second[1], "Cell scan source does not bind")
    carrier = setup["carrier"]
    clip = "unaff_retaddr"
    decl = "undefined4 " + carrier + ";"
    clip_decl = "int *" + clip + ";"
    require(view.count(decl) == view.count(clip_decl) == 1 and view.count(first[2]) == 2 and
            len(re.findall(r"\b" + carrier + r"\b", view)) == 9 and
            len(re.findall(r"\b" + clip + r"\b", view)) == 7 and
            view.count("*" + carrier) == view.count(carrier + "[1]") == 1,
            "Rectangle carrier has additional uses or declarations")
    require(re.search(r"\*unaff_retaddr <= (\w+)\)\s*&&\s*\(\1 < unaff_retaddr\[2\] \+ \*unaff_retaddr\)", view) and
            re.search(r"unaff_retaddr\[1\] <= (\w+)\)?\s*&&\s*\(\1 < unaff_retaddr\[3\] \+ unaff_retaddr\[1\]\)", view),
            "Clip does not have the expected four independent bounds")
    latch = re.compile(r"(?m)^(?P<indent> +)" + row["count"] + r" = " + row["count"] +
                       r" \+ -1;\n +\} while \(" + row["count"] + r" != 0\);")
    latches = list(latch.finditer(view))
    require(len(latches) == 1 and second.end() < latches[0].start(), "No unique source scan latch")
    require(len(re.findall(r"\b"+setup["base"]+r"\b", view[setup.end():])) == 2,
            "The packed base has another live use")
    for match in (setup, row, first, second, latches[0]):
        require(match.group() == body[match.start():match.end()], "Scan source is masked")
    for token in (carrier, clip, first[2]):
        require(view.count(token) == body.count(token), "Scan token appears in masked source")
    fresh = ("scan_area", "scan_clip", "scan_origin", "scan_base", "row_cell", "scan_cell", "area_height", "area_width")
    require(not any(re.search(r"\b"+name+r"\b", view) for name in fresh), "Scan local name collision")
    return setup, row, calls, latches[0]


def native_binding(repository, audit, inventory, entry, callees, catalog):
    graph = guarded_graph(audit, inventory, entry)
    nodes, pred, edges = graph["nodes"], graph["pred"], graph["edges"]
    binding = Receivers(repository, catalog, audit, inventory, graph, None)
    pops = {}
    sites = {callee: [] for callee in callees}
    for at, ins in nodes.items():
        if ins.mnemonic != "call" or ins.operands[0].type != X.X86_OP_IMM:
            continue
        target = ins.operands[0].imm
        if target in sites:
            sites[target].append(at)
        try:
            pops[at] = binding.cleanup(target)
        except CFGRefusal:
            pass
    frames = AffineFrames(graph, pops)
    for callee in callees:
        require(len(sites[callee]) == 1 and pops.get(sites[callee][0]) == 4,
                "Map cell call is not unique or lacks DWORD cleanup")
        row = binding.accepted(callee)
        require("ABVCell@@" in row["symbol"] or "ABUCell@@" in row["symbol"],
                "Map call lacks a pinned const Cell argument")

    def trace(at, shape):
        for pattern in reversed(shape):
            require(len(pred[at]) == 1, "Nonlinear native scan window")
            at, = pred[at]
            ins = nodes[at]
            require(re.fullmatch(pattern, ins.mnemonic + " " + ins.op_str),
                    "Native scan window differs")
        return at

    first, second = (sites[target][0] for target in callees)
    left = trace(first, [r"lea eax, \[esp \+ 0x10\]", "mov ecx, 0x[0-9a-f]+", "push eax"])
    right = trace(second, [r"lea ecx, \[esp \+ 0x10\]", "push ecx", "mov ecx, 0x[0-9a-f]+"])
    require(second not in reachable(edges, [graph["entry"]], blocked=[first]),
            "Lookup can bypass the scan predicate")
    require(nodes[left + nodes[left].size].operands[1].imm ==
            nodes[next(iter(pred[second]))].operands[1].imm, "Cell receivers differ")

    def unique(instruction):
        sites = [at for at, ins in nodes.items() if ins.mnemonic + " " + ins.op_str == instruction]
        require(len(sites) == 1, "Ambiguous native scan instruction: " + instruction)
        return sites[0]

    area = unique("mov esi, dword ptr [esp + 0x5c]")
    clip = unique("mov ecx, dword ptr [esp + 0x60]")
    copy = unique("mov dword ptr [esp + 0x10], edx")
    read = trace(copy, [r"mov edx, dword ptr \[esp \+ 0x5c\]", "test esi, esi"])
    latch = unique("inc word ptr [esp + 0x10]")
    decrement = unique("dec word ptr [esp + 0x12]")
    require(trace(decrement, [r"inc word ptr \[esp \+ 0x10\]", r"mov eax, dword ptr \[esp \+ 0x18\]"]) == latch,
            "The two scan updates do not share a latch")
    require(copy < first < second < clip < latch and
            first not in reachable(edges, [graph["entry"]], blocked=[copy]),
            "The scan can execute before the packed copy")
    for at, displacement, offset in ((area, 0x5c, 4), (clip, 0x60, 8), (read, 0x5c, 4),
                                      (copy, 0x10, -72), (left, 0x10, -72), (right, 0x10, -72),
                                      (latch, 0x10, -72), (decrement, 0x12, -70)):
        require(frames.height(at) + displacement == offset, "Native scan storage differs")
    require({ins.operands[0].imm for ins in nodes.values() if ins.mnemonic == "ret" and ins.operands} == {8}
            and all(ins.operands for ins in nodes.values() if ins.mnemonic == "ret"),
            "Native scan does not pop exactly two arguments")
    # On the straight-line setup, bind all four signed DWORD loads to the
    # saved first argument pointer before the old argument slot is reused.
    area_reads = []
    visited = set()
    cursor = area
    while cursor != copy:
        require(cursor not in visited, "Area pointer read cycle")
        visited.add(cursor)
        next_sites = edges[cursor]
        require(len(next_sites) == 1, "Area inputs do not form a straight-line setup")
        cursor, = next_sites
        ins = nodes[cursor]
        for op in ins.operands:
            if op.type == X.X86_OP_MEM and op.mem.base == X.X86_REG_ESI:
                require(op.size == 4 and op.access == 1 and not op.mem.index,
                        "Area operand is not a DWORD read")
                area_reads.append(op.mem.disp)
        if len(area_reads) == 4:
            break
        require(X.X86_REG_ESI not in ins.regs_access()[1], "Area pointer is overwritten early")
    require(area_reads == [0, 4, 12, 8], "Area rectangle fields differ")
    clip_reads = []
    cursor = clip
    while len(clip_reads) < 4:
        successors = set(edges[cursor]) - {latch}
        require(len(successors) == 1,
                "Clip pointer leaves the bounds chain")
        following, = successors
        require(following > cursor, "Clip bounds do not proceed forward")
        cursor = following
        ins = nodes[cursor]
        for op in ins.operands:
            if op.type == X.X86_OP_MEM and op.mem.base == X.X86_REG_ECX:
                require(op.size == 4 and op.access == 1 and not op.mem.index,
                        "Clip operand is not a DWORD read")
                clip_reads.append(op.mem.disp)
        require(len(clip_reads) == 4 or X.X86_REG_ECX not in ins.regs_access()[1],
                "Clip pointer is overwritten early")
    require(clip_reads == [0, 8, 4, 12], "Clip rectangle fields differ")
    for operation, displacement in (("mov word ptr [esp + 0x5c], ax", 0x5c),
                                    ("mov word ptr [esp + 0x5e], ax", 0x5e)):
        writes = [at for at, ins in nodes.items()
                  if ins.mnemonic + " " + ins.op_str == operation and area < at < read]
        require(len(writes) >= 1 and all(frames.height(at) + displacement ==
                                         displacement - 0x58 for at in writes),
                "First argument home is not reused by the two WORD coordinates")
    require(first not in reachable(edges, edges[first], blocked=[latch, copy]),
            "An inner scan iteration bypasses the coordinate updates")
    # Until another row copy, the only explicit stack references overlapping
    # the scan cell must be its two bound addresses and the two WORD updates.
    live = reachable(edges, list(edges[copy]), blocked=[copy])
    for at in live:
        ins = nodes[at]
        for op in ins.operands:
            if op.type != X.X86_OP_MEM or op.mem.base != X.X86_REG_ESP:
                continue
            require(not op.mem.index and op.mem.segment in (0, X.X86_REG_SS),
                    "Unbound stack alias during scan")
            slot = frames.height(at) + op.mem.disp
            require(slot >= -68 or slot + op.size <= -72 or at in (left, right, latch, decrement),
                    "Another stack operation aliases the scan cell")
    return frames, (area, clip, read, copy, left, right, latch, decrement)


def render(body, binding):
    setup, row, calls, latch = binding
    i, j, k = setup["indent"], row["indent"], latch["indent"]
    first = [
        "Cell scan_origin;",
        "scan_origin.X = " + setup["x"] + ";",
        "const int area_height = scan_area->Height;",
        "const int area_width = scan_area->Width;",
        "scan_origin.Y = " + setup["y"] + ";",
        setup["rows"] + " = area_height / 15 + 17;",
        "Cell scan_base = scan_origin;",
        "scan_base.X -= 2;",
        setup["index"] + " = 0;",
        setup["columns"] + " = area_width / 60 + 4;",
    ]
    second = [
        "row_cell.X = (short)((short)" + row["step"] + " + scan_base.X);",
        "row_cell.Y = (short)(scan_base.Y + " + row["step"] + "._2_2_);",
        "Cell scan_cell = row_cell;",
        row["count"] + " = " + row["columns"] + ";",
    ]
    body = (body[:latch.start()] + k+"++scan_cell.X;\n"+k+"--scan_cell.Y;\n" + body[latch.start():])
    body = body[:row.start()] + "\n".join(j+line for line in second) + body[row.end():]
    body = body[:setup.start()] + "\n".join(i+line for line in first) + body[setup.end():]
    carrier = setup["carrier"]
    return (body.replace("undefined4 "+carrier+";", "Cell row_cell;")
            .replace("    int *unaff_retaddr;\n", "")
            .replace("*"+carrier, "scan_area->X").replace(carrier+"[1]", "scan_area->Y")
            .replace("*unaff_retaddr", "scan_clip->X").replace("unaff_retaddr[1]", "scan_clip->Y")
            .replace("unaff_retaddr[2]", "scan_clip->Width").replace("unaff_retaddr[3]", "scan_clip->Height")
            .replace(calls[0][2], "&scan_cell.X"))


def optional_rect_scan(body, frozen, params, conv, entry, inventory, repository, seed):
    if conv != "__fastcall" or "unaff_retaddr[" not in frozen or "._2_2_" not in frozen:
        return body, params, [], []
    try:
        require(body == frozen, "Another seed repair modified the scan")
        proof = source_binding(frozen, params, seed)
        repository = Path(repository)
        from verify_manifest import Audit
        import catalog_io
        source = inventory.decompiled(entry)
        _, _, _, _, span = seed.parse_header(source)
        require(frozen == seed.nonnegative_tests(seed.reindent(source[span[1]:].strip())),
                "Scan source differs from the frozen export")
        audit = Audit(repository, "private/original-target-20261020/inputs/game.exe")
        catalog = catalog_io.load(catalog_io.locate(repository), references=False)
        native_binding(repository, audit, inventory, entry,
                       tuple(int(call[1][4:], 16) for call in proof[2]), catalog)
    except (ValueError, TypeError, KeyError, OSError):
        return body, params, [], []
    return (render(body, proof), params[:2]+["Rect const *scan_area", "Rect const *scan_clip"],
            ['#include "shared/coord.hpp"', '#include "shared/rect.hpp"'],
            ["//   Scratch hypothesis: split two rectangle inputs and recover the native packed cell scan."])
