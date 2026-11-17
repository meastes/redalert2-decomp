"""native_units.py header-static copies keyed by the catalog (Route A regression: re-filed units
lost them); synthetic rows and text, no game bytes."""
import pathlib
import sys
import tempfile
import unittest

ROOT = pathlib.Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'native' / 'tools'))
import native_units

BSS = {0xAD057C: '_unknown_bridge_height_ad057c', 0x850C40: '_unknown_level_850c40'}
ROWS = [dict(relocations=[dict(symbol='_unknown_geometry_j', target=0xAD057C),
                          dict(symbol='?Rules@@3PAVRulesClass@@A', target=0x839848)]),
        dict(relocations=[dict(symbol='_unknown_level', target=0x850C40), dict(symbol='_other', target=0x111111)])]
MODULE = 'static int const unknown_geometry_j = 416;\nstatic int unknown_level = 104;\nint f() { return 1; }\n'


class HeaderStaticTests(unittest.TestCase):
    def test_candidates_are_the_c_named_data_of_the_rows(self):
        self.assertEqual(native_units.header_static_candidates(ROWS),
                         [('other', 0x111111), ('unknown_geometry_j', 0xAD057C), ('unknown_level', 0x850C40)])

    def test_a_module_source_copies_by_catalog_without_ref_lines(self):
        record, done = [], set()
        text = native_units.header_static_copies(MODULE, BSS, native_units.header_static_candidates(ROWS), done, record, 'u.cpp')
        self.assertIn('__asm__("_unknown_bridge_height_ad057c")', text)
        self.assertIn('&unknown_level, sizeof unknown_level', text)
        self.assertEqual(sorted(c['symbol'] for c in record), ['_unknown_bridge_height_ad057c', '_unknown_level_850c40'])
        self.assertEqual(done, {0xAD057C, 0x850C40})

    def test_one_copy_per_storage_and_unit(self):
        done = set()
        candidates = native_units.header_static_candidates(ROWS)
        native_units.header_static_copies(MODULE, BSS, candidates, done)
        self.assertEqual(native_units.header_static_copies(MODULE, BSS, candidates, done), '')

    def test_ref_lines_still_key_a_one_row_file(self):
        text = '// @ref _unknown_geometry_j=00ad057c\nstatic int const unknown_geometry_j = 416;\n'
        self.assertIn('native_hs_ad057c', native_units.header_static_copies(text, BSS))
        self.assertEqual(native_units.header_static_copies('int unknown_geometry_j;\n', BSS, [('unknown_geometry_j', 0xAD057C)]), '')

    def test_check_names_missing_listed_and_unlisted_storages(self):
        saved = native_units.HEADER_STATICS
        with tempfile.TemporaryDirectory() as folder:
            native_units.HEADER_STATICS = pathlib.Path(folder) / 'header_statics.tsv'
            try:
                copies = [dict(address='0x00AD057C', symbol='_unknown_bridge_height_ad057c', name='unknown_geometry_j', unit='u.cpp')]
                expected = [('u.cpp', 'unknown_geometry_j', 0xAD057C, '_unknown_bridge_height_ad057c')]
                with self.assertRaises(SystemExit) as unlisted:
                    native_units.check_header_statics(copies, expected)
                self.assertIn('not listed', str(unlisted.exception))
                self.assertEqual(native_units.check_header_statics(copies, expected, write=True), 1)
                self.assertEqual(native_units.check_header_statics(copies, expected), 1)
                with self.assertRaises(SystemExit) as dropped:
                    native_units.check_header_statics([], [])
                self.assertIn('_unknown_bridge_height_ad057c (0x00AD057C) is listed', str(dropped.exception))
                with self.assertRaises(SystemExit) as missing:
                    native_units.check_header_statics(copies, expected + [('v.cpp', 'unknown_level', 0x850C40, '_unknown_level_850c40')])
                self.assertIn('v.cpp: static unknown_level is not copied to _unknown_level_850c40', str(missing.exception))
            finally:
                native_units.HEADER_STATICS = saved


if __name__ == '__main__':
    unittest.main()
