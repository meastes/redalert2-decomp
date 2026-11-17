from collections import defaultdict
import unittest

import capstone

from seed_frame_equations import AffineFrames, aligned_frames
from seed_guarded_cfg import CFGRefusal


def graph(body):
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True
    nodes = {ins.address: ins for ins in decoder.disasm(bytes.fromhex(body), 0x1000)}
    edges, predecessors = {}, defaultdict(set)
    for at, ins in nodes.items():
        if ins.mnemonic == "ret":
            edges[at] = []
        elif ins.group(capstone.CS_GRP_JUMP):
            edges[at] = [ins.operands[0].imm]
            if ins.mnemonic != "jmp":
                edges[at].append(at + ins.size)
        else:
            edges[at] = [at + ins.size]
        for target in edges[at]:
            predecessors[target].add(at)
    return dict(entry=0x1000, nodes=nodes, edges=edges, pred=predecessors)


START = "55 8bec 83e4f8 83ec10 53 55"
RESTORE = "5d 5b 8be5 5d c3"


class AlignedFrameTests(unittest.TestCase):
    def test_relative_slot_across_two_returns(self):
        native = graph(START + " 85c0 7406 " + RESTORE + RESTORE)
        frames = aligned_frames(native, {})
        self.assertEqual(frames.graph["entry"], 0x1006)
        self.assertEqual(frames.height(0x100b), -24)
        self.assertEqual(frames.address(0x100b, 8)["constant"], "-16")
        self.assertNotIn(0x1000, frames.states)

    def test_known_call_cleanup_restores_the_saved_slot(self):
        native = graph(START + " 50 e800000000 " + RESTORE)
        self.assertEqual(aligned_frames(native, {0x100c: 4}).height(0x100c), -28)

    def test_unknown_cleanup_refuses_to_guess_restore_slot(self):
        native = graph(START + " 50 e800000000 " + RESTORE)
        with self.assertRaisesRegex(CFGRefusal, "restore slot is not proved"):
            aligned_frames(native, {})

    def test_unbalanced_saved_slot_refuses(self):
        native = graph(START + " 50 " + RESTORE)
        with self.assertRaisesRegex(CFGRefusal, "restore slot is not proved"):
            aligned_frames(native, {})

    def test_nonstandard_restore_order_refuses(self):
        native = graph(START + " 5b 5d 8be5 5d c3")
        with self.assertRaisesRegex(CFGRefusal, "restore order"):
            aligned_frames(native, {})

    def test_bypass_to_return_refuses(self):
        native = graph(START + " eb05 " + RESTORE)
        with self.assertRaises(CFGRefusal):
            aligned_frames(native, {})

    def test_wrong_alignment_refuses(self):
        native = graph(START.replace("83e4f8", "83e4fc") + " " + RESTORE)
        with self.assertRaisesRegex(CFGRefusal, "alignment"):
            aligned_frames(native, {})

    def test_disjoint_addresses_uses_symbolic_sign(self):
        native = graph("50 e800000000 e800000000 c3")
        frames = AffineFrames(native, {})
        self.assertFalse(frames.address(0x1006)["proved"])
        self.assertTrue(frames.disjoint_addresses(0x1006, 8, 4, 0x1001, 0, 4))
        self.assertTrue(frames.disjoint_addresses(0x1001, 0, 4, 0x1006, 8, 4))
        self.assertFalse(frames.disjoint_addresses(0x1006, 0, 4, 0x1001, 0, 4))
