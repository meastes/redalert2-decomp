from pathlib import Path
import os,sys,unittest
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools/original_target'))
if os.environ.get('RA2_SEED_TEST_TOOLS'):sys.path.append(os.environ['RA2_SEED_TEST_TOOLS'])
import seed_candidate as seed
from seed_integer_scope import bind_integer_scope
from seed_guarded_cfg import CFGRefusal
class SourceScope(unittest.TestCase):
 def source(self,carrier='carrier',counter='counter',kind='int',fp='float',before='',middle='',after='',reset='DAT_00123456',prefix=''):
  return prefix+f'void FUN_00001000(void)\n{{\n {fp} {carrier};\n {kind} {counter};\n {counter} = 0;\n {before}\n {carrier} = -NAN;\n do {{\n {middle}\n if ({counter} - (int){carrier} < 2) {counter} = {counter} + 1;\n {carrier} = (float){counter};\n }} while ({counter} < 10);\n {carrier} = {reset};\n {after}\n}}\n'
 def bind(self,text):return bind_integer_scope(text,seed)
 def reject(self,**kwargs):
  with self.assertRaises(CFGRefusal):self.bind(self.source(**kwargs))
 def test_discovers_scope_but_never_nan_payload_or_rewrite(self):
  b=self.bind(self.source());self.assertEqual((b['carrier'],b['counter'],b['sign_bit']),('carrier','counter',1));self.assertFalse(b['native_constant_known']);self.assertFalse(b['source_emission_allowed'])
 def test_source_coordinates_are_on_original_text(self):
  t=self.source();b=self.bind(t);self.assertEqual(t[slice(*b['initializer_span'])],'carrier = -NAN;');self.assertEqual(t[slice(*b['read_span'])],'counter - (int)carrier')
 def test_identifiers_are_not_fixed(self):self.assertEqual(self.bind(self.source(carrier='reused',counter='line_number'))['counter'],'line_number')
 def test_unsigned_counter_width_remains_explicit(self):self.assertEqual(self.bind(self.source(kind='unsigned int'))['counter_kind'],'unsigned int')
 def test_float64_carrier_refuses(self):self.reject(fp='double')
 def test_wide_integer_counter_refuses(self):self.reject(kind='__int64')
 def test_carrier_address_escape_refuses(self):self.reject(middle='FUN_00002000(&carrier);')
 def test_extra_float_read_refuses(self):self.reject(middle='FUN_00002000(carrier);')
 def test_counter_read_before_initializer_refuses(self):self.reject(before='FUN_00002000(counter);')
 def test_later_float_reads_are_separate_lifetime(self):self.assertTrue(self.bind(self.source(after='FUN_00002000(carrier);'))['later_lifetime_span'])
 def test_later_reset_cannot_read_old_lifetime(self):self.reject(reset='carrier + DAT_00123456')
 def test_real_nan_macro_refuses(self):self.reject(prefix='#define NAN 0.0f\n')
 def test_real_nan_global_refuses(self):self.reject(prefix='float NAN;\n')
 def test_carrier_shadow_refuses(self):self.reject(middle='{ float carrier; }')
 def test_counter_shadow_refuses(self):self.reject(middle='{ int counter; }')
 def test_selected_comment_is_preserved_by_refusal(self):
  t=self.source().replace('counter - (int)carrier','counter /* value */ - (int)carrier')
  with self.assertRaisesRegex(CFGRefusal,'Comment'):self.bind(t)
 def test_member_assignment_is_not_local_write(self):
  t=self.source().replace('carrier = -NAN','object.carrier = -NAN')
  with self.assertRaisesRegex(CFGRefusal,'standalone'):self.bind(t)
 def test_member_read_is_not_counter_value(self):
  t=self.source().replace('counter - (int)carrier','object.counter - (int)carrier')
  with self.assertRaisesRegex(CFGRefusal,'declared counter'):self.bind(t)
 def test_two_bare_nan_initializers_refuse(self):self.reject(middle='other = -NAN;')
 def test_different_read_counter_refuses(self):
  t=self.source().replace('counter - (int)carrier','other - (int)carrier')
  with self.assertRaises(CFGRefusal):self.bind(t)
 def test_literal_comparison_metadata(self):
  b=self.bind(self.source());self.assertEqual(b['comparison_limit'],2);self.assertEqual(b['counter_increment_count'],1)
 def test_compound_comparison_bound_refuses(self):
  with self.assertRaises(CFGRefusal):self.bind(self.source().replace('< 2)','< 2 + 1)'))
 def test_signed_literal_comparison_supported(self):self.assertEqual(self.bind(self.source().replace('< 2)','< -3)'))['comparison_limit'],-3)
 def test_counter_address_escape_refuses(self):self.reject(middle='FUN_00002000(&counter);')
 def test_compound_counter_assignment_refuses(self):self.reject(middle='counter += 2;')
 def test_unary_counter_increment_refuses(self):self.reject(middle='++counter;')
 def test_additional_counter_reset_refuses(self):self.reject(middle='counter = 0;')
if __name__=='__main__':unittest.main()
