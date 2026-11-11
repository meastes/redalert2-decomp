from pathlib import Path
import unittest,json,hashlib,importlib.util,contextlib,io,sys
from types import SimpleNamespace
from unittest.mock import patch
import capstone
r=Path(__file__).resolve().parent

sys.path.insert(0,str(r.parent/'tools/original_target'))
from seed_fp_native import NativeProofs,NativeRefusal,ptr
ENTRY=0x1000;X=capstone.x86_const
# These are authored toy x86 fixtures for the analysis models, never C/C++
# candidates or original-game instruction arrays.
MOVE_OUT=bytes.fromhex('8bc1')
WRITES12=bytes.fromhex('c70000000000c7400400000000c7400800000000')
RETURN=bytes.fromhex('c3');ZERO_RET4=bytes.fromhex('33c0c20400')
class Models(unittest.TestCase):
 def fixture(self,body,source_hash_bad=False,body_hash_bad=False):
  directory=r.parent/'build/test-seed-fp-native'/self._testMethodName;source=directory/'src/recovered/Toy.cpp';source.parent.mkdir(parents=True,exist_ok=True);source.write_text('// Authored synthetic proof-model fixture; canonical acceptance is mocked.\nint toy() { return 0; }\n',encoding='utf-8');row=dict(source='src/recovered/Toy.cpp',source_sha256='0'*64 if source_hash_bad else hashlib.sha256(source.read_bytes()).hexdigest(),body_bytes=len(body),body_sha256='0'*64 if body_hash_bad else hashlib.sha256(body).hexdigest());decoder=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);decoder.detail=True
  audit=SimpleNamespace(decoder=decoder,original=lambda at,size:body[at-ENTRY:at-ENTRY+size]);inventory=SimpleNamespace(function=lambda entry:(None,dict(contiguous=True,ranges=[dict(start='%08x'%ENTRY,end_inclusive='%08x'%(ENTRY+len(body)-1),bytes_hex=body.hex())])))
  return NativeProofs(directory,inventory,audit,{ENTRY:row})
 def output(self,body=None,extent=12,registers=None,**kwargs):
  body=body if body is not None else MOVE_OUT+WRITES12+RETURN
  return self.fixture(body,**kwargs).output_contract(ENTRY,registers or {X.X86_REG_ECX:ptr('out')},{},extent)
 def readonly(self,body=ZERO_RET4,ecx_symbol=False,mod=0,**kwargs):
  return self.fixture(body,**kwargs).readonly_contract(ENTRY,stack_input=True,ecx_symbol=ecx_symbol,entry_mod8=mod)
 def reject_output(self,*args,**kwargs):
  with self.assertRaises(NativeRefusal):self.output(*args,**kwargs)
 def reject_readonly(self,*args,**kwargs):
  with self.assertRaises(NativeRefusal):self.readonly(*args,**kwargs)
 def test_output_full_coverage_and_return_alias(self):self.assertEqual(self.output()['return_alias'],'out+0')
 def test_output_overrun_refuses(self):self.reject_output(MOVE_OUT+WRITES12+bytes.fromhex('c7400c00000000')+RETURN)
 def test_output_underrun_coverage_refuses(self):self.reject_output(MOVE_OUT+bytes.fromhex('c70000000000')+RETURN)
 def test_output_unknown_destination_refuses(self):self.reject_output(MOVE_OUT+WRITES12+bytes.fromhex('c70300000000')+RETURN)
 def test_output_image_write_refuses(self):self.reject_output(MOVE_OUT+WRITES12+bytes.fromhex('c7050040000000000000')+RETURN)
 def test_output_caller_argument_clobber_refuses(self):self.reject_output(MOVE_OUT+WRITES12+bytes.fromhex('c744240400000000')+RETURN)
 def test_output_incoming_saved_register_leak_refuses(self):self.reject_output(MOVE_OUT+bytes.fromhex('8930')+WRITES12+RETURN)
 def test_output_input_pointer_leak_refuses(self):self.reject_output(MOVE_OUT+bytes.fromhex('8910')+WRITES12+RETURN,registers={X.X86_REG_ECX:ptr('out'),X.X86_REG_EDX:ptr('input')})
 def test_output_wrong_return_alias_refuses(self):self.reject_output(MOVE_OUT+WRITES12+bytes.fromhex('b800000000')+RETURN)
 def test_output_saved_register_corruption_refuses(self):self.reject_output(bytes.fromhex('bb01000000')+MOVE_OUT+WRITES12+RETURN)
 def test_output_unknown_call_refuses(self):self.reject_output(MOVE_OUT+WRITES12+bytes.fromhex('e800000000')+RETURN)
 def test_output_branch_refuses(self):self.reject_output(bytes.fromhex('7500')+MOVE_OUT+WRITES12+RETURN)
 def test_output_source_hash_mismatch_refuses(self):self.reject_output(source_hash_bad=True)
 def test_output_body_hash_mismatch_refuses(self):self.reject_output(body_hash_bad=True)
 def test_output_zero_extent_refuses(self):self.reject_output(extent=0)
 def test_output_non_dword_extent_refuses(self):self.reject_output(extent=10)
 def test_readonly_no_writes_passes(self):self.assertTrue(self.readonly()['argument_aliases_only_removed'])
 def test_readonly_image_data_write_passes(self):self.assertTrue(self.readonly(bytes.fromhex('c7050040000000000000')+ZERO_RET4)['writes'])
 def test_readonly_input_pointee_write_refuses(self):self.reject_readonly(bytes.fromhex('8b442404c70000000000')+ZERO_RET4)
 def test_readonly_input_pointer_publication_refuses(self):self.reject_readonly(bytes.fromhex('8b442404a300400000')+ZERO_RET4)
 def test_readonly_saved_register_publication_refuses(self):self.reject_readonly(bytes.fromhex('893500400000')+ZERO_RET4)
 def test_readonly_preserved_ecx_publication_refuses(self):self.reject_readonly(bytes.fromhex('890d00400000')+ZERO_RET4,ecx_symbol=True)
 def test_readonly_new_alias_in_argument_refuses(self):self.reject_readonly(bytes.fromhex('8b44240483c00489442404')+ZERO_RET4)
 def test_readonly_half_cleared_argument_keeps_unknown_fragment(self):
  with self.assertRaisesRegex(NativeRefusal,'New alias'):self.readonly(bytes.fromhex('66c74424040000')+ZERO_RET4)
 def test_readonly_both_halves_cleared_argument_passes(self):
  proof=self.readonly(bytes.fromhex('66c7442404000066c74424060000')+ZERO_RET4);self.assertEqual([w[1] for w in proof['writes']],['owned_argument','owned_argument'])
 def test_readonly_partial_register_pointer_publication_refuses(self):self.reject_readonly(bytes.fromhex('8b44240466b80000a300400000')+ZERO_RET4)
 def test_readonly_returning_input_pointer_refuses(self):self.reject_readonly(bytes.fromhex('8b442404c20400'))
 def test_readonly_unknown_call_refuses(self):self.reject_readonly(bytes.fromhex('e800000000')+ZERO_RET4)
 def test_readonly_source_hash_mismatch_refuses(self):self.reject_readonly(source_hash_bad=True)
 def test_readonly_body_hash_mismatch_refuses(self):self.reject_readonly(body_hash_bad=True)
 def test_readonly_private_aligned_stack_both_entry_cases(self):
  body=bytes.fromhex('558bec83e4f883ec08c70424000000008be55d')+ZERO_RET4
  a=self.readonly(body,mod=0);b=self.readonly(body,mod=4);self.assertEqual(a['writes'][0][2],-16);self.assertEqual(b['writes'][0][2],-12)
 def test_readonly_argument_slot_outside_owned_extent_refuses(self):self.reject_readonly(bytes.fromhex('c744240800000000')+ZERO_RET4)
 def test_readonly_return_address_write_refuses(self):self.reject_readonly(bytes.fromhex('c7042400000000')+ZERO_RET4)
 def test_readonly_cyclic_graph_refuses(self):
  with self.assertRaisesRegex(NativeRefusal,'cyclic'):self.readonly(bytes.fromhex('ebfe'))

 def test_output_partial_pointer_register_publication_refuses(self):self.reject_output(MOVE_OUT+bytes.fromhex('8810')+WRITES12+RETURN,registers={X.X86_REG_ECX:ptr('out'),X.X86_REG_EDX:ptr('input')})
 def test_output_half_pointer_stack_clear_stays_tainted(self):self.reject_output(bytes.fromhex('5283ec0466c744240400008b54240483c408')+MOVE_OUT+bytes.fromhex('8910')+WRITES12+RETURN,registers={X.X86_REG_ECX:ptr('out'),X.X86_REG_EDX:ptr('input')})
 def test_output_pointer_stack_fp_transport_refuses(self):self.reject_output(bytes.fromhex('52d90424d91983c404')+MOVE_OUT+WRITES12+RETURN,registers={X.X86_REG_ECX:ptr('out'),X.X86_REG_EDX:ptr('input')})
 def test_output_saved_register_stack_fp_transport_refuses(self):self.reject_output(bytes.fromhex('56d90424d91983c404')+MOVE_OUT+WRITES12+RETURN)
 def test_readonly_input_pointer_fp_transport_refuses(self):self.reject_readonly(bytes.fromhex('d9442404d91d00400000')+ZERO_RET4)
 def test_readonly_saved_register_fp_transport_refuses(self):self.reject_readonly(bytes.fromhex('56d90424d91d004000005e')+ZERO_RET4)

class EnvironmentTests(unittest.TestCase):
    fixture=Models.fixture
    # Run the same model cases through the explicit environment plus boundary tests.
    def test_source_escape_refused(self):
        env=self.fixture(MOVE_OUT+WRITES12+RETURN);env.rows[ENTRY]['source']='src/recovered/../../escape.cpp'
        with self.assertRaisesRegex(NativeRefusal,'escaped'):env.provenance(ENTRY)
    def test_noncanonical_source_refused(self):
        env=self.fixture(MOVE_OUT+WRITES12+RETURN);env.rows[ENTRY]['source']='build/Toy.cpp'
        with self.assertRaisesRegex(NativeRefusal,'Noncanonical'):env.provenance(ENTRY)
    def test_duplicate_catalog_entry_refused(self):
        env=self.fixture(MOVE_OUT+WRITES12+RETURN);row=dict(env.rows[ENTRY],entry=ENTRY);env=NativeProofs(env.repository,env.inventory,env.audit,[row,row])
        with self.assertRaisesRegex(NativeRefusal,'Ambiguous'):env.provenance(ENTRY)
    def test_missing_provenance_refused(self):
        env=self.fixture(MOVE_OUT+WRITES12+RETURN);env.rows.clear()
        with self.assertRaisesRegex(NativeRefusal,'Missing accepted'):env.provenance(ENTRY)
    def test_incomplete_metadata_refused(self):
        env=self.fixture(MOVE_OUT+WRITES12+RETURN);env.rows[ENTRY].pop('body_sha256')
        with self.assertRaisesRegex(NativeRefusal,'Incomplete'):env.provenance(ENTRY)
    def test_accepted_prefix_is_not_complete_helper_evidence(self):
        env=self.fixture(MOVE_OUT+WRITES12+RETURN);env.rows[ENTRY]['body_bytes']=2;env.rows[ENTRY]['body_sha256']=hashlib.sha256(MOVE_OUT).hexdigest()
        with self.assertRaisesRegex(NativeRefusal,'extent'):env.provenance(ENTRY)
    def test_complete_tail_dispatch_has_provenance_but_no_closed_graph(self):
        env=self.fixture(bytes.fromhex('ff60a0'));self.assertTrue(env.provenance(ENTRY))
        with self.assertRaisesRegex(NativeRefusal,'indirect branch'):env.readonly_contract(ENTRY)
    def test_import_has_no_file_reads_or_proof_runs(self):
        directory=Path(__file__).resolve().parents[1]/'tools/original_target'
        for path in directory.glob('seed_fp_*.py'):
            with self.subTest(module=path.name):
                spec=importlib.util.spec_from_file_location('fresh_'+path.stem+'_no_io',path);module=importlib.util.module_from_spec(spec)
                with patch.object(Path,'read_bytes',side_effect=AssertionError('import-time read')),patch.object(Path,'read_text',side_effect=AssertionError('import-time read')):spec.loader.exec_module(module)

if __name__=='__main__':unittest.main()
