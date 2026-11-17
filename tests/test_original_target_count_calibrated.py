"""Count-calibrated rows (user decision 2026-11-09): the `count_calibrated` flag needs its note, travels
through plans and acceptance, stays equal between frozen candidate and accepted row, and STATUS reports it
as its own category inside matched C/C++."""
from datetime import datetime, timezone
from pathlib import Path
import sys
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1]/'tools/original_target'))
from canonical import COUNT_CALIBRATED_KEYS, validate_count_calibrated
import accept_sources
import lane_tools
import mark_count_calibrated
import status
from verify_manifest import Audit

NOTE = 'named locals dy, distance (count window k=0..1); checked YR twin 0x6FDD50 and TibSun Fire_At first'
CALIBRATED = dict(count_calibrated=True, count_calibrated_note=NOTE)


class ValidateTests(unittest.TestCase):
    def test_unflagged_row_is_accepted(self):
        self.assertFalse(validate_count_calibrated({}))

    def test_flag_with_note_is_accepted(self):
        self.assertTrue(validate_count_calibrated(CALIBRATED))

    def test_flag_needs_a_note(self):
        for row in (dict(count_calibrated=True), dict(CALIBRATED, count_calibrated_note=''),
                    dict(CALIBRATED, count_calibrated_note='   '), dict(CALIBRATED, count_calibrated_note=['x'])):
            with self.assertRaisesRegex(ValueError, 'needs a non-empty count_calibrated_note'):
                validate_count_calibrated(row)

    def test_flag_must_be_true(self):
        for value in (False, 'true', 1, 'yes'):
            with self.assertRaisesRegex(ValueError, 'count_calibrated must be true'):
                validate_count_calibrated(dict(CALIBRATED, count_calibrated=value))

    def test_note_without_flag_is_refused(self):
        with self.assertRaisesRegex(ValueError, 'count_calibrated_note without'):
            validate_count_calibrated(dict(count_calibrated_note=NOTE))

    def test_key_lists_agree(self):
        self.assertEqual(accept_sources.COUNT_CALIBRATED_KEYS, COUNT_CALIBRATED_KEYS)
        self.assertEqual(lane_tools.LEDGER_KEYS, accept_sources.LEDGER_KEYS)


class CarryTests(unittest.TestCase):
    def plan(self, **item):
        return dict(sources=[dict(entry='00500000', **item), dict(entry='00500010')])

    def test_unstated_item_inherits_the_existing_flag(self):
        plan = self.plan()
        existing = {0x500000: dict(entry=0x500000, **CALIBRATED), 0x500010: dict(entry=0x500010)}
        self.assertEqual(accept_sources.carry_count_calibration(plan, existing), [0x500000])
        self.assertEqual({k: plan['sources'][0][k] for k in COUNT_CALIBRATED_KEYS}, CALIBRATED)
        self.assertNotIn('count_calibrated', plan['sources'][1])

    def test_stated_item_keeps_its_own_note(self):
        plan = self.plan(count_calibrated=True, count_calibrated_note='new note')
        existing = {0x500000: dict(entry=0x500000, **CALIBRATED)}
        self.assertEqual(accept_sources.carry_count_calibration(plan, existing), [])
        self.assertEqual(plan['sources'][0]['count_calibrated_note'], 'new note')

    def test_false_clears_the_flag(self):
        plan = self.plan(count_calibrated=False, count_calibrated_note='natural form found')
        existing = {0x500000: dict(entry=0x500000, **CALIBRATED)}
        accept_sources.carry_count_calibration(plan, existing)
        self.assertFalse(any(k in plan['sources'][0] for k in COUNT_CALIBRATED_KEYS))

    def test_new_row_without_keys_stays_unflagged(self):
        plan = self.plan()
        self.assertEqual(accept_sources.carry_count_calibration(plan, {}), [])
        self.assertNotIn('count_calibrated', plan['sources'][0])


class LanePlanTests(unittest.TestCase):
    def test_plan_copies_ledger_flags(self):
        regions = [dict(entry=0x500000, source_order=0, population='p1', symbol_prefix='?a@@YAXXZ',
                        source='src/recovered/a.cpp', **CALIBRATED),
                   dict(entry=0x500010, source_order=1, population='p1', symbol_prefix='?b@@YAXXZ',
                        source='src/recovered/b.cpp', inline_asm='evidenced', inline_asm_citation='TibSun lcw.cpp:1')]
        _, plan = lane_tools.build_plan(regions, '500000', 'lane', [], 'p', copy=lambda a, b: None)
        self.assertEqual({k: plan['sources'][0][k] for k in COUNT_CALIBRATED_KEYS}, CALIBRATED)
        self.assertNotIn('inline_asm', plan['sources'][0])
        self.assertEqual(plan['sources'][1]['inline_asm_citation'], 'TibSun lcw.cpp:1')
        self.assertNotIn('count_calibrated', plan['sources'][1])


class VerifyRowTests(unittest.TestCase):
    """Audit.verify_row: the flag and note equal the frozen candidate's, and the note goes with the flag."""
    def verify(self, row_keys, candidate_keys):
        base = dict(symbol_prefix='?a@@YAXXZ', references={}, padding_provenance=None)
        candidate = dict(base, **candidate_keys)
        row = dict(base, entry=0x500000, method='00500000', variant=0, accepted_batch_manifest='m', **row_keys)
        audit = Audit.__new__(Audit)
        audit.verify_row(row, 'm', {'m': ({'00500000': candidate}, {}, {}, {}, 'h')})

    def test_equal_flag_reaches_the_compiler_case_check(self):
        with self.assertRaisesRegex(ValueError, 'Missing exact compiler case'):
            self.verify(CALIBRATED, CALIBRATED)
        with self.assertRaisesRegex(ValueError, 'Missing exact compiler case'):
            self.verify({}, {})

    def test_flag_or_note_differing_from_the_candidate_is_refused(self):
        for row, candidate in ((CALIBRATED, {}), ({}, CALIBRATED),
                               (CALIBRATED, dict(CALIBRATED, count_calibrated_note='other'))):
            with self.assertRaisesRegex(ValueError, 'count-calibration flag differs from frozen candidate'):
                self.verify(row, candidate)

    def test_flag_without_note_is_refused(self):
        with self.assertRaisesRegex(ValueError, 'needs a non-empty count_calibrated_note'):
            self.verify(dict(count_calibrated=True), dict(count_calibrated=True))


class StatusTests(unittest.TestCase):
    def test_calibrated_bytes_are_reported_inside_matched_source(self):
        catalog = dict(regions=[dict(body_bytes=100, padding_bytes=4, population='p1'),
                                dict(body_bytes=60, padding_bytes=0, population='p1', **CALIBRATED),
                                dict(body_bytes=40, padding_bytes=0, population='p1', inline_asm='evidenced',
                                     inline_asm_citation='TibSun conquer.cpp:346')])
        self.assertEqual(status.calibrated_coverage(catalog), (1, 60))
        start = datetime(2026, 11, 9, 12, tzinfo=timezone.utc)
        text = status.render(catalog, 1000, [dict(commit='a', time=start, subject='', regions=3, body=160, padding=4)])
        lines = text.splitlines()
        self.assertIn('| Matched C/C++ body bytes | 160 (16.00% of .text) |', lines)
        calibrated = [l for l in lines if l.startswith('| Count-calibrated C/C++ bytes')]
        self.assertEqual(len(calibrated), 1)
        self.assertTrue(calibrated[0].endswith('| 60 in 1 regions (6.00% of .text) |'))
        self.assertEqual(lines.index(calibrated[0]), lines.index('| Matched C/C++ body bytes | 160 (16.00% of .text) |') + 1)
        self.assertIn('| Original .text scaffold bytes | 796 |', lines)   # unchanged: calibrated bytes stay C/C++
        self.assertEqual(status.coverage(catalog)['body'], 160)

    def test_zero_when_no_row_is_flagged(self):
        text = status.render(dict(regions=[dict(body_bytes=10, padding_bytes=0, population='p1')]), 1000, [])
        self.assertIn('| 0 in 0 regions (0.00% of .text) |', text)


class MarkTests(unittest.TestCase):
    def catalog(self):
        return dict(regions=[dict(entry=0x500000), dict(entry=0x500010, **CALIBRATED)])

    def test_marks_set_replace_and_clear(self):
        catalog = self.catalog()
        changed = mark_count_calibrated.apply_marks(catalog, {0x500000: NOTE, 0x500010: None})
        self.assertEqual(changed, [0x500000, 0x500010])
        self.assertEqual({k: catalog['regions'][0][k] for k in COUNT_CALIBRATED_KEYS}, CALIBRATED)
        self.assertFalse(any(k in catalog['regions'][1] for k in COUNT_CALIBRATED_KEYS))

    def test_marks_are_idempotent(self):
        catalog = self.catalog()
        self.assertEqual(mark_count_calibrated.apply_marks(catalog, {0x500010: NOTE}), [])

    def test_marks_refuse_unaccepted_entries_and_empty_notes(self):
        with self.assertRaises(SystemExit):
            mark_count_calibrated.apply_marks(self.catalog(), {0x500020: NOTE})
        with self.assertRaisesRegex(ValueError, 'non-empty count_calibrated_note'):
            mark_count_calibrated.apply_marks(self.catalog(), {0x500000: ' '})


if __name__ == '__main__':
    unittest.main()
