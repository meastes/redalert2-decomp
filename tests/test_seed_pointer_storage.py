from pathlib import Path
import os,sys,unittest
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools/original_target'))
if os.environ.get('RA2_SEED_TEST_TOOLS'):sys.path.append(os.environ['RA2_SEED_TEST_TOOLS'])
import seed_candidate as seed
from seed_pointer_storage import validate_pointer_storage
class PointerStorage(unittest.TestCase):
 def reject(self,text):
  with self.assertRaisesRegex(ValueError,'extent and overlap are unverified'):validate_pointer_storage(text,[],seed)
 def test_widened_double_assignment_to_undeclared_pointer_alias_refuses(self):self.reject('{\n int *piStack_f0;\n _piStack_f0 = (double)CONCAT44(high,low);\n}')
 def test_widened_double_literal_alias_refuses(self):self.reject('{\n uint *puStack_10;\n _puStack_10 = 1.0e-300;\n}')
 def test_explicit_declared_alias_keeps_own_storage(self):validate_pointer_storage('{\n int *piStack_f0;\n double _piStack_f0;\n _piStack_f0 = (double)value;\n}',[],seed)
 def test_plain_declared_pointer_is_unmodified(self):validate_pointer_storage('{\n int *piStack_f0;\n piStack_f0 = (int *)value;\n}',[],seed)
 def test_real_global_dat_name_is_not_stack_alias(self):validate_pointer_storage('{\n int *piStack_f0;\n _DAT_00123456 = 1.0;\n}',[],seed)
 def test_member_name_is_not_local_alias(self):validate_pointer_storage('{\n int *piStack_f0;\n object._piStack_f0 = (double)value;\n}',[],seed)
 def test_non_auto_identifier_is_not_guessed_alias(self):validate_pointer_storage('{\n int *name;\n _name = (double)value;\n}',[],seed)
 def test_ambiguous_base_declarations_are_not_guessed(self):validate_pointer_storage('{\n int *piStack_f0;\n { int *piStack_f0; }\n _piStack_f0 = (double)value;\n}',[],seed)
 def test_nonfloating_assignment_does_not_claim_widened_extent(self):validate_pointer_storage('{\n int *piStack_f0;\n _piStack_f0 = other;\n}',[],seed)
if __name__=='__main__':unittest.main()
