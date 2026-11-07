"""status.py ledger split: CRT/library and handwritten assembly booked apart, in-scope percentage beside .text."""
from pathlib import Path
import sys
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1]/'tools/original_target'))
import status

START = status.TEXT_START
# A .text that reaches 0x100 bytes past CRT_START, so the CRT span is 0x100 bytes.
TEXT = status.CRT_START + 0x100 - START


def region(entry, body, padding=0, **extra):
    return dict(entry=entry, body_bytes=body, padding_bytes=padding, population='p1', **extra)


class LedgerSplitTests(unittest.TestCase):
    def setUp(self):
        self.saved = status.HANDWRITTEN
        status.HANDWRITTEN = ((0x500000, 0x500040, 'codec'), (status.CRT_START - 0x10, status.CRT_START + 0x10, 'tail'))

    def tearDown(self):
        status.HANDWRITTEN = self.saved

    def test_categories_are_disjoint(self):
        catalog = dict(regions=[region(0x401000, 100, 4), region(status.CRT_START + 0x20, 16, 0)])
        split = status.ledger(catalog, TEXT)
        self.assertEqual(split['asm'], 0x40 + 0x20)
        self.assertEqual(split['crt'], 0x100 - 0x10)          # the handwritten tail wins the overlap
        self.assertEqual(split['crt_matched'], 16)
        self.assertEqual(split['in_scope_matched'], 100)
        self.assertEqual(split['in_scope_total'], TEXT - (0x100 - 0x10) - 0x60 - 4)

    def test_evidenced_region_joins_handwritten_once(self):
        overlapping = region(0x500020, 0x40, inline_asm='evidenced', inline_asm_citation='TibSun lcw.cpp:1')
        separate = region(0x600000, 0x30, inline_asm='evidenced', inline_asm_citation='TibSun conquer.cpp:346')
        split = status.ledger(dict(regions=[overlapping, separate]), TEXT)
        self.assertEqual(split['asm'], 0x60 + 0x20 + 0x30)   # union 0x500000-0x500060, tail, separate
        self.assertEqual(split['in_scope_matched'], 0)

    def test_matched_bytes_inside_a_handwritten_range_are_not_in_scope(self):
        split = status.ledger(dict(regions=[region(0x500000, 0x40, 0)]), TEXT)
        self.assertEqual((split['asm_matched'], split['in_scope_matched']), (0x40, 0))

    def test_regions_without_entry_and_short_text_count_as_in_scope(self):
        split = status.ledger(dict(regions=[dict(body_bytes=100, padding_bytes=4, population='p1')]), 1000)
        self.assertEqual(split, dict(crt=0, crt_matched=0, asm=0, asm_matched=0, in_scope_matched=100,
                                     in_scope_total=996))

    def test_render_keeps_existing_rows_and_adds_the_split(self):
        catalog = dict(regions=[region(0x401000, 100, 4), region(status.CRT_START + 0x20, 16, 0)])
        text = status.render(catalog, TEXT, [])
        lines = text.splitlines()
        self.assertIn('| Matched C/C++ body bytes | 116 ({:.2f}% of .text) |'.format(11600.0 / TEXT), lines)
        self.assertIn('| Frozen padding bytes | 4 |', lines)
        self.assertIn('| Original .text scaffold bytes | {:,} |'.format(TEXT - 120), lines)
        self.assertIn('| C runtime / library bytes (.text >= 0x781000; library ledger, not source credit) '
                      '| 240 ({:.2f}% of .text; 16 matched) |'.format(24000.0 / TEXT), lines)
        total = TEXT - 240 - 0x60 - 4
        self.assertIn('| In-scope matched C/C++ bytes (of .text minus CRT, handwritten assembly and in-scope '
                      'padding) | 100 of {:,} ({:.2f}%) |'.format(total, 10000.0 / total), lines)
        self.assertLess(lines.index('| Frozen padding bytes | 4 |'),
                        [i for i, l in enumerate(lines) if l.startswith('| C runtime')][0])


class HandwrittenListTests(unittest.TestCase):
    def test_ranges_are_ordered_disjoint_and_inside_text(self):
        spans = sorted((a, b) for a, b, _ in status.HANDWRITTEN)
        self.assertTrue(all(a < b for a, b in spans))
        self.assertTrue(all(p[1] <= n[0] for p, n in zip(spans, spans[1:])))
        self.assertTrue(all(status.TEXT_START <= a for a, _ in spans))


if __name__ == '__main__':
    unittest.main()
