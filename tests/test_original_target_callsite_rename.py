"""Synthetic contracts for callsite_rename.py's pure steps; no compiler, catalog or target bytes required."""
from pathlib import Path
import sys
import unittest

REPOSITORY = next(parent for parent in Path(__file__).resolve().parents
                  if (parent / 'tools/original_target/callsite_rename.py').is_file())
sys.path.insert(0, str(REPOSITORY / 'tools/original_target'))
import callsite_rename as cr

HEADER = '''class Base : public Root {
public:
    virtual int unknown_slot5(int a);   // slot 5
    void helper() { other->unknown_slot5(1); }
};
class Unrelated {
    virtual int unknown_slot5(int a);
};
class Leaf : public Middle, public Mixin {
    virtual int unknown_slot5(int a);
};
'''
METHODS = [('unknown_slot5', 'Fire')]


class CallsiteRenameTests(unittest.TestCase):
    def test_family_follows_every_base_transitively(self):
        bases = cr.base_map([HEADER, 'class Middle : public Base {};', 'class Mixin {};'])
        self.assertEqual(cr.family_of('Base', bases), {'Base', 'Middle', 'Leaf'})
        self.assertEqual(bases['Leaf'], {'Middle', 'Mixin'})

    def test_bodies_rename_only_family_classes(self):
        out = cr.rename_in_bodies(HEADER, {'Base', 'Leaf'}, METHODS)
        self.assertIn('virtual int Fire(int a);   // slot 5', out)
        self.assertIn('class Unrelated {\n    virtual int unknown_slot5(int a);', out)
        self.assertIn('class Leaf : public Middle, public Mixin {\n    virtual int Fire(int a);', out)
        # inline bodies are renamed too; the compile loop puts back calls on other classes
        self.assertIn('other->Fire(1)', out)

    def test_definitions_rename_qualified_and_unqualified_calls_only(self):
        text = 'int Leaf::unknown_slot5(int a)\n{\n    unknown_slot5(a);\n    x->unknown_slot5(a);\n    Base::unknown_slot5(a);\n    b = a>unknown_slot5(1);\n}\n'
        out = cr.rename_definitions(text, {'Base', 'Leaf'}, METHODS)
        self.assertEqual(out, 'int Leaf::Fire(int a)\n{\n    Fire(a);\n    x->unknown_slot5(a);\n    Base::Fire(a);\n    b = a>Fire(1);\n}\n')

    def test_undeclared_old_name_is_a_call_site(self):
        log = "accepted.cpp(2) : error C2065: 'unknown_slot5' : undeclared identifier\n"
        fixes, _, other = cr.classify(log, [(1, 'src/a.cpp')], [({'Leaf'}, METHODS)])
        self.assertEqual((fixes, other), ({'src/a.cpp': {2: {('unknown_slot5', 'Fire')}}}, []))

    def test_group_lines_map_back_to_sources_and_headers(self):
        starts = cr.unit_starts([('src/a.cpp', b'1\n2\n3'), ('src/b.cpp', b'1\n2')])
        self.assertEqual(starts, [(1, 'src/a.cpp'), (4, 'src/b.cpp')])
        self.assertEqual(cr.locate('accepted.cpp', 5, starts), ('src/b.cpp', 2))
        self.assertEqual(cr.locate('C:\\ra2\\remote\\c-1\\src\\shared/x.hpp', 9, starts), ('src/shared/x.hpp', 9))

    def test_classify_separates_call_sites_undos_and_others(self):
        starts = [(1, 'src/a.cpp')]
        log = ("accepted.cpp(3) : error C2039: 'unknown_slot5' : is not a member of 'Leaf'\n"
               "accepted.cpp(7) : error C2039: 'Fire' : is not a member of 'Unrelated'\n"
               "accepted.cpp(9) : error C2039: 'unknown_slot5' : is not a member of 'Unrelated'\n")
        fixes, undo, other = cr.classify(log, starts, [({'Base', 'Leaf'}, METHODS)])
        self.assertEqual(fixes, {'src/a.cpp': {3: {('unknown_slot5', 'Fire')}}})
        self.assertEqual(undo, {'src/a.cpp': {7: {('Fire', 'unknown_slot5')}}})
        self.assertEqual(len(other), 1)

    def test_parts_give_one_old_name_a_new_name_per_family(self):
        parts = [({'TechnoType'}, [('unknown_slot40', 'CanUseWaypoint')]), ({'OverlayType'}, [('unknown_slot40', 'Draw_It')])]
        log = ("accepted.cpp(2) : error C2039: 'unknown_slot40' : is not a member of 'OverlayType'\n"
               "accepted.cpp(4) : error C2039: 'unknown_slot40' : is not a member of 'TechnoType'\n")
        fixes, _, _ = cr.classify(log, [(1, 'src/a.cpp')], parts)
        self.assertEqual(fixes, {'src/a.cpp': {2: {('unknown_slot40', 'Draw_It')}, 4: {('unknown_slot40', 'CanUseWaypoint')}}})
        text = 'family: TechnoType UnitType | unknown_slot40=CanUseWaypoint unknown_slot41=CanAttackMove\nfamily: OverlayType | unknown_slot40=Draw_It\nsrc/x.cpp:1 y\n'
        self.assertEqual(cr.read_parts(text), [({'TechnoType', 'UnitType'}, [('unknown_slot40', 'CanUseWaypoint'), ('unknown_slot41', 'CanAttackMove')]),
                                               ({'OverlayType'}, [('unknown_slot40', 'Draw_It')])])

    def test_ambiguous_undeclared_name_takes_the_enclosing_members_family(self):
        rows = ['void Particle::AI()', '{', '    x = unknown_slot116();', '}']
        self.assertEqual(cr.enclosing_class(rows, 3), 'Particle')
        parts = [({'Mission'}, [('unknown_slot116', 'QueueMission')]), ({'Particle'}, [('unknown_slot116', 'Shape_Number')])]
        log = "accepted.cpp(3) : error C2065: 'unknown_slot116' : undeclared identifier\n"
        fixes, _, other = cr.classify(log, [(1, 'src/a.cpp')], parts, lambda rel: rows)
        self.assertEqual((fixes, other), ({'src/a.cpp': {3: {('unknown_slot116', 'Shape_Number')}}}, []))

    def test_mangling_rules_are_family_limited(self):
        rule = cr.mangling_rules(METHODS, {'Base', 'Leaf'})[0]
        import re
        self.assertEqual(re.sub(rule['pattern'], rule['replace'], '?unknown_slot5@Leaf@@UAEHH@Z ?unknown_slot5@Unrelated@@UAEHH@Z'),
                         '?Fire@Leaf@@UAEHH@Z ?unknown_slot5@Unrelated@@UAEHH@Z')
        self.assertEqual(re.sub(rule['pattern'], rule['replace'], '?unknown_slot55@Leaf@@'), '?unknown_slot55@Leaf@@')
        self.assertEqual(re.sub(rule['pattern'], rule['replace'], '?unknown_slot5@Leaf'), '?Fire@Leaf')
        self.assertEqual(re.sub(rule['pattern'], rule['replace'], '?unknown_slot5@LeafX@@'), '?unknown_slot5@LeafX@@')

    def test_native_symbols_and_clang_column_edits(self):
        text = '__ZN4Leaf13unknown_slot5Ei ?unknown_slot5@Leaf@@ Leaf::unknown_slot5 Other::unknown_slot5'
        self.assertEqual(cr.native_symbol_text(text, METHODS, {'Leaf'}),
                         '__ZN4Leaf4FireEi ?Fire@Leaf@@ Leaf::Fire Other::unknown_slot5')
        self.assertEqual(cr.rename_at('a->unknown_slot5(b->unknown_slot5());', 4, 'unknown_slot5', 'Fire'),
                         'a->Fire(b->unknown_slot5());')
        u = cr.CLANG_UNDECLARED.search("/x/y.cpp:3:5: error: use of undeclared identifier 'unknown_slot5'; did you mean 'unknown_slot4'?")
        self.assertEqual((u.group('l'), u.group('c'), u.group('n')), ('3', '5', 'unknown_slot5'))
        m = cr.CLANG.search("/x/y.cpp:12:9: error: no member named 'unknown_slot5' in 'Leaf'")
        self.assertEqual((m.group('l'), m.group('c'), m.group('k')), ('12', '9', 'Leaf'))


if __name__ == '__main__':
    unittest.main()
