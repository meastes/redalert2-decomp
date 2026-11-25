"""Tests for the rebuild's 'exact with no judgment call' figure (rebuild_census.clean_exact, judgment_entries;
user-approved process change, 2026-11-22, lane O37): the headline sets with every flagged row left out.

Run: python -m unittest discover -s tests"""
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools' / 'original_target'))
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



class JudgmentKindTests(unittest.TestCase):
    """Judgment kinds and 'exact with no soft judgment' (review session request, 2026-11-23, lane O63)."""
    PROV = [dict(entry='00401000', judgment='[iv-a] twin statement at the same place'),
            dict(entry='00402000', judgment='[iv-b] a looked-up local no reference has'),
            dict(entry='00403000', judgment='an untagged note'),
            dict(entry='00404000', judgment=''),
            dict(entry='00405000', judgment=' [v] bound: image-derived')]
    HEADER = [dict(header='a.h', member='A', judgment='[i] the one reference form every caller is exact under',
                   dependents='a 00406000; b 0x407000; c 00401000'),
              dict(header='b.h', member='B', judgment='[vii] which group is const is not fixed by the image',
                   dependents='b 00407000; d 00999000'),
              dict(header='c.h', member='C', judgment='', dependents='e 00408000')]
    KNOWN = {'0040%d000' % i for i in range(1, 9)}

    def test_prefix_is_read_and_stripped(self):
        self.assertEqual(C.judgment_kind('[iv-b] x'), 'iv-b')
        self.assertEqual(C.judgment_kind('[vii]x'), 'vii')
        self.assertIsNone(C.judgment_kind('inliner: [i] not a prefix'))
        self.assertEqual(C.judgment_text('[iii] inliner: return;'), 'inliner: return;')
        self.assertEqual(C.judgment_text(' plain '), 'plain')

    def test_joined_kind_is_read_and_not_soft(self):
        self.assertEqual(C.judgment_kind('[joined] TibSun statements + twin term'), 'joined')
        kinds, unclassified = C.classify_judgments([{'entry': '00401000', 'judgment': '[joined] x'}], [], {'00401000'})
        self.assertEqual((kinds, unclassified), ({'00401000': ('joined', False)}, []))

    def test_tree_header_judgments_hold_no_held_items(self):
        # A held structure item is not in the tree, so the rows it would flip rest on no judgment: it belongs in
        # variation_candidates.csv, not header_judgments.csv (owner, 2026-11-23: OwnerDraw::WinData, codex1 r2703).
        import csv
        path = Path(__file__).resolve().parents[1] / 'manifests' / 'header_judgments.csv'
        for r in csv.DictReader(path.open(encoding='utf-8')):
            self.assertNotRegex(C.judgment_text(r['judgment']), r'^(structure item, )?held\b', r['member'])
            self.assertIsNotNone(C.judgment_kind(r['judgment']), r['member'])

    def test_classification(self):
        kinds, unclassified = C.classify_judgments(self.PROV, self.HEADER, self.KNOWN)
        self.assertEqual(kinds, {'00401000': ('iv-a', False), '00402000': ('iv-b', True),
                                 '00403000': ('vii', True), '00405000': ('v', False),
                                 '00406000': ('vi', False),     # rests on an [i] form only
                                 '00407000': ('vi', True)})     # rests on a [vii] form too
        self.assertEqual(unclassified, [('provenance', '00403000')])

    def test_split_and_soft_free(self):
        kinds, _ = C.classify_judgments(self.PROV, self.HEADER, self.KNOWN)
        body = {e: 10 * int(e[4]) for e in self.KNOWN}
        function = {e: body[e] + 2 for e in self.KNOWN if e != '00408000'}
        split, soft = C.kind_split(function, kinds, body)
        self.assertEqual(split['vi'], (2, 130))
        self.assertEqual(split['ii'], (0, 0))
        self.assertEqual(soft, (3, 20 + 30 + 70))
        self.assertEqual(C.format_kinds(split), 'iv-a 1/10; iv-b 1/20; v 1/50; vi 2/130; vii 1/30')
        soft_set = {e for e, (_, s) in kinds.items() if s}
        out = C.clean_exact(function, function, soft_set, set(), {}, body)
        self.assertEqual(out['function'], (4, 10 + 40 + 50 + 60 + 8, 160))
        self.assertEqual(out['judgment'], soft)


if __name__ == '__main__':
    unittest.main()
