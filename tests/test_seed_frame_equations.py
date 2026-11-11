from pathlib import Path
from types import SimpleNamespace
import capstone,sys,unittest
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools/original_target'))
from seed_guarded_cfg import guarded_graph,CFGRefusal
from seed_frame_equations import AffineFrames
ENTRY=0x1000
class FrameEquations(unittest.TestCase):
 def solve(self,code,known=None):
  body=bytes.fromhex(code);decoder=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);decoder.detail=True
  audit=SimpleNamespace(decoder=decoder,original=lambda at,size:body[at-ENTRY:at-ENTRY+size]);inventory=SimpleNamespace(function=lambda entry:(None,dict(contiguous=True,ranges=[dict(start='%08x'%ENTRY,end_inclusive='%08x'%(ENTRY+len(body)-1),bytes_hex=body.hex())])))
  graph=guarded_graph(audit,inventory,ENTRY);return AffineFrames(graph,known or {})
 def test_unique_indirect_cleanup_solved(self):
  f=self.solve('6a00ff10c3');self.assertEqual(f.solved_pops,{ENTRY+2:4});self.assertEqual(f.height(ENTRY+4),0)
 def test_known_cleanup_respected(self):self.assertEqual(self.solve('6a00e800100000c3',{ENTRY+2:4}).height(ENTRY+7),0)
 def test_constant_frame_without_calls(self):self.assertEqual(self.solve('83ec0883c408c3').height(ENTRY+3),-8)
 def test_two_unknown_calls_are_not_individually_guessed(self):
  f=self.solve('6a006a00ff10ff12c3');self.assertFalse(f.solved_pops);self.assertEqual(f.height(ENTRY+8),0)
  with self.assertRaisesRegex(CFGRefusal,'underdetermined'):f.height(ENTRY+6)
 def test_relative_addresses_can_be_known_with_unknown_absolute_height(self):
  f=self.solve('6a006a00ff109090ff12c3');self.assertFalse(f.address(ENTRY+6)['proved']);self.assertTrue(f.address(ENTRY+7,4,relative_to=ENTRY+6,relative_displacement=4)['proved']);self.assertEqual(f.address(ENTRY+7,4,relative_to=ENTRY+6,relative_displacement=4)['constant'],'0')
 def test_loop_stack_invariance_solves_cleanup(self):self.assertEqual(self.solve('ff1075fcc3').solved_pops,{ENTRY:0})
 def test_branch_join_stack_invariance_solves_cleanup(self):self.assertEqual(self.solve('85c07402ff10c3').solved_pops,{ENTRY+4:0})
 def test_inconsistent_known_cleanup_refuses(self):
  with self.assertRaisesRegex(CFGRefusal,'disagree'):self.solve('6a00e800100000c3',{ENTRY+2:0})
 def test_negative_solved_cleanup_refuses(self):
  with self.assertRaisesRegex(CFGRefusal,'valid normal'):self.solve('83c404ff10c3')
 def test_non_dword_known_cleanup_refuses(self):
  with self.assertRaisesRegex(CFGRefusal,'DWORD'):self.solve('6a00e800100000c3',{ENTRY+2:2})
 def test_dynamic_esp_assignment_refuses(self):
  with self.assertRaisesRegex(CFGRefusal,'ESP effect'):self.solve('8be0c3')
 def test_noncall_cleanup_binding_refuses(self):
  with self.assertRaisesRegex(CFGRefusal,'non-call'):self.solve('90c3',{ENTRY:0})

if __name__=='__main__':unittest.main()
