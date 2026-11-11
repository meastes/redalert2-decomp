from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch
import hashlib,os,sys,unittest
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools/original_target'))
if os.environ.get('RA2_SEED_TEST_TOOLS'):sys.path.append(os.environ['RA2_SEED_TEST_TOOLS'])
import seed_candidate as seed
import seed_fp_bitview as bitview
from seed_fp_source import bind_capture
from seed_fp_native import NativeRefusal
import test_seed_fp_source as sources

class BitViewEmission(unittest.TestCase):
 def source(self,**kwargs):return sources.SourceBindings().source(**kwargs)
 def proof(self,text):return dict(qualified_guard_complete=True,frozen_source_sha256=hashlib.sha256(text.encode()).hexdigest(),discovery={'source_binding':bind_capture(text,'FUN_00002000',seed)},source_emitted=False,source_emission_allowed=False)
 def rewrite(self,text,change=None):
  proof=self.proof(text)
  if change:change(proof)
  with patch.object(bitview,'prove_raw_scalar',return_value=proof):return bitview.rewrite_raw_scalar(None,0x1000,text,seed)
 def test_only_inner_view_and_one_local_declaration_change(self):
  original=self.source();result,proof=self.rewrite(original);name=proof['view_name'];decl='\n    union { uint words[2]; double value; } '+name+';';expr='('+name+'.words[0] = (uint)low, '+name+'.words[1] = (uint)high, '+name+'.value)';restored=result.replace(decl,'',1).replace(expr,'(double)CONCAT44(high,low)',1);self.assertEqual(restored,original);self.assertTrue(proof['source_emitted'])
 def test_outer_float_narrowing_is_preserved(self):self.assertIn('(float)(captured_scalar_view.words[0]',self.rewrite(self.source())[0])
 def test_fresh_name_avoids_existing_local(self):self.assertEqual(self.rewrite(self.source(extra='int captured_scalar_view;'))[1]['view_name'],'captured_scalar_view_1')
 def test_fresh_name_avoids_all_existing_suffixes(self):self.assertEqual(self.rewrite(self.source(extra='int captured_scalar_view; int captured_scalar_view_1;'))[1]['view_name'],'captured_scalar_view_2')
 def test_source_identifier_renaming_is_not_hardcoded(self):
  result,proof=self.rewrite(self.source(low='first_word',high='second_word',root='incoming'));self.assertIn('words[0] = (uint)first_word',result);self.assertIn('words[1] = (uint)second_word',result)
 def test_signed_word_storage_uses_raw_uint_bits(self):self.assertIn('words[0] = (uint)low',self.rewrite(self.source(kind='int'))[0])
 def test_later_existing_alias_remains_unchanged(self):
  text=self.source(after='FUN_00003000(&low);');result,_=self.rewrite(text);self.assertIn('FUN_00003000(&low);',result)
 def test_missing_qualified_guard_refuses(self):
  with self.assertRaisesRegex(NativeRefusal,'source proof'):self.rewrite(self.source(),lambda proof:proof.update(qualified_guard_complete=False))
 def test_source_hash_mismatch_refuses(self):
  with self.assertRaisesRegex(NativeRefusal,'source proof'):self.rewrite(self.source(),lambda proof:proof.update(frozen_source_sha256='0'*64))
 def test_changed_inner_coordinates_refuse(self):
  def change(proof):proof['discovery']['source_binding']['inner_view_span'][0]+=1
  with self.assertRaisesRegex(NativeRefusal,'span changed'):self.rewrite(self.source(),change)
 def test_source_must_be_the_frozen_export(self):
  env=SimpleNamespace(inventory=SimpleNamespace(decompiled=lambda entry:'different'))
  with self.assertRaisesRegex(NativeRefusal,'frozen inventory'):bitview.prove_raw_scalar(env,0x1000,self.source(),seed)
 def test_no_shape_does_not_construct_environment(self):
  with patch.object(bitview,'_environment',side_effect=AssertionError('unnecessary environment')):self.assertEqual(bitview.optional_raw_scalar(0x1000,'int f() { return 0; }',None,None,None),('int f() { return 0; }',[]))
 def test_comment_shape_does_not_construct_environment(self):
  text='// (float)(double)CONCAT44(high,low)\nint f() { return 0; }'
  with patch.object(bitview,'_environment',side_effect=AssertionError('comment proof')):self.assertEqual(bitview.optional_raw_scalar(0x1000,text,None,None,seed),(text,[]))
 def test_missing_native_evidence_preserves_exact_source(self):
  text=self.source()
  with patch.object(bitview,'_environment',side_effect=OSError('missing')):self.assertEqual(bitview.optional_raw_scalar(0x1000,text,None,None,seed),(text,[]))
 def test_wrong_target_preserves_exact_source(self):
  text=self.source()
  with patch.object(bitview,'_environment',side_effect=ValueError('wrong hash')):self.assertEqual(bitview.optional_raw_scalar(0x1000,text,None,None,seed),(text,[]))
 def test_refused_guard_preserves_exact_source_without_notes(self):
  text=self.source()
  with patch.object(bitview,'_environment',return_value=None),patch.object(bitview,'prove_raw_scalar',side_effect=NativeRefusal('unknown alias')):self.assertEqual(bitview.optional_raw_scalar(0x1000,text,None,None,seed),(text,[]))
 def test_namespace_dictionary_supports_importlib_seed_modules(self):
  text=self.source()
  with patch.object(bitview,'_environment',return_value=None),patch.object(bitview,'prove_raw_scalar',return_value=self.proof(text)):
   result,notes=bitview.optional_raw_scalar(0x1000,text,None,None,vars(seed));self.assertNotEqual(result,text);self.assertEqual(len(notes),1)

if __name__=='__main__':unittest.main()
