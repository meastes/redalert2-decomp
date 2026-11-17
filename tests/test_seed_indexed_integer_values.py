from pathlib import Path
import os,sys,unittest
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools/original_target'))
import seed_candidate as seed
from seed_wide_partial_stores import _integer_value,low_word_store_hypotheses
from seed_opaque_fp_storage import fp_storage_hypotheses
FACTS=dict(original_ranges_verified=True,fp64_loads=1,fp64_stores=1,integer64_conversions=0)
class IndexedValues(unittest.TestCase):
 def value(self,text,p='int *',index='int',array='parameter'):
  return _integer_value(text,dict(p=[(p,array)],q=[('uint *','parameter')],i=[(index,'parameter')]),seed)
 def test_constant_index(self):self.assertTrue(self.value('p[0x24]'))
 def test_variable_index(self):self.assertTrue(self.value('p[i]'))
 def test_negative_index(self):self.assertTrue(self.value('p[-1]'))
 def test_wrapped_integer_index_expression(self):self.assertTrue(self.value('p[(uint)i & 31]'))
 def test_two_integer_loads(self):self.assertTrue(self.value('p[i] + q[2]'))
 def test_short_pointee(self):self.assertTrue(self.value('p[i]',p='short *'))
 def test_byte_pointee(self):self.assertTrue(self.value('p[i]',p='unsigned char *'))
 def test_numeric_parameter_marker_is_not_an_array(self):self.assertTrue(self.value('i + 1'))
 def test_floating_pointee_refuses(self):self.assertFalse(self.value('p[i]',p='double *'))
 def test_opaque_eight_byte_pointee_refuses(self):self.assertFalse(self.value('p[i]',p='undefined8 *'))
 def test_pointer_to_pointer_refuses(self):self.assertFalse(self.value('p[i]',p='int **'))
 def test_scalar_base_refuses(self):self.assertFalse(self.value('p[i]',p='int'))
 def test_volatile_pointee_refuses(self):self.assertFalse(self.value('p[i]',p='volatile int *'))
 def test_const_pointee_stays_conservative(self):self.assertFalse(self.value('p[i]',p='const int *'))
 def test_array_extent_not_invented(self):self.assertFalse(self.value('p[i]',array='8'))
 def test_reference_base_refuses(self):self.assertFalse(self.value('p[i]',p='int *&'))
 def test_floating_index_refuses(self):self.assertFalse(self.value('p[i]',index='double'))
 def test_pointer_index_refuses(self):self.assertFalse(self.value('p[i]',index='int *'))
 def test_nested_index_refuses(self):self.assertFalse(self.value('p[q[i]]'))
 def test_second_subscript_refuses(self):self.assertFalse(self.value('p[i][0]'))
 def test_index_write_refuses(self):self.assertFalse(self.value('p[i++]'))
 def test_index_call_refuses(self):self.assertFalse(self.value('p[FUN_00001000()]'))
 def test_unknown_index_refuses(self):self.assertFalse(self.value('p[missing]'))
 def test_unknown_base_refuses(self):self.assertFalse(self.value('missing[i]'))
 def test_fp_literal_index_refuses(self):self.assertFalse(self.value('p[1e2]'))
 def test_member_access_refuses(self):self.assertFalse(self.value('p[i].member'))
 def test_pointer_member_refuses(self):self.assertFalse(self.value('p[i]->member'))
 def test_assignment_in_index_refuses(self):self.assertFalse(self.value('p[i=1]'))
 def test_index_comma_refuses(self):self.assertFalse(self.value('p[i,1]'))
 def test_unbalanced_index_refuses(self):self.assertFalse(self.value('p[i'))
 def test_low_dword_uses_original_integer_load_once(self):
  body='{\n undefined8 wide;\n double input;\n wide = (double)input;\n wide = (double)CONCAT44(wide._4_4_,p[0x24]);\n}';text,notes=fp_storage_hypotheses(body,['int *p'],seed,FACTS);self.assertIn('*((uint *)&(wide)) = (uint)(p[0x24]);',text);self.assertEqual(text.count('p[0x24]'),1)
 def test_low_word_uses_original_index_expression_once(self):
  body='{\n double wide;\n wide = (double)CONCAT62(wide._2_6_,p[i]);\n}';text,notes=low_word_store_hypotheses(body,['int *p','int i'],seed);self.assertIn('*((ushort *)&(wide)) = (ushort)(p[i]);',text);self.assertEqual(text.count('p[i]'),1)
if __name__=='__main__':unittest.main()
