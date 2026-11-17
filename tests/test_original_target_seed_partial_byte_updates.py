"""Preserved byte-two updates: unsupported uint24 lvalues must not be widened."""
from pathlib import Path
import sys
import unittest
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools/original_target'))
import seed_candidate as seed

class PartialByteUpdateTests(unittest.TestCase):
    def body(self, statement, declarations='uint slot;', params=()):
        text='{\n  '+declarations+'\n  '+statement+'\n}'
        return text,seed.partial_byte_update_artifacts(text,params)

    def test_self_preserving_concat_writes_only_byte_two(self):
        _,text=self.body('slot._0_3_ = CONCAT12(flag,(ushort)slot);')
        self.assertIn('((uchar *)&(slot))[2] = (uchar)(flag);',text)
        self.assertNotIn('CONCAT12',text)
        self.assertNotIn('._0_3_',text)
        self.assertNotIn('uint *',text)

    def test_truncated_low_word_clears_only_byte_two(self):
        _,text=self.body('slot._0_3_ = (uint3)(ushort)slot;')
        self.assertIn('((uchar *)&(slot))[2] = (uchar)(0);',text)
        self.assertNotIn('(ushort)slot',text)

    def test_uninitialized_other_bytes_are_not_read(self):
        _,text=self.body('slot._0_3_ = CONCAT12(1,(ushort)slot);')
        self.assertEqual(text.count('slot'),2)  # declaration and address, no value read
        self.assertNotIn('(ushort)',text)
        self.assertNotIn('uint3',text)

    def test_mutable_four_byte_slots_and_pointer_representations(self):
        for kind in ('uint','int','undefined4','unsigned long','HWND','byte *','Unknown *','undefined4 **'):
            with self.subTest(kind=kind):
                _,text=self.body('slot._0_3_ = CONCAT12(flag,(ushort)slot);',kind+' slot;')
                self.assertIn('((uchar *)&(slot))[2]',text)

    def test_declared_parameter_is_storage(self):
        _,text=self.body('slot._0_3_ = (uint3)(ushort)slot;','',('uint slot',))
        self.assertIn('((uchar *)&(slot))[2]',text)

    def test_comparison_and_pointer_load_are_evaluated_once(self):
        value='*(undefined1 *)(base + 0x10b) != 0'
        _,text=self.body('slot._0_3_ = CONCAT12('+value+',(ushort)slot);')
        self.assertIn('((uchar *)&(slot))[2]',text)
        self.assertEqual(text.count(value),1)

    def test_comments_and_indentation_survive(self):
        _,text=self.body('slot._0_3_ = CONCAT12(flag /* source byte */, (ushort) /* keep */ slot);')
        self.assertIn('  ((uchar *)&(slot))[2]',text)
        self.assertIn('flag /* source byte */',text)

    def test_comment_and_literal_examples_do_not_change(self):
        source='{\n uint slot;\n // slot._0_3_ = (uint3)(ushort)slot;\n use("slot._0_3_ = CONCAT12(1,(ushort)slot);");\n}'
        self.assertEqual(seed.partial_byte_update_artifacts(source),source)

    def test_different_preserved_owner_is_not_rewritten(self):
        source,text=self.body('slot._0_3_ = CONCAT12(flag,(ushort)other);')
        self.assertEqual(source,text)

    def test_other_widths_and_storage_kinds_stay_refused(self):
        for kind in ('ushort','short','byte','bool','uint3','undefined3','ulonglong','float','double','Unknown','const uint','volatile uint','volatile byte *'):
            with self.subTest(kind=kind):
                source,text=self.body('slot._0_3_ = CONCAT12(flag,(ushort)slot);',kind+' slot;')
                self.assertEqual(source,text)

    def test_unknown_global_pointee_and_array_are_not_rewritten(self):
        for decl,slot in [('', 'DAT_00801000'),('uint *slot;','slot[0]'),('uint slots[2];','slots[0]'),('Unknown object;','object.field')]:
            source,text=self.body(slot+'._0_3_ = CONCAT12(flag,(ushort)'+slot+');',decl)
            self.assertEqual(source,text)

    def test_shadowed_and_comma_declarations_are_not_rewritten(self):
        for decl in ('uint slot; { uint slot; }','uint slot, other;'):
            source,text=self.body('slot._0_3_ = CONCAT12(flag,(ushort)slot);',decl)
            self.assertEqual(source,text)

    def test_effectful_values_are_not_rewritten(self):
        for value in ('get()','(*fn)()','(fn)(x)','++flag','flag++','(flag=1)','(a,b)','*(byte *)(base++)'):
            with self.subTest(value=value):
                source,text=self.body('slot._0_3_ = CONCAT12('+value+',(ushort)slot);')
                self.assertEqual(source,text)

    def test_used_assignment_results_and_suffixes_are_not_rewritten(self):
        for statement in ('return slot._0_3_ = CONCAT12(flag,(ushort)slot);',
                          'return\n  slot._0_3_ = CONCAT12(flag,(ushort)slot);',
                          'use(\n  slot._0_3_ = CONCAT12(flag,(ushort)slot));',
                          'slot._0_3_ = CONCAT12(flag,(ushort)slot), use(slot);',
                          'slot._0_3_ = CONCAT12(flag,(ushort)slot) & 0xffffff;',
                          'slot._0_3_ = (uint3)(ushort)slot + 1;'):
            with self.subTest(statement=statement):
                source,text=self.body(statement)
                self.assertEqual(source,text)

    def test_shadowed_operators_types_and_preprocessor_stay_untouched(self):
        for extra in ('code *CONCAT12;', 'int ushort;', 'int uint3;', '#define CONCAT12(a,b) 0'):
            source,text=self.body('slot._0_3_ = CONCAT12(flag,(ushort)slot);','uint slot;\n '+extra)
            self.assertEqual(source,text)

    def test_arbitrary_nonstandard_slices_are_still_errors(self):
        for statement in ('slot._0_3_ = value;','slot._1_3_ = CONCAT12(flag,(ushort)slot);','value = slot._0_3_;'):
            _,text=self.body(statement)
            with self.assertRaisesRegex(ValueError,'Unsupported Ghidra scalar slice width 3'):
                seed.scalar_partials(text)

    def test_multiple_updates_lower_in_place(self):
        _,text=self.body('slot._0_3_ = CONCAT12(flag,(ushort)slot);\n  slot._0_3_ = (uint3)(ushort)slot;')
        self.assertEqual(text.count('((uchar *)&(slot))[2]'),2)

    def test_complete_seed_recovers_both_forms_before_slice_lowering(self):
        class Inventory:
            passes=[]
            def decompiled(self,entry):
                return ('uint __cdecl FUN_00401000(uint slot,byte flag)\n{\n'
                        '  slot._0_3_ = CONCAT12(flag,(ushort)slot);\n'
                        '  slot._0_3_ = (uint3)(ushort)slot;\n  return slot;\n}\n')
            def function(self,entry):return None,{'ranges':[]}
        text=seed.seed(0x401000,Inventory(),{})
        self.assertEqual(text.count('((uchar *)&(slot))[2]'),2)
        self.assertNotIn('._0_3_',text)
        self.assertNotIn('CONCAT12',text)
        self.assertIn('return (uint)(slot);',text)

if __name__=='__main__':unittest.main()
