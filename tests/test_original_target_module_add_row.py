"""module_add_row.py contracts on synthetic text; no game bytes, no compiler."""
import json
import pathlib
import sys
import tempfile
import unittest

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parents[1] / 'tools' / 'original_target'))
import module_add_row
import non_exact

MARK = '// Not exact (RA2_NON_MATCHING): Foo::Bar at 0x00401100, best score 61.50, 2026-11-13; differs: registers'
TEXT = '\n'.join([
    '#include "shared/a.hpp"',
    '',
    '// int Foo::One() (0x00401000)',
    'int Foo::One() { return 1; }',
    '',
    MARK,
    '#ifdef RA2_NON_MATCHING',
    '// int Foo::Bar() (0x00401100)',
    'int Foo::Bar() { return 2; }',
    '#endif',
    '',
    '// int Foo::Two() (0x00401200)',
    'int Foo::Two() { return 3; }',
    ''])


class UnguardTests(unittest.TestCase):
    def test_marker_and_guard_go_text_stays(self):
        out = module_add_row.unguard(TEXT, 0x401100)
        self.assertNotIn('RA2_NON_MATCHING', out)
        self.assertNotIn('Not exact', out)
        self.assertIn('// int Foo::Bar() (0x00401100)\nint Foo::Bar() { return 2; }\n\n// int Foo::Two()', out)
        self.assertEqual(non_exact.guarded_blocks(out), ([], []))

    def test_replacement_takes_the_guarded_lines(self):
        out = module_add_row.unguard(TEXT, 0x401100, '// int Foo::Bar() (0x00401100)\nint Foo::Bar() { return 4; }\n')
        self.assertIn('return 4;', out)
        self.assertNotIn('return 2;', out)
        self.assertNotIn('#endif', out)

    def test_other_entry_or_missing_guard_refuses(self):
        with self.assertRaises(SystemExit):
            module_add_row.unguard(TEXT, 0x401180)
        broken = TEXT.replace('#ifdef RA2_NON_MATCHING\n', '')
        with self.assertRaises(SystemExit):
            module_add_row.unguard(broken, 0x401100)


class PlaceTests(unittest.TestCase):
    def test_insert_in_address_order(self):
        clean = module_add_row.unguard(TEXT, 0x401100)
        draft = '// int Foo::Mid() (0x00401180)\nint Foo::Mid() { return 5; }\n'
        out = module_add_row.place(clean, 0x401180, draft, False, {0x401000, 0x401100, 0x401200})
        self.assertLess(out.index('Foo::Bar() {'), out.index('Foo::Mid() {'))
        self.assertLess(out.index('Foo::Mid() {'), out.index('// int Foo::Two()'))

    def test_no_draft_without_marker_refuses(self):
        with self.assertRaises(SystemExit):
            module_add_row.place(TEXT, 0x401180, None, False, {0x401000})

    def test_address_already_named_refuses(self):
        clean = module_add_row.unguard(TEXT, 0x401100)
        with self.assertRaises(SystemExit):
            module_add_row.place(clean, 0x401100, 'int x;', False, {0x401000, 0x401200})

    def test_marker_path_unguards(self):
        out = module_add_row.place(TEXT, 0x401100, None, True, {0x401000, 0x401200})
        self.assertNotIn('RA2_NON_MATCHING', out)


class RegionTests(unittest.TestCase):
    def test_region_extent_and_bound_population(self):
        region = module_add_row.new_region(
            0x401100, b'\xc3' * 13, '909090', population='population-1', bound_population='population-2',
            source='src/recovered/foo.cpp', symbol_prefix='?Bar@Foo@@', options=['/O2'], references={},
            source_sha256='0' * 64, source_order=7)
        self.assertEqual((region['body_bytes'], region['padding_bytes'], region['full_extent_bytes']), (13, 3, 16))
        self.assertEqual(region['bound_population'], 'population-2')
        self.assertEqual(region['population'], 'population-1')
        self.assertEqual(region['method'], '00401100')
        self.assertEqual(region['relocations'], [])

    def test_overlapping(self):
        catalog = dict(regions=[dict(entry=0x401000, full_extent_bytes=0x20), dict(entry=0x401200, full_extent_bytes=0x10)])
        self.assertEqual(module_add_row.overlapping(catalog, 0x401100, 0x100), [])
        self.assertEqual(len(module_add_row.overlapping(catalog, 0x401010, 0x10)), 1)
        self.assertEqual(len(module_add_row.overlapping(catalog, 0x4011F8, 0x10)), 1)


class ModuleItemTests(unittest.TestCase):
    def test_unknown_unit_refuses_and_known_reads_map(self):
        with tempfile.TemporaryDirectory() as root:
            root = pathlib.Path(root)
            with self.assertRaises(SystemExit):
                module_add_row.module_item('foo', root)
            folder = root / 'manifests/original_target/modules'
            folder.mkdir(parents=True)
            (folder / 'foo.json').write_text(json.dumps(dict(source='src/recovered/foo.cpp', population='population-9')))
            self.assertEqual(module_add_row.module_item('foo', root), ('src/recovered/foo.cpp', 'population-9'))


if __name__ == '__main__':
    unittest.main()
