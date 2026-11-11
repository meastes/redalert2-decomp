from types import SimpleNamespace
import struct
import unittest

import capstone
from seed_guarded_cfg import CFGRefusal, guarded_graph

ENTRY = 0x1000
TABLE = 0x1020


class EmbeddedSwitchTests(unittest.TestCase):
    def fixture(self, default=0x1010, end=0x10c5):
        code = bytearray(b"\x90" * (0x10c5 - ENTRY))
        code[:12] = b"\x83\xf8\x01\x77" + bytes([default - 0x1005]) + b"\xff\x24\x85" + struct.pack("<I", TABLE)
        for address in (0x1010, 0x10c3, 0x10c4):
            code[address - ENTRY] = 0xc3
        struct.pack_into("<II", code, TABLE - ENTRY, 0x10c3, 0x10c4)
        section = SimpleNamespace(VirtualAddress=ENTRY, SizeOfRawData=len(code), Misc_VirtualSize=len(code),
                                  Characteristics=0x60000020)
        decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        decoder.detail = True
        audit = SimpleNamespace(base=0, decoder=decoder, pe=SimpleNamespace(get_section_by_rva=lambda _: section),
                                original=lambda at, size: bytes(code[at - ENTRY:at - ENTRY + size]))
        span = dict(start=hex(ENTRY), end_inclusive=hex(end - 1), bytes_hex=code[:end - ENTRY].hex())
        inventory = SimpleNamespace(function=lambda _: (None, dict(contiguous=True, ranges=[span])))
        return audit, inventory

    def test_embedded_table_proves_all_destinations(self):
        proof = guarded_graph(*self.fixture(), ENTRY)
        self.assertEqual(proof["switches"][0]["targets"], [0x10c3, 0x10c4])
        self.assertEqual(proof["switches"][0]["literals"][0]["address"], TABLE)
        self.assertNotIn(TABLE, proof["nodes"])

    def test_reachable_instruction_in_literal_refuses(self):
        with self.assertRaisesRegex(CFGRefusal, "overlaps reachable instructions"):
            guarded_graph(*self.fixture(default=TABLE), ENTRY)

    def test_literal_crossing_the_body_end_refuses(self):
        with self.assertRaisesRegex(CFGRefusal, "Unknown switch data extent"):
            guarded_graph(*self.fixture(end=TABLE + 4), ENTRY)
