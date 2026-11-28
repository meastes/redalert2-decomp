"""permute.py (lane O108): the text mutations it offers, without the compiler."""
import pathlib
import sys
import unittest

TOOLS = pathlib.Path(__file__).resolve().parents[1] / 'tools' / 'original_target'
sys.path.insert(0, str(TOOLS))
import permute as P  # noqa: E402

FUNC = '''int f(int a, int b, Thing * p)
{
\tint x = a + b;
\tint y = 0;
\tif (a < b && p->Value != 0) {
\t\ty = x;
\t}
\tif (!p) {
\t\ty = a ? b : 2;
\t}
\tif (a) {
\t\tif (b) {
\t\t\ty += 1;
\t\t}
\t}
\tif (b == 0) {
\t\ty = 1;
\t} else {
\t\ty = 2;
\t}
\treturn y + Get(p);
}
'''


def variants(kinds):
    return [(k, d, t) for k, d, t in P.mutations(FUNC, 'f', set(kinds), [], True)]


class ExpressionTests(unittest.TestCase):
    def test_commutative_skips_calls(self):
        texts = [t for k, d, t in variants(['comm'])]
        self.assertIn('\tint x = b + a;', '\n'.join(texts))
        self.assertTrue(any('p->Value != 0 && a < b' not in t and 'p->Value' in t for t in texts))
        self.assertFalse(any('Get(p) + y' in t for t in texts))     # a call operand never moves

    def test_declaration_type_is_not_an_operand(self):
        text = 'void g(int a)\n{\n\tThing * p = Make(a);\n\tUse(p);\n}\n'
        self.assertFalse(any('p * Thing' in t for k, d, t in P.mutations(text, 'g', {'comm'}, [], True)))

    def test_and_with_deref_not_swapped(self):
        self.assertFalse(any('p->Value != 0 && a < b' in t for k, d, t in variants(['comm'])))

    def test_relational_flip(self):
        self.assertTrue(any('b > a && p->Value' in t for k, d, t in variants(['rel'])))

    def test_not_spellings(self):
        texts = '\n'.join(t for k, d, t in variants(['not']))
        self.assertIn('if (p == 0) {', texts)
        self.assertIn('if (!b) {', texts)
        self.assertIn('p->Value) {', texts)
        self.assertIn('if (a != 0) {', texts)

    def test_reassociation_is_parenthesised(self):
        out = P.expression_mutations('a + b + c', 0, 9, False)
        self.assertIn('c + (a + b)', [t for k, d, t in out])


class StatementTests(unittest.TestCase):
    def test_andif_both_ways(self):
        texts = [t for k, d, t in variants(['andif'])]
        self.assertTrue(any('if (a < b) {\n\t\tif (p->Value != 0) {\n\t\t\ty = x;' in t for t in texts))
        self.assertTrue(any('if (a && b) {\n\t\ty += 1;\n\t}\n\tif (b == 0)' in t for t in texts))

    def test_ternary_both_ways(self):
        texts = [t for k, d, t in variants(['tern'])]
        self.assertTrue(any('if (a) {\n\t\t\ty = b;\n\t\t} else {\n\t\t\ty = 2;\n\t\t}' in t for t in texts))
        self.assertTrue(any('\ty = (b == 0) ? 1 : 2;' in t for t in texts))

    def test_inline_once_used_local(self):
        texts = [t for k, d, t in variants(['inline'])]
        self.assertTrue(any('\t\ty = a + b;' in t or '\t\ty = (a + b);' in t for t in texts))

    def test_no_named_condition_under_a_case_label(self):
        text = 'int h(int a)\n{\n\tswitch (a) {\n\tcase 1:\n\t\tif (a > 2) {\n\t\t\treturn 1;\n\t\t}\n\t\tbreak;\n\t}\n\treturn 0;\n}\n'
        self.assertEqual([], P.mutations(text, 'h', {'name'}, [], True))

    def test_uninitialised_declaration_untouched(self):
        text = 'void g(int a)\n{\n\tThing * p;\n\tp = Make(a);\n}\n'
        self.assertFalse(any('p * Thing' in t for k, d, t in P.mutations(text, 'g', {'comm'}, [], True)))

    def test_named_condition(self):
        texts = [t for k, d, t in variants(['name'])]
        self.assertTrue(any('bool test_' in t and 'if (test_' in t for t in texts))


if __name__ == '__main__':
    unittest.main()
