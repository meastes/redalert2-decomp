"""Recursive seeds cannot invent a second signature for their own definition."""
import unittest
import seed_candidate as seed


class RecursiveCalls(unittest.TestCase):
    NAME = 'FUN_00401000'

    def check(self, body, conv='__fastcall', params=('int first', 'int second', 'int stack')):
        return seed.validate_recursive_calls(body, self.NAME, conv, list(params))

    def test_complete_fastcall_arguments_are_valid(self):
        self.check('{ return FUN_00401000(a, b, c); }')

    def test_missing_register_arguments_refuse(self):
        with self.assertRaisesRegex(ValueError, 'Recursive.*supplies 1.*requires 3.*Register operands'):
            self.check('{ return FUN_00401000(c); }')

    def test_too_many_arguments_refuse(self):
        with self.assertRaisesRegex(ValueError, 'supplies 4'):
            self.check('{ FUN_00401000(a,b,c,d); }')

    def test_every_call_must_fit_one_self_signature(self):
        with self.assertRaisesRegex(ValueError, 'supplies 1'):
            self.check('{ FUN_00401000(a,b,c); FUN_00401000(c); }')

    def test_member_receiver_can_be_explicit_or_implicit(self):
        self.check('{ FUN_00401000(value); FUN_00401000(owner,value); }', '__thiscall',
                   ('void *this_', 'int value'))
        with self.assertRaisesRegex(ValueError, 'requires 1/2'):
            self.check('{ FUN_00401000(); }', '__thiscall', ('void *this_', 'int value'))

    def test_free_cdecl_and_stdcall_require_exact_argument_count(self):
        for conv in ('', '__cdecl', '__stdcall'):
            self.check('{FUN_00401000(a,b);}', conv, ('int a','int b'))
            with self.assertRaises(ValueError):self.check('{FUN_00401000(a);}', conv, ('int a','int b'))

    def test_cdecl_varargs_keep_their_real_argument_range(self):
        for body in ('{ FUN_00401000(format); }','{ FUN_00401000(format,a,b); }'):
            self.check(body,'__cdecl',('char *format','...'))
        with self.assertRaises(ValueError):self.check('{FUN_00401000();}','__cdecl',('char *format','...'))

    def test_comments_literals_and_address_only_uses_are_not_invocations(self):
        self.check('{ /* FUN_00401000() */ use("FUN_00401000()"); use(FUN_00401000); }')

    def test_parameter_binding_or_qualified_other_member_is_not_self_recursion(self):
        self.check('{ FUN_00401000(0); }','__cdecl',('code *FUN_00401000',))
        self.check('{ other.FUN_00401000(); other->FUN_00401000(); }')
