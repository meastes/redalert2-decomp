from pathlib import Path
import os,sys,unittest
tool_root=Path(__file__).resolve().parents[1]/'tools/original_target'
sys.path.insert(0,str(tool_root))
if os.environ.get('RA2_SEED_TEST_TOOLS'):sys.path.append(os.environ['RA2_SEED_TEST_TOOLS'])
import seed_candidate as seed
from seed_fp_source import bind_capture
class SourceBindings(unittest.TestCase):
 def source(self,*,low='low',high='high',root='owner',kind='undefined4',rootkind='double *',prefix='',middle='',before='',after='',argument=None,extra='',conv='__fastcall',offset='4',highfirst=False):
  lo=f'{low} = *(undefined4 *){root};';hi=f'{high} = *(undefined4 *)((int){root} + {offset});';initial=(hi+'\n '+lo) if highfirst else (lo+'\n '+middle+'\n '+hi)
  argument=argument if argument is not None else f'(float)(double)CONCAT44({high},{low})'
  return f'undefined4 {conv} FUN_00001000({rootkind} {root})\n{{\n {kind} {low};\n {kind} {high};\n {extra}\n {prefix}\n {initial}\n {before}\n FUN_00002000({argument});\n {after}\n return 0;\n}}\n'
 def bind(self,text):return bind_capture(text,'FUN_00002000',seed)
 def reject(self,**kwargs):
  with self.assertRaises(ValueError):self.bind(self.source(**kwargs))
 def test_basic_capture_and_exact_span(self):
  text=self.source();p=self.bind(text);self.assertEqual(p['root_byte_offsets'],[0,4]);self.assertEqual(text[slice(*p['inner_view_span'])],'(double)CONCAT44(high,low)')
 def test_names_are_semantic_not_fixed_identifiers(self):self.assertEqual(self.bind(self.source(low='first_word',high='second_word',root='object'))['low'],'first_word')
 def test_signed_dword_storage_preserves_bits(self):self.assertEqual(self.bind(self.source(kind='int'))['high'],'high')
 def test_thiscall_owner_supported(self):self.assertEqual(self.bind(self.source(conv='__thiscall'))['root'],'owner')
 def test_integer_address_carrier_supported(self):self.assertEqual(self.bind(self.source(rootkind='uint'))['root_kind'],'uint')
 def test_hex_byte_offset_supported(self):self.assertEqual(self.bind(self.source(offset='0x0004'))['root_byte_offsets'],[0,4])
 def test_later_address_use_does_not_change_earlier_lifetime(self):self.assertEqual(self.bind(self.source(after='later(&low);'))['low'],'low')
 def test_other_call_is_counted_without_becoming_witness(self):
  p=self.bind(self.source(before='FUN_00002000((float)1);'));self.assertEqual((p['source_call_count'],p['source_call_ordinal']),(2,1))
 def test_word_rewrite_before_use_refuses(self):self.reject(before='low = 0;')
 def test_address_escape_before_use_refuses(self):self.reject(before='use(&low);')
 def test_extra_read_before_use_refuses(self):self.reject(before='copy = high;')
 def test_partial_word_store_before_use_refuses(self):self.reject(before='low._0_2_ = 0;')
 def test_capture_high_offset_wrong_refuses(self):self.reject(offset='8')
 def test_high_read_before_low_refuses(self):self.reject(highfirst=True)
 def test_short_storage_refuses(self):self.reject(kind='short')
 def test_wide_storage_refuses(self):self.reject(kind='ulonglong')
 def test_volatile_storage_refuses(self):self.reject(kind='volatile undefined4')
 def test_source_shadowing_refuses(self):self.reject(extra='float low;')
 def test_root_shadowing_refuses(self):self.reject(extra='int owner;')
 def test_root_assignment_refuses(self):self.reject(prefix='owner = replacement;')
 def test_root_increment_refuses(self):self.reject(prefix='owner++;')
 def test_memory_store_before_capture_refuses(self):self.reject(middle='*destination = 0;')
 def test_conditional_capture_refuses(self):self.reject(prefix='if (ready)')
 def test_call_before_capture_refuses(self):self.reject(prefix='touch();')
 def test_extra_arithmetic_refuses(self):self.reject(argument='(float)(double)CONCAT44(high,low) * 2')
 def test_reversed_concat_words_refuses(self):self.reject(argument='(float)(double)CONCAT44(low,high)')
 def test_same_word_twice_refuses(self):self.reject(argument='(float)(double)CONCAT44(low,low)')
 def test_multiple_raw_views_refuse(self):self.reject(before='FUN_00002000((float)(double)CONCAT44(high,low));')
 def test_comment_inside_selected_argument_refuses(self):self.reject(argument='(float)(double)CONCAT44(high, /* preserve */ low)')
 def test_comment_cannot_forge_a_witness(self):
  text=self.source(argument='0').replace(' FUN_00002000(0);',' // FUN_00002000((float)(double)CONCAT44(high,low));\n FUN_00002000(0);')
  with self.assertRaises(ValueError):self.bind(text)
 def test_unknown_root_width_refuses(self):self.reject(rootkind='float')
 def test_cdecl_root_does_not_claim_ecx_binding(self):self.reject(conv='__cdecl')

if __name__=='__main__':unittest.main()
