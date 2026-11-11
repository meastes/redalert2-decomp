import unittest
import seed_candidate as seed

class FastcallPaddingTests(unittest.TestCase):
    def fit(self,params,pop):return seed.fit_convention('__fastcall',params,pop,False)
    def test_ecx_plus_two_stack_dwords_reserves_edx(self):
        self.assertEqual(self.fit(['int owner'],8),('__fastcall',['int owner','int unused_fastcall_register_1','int unused_0','int unused_1']))
    def test_ecx_plus_one_stack_dword_reserves_edx(self):self.assertEqual(len(self.fit(['float *owner'],4)[1]),3)
    def test_ecx_plus_five_stack_dwords_reserves_edx(self):self.assertEqual(len(self.fit(['int owner'],20)[1]),7)
    def test_missing_both_registers_reserves_both(self):self.assertEqual(self.fit([],4)[1],['int unused_fastcall_register_0','int unused_fastcall_register_1','int unused_0'])
    def test_pure_register_function_unchanged(self):self.assertEqual(self.fit(['int owner'],0),('__fastcall',['int owner']))
    def test_unknown_cleanup_unchanged(self):self.assertEqual(self.fit(['int owner'],None),('__fastcall',['int owner']))
    def test_existing_two_registers_unchanged(self):self.assertEqual(self.fit(['int first','int second'],8)[1],['int first','int second','int unused_0','int unused_1'])
    def test_existing_register_and_stack_args_unchanged(self):self.assertEqual(self.fit(['int first','int second','int third'],4)[1],['int first','int second','int third'])
    def test_thiscall_unchanged(self):self.assertEqual(seed.fit_convention('__thiscall',['int *owner'],8,True),('__thiscall',['int *owner','int unused_0','int unused_1']))
    def test_stdcall_unchanged(self):self.assertEqual(seed.fit_convention('__stdcall',[],8,False),('__stdcall',['int unused_0','int unused_1']))
    def test_cdecl_unchanged(self):self.assertEqual(seed.fit_convention('__cdecl',[],0,False),('__cdecl',[]))
    def test_input_parameter_list_not_mutated(self):
        params=['int owner'];self.fit(params,8);self.assertEqual(params,['int owner'])
    def test_reserved_name_collision_avoided(self):self.assertEqual(self.fit(['int unused_fastcall_register_1'],4)[1][1],'int unused_fastcall_register_1_')
    def test_small_integer_registers_supported(self):
        for kind in ['char','unsigned char','short','unsigned short','byte','bool','undefined1','undefined2']:
            with self.subTest(kind=kind):self.assertEqual(len(self.fit([kind+' first'],4)[1]),3)
    def test_opaque_pointer_register_supported(self):self.assertEqual(len(self.fit(['UnknownRecord *owner'],4)[1]),3)
    def test_known_pointer_alias_register_supported(self):self.assertEqual(len(self.fit(['HWND window'],4)[1]),3)
    def test_fp_only_header_does_not_establish_integer_slots(self):
        for kind in ['float','double','float10']:
            with self.subTest(kind=kind),self.assertRaisesRegex(ValueError,'unverified parameter'):self.fit([kind+' value'],4)
    def test_wide_integer_header_refused(self):
        for kind in ['longlong','ulonglong','undefined8']:
            with self.subTest(kind=kind),self.assertRaisesRegex(ValueError,'unverified parameter'):self.fit([kind+' value'],4)
    def test_unknown_by_value_record_refused(self):
        with self.assertRaisesRegex(ValueError,'unverified parameter'):self.fit(['UnknownRecord value'],4)
    def test_non_dword_cleanup_still_refused(self):
        with self.assertRaisesRegex(ValueError,'Non-DWORD'):self.fit(['int owner'],2)

if __name__=='__main__':unittest.main()
