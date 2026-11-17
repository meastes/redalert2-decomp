from pathlib import Path
from types import SimpleNamespace
import os,sys,unittest
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools/original_target'))
if os.environ.get('RA2_SEED_TEST_TOOLS'):sys.path.append(os.environ['RA2_SEED_TEST_TOOLS'])
import seed_candidate as seed
from seed_fp_abi import member_stack_arguments,value_bytes
from seed_fp_native import NativeRefusal

class MemberABI(unittest.TestCase):
 def contract(self,parameters='',result='void',symbol='?Act@Toy@@QAEXXZ',extra=''):
  source=f'{result} Toy::Act({parameters})\n{{ {extra} }}\n'.encode();row=dict(source='src/recovered/Toy.cpp',source_sha256='mock',symbol=symbol)
  env=SimpleNamespace(provenance=lambda entry:'mock',rows={0x1000:row},_source_bytes=lambda path,digest:source)
  return member_stack_arguments(env,0x1000,seed)
 def test_empty_member_has_zero_stack_arguments(self):self.assertEqual(self.contract()['stack_argument_bytes'],0)
 def test_primitives_use_standard_i386_stack_widths(self):self.assertEqual(self.contract('int a, float b, double c, unsigned char d')['stack_argument_bytes'],20)
 def test_record_pointer_and_reference_do_not_guess_layout(self):self.assertEqual(self.contract('Unknown *p, const Other &r')['stack_argument_bytes'],8)
 def test_pointer_and_reference_results_have_no_hidden_result(self):
  self.assertFalse(self.contract(result='Unknown *')['hidden_result_pointer']);self.assertFalse(self.contract(result='Unknown &')['hidden_result_pointer'])
 def test_record_value_argument_refuses(self):
  with self.assertRaisesRegex(NativeRefusal,'by-value'):self.contract('Unknown record')
 def test_record_value_result_refuses_hidden_result_guess(self):
  with self.assertRaisesRegex(NativeRefusal,'by-value'):self.contract(result='Unknown')
 def test_cdecl_symbol_refuses(self):
  with self.assertRaisesRegex(NativeRefusal,'thiscall'):self.contract(symbol='?Act@@YAXXZ')
 def test_symbol_owner_must_match_definition(self):
  with self.assertRaisesRegex(NativeRefusal,'definition'):self.contract(symbol='?Act@Other@@QAEXXZ')
 def test_explicit_nonstandard_source_abi_refuses(self):
  with self.assertRaisesRegex(NativeRefusal,'nonstandard'):self.contract(result='void __stdcall')
 def test_variadic_source_refuses(self):
  with self.assertRaises(NativeRefusal):self.contract('int count, ...')
 def test_default_argument_refuses(self):
  with self.assertRaises(NativeRefusal):self.contract('int value=1')
 def test_inline_assembly_opaque_source_refuses(self):
  with self.assertRaisesRegex(NativeRefusal,'assembly'):self.contract(extra='__asm { nop }')
 def test_unknown_typedef_width_refuses(self):
  with self.assertRaises(NativeRefusal):value_bytes('word_type')
 def test_pointer_inside_template_does_not_imply_pointer_result(self):
  with self.assertRaises(NativeRefusal):self.contract(result='Container<int *>')
 def test_explicit_64_bit_scalar_is_eight_bytes(self):self.assertEqual(value_bytes('unsigned __int64'),8)

if __name__=='__main__':unittest.main()
