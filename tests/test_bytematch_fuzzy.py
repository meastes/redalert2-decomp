"""Tests for the ByteMatch fuzzy figure (bytematch_report.py; lane O90, user 2026-11-25): each function not exact
at its best measured draft score, (body - differing) / body against its image-derived bound, clamped to [0, 100].

Run: python -m unittest discover -s tests"""
import hashlib
import json
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools' / 'original_target'))
import bytematch_report as B     # noqa: E402


class FuzzyPercentTests(unittest.TestCase):
    def test_share_of_the_body_that_matches(self):
        self.assertAlmostEqual(B.fuzzy_percent(1000, 250), 75.0)
        self.assertEqual(B.fuzzy_percent(64, 0), 100.0)

    def test_clamped_and_unbounded(self):
        self.assertEqual(B.fuzzy_percent(10, 30), 0.0)        # a longer draft: differing counts the extra length
        self.assertEqual(B.fuzzy_percent(0, 0), 0.0)          # no bound
        self.assertEqual(B.fuzzy_percent(None, 3), 0.0)
        self.assertEqual(B.fuzzy_percent(100, None), 0.0)     # no measurement


class LabelTests(unittest.TestCase):
    def test_counts_and_stated_bounds(self):
        self.assertEqual(B.label_differing('// not exact: 61 of 2507 bytes differ (image-derived bound 005befe0)'),
                         (61, [2507]))
        self.assertEqual(B.label_differing('// not exact: against the image bound (4,012 B) 1,285 bytes differ'),
                         (1285, [4012]))
        self.assertEqual(B.label_differing('// not exact: 1444 bytes differ (adopted text)'), (1444, []))
        self.assertEqual(B.label_differing('// not exact: no row of the original matches it yet (unplaced)'),
                         (None, []))

    def test_by_position_wins_and_side_sizes_are_not_bounds(self):
        self.assertEqual(B.label_differing('// not exact: 17 bytes differ in 16 instructions (1871 by position)')[0],
                         1871)
        # 1207 B is another form's size, not the bound the count was measured against
        self.assertEqual(B.label_differing('// not exact: 1045 bytes differ; any archive form expands it (1207 B)'),
                         (1045, []))

    def test_entry_from_the_bound_not_from_callees(self):
        rows = [dict(entry='0072a880', symbol='?WDT_Write_Map_Ini@@YIXABVWstring@@0@Z'),
                dict(entry='0072af50', symbol='??4VectorCursor@@QAEAAV0@ABV0@@Z')]
        bounded = {0x72a880: 1652, 0x72af50: 21}
        label = '// not exact: 1 of 1652 bytes differ; VectorCursor::operator= at 0x72AF50 matches'
        body = '\nvoid __fastcall WDT_Write_Map_Ini(Wstring const & a, Wstring const & b)\n{\n}\n'
        self.assertEqual(B.guard_entry(label, body, rows, bounded), 0x72a880)
        self.assertEqual(B.guard_entry('// not exact: 9 bytes differ against the image-derived bound 0072af50 (21 B)',
                                       '\nint f(void)\n{\n}\n', rows, bounded), 0x72af50)
        self.assertIsNone(B.guard_entry(label, '\nint Other(void)\n{\n}\n', rows, bounded))


class ScoreSourceTests(unittest.TestCase):
    """Sources in order: current last check, guard label, parked_rows, older last check, else 0."""

    def worktree(self, last_source_current=True):
        root = Path(tempfile.mkdtemp())
        (root / 'manifests').mkdir()
        (root / 'src').mkdir()
        (root / 'build/r/last').mkdir(parents=True)
        text = ('void A(void) {}\n'
                '#ifdef RA2_NON_MATCHING // not exact: 30 of 200 bytes differ\nvoid B(void)\n{\n}\n#endif\n'
                '#ifdef RA2_NON_MATCHING // not exact: 5 of 999 bytes differ\nvoid E(void)\n{\n}\n#endif\n'
                '#ifdef RA2_NON_MATCHING // not exact: 7 bytes differ against the image-derived bound 00406000 (80 B)\n'
                'void F(void)\n{\n}\n#endif\n')
        (root / 'src/u.cpp').write_text(text)
        rows = [dict(entry='00401000', body=100, symbol='?A@@YAXXZ'), dict(entry='00402000', body=200, symbol='?B@@YAXXZ'),
                dict(entry='00403000', body=50, symbol='?C@@YAXXZ'), dict(entry='00404000', body=40, symbol='?D@@YAXXZ'),
                dict(entry='00405000', body=60, symbol='?E@@YAXXZ')]
        units = dict(u=dict(source='src/u.cpp', rows=rows,
                            parked_rows=[dict(entry='00403000', body=50, best_differing=10),
                                         dict(entry='00405000', body=61, best_differing=1)]))
        (root / 'manifests/units.json').write_text(json.dumps(units))
        (root / 'manifests/image_bounds.csv').write_text(
            'entry,end_exclusive,body_bytes,padding_bytes,next_start,status\n'
            '00406000,00406050,80,0,00406050,bound\n00407000,00407050,80,0,00407050,refused\n')
        sha = hashlib.sha256(text.encode() if last_source_current else b'old').hexdigest()
        (root / 'build/r/last/u.json').write_text(json.dumps(dict(source_sha256=sha, results=[
            dict(entry='00401000', body=100, status='differs', differing=20, size=100),
            dict(entry='00402000', body=200, status='missing'),
            dict(entry='00404000', body=40, status='differs', differing=4, size=44)])))
        return root

    def test_priority(self):
        _, owner, score, _ = B.rebuild_scores(self.worktree())
        self.assertEqual(score[0x401000], (80.0, 'check'))
        self.assertEqual(score[0x402000], (85.0, 'label'))
        self.assertEqual(score[0x403000], (80.0, 'parked'))
        self.assertEqual(score[0x404000], (90.0, 'check'))
        self.assertNotIn(0x405000, score)        # label and parked row measured against other bounds
        self.assertEqual(score[0x406000], (91.25, 'label'))   # no row; image_bounds.csv bound
        self.assertEqual(owner[0x401000], 'u')

    def test_older_source_is_marked_and_last(self):
        _, _, score, _ = B.rebuild_scores(self.worktree(last_source_current=False))
        self.assertEqual(score[0x401000], (80.0, 'check (older source)'))
        self.assertEqual(score[0x402000], (85.0, 'label'))


if __name__ == '__main__':
    unittest.main()
