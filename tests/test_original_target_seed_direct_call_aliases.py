"""Ghidra direct-call aliases use the existing callee ABI pipeline."""
import unittest
import seed_candidate as seed


class DirectCallAliases(unittest.TestCase):
    def normalize(self, body, params=(), targets=(0x612cc0,)):
        return seed.direct_call_aliases(body, params, targets)

    def test_observed_call_becomes_existing_address_named_callee(self):
        self.assertEqual(self.normalize('return func_0x00612cc0(owner);'),
                         'return FUN_00612cc0(owner);')

    def test_missing_direct_target_is_not_guessed_from_name(self):
        text = 'return func_0x00612cc0(owner);'
        self.assertEqual(self.normalize(text, targets=()), text)

    def test_multiple_calls_and_mixed_case_addresses(self):
        text = 'func_0x00612Cc0(a); func_0x00612Cc0(b);'
        self.assertEqual(self.normalize(text), 'FUN_00612cc0(a); FUN_00612cc0(b);')

    def test_comments_literals_and_spacing_are_retained(self):
        text = '/* func_0x00612cc0(x) */ func_0x00612cc0 /*note*/ (a); "func_0x00612cc0(x)";'
        self.assertEqual(self.normalize(text),
                         '/* func_0x00612cc0(x) */ FUN_00612cc0 /*note*/ (a); "func_0x00612cc0(x)";')

    def test_local_parameter_or_shadowing_binding_is_not_rewritten(self):
        body = 'return func_0x00612cc0(x);'
        self.assertEqual(self.normalize(body, ['code *func_0x00612cc0']), body)
        for declaration in ('int func_0x00612cc0;', 'code *func_0x00612cc0;',
                            'int (*func_0x00612cc0)(int);'):
            text = '{ ' + declaration + body + ' }'
            self.assertEqual(self.normalize(text), text)

    def test_qualified_or_mixed_address_use_is_not_rewritten(self):
        for text in ('ns::func_0x00612cc0(x);', 'p->func_0x00612cc0(x);',
                     'p.func_0x00612cc0(x);',
                     'func_0x00612cc0(x); take(func_0x00612cc0);',
                     '((code *)func_0x00612cc0)(x);'):
            with self.subTest(text=text):
                self.assertEqual(self.normalize(text), text)

    def test_macros_and_existing_destination_local_are_not_rewritten(self):
        for text in ('#define func_0x00612cc0(x) other(x)\nfunc_0x00612cc0(x);',
                     '{ code *FUN_00612cc0; func_0x00612cc0(x); }'):
            self.assertEqual(self.normalize(text), text)

    def test_existing_member_pipeline_retains_receiver_and_stack_parameter(self):
        text = self.normalize('func_0x00612cc0(owner, value);')
        body, declarations = seed.declare_callee(text, 'FUN_00612cc0',
                                                ('void', '__thiscall', ['void *this_', 'int value']))
        self.assertIn('((Callee_00612cc0*)(owner))->FUN_00612cc0((int)(value))', body)
        self.assertIn('struct Callee_00612cc0 { void FUN_00612cc0(int value); };', '\n'.join(declarations))

    def test_existing_FUN_spelling_and_longer_identifiers_are_unchanged(self):
        text = 'FUN_00612cc0(x); my_func_0x00612cc0(x); func_0x00612cc0_tail(x);'
        self.assertEqual(self.normalize(text), text)


if __name__ == '__main__':
    unittest.main()
