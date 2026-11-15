"""Synthetic rename-batch contracts; no target bytes or compiler required."""
from pathlib import Path
import re
import sys
import unittest

REPOSITORY = next(parent for parent in Path(__file__).resolve().parents
                  if (parent / 'tools/original_target/apply_rename.py').is_file())
sys.path.insert(0, str(REPOSITORY / 'tools/original_target'))
import apply_rename
import random
from verify_manifest import prefix_bound_values

OLD, NEW = '?Get@Owner@@QAEHH@Z', '?Get@Owner@@QAEH_N@Z'


def region(population, symbol, references, relocations=(), source='src/recovered/a/B_00401000.cpp'):
    return dict(population=population, symbol=symbol, symbol_prefix=symbol, references=references,
                relocations=[dict(symbol=s) for s in relocations], source=source)


class ApplyRenameTests(unittest.TestCase):
    def test_rewrite_applies_rules_then_longest_names_first(self):
        spec = dict(rules=[(re.compile(r'(Get\(int \w+, )int'), r'\1bool')],
                    renames={OLD: NEW, '?Get@Owner@@': '?Fetch@Owner@@'})
        text = 'int Get(int a, int b);\n// @ref ?Get@Owner@@QAEHH@Z=00401000\n'
        self.assertEqual(apply_rename.rewrite(text, spec),
                         'int Get(int a, bool b);\n// @ref ?Get@Owner@@QAEH_N@Z=00401000\n')

    def test_leftover_ignores_old_names_inside_new_ones(self):
        renames = {'?A@@YAXXZ': '?A@@YAXXZ_extra'}
        self.assertEqual(apply_rename.leftover('x ?A@@YAXXZ_extra y', renames), [])
        self.assertEqual(apply_rename.leftover('x ?A@@YAXXZ y', renames), ['?A@@YAXXZ'])

    def test_catalog_rename_is_exact_and_marks_only_users(self):
        shared = {OLD: 1, '?Other@@YAXXZ': 2}
        untouched = {'?Unrelated@@YAXXZ': 3, OLD + 'x': 4}
        catalog = dict(regions=[region('population-1', '?F@@YAXXZ', shared, [OLD]),
                                region('population-1', OLD, shared),
                                region('population-2', '?G@@YAXXZ', untouched)])
        affected = apply_rename.rename_catalog(catalog, {OLD: NEW}, {}, {})
        self.assertEqual(affected, {'population-1'})
        first, second, third = catalog['regions']
        self.assertIs(first['references'], second['references'])
        self.assertEqual(first['references'], {NEW: 1, '?Other@@YAXXZ': 2})
        self.assertEqual((second['symbol'], second['symbol_prefix']), (NEW, NEW))
        self.assertEqual(third['references'], untouched)
        self.assertEqual(first['relocations'], [dict(symbol=NEW)])

    def test_address_scoped_rename_touches_only_the_listed_instance(self):
        mutable, const = '?Clear@?$VectorClass@PAVX@@@@UAEXXZ', '?Clear@?$VectorClass@PBVX@@@@UAEXXZ'
        scoped = {mutable: (const, {0x500})}
        rows = [dict(region('population-1', mutable, {mutable: 0x500}), entry=0x500),
                dict(region('population-2', mutable, {mutable: 0x400}), entry=0x400)]
        rows[0]['relocations'] = [dict(symbol=mutable, target=0x500)]
        rows[1]['relocations'] = [dict(symbol=mutable, target=0x400)]
        catalog = dict(regions=rows)
        self.assertEqual(apply_rename.rename_catalog(catalog, {}, {}, {}, scoped), {'population-1'})
        self.assertEqual((rows[0]['symbol'], rows[0]['references'], rows[0]['relocations'][0]['symbol']),
                         (const, {const: 0x500}, const))
        self.assertEqual((rows[1]['symbol'], rows[1]['references']), (mutable, {mutable: 0x400}))
        self.assertEqual(apply_rename.scoped_left(catalog, scoped), [])
        spec = dict(rules=[], renames={}, address_renames=scoped)
        text = '// @ref %s=00000500\n// @ref %s=00000400\n' % (mutable, mutable)
        self.assertEqual(apply_rename.rewrite(text, spec), '// @ref %s=00000500\n// @ref %s=00000400\n' % (const, mutable))

    def test_overload_renames_give_two_functions_one_name(self):
        cell = '?unknown_slot77@AircraftClass@@UAE_NHPBVCell@@0_N@Z'
        obj = '?unknown_slot78@AircraftClass@@UAE_NHPAVObjectClass@@_N@Z'
        new_cell = '?Active_Click_With@AircraftClass@@UAE_NHPBVCell@@0_N@Z'
        new_obj = '?Active_Click_With@AircraftClass@@UAE_NHPAVObjectClass@@_N@Z'
        overloads = {cell: (new_cell, 0x417560), obj: (new_obj, 0x4174b0)}
        # A plan bound both by prefix; another population by full name; a caller relocates to both.
        prefixed = {'?unknown_slot77@AircraftClass@@': 0x417560, '?unknown_slot78@AircraftClass@@': 0x4174b0,
                    '?unknown_slot7': 0x500}
        rows = [dict(region('population-1', cell, prefixed), entry=0x417560, symbol_prefix='?unknown_slot77@AircraftClass@@'),
                dict(region('population-1', obj, prefixed), entry=0x4174b0, symbol_prefix='?unknown_slot78@AircraftClass@@'),
                dict(region('population-2', '?F@@YAXXZ', {cell: 0x417560, obj: 0x4174b0}), entry=0x600)]
        rows[2]['relocations'] = [dict(symbol=cell, target=0x417560), dict(symbol=obj, target=0x4174b0)]
        catalog = dict(regions=rows)
        self.assertEqual(apply_rename.rename_catalog(catalog, {}, {}, {}, {}, overloads), {'population-1', 'population-2'})
        self.assertEqual(rows[0]['references'], {new_cell: 0x417560, new_obj: 0x4174b0, '?unknown_slot7': 0x500})
        self.assertEqual([(r['symbol'], r['symbol_prefix']) for r in rows[:2]], [(new_cell, new_cell), (new_obj, new_obj)])
        self.assertEqual(rows[2]['references'], {new_cell: 0x417560, new_obj: 0x4174b0})
        self.assertEqual([r['symbol'] for r in rows[2]['relocations']], [new_cell, new_obj])
        self.assertEqual(apply_rename.overload_left(catalog, overloads), [])
        spec = dict(rules=[], renames={}, overload_renames=overloads)
        text = ('// @ref ?unknown_slot77@AircraftClass@@=00417560\n// @ref %s=004174b0\n'
                '// @ref ?unknown_slot77@AircraftClass@@=00999999\n' % obj)
        self.assertEqual(apply_rename.rewrite(text, spec), '// @ref %s=00417560\n// @ref %s=004174b0\n'
                         '// @ref ?unknown_slot77@AircraftClass@@=00999999\n' % (new_cell, new_obj))

    def test_overload_rename_refuses_a_name_bound_elsewhere_or_a_shared_prefix(self):
        old, new = '?unknown_slot77@A@@UAE_NH@Z', '?Click@A@@UAE_NH@Z'
        overloads = {old: (new, 0x100)}
        with self.assertRaisesRegex(SystemExit, 'bound at 00000200'):
            apply_rename.rename_catalog(dict(regions=[region('population-1', '?F@@YAXXZ', {old: 0x200})]), {}, {}, {}, {}, overloads)
        # A prefix key of the new name that binds another address would cover this overload too.
        catalog = dict(regions=[region('population-1', '?F@@YAXXZ', {old: 0x100, '?Click@A@@': 0x300})])
        with self.assertRaisesRegex(SystemExit, 'would bind two addresses'):
            apply_rename.rename_catalog(catalog, {}, {}, {}, {}, overloads)

    def test_overload_spec_needs_full_names_and_distinct_addresses(self):
        import json, tempfile
        def load(overloads, renames=None):
            with tempfile.NamedTemporaryFile('w', suffix='.json', delete=False) as handle:
                json.dump(dict(name='t', renames=renames or {}, overload_renames=overloads), handle)
            return apply_rename.load_spec(handle.name, dict(regions=[]))
        good = load({'?a@X@@QAEXH@Z': dict(new='?b@X@@QAEXH@Z', at='00401000'),
                     '?c@X@@QAEXN@Z': dict(new='?b@X@@QAEXN@Z', at='00401010')})
        self.assertEqual(good['overload_renames']['?a@X@@QAEXH@Z'], ('?b@X@@QAEXH@Z', 0x401000))
        for bad in ({'?a@X@@QAEXH@Z': dict(new='?b@X@@QAEXH@Z', at=['00401000'])},
                    {'?a@X@@QAEXH@Z': dict(new='?b@X@@QAEXH@Z', at='00401000'),
                     '?c@X@@QAEXN@Z': dict(new='?b@X@@QAEXH@Z', at='00401010')},
                    {'?a@X@@QAEXH@Z': dict(new='?b@X@@QAEXH@Z', at='00401000'),
                     '?c@X@@QAEXN@Z': dict(new='?b@X@@QAEXN@Z', at='00401000')}):
            with self.assertRaises(SystemExit):
                load(bad)
        with self.assertRaises(SystemExit):
            load({'?a@X@@QAEXH@Z': dict(new='?b@X@@QAEXH@Z', at='00401000')}, {'?a@X@@QAEXH@Z': '?z@X@@QAEXH@Z'})

    def test_rename_onto_an_existing_binding_at_another_address_refuses(self):
        catalog = dict(regions=[region('population-1', '?F@@YAXXZ', {OLD: 1, NEW: 2})])
        with self.assertRaises(SystemExit):
            apply_rename.rename_catalog(catalog, {OLD: NEW}, {}, {})

    def test_changed_and_moved_sources_mark_their_population(self):
        old, new = 'src/recovered/a/Old_00401000.cpp', 'src/recovered/a/New_00401000.cpp'
        row = dict(region('population-3', '?F@@YAXXZ', {}, source=old), semantic_name='Old_00401000')
        catalog = dict(regions=[row])
        self.assertEqual(apply_rename.rename_catalog(catalog, {}, {new: b''}, {old: new}), {'population-3'})
        self.assertEqual((row['source'], row['semantic_name']), (new, 'New_00401000'))

    def test_header_only_batches_always_verify_their_header_users(self):
        headers = ['src/shared/a.hpp']
        self.assertTrue(apply_rename.use_header_users(False, headers, set()))
        self.assertFalse(apply_rename.use_header_users(False, headers, {'population-1'}))
        self.assertTrue(apply_rename.use_header_users(True, headers, {'population-1'}))
        self.assertFalse(apply_rename.use_header_users(False, [], set()))

    def test_prefix_lookup_equals_the_full_table_scan(self):
        generator = random.Random(7)
        alphabet = '?@AB_'
        for _ in range(2000):
            table = {''.join(generator.choice(alphabet) for _ in range(generator.randint(0, 6))): generator.randint(0, 3)
                     for _ in range(generator.randint(0, 12))}
            name = ''.join(generator.choice(alphabet) for _ in range(generator.randint(0, 8)))
            scan = {v for n, v in table.items() if name == n or (n.startswith('?') and name.startswith(n))}
            self.assertEqual(prefix_bound_values(table, name), scan)


class ApplyModulesTests(unittest.TestCase):
    def test_module_gathers_regions_of_one_population(self):
        import tempfile
        with tempfile.TemporaryDirectory() as scratch:
            draft = Path(scratch) / 'version.cpp'; draft.write_bytes(b'// module\n')
            rows = [dict(region('population-7', '?A@@YAXXZ', {}, source='src/recovered/a/A_00401000.cpp'),
                         entry=0x401000, semantic_name='A_00401000', options=['/O2', '/Ob2', '/G5', '/Gy']),
                    dict(region('population-7', '?B@@YAXXZ', {}, source='src/recovered/a/B_00401010.cpp'),
                         entry=0x401010, options=['/O2', '/Ob2', '/G5', '/Gy'])]
            catalog = dict(regions=rows)
            spec = dict(modules={'src/recovered/version.cpp': dict(draft=str(draft), regions=['00401000', '00401010'])},
                        options={'population-7': ['/O2', '/Ob2', '/G5', '/Gy', '/GR']})
            files = {'src/recovered/a/A_00401000.cpp': b'rewritten'}; removed = []
            affected = apply_rename.apply_modules(catalog, spec, files, removed)
        self.assertEqual(affected, {'population-7'})
        self.assertEqual([r['source'] for r in rows], ['src/recovered/version.cpp'] * 2)
        self.assertNotIn('semantic_name', rows[0])
        self.assertEqual(sorted(removed), ['src/recovered/a/A_00401000.cpp', 'src/recovered/a/B_00401010.cpp'])
        self.assertEqual(files, {'src/recovered/version.cpp': b'// module\n'})   # removed files are not rewritten
        self.assertEqual(rows[1]['options'], ['/O2', '/Ob2', '/G5', '/Gy', '/GR'])

    def test_module_refuses_regions_of_two_populations_and_unknown_options(self):
        rows = [dict(region('population-7', '?A@@YAXXZ', {}), entry=0x401000),
                dict(region('population-8', '?B@@YAXXZ', {}), entry=0x401010)]
        spec = dict(modules={'src/recovered/m.cpp': dict(draft='x', regions=['00401000', '00401010'])})
        with self.assertRaisesRegex(SystemExit, 'several populations'):
            apply_rename.apply_modules(dict(regions=rows), spec, {}, [])
        with self.assertRaisesRegex(SystemExit, 'not a recorded option set'):
            apply_rename.apply_modules(dict(regions=rows), dict(options={'population-7': ['/Od']}), {}, [])


if __name__ == '__main__':
    unittest.main()
