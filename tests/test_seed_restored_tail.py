import unittest

import capstone

from seed_capture_receivers import restored_tail_target
from seed_guarded_cfg import CFGRefusal


class RestoredTailTests(unittest.TestCase):
    def setUp(self):
        self.decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        self.decoder.detail = True

    def target(self, body, cleanup=lambda address: 0):
        return restored_tail_target(self.decoder, 0x1000, bytes.fromhex(body), cleanup)

    def test_caller_cleaned_argument(self):
        self.assertEqual(self.target("83ec08 dd1c24 e810000000 83c408 e910000000"), 0x1023)

    def test_callee_cleaned_argument(self):
        self.assertEqual(self.target("83ec04 e810000000 e910000000", lambda address: 4), 0x101d)

    def test_tail_with_live_stack_storage_refuses(self):
        with self.assertRaisesRegex(CFGRefusal, "restore incoming ESP"):
            self.target("83ec08 e910000000")

    def test_write_to_return_address_refuses(self):
        with self.assertRaisesRegex(CFGRefusal, "beyond its local stack frame"):
            self.target("890424 e910000000")

    def test_indirect_nested_cleanup_refuses(self):
        with self.assertRaisesRegex(CFGRefusal, "Unknown nested tail cleanup"):
            self.target("ffd0 e910000000")

    def test_internal_branch_refuses(self):
        with self.assertRaisesRegex(CFGRefusal, "Nonlinear"):
            self.target("7500 e910000000")

    def test_tail_cannot_branch_back_into_body(self):
        with self.assertRaisesRegex(CFGRefusal, "inside the body"):
            self.target("90 e9faffffff")

    def test_dynamic_stack_adjustment_refuses(self):
        with self.assertRaisesRegex(CFGRefusal, "Unmodeled tail stack adjustment"):
            self.target("89c4 e910000000")

    def test_nested_call_cannot_enter_the_body(self):
        with self.assertRaisesRegex(CFGRefusal, "enters the tail body"):
            self.target("83ec08 e800000000 83c408 e910000000")
