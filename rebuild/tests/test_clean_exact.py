"""Tests for the rebuild's 'exact with no judgment call' figure (rebuild_census.clean_exact, judgment_entries;
user-approved process change, 2026-11-22, lane O37): the headline sets with every flagged row left out.

Run: RA2_TREE=rebuild python -m unittest discover -s rebuild/tests"""
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
import rebuild_census as C     # noqa: E402


class CleanExactTests(unittest.TestCase):
    # a, b catalogued; c exact in its unit check only; d count-sensitive; e evidenced inline asm.
    WHOLE = {'00401000': 104, '00402000': 50, '00405000': 30}
    FUNCTION = dict(WHOLE, **{'00403000': 20, '00404000': 40})
    BODY = {'00401000': 100, '00402000': 48, '00403000': 16, '00404000': 36, '00405000': 30}
    ASM = {'00405000': 30}

    def test_nothing_flagged_keeps_the_headline(self):
        out = C.clean_exact(self.WHOLE, self.FUNCTION, set(), set(), self.ASM, self.BODY)
        self.assertEqual(out['whole'], (3, 184, 148))           # source bytes: body only, no padding, no asm
        self.assertEqual(out['function'], (5, 244, 200))
        self.assertEqual((out['judgment'], out['sensitive']), ((0, 0), (0, 0)))

    def test_judgment_and_count_sensitive_rows_are_left_out_of_both_levels(self):
        out = C.clean_exact(self.WHOLE, self.FUNCTION, {'00401000', '00403000', '00999000'}, {'00404000'},
                            self.ASM, self.BODY)
        self.assertEqual(out['whole'], (2, 80, 48))            # b and e; source bytes are b's body (e is asm)
        self.assertEqual(out['function'], (2, 80, 48))
        self.assertEqual(out['judgment'], (2, 116))            # body bytes; the unknown entry counts nothing
        self.assertEqual(out['sensitive'], (1, 36))

    def test_a_row_both_flagged_and_count_sensitive_counts_once_as_judgment(self):
        out = C.clean_exact(self.WHOLE, self.FUNCTION, {'00404000'}, {'00404000'}, {}, self.BODY)
        self.assertEqual((out['judgment'], out['sensitive']), ((1, 36), (0, 0)))
        self.assertEqual(out['function'][:2], (4, 204))

    def test_header_dependents_read_addresses_from_free_text(self):
        text = 'cell 00476cc0; uicmnds 006f6d00, 006f8a70; netshare LBSaveSelections 0x5C5FF0; Defaced 2026-11-20'
        found = {m.group(1).lower().zfill(8) for m in C.HEX_ENTRY.finditer(text)}
        self.assertEqual(found, {'00476cc0', '006f6d00', '006f8a70', '005c5ff0'})


if __name__ == '__main__':
    unittest.main()
