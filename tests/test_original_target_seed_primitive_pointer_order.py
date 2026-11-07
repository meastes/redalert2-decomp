"""Guarded i386 address comparisons between incompatible primitive views."""
import unittest
import seed_candidate as seed
class PrimitivePointerOrderingTests(unittest.TestCase):
    def change(self, expression, **extra):
        types={'p':'char *','q':'int *','same':'char *','n':'uint','s':'int','fp':'double',
               'obj':'Thing *','callback':'code *','qualified':'const char *','v':'void *'}
        types.update(extra)
        return seed.primitive_pointer_ordering('{\n  return '+expression+';\n}',globals_=types)
    def test_four_directions_preserve_complete_address_operands(self):
        for op in ('<','<=','>','>='):
            with self.subTest(op=op):self.assertIn('(uint)(p) '+op+' (uint)(q)',self.change('p '+op+' q'))
    def test_address_of_known_scalar_is_not_a_value_load(self):
        text=self.change('&n < p')
        self.assertIn('(uint)(&n) < (uint)(p)',text)
        self.assertNotIn('*(uint)',text)
    def test_reverse_address_and_multilevel_primitive_pointer(self):
        self.assertIn('(uint)(p) >= (uint)(&q)',self.change('p >= &q'))
    def test_same_types_remain_unchanged(self):
        self.assertIn('return p < same;',self.change('p < same'))
    def test_nonpointer_operands_are_never_promoted(self):
        for expr in ('p < n','p < s','p < fp','p < 0','p < unknown','&unknown < p'):
            with self.subTest(expr=expr):self.assertIn('return '+expr+';',self.change(expr))
    def test_unknown_class_callback_and_qualified_views_refuse(self):
        for expr in ('p < obj','callback < q','qualified < q','p < v'):
            with self.subTest(expr=expr):self.assertIn('return '+expr+';',self.change(expr))
    def test_compound_and_effectful_operands_refuse(self):
        for expr in ('p+1 < q','p++ < q','p < *qq','p < q[0]','call() < q','owner.p < q','owner->p < q','(n ? p : q) < q'):
            with self.subTest(expr=expr):self.assertIn('return '+expr+';',self.change(expr))
    def test_chained_comparisons_and_shifts_stay_unchanged(self):
        for expr in ('p < q < same','p << q','p == q','p != q'):
            with self.subTest(expr=expr):self.assertIn('return '+expr+';',self.change(expr))
    def test_comments_literals_and_layout_text_are_unchanged(self):
        body='{\n use("p < q"); /* p > q */\n}'
        self.assertEqual(seed.primitive_pointer_ordering(body,globals_={'p':'char *','q':'int *'}),body)
        body='{\n struct Thing { int x; };\n return p < q;\n}'
        self.assertEqual(seed.primitive_pointer_ordering(body,globals_={'p':'char *','q':'int *'}),body)
    def test_shadowed_local_or_parameter_is_unchanged(self):
        for body,params in [('{\n char *p;\n {\n int *p;\n return p < q;\n }\n}',[]),('{\n char *p;\n return p < q;\n}',['int *p'])]:
            self.assertEqual(seed.primitive_pointer_ordering(body,params,{'q':'short *'}),body)
    def test_initialized_loop_shadow_comma_and_macro_refuse(self):
        for body in ('{\n char *p;\n for (int *p=0; p < q;) {}\n}',
                     '{\n char *p, *q;\n return p < q;\n}',
                     '{\n #define p 1\n return p < q;\n}'):
            self.assertEqual(seed.primitive_pointer_ordering(body,globals_={'p':'char *','q':'int *'}),body)
    def test_initialized_local_overrides_global_type(self):
        body='{\n Thing *p=0;\n return p < q;\n}'
        self.assertEqual(seed.primitive_pointer_ordering(body,globals_={'p':'char *','q':'int *'}),body)
    def test_local_parameter_and_global_facts(self):
        body='{\n int *q;\n return p < q && q <= &n;\n}'
        text=seed.primitive_pointer_ordering(body,['char *p'],{'n':'uint'})
        self.assertIn('(uint)(p) < (uint)(q) && (uint)(q) <= (uint)(&n)',text)
    def test_idempotent(self):
        text=self.change('&n < p')
        self.assertEqual(seed.primitive_pointer_ordering(text,globals_={'n':'uint','p':'char *'}),text)
if __name__=='__main__':unittest.main()