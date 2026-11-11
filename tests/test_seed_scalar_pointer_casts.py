"""Explicit source casts only: authored expressions, no original body acceptance."""
from types import SimpleNamespace
import unittest
import seed_candidate as seed
from seed_scalar_fp import expression_type
from seed_scalar_pairs import source_pairs
from seed_callee_abi import AbiRefusal

class PointerScalarCasts(unittest.TestCase):
    def typed(self,text,kind='int *'):
        schema=SimpleNamespace(records={'KnownRecord':object()})
        api=SimpleNamespace(matching_paren=seed.matching_paren,unary_operator=seed.unary_operator,seed_expression_type=seed.seed_expression_type,typed_schema=lambda:schema)
        return expression_type(text,{'p':kind,'q':'int *','d':'double','i':'int'},api)
    def test_existing_explicit_signed_word_cast(self):self.assertEqual(self.typed('(int)p'),'int')
    def test_supported_explicit_word_casts(self):
        for kind in ('int','unsigned int','long','unsigned long','uint','undefined4'):
            with self.subTest(kind=kind):self.assertEqual(self.typed('('+kind+')p'),kind)
    def test_nested_explicit_integer_then_double(self):self.assertEqual(self.typed('(double)(int)p'),'double')
    def test_nested_explicit_integer_then_float(self):self.assertEqual(self.typed('(float)(uint)p'),'float')
    def test_pointer_word_cast_in_real_floating_arithmetic(self):self.assertEqual(self.typed('d*d+(double)(int)p*(double)(int)p'),'double')
    def test_parenthesized_pointer_read(self):self.assertEqual(self.typed('(double)(int)(((p)))'),'double')
    def test_whitespace_preserves_recognition(self):self.assertEqual(self.typed('( double ) ( int ) ( p )'),'double')
    def test_known_ordinary_object_pointer_types(self):
        for kind in ('void *','double *','float *','int **','undefined8 *','longlong *','KnownRecord *','struct KnownRecord *','class KnownRecord *'):
            with self.subTest(kind=kind):self.assertEqual(self.typed('(double)(int)p',kind),'double')
    def test_bare_pointer_is_not_floating_input(self):self.assertIsNone(self.typed('p'))
    def test_direct_pointer_to_double_remains_refused(self):self.assertIsNone(self.typed('(double)p'))
    def test_direct_pointer_to_float_remains_refused(self):self.assertIsNone(self.typed('(float)p'))
    def test_narrow_and_wide_casts_remain_refused(self):
        for kind in ('char','signed char','unsigned char','short','unsigned short','bool','byte','ushort','longlong','ulonglong','__int64','unsigned __int64','double','float'):
            with self.subTest(kind=kind):self.assertIsNone(self.typed('('+kind+')p'))
    def test_nondefault_pointer_representations_refuse(self):
        for kind in ('int __ptr64 *','int * __ptr64','int __ptr32 *','int __sptr *','int __uptr *','int far *','int near *','int __far *','int __near *','int __based(x) *'):
            with self.subTest(kind=kind):self.assertIsNone(self.typed('(double)(int)p',kind))
    def test_qualified_pointer_declarations_refuse(self):
        for kind in ('const int *','volatile int *','int *const','int *volatile','int *restrict','int *__restrict'):
            with self.subTest(kind=kind):self.assertIsNone(self.typed('(double)(int)p',kind))
    def test_function_or_code_pointer_declarations_refuse(self):
        for kind in ('int (*)(int)','void (__cdecl *)(void)','code *','code **','FunctionAlias *','int __cdecl *'):
            with self.subTest(kind=kind):self.assertIsNone(self.typed('(double)(int)p',kind))
    def test_unknown_typedef_or_pointee_refuses(self):
        for kind in ('Mystery *','LPCSTR','UnknownAlias','Thing **','struct Mystery *'):
            with self.subTest(kind=kind):self.assertIsNone(self.typed('(double)(int)p',kind))
    def test_array_and_reference_declarations_refuse(self):
        for kind in ('int[3]','int *[3]','int &','int *&'):
            with self.subTest(kind=kind):self.assertIsNone(self.typed('(double)(int)p',kind))
    def test_untyped_operand_refuses(self):self.assertIsNone(self.typed('(double)(int)missing'))
    def test_call_operand_refuses(self):self.assertIsNone(self.typed('(double)(int)callee()'))
    def test_mutating_pointer_operands_refuse(self):
        for expr in ('p++','++p','p--','--p','(p=q)','(i,p)'):
            with self.subTest(expr=expr):self.assertIsNone(self.typed('(double)(int)'+expr))
    def test_compound_pointer_operations_remain_unsupported(self):
        for expr in ('(p+1)','p[0]','*p','&i'):
            # Ordinary dereference/index integer loads were already supported.
            if expr in ('*p','p[0]'):self.assertEqual(self.typed('(double)(int)'+expr),'double')
            else:
                with self.subTest(expr=expr):self.assertIsNone(self.typed('(double)(int)'+expr))
    def test_comments_are_not_discarded(self):self.assertIsNone(self.typed('(double)(int)/* keep */p'))
    def test_existing_integer_and_fp_inputs_unchanged(self):self.assertEqual(self.typed('(double)i+d'),'double')
    def test_source_pair_retains_original_cast_expression(self):
        expr='x*x+(double)(int)p*(double)(int)p';body='{\n double x;\n int *p;\n int result;\n FUN_00001000('+expr+');\n result = FUN_00002000();\n}'
        rows=source_pairs(body,[],0x1000,0x2000,{},seed);self.assertEqual(len(rows),1);self.assertEqual(rows[0][2],'result = (longlong)FUN_00001000('+expr+');')
    def test_source_pair_rejects_bare_pointer_even_with_other_fp_term(self):
        body='{\n double x;\n int *p;\n int result;\n FUN_00001000(x*x+(double)p);\n result = FUN_00002000();\n}'
        with self.assertRaises(AbiRefusal):source_pairs(body,[],0x1000,0x2000,{},seed)
    def test_source_pair_rejects_missing_integer_conversion(self):
        body='{\n int *p;\n FUN_00001000((double)(int)p);\n}'
        with self.assertRaises(AbiRefusal):source_pairs(body,[],0x1000,0x2000,{},seed)

if __name__=='__main__':unittest.main()
