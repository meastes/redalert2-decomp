"""Scalar pointer casts in byte-store operands must not look like calls."""
from pathlib import Path
import sys
import unittest
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools/original_target'))
import seed_candidate as seed

class PointerCastOperandTests(unittest.TestCase):
    def convert(self, value):
        return seed.byte_update_artifacts('{\n  uint slot;\n  slot = CONCAT31(slot._1_3_,'+value+');\n}')

    def test_parenthesized_scalar_pointer_load_preserves_one_read(self):
        for kind in ('undefined1', 'byte', 'uchar', 'char', 'ushort', 'short', 'uint', 'int', 'unsigned int'):
            with self.subTest(kind=kind):
                value = '*('+kind+' *)(base + 0x10b)'
                text = self.convert(value)
                self.assertIn('*(uchar *)&(slot) = (uchar)('+value+');', text)
                self.assertEqual(text.count(value), 1)
                self.assertNotIn('._1_3_', text)

    def test_effectful_addresses_and_indirect_calls_still_refuse(self):
        for value in ('*(byte *)(base++)', '*(byte *)(get())',
                      '*(byte *)((base = other))', '*(byte *)((base, other))',
                      '(callback)(base)', '(*(callback))(base)',
                      '((byte (*)(int))callback)(base)'):
            with self.subTest(value=value):
                self.assertIn('CONCAT31', self.convert(value))

    def test_unknown_qualified_and_multilevel_pointer_casts_refuse(self):
        for kind in ('Unknown *', 'const byte *', 'volatile byte *', 'byte **'):
            with self.subTest(kind=kind):
                self.assertIn('CONCAT31', self.convert('*('+kind+')(base)'))

    def test_complete_seed_has_no_invented_three_byte_read(self):
        class Inventory:
            passes = []
            def decompiled(self, entry):
                return ('int __cdecl FUN_00401000(int base)\n{\n  uint slot;\n'
                        '  slot = CONCAT31(slot._1_3_,*(undefined1 *)(base + 0x10b));\n'
                        '  return slot;\n}\n')
            def function(self, entry):
                return None, {'ranges': []}
        text = seed.seed(0x401000, Inventory(), {})
        self.assertIn('*(uchar *)&(slot)', text)
        self.assertNotIn('._1_3_', text)
        self.assertNotIn('CONCAT31', text)

if __name__ == '__main__':
    unittest.main()
