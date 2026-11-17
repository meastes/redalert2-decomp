from pathlib import Path
import sys
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parent.parent / "tools/original_target"))
import seed_capture_receivers as receiver


class ClassRecordTests(unittest.TestCase):
    def family(self, symbols, pops):
        machine = receiver.Receivers.__new__(receiver.Receivers)
        machine.audit = None
        machine.family_cache = {}
        rows = {index: dict(symbol=symbol, source="fixture.cpp", source_sha256="source", body_sha256="body")
                for index, symbol in enumerate(symbols)}
        machine.accepted = rows.__getitem__
        machine.cleanup = lambda entry: pops[entry]
        with patch.object(receiver, "physical_family", return_value=[dict(target=index) for index in rows]):
            return machine.family(("input", 0, "Owner"), 30)

    def test_class_record_return_has_the_same_hidden_pointer(self):
        proof = self.family(["?Get_Rect@Surface@@UAE?AVRect@@XZ"], [4])
        self.assertEqual(proof["record_result"], "Rect")
        self.assertEqual(proof["pop"], 4)

    def test_struct_record_return_is_preserved(self):
        proof = self.family(["?Center_Coord@ObjectClass@@UBE?AUCoordStruct@@XZ"], [4])
        self.assertEqual(proof["record_result"], "CoordStruct")

    def test_class_return_requires_verified_hidden_pointer_cleanup(self):
        with self.assertRaisesRegex(ValueError, "hidden result pointer"):
            self.family(["?Get_Rect@Surface@@UAE?AVRect@@XZ"], [0])

    def test_conflicting_record_types_keep_the_family_unclassified(self):
        proof = self.family(["?Get_Rect@Surface@@UAE?AVRect@@XZ", "?Get_Coord@Object@@UAE?AUCoordStruct@@XZ"], [4, 4])
        self.assertIsNone(proof["record_result"])

    def test_explicit_argument_method_is_not_a_zero_argument_record_getter(self):
        proof = self.family(["?Get_Rect@Surface@@UAE?AVRect@@H@Z"], [4])
        self.assertIsNone(proof["record_result"])


if __name__ == "__main__":
    unittest.main()
