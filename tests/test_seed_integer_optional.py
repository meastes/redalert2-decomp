from pathlib import Path
import os,sys,unittest,json,hashlib
from unittest.mock import patch
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools/original_target'))
for name in ('RA2_SEED_ALIAS_TOOLS','RA2_SEED_TEST_TOOLS'):
 if os.environ.get(name):sys.path.append(os.environ[name])
import test_seed_integer_candidate as fixtures
import seed_candidate as seed
from seed_integer_candidate import optional_integer_candidate
class Optional(unittest.TestCase):
 def call(self,text):return optional_integer_candidate(0x1000,text,None,Path('unavailable-repository'),seed)
 def test_no_nan_has_no_environment_reads(self):
  with patch('seed_integer_candidate._environment',side_effect=AssertionError('Unexpected I/O')):self.assertEqual(self.call('void f() {}'),('void f() {}',[]))
 def test_ineligible_nan_has_no_environment_reads(self):
  text='float NAN; void f() { NAN = 1.0f; }'
  with patch('seed_integer_candidate._environment',side_effect=AssertionError('Unexpected I/O')):self.assertEqual(self.call(text),(text,[]))
 def test_missing_environment_keeps_text_and_notes_identical(self):
  env,text=fixtures.Candidates().fixture()
  with patch('seed_integer_candidate._environment',side_effect=FileNotFoundError('Unavailable private inputs')):self.assertEqual(self.call(text),(text,[]))
 def test_invalid_environment_keeps_text_and_notes_identical(self):
  env,text=fixtures.Candidates().fixture()
  with patch('seed_integer_candidate._environment',side_effect=ValueError('Wrong supplied executable')):self.assertEqual(self.call(text),(text,[]))
 def test_unknown_native_roles_keep_text_identical(self):
  env,text=fixtures.Candidates().fixture(threshold=0)
  with patch('seed_integer_candidate._environment',return_value=(env,{},None,{})):self.assertEqual(self.call(text),(text,[]))
 def test_actual_authored_native_guard_emits_only_hypothesis_note(self):
  env,text=fixtures.Candidates().fixture()
  with patch('seed_integer_candidate._environment',return_value=(env,{},None,{})):result,notes=self.call(text)
  self.assertNotIn('NAN',result);self.assertEqual(len(notes),1);self.assertIn('Scratch hypothesis',notes[0]);self.assertIn('alias closure unproved',notes[0])
 def test_globals_dictionary_parser_supported(self):
  env,text=fixtures.Candidates().fixture()
  with patch('seed_integer_candidate._environment',return_value=(env,{},None,{})):result,notes=optional_integer_candidate(0x1000,text,None,Path('unavailable-repository'),vars(seed))
  self.assertNotIn('NAN',result)
 def test_nan_macro_refuses_before_environment(self):
  env,text=fixtures.Candidates().fixture();text='#define NAN 0.0f\n'+text
  with patch('seed_integer_candidate._environment',side_effect=AssertionError('Unexpected I/O')):self.assertEqual(self.call(text),(text,[]))
class Locations(unittest.TestCase):
 def environment(self,configured=False,wrong=False):
  from seed_integer_candidate import _environment
  root=Path(__file__).resolve().parents[1]/'build/test-integer-config'/self._testMethodName;root.mkdir(parents=True,exist_ok=True)
  library=root/'manifests/original_target/library.json';library.parent.mkdir(parents=True,exist_ok=True);data=b'authored archive location fixture';wanted=hashlib.sha256(data).hexdigest();default=root/'private/original-target-20261020/toolchain/media-sp3/VC98/LIB/LIBCMT.LIB';default.parent.mkdir(parents=True,exist_ok=True);default.write_bytes(b'wrong archive' if wrong else data)
  library.write_text(json.dumps(dict(regions=[dict(archive='LIBCMT.LIB',archive_role='VC6 SP3',archive_sha256=wanted)])),encoding='utf-8')
  config=dict(executable='authored-target.exe')
  if configured:
   wrong_dir=root/'configured';wrong_dir.mkdir(exist_ok=True);(wrong_dir/'LIBCMT.LIB').write_bytes(b'wrong configured archive');config['libraries']=[str(wrong_dir)]
  path=root/'config.json';path.write_text(json.dumps(config),encoding='utf-8')
  with patch.dict(os.environ,{'RA2_CONFIG':str(path)}),patch('verify_manifest.Audit'),patch('catalog_io.locate',return_value=root/'manifest.json'),patch('catalog_io.load',return_value=dict(regions=[],headers=[])):
   return _environment(root,None)[3],default
 def test_default_sp3_media_location_supported(self):archives,path=self.environment();self.assertEqual(archives,{'LIBCMT.LIB':path})
 def test_wrong_configured_archive_does_not_hide_pinned_default(self):archives,path=self.environment(configured=True);self.assertEqual(archives,{'LIBCMT.LIB':path})
 def test_wrong_archive_hash_supplies_no_abi_contract(self):archives,path=self.environment(wrong=True);self.assertEqual(archives,{})
if __name__=='__main__':unittest.main()
