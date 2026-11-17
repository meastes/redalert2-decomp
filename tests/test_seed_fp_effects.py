from pathlib import Path
import sys,unittest
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools/original_target'))
import test_seed_fp_native as fixtures
from seed_fp_native import X,ptr,NativeRefusal

class InferredEffects(unittest.TestCase):
 def infer(self,code,regs=None,args=None):
  env=fixtures.Models().fixture(bytes.fromhex(code));return env.linear_effects(fixtures.ENTRY,regs if regs is not None else {X.X86_REG_ECX:ptr('receiver')},args or {})
 def reject(self,code,regs=None,args=None):
  with self.assertRaises(NativeRefusal):self.infer(code,regs,args)
 def test_discovers_output_extent_without_layout(self):
  c=self.infer('8bc1c70000000000c7400400000000c3');self.assertEqual([(w['space'],w['offset'],w['size']) for w in c['writes']],[('receiver',0,4),('receiver',4,4)]);self.assertFalse(c['roles_or_record_sizes_guessed'])
 def test_sparse_writes_are_exact_not_filled(self):self.assertEqual([w['offset'] for w in self.infer('c70100000000c7410800000000c3')['writes']],[0,8])
 def test_two_output_roots_need_no_role_guess(self):
  c=self.infer('c70100000000c70200000000c3',{X.X86_REG_ECX:ptr('first'),X.X86_REG_EDX:ptr('second')});self.assertEqual({w['space'] for w in c['writes']},{'first','second'})
 def test_fp_input_read_footprint_is_explicit(self):
  c=self.infer('8bc1d94204d918c3',{X.X86_REG_ECX:ptr('destination'),X.X86_REG_EDX:ptr('input')});self.assertEqual([(v['space'],v['offset'],v['size']) for v in c['reads']],[('input',4,4)])
 def test_integer_input_read_footprint_is_explicit(self):
  c=self.infer('8b42048901c3',{X.X86_REG_ECX:ptr('destination'),X.X86_REG_EDX:ptr('input')});self.assertEqual([(v['space'],v['offset'],v['size']) for v in c['reads']],[('input',4,4)])
 def test_negative_input_read_refuses(self):self.reject('8b42fc8901c3',{X.X86_REG_ECX:ptr('destination'),X.X86_REG_EDX:ptr('input')})
 def test_dynamic_input_index_refuses(self):self.reject('8b048a8901c3',{X.X86_REG_ECX:ptr('destination'),X.X86_REG_EDX:ptr('input')})
 def test_write_outside_bounded_window_refuses(self):self.reject('c7810010000000000000c3')
 def test_pointer_arithmetic_return_alias_preserved(self):self.assertEqual(self.infer('c701000000008d4104c3')['end_register_values'][str(X.X86_REG_EAX)],ptr('receiver',4))
 def test_saved_frame_register_restored_as_same_symbol(self):self.assertTrue(self.infer('56c701000000005ec3',{X.X86_REG_ECX:ptr('destination'),X.X86_REG_ESI:ptr('saved_frame')})['callee_saved_and_stack_restored'])
 def test_hidden_stack_output_root_is_discovered(self):
  c=self.infer('8b442404c70000000000c20400',{}, {4:ptr('result')});self.assertEqual(c['writes'][0]['space'],'result');self.assertEqual(c['pop'],4)
 def test_stack_read_beyond_native_argument_extent_refuses(self):self.reject('8b4424088901c20400')
 def test_rep_input_and_output_footprints_are_inferred(self):
  c=self.infer('56578bf28bf9b902000000f3a58bc75f5ec3',{X.X86_REG_ECX:ptr('destination'),X.X86_REG_EDX:ptr('input')});self.assertEqual([(v['space'],v['offset'],v['size']) for v in c['reads']],[('input',0,4),('input',4,4)]);self.assertEqual([v['offset'] for v in c['writes']],[0,4])
 def test_pointer_output_publication_refuses(self):self.reject('8911c3',{X.X86_REG_ECX:ptr('destination'),X.X86_REG_EDX:ptr('input')})
 def test_saved_pointer_add_does_not_erase_taint(self):self.reject('568bc683c00489015ec3')
 def test_two_operand_imul_does_not_erase_pointer_taint(self):self.reject('8bc10fafc28901c3')
 def test_segment_override_absolute_read_refuses(self):self.reject('64a1000000008901c3')
 def test_helper_call_refuses(self):self.reject('e800000000c70100000000c3')
 def test_branching_writer_refuses(self):self.reject('7500c70100000000c3')
 def test_no_write_helper_is_not_misclassified_writer(self):self.reject('8bc1c3')

class CallBinding(unittest.TestCase):
 def prepare(self,code,pop=0):
  import struct
  import test_seed_fp_alias as aliases
  from seed_fp_alias import GP,SCALAR
  m=aliases.Aliases().machine();env=fixtures.Models().fixture(bytes.fromhex(code));at=0x3000;raw=b'\xe8'+struct.pack('<i',fixtures.ENTRY-at-5);ins=next(env.audit.decoder.disasm(raw,at));m.frame['nodes'][at]=ins;m.frame['direct_pops'][at]=pop;m.at=at;m.height=-24;m.regs={reg:SCALAR for reg in GP};m.memory={};return m,env
 def call(self,m,env):
  from seed_fp_effects import linear_call
  return linear_call(m,env,fixtures.ENTRY)
 def test_resolved_write_uses_actual_frame_address(self):
  m,env=self.prepare('c70100000000c3');m.regs[X.X86_REG_ECX]=frozenset({('frame',-20)});event=self.call(m,env);self.assertEqual(m.write_events[0]['frame'],-20);self.assertFalse(event['roles_or_record_sizes_guessed'])
 def test_pointer_payload_read_refuses_before_writes(self):
  m,env=self.prepare('8b42048901c3');m.regs[X.X86_REG_ECX]=frozenset({('frame',-20)});m.regs[X.X86_REG_EDX]=frozenset({('frame',-24)});m.memory.update({offset:frozenset({('frame',-8)}) for offset in range(-20,-16)})
  with self.assertRaisesRegex(NativeRefusal,'numeric input'):self.call(m,env)
  self.assertFalse(m.write_events)
 def test_late_capture_write_refuses_transactionally(self):
  m,env=self.prepare('c70100000000c70200000000c3');m.regs[X.X86_REG_ECX]=frozenset({('frame',-20)});m.regs[X.X86_REG_EDX]=frozenset({('frame',-8)})
  with self.assertRaisesRegex(NativeRefusal,'clobbers'):self.call(m,env)
  self.assertFalse(m.write_events)
 def test_mixed_external_and_frame_write_root_refuses(self):
  from seed_fp_alias import SCALAR
  m,env=self.prepare('c70100000000c3');m.regs[X.X86_REG_ECX]=SCALAR|frozenset({('frame',-20)})
  with self.assertRaisesRegex(NativeRefusal,'Mixed'):self.call(m,env)
 def test_wrong_callee_cannot_reuse_another_contract(self):
  from seed_fp_effects import linear_call
  m,env=self.prepare('c70100000000c3')
  with self.assertRaisesRegex(NativeRefusal,'callee differs'):linear_call(m,env,0x2000)
 def test_unknown_input_pointer_offset_refuses(self):
  m,env=self.prepare('c70100000000c3');m.regs[X.X86_REG_ECX]=frozenset({('frame',None)})
  with self.assertRaisesRegex(NativeRefusal,'offset'):self.call(m,env)
 def test_external_receiver_write_and_frame_argument_read(self):
  m,env=self.prepare('8b4424048b008901c20400',4);m.memory.update({offset:frozenset({('frame',-16)}) for offset in range(-24,-20)});event=self.call(m,env);self.assertFalse(m.write_events);self.assertEqual(event['reads'][0]['space'],'arg_0')
 def test_ambiguous_frame_output_refuses(self):
  m,env=self.prepare('c70100000000c3');m.regs[X.X86_REG_ECX]=frozenset({('frame',-20),('frame',-24)})
  with self.assertRaisesRegex(NativeRefusal,'Ambiguous'):self.call(m,env)

class InputKills(unittest.TestCase):
 def prove(self,code,reg=X.X86_REG_EAX):
  from seed_fp_effects import prove_killed_inputs
  env=fixtures.Models().fixture(bytes.fromhex(code));return prove_killed_inputs(env,fixtures.ENTRY,{reg})
 def test_full_write_kills_unused_pointer(self):self.assertTrue(self.prove('b800000000c3')['killed_before_any_use'])
 def test_xor_self_kills_unused_pointer(self):self.assertTrue(self.prove('33c0c3')['killed_before_any_use'])
 def test_edx_full_write_is_checked(self):self.assertTrue(self.prove('ba00000000c3',X.X86_REG_EDX)['killed_before_any_use'])
 def test_copy_before_overwrite_refuses(self):
  with self.assertRaisesRegex(NativeRefusal,'read before'):self.prove('8bc8b800000000c3')
 def test_partial_overwrite_is_not_full_kill(self):
  with self.assertRaisesRegex(NativeRefusal,'return'):self.prove('66b80000c3')
 def test_branch_leaving_input_live_refuses(self):
  with self.assertRaises(NativeRefusal):self.prove('7505b800000000c3')
 def test_call_before_overwrite_refuses(self):
  with self.assertRaisesRegex(NativeRefusal,'call/return'):self.prove('e800000000b800000000c3')
 def test_untouched_return_alias_refuses(self):
  with self.assertRaisesRegex(NativeRefusal,'return'):self.prove('c3')

if __name__=='__main__':unittest.main()
