from pathlib import Path
import os,sys,json,hashlib,copy,unittest
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools/original_target'))
for name in ('RA2_SEED_ALIAS_TOOLS','RA2_SEED_TEST_TOOLS'):
 if os.environ.get(name):sys.path.append(os.environ[name])
from types import SimpleNamespace
import capstone
import seed_candidate as seed
import test_seed_integer_scope as scopefixtures
import test_seed_integer_native as nativefixtures
ENTRY=nativefixtures.ENTRY
from seed_integer_candidate import draft_integer_candidate,discover_integer_candidate,emit_integer_candidate
class Candidates(unittest.TestCase):
 def fixture(self,text=None,**kwargs):
  graph,frames,binding=nativefixtures.NativeLifetime().fixture(**kwargs);raw=graph['raw'];decoder=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);decoder.detail=True
  text=text or scopefixtures.SourceScope().source()
  inv=SimpleNamespace(decompiled=lambda entry:text,function=lambda entry:(None,dict(contiguous=True,ranges=[dict(start='%08x'%ENTRY,end_inclusive='%08x'%(ENTRY+len(raw)-1),bytes_hex=raw.hex())])))
  proof=SimpleNamespace(rows={},audit=SimpleNamespace(decoder=decoder,original=lambda at,size:raw[at-ENTRY:at-ENTRY+size]),inventory=inv)
  return proof,text or scopefixtures.SourceScope().source()
 def draft(self,**kwargs):
  proof,text=self.fixture(**kwargs);return draft_integer_candidate(proof,ENTRY,text,seed,{})
 def test_generic_native_and_source_guard_emit_real_integer_local(self):
  result,p=self.draft();self.assertIn('int '+p['integer_name']+';',result);self.assertIn(p['integer_name']+' = -2;',result);self.assertNotIn('NAN',result)
 def test_later_float_lifetime_and_declaration_are_preserved(self):
  result,p=self.draft();self.assertIn('float carrier;',result);self.assertIn('carrier = DAT_00123456;',result)
 def test_subtraction_is_explicit_wrapping_dword(self):
  result,p=self.draft();self.assertIn('(int)((uint)counter - (uint)'+p['integer_name']+')',result)
 def test_native_payload_not_fixed_to_minus_two(self):
  result,p=self.draft(word=0xffc00001);self.assertIn(p['integer_name']+' = '+str(0xffc00001-(1<<32))+';',result)
 def test_positive_native_nan_label_supported(self):
  result,p=self.draft(text=scopefixtures.SourceScope().source().replace('-NAN','NAN'),word=0x7fc00001,sign=0);self.assertIn(p['integer_name']+' = '+str(0x7fc00001)+';',result)
 def test_candidate_is_never_an_alias_or_acceptance_claim(self):
  result,p=self.draft();self.assertTrue(p['source_emitted_as_scratch_hypothesis']);self.assertFalse(p['alias_effect_closure_complete']);self.assertFalse(p['accepted_source_emission_allowed']);self.assertFalse(p['body_acceptance_claimed'])
 def test_changed_source_refuses(self):
  env,text=self.fixture();p=discover_integer_candidate(env,ENTRY,text,seed,{})
  with self.assertRaisesRegex(ValueError,'Frozen candidate source changed'):emit_integer_candidate(text+' ',p,seed)
 def test_changed_binding_refuses(self):
  env,text=self.fixture();p=discover_integer_candidate(env,ENTRY,text,seed,{});p['source']['counter']='other'
  with self.assertRaisesRegex(ValueError,'Source binding changed'):emit_integer_candidate(text,p,seed)
 def test_missing_native_roles_refuses(self):
  env,text=self.fixture();p=discover_integer_candidate(env,ENTRY,text,seed,{});p['native']['native_source_roles_verified']=False
  with self.assertRaisesRegex(ValueError,'Missing native value/control'):emit_integer_candidate(text,p,seed)
 def test_real_float_native_word_refuses(self):
  env,text=self.fixture();p=discover_integer_candidate(env,ENTRY,text,seed,{});p['native']['word']=0x3f800000
  with self.assertRaisesRegex(ValueError,'Native word is not'):emit_integer_candidate(text,p,seed)
 def test_payload_signed_value_mismatch_refuses(self):
  env,text=self.fixture();p=discover_integer_candidate(env,ENTRY,text,seed,{});p['native']['signed_value']=-1
  with self.assertRaisesRegex(ValueError,'word/signed value disagree'):emit_integer_candidate(text,p,seed)
 def test_name_collision_gets_a_fresh_identifier(self):
  env,text=self.fixture();p=discover_integer_candidate(env,ENTRY,text,seed,{});stem='integer_cache_%08x'%p['native']['initializer'];text=scopefixtures.SourceScope().source(prefix='int '+stem+';\n');result,p=self.draft(text=text);self.assertEqual(p['integer_name'],stem+'_1')
 def test_unsigned_counter_keeps_wrapping_expression(self):
  result,p=self.draft(text=scopefixtures.SourceScope().source(kind='unsigned int'));self.assertIn('(uint)counter - (uint)',result)
 def test_wrong_native_predicate_never_emits(self):
  with self.assertRaises(ValueError):self.draft(threshold=0)
 def test_discovery_requires_the_exact_frozen_export(self):
  env,text=self.fixture()
  with self.assertRaisesRegex(ValueError,'not the frozen inventory export'):discover_integer_candidate(env,ENTRY,text+' ',seed,{})
if __name__=='__main__':unittest.main()
