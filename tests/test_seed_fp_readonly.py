from pathlib import Path
from types import SimpleNamespace
import hashlib,struct,sys,unittest
import capstone
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools/original_target'))
from seed_fp_native import NativeProofs,NativeRefusal,X
from seed_fp_effects import readonly_tree,readonly_call
import test_seed_fp_effects as effects
ENTRY=0x1000;CHILD=0x2000

def call(prefix,target,suffix):
    prefix=bytes.fromhex(prefix);return prefix+b'\xe8'+struct.pack('<i',target-ENTRY-len(prefix)-5)+bytes.fromhex(suffix)

class ReadonlyDiscovery(unittest.TestCase):
 def env(self,bodies):
  directory=Path(__file__).resolve().parents[1]/'build/test-seed-fp-readonly'/self._testMethodName;rows={}
  decoder=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);decoder.detail=True
  for entry,body in bodies.items():
   name='src/recovered/Toy_%x.cpp'%entry;source=directory/name;source.parent.mkdir(parents=True,exist_ok=True);source.write_text('int toy_%x() { return 0; }\n'%entry,encoding='utf-8');rows[entry]=dict(source=name,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),body_bytes=len(body),body_sha256=hashlib.sha256(body).hexdigest())
  def original(at,size):
   for entry,body in bodies.items():
    if entry<=at and at+size<=entry+len(body):return body[at-entry:at-entry+size]
   raise NativeRefusal('Synthetic read escaped fixture bodies')
  inventory=SimpleNamespace(function=lambda entry:(None,dict(contiguous=True,ranges=[dict(start='%08x'%entry,end_inclusive='%08x'%(entry+len(bodies[entry])-1),bytes_hex=bodies[entry].hex())])))
  return NativeProofs(directory,inventory,SimpleNamespace(original=original,decoder=decoder),rows)
 def tree(self,code,ecx=False):return readonly_tree(self.env({ENTRY:bytes.fromhex(code)}),ENTRY,ecx)
 def spans(self,contract):return {(read['space'],read['offset'],read['size']) for read in contract['reads']}
 def test_input_read_range_inferred(self):self.assertEqual(self.spans(self.tree('8b4424048b4004c20400')),{('input',4,4)})
 def test_ecx_input_read_range_inferred(self):self.assertEqual(self.spans(self.tree('8b4104c3',True)),{('ecx',4,4)})
 def test_saved_register_input_read_is_explicit(self):self.assertEqual(self.spans(self.tree('8b4604c3')),{('saved_%d'%X.X86_REG_ESI,4,4)})
 def test_branch_read_ranges_are_unioned(self):self.assertEqual(self.spans(self.tree('8b44240485c974048b10eb038b500433c0c20400')),{('input',0,4),('input',4,4)})
 def test_nested_direct_input_range_propagates(self):
  parent=call('8b44240450',CHILD,'33c0c20400');child=bytes.fromhex('8b4424048b4004c20400');c=readonly_tree(self.env({ENTRY:parent,CHILD:child}),ENTRY);self.assertEqual(self.spans(c),{('input',4,4)});self.assertEqual(len(c['nested_calls']),1)
 def test_nested_owned_numeric_point_buffer_passes(self):
  parent=call('83ec088b44240c8b108914248b5004895424048bc450',CHILD,'83c40833c0c20400');child=bytes.fromhex('8b4424048b108b4004c20400');c=readonly_tree(self.env({ENTRY:parent,CHILD:child}),ENTRY);self.assertEqual(self.spans(c),{('input',0,4),('input',4,4)})
 def test_nested_owned_buffer_pointer_payload_refuses(self):
  parent=call('83ec048d042489042450',CHILD,'83c40433c0c3');child=bytes.fromhex('8b4424048b0033c0c20400')
  with self.assertRaisesRegex(NativeRefusal,'Pointer bits in nested'):readonly_tree(self.env({ENTRY:parent,CHILD:child}),ENTRY)
 def test_nested_unmodeled_eax_input_read_refuses(self):
  parent=call('8b44240450',CHILD,'33c0c20400');child=bytes.fromhex('8bc8b800000000c20400')
  with self.assertRaisesRegex(NativeRefusal,'unmodeled frame register'):readonly_tree(self.env({ENTRY:parent,CHILD:child}),ENTRY)
 def test_recursive_call_graph_refuses(self):
  parent=call('ff742404',ENTRY,'33c0c20400')
  with self.assertRaisesRegex(NativeRefusal,'Recursive'):readonly_tree(self.env({ENTRY:parent}),ENTRY)
 def test_dynamic_input_index_refuses(self):
  with self.assertRaisesRegex(NativeRefusal,'Unbounded'):self.tree('8b4424048b0c9033c0c20400')
 def test_negative_input_offset_refuses(self):
  with self.assertRaisesRegex(NativeRefusal,'footprint'):self.tree('8b4424048b40fcc20400')
 def test_extra_native_argument_read_refuses(self):
  with self.assertRaisesRegex(NativeRefusal,'readonly stack'):self.tree('8b44240833c0c20400')
 def test_two_argument_slots_are_not_guessed(self):
  with self.assertRaisesRegex(NativeRefusal,'argument extent'):self.tree('33c0c20800')
 def test_input_pointee_write_refuses(self):
  with self.assertRaisesRegex(NativeRefusal,'input-pointer write'):self.tree('8b442404c7000000000033c0c20400')
 def test_aligned_frame_read_footprint_independent_of_entry_alignment(self):
  c=self.tree('558bec83e4f883ec088b45088b40048be55dc20400');self.assertEqual(c['verified_entry_alignments'],[0,4]);self.assertEqual(self.spans(c),{('input',4,4)})
 def test_fp_input_read_range_inferred(self):self.assertEqual(self.spans(self.tree('8b442404d900d91d0040000033c0c20400')),{('input',0,4)})

 def test_partial_pointer_register_store_cannot_restore_full_argument_alias(self):
  with self.assertRaisesRegex(NativeRefusal,'New alias'):self.tree('8b442404668944240433c0c20400')
 def test_partial_pointer_memory_read_is_not_exact_word_alias(self):
  with self.assertRaisesRegex(NativeRefusal,'New alias'):self.tree('668b442404668944240433c0c20400')
 def test_nested_argument_reuse_is_recorded_in_parent_owned_stack(self):
  parent=call('ff742404',CHILD,'33c0c20400');child=bytes.fromhex('66c7442404000066c7442406000033c0c20400');c=readonly_tree(self.env({ENTRY:parent,CHILD:child}),ENTRY);self.assertTrue(any(kind=='stack' and where==-4 for at,kind,where,size in c['writes']))
 def test_nested_argument_reuse_does_not_create_definite_old_pointer(self):
  parent=call('83ec088d442404890424',CHILD,'83ec048b0c24c7010000000083c40833c0c3');child=bytes.fromhex('c74424040000000033c0c20400')
  with self.assertRaisesRegex(NativeRefusal,'input-pointer write'):readonly_tree(self.env({ENTRY:parent,CHILD:child}),ENTRY)
 def test_nested_pointer_to_return_address_is_not_numeric_private_buffer(self):
  parent=call('8d042450',CHILD,'33c0c3');child=bytes.fromhex('8b4424048b0033c0c20400')
  with self.assertRaisesRegex(NativeRefusal,'outside private'):readonly_tree(self.env({ENTRY:parent,CHILD:child}),ENTRY)

class ReadonlyCallBinding(unittest.TestCase):
 def prepare(self):
  return effects.CallBinding().prepare('8b4424048b4004c20400',4)
 def test_exact_range_does_not_require_unread_adjacent_word_numeric(self):
  m,env=self.prepare();m.memory.update({at:frozenset({('frame',-20)}) for at in range(-24,-20)});m.memory.update({at:frozenset({('frame',-8)}) for at in range(-20,-16)});event=readonly_call(m,env,ENTRY);self.assertEqual(event['reads'][0]['offset'],4)
 def test_pointer_in_actual_read_range_refuses(self):
  m,env=self.prepare();m.memory.update({at:frozenset({('frame',-20)}) for at in range(-24,-20)});m.memory.update({at:frozenset({('frame',-8)}) for at in range(-16,-12)})
  with self.assertRaisesRegex(NativeRefusal,'numeric input'):readonly_call(m,env,ENTRY)
 def test_live_eax_is_proved_overwritten(self):
  m,env=self.prepare();m.memory.update({at:frozenset({('frame',-20)}) for at in range(-24,-20)});m.regs[X.X86_REG_EAX]=frozenset({('frame',-16)});self.assertEqual(readonly_call(m,env,ENTRY)['kind'],'inferred read-only')

 def test_argument_reuse_preserves_alias_and_scalar_alternative(self):
  from seed_fp_alias import SCALAR,frames
  m,env=effects.CallBinding().prepare('8b4c240466c7442404000066c7442406000033c0c20400',4);m.memory.update({at:frozenset({('frame',-16)}) for at in range(-24,-20)});readonly_call(m,env,ENTRY);self.assertTrue(SCALAR<=m.memory[-24]);self.assertEqual(frames(m.memory[-24]),{-16});self.assertEqual(frames(m.regs[X.X86_REG_ECX]),{-16})
 def test_argument_reuse_over_saved_capture_refuses_transactionally(self):
  m,env=effects.CallBinding().prepare('c74424040000000033c0c20400',4);m.height=-8;m.memory.update({at:frozenset({('frame',-16)}) for at in range(-8,-4)});before=dict(m.memory)
  with self.assertRaisesRegex(NativeRefusal,'clobbers saved capture'):readonly_call(m,env,ENTRY)
  self.assertEqual(m.memory,before);self.assertFalse(m.write_events)
 def test_automatic_direct_selector_discovers_readonly_fallback(self):
  from seed_fp_effects import direct_call
  from types import SimpleNamespace
  m,env=self.prepare();m.memory.update({at:frozenset({('frame',-20)}) for at in range(-24,-20)});self.assertEqual(direct_call(m,env,SimpleNamespace())['kind'],'inferred read-only')
 def test_automatic_direct_selector_discovers_writer(self):
  from seed_fp_effects import direct_call
  from types import SimpleNamespace
  m,env=effects.CallBinding().prepare('c70100000000c3');m.regs[X.X86_REG_ECX]=frozenset({('frame',-20)});self.assertEqual(direct_call(m,env,SimpleNamespace())['kind'],'inferred native effects')

if __name__=='__main__':unittest.main()
