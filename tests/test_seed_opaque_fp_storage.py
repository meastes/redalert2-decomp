from pathlib import Path
import importlib.util,os,sys,unittest
from types import SimpleNamespace
from unittest.mock import patch
import capstone
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools/original_target'))
if os.environ.get('RA2_SEED_TEST_TOOLS'):sys.path.append(os.environ['RA2_SEED_TEST_TOOLS'])
import seed_candidate as seed
from seed_opaque_fp_storage import fp_storage_hypotheses,source_fp_carriers,native_fp64_facts
FACTS=dict(original_ranges_verified=True,fp64_loads=1,fp64_stores=1,integer64_conversions=0)
class Views(unittest.TestCase):
 def body(self,kind='undefined8',producer='(double)input',read='(float10)carrier',before='',after='',prefix=''):
  return '{\n '+kind+' carrier;\n double input;\n double result;\n '+prefix+'\n '+before+'\n carrier = '+producer+';\n result = '+read+';\n '+after+'\n}\n'
 def transform(self,body,params=(),facts=None):return fp_storage_hypotheses(body,params,seed,FACTS if facts is None else facts)
 def unchanged(self,**kwargs):
  body=self.body(**kwargs);self.assertEqual(self.transform(body),(body,[]))
 def test_producer_is_same_eight_byte_object_not_new_storage(self):
  text,notes=self.transform(self.body());self.assertIn('*((double *)&(carrier)) = (double)input;',text);self.assertIn('undefined8 carrier;',text);self.assertNotIn('union',text);self.assertIn('correspondence unproved',notes[0])
 def test_explicit_fp_read_is_bit_view(self):self.assertIn('(float10)(*((double const *)&(carrier)))',self.transform(self.body())[0])
 def test_double_cast_read_is_bit_view(self):self.assertIn('(double)(*((double const *)&(carrier)))',self.transform(self.body(read='(double)carrier'))[0])
 def test_plain_copy_to_known_double_reads_fp_view(self):self.assertIn('result = *((double const *)&(carrier));',self.transform(self.body(read='carrier'))[0])
 def test_integer_uses_keep_original_storage(self):self.assertIn('value = (uint)carrier;',self.transform(self.body(after='value = (uint)carrier;'))[0])
 def test_low_dword_preserves_high_part_without_reread(self):
  body=self.body(after='carrier = (double)CONCAT44(carrier._4_4_,word);',prefix='uint word;');result,notes=self.transform(body);self.assertIn('*((uint *)&(carrier)) = (uint)(word);',result)
 def test_complex_pointer_word_copy_keeps_low_bits(self):
  body=self.body(after='result = (double)CONCAT44(result._4_4_,(double *)carrier);');result,notes=self.transform(body);self.assertIn('*((uint *)&(result)) = (uint)((double *)carrier);',result)
 def test_unknown_word_operand_is_not_repaired(self):
  body=self.body(after='carrier = (double)CONCAT44(carrier._4_4_,missing);');self.assertIn('CONCAT44',self.transform(body)[0])
 def test_nonself_high_part_is_not_repaired(self):
  body=self.body(after='carrier = (double)CONCAT44(other._4_4_,word);',prefix='uint word;');self.assertIn('CONCAT44',self.transform(body)[0])
 def test_side_effectful_word_value_keeps_concat(self):
  body=self.body(after='carrier = (double)CONCAT44(carrier._4_4_,FUN_00002000());');self.assertIn('CONCAT44',self.transform(body)[0])
 def test_concrete_unsigned64_is_not_reinterpreted(self):self.unchanged(kind='ulonglong')
 def test_concrete_signed64_is_not_reinterpreted(self):self.unchanged(kind='longlong')
 def test_narrow_storage_is_not_reinterpreted(self):self.unchanged(kind='uint')
 def test_volatile_carrier_refuses(self):self.unchanged(kind='volatile undefined8')
 def test_const_carrier_refuses(self):self.unchanged(kind='const undefined8')
 def test_pointer_carrier_refuses(self):self.unchanged(kind='undefined8 *')
 def test_reference_carrier_refuses(self):self.unchanged(kind='undefined8 &')
 def test_array_carrier_refuses(self):
  body=self.body().replace('undefined8 carrier;','undefined8 carrier[2];');self.assertEqual(self.transform(body),(body,[]))
 def test_shadowed_carrier_refuses(self):self.unchanged(before='{ undefined8 carrier; }')
 def test_const_source_has_no_nonconstant_producer(self):
  body=self.body().replace('double input;','const double input = 1.0;');self.assertEqual(self.transform(body),(body,[]))
 def test_bare_float_source_qualifies(self):self.assertIn('*((double *)&(carrier)) = input;',self.transform(self.body(producer='input'))[0])
 def test_floating_pointee_source_qualifies(self):self.assertIn('*((double *)&(carrier)) = *p;',self.transform(self.body(producer='*p'),['double *p'])[0])
 def test_integer_pointee_without_fp_cast_is_not_fp_producer(self):
  body=self.body(producer='*p');self.assertEqual(self.transform(body,['uint *p']),(body,[]))
 def test_literal_producer_does_not_qualify(self):self.unchanged(producer='(double)0')
 def test_concat_producer_does_not_establish_fp_value(self):self.unchanged(producer='(double)CONCAT44(high,low)')
 def test_self_reference_cannot_establish_first_fp_value(self):self.unchanged(producer='(double)carrier')
 def test_numeric_integer64_original_conversion_refuses(self):
  body=self.body();self.assertEqual(self.transform(body,facts=dict(FACTS,integer64_conversions=1)),(body,[]))
 def test_missing_original_verification_refuses(self):
  body=self.body();self.assertEqual(self.transform(body,facts=dict(FACTS,original_ranges_verified=False)),(body,[]))
 def test_no_native_fp_load_refuses(self):
  body=self.body();self.assertEqual(self.transform(body,facts=dict(FACTS,fp64_loads=0)),(body,[]))
 def test_no_native_fp_store_refuses(self):
  body=self.body();self.assertEqual(self.transform(body,facts=dict(FACTS,fp64_stores=0)),(body,[]))
 def test_macro_or_typedef_refuses(self):self.unchanged(prefix='#define carrier elsewhere');self.unchanged(prefix='typedef int other;')
 def test_real_concat_object_refuses(self):self.unchanged(prefix='int CONCAT44;')
 def test_real_concat_function_refuses(self):self.unchanged(prefix='extern double CONCAT44(int,int);')
 def test_selected_read_comment_preserved(self):
  text,notes=self.transform(self.body(read='(double) /* keep */ carrier'));self.assertIn('(double) /* keep */ carrier',text)
 def test_member_suffix_is_not_mistaken_for_full_fp_read(self):
  text,notes=self.transform(self.body(read='(double)carrier._0_4_'));self.assertIn('(double)carrier._0_4_',text)
 def test_rhs_call_count_and_text_remain_identical(self):
  body=self.body(producer='(double)FUN_00002000(input)');text,notes=self.transform(body);self.assertEqual(text.count('FUN_00002000(input)'),1)
 def test_import_has_no_file_reads(self):
  path=Path(__file__).resolve().parents[1]/'tools/original_target/seed_opaque_fp_storage.py';spec=importlib.util.spec_from_file_location('fresh_opaque_fp_no_io',path);module=importlib.util.module_from_spec(spec)
  with patch.object(Path,'read_bytes',side_effect=AssertionError('Import I/O')),patch.object(Path,'read_text',side_effect=AssertionError('Import I/O')):spec.loader.exec_module(module)
class ExpressionViews(unittest.TestCase):
 body=Views.body
 transform=Views.transform
 def test_typed_double_dereference_producer(self):
  text,_=self.transform(self.body(producer='*(double *)(p + 8)'),['int p']);self.assertIn('*((double *)&(carrier)) = *(double *)(p + 8);',text)
 def test_float_narrowing_reads_fp_bits(self):
  text,_=self.transform(self.body(read='(float)carrier'));self.assertIn('(float)(*((double const *)&(carrier)))',text)
 def test_high_word_preserves_low_word(self):
  text,_=self.transform(self.body(prefix='uint high;',after='carrier = (double)CONCAT44(high,(int)carrier);'));self.assertIn('*((uint *)((char *)&(carrier) + 4)) = (uint)(high);',text)
 def test_high_word_requires_exact_self_low_word(self):
  text,_=self.transform(self.body(prefix='uint high;',after='carrier = (double)CONCAT44(high,(short)carrier);'));self.assertIn('CONCAT44',text)
 def test_high_word_side_effect_refuses(self):
  text,_=self.transform(self.body(after='carrier = (double)CONCAT44(FUN_00002000(),(int)carrier);'));self.assertIn('CONCAT44',text)
 def norm(self,peer='double',expression='carrier * carrier + y * y + z * z'):
  return self.body(prefix=peer+' y;\n double z;',after='result = '+expression+';')
 def test_three_term_norm_reads_opaque_in_fp_context(self):
  text,_=self.transform(self.norm());self.assertIn('*((double const *)&(carrier)) * *((double const *)&(carrier)) + y * y + z * z',text)
 def test_norm_integer_peer_refuses(self):
  text,_=self.transform(self.norm(peer='uint'));self.assertIn('carrier * carrier + y * y + z * z',text)
 def test_norm_duplicate_term_refuses(self):
  text,_=self.transform(self.norm(expression='carrier * carrier + y * y + y * y'));self.assertIn('carrier * carrier + y * y + y * y',text)
 def test_norm_preserves_comment_and_evaluation_count(self):
  text,_=self.transform(self.norm(expression='carrier /* keep */ * carrier + y * y + z * z'));self.assertIn('/* keep */',text);self.assertEqual(text.count('/* keep */'),1)
 def comparison(self,left,tail='',prefix=''):
  return self.body(prefix=prefix,after='if ('+left+' < carrier'+tail+') result = input;')
 def test_comparison_known_floating_peer(self):
  text,_=self.transform(self.comparison('input'));self.assertIn('input < *((double const *)&(carrier))',text)
 def test_comparison_explicit_cast_peer(self):
  text,_=self.transform(self.comparison('(double)word',prefix='int word;'));self.assertIn('(double)word < *((double const *)&(carrier))',text)
 def test_comparison_fp_product_peer(self):
  text,_=self.transform(self.comparison('input * (double)word',prefix='int word;'));self.assertIn('< *((double const *)&(carrier))',text)
 def test_comparison_integer_cast_is_barrier(self):
  text,_=self.transform(self.comparison('(int)(double)input'));self.assertIn('< carrier)',text)
 def test_neighboring_logical_fp_clause_is_barrier(self):
  text,_=self.transform(self.comparison('input && word',prefix='int word;'));self.assertIn('word < carrier)',text)
 def test_neighboring_logical_or_fp_clause_is_barrier(self):
  text,_=self.transform(self.comparison('input || word',prefix='int word;'));self.assertIn('word < carrier)',text)
 def test_comparison_nonplain_rhs_refuses(self):
  text,_=self.transform(self.comparison('input',tail=' + 1'));self.assertIn('< carrier + 1',text)
 def test_shift_is_not_comparison(self):
  text,_=self.transform(self.body(after='value = input >> carrier;'));self.assertIn('input >> carrier;',text)
 def test_member_suffix_is_not_plain_comparison_operand(self):
  text,_=self.transform(self.comparison('input',tail='._0_4_'));self.assertIn('< carrier._0_4_',text)
 def test_pointer_member_is_not_comparison(self):
  text,_=self.transform(self.body(after='value = p->carrier;'));self.assertIn('p->carrier;',text)

class ProducerResults(unittest.TestCase):
 body=Views.body
 transform=Views.transform
 def test_top_level_comparison_is_not_fp_producer(self):
  body=self.body(producer='(double)input < input');self.assertEqual(self.transform(body),(body,[]))
 def test_top_level_logical_result_is_not_fp_producer(self):
  body=self.body(producer='(double)input && input');self.assertEqual(self.transform(body),(body,[]))
 def test_top_level_conditional_is_not_fp_producer(self):
  body=self.body(producer='(double)input ? 1 : 0');self.assertEqual(self.transform(body),(body,[]))
 def test_nested_comparison_explicitly_converted_is_fp(self):
  text,_=self.transform(self.body(producer='(double)(input < result)'));self.assertIn('*((double *)&(carrier))',text)
 def test_member_arrow_is_not_comparison(self):
  text,_=self.transform(self.body(producer='*(double *)(p->field)'),['int *p']);self.assertIn('*((double *)&(carrier))',text)

class NativeFacts(unittest.TestCase):
 def facts(self,body,wrong=False):
  decoder=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);decoder.detail=True;entry=0x1000;inv=SimpleNamespace(function=lambda e:(None,dict(ranges=[dict(start='00001000',end_inclusive='%08x'%(entry+len(body)-1),bytes_hex=body.hex())])));audit=SimpleNamespace(decoder=decoder,original=lambda at,size:(b'X'+body[1:] if wrong else body)[at-entry:at-entry+size]);return native_fp64_facts(audit,inv,entry)
 def test_floating_qword_load_store_count(self):
  p=self.facts(bytes.fromhex('dd0424dd1c24c3'));self.assertEqual((p['fp64_loads'],p['fp64_stores'],p['integer64_conversions']),(1,1,0));self.assertTrue(p['original_ranges_verified'])
 def test_qword_integer_load_is_not_fp_transport(self):self.assertEqual(self.facts(bytes.fromhex('df2c24c3'))['integer64_conversions'],1)
 def test_qword_integer_store_is_not_fp_transport(self):self.assertEqual(self.facts(bytes.fromhex('df3c24c3'))['integer64_conversions'],1)
 def test_qword_truncating_integer_store_is_not_fp_transport(self):self.assertEqual(self.facts(bytes.fromhex('dd0c24c3'))['integer64_conversions'],1)
 def test_dword_integer_conversion_does_not_claim_integer64(self):self.assertEqual(self.facts(bytes.fromhex('db0424c3'))['integer64_conversions'],0)
 def test_byte_mismatch_never_verifies_ranges(self):self.assertFalse(self.facts(bytes.fromhex('dd0424dd1c24c3'),wrong=True)['original_ranges_verified'])
 def test_incomplete_decode_never_claims_no_integer64_conversion(self):self.assertFalse(self.facts(bytes.fromhex('dd0424dd1c24ff'))['original_ranges_verified'])
if __name__=='__main__':unittest.main()
