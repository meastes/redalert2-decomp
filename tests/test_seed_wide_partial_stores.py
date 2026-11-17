from pathlib import Path
import importlib.util,os,sys,unittest
from unittest.mock import patch
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools/original_target'))
if os.environ.get('RA2_SEED_TEST_TOOLS'):sys.path.append(os.environ['RA2_SEED_TEST_TOOLS'])
import seed_candidate as seed
from seed_wide_partial_stores import low_word_store_hypotheses
class Stores(unittest.TestCase):
 def body(self,kind='double',value='word',high='wide._2_6_',before='',after='',decl='ushort word;',prefix=''):
  return '{\n '+kind+' wide;\n '+decl+'\n '+prefix+'\n '+before+'\n wide = (double)CONCAT62('+high+','+value+');\n '+after+'\n}\n'
 def lower(self,text,params=()):return low_word_store_hypotheses(text,params,seed)
 def reject(self,**kwargs):
  body=self.body(**kwargs);self.assertEqual(self.lower(body),(body,[]))
 def test_double_low_word_is_two_byte_lvalue(self):
  result,notes=self.lower(self.body());self.assertIn('*((ushort *)&(wide)) = (ushort)(word);',result);self.assertNotIn('CONCAT62',result);self.assertEqual(len(notes),1)
 def test_unsigned_eight_byte_storage_supported(self):self.assertNotIn('CONCAT62',self.lower(self.body(kind='undefined8'))[0])
 def test_unsigned_builtin64_storage_supported(self):self.assertNotIn('CONCAT62',self.lower(self.body(kind='unsigned __int64'))[0])
 def test_signed64_storage_not_silently_reinterpreted(self):self.reject(kind='__int64')
 def test_narrow_storage_refuses(self):self.reject(kind='uint')
 def test_pointer_storage_refuses(self):self.reject(kind='double *')
 def test_array_storage_refuses(self):
  body=self.body().replace('double wide;','double wide[2];');self.assertEqual(self.lower(body),(body,[]))
 def test_reference_storage_refuses(self):self.reject(kind='double &')
 def test_parameter_storage_is_not_a_local(self):
  body=self.body().replace(' double wide;\n','');self.assertEqual(self.lower(body,['double wide']),(body,[]))
 def test_volatile_storage_refuses(self):self.reject(kind='volatile double')
 def test_const_storage_refuses(self):self.reject(kind='const double')
 def test_shadowed_storage_refuses(self):self.reject(before='{ double wide; }')
 def test_comma_declarations_refuse(self):self.reject(decl='ushort word, other;')
 def test_preserved_high_part_must_be_same_local(self):self.reject(high='other._2_6_',decl='ushort word;\n double other;')
 def test_different_preserved_extent_refuses(self):self.reject(high='wide._3_5_')
 def test_other_concat_width_is_not_changed(self):
  body=self.body().replace('CONCAT62','CONCAT44');self.assertEqual(self.lower(body),(body,[]))
 def test_trailing_numeric_operation_refuses(self):
  body=self.body().replace('CONCAT62(wide._2_6_,word);','CONCAT62(wide._2_6_,word) + 1;');self.assertEqual(self.lower(body),(body,[]))
 def test_used_assignment_result_refuses(self):
  body=self.body().replace('wide = (double)CONCAT62(wide._2_6_,word);','if ((wide = (double)CONCAT62(wide._2_6_,word)) != 0) {}');self.assertEqual(self.lower(body),(body,[]))
 def test_real_builtin_object_refuses(self):self.reject(prefix='int CONCAT62;')
 def test_real_builtin_function_refuses(self):self.reject(prefix='extern double CONCAT62(int,int);')
 def test_macro_refuses(self):self.reject(prefix='#define CONCAT62 other')
 def test_calling_value_refuses(self):self.reject(value='FUN_00002000()')
 def test_mutating_value_refuses(self):self.reject(value='word++')
 def test_unknown_value_refuses(self):self.reject(value='missing')
 def test_volatile_value_refuses(self):self.reject(decl='volatile ushort word;')
 def test_floating_value_refuses(self):self.reject(decl='float word;')
 def test_float_literal_refuses(self):self.reject(value='0.5')
 def test_exponent_literal_refuses(self):self.reject(value='1e2')
 def test_hex_with_letter_e_is_integer(self):self.assertNotIn('CONCAT62',self.lower(self.body(value='0x1e2'))[0])
 def test_integer_pointee_value_evaluated_once(self):
  result,notes=self.lower(self.body(value='(short)*p / 2'),['uint *p']);self.assertNotIn('CONCAT62',result);self.assertEqual(result.count('*p'),1)
 def test_volatile_pointee_refuses(self):
  body=self.body(value='(short)*p / 2');self.assertEqual(self.lower(body,['volatile uint *p']),(body,[]))
 def test_selected_comment_is_not_discarded(self):
  body=self.body(value='word /* keep */');self.assertEqual(self.lower(body),(body,[]))
 def test_self_floating_rhs_is_not_numeric_to_word_fix(self):self.reject(value='(ushort)wide')
 def test_unselected_text_and_literals_preserved(self):
  body=self.body(before='word = word + 1;',after='FUN_00002000();',prefix='char *message = "CONCAT62";');result,notes=self.lower(body);self.assertIn('word = word + 1;',result);self.assertIn('FUN_00002000();',result);self.assertIn('"CONCAT62"',result)
 def test_multiple_stores_have_separate_word_writes(self):
  body=self.body();body=body.replace('\n}', '\n wide = (double)CONCAT62(wide._2_6_,0x2000);\n}');result,notes=self.lower(body);self.assertEqual(result.count('*((ushort *)&(wide))'),2)
 def test_import_performs_no_path_reads(self):
  path=Path(__file__).resolve().parents[1]/'tools/original_target/seed_wide_partial_stores.py';spec=importlib.util.spec_from_file_location('fresh_wide_store_no_io',path);module=importlib.util.module_from_spec(spec)
  with patch.object(Path,'read_bytes',side_effect=AssertionError('Import I/O')),patch.object(Path,'read_text',side_effect=AssertionError('Import I/O')):spec.loader.exec_module(module)
if __name__=='__main__':unittest.main()
