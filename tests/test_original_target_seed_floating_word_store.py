"""Merged floating storage: preserve a DWORD, never numerically convert it."""
from pathlib import Path
import sys,unittest,struct,random
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools/original_target'))
import seed_candidate as seed
class FloatingWordStoreTests(unittest.TestCase):
    def lower(self,statement='slot = (double)CONCAT44(slot._4_4_,value);',decl='double slot; uint value;',params=()):
        source='{\n '+decl+'\n '+statement+'\n}'
        return source,seed.floating_word_store_artifacts(source,params)
    def test_integer_word_is_stored_without_reading_upper_half(self):
        _,text=self.lower();self.assertIn('*((uint *)&(slot)) = (uint)(value);',text);self.assertNotIn('_4_4_',text)
    def test_pointer_bits_keep_the_same_word_width(self):
        _,text=self.lower(decl='double slot; int *value;');self.assertIn('(uint)(value)',text)
    def test_float_supplies_representation_bits(self):
        _,text=self.lower(decl='double slot; float value;');self.assertIn('*((uint const *)&(value))',text);self.assertNotIn('(uint)(value)',text)
    def test_const_float_bits_are_read_through_const_view(self):
        _,text=self.lower(decl='double slot; const float value;');self.assertIn('*((uint const *)&(value))',text)
    def test_integer_literal_clamps_are_supported(self):
        for value in ('0','10','100','-1','0xffffffffU'):
            _,text=self.lower('slot = (double)CONCAT44(slot._4_4_,'+value+');',decl='double slot;');self.assertIn('(uint)('+value+')',text)
    def test_double_value_parameters_are_supported(self):
        _,text=self.lower(decl='',params=['double slot','uint value']);self.assertIn('*((uint *)&(slot))',text)
    def test_arrays_references_qualifiers_and_unknown_slots_are_not_inferred(self):
        for decl in ('float slot; uint value;','float10 slot; uint value;','ulonglong slot; uint value;','volatile double slot; uint value;','const double slot; uint value;','double *slot; uint value;','double slot[2]; uint value;','uint value;'):
            source,text=self.lower(decl=decl);self.assertEqual(source,text)
    def test_shadowed_and_comma_declarations_refuse(self):
        for decl in ('double slot; { double slot; } uint value;','double slot,other; uint value;'):
            source,text=self.lower(decl=decl);self.assertEqual(source,text)
    def test_wide_and_unknown_rhs_types_are_not_converted(self):
        for decl in ('double slot; double value;','double slot; ulonglong value;','double slot; Unknown value;','double slot; uint value[2];','double slot; volatile uint value;','double slot;'):
            source,text=self.lower(decl=decl);self.assertEqual(source,text)
    def test_nonpreserving_high_words_stay_unresolved(self):
        source,text=self.lower('slot = (double)CONCAT44(other._4_4_,value);',decl='double slot; double other; uint value;');self.assertEqual(source,text)
    def test_high_word_math_and_different_slice_are_not_folded(self):
        for high in ('slot._0_4_','slot._4_4_+1','slot._4_4_&mask'):
            source,text=self.lower('slot = (double)CONCAT44('+high+',value);');self.assertEqual(source,text)
    def test_effectful_and_floating_expressions_stay_unresolved(self):
        for value in ('next()','value++','++value','(value=1)','value+1','(a,b)','*value','values[0]','(float)value','1.0'):
            source,text=self.lower('slot = (double)CONCAT44(slot._4_4_,'+value+');');self.assertEqual(source,text)
    def test_used_results_and_suffixes_stay_unresolved(self):
        for statement in ('return slot = (double)CONCAT44(slot._4_4_,value);','return\n slot = (double)CONCAT44(slot._4_4_,value);','use(\n slot = (double)CONCAT44(slot._4_4_,value));','slot = (double)CONCAT44(slot._4_4_,value),use(slot);','slot = (double)CONCAT44(slot._4_4_,value)+1;'):
            source,text=self.lower(statement);self.assertEqual(source,text)
    def test_comments_literals_and_internal_comments_are_retained(self):
        source,text=self.lower('// slot = (double)CONCAT44(slot._4_4_,value);\n char *s="slot = (double)CONCAT44(slot._4_4_,value);";');self.assertEqual(source,text)
        source,text=self.lower('slot = (double)CONCAT44(slot /*keep*/._4_4_,value);');self.assertEqual(source,text)
    def test_shadowed_concat_and_preprocessor_refuse(self):
        for decl in ('double slot; uint value; code *CONCAT44;','double slot; uint value; #define CONCAT44(a,b) 0'):
            source,text=self.lower(decl=decl);self.assertEqual(source,text)
    def test_float_nan_payloads_and_signed_zero_are_bits(self):
        for bits in (0,0x80000000,0x3f800000,0x7f800000,0x7fc01234,0x7f801234,0xffffffff):
            raw=bytearray(struct.pack('<Q',0x7ff0000000000001));raw[:4]=struct.pack('<I',bits)
            self.assertEqual(struct.unpack('<Q',raw)[0],0x7ff0000000000000|bits)
    def test_multiple_updates_keep_order(self):
        _,text=self.lower('slot = (double)CONCAT44(slot._4_4_,10);\n slot = (double)CONCAT44(slot._4_4_,value);')
        self.assertLess(text.index('(uint)(10)'),text.index('(uint)(value)'))
    def test_full_seed_lowers_before_concat_and_slice_expansion(self):
        class Inventory:
            passes=[]
            def decompiled(self,entry):return 'uint __cdecl FUN_00401000(double slot,uint value)\n{\n slot = (double)CONCAT44(slot._4_4_,value);\n return slot._0_4_;\n}'
            def function(self,entry):return None,{'ranges':[]}
        text=seed.seed(0x401000,Inventory(),{});self.assertNotIn('CONCAT44',text);self.assertNotIn('slot._',text);self.assertIn('*((uint *)&(slot))',text);self.assertNotIn('seed_unsigned64_double',text)
if __name__=='__main__':unittest.main()
