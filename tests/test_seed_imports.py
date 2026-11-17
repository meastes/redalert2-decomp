from pathlib import Path
import importlib.util,sys,unittest,os
from unittest.mock import patch
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools/original_target'))
for name in ('RA2_SEED_ALIAS_TOOLS','RA2_SEED_TEST_TOOLS'):
 if os.environ.get(name):sys.path.append(os.environ[name])
class PureImports(unittest.TestCase):
 def check(self,name):
  path=Path(__file__).resolve().parents[1]/'tools/original_target'/(name+'.py')
  spec=importlib.util.spec_from_file_location('fresh_'+name,path);module=importlib.util.module_from_spec(spec)
  with patch.object(Path,'read_bytes',side_effect=AssertionError('Import read bytes')),patch.object(Path,'read_text',side_effect=AssertionError('Import read text')):
   spec.loader.exec_module(module)
 def test_frames(self):self.check('seed_frame_equations')
 def test_source(self):self.check('seed_integer_scope')
 def test_native(self):self.check('seed_integer_native')
 def test_runtime(self):self.check('seed_library_abi')
 def test_integer_candidate(self):self.check('seed_integer_candidate')
 def test_global_leaf(self):self.check('seed_global_leaf_abi')
 def test_receivers(self):self.check('seed_capture_receivers')
 def test_rectangle_scan(self):self.check('seed_rect_scan')
 def test_bullet_ai(self):self.check('seed_bullet_ai')
if __name__=='__main__':unittest.main()
