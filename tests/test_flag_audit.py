"""flag_audit.py's classes (lane O93): synthetic provenance rows, no compiler."""
import csv
from pathlib import Path
import sys
import tempfile
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools/original_target'))
import flag_audit


def row(text_source='', note='', judgment='', entry='00401000'):
    return dict(unit='u', entry=entry, symbol='?f@@YIXXZ', text_source=text_source, note=note, judgment=judgment)


class FlagAuditTest(unittest.TestCase):
    def test_twin_over_archive(self):
        r = row("ra2-decomp's matched twin over the archive's text")
        self.assertEqual(flag_audit.classify(r), [('twin-over-archive', 'empty')])
        self.assertEqual(flag_audit.classify(dict(r, judgment='[spelling] x')), [('twin-over-archive', 'kind spelling')])
        self.assertEqual(flag_audit.classify(dict(r, judgment='[spelling] x [iv-a] y')), [])
        self.assertEqual(flag_audit.classify(dict(r, judgment='[joined] x')), [])

    def test_joined_and_count(self):
        self.assertEqual(flag_audit.classify(row("the twin's text with the TibSun archive's test")), [('joined', 'empty')])
        self.assertEqual(flag_audit.classify(row('mischa85', 'count-sensitive row exact again')), [('count', 'empty')])
        self.assertEqual(flag_audit.classify(row("ra2-decomp's matched twin, whole")), [])

    def test_fix_rewrites_only_the_flagged_line(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / 'provenance.csv'
            lines = ['unit,entry,symbol,text_source,note,judgment',
                     'u,00401000,?f@@YIXXZ,"twin over the archive\'s text",,',
                     'u,00401010,?g@@YIXXZ,"quoted, kept",n,']
            path.write_text('\n'.join(lines) + '\n')
            flag_audit.main(['--fix', '--csv', str(path), '--units', str(Path(tmp) / 'none.json')])
            out = path.read_text().split('\n')
            self.assertEqual(out[2], lines[2])
            self.assertTrue(next(csv.reader([out[1]]))[5].startswith('[iv-a] twin text where the archive differs'))


if __name__ == '__main__':
    unittest.main()
