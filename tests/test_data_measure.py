"""Tests for tools/original_target/data_measure.py ("data reproduced from source", lane O89, 2026-11-25): the
counting of proven, referenced and remaining bytes per kind over a synthetic kind map, the left-out vtable slots,
bss outside the file-backed total, and the receipt the progress series reads.

Run: python -m unittest discover -s tests"""
import json
import os
import sys
import tempfile
import time
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools' / 'original_target'))
import data_measure as D     # noqa: E402

BASE = 0x400000


class FakeImage:
    """A kind map of 64 bytes: 16 of strings, 16 of tables, 16 of globals, 16 of bss."""

    def __init__(self):
        self.base = BASE
        self.kind = bytearray([D.CODE['strings']] * 16 + [D.CODE['tables']] * 16 + [D.CODE['globals']] * 16 +
                              [D.BSS] * 16)
        self.backed = [(BASE, BASE + 48)]

    def kind_at(self, va):
        offset = va - self.base
        return self.kind[offset] if 0 <= offset < len(self.kind) else 0


class MeasureTest(unittest.TestCase):
    def setUp(self):
        self.saved = D.classify, D.item_starts
        D.classify = lambda pe, catalog, repository=None: FakeImage()
        D.item_starts = lambda image, repository=None, extra=(): sorted(set(extra) | {BASE + 40})

    def tearDown(self):
        D.classify, D.item_starts = self.saved

    def run_measure(self, copies, placeholders=(), relocations=(), bss=()):
        catalog = dict(regions=[dict(relocations=[dict(kind=6, target=t) for t in relocations])])
        proof = dict(copies=list(copies), placeholders=list(placeholders), bss=list(bss), refused=[])
        return D.measure(None, catalog, proof)

    def test_proven_bytes_counted_once_per_address(self):
        # Two objects emit the same literal (a COMDAT copy each): counted once.
        r = self.run_measure([(BASE, 8, '??_C@a', '.data'), (BASE, 8, '??_C@a', '.data'), (BASE + 16, 4, 'x', '.rdata')])
        self.assertEqual(r['reproduced_bytes'], 12)
        self.assertEqual(r['file_backed_bytes'], 48)
        self.assertEqual(r['kinds']['strings']['proven'], 8)
        self.assertEqual(r['kinds']['tables']['proven'], 4)
        self.assertEqual(r['reproduced_ranges'], ['00400000-00400008', '00400010-00400014'])

    def test_vtable_slot_kept_from_original_is_left_out(self):
        r = self.run_measure([(BASE + 16, 16, '??_7X@@6B@', '.rdata')], placeholders=[(BASE + 20, 4)])
        self.assertEqual(r['reproduced_bytes'], 12)
        self.assertEqual(r['vtable_slot_bytes_left_out'], 4)

    def test_referenced_runs_to_next_item_or_proven_byte(self):
        # Target in globals at +32: the next item start is +40 (8 bytes referenced).
        r = self.run_measure([], relocations=[BASE + 32])
        self.assertEqual(r['kinds']['globals']['referenced'], 8)
        self.assertEqual(r['kinds']['globals']['rest'], 8)
        # A proven byte stops the run before the next item.
        r = self.run_measure([(BASE + 36, 2, 'g', '.data')], relocations=[BASE + 32])
        self.assertEqual(r['kinds']['globals']['referenced'], 4)

    def test_bss_is_outside_the_file_backed_total(self):
        r = self.run_measure([(BASE + 48, 8, 'z', '.bss')], bss=[(BASE + 48, 8)])
        self.assertEqual(r['reproduced_bytes'], 0)
        self.assertEqual(r['kinds']['bss']['bytes'], 16)
        self.assertEqual(r['kinds']['bss']['proven'], 0)
        self.assertEqual(r['bss_laid_out_bytes'], 8)

    def test_with_generated_modules_adds_each_block_once(self):
        # The authored headline stays; the sub-line adds a generated block over bytes no object proves (lane O104).
        catalog = dict(regions=[])
        proof = dict(copies=[(BASE + 16, 4, 'x', '.rdata')], placeholders=[], bss=[], refused=[])
        module = dict(name='m', range=['%08x' % (BASE + 16), '%08x' % (BASE + 32)], bytes=16,
                      flagged_constants=['K'])
        r = D.measure(None, catalog, proof, generated=[module])
        self.assertEqual(r['reproduced_bytes'], 4)
        self.assertEqual(r['with_generated_bytes'], 16)
        self.assertEqual(r['generated_modules'][0]['flagged_constants'], ['K'])
        r = D.measure(None, catalog, proof)
        self.assertEqual(r['with_generated_bytes'], r['reproduced_bytes'])

    def test_table_totals(self):
        r = self.run_measure([(BASE, 16, 's', '.data')])
        table = D.table_markdown(r)
        self.assertIn('| **File-backed total** | **48** | **16** | **0** | **32** |', table)


class ReceiptTest(unittest.TestCase):
    def test_progress_columns_from_newest_exact_receipt(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            self.assertEqual(D.progress_columns(root), dict(data_reproduced_bytes='', data_reproduced_share='',
                                                            data_with_generated_bytes='', data_with_generated_share=''))
            for name, exact, value, age in (('wfo1', True, 100, 20), ('wfo2', False, 300, 10), ('wfo3', True, 200, 0)):
                path = root / 'build' / 'r' / name / 'build-receipt.json'
                path.parent.mkdir(parents=True)
                path.write_text(json.dumps(dict(whole_file_exact=exact, data_reproduced_bytes=value,
                                                data_file_backed_bytes=400)))
                stamp = time.time() - age
                os.utime(path, (stamp, stamp))
            self.assertEqual(D.progress_columns(root), dict(data_reproduced_bytes=200, data_reproduced_share='50.000%',
                                                            data_with_generated_bytes='', data_with_generated_share=''))
            path = root / 'build' / 'r' / 'wfo4' / 'build-receipt.json'
            path.parent.mkdir(parents=True)
            path.write_text(json.dumps(dict(whole_file_exact=True, data_reproduced_bytes=200, data_file_backed_bytes=400,
                                            data_with_generated_bytes=300)))
            self.assertEqual(D.progress_columns(root), dict(data_reproduced_bytes=200, data_reproduced_share='50.000%',
                                                            data_with_generated_bytes=300,
                                                            data_with_generated_share='75.000%'))


class HelpersTest(unittest.TestCase):
    def test_ranges_and_union(self):
        self.assertEqual(D.ranges_of(bytearray(b'\0\1\1\0\1'), BASE), ['00400001-00400003', '00400004-00400005'])
        self.assertEqual(D.union([(5, 8), (0, 2), (1, 4), (8, 9)]), [[0, 4], [5, 9]])

    def test_data_sections(self):
        self.assertTrue(D.is_data_section(dict(size=4, flags=0x40, name='.rdata')))
        self.assertTrue(D.is_data_section(dict(size=4, flags=0x80, name='.bss')))
        self.assertFalse(D.is_data_section(dict(size=4, flags=0x60000020, name='.text')))
        self.assertFalse(D.is_data_section(dict(size=4, flags=0x42000040, name='.debug$S')))
        self.assertFalse(D.is_data_section(dict(size=0, flags=0x40, name='.rdata')))


if __name__ == '__main__':
    unittest.main()
