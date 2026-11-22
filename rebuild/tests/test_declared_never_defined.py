"""Tests for rebuild/tools/declared_never_defined.py: the three forms lane O14 found misread (review session,
2026-11-21, condition of the measure fix 0f85b91f4 / b26573803; rebuild/manifest/b_log_o14.csv).

  1. an unqualified friend declaration inside a class names a function of the enclosing namespace, not a member;
  2. a friend declaration after an access label (public:/private:/protected:) is read the same way;
  3. an out-of-class template member definition with a space before the template arguments
     (PriorityQueueClass <T>::Remove_Matching) is a definition of that member.

Run: RA2_TREE=rebuild python -m unittest discover -s rebuild/tests"""
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
import declared_never_defined as D     # noqa: E402


def found(text):
    """[(kind, scope, name)] the scanner reads from TEXT."""
    return [f[:3] for f in D.analyse(text)]


def keys(text, kind):
    """{(scope, name)} of KIND entries, scope shortened as the census keys them."""
    return {(D.short(scope), name) for k, scope, name in found(text) if k == kind}


class FriendDeclarations(unittest.TestCase):
    def test_unqualified_friend_is_a_free_function(self):
        self.assertEqual(found('class DialogClass { friend int Modem_Dialog_Proc(int); };'),
                         [('decl', '', 'Modem_Dialog_Proc')])

    def test_unqualified_friend_takes_the_enclosing_namespace(self):
        self.assertEqual(found('namespace N { class A { friend int Gcd(int, int); }; }'),
                         [('decl', 'N', 'Gcd')])

    def test_friend_in_a_nested_class_skips_every_class(self):
        self.assertEqual(found('class Outer { class Inner { friend void Restore_Surface(); }; };'),
                         [('decl', '', 'Restore_Surface')])

    def test_template_friend_is_a_free_function(self):
        self.assertEqual(found('class A { template <class T> friend T Sum(T, T); };'),
                         [('decl', '', 'Sum')])

    def test_qualified_friend_keeps_its_qualifier(self):
        self.assertEqual(found('class A { friend void B::Grant(int); };'), [('decl', 'B', 'Grant')])

    def test_member_declaration_is_still_a_member(self):
        self.assertEqual(found('class A { int Update(int); };'), [('decl', 'A', 'Update')])

    def test_defined_free_function_matches_its_friend(self):
        text = 'class A { friend int Generate_Prime(int); };\nint Generate_Prime(int bits) { return bits; }\n'
        self.assertEqual(keys(text, 'decl'), keys(text, 'def'))


class FriendAfterAccessLabel(unittest.TestCase):
    def test_each_access_label(self):
        for label in ('public', 'private', 'protected'):
            with self.subTest(label=label):
                self.assertEqual(found('class HouseClass { %s: friend void Recalc_House_Factories(); };' % label),
                                 [('decl', '', 'Recalc_House_Factories')])

    def test_several_labels_and_spacing(self):
        self.assertEqual(found('class A {\n  public :\n  private:\n  friend void Main_Loop(void);\n};'),
                         [('decl', '', 'Main_Loop')])

    def test_label_then_template_friend(self):
        self.assertEqual(found('class A { public: template <class T> friend void Swap(T &, T &); };'),
                         [('decl', '', 'Swap')])

    def test_label_then_member_is_still_a_member(self):
        self.assertEqual(found('class A { public: void Dropship_Screen(void); };'),
                         [('decl', 'A', 'Dropship_Screen')])


class SpacedTemplateQualifier(unittest.TestCase):
    def test_spaced_qualifier_is_a_definition(self):
        text = 'template <class T> int PriorityQueueClass <T>::Remove_Matching(T * item) { return 0; }'
        self.assertEqual(found(text), [('def', 'PriorityQueueClass', 'Remove_Matching')])

    def test_unspaced_qualifier_reads_the_same(self):
        text = 'template <class T> int PriorityQueueClass<T>::Remove_Matching(T * item) { return 0; }'
        self.assertEqual(found(text), [('def', 'PriorityQueueClass', 'Remove_Matching')])

    def test_spaces_around_the_arguments(self):
        text = 'template <class T> void VectorClass < T >::Clear(void) {}'
        self.assertEqual(found(text), [('def', 'VectorClass', 'Clear')])

    def test_definition_matches_the_class_declaration(self):
        header = 'template <class T> class PriorityQueueClass { public: int Remove_Matching(T * item); };'
        body = 'template <class T> int PriorityQueueClass <T>::Remove_Matching(T * item) { return 0; }'
        self.assertEqual(keys(header, 'decl'), {('PriorityQueueClass', 'Remove_Matching')})
        self.assertEqual(keys(header, 'decl'), keys(body, 'def'))


if __name__ == '__main__':
    unittest.main()
