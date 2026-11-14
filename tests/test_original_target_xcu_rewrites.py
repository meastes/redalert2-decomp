"""Owner rulings 2026-11-14 for the $E rows: a row rewritten into the real dynamic-initializer
form binds its fresh `_$E<n>` by its .CRT$XCU slot (apply_rename `xcu_rewrites`), and a binding
conflict on a name no row of the unit relocates to is a stale table entry. Synthetic fixtures."""
from pathlib import Path
import sys
import unittest

REPOSITORY = next(parent for parent in Path(__file__).resolve().parents
                  if (parent / 'tools/original_target/apply_rename.py').is_file())
sys.path.insert(0, str(REPOSITORY / 'tools/original_target'))
import apply_rename
import initializer_rebind
import refile_unit

INIT_A, INIT_B, DTOR_B, OTHER_UNIT, NEXT_UNIT = 0x401000, 0x401040, 0x401060, 0x400000, 0x402000


class FakeCoff:
    """One section per function plus a .CRT$XCU section of DIR32 entries."""
    def __init__(self, functions, xcu):
        self.sections, self.symbols, names = [], {}, {}

        def index(name):
            if name not in names:
                names[name] = len(self.symbols)
                self.symbols[names[name]] = dict(name=name, value=0, section=0, type=0, storage=2, size=0)
            return names[name]
        for name, data, relocs in functions:
            self.sections.append(dict(name='.text', data=bytes(data), size=len(data), flags=0x60101020,
                                      relocs=[(o, index(t), 20) for o, t in relocs]))
            self.symbols[index(name)].update(section=len(self.sections), type=0x20, storage=3, size=len(data))
        self.sections.append(dict(name='.CRT$XCU', data=bytes(4 * len(xcu)), size=4 * len(xcu), flags=0x40300040,
                                  relocs=[(4 * i, index(n), 6) for i, n in enumerate(xcu)]))

    def function(self, name):
        found = [s for s in self.symbols.values() if s['name'] == name and s['section']]
        if len(found) != 1:
            raise ValueError('Function symbol ambiguous/missing: ' + name)
        section = self.sections[found[0]['section'] - 1]
        return found[0], section, 0, len(section['data'])


def row(entry, name, population='population-1', relocations=()):
    return dict(entry=entry, symbol=name, symbol_prefix=name, population=population, body_bytes=1,
                relocations=[dict(symbol=s, target=t, offset=0, kind=6) for s, t in relocations])


def catalog():
    """population-1: the unit (initializer A, already `_$E2`; B and its destructor rewritten
    from seed functions); population-2 holds a stale copy of B's old name beside its own
    `_$E9`; population-3 has a row that relocates to the old destructor name."""
    own = {'_$E2': INIT_A, '?seed_b@@YAXXZ': INIT_B, '?seed_dtor_b@@YAXXZ': DTOR_B}
    stale = {'?seed_b@@YAXXZ': INIT_B, '_$E9': 0x4D0000}
    user = {'?seed_dtor_b@@YAXXZ': DTOR_B}
    rows = [row(INIT_A, '_$E2'), row(INIT_B, '?seed_b@@YAXXZ', relocations=[('?seed_dtor_b@@YAXXZ', DTOR_B)]),
            row(DTOR_B, '?seed_dtor_b@@YAXXZ'), row(0x4D0000, '_$E9', 'population-2'),
            row(0x4E0000, '?user@@YAXXZ', 'population-3', [('?seed_dtor_b@@YAXXZ', DTOR_B)])]
    for r in rows:
        r['references'] = {'population-1': own, 'population-2': stale, 'population-3': user}[r['population']]
    return dict(regions=rows)


REWRITES = {INIT_B: 'initializer', DTOR_B: INIT_B}


class XcuRewriteTests(unittest.TestCase):
    def test_placeholders_reach_the_home_and_user_populations_and_stale_copies_leave(self):
        cat = catalog()
        affected, stale = apply_rename.place_xcu_rewrites(cat, REWRITES)
        b, d = apply_rename.xcu_placeholder(INIT_B), apply_rename.xcu_placeholder(DTOR_B)
        rows = {r['entry']: r for r in cat['regions']}
        self.assertEqual((rows[INIT_B]['symbol'], rows[DTOR_B]['symbol_prefix']), (b, d))
        self.assertEqual(rows[INIT_B]['relocations'][0]['symbol'], d)
        self.assertEqual(rows[INIT_A]['references'], {'_$E2': INIT_A, b: INIT_B, d: DTOR_B})
        self.assertEqual(rows[0x4D0000]['references'], {'_$E9': 0x4D0000})
        self.assertEqual(stale, {'population-2': ['?seed_b@@YAXXZ']})
        self.assertEqual(rows[0x4E0000]['references'], {d: DTOR_B})
        self.assertEqual(rows[0x4E0000]['relocations'][0]['symbol'], d)
        self.assertEqual(affected, {'population-1', 'population-2', 'population-3'})

    def test_initializer_binds_by_slot_and_destructor_by_its_registration(self):
        cat = catalog()
        apply_rename.place_xcu_rewrites(cat, REWRITES)
        group = [r for r in cat['regions'] if r['population'] == 'population-1']
        # Fresh numbering: A is _$E3, B is _$E8 (registering _$E6); the section lists A, B.
        coff = FakeCoff([('_$E3', b'\xc3', []), ('_$E6', b'\xc3', []),
                         ('_$E8', b'\x68\0\0\0\0\xc3', [(1, '_$E6'), (1, '_atexit')])], ['_$E3', '_$E8'])
        table = [OTHER_UNIT, INIT_A, INIT_B, NEXT_UNIT]
        bound = apply_rename.bind_xcu_rewrites(cat, [(group, 'obj')], lambda obj: coff, REWRITES, table)
        self.assertEqual(bound, {'population-1': {apply_rename.xcu_placeholder(INIT_B): '_$E8',
                                                  apply_rename.xcu_placeholder(DTOR_B): '_$E6'}})

    def test_a_section_that_fits_no_window_of_the_table_refuses(self):
        cat = catalog()
        apply_rename.place_xcu_rewrites(cat, REWRITES)
        group = [r for r in cat['regions'] if r['population'] == 'population-1']
        coff = FakeCoff([('_$E8', b'\xc3', [])], ['_$E1', '_$E8', '_$E9'])
        with self.assertRaises(SystemExit):
            apply_rename.bind_xcu_rewrites(cat, [(group, 'obj')], lambda obj: coff, REWRITES,
                                           [OTHER_UNIT, INIT_A, INIT_B, NEXT_UNIT])

    def test_fixed_bindings_are_kept_and_reserved_in_the_rebinding(self):
        coff = FakeCoff([('_$E3', b'\xc3', []), ('_$E8', b'\xc3', [])], [])
        rows = [row(INIT_A, '_$E8')]
        fixed = {apply_rename.xcu_placeholder(INIT_B): '_$E8'}
        mapping = initializer_rebind.group_mapping(coff, rows, {}, lambda a, n: b'\xc3' * n, fixed=fixed)
        self.assertEqual(mapping[apply_rename.xcu_placeholder(INIT_B)], '_$E8')
        self.assertNotEqual(mapping.get('_$E8'), '_$E8')     # the old _$E8 may not keep the reserved name


class StaleTableEntryTests(unittest.TestCase):
    def test_a_name_no_unit_row_relocates_to_is_stale_not_a_conflict(self):
        joining = row(0x500000, '_$E1', 'population-9', [('_unknown_empty_rect', 0xA6E8A0)])
        table = {'_unknown_empty_rect': 0x832490}
        self.assertIn('_unknown_empty_rect', refile_unit.join_conflict(joining, table, ''))
        stale = []
        self.assertIsNone(refile_unit.join_conflict(joining, table, '', used=set(), stale=stale))
        self.assertEqual(stale, ['_unknown_empty_rect'])

    def test_a_used_reference_is_still_compared_in_full(self):
        joining = row(0x500000, '_$E1', 'population-9', [('_unknown_empty_rect', 0xA6E8A0)])
        table = {'_unknown_empty_rect': 0x832490}
        stale = []
        self.assertIn('_unknown_empty_rect', refile_unit.join_conflict(
            joining, table, '', used={'_unknown_empty_rect'}, stale=stale))
        self.assertEqual(stale, [])

    def test_population_used_and_stale_names(self):
        cat = catalog()
        self.assertEqual(refile_unit.population_used(cat, 'population-1'), {'?seed_dtor_b@@YAXXZ'})
        members = [r for r in cat['regions'] if r['population'] == 'population-1']
        self.assertEqual(apply_rename.stale_names(members, set(), '?seed_dtor_b@@YAXXZ', '_x'), {'_x'})


if __name__ == '__main__':
    unittest.main()
