"""Mixed named SDK halves and raw DWORD slices keep their separate meanings."""
from pathlib import Path
import sys,unittest
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools/original_target'))
import seed_candidate as seed
class LargeIntegerSlices(unittest.TestCase):
    def lower(self,operations,decl='LARGE_INTEGER value;',params=()):
        text='{\n '+decl+'\n '+operations+'\n}'
        return text,seed.large_integer_member_artifacts(text,params)
    def test_raw_low_read_does_not_refuse_named_halves(self):
        _,text=self.lower('use(value._0_4_); return value.s.HighPart;')
        self.assertIn('value._0_4_',text);self.assertIn('return value.HighPart;',text)
    def test_raw_high_keeps_unsigned_slice_and_signed_sdk_member(self):
        _,text=self.lower('use(value._4_4_); return value.s.HighPart < 0;')
        self.assertIn('value._4_4_',text);self.assertIn('value.HighPart < 0',text)
    def test_both_slice_writes_keep_order_and_ordinary_members(self):
        _,text=self.lower('value._0_4_=a; value.s.HighPart=b; value._4_4_=c; use(value.s.LowPart);')
        self.assertIn('value._0_4_=a; value.HighPart=b; value._4_4_=c; use(value.LowPart);',text)
    def test_unknown_widths_and_offsets_refuse_entire_local(self):
        for part in ['_0_1_','_0_2_','_0_3_','_0_6_','_0_8_','_1_4_','_2_4_','_3_4_','_5_4_','_8_4_','_04_4_']:
            with self.subTest(part=part):
                original,text=self.lower('use(value.'+part+'); use(value.s.LowPart);');self.assertEqual(original,text)
    def test_parameter_and_shadowed_local_remain_refused(self):
        for params,extra in [(('uint value',),''),((),'{ int value; }')]:
            original,text=self.lower(extra+' use(value._0_4_); use(value.s.LowPart);',params=params);self.assertEqual(original,text)
    def test_non_sdk_arrays_pointers_and_qualified_records_refuse(self):
        for decl in ['int value;','LARGE_INTEGER *value;','LARGE_INTEGER value[2];','volatile LARGE_INTEGER value;','const LARGE_INTEGER value;','Other::LARGE_INTEGER value;']:
            original,text=self.lower('use(value._0_4_); use(value.s.LowPart);',decl);self.assertEqual(original,text)
    def test_comment_or_space_inside_slice_still_refuses(self):
        for expression in ['value /*keep*/._0_4_','value ._0_4_','value. /*keep*/_0_4_']:
            original,text=self.lower('use('+expression+'); use(value.s.LowPart);');self.assertEqual(original,text)
    def test_qualified_owner_and_literals_are_untouched(self):
        _,text=self.lower('use(obj.value._0_4_); use("value._4_4_"); /* value.s.HighPart */ return value.s.LowPart;')
        self.assertIn('obj.value._0_4_',text);self.assertIn('"value._4_4_"',text);self.assertIn('/* value.s.HighPart */',text);self.assertIn('return value.LowPart;',text)
    def test_unknown_use_of_same_local_still_refuses(self):
        original,text=self.lower('use(value._0_4_); use(value); use(value.s.LowPart);');self.assertEqual(original,text)
    def test_only_independent_eligible_local_changes(self):
        _,text=self.lower('use(value._0_4_); use(value.s.LowPart); use(other._8_4_); use(other.s.HighPart);','LARGE_INTEGER value;\n LARGE_INTEGER other;')
        self.assertIn('use(value.LowPart)',text);self.assertIn('other.s.HighPart',text)
    def test_full_seed_order_handles_mixed_views_before_scalar_lowering(self):
        class Inventory:
            passes=[]
            def decompiled(self,entry):return 'uint __cdecl FUN_00401000(uint lo,int hi)\n{\n LARGE_INTEGER value;\n value._0_4_=lo;\n value.s.HighPart=hi;\n return value.s.LowPart ^ value._4_4_;\n}'
            def function(self,entry):return None,{'ranges':[]}
        text=seed.seed(0x401000,Inventory(),{});self.assertNotIn('value.s.',text);self.assertNotIn('value._',text);self.assertIn('DWORD LowPart; LONG HighPart;',text);self.assertIn('value.HighPart',text)
    def test_valid_slice_prefix_does_not_accept_unknown_selector(self):
        for selector in ['_0_4_extra','_4_4_extra','_0_4__']:
            original,text=self.lower('use(value.'+selector+'); use(value.s.LowPart);');self.assertEqual(original,text)
if __name__=='__main__':unittest.main()
