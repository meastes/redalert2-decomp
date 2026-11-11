"""Function-address arithmetic is scratch syntax repair, never an ABI guess."""
from pathlib import Path
import sys
import unittest
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools/original_target'))
import seed_candidate as seed

class FunctionAddressArithmeticTests(unittest.TestCase):
    def change(self, expression, declarations='WNDPROC p;\n int n;', params=()):
        body = '{\n '+declarations+'\n return '+expression+';\n}'
        return body, seed.function_address_arithmetic(body, params)

    def test_signed_division_stays_inside_the_integer_rhs(self):
        body, changed = self.change('(WNDPROC)(p + ((int)a - (int)b) / 2)',
                                    'WNDPROC p;\n int *a;\n int *b;')
        self.assertEqual(changed, body.replace('p +', '((unsigned int)p) +'))

    def test_code_address_and_negative_offset_keep_the_result_cast(self):
        body, changed = self.change('(code *)(p + -2 + (int)q)', 'WNDPROC p;\n WNDPROC q;')
        self.assertEqual(changed, body.replace('p +', '((unsigned int)p) +'))
        body, changed = self.change('(LONG)(p - n)', 'code *p;\n int n;')
        self.assertEqual(changed, body.replace('p -', '((unsigned int)p) -'))

    def test_parameters_and_wrapping_constants(self):
        body, changed = self.change('(WNDPROC)(p + 0xffffffffU)', '', ['WNDPROC p'])
        self.assertIn('((unsigned int)p) + 0xffffffffU', changed)

    def test_calls_and_other_pointer_uses_are_unchanged(self):
        body = '{\n WNDPROC p;\n int n;\n p(hwnd, msg, w, l);\n use(p);\n if(p==q) return p;\n p = (WNDPROC)(p + n);\n}'
        self.assertEqual(seed.function_address_arithmetic(body), body.replace('p +', '((unsigned int)p) +'))

    def test_effects_and_noninteger_rhs_refuse(self):
        for rhs in ('next()', '++n', '--n', 'n++', 'n--', '(n=2)', 'n,2', '*a', 'a[0]',
                    'p', '(int)unknown', 'n ? 1 : 2', '1.5', '1ULL', '0x100000000U',
                    '0x1LL', 'n << 1', 'owner.n', '(int)f', 'n / f'):
            with self.subTest(rhs=rhs):
                body, changed = self.change('(WNDPROC)(p + '+rhs+')', 'WNDPROC p;\n int n;\n int *a;\n float f;')
                self.assertEqual(changed, body)

    def test_unknown_volatile_wide_reference_and_array_rhs_refuse(self):
        for kind in ('float', 'double', '__int64', 'unsigned __int64', 'volatile int', 'int &', 'Unknown'):
            with self.subTest(kind=kind):
                body, changed = self.change('(WNDPROC)(p + n)', 'WNDPROC p;\n '+kind+' n;')
                self.assertEqual(changed, body)
        body, changed = self.change('(WNDPROC)(p + (int)n)', 'WNDPROC p;\n int n[2];')
        self.assertEqual(changed, body)

    def test_object_pointers_and_unknown_function_aliases_refuse(self):
        for kind in ('int *', 'void *', 'Unknown *', 'FARPROC', 'volatile WNDPROC', 'WNDPROC &'):
            body, changed = self.change('(WNDPROC)(p + n)', kind+' p;\n int n;')
            self.assertEqual(changed, body)

    def test_shadowed_declarations_and_parameters_refuse(self):
        for extra in ('{ int p; }', '{ int n; }', 'int x,p;', 'int (*p)(int);', 'int(*p)(int);', 'int (__stdcall *p)(int);'):
            body, changed = self.change('(WNDPROC)(p + n)', 'WNDPROC p;\n int n;\n '+extra)
            self.assertEqual(changed, body)
        body, changed = self.change('(WNDPROC)(p + n)', 'WNDPROC p;\n int n;', ['WNDPROC p'])
        self.assertEqual(changed, body)

    def test_type_shadowing_preprocessor_and_local_types_refuse(self):
        for extra in ('int WNDPROC;', 'int LONG;', 'int code;', 'typedef int WNDPROC;',
                      'struct code {};', '#define n next()'):
            body, changed = self.change('(WNDPROC)(p + n)', 'WNDPROC p;\n int n;\n '+extra)
            self.assertEqual(changed, body)

    def test_outer_cast_and_bare_base_are_required(self):
        for expression in ('p + n', '(float)(p + n)', '(Unknown)(p + n)',
                           '(WNDPROC)(owner.p + n)', '(WNDPROC)(*pp + n)',
                           '(WNDPROC)(p++ + n)', '(WNDPROC)(p += n)', '(WNDPROC)(p * n)'):
            body, changed = self.change(expression)
            self.assertEqual(changed, body)

    def test_comments_and_literals_preserve_spelling_and_single_read(self):
        body = '{\n WNDPROC p;\n int n;\n char *s="(WNDPROC)(p + n)";\n // (WNDPROC)(p + n)\n return (WNDPROC)(p /*base*/ + /*rhs*/ n / -2);\n}'
        changed = seed.function_address_arithmetic(body)
        self.assertEqual(changed, body.replace('p /*base*/', '((unsigned int)p) /*base*/'))
        self.assertEqual(seed.function_address_arithmetic(changed), changed)

    def test_unbalanced_or_unproved_rhs_remains_unmodified(self):
        for expression in ('(WNDPROC)(p + n', '(WNDPROC)(p + (n)', '(WNDPROC)(p + (int)q)'):
            body, changed = self.change(expression)
            self.assertEqual(changed, body)

if __name__ == '__main__':
    unittest.main()
