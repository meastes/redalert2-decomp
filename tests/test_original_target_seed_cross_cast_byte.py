"""A 32-bit representation cast must not turn a byte store into a wide load."""
from pathlib import Path
import sys,unittest
sys.path.insert(0,str(Path.cwd()/'tools/original_target'))
import seed_candidate as seed
class CrossCastByteTests(unittest.TestCase):
    def body(self,stmt='slot = (int *)CONCAT31(slot._1_3_,flag);',decl='undefined4 slot; byte flag;',params=()):
        source='{\n '+decl.replace('; ', ';\n ')+'\n '+stmt+'\n}'
        return source,seed.byte_update_artifacts(source,params)
    def test_integer_carrier_pointer_cast_writes_only_one_byte(self):
        for kind,cast in [('undefined4','int *'),('uint','void *'),('int','unsigned short **'),('DWORD','float *')]:
            _,out=self.body('slot = ('+cast+')CONCAT31(slot._1_3_,flag);',kind+' slot; byte flag;')
            self.assertIn('*(uchar *)&(slot) = (uchar)(flag);',out)
            self.assertNotIn('._1_3_',out)
    def test_pointer_carrier_integer_cast_preserves_pointer_bits(self):
        for kind,cast in [('void *','uint'),('char **','long'),('HANDLE','unsigned int')]:
            _,out=self.body('slot = ('+cast+')CONCAT31(slot._1_3_,flag);',kind+' slot; byte flag;')
            self.assertNotIn('CONCAT31',out)
    def test_distinct_integer_aliases_keep_the_same_word(self):
        for kind,cast in [('int','unsigned long'),('undefined4','uint'),('DWORD','int')]:
            _,out=self.body('slot = ('+cast+')CONCAT31(slot._1_3_,flag);',kind+' slot; byte flag;')
            self.assertNotIn('CONCAT31',out)
    def test_declared_parameter_and_constant_array_element(self):
        _,out=self.body(decl='byte flag;',params=['undefined4 slot']);self.assertNotIn('CONCAT31',out)
        _,out=self.body('slots[1] = (void *)CONCAT31(slots[1]._1_3_,flag);','uint slots[2]; byte flag;');self.assertIn('&(slots[1])',out)
    def test_narrow_wide_floating_and_unknown_scalar_casts_refuse(self):
        for cast in ['short','ushort','char','byte','undefined3','ulonglong','longlong','float','double','Mystery']:
            before,out=self.body('slot = ('+cast+')CONCAT31(slot._1_3_,flag);');self.assertEqual(before,out,cast)
    def test_qualified_and_address_space_casts_refuse(self):
        for cast in ['const int *','volatile int *','int __ptr64 *','int __far *']:
            before,out=self.body('slot = ('+cast+')CONCAT31(slot._1_3_,flag);');self.assertEqual(before,out)
    def test_ambiguous_or_shadowed_storage_refuses_new_path(self):
        for decl,params in [('uint slot; {uint slot;} byte flag;',()),('uint slot; byte flag;',['uint slot']),('typedef short uint; undefined4 slot; byte flag;',()),('undefined4 slot; int uint; byte flag;',())]:
            stmt='slot = (uint)CONCAT31(slot._1_3_,flag);' if 'int uint' in decl else 'slot = (int *)CONCAT31(slot._1_3_,flag);'
            before,out=self.body(stmt,decl,params);self.assertEqual(before,out)
    def test_unknown_pointee_and_dynamic_array_index_refuse(self):
        for stmt,decl in [('slot[0] = (void *)CONCAT31(slot[0]._1_3_,flag);','uint *slot; byte flag;'),('slots[i] = (void *)CONCAT31(slots[i]._1_3_,flag);','uint slots[2]; int i; byte flag;')]:
            before,out=self.body(stmt,decl);self.assertEqual(before,out)
    def test_other_preserved_carrier_and_high_byte_masks_refuse(self):
        for stmt in ['slot = (int *)CONCAT31(other._1_3_,flag);','slot = (int *)CONCAT31(slot._1_3_,flag) & 0xffff0001;','slot = (int *)CONCAT31(slot._0_3_,flag);']:
            before,out=self.body(stmt);self.assertEqual(before,out)
    def test_comments_preprocessor_and_effectful_values_refuse(self):
        for stmt in ['slot = (int /*keep*/ *)CONCAT31(slot._1_3_,flag);','slot = (int *)CONCAT31(slot._1_3_,flag++);','#define X 1\n slot = (int *)CONCAT31(slot._1_3_,flag);']:
            before,out=self.body(stmt);self.assertEqual(before,out)
    def test_used_assignment_and_nested_casts_refuse(self):
        for stmt in ['return slot = (int *)CONCAT31(slot._1_3_,flag);','slot = (int *)(uint)CONCAT31(slot._1_3_,flag);','use(slot = (int *)CONCAT31(slot._1_3_,flag));']:
            before,out=self.body(stmt);self.assertEqual(before,out)
    def test_inline_shadow_and_comma_declarations_are_not_inferred(self):
        source='{uint slot; byte flag;\n slot=(int *)CONCAT31(slot._1_3_,flag);}'
        self.assertEqual(seed.byte_update_artifacts(source,['uint slot']),source)
        before,out=self.body(decl='uint slot,other; byte flag;');self.assertEqual(before,out)
    def test_complete_seed_repairs_before_scalar_slice_refusal(self):
        class Inventory:
            passes=[]
            def decompiled(self,e):return 'uint __cdecl FUN_00401000(uint slot, byte flag)\n{\n slot=(int *)CONCAT31(slot._1_3_,flag);\n return slot;\n}'
            def function(self,e):return None,{'ranges':[]}
        out=seed.seed(0x401000,Inventory(),{});self.assertNotIn('CONCAT31',out);self.assertNotIn('._1_3_',out);self.assertIn('*(uchar *)&(slot)',out)
if __name__=='__main__':unittest.main()
