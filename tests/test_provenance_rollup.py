"""provenance_rollup.py (lane O88): free-text text_source mapped to one source; workers and unit-file origins."""
import csv
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools' / 'original_target'))
import provenance_rollup as R     # noqa: E402


class NormaliseTests(unittest.TestCase):
    def test_classes_and_single_references(self):
        self.assertEqual(R.normalise('b', 'old tree'), 'reshaped-from-main')
        self.assertEqual(R.normalise('c', 'seed'), 'fresh')
        self.assertEqual(R.normalise('a', 'mischa85'), 'mischa85')
        self.assertEqual(R.normalise('a', "ra2-decomp's matched Yuri's Revenge twin"), 'ra2-decomp')
        self.assertEqual(R.normalise('a', 'gibbo101/ra2-decomp pinned 449afe2'), 'ra2-decomp')
        self.assertEqual(R.normalise('a', 'TibSun vector.h'), 'tibsun')
        self.assertEqual(R.normalise('a', 'RobinDigitalPro Draw_All'), 'robin')
        self.assertEqual(R.normalise('a', 'Generals GameText.cpp'), 'generals')
        self.assertEqual(R.normalise('a', 'adopted from robin-ra2-1001-decomp bd7e611 (src/m85/src/alphashp.cpp; mischa85 had none)'),
                         'robin')

    def test_several_or_none_is_mixed(self):
        self.assertEqual(R.normalise('a', 'existing adopted mischa85/ra2-decomp bodies'), 'mixed')
        self.assertEqual(R.normalise('a', 'inherited adopted unit'), 'mixed')
        self.assertEqual(R.normalise('a', ''), 'mixed')


class RollupTests(unittest.TestCase):
    def test_workers_first_then_file_origin(self):
        with tempfile.TemporaryDirectory() as tmp:
            m = Path(tmp)
            def write(name, header, rows):
                with open(m / name, 'w', newline='') as f:
                    w = csv.writer(f); w.writerow(header); w.writerows(rows)
            write('provenance.csv', ['unit', 'entry', 'symbol', 'body_bytes', 'class', 'text_source'],
                  [['u', '00401000', 'a', 10, 'a', 'inherited adopted unit'], ['u', '00402000', 'b', 20, 'b', 'old tree'],
                   ['v', '00403000', 'c', 30, 'a', 'inherited adopted unit']])
            write('adopted.csv', ['file', 'origin', 'commit', 'origin_path', 'sha256'], [['src/u.cpp', 'mischa85', 'x', 'y', 'z']])
            write('workers.csv', ['unit', 'worker', 'reason'], [])
            write('worker_rows.csv', ['unit', 'entry', 'worker'], [['u', '00402000', 'codex1']])
            regions = [dict(entry=0x401000, source='src/u.cpp', body_bytes=10),
                       dict(entry=0x402000, source='src/u.cpp', body_bytes=20),
                       dict(entry=0x403000, source='src/v.cpp', body_bytes=30)]
            out = R.rollup(regions, m)
            self.assertEqual(out['mischa85'], [1, 10, 1])
            self.assertEqual(out['windows-workers'], [1, 20, 0])
            self.assertEqual(out['mixed'], [1, 30, 0])


if __name__ == '__main__':
    unittest.main()
