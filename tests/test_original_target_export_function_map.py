"""export_function_map.py: one CSV row per catalog region and draft, ordered by address; synthetic rows."""
import csv
import io
from pathlib import Path
import sys
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools/original_target'))
import export_function_map as fm

CATALOG = dict(regions=[
    dict(entry=0x402000, body_bytes=40, source='src/recovered/b.cpp', symbol='?B@@YAXXZ', population='population-2',
         options=['/O2', '/Ob1'], count_calibrated=True, unit_evidence='xcu-prefix', bound_population='population-9'),
    dict(entry=0x401000, body_bytes=16, source='src/recovered/a.cpp', symbol='_a', population='population-1',
         options=['/O2'], declarations_only=False, inline_asm='evidenced'),
])
DRAFTS = [dict(entry='00402000', bytes=44, source='src/recovered/b.cpp', symbol='?B@@YAXXZ',
               non_exact=dict(score=97.5)),
          dict(entry='00403000', bytes=80, source='src/recovered/c.cpp', non_exact=dict(score=12.0))]


class FunctionMapTests(unittest.TestCase):
    def table(self):
        return list(csv.reader(io.StringIO(fm.render(fm.rows(CATALOG, DRAFTS)))))

    def test_header_and_order(self):
        table = self.table()
        self.assertEqual(tuple(table[0]), fm.COLUMNS)
        self.assertEqual([r[0] for r in table[1:]], ['0x00401000', '0x00402000', '0x00402000', '0x00403000'])
        self.assertEqual(table[2][6].split(';')[0], 'exact')          # the accepted row before the draft
        self.assertEqual(table[3][6], 'non_exact score=97.5')

    def test_columns_and_flags(self):
        table = self.table()
        self.assertEqual(table[1], ['0x00401000', '16', 'src/recovered/a.cpp', '_a', 'population-1', '/O2',
                                    'exact;inline_asm=evidenced'])
        self.assertEqual(table[2][4:], ['population-2', '/O2 /Ob1',
                                        'exact;count_calibrated;unit_evidence=xcu-prefix;bound_population=population-9'])
        self.assertEqual(table[4], ['0x00403000', '80', 'src/recovered/c.cpp', '', '', '', 'non_exact score=12.0'])


if __name__ == '__main__':
    unittest.main()
