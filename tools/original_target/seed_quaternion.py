"""Render the independently bounded Quaternion seed misidentified as CRect."""
import hashlib

from seed_guarded_cfg import X, guarded_graph, require

ENTRY = 0x622290
RAW_SHA256 = "ff659d681db2bd56e967672794d386f91ad2597e65b9047c54642e4de1153c7f"
BODY_SHA256 = "f2f29adf80282ce043cc5493e92b5aa05610efeaf58701c3bb72f734898ad797"

PROVIDERS = {
    0x622160: ("??0Quaternion@@QAE@MMMM@Z", 16),
    0x622400: ("??D@YI?AVQuaternion@@ABV0@0@Z", 4),
    0x6225f0: ("?Conjugate@@YI?AVQuaternion@@ABV1@@Z", 0),
}


def verify_native(inventory, repository):
    import catalog_io
    from seed_capture_receivers import Receivers
    from verify_manifest import Audit

    audit = Audit(repository, "private/original-target-20261020/inputs/game.exe")
    graph = guarded_graph(audit, inventory, ENTRY)
    require(graph["body_sha256"] == BODY_SHA256, "Frozen quaternion body differs")
    catalog = catalog_io.load(catalog_io.locate(repository), references=False)
    receivers = Receivers(repository, catalog, audit, inventory, graph, None)
    for target, (symbol, cleanup) in PROVIDERS.items():
        require(receivers.accepted(target)["symbol"] == symbol and receivers.cleanup(target) == cleanup,
                "Quaternion provider ABI differs")
    nodes = graph["nodes"]
    calls = {at: instruction.operands[0].imm for at, instruction in nodes.items()
             if instruction.mnemonic == "call" and instruction.operands[0].type == X.X86_OP_IMM}
    require(calls == {0x6222be: 0x6225f0, 0x6222cc: 0x622400, 0x622313: 0x622160,
                      0x622339: 0x6225f0, 0x622347: 0x622400}, "Quaternion call sequence differs")
    expected = {
        0x622294: ("mov", "esi, ecx"),
        0x622299: ("mov", "edi, dword ptr [esp + 0x6c]"),
        0x6222bc: ("mov", "edx, edi"),
        0x6222c3: ("push", "eax"),
        0x622337: ("mov", "edx, edi"),
        0x62233e: ("push", "eax"),
        0x6223bb: ("fcompp", ""),
        0x6223c7: ("test", "ah, 0x41"),
        0x6223cc: ("jne", "0x6223ec"),
        0x6223ec: ("mov", "eax, esi"),
        0x6223f3: ("ret", "4"),
    }
    require(all((nodes[at].mnemonic, nodes[at].op_str) == shape for at, shape in expected.items()),
            "Quaternion inputs, comparison or return differ")
    for at, field in ((0x6222d3, 0xc), (0x6222f1, 8), (0x6222fe, 4), (0x622307, 0)):
        operand = nodes[at].operands[0]
        require(nodes[at].mnemonic == "fld" and operand.type == X.X86_OP_MEM
                and operand.mem.base == X.X86_REG_ESI and not operand.mem.index
                and operand.mem.disp == field and operand.size == 4, "Quaternion constructor input differs")
    require(all(nodes[at].mnemonic == "fchs" for at in (0x6222dd, 0x6222f9, 0x622302, 0x62230e)),
            "Quaternion constructor signs differ")
    require(all(nodes[at].mnemonic == "fstp" and nodes[at].op_str == "dword ptr [esp]"
                for at in (0x6222ee, 0x6222fb, 0x622304, 0x622310)), "Quaternion constructor stores differ")


def optional_quaternion_seed(raw, inventory, repository):
    if hashlib.sha256(raw.encode()).hexdigest() != RAW_SHA256:
        return None
    try:
        verify_native(inventory, repository)
    except (ValueError, KeyError, TypeError, OSError):
        return None
    return CANDIDATE


CANDIDATE = """#include "shared/quaternion.hpp"

Quaternion &Quaternion::Make_Closest(Quaternion const &target)
{
    Quaternion current = *this;
    Quaternion first = current * Conjugate(target);
    Quaternion opposite(-X, -Y, -Z, -W);
    Quaternion second = opposite * Conjugate(target);
    if (first.W * first.W + first.Z * first.Z + first.Y * first.Y + first.X * first.X >
        second.W * second.W + second.Z * second.Z + second.Y * second.Y + second.X * second.X) {
        X = -X;
        Y = -Y;
        Z = -Z;
        W = -W;
    }
    return *this;
}
"""
