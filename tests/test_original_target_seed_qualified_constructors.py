"""Qualified constructor tags need an explicit, address-evidenced member ABI."""
import unittest
import seed_candidate as seed


class Inventory:
    def __init__(self, text=None, cleanup='c21000'):
        self._seed_named_calls = {'Box': {0x401000, 0x402000}}
        self.text = text or 'void __thiscall Box::Box(Box *this,int a,int b,int c,int d)\n{ }'
        self.cleanup = cleanup

    def decompiled(self, target):
        return self.text

    def function(self, target):
        return None, {'ranges': [{'start': '%08x' % target, 'bytes_hex': self.cleanup}]}


class QualifiedConstructors(unittest.TestCase):
    CALL = 'Box::Box((Box *)owner, a, b, c, d);'

    def lower(self, body, params=(), targets=(0x401000,), inventory=None):
        return seed.qualified_constructor_calls(body, params, inventory or Inventory(), targets)

    def unchanged(self, body, **kwargs):
        self.assertEqual(self.lower(body, **kwargs), (body, {}))

    def test_explicit_member_export_uses_existing_member_pipeline(self):
        body, protos = self.lower(self.CALL)
        self.assertEqual(body, 'FUN_00401000((void *)owner, a, b, c, d);')
        self.assertEqual(protos[0x401000], ('void', '__thiscall',
                         ['Box *this', 'int a', 'int b', 'int c', 'int d']))
        body, declarations = seed.declare_callee(body, 'FUN_00401000', protos[0x401000])
        self.assertIn('((Callee_00401000*)((void *)owner))->FUN_00401000((int)(a)', body)
        self.assertEqual(declarations, ['struct Callee_00401000 { void FUN_00401000(int a, int b, int c, int d); };'])

    def test_unobserved_or_ambiguous_target_stays_refused(self):
        for targets in ((), (0x403000,), (0x401000, 0x402000)):
            self.unchanged(self.CALL, targets=targets)

    def test_member_prototype_and_native_cleanup_are_not_inferred(self):
        for text in ('void __cdecl Box::Box(Box *this,int a,int b,int c,int d)\n{}',
                     'void __thiscall Other::Other(Other *this,int a,int b,int c,int d)\n{}',
                     'void __thiscall Box::Box(void *this,int a,int b,int c,int d)\n{}',
                     'void __thiscall Box::Box(Box *this,Unknown a,int b,int c,int d)\n{}'):
            self.unchanged(self.CALL, inventory=Inventory(text))
        for cleanup in ('c20c00', 'ebfe'):
            self.unchanged(self.CALL, inventory=Inventory(cleanup=cleanup))

    def test_every_receiver_and_argument_list_must_be_explicit(self):
        for body in ('Box::Box(owner,a,b,c,d);', 'Box::Box((Box *)owner,a,b,c);',
                     'Box::Box((Box *)owner,a,b,c,d,e);', 'return '+self.CALL,
                     'x = '+self.CALL, self.CALL+' Box::Box((Box *)p,a);'):
            self.unchanged(body)

    def test_class_storage_or_other_class_uses_are_not_guessed(self):
        for prefix in ('Box local[48]; ', 'Box local; ', 'Box *p; ', 'use(sizeof(Box)); '):
            self.unchanged('{ '+prefix+self.CALL+' }')
        self.unchanged(self.CALL, params=['Box *parameter'])
        self.unchanged(self.CALL+' take(Box::Box);')

    def test_macros_qualified_members_and_namespace_prefixes_are_untouched(self):
        for body in ('#define Box other\n'+self.CALL, 'ns::'+self.CALL,
                     'p.'+self.CALL, 'p->'+self.CALL,
                     self.CALL+' ns::'+self.CALL,
                     'Box /*note*/ ::Box((Box *)owner,a,b,c,d);'):
            self.unchanged(body)
        text='/* '+self.CALL+' */ take("'+self.CALL+'");'
        self.unchanged(text)

    def test_source_bindings_and_destination_bindings_are_not_rewritten(self):
        for declaration in ('int Box;', 'code *FUN_00401000;', 'int (*FUN_00401000)(int);'):
            self.unchanged('{ '+declaration+self.CALL+' }')
        self.unchanged(self.CALL, params=['code *FUN_00401000'])

    def test_argument_expressions_and_side_effect_counts_are_preserved(self):
        body='/*before*/ Box::Box((Box *)(p++), f(), (a+b), c, d); /*after*/'
        changed, protos=self.lower(body)
        self.assertEqual(changed,'/*before*/ FUN_00401000((void *)(p++), f(), (a+b), c, d); /*after*/')
        self.assertEqual(changed.count('p++'),1)
        self.assertEqual(changed.count('f()'),1)
        self.assertEqual(set(protos),{0x401000})

    def test_repeated_calls_share_one_supplied_prototype(self):
        body='{ '+self.CALL+' '+self.CALL.replace('owner','other')+' }'
        changed, protos=self.lower(body)
        self.assertEqual(changed.count('FUN_00401000('),2)
        self.assertEqual(len(protos),1)
        self.assertNotIn('Box',changed)
