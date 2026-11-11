import struct
import unittest
from collections import defaultdict

import capstone

import seed_candidate as seed
from seed_guarded_cfg import CFGRefusal
from seed_recursive_diamond import native_binding, render, source_binding


NAME = "FUN_00001000"
PARAMS = ["int *coord", "int damage", "int *unit", "int warhead", "char flag"]
BODY = """{
    int *saved;
    void *object;
    saved = coord;
    if (object != (void *)0x0) {
        FUN_00004000(*(uint *)(DAT_00002000 + 0x5c), saved, 0);
    }
    FUN_00001000(0,*(undefined4 *)(DAT_00002000 + 0x70),1);
}
"""


def fixture(other_offset=0x34, damage=0xc, predicate=0x74, extra_load=False):
    code = bytearray(b"\x85\xc0" + bytes([predicate, 0]))

    def call(target):
        code.extend(b"\xe8" + struct.pack("<i", target - (0x1000 + len(code) + 5)))

    code.extend(bytes.fromhex("8b0d00200000 8b7c2434 6a00 57 8b495c 51 89c1"))
    call(0x4000)
    jump = len(code)
    code.extend(b"\xeb\0")
    code[3] = len(code) - 4
    code.extend(bytes.fromhex("8b7c24") + bytes([other_offset]))
    code[jump+1] = len(code) - (jump + 2)
    code.extend(bytes.fromhex("a100200000 6a01 8b4870 8b50") + bytes([damage]))
    if extra_load:
        code.extend(bytes.fromhex("8b5874"))
    code.extend(bytes.fromhex("51 6a00 89f9"))
    call(0x1000)
    code.extend(bytes.fromhex("c20c00"))
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True
    nodes = {i.address: i for i in decoder.disasm(code, 0x1000)}
    edges = {}
    pred = defaultdict(set)
    for at, ins in nodes.items():
        next_at = at + ins.size
        if ins.mnemonic == "ret":
            dest = []
        elif ins.mnemonic == "jmp":
            dest = [ins.operands[0].imm]
        elif ins.mnemonic in ("je", "jne"):
            dest = [ins.operands[0].imm, next_at]
        else:
            dest = [next_at]
        edges[at] = dest
        for to in dest:
            pred[to].add(at)
    return {"entry": 0x1000, "nodes": nodes, "edges": edges, "pred": pred}


class RecursiveDiamond(unittest.TestCase):
    def bind(self, body=BODY):
        return source_binding(body, NAME, PARAMS, seed)

    def test_shared_frame_home_and_ordered_global_reads(self):
        proof = native_binding(fixture(), self.bind())
        self.assertEqual(proof["coordinate_frame_offset"], 0x34)
        self.assertEqual(proof["damage_offset"], 0xc)
        self.assertEqual(proof["reads"], [
            ("global", 0x2000), ("field", 0x2000, 0x70), ("field", 0x2000, 0xc)
        ])

    def test_distinct_frame_homes_refuse(self):
        with self.assertRaises(CFGRefusal):
            native_binding(fixture(other_offset=0x38), self.bind())

    def test_reversed_constructor_predicate_refuses(self):
        with self.assertRaises(CFGRefusal):
            native_binding(fixture(predicate=0x75), self.bind())

    def test_mismatched_native_stack_field_refuses(self):
        with self.assertRaises(CFGRefusal):
            native_binding(fixture(), self.bind(BODY.replace("+ 0x70", "+ 0x74")))

    def test_unrepresented_extra_field_read_refuses(self):
        with self.assertRaises(CFGRefusal):
            native_binding(fixture(extra_load=True), self.bind())

    def test_coordinate_home_escape_refuses(self):
        with self.assertRaises(CFGRefusal):
            self.bind(BODY.replace("saved = coord;", "saved = coord; receive(&(saved));"))

    def test_unrepresented_source_effect_refuses(self):
        with self.assertRaises(CFGRefusal):
            self.bind(BODY.replace("saved, 0);", "saved, 0); changed();"))

    def test_render_keeps_one_base_read_and_native_field_order(self):
        binding = self.bind()
        changed = render(BODY, NAME, binding, native_binding(fixture(), binding))
        tail = changed[binding["start"]:]
        self.assertEqual(tail.count("DAT_00002000"), 1)
        self.assertLess(tail.index("+ 0x70"), tail.index("+ 0xc"))
        self.assertIn("FUN_00001000(saved,", tail)
