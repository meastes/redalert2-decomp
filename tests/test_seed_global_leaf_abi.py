from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch
import capstone,hashlib,sys,unittest
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools/original_target'))
from seed_guarded_cfg import guarded_graph,CFGRefusal
from seed_global_leaf_abi import global_receiver,global_leaf_cleanup
ENTRY=0x1000

def graph_for(body):
 decoder=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);decoder.detail=True;audit=SimpleNamespace(decoder=decoder,original=lambda at,size:body[at-ENTRY:at-ENTRY+size]);inventory=SimpleNamespace(function=lambda entry:(None,dict(contiguous=True,ranges=[dict(start='%08x'%ENTRY,end_inclusive='%08x'%(ENTRY+len(body)-1),bytes_hex=body.hex())])))
 return guarded_graph(audit,inventory,ENTRY)

class GlobalFlow(unittest.TestCase):
 def prove(self,prefix=bytes.fromhex('8b0d002040008b01'),dispatch=bytes.fromhex('ff9008000000')):
  body=prefix+dispatch+b'\xc3';g=graph_for(body);return global_receiver(g,ENTRY+len(prefix))
 def test_exact_global_load_and_primary_vptr(self):p=self.prove();self.assertEqual(p['global_address'],0x402000);self.assertEqual(p['slot'],2)
 def test_copied_receiver_register_keeps_same_original_value(self):self.assertEqual(self.prove(prefix=bytes.fromhex('8b0d002040008bf18b168bce'),dispatch=bytes.fromhex('ff5204'))['slot'],1)
 def test_reloaded_same_global_is_not_assumed_same_read(self):
  with self.assertRaisesRegex(CFGRefusal,'same original load'):self.prove(prefix=bytes.fromhex('8b0d002040008b018b0d00204000'))
 def test_partial_receiver_register_write_refuses(self):
  with self.assertRaisesRegex(CFGRefusal,'definition'):self.prove(prefix=bytes.fromhex('8b0d002040008b01b100'))
 def test_nonprimary_vptr_offset_refuses(self):
  with self.assertRaisesRegex(CFGRefusal,'primary vptr'):self.prove(prefix=bytes.fromhex('8b0d002040008b4104'))
 def test_fs_global_load_is_not_a_flat_global_pointer(self):
  with self.assertRaisesRegex(CFGRefusal,'operand'):self.prove(prefix=bytes.fromhex('648b0d002040008b01'))
 def test_intervening_call_does_not_preserve_unknown_ecx(self):
  with self.assertRaisesRegex(CFGRefusal,'Nonlocal'):self.prove(prefix=bytes.fromhex('8b0d00204000e8001000008b01'))
 def test_register_indirect_call_is_outside_domain(self):
  with self.assertRaisesRegex(CFGRefusal,'virtual operand'):self.prove(dispatch=bytes.fromhex('ffd0'))
 def test_unaligned_slot_refuses(self):
  with self.assertRaisesRegex(CFGRefusal,'virtual operand'):self.prove(dispatch=bytes.fromhex('ff5002'))
 def test_ambiguous_setup_ingress_refuses(self):
  with self.assertRaisesRegex(CFGRefusal,'ingress'):self.prove(prefix=bytes.fromhex('85db74068b0d002040008b01'))

class GlobalLeafContract(unittest.TestCase):
 def invoke(self,mutate=None,second=bytes.fromhex('c20400')):
  directory=Path(__file__).resolve().parents[1]/'build/test-global-leaf-abi'/self._testMethodName;header=directory/'src/shared/globals.hpp';header.parent.mkdir(parents=True,exist_ok=True);header.write_text('class Root;\nextern Root *surface; // 0x00402000\n',encoding='utf-8');pins={'src/shared/globals.hpp':hashlib.sha256(header.read_bytes()).hexdigest()};known=dict(kind='Root *',header='shared/globals.hpp',forward='class Root;');seed=SimpleNamespace(shared_pointer_types=lambda path:{0x402000:known});bodies={0x2000:bytes.fromhex('c20400'),0x2100:second};decoder=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);decoder.detail=True
  def original(at,size):
   for entry,body in bodies.items():
    if entry<=at and at+size<=entry+len(body):return body[at-entry:at-entry+size]
   raise CFGRefusal('Synthetic helper extent escaped')
  inventory=SimpleNamespace(function=lambda entry:(None,dict(contiguous=True,ranges=[dict(start='%08x'%entry,end_inclusive='%08x'%(entry+len(bodies[entry])-1),bytes_hex=bodies[entry].hex())])));proofs=SimpleNamespace(repository=directory,audit=SimpleNamespace(decoder=decoder,original=original),inventory=inventory,provenance=lambda entry:hashlib.sha256(bodies[entry]).hexdigest());g=graph_for(bytes.fromhex('8b0d002040008b01ff5008c3'));family=[dict(target=entry) for entry in bodies]
  if mutate:mutate(header,pins,known)
  with patch.dict(sys.modules,{'seed_fp_virtual':SimpleNamespace(physical_family=lambda audit,name,slot:family)}):return global_leaf_cleanup(proofs,g,ENTRY+8,seed,pins)
 def test_derived_cleanup_is_only_stack_fact(self):p=self.invoke();self.assertEqual(p['pop'],4);self.assertEqual(len(p['targets']),2);self.assertFalse(p['memory_effects_proved'])
 def test_header_must_be_canonically_pinned(self):
  with self.assertRaisesRegex(CFGRefusal,'canonically pinned'):self.invoke(lambda header,pins,known:pins.clear())
 def test_header_hash_must_match_current_bytes(self):
  with self.assertRaisesRegex(CFGRefusal,'hash differs'):self.invoke(lambda header,pins,known:header.write_text(header.read_text()+'// changed'))
 def test_declaration_rebound_even_if_registry_is_stale(self):
  def change(header,pins,known):header.write_text('extern Other *surface; // 0x00402000\n');pins['src/shared/globals.hpp']=hashlib.sha256(header.read_bytes()).hexdigest()
  with self.assertRaisesRegex(CFGRefusal,'absent/changed'):self.invoke(change)
 def test_primitive_pointer_is_not_class_virtual_evidence(self):
  with self.assertRaisesRegex(CFGRefusal,'existing class'):self.invoke(lambda header,pins,known:known.update(forward=None))
 def test_all_physical_targets_must_agree(self):
  with self.assertRaisesRegex(CFGRefusal,'set disagrees'):self.invoke(second=bytes.fromhex('c20800'))
 def test_nonleaf_target_does_not_get_cleanup_from_a_guess(self):
  with self.assertRaisesRegex(CFGRefusal,'not native call-free'):self.invoke(second=bytes.fromhex('e800100000c20400'))
 def test_unrestored_leaf_frame_refuses(self):
  with self.assertRaisesRegex(CFGRefusal,'disagree'):self.invoke(second=bytes.fromhex('83ec04c20400'))

if __name__=='__main__':unittest.main()
