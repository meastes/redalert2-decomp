"""Authored native fixtures for the reusable qualified caller interpreter."""
from pathlib import Path
import sys,unittest
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools/original_target'))
import test_seed_fp_capture as fixtures
from seed_fp_alias import PrefixMachine,merge,constant,frames,SCALAR,X
from seed_fp_native import NativeRefusal
from seed_fp_capture import initial_word_capture

class Aliases(unittest.TestCase):
 def machine(self,extra=b''):
  helper=fixtures.Discovery();body=fixtures.PREFIX+extra+fixtures.FP+fixtures.CALL+fixtures.TAIL;frame=helper.frame(body);capture=initial_word_capture(frame,fixtures.ENTRY);target=next(at for at,i in frame['nodes'].items() if i.mnemonic=='fld');return PrefixMachine(frame,fixtures.ENTRY,target,capture)
 def run_fixture(self,extra=b'',effect=None):return self.machine(extra).run(effect)
 def reject(self,extra,reason=None,effect=None):
  with self.assertRaisesRegex(NativeRefusal,reason or '.') :self.run_fixture(bytes.fromhex(extra),effect)
 def opaque(self,machine,ins):machine.opaque(4);return dict(kind='authored toy normal ABI')
 def test_scalar_capture_passes_emission_disabled(self):self.assertFalse(self.run_fixture()['source_emission_allowed'])
 def test_capture_dword_clobber_refuses(self):self.reject('c7042400000000','capture clobbered')
 def test_capture_partial_clobber_refuses(self):self.reject('66c74424020000','capture clobbered')
 def test_frame_pointer_global_publication_refuses(self):self.reject('8d0424a300400000','published')
 def test_partial_pointer_register_publication_refuses(self):self.reject('8d042466b80000a300400000','published')
 def test_unknown_frame_index_refuses(self):self.reject('8d04248b0408','constant')
 def test_unknown_frame_offset_refuses(self):self.reject('8d042431d001d08b00','address')
 def test_fp_pointer_bits_transport_refuses(self):self.reject('83ec048d442404890424d90424d91d0040000083c404','FP operation')
 def test_rep_capture_overwrite_refuses(self):self.reject('8d7424048d3c24b901000000f3a5','capture clobbered')
 def test_rep_unknown_count_refuses(self):self.reject('8d7424048d3c24f3a5','constant')
 def test_call_without_provider_refuses(self):self.reject('6a00e800100000','provider')
 def test_opaque_call_with_frame_register_refuses(self):self.reject('8d04246a00e800100000','Frame register',self.opaque)
 def test_opaque_call_with_frame_stack_argument_refuses(self):self.reject('8d04245033c0e800100000','Frame stack argument',self.opaque)
 def test_opaque_call_without_frame_aliases_passes(self):self.assertEqual(len(self.run_fixture(bytes.fromhex('6a00e800100000'),self.opaque)['calls']),1)
 def test_call_provider_must_return_evidence(self):self.reject('6a00e800100000','establish',lambda machine,ins:None)
 def test_constant_not_definite_on_unknown_join(self):
  regs,mem=merge([({X.X86_REG_ECX:frozenset({('constant',12)})},{}),({}, {})])
  with self.assertRaises(NativeRefusal):constant(regs[X.X86_REG_ECX])
 def test_equal_constant_on_both_paths_remains_definite(self):
  a=({X.X86_REG_ECX:frozenset({('constant',12)})},{})
  self.assertEqual(constant(merge([a,a])[0][X.X86_REG_ECX]),12)
 def test_pointer_alias_survives_scalar_join(self):
  value=frozenset({('frame',-8)})
  regs,mem=merge([({X.X86_REG_EAX:value},{-12:value}),({}, {})]);self.assertEqual(frames(regs[X.X86_REG_EAX]),{-8});self.assertEqual(frames(mem[-12]),{-8})
 def test_partial_memory_clear_preserves_unknown_pointer_fragment(self):
  m=self.machine();m.at=0;m.height=-16;m.regs={};m.memory={};m.store({-16},4,frozenset({('frame',-8)}),'toy');m.store({-16},2,SCALAR,'toy');self.assertEqual(frames(m.readmem({-16},4)),{None})
 def test_complete_memory_clear_removes_pointer_alias(self):
  m=self.machine();m.at=0;m.height=-16;m.regs={};m.memory={};m.store({-16},4,frozenset({('frame',-8)}),'toy');m.store({-16},2,SCALAR,'toy');m.store({-14},2,SCALAR,'toy');self.assertFalse(frames(m.readmem({-16},4)))
 def test_ambiguous_write_destination_refuses_strong_update(self):
  m=self.machine();m.at=0;m.height=-24;m.regs={};m.memory={}
  with self.assertRaisesRegex(NativeRefusal,'weak memory update'):m.store({-20,-24},4,SCALAR,'toy')
 def test_mixed_frame_and_nonframe_base_refuses(self):
  m=self.machine();m.height=-16;m.regs={X.X86_REG_ESI:frozenset({('frame',-16)})|SCALAR};m.memory={}
  operand=m.frame['nodes'][fixtures.ENTRY+5].operands[1]
  with self.assertRaisesRegex(NativeRefusal,'Mixed'):m.addresses(operand)
 def test_memory_is_not_unverified_constant_source(self):
  m=self.machine();m.at=0;m.height=-16;m.regs={};m.memory={};m.store({-16},4,frozenset({('constant',12)}),'toy')
  with self.assertRaises(NativeRefusal):constant(m.readmem({-16},4))

if __name__=='__main__':unittest.main()
