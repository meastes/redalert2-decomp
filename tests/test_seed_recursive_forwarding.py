from collections import defaultdict
import unittest
import struct

import capstone
import seed_candidate as seed
from seed_guarded_cfg import CFGRefusal
from seed_recursive_forwarding import native_control_lookup, native_site, source_control_id, source_site


NAME = "FUN_00001000"
PARAMS = ["HWND dialog", "int slot", "int entry", "int update"]
BODY = """{
    HWND control;
    int id;
    id = slot + 1;
    control = GetDlgItem(dialog,id);
    FUN_00001000(DAT_00002000,0);
}
"""


def graph(hex_bytes):
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True
    nodes = {ins.address: ins for ins in decoder.disasm(bytes.fromhex(hex_bytes), 0x1000)}
    edges = {}
    pred = defaultdict(set)
    for at, ins in nodes.items():
        dest = [] if ins.mnemonic == "ret" else [at + ins.size]
        edges[at] = dest
        for to in dest:
            pred[to].add(at)
    return {"entry": 0x1000, "nodes": nodes, "edges": edges, "pred": pred}


class SourceBinding(unittest.TestCase):
    def test_existing_id_and_stack_arguments(self):
        at, names, values = source_site(BODY, NAME, PARAMS, seed)
        self.assertEqual(names, ["dialog", "slot"])
        self.assertEqual(values, [("global", 0x2000), ("imm", 0)])
        self.assertEqual(BODY[at:at+12], "DAT_00002000")
        self.assertEqual(source_control_id(BODY, NAME, "dialog", PARAMS, seed), "id")

    def test_forwarded_argument_mutation_refuses(self):
        body = BODY.replace("id = slot + 1;", "slot++; id = slot + 1;")
        with self.assertRaises(CFGRefusal):
            source_site(body, NAME, PARAMS, seed)

    def test_indirect_argument_escape_refuses(self):
        body = BODY.replace("id = slot + 1;", "consume(&dialog); id = slot + 1;")
        with self.assertRaises(CFGRefusal):
            source_site(body, NAME, PARAMS, seed)

    def test_id_mutation_after_lookup_refuses(self):
        body = BODY.replace("    FUN_", "    id += 1;\n    FUN_")
        with self.assertRaises(CFGRefusal):
            source_control_id(body, NAME, "dialog", PARAMS, seed)

    def test_label_can_bypass_lookup(self):
        body = BODY.replace("    FUN_", "resume:\n    FUN_")
        with self.assertRaises(CFGRefusal):
            source_control_id(body, NAME, "dialog", PARAMS, seed)

    def test_nested_control_lookup_cannot_bind(self):
        body = BODY.replace("    control = GetDlgItem(dialog,id);",
                            "    if (slot) { control = GetDlgItem(dialog,id); }")
        with self.assertRaises(CFGRefusal):
            source_control_id(body, NAME, "dialog", PARAMS, seed)

    def test_stack_expression_is_not_guessed(self):
        with self.assertRaises(CFGRefusal):
            source_site(BODY.replace("DAT_00002000","DAT_00002000 + 1"), NAME, PARAMS, seed)


class NativeBinding(unittest.TestCase):
    def test_direct_forwarding_and_two_stack_arguments(self):
        g = graph("6a00 ff3500200000 e8f3ffffff c20800")
        self.assertEqual(native_site(g, [("global", 0x2000), ("imm", 0)]), 0x1008)

    def test_partial_register_clobber_refuses(self):
        g = graph("b201 6a00 ff3500200000 e8f1ffffff c20800")
        with self.assertRaises(CFGRefusal):
            native_site(g, [("global", 0x2000), ("imm", 0)])

    def test_wrong_global_or_cleanup_refuses(self):
        for code in ("6a00 ff3504200000 e8f3ffffff c20800",
                     "6a00 ff3500200000 e8f3ffffff c20400"):
            with self.assertRaises(CFGRefusal):
                native_site(graph(code), [("global", 0x2000), ("imm", 0)])

    def test_lookup_anchors_computed_second_operand(self):
        prefix = bytes.fromhex("57 56 89cf 89d6 83c601 56 57 ff1500300000 "
                               "89f9 6a00 ff3500200000 89f2")
        displacement = struct.pack("<i", -(len(prefix) + 5))
        g = graph((prefix + b"\xe8" + displacement + bytes.fromhex("5e 5f c20800")).hex())
        anchor = native_control_lookup(g, 0x3000)
        self.assertEqual(anchor[0], 0x100b)
        self.assertEqual(native_site(g, [("global", 0x2000), ("imm", 0)], anchor),
                         0x1000 + len(prefix))

    def test_computed_id_clobber_after_lookup_refuses(self):
        prefix = bytes.fromhex("57 56 89cf 89d6 83c601 56 57 ff1500300000 "
                               "89f9 6a00 ff3500200000 46 89f2")
        displacement = struct.pack("<i", -(len(prefix) + 5))
        g = graph((prefix + b"\xe8" + displacement + bytes.fromhex("5e 5f c20800")).hex())
        with self.assertRaises(CFGRefusal):
            native_control_lookup(g, 0x3000)

    def test_wrong_lookup_import_refuses(self):
        g = graph("6a00 ff3500200000 e8f3ffffff c20800")
        with self.assertRaises(CFGRefusal):
            native_control_lookup(g, 0x3000)
