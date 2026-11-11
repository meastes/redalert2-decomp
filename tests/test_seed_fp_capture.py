"""Authored toy native discovery cases; no game instruction arrays."""
from pathlib import Path
import sys,unittest
from types import SimpleNamespace
from unittest.mock import patch
import capstone
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools/original_target'))
import seed_fp_capture as capture
from seed_fp_native import graph,NativeRefusal
ENTRY=0x1000
PREFIX=bytes.fromhex('8bf183ec088b068904248b560489542404')
FP=bytes.fromhex('dd042483ec04d91c24')
CALL=bytes.fromhex('e800100000')
TAIL=bytes.fromhex('83c408c3')
class Discovery(unittest.TestCase):
 def env(self,body):
  decoder=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);decoder.detail=True
  audit=SimpleNamespace(decoder=decoder,original=lambda at,size:body[at-ENTRY:at-ENTRY+size])
  inventory=SimpleNamespace(function=lambda entry:(None,dict(contiguous=True,ranges=[dict(start='%08x'%ENTRY,end_inclusive='%08x'%(ENTRY+len(body)-1),bytes_hex=body.hex())])))
  return SimpleNamespace(audit=audit,inventory=inventory)
 def frame(self,body=None,pop=4):
  with patch.object(capture,'cleanup',return_value=pop):return capture.caller_frame(self.env(body or PREFIX+FP+CALL+TAIL),ENTRY)
 def witness(self,body=None):
  f=self.frame(body);c=capture.initial_word_capture(f,ENTRY);at=next(at for at,i in f['nodes'].items() if i.mnemonic=='call');return capture.argument_witness(f,at,c['frame'])
 def reject(self,body):
  with self.assertRaises(NativeRefusal):self.witness(body)
 def test_capture_and_argument(self):
  f=self.frame();c=capture.initial_word_capture(f,ENTRY);self.assertEqual(c['frame'],-8);self.assertEqual(c['high']['frame'],-4);self.assertLess(self.witness()['load'],self.witness()['store'])
 def test_outgoing_argument_overwrite_refuses(self):self.reject(PREFIX+FP+bytes.fromhex('c7042400000000')+CALL+TAIL)
 def test_disjoint_outgoing_setup_store_passes(self):self.witness(PREFIX+FP+bytes.fromhex('c744240400000000')+CALL+TAIL)
 def test_partial_outgoing_argument_overlap_refuses(self):self.reject(PREFIX+FP+bytes.fromhex('66c74424020000')+CALL+TAIL)
 def test_integer_fp_load_refuses(self):self.reject(PREFIX+FP.replace(bytes.fromhex('dd0424'),bytes.fromhex('df2c24'))+CALL+TAIL)
 def test_shifted_double_read_refuses(self):self.reject(PREFIX+bytes.fromhex('dd44240483ec04d91c24')+CALL+TAIL)
 def test_wide_argument_store_refuses(self):self.reject(PREFIX+FP.replace(bytes.fromhex('d91c24'),bytes.fromhex('dd1c24'))+CALL+TAIL)
 def test_nonargument_store_refuses(self):self.reject(PREFIX+FP.replace(bytes.fromhex('d91c24'),bytes.fromhex('d95c2404'))+CALL+TAIL)
 def test_reversed_source_words_refuse(self):self.reject(PREFIX.replace(bytes.fromhex('8b06'),bytes.fromhex('8b4604')).replace(bytes.fromhex('8b5604'),bytes.fromhex('8b16'))+FP+CALL+TAIL)
 def test_nonadjacent_capture_refuses(self):self.reject(PREFIX.replace(bytes.fromhex('89542404'),bytes.fromhex('89542408'))+FP+CALL+TAIL)
 def test_duplicate_capture_refuses(self):self.reject(PREFIX+bytes.fromhex('890424')+FP+CALL+TAIL)
 def test_partial_owner_register_clobber_refuses(self):self.reject(PREFIX[:5]+bytes.fromhex('66be0000')+PREFIX[5:]+FP+CALL+TAIL)
 def test_partial_owner_bx_clobber_refuses(self):self.reject(PREFIX.replace(bytes.fromhex('8bf1'),bytes.fromhex('8bd9')).replace(bytes.fromhex('8b06'),bytes.fromhex('66bb00008b03')).replace(bytes.fromhex('8b5604'),bytes.fromhex('8b5304'))+FP+CALL+TAIL)
 def test_dynamic_stack_effect_refuses(self):self.reject(PREFIX+bytes.fromhex('2be0')+FP+CALL+TAIL)
 def test_unbalanced_frame_refuses(self):self.reject(PREFIX+FP+CALL+bytes.fromhex('83c404c3'))
 def test_root_stack_arguments_refuse(self):self.reject(PREFIX+FP+CALL+bytes.fromhex('83c408c20400'))
 def test_two_indirect_cleanups_refuse(self):self.reject(PREFIX+FP+bytes.fromhex('ffd0ffd0')+TAIL)
 def test_extra_fp_operation_refuses(self):self.reject(PREFIX+FP[:3]+bytes.fromhex('d9e0')+FP[3:]+CALL+TAIL)
 def test_conditional_argument_ingress_refuses(self):self.reject(PREFIX+FP[:3]+bytes.fromhex('7500')+FP[3:]+CALL+TAIL)
if __name__=='__main__':unittest.main()
