import unittest

import capstone

from seed_frame_equations import AffineFrames, zero_cleanup_columns


class SeedZeroCleanupTest(unittest.TestCase):
    def test_mixed_sign_and_nonzero_rows_leave_variables_unresolved(self):
        rows = [(0, [1, -1, 0]), (1, [0, 1, 4])]
        self.assertEqual(zero_cleanup_columns(rows), set())

    def test_zero_row_exposes_another_zero_row(self):
        rows = [(0, [1, -1, 0, 0]), (1, [0, 1, 2, 0])]
        self.assertEqual(zero_cleanup_columns(rows), {0, 1, 2})

    def test_negative_coefficients_also_force_zero_cleanup(self):
        self.assertEqual(zero_cleanup_columns([(0, [-1, -2, 0])]), {0, 1})

    def test_two_calls_between_balanced_entry_and_return_pop_nothing(self):
        decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        decoder.detail = True
        instructions = list(decoder.disasm(bytes.fromhex("e810000000 e820000000 c3"), 0x1000))
        first, second, ret = (ins.address for ins in instructions)
        graph = {
            "entry": first,
            "nodes": {ins.address: ins for ins in instructions},
            "edges": {first: [second], second: [ret], ret: []},
        }
        frames = AffineFrames(graph, {})
        self.assertEqual(frames.solved_pops, {first: 0, second: 0})
        self.assertEqual(frames.height(second), 0)

    def test_two_calls_can_share_nonzero_cleanup_without_fixed_widths(self):
        decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        decoder.detail = True
        instructions = list(decoder.disasm(bytes.fromhex("50 e810000000 e820000000 c3"), 0x1000))
        push, first, second, ret = (ins.address for ins in instructions)
        graph = {
            "entry": push,
            "nodes": {ins.address: ins for ins in instructions},
            "edges": {push: [first], first: [second], second: [ret], ret: []},
        }
        frames = AffineFrames(graph, {})
        self.assertEqual(frames.solved_pops, {})
        self.assertFalse(frames.address(second)["proved"])
        self.assertEqual(frames.height(ret), 0)
