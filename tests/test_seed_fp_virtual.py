from pathlib import Path
from types import SimpleNamespace
import os,struct,sys,unittest
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools/original_target'))
if os.environ.get('RA2_SEED_TEST_TOOLS'):sys.path.append(os.environ['RA2_SEED_TEST_TOOLS'])
import seed_candidate as seed
from seed_fp_native import NativeRefusal,X
from seed_fp_virtual import physical_family,ReceiverFlow,pointer_result
import test_seed_fp_capture as fixtures
BASE=0x400000

class PhysicalFamily(unittest.TestCase):
 def fixture(self):
  self.data=bytearray(0x6000)
  self.put(BASE+0x3100,'I',BASE+0x1100)
  for at,name in ((BASE+0x3200,'Root'),(BASE+0x3280,'Derived')):
   self.put(at,'II',BASE+0x3100,0);raw=('.?AV'+name+'@@').encode()+b'\0';self.data[at+8-BASE-0x3000:at+8-BASE-0x3000+len(raw)]=raw
  for table,loc,td,chd,array,bases in [(0x4004,0x4200,0x3200,0x4240,0x4280,[0x42c0]),(0x5004,0x5200,0x3280,0x5240,0x5280,[0x52c0,0x5300])]:
   self.put(BASE+table-4,'III',BASE+loc,BASE+0x1200,BASE+0x1240);self.put(BASE+loc,'IIIII',0,0,0,BASE+td,BASE+chd);self.put(BASE+chd,'IIII',0,0,len(bases),BASE+array)
   for index,bcd in enumerate(bases):
    self.put(BASE+array+4*index,'I',BASE+bcd);self.put(BASE+bcd,'IIiiiI',BASE+(td if index==0 else 0x3200),len(bases)-1 if index==0 else 0,0,-1,0,0)
  sections=[SimpleNamespace(VirtualAddress=0x1000,SizeOfRawData=0x1000,Misc_VirtualSize=0x1000,Characteristics=0x60000020),SimpleNamespace(VirtualAddress=0x3000,SizeOfRawData=len(self.data),Misc_VirtualSize=len(self.data),Characteristics=0xc0000040)]
  def original(at,size):
   if BASE+0x3000<=at and at+size<=BASE+0x9000:return bytes(self.data[at-BASE-0x3000:at-BASE-0x3000+size])
   raise NativeRefusal('Synthetic RTTI read escaped')
  return SimpleNamespace(base=BASE,pe=SimpleNamespace(sections=sections),original=original)
 def put(self,at,fmt,*values):struct.pack_into('<'+fmt,self.data,at-BASE-0x3000,*values)
 def reject(self,mutation,slot=1,reason=None):
  audit=self.fixture();mutation()
  with self.assertRaisesRegex(NativeRefusal,reason or '.'):physical_family(audit,'Root',slot)
 def test_derives_family_from_physical_hierarchy_without_manifest(self):
  audit=self.fixture();rows=physical_family(audit,'Root',1);self.assertEqual([row['class_name'] for row in rows],['Root','Derived']);self.assertEqual({row['target'] for row in rows},{BASE+0x1240})
 def test_nonzero_base_offset_refuses(self):self.reject(lambda:self.put(BASE+0x5308,'i',4),reason='nonzero')
 def test_virtual_base_adjustment_refuses(self):self.reject(lambda:self.put(BASE+0x530c,'i',0),reason='virtual')
 def test_unknown_requested_base_descriptor_flags_refuse(self):self.reject(lambda:self.put(BASE+0x5314,'I',2),reason='ambiguous')
 def test_malformed_hierarchy_refuses(self):self.reject(lambda:self.put(BASE+0x5248,'I',0),reason='hierarchy')
 def test_self_descriptor_mismatch_refuses(self):self.reject(lambda:self.put(BASE+0x52c0,'I',BASE+0x3200),reason='self descriptor')
 def test_slot_past_code_pointer_run_refuses(self):self.reject(lambda:self.put(BASE+0x4008,'I',0),reason='slot exceeds')
 def test_noncode_virtual_target_refuses(self):self.reject(lambda:self.put(BASE+0x5008,'I',BASE+0x6000),reason='code-pointer')
 def test_construction_displacement_refuses(self):self.reject(lambda:self.put(BASE+0x5208,'I',4),reason='construction')
 def test_missing_primary_for_derived_type_refuses(self):self.reject(lambda:self.put(BASE+0x5204,'I',4),reason='primary table')
 def test_type_info_header_mismatch_refuses(self):self.reject(lambda:self.put(BASE+0x3284,'I',1),reason='type_info')
 def test_unknown_family_refuses(self):
  with self.assertRaisesRegex(NativeRefusal,'Missing'):physical_family(self.fixture(),'Other',1)
 def test_unbounded_slot_refuses(self):
  with self.assertRaises(NativeRefusal):physical_family(self.fixture(),'Root',-1)
 def test_ambiguous_root_bases_refuse(self):
  audit=self.fixture();self.put(BASE+0x5248,'I',3);self.put(BASE+0x5288,'I',BASE+0x5340);self.put(BASE+0x5340,'IIiiiI',BASE+0x3200,0,0,-1,0,0)
  with self.assertRaisesRegex(NativeRefusal,'ambiguous'):physical_family(audit,'Root',1)

class Flow(unittest.TestCase):
 def machine(self,kind='spill',partial=False,slot=4,extra=b''):
  entry=fixtures.ENTRY;body=bytearray(bytes.fromhex('83ec04'))
  def invoke(target):
   at=entry+len(body);body.extend(b'\xe8'+struct.pack('<i',target-at-5));return at
  lookup=invoke(0x2000);spill=None;clobber=None;partial_at=None
  if kind=='spill':
   spill=entry+len(body);body.extend(bytes.fromhex('890424'))
   if partial:partial_at=entry+len(body);body.extend(bytes.fromhex('c6042400'))
   clobber=invoke(0x2400);body.extend(bytes.fromhex('8b0424'))
  elif kind=='mismatch':body.extend(bytes.fromhex('8bf0'));invoke(0x2400)
  body.extend(extra);body.extend(bytes.fromhex('8b10'));body.extend(bytes.fromhex('8bce' if kind=='mismatch' else '8bc8'));target=entry+len(body);body.extend(bytes.fromhex('ff52')+bytes([slot]));body.extend(bytes.fromhex('83c404c3'))
  frame=fixtures.Discovery().frame(bytes(body),pop=0);need=set();pending=[target]
  while pending:
   for parent in frame['pred'][pending.pop()]:
    if parent not in need:need.add(parent);pending.append(parent)
  events=[] if spill is None else [dict(address='%08x'%spill,frame=-4,size=4,reason='direct')]
  if partial_at is not None:events.append(dict(address='%08x'%partial_at,frame=-4,size=1,reason='direct'))
  calls=[dict(address='%08x'%at,kind='verified') for at,i in frame['nodes'].items() if i.mnemonic=='call' and i.operands[0].type==X.X86_OP_IMM]
  m=SimpleNamespace(frame=frame,completed=need,write_events=events,call_events=calls,at=target);return m,lookup,clobber
 def test_spill_reload_across_verified_call_has_same_object_origin(self):
  m,lookup,clobber=self.machine();proof=ReceiverFlow(m).virtual(m.at);self.assertEqual(proof['lookup_call'],lookup);self.assertEqual(proof['lookup_entry'],0x2000);self.assertEqual(proof['slot'],1)
 def test_direct_return_origin_without_spill(self):
  m,lookup,clobber=self.machine('direct');self.assertEqual(ReceiverFlow(m).virtual(m.at)['lookup_call'],lookup)
 def test_partial_spill_clobber_refuses(self):
  m,lookup,clobber=self.machine(partial=True)
  with self.assertRaisesRegex(NativeRefusal,'Partial'):ReceiverFlow(m).virtual(m.at)
 def test_helper_spill_clobber_refuses(self):
  m,lookup,clobber=self.machine();m.write_events.append(dict(address='%08x'%clobber,frame=-4,size=4,reason='inferred helper'))
  with self.assertRaisesRegex(NativeRefusal,'Helper'):ReceiverFlow(m).virtual(m.at)
 def test_unverified_call_cannot_be_crossed_for_spill(self):
  m,lookup,clobber=self.machine();m.call_events=[event for event in m.call_events if int(event['address'],16)!=clobber]
  with self.assertRaisesRegex(NativeRefusal,'unverified call'):ReceiverFlow(m).virtual(m.at)
 def test_receiver_vptr_origin_mismatch_refuses(self):
  m,lookup,clobber=self.machine('mismatch')
  with self.assertRaisesRegex(NativeRefusal,'different/unknown'):ReceiverFlow(m).virtual(m.at)
 def test_partial_receiver_register_clobber_refuses(self):
  m,lookup,clobber=self.machine('direct',extra=bytes.fromhex('66b80000'))
  with self.assertRaisesRegex(NativeRefusal,'definition'):ReceiverFlow(m).virtual(m.at)
 def test_unaligned_slot_displacement_refuses(self):
  m,lookup,clobber=self.machine(slot=2)
  with self.assertRaisesRegex(NativeRefusal,'slot displacement'):ReceiverFlow(m).virtual(m.at)
 def test_unverified_native_instruction_refuses(self):
  m,lookup,clobber=self.machine();m.completed.remove(lookup)
  with self.assertRaisesRegex(NativeRefusal,'unverified prefix'):ReceiverFlow(m).virtual(m.at)
 def test_long_unknown_flow_refuses_before_python_recursion_limit(self):
  m,lookup,clobber=self.machine('direct',extra=b'\x90'*160)
  with self.assertRaisesRegex(NativeRefusal,'depth'):ReceiverFlow(m).virtual(m.at)

class PointerResult(unittest.TestCase):
 def env(self,result='Root *',symbol='?Find@Owner@@QBEPAVRoot@@XZ'):
  source=f'{result}Owner::Find() const\n{{ return 0; }}\n'.encode();row=dict(source='src/recovered/Toy.cpp',source_sha256='mock',symbol=symbol);return SimpleNamespace(provenance=lambda entry:'mock',rows={0x2000:row},_source_bytes=lambda path,digest:source)
 def test_source_and_symbol_establish_return_class(self):self.assertEqual(pointer_result(self.env(),0x2000,seed)['class_name'],'Root')
 def test_return_pointer_name_must_agree_with_symbol(self):
  with self.assertRaisesRegex(NativeRefusal,'symbol differs'):pointer_result(self.env(symbol='?Find@Owner@@QBEPAVOther@@XZ'),0x2000,seed)
 def test_nonpointer_return_refuses(self):
  with self.assertRaises(NativeRefusal):pointer_result(self.env('int ',symbol='?Find@Owner@@QBEHXZ'),0x2000,seed)
 def test_double_pointer_result_refuses(self):
  with self.assertRaisesRegex(NativeRefusal,'simple class pointer'):pointer_result(self.env('Root **',symbol='?Find@Owner@@QBEPAPAVRoot@@XZ'),0x2000,seed)

class VirtualClosure(unittest.TestCase):
 def invoke(self,second_result='bool',pop=0,opaque_reject=False):
  from contextlib import ExitStack
  from unittest.mock import patch
  import seed_fp_virtual as virtual
  self.applied=[]
  def opaque(size):
   if opaque_reject:raise NativeRefusal('Frame register supplied to opaque call')
   self.applied.append(size)
  m=SimpleNamespace(write_events=[],call_events=[],at=0x3000,frame={'direct_pops':{0x1000:0},'indirect_pops':{0x3000:pop}},opaque=opaque)
  flow=dict(lookup_call=0x1000,lookup_entry=0x2000,slot=3,trace=[]);lookup=dict(stack_argument_bytes=0,class_name='Root');family=[dict(target=0x4000),dict(target=0x5000)]
  def member(proofs,entry,seed):return dict(return_type='bool' if entry==0x4000 else second_result,parameter_types=[],stack_argument_bytes=0,symbol='?Act@Toy%x@@QAE_NXZ'%entry)
  with ExitStack() as stack:
   stack.enter_context(patch.object(virtual.ReceiverFlow,'virtual',return_value=flow));stack.enter_context(patch.object(virtual,'pointer_result',return_value=lookup));stack.enter_context(patch.object(virtual,'physical_family',return_value=family));stack.enter_context(patch.object(virtual,'member_stack_arguments',side_effect=member))
   return virtual.virtual_call(m,SimpleNamespace(audit=None),None)
 def test_agreed_target_set_applies_opaque_abi(self):self.assertEqual(self.invoke()['static_class'],'Root');self.assertEqual(self.applied,[0])
 def test_mismatched_target_results_refuse_before_application(self):
  with self.assertRaisesRegex(NativeRefusal,'set disagrees'):self.invoke(second_result='int')
  self.assertFalse(self.applied)
 def test_frame_cleanup_mismatch_refuses_before_application(self):
  with self.assertRaisesRegex(NativeRefusal,'frame cleanup'):self.invoke(pop=4)
  self.assertFalse(self.applied)
 def test_even_proven_virtual_abi_requires_no_frame_aliases(self):
  with self.assertRaisesRegex(NativeRefusal,'Frame register'):self.invoke(opaque_reject=True)
  self.assertFalse(self.applied)

if __name__=='__main__':unittest.main()
