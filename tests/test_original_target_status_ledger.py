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



class HandwrittenListTests(unittest.TestCase):
    def test_ranges_are_ordered_disjoint_and_inside_text(self):
        spans = sorted((a, b) for a, b, _ in status.HANDWRITTEN)
        self.assertTrue(all(a < b for a, b in spans))
        self.assertTrue(all(p[1] <= n[0] for p, n in zip(spans, spans[1:])))
        self.assertTrue(all(status.TEXT_START <= a for a, _ in spans))



class OwnBoundTests(unittest.TestCase):


    def test_declarations_only_rows_are_judgment_calls(self):
        catalog = dict(regions=[region(0x401000, 16, declarations_only=True), region(0x401010, 8)])
        self.assertIn('declarations_only', status.JUDGMENT_FLAGS)
        self.assertEqual(status.judgment_coverage(catalog), (1, 16))

if __name__ == '__main__':
    unittest.main()


class FunctionStatsTest(unittest.TestCase):
    def test_counts_and_partial_average(self):
        import status
        treemap = [dict(entry=0x401000, bytes=100, status='exact'),
                   dict(entry=0x402000, bytes=100, status='draft'),
                   dict(entry=0x403000, bytes=100, status='remaining'),
                   dict(entry=0x404001, bytes=5, status='remaining'),      # false row
                   dict(entry=0x534D40, bytes=407, status='remaining'),    # handwritten range
                   dict(entry=0x781100, bytes=50, status='remaining')]     # CRT
        out = status.functions(treemap, {0x404001}, {0x402000: 90.0})
        self.assertEqual((out['total'], out['done'], out['open'], out['scored']), (3, 1, 2, 1))
        self.assertAlmostEqual(out['fuzzy_bytes'], 100.0 * (100 + 90) / 300)
        self.assertAlmostEqual(out['fuzzy_functions'], (100 + 90 + 0) / 3)


class StructureLinesTests(unittest.TestCase):
    RESULT = dict(
        classes=dict(defined=10, multiply_defined=2, extra_definitions=3, top=[]),
        switches=dict(count=4, files=5, guard_tests=0, by_switch={}),
        files_outside=dict(unit_files=6, outside=7, one_function=5, one_function_folders=1,
                           one_function_top_folders=1, several_functions=1, no_row=1, top_folders=[]),
        member_counterparts=dict(rows=8, bytes=900, free=5, stand_in=3, by_reference={},
                                 references={'mischa85': True, 'tibsun': False}, list=[]),
        address_suffixed=dict(identifiers=11, occurrences=1200, files=13, eight_digit=5, six_digit=6),
        ref_lines=dict(lines=0, files=0),
        externs=dict(lines=14, names=15, files=16, declared_in_a_header=1, in_several_cpp=2))


    def test_no_structure_rows_without_a_census(self):
        catalog = dict(regions=[region(0x401000, 100, 4)])
        self.assertNotIn('| Structure:', status.render(catalog, TEXT, []))


class RebuildCategoryLineTests(unittest.TestCase):
    """Both trees on the same category lines (lane O41, 2026-11-22): Reproduced .text is the sum of C/C++ body,
    evidenced inline asm, frozen padding, linked library and assembled asm in both; C/C++ is body bytes only; the
    rebuild's whole-unit categories come from its catalog, its function-level ones and the 'exact with no
    judgment call' row (lane O37) from the last row of manifests/progress.csv. Since 2026-11-24 the rebuild
    (tree of record) is the first column, whole-unit as the headline, and src-final the frozen reference."""
    BASE = dict(time_utc='2026-11-22T20:00Z', units='2', functions_exact='10', bytes_exact='1000', a_functions='8',
                a_bytes='800', b_functions='1', b_bytes='100', c_functions='1', c_bytes='100', workers_functions='0',
                workers_bytes='0', source_share_of_text='5.000%', reproduced_text='6.000%', duplicates='0',
                externs_landed='0', externs_all='0', address_suffixed='0', parked_functions='3', parked_bytes='300',
                parked_large_functions='0', parked_large_bytes='0', judgment_rows='2', judgment_bytes='200',
                count_sensitive_rows='1', count_sensitive_bytes='64', function_level_functions='12',
                function_level_source_share_of_text='5.500%', function_level_reproduced_text='6.500%',
                tree='rebuild-trial')
    BODY = dict(c_body_bytes='80', padding_bytes='10', library_bytes='5', assembly_bytes='3',
                function_level_c_body_bytes='90', function_level_padding_bytes='12')
    CLEAN = dict(clean_functions='8', clean_source_share_of_text='4.000%', clean_function_level_functions='9',
                 clean_function_level_source_share_of_text='4.250%', judgment_any_rows='3', judgment_any_bytes='1234',
                 header_dependent_rows='1', header_dependent_bytes='34')
    MAIN = dict(regions=5, body=100, padding=20, evidenced_inline_asm=2)
    CAT = dict(regions=10, body=80, padding=10, evidenced_inline_asm=4, inline_asm_rows=1)

    def lines(self, row):
        import csv, tempfile
        with tempfile.TemporaryDirectory() as d:
            path = Path(d) / 'manifests/progress.csv'
            path.parent.mkdir(parents=True)
            with path.open('w', newline='') as f:
                w = csv.DictWriter(f, fieldnames=list(row)); w.writeheader(); w.writerow(row)
            return status.rebuild_lines(1000, self.MAIN, 5, 3, (2, 30), 70, repository=d, categories=self.CAT)

    def line(self, lines, start):
        return [l for l in lines if l.startswith(start)][0]

    def test_reproduced_is_the_sum_of_the_same_categories_in_both_trees(self):
        lines = self.lines(dict(self.BASE, **self.BODY))
        self.assertEqual(self.line(lines, '| Reproduced .text'),
                         '| Reproduced .text (both trees: C/C++ body + evidenced inline asm + frozen padding + linked '
                         'library members + assembled handwritten asm, the six lines below) '
                         '| **10.20% whole-unit** (102 B) (11.40% function-level, 114 B) | 13.00% (130 B) |')

    def test_revised_assembly_has_its_own_line_and_no_cpp_credit(self):
        """Revised, binary-derived lines (user decision 2026-11-24): split off the assembled line, inside Reproduced
        only; the C/C++ line does not move."""
        import csv, tempfile
        row = dict(self.BASE, **self.BODY)
        with tempfile.TemporaryDirectory() as d:
            path = Path(d) / 'manifests/progress.csv'
            path.parent.mkdir(parents=True)
            with path.open('w', newline='') as f:
                w = csv.DictWriter(f, fieldnames=list(row)); w.writeheader(); w.writerow(row)
            lines = status.rebuild_lines(1000, self.MAIN, 5, 3, (2, 30), 70, repository=d, categories=self.CAT,
                                         revised_bytes=2)
        self.assertTrue(self.line(lines, '| Assembled handwritten assembly (the same').endswith('| 1 B | 1 B |'))
        self.assertTrue(self.line(lines, '| Assembled handwritten assembly, revised').endswith('| 2 B | 2 B |'))
        self.assertIn('| 13.00% (130 B) |', self.line(lines, '| Reproduced .text'))
        self.assertEqual(self.line(lines, '| C/C++ bodies matched'), self.line(self.lines(row), '| C/C++ bodies matched'))

    def test_cpp_line_is_body_bytes_with_the_gap_to_main(self):
        lines = self.lines(dict(self.BASE, **self.BODY))
        self.assertEqual(self.line(lines, '| C/C++ bodies matched'),
                         '| C/C++ bodies matched (body bytes: no padding, no evidenced inline asm; the parity line) '
                         '| **80 B, 8.00% whole-unit** (90 B, 9.00% function-level); gap to src-final '
                         '0.0 KB (0.0 KB function-level) | 100 B, 10.00% |')
        self.assertEqual(self.line(lines, '| Frozen padding'), '| Frozen padding of accepted rows | 10 B '
                         'whole-unit (12 B function-level) | 20 B |')

    def test_clean_row_needs_the_body_only_columns(self):
        lines = self.lines(dict(self.BASE, **dict(self.BODY, **self.CLEAN)))
        self.assertEqual(self.line(lines, '| C/C++ bodies, exact with no judgment call'),
                         '| C/C++ bodies, exact with no judgment call (each tree\'s judgment rows left out) '
                         '| **4.000% whole-unit** (4.250% function-level) of .text; left out: judgment calls '
                         '3 rows / 1,234 B (of which header-form dependents 1 / 34 B), count-sensitive 1 rows / 64 B '
                         '| 70 B, 7.00% |')
        # A row from before the series break (source shares with padding) shows no rebuild clean figure.
        lines = self.lines(dict(self.BASE, **self.CLEAN))
        self.assertTrue(self.line(lines, '| C/C++ bodies, exact with no judgment call').endswith(
            '| from the next gated landing | 70 B, 7.00% |'))
        self.assertIn('function-level from the next gated landing', self.line(lines, '| C/C++ bodies matched'))

    def test_clean_row_shows_body_bytes_when_the_row_has_them(self):
        row = dict(self.BASE, **dict(self.BODY, **dict(self.CLEAN, clean_c_body_bytes='40',
                                                         clean_function_level_c_body_bytes='42')))
        self.assertIn('**40 B, 4.000% whole-unit** (42 B, 4.250% function-level) of .text; left out',
                      self.line(self.lines(row), '| C/C++ bodies, exact with no judgment call'))

    def test_soft_judgment_and_kind_rows(self):
        """Lane O63 (review session request, 2026-11-23): exact with no soft judgment, and judgment by kind."""
        row = dict(self.BASE, **dict(self.BODY, **self.CLEAN))
        self.assertTrue(self.line(self.lines(row), '| C/C++ bodies, exact with no soft judgment').endswith(
            '| from the next gated landing | 70 B, 7.00% |'))
        row.update(judgment_by_kind='iv-a 2/1000; vii 1/234', soft_judgment_rows='1', soft_judgment_bytes='234',
                   soft_free_c_body_bytes='60', soft_free_source_share_of_text='6.000%',
                   soft_free_function_level_c_body_bytes='65', soft_free_function_level_source_share_of_text='6.500%')
        lines = self.lines(row)
        self.assertIn('| **60 B, 6.000% whole-unit** (65 B, 6.500% function-level); left out: soft judgment rows 1 / 234 B',
                      self.line(lines, '| C/C++ bodies, exact with no soft judgment'))
        self.assertTrue(self.line(lines, '| Rebuild judgment calls by kind').endswith('| iv-a 2/1000; vii 1/234 | |'))

    def test_flagged_note_reads_past_a_kind_prefix(self):
        self.assertTrue(status.FLAGGED_NOTE.match('[iv-b] count_calibrated: a looked-up local'))
        self.assertFalse(status.FLAGGED_NOTE.match('[iv-a] twin statement at the same place'))

    def test_judgment_line_names_both_definitions(self):
        line = self.line(self.lines(dict(self.BASE, **dict(self.BODY, **self.CLEAN))), '| Judgment calls, each')
        self.assertIn('not comparable', line)
        self.assertTrue(line.endswith('| 3 rows / 1,234 B | 2 rows / 30 B |'))

    def test_one_definition_judgment_line(self):
        """Lane O46 (owner ruling 2026-11-22): one line with main's flag definition in both trees."""
        row = dict(self.BASE, **dict(self.BODY, **self.CLEAN))
        line = self.line(self.lines(row), '| Judgment calls, one definition')
        self.assertTrue(line.endswith('| n/a | 2 rows / 30 B |'))
        saved = self.CAT
        try:
            self.CAT = dict(saved, flagged_judgment=(1, 4617))
            line = self.line(self.lines(row), '| Judgment calls, one definition')
        finally:
            self.CAT = saved
        self.assertTrue(line.endswith('| 1 rows / 4,617 B | 2 rows / 30 B |'))

    def test_main_only_line(self):
        """Lane O50 (owner, 2026-11-22): main's accepted entries with no rebuild catalog row, until zero."""
        main = dict(regions=[dict(entry=0x401000, body_bytes=100), dict(entry=0x402000, body_bytes=40),
                             dict(entry=0x403000, body_bytes=7)])
        rebuild = dict(regions=[dict(entry=0x402000, body_bytes=40), dict(entry=0x404000, body_bytes=9)])
        self.assertEqual(status.main_only(main, rebuild), (2, 107))
        row = dict(self.BASE, **dict(self.BODY, **self.CLEAN))
        self.assertTrue(self.line(self.lines(row), '| Matched in src-final (main)').endswith('| n/a | |'))
        saved = self.CAT
        try:
            self.CAT = dict(saved, main_only=(387, 171314))
            line = self.line(self.lines(row), '| Matched in src-final (main)')
        finally:
            self.CAT = saved
        self.assertTrue(line.endswith('| 387 functions / 171,314 B | |'))

    def test_rebuild_flagged_judgment_reads_named_flags_only(self):
        import csv, tempfile
        catalog = dict(regions=[dict(entry=0x401000, body_bytes=100), dict(entry=0x402000, body_bytes=40),
                                dict(entry=0x403000, body_bytes=7), dict(entry=0x404000, body_bytes=9)])
        with tempfile.TemporaryDirectory() as d:
            manifest = Path(d) / 'manifests'
            manifest.mkdir(parents=True)
            with (manifest / 'provenance.csv').open('w', newline='') as f:
                w = csv.DictWriter(f, fieldnames=['unit', 'entry', 'judgment']); w.writeheader()
                w.writerow(dict(unit='a', entry='00401000', judgment='count_calibrated: a looked-up local'))
                w.writerow(dict(unit='a', entry='00402000', judgment='twin statement at the same place'))
                w.writerow(dict(unit='a', entry='00409000', judgment='count_calibrated: not in the catalog'))
            with (manifest / 'header_judgments.csv').open('w', newline='') as f:
                w = csv.DictWriter(f, fieldnames=['header', 'member', 'judgment', 'dependents']); w.writeheader()
                w.writerow(dict(header='a.h', member='A', judgment='split_declaration: b.h keeps a second A',
                                dependents='a 00403000; b 0x401000'))
                w.writerow(dict(header='c.h', member='C', judgment='ra2-decomp form', dependents='c 00404000'))
            self.assertEqual(status.rebuild_flagged_judgment(catalog, d), (2, 107))


class AssemblyLedgerAuditTests(unittest.TestCase):
    """The strict audit of the assembly ledger's revised, binary-derived category (user decision 2026-11-24)."""
    def setUp(self):
        import assembly_tools as AT
        self.AT = AT
        self.source = next(iter(AT.REVISED_ALLOWLIST))
        self.symbol = next(iter(AT.REVISED_ALLOWLIST[self.source]))
        self.parent = AT.REVISED_ALLOWLIST[self.source][self.symbol]

    def doc(self, rows, revised=True):
        import tempfile
        self.tmp = tempfile.TemporaryDirectory()
        root = Path(self.tmp.name)
        for source in self.AT.REVISED_ALLOWLIST:
            (root / source).parent.mkdir(parents=True, exist_ok=True)
            (root / source).write_bytes(b'; ' + b'\n; '.join(self.AT.REVISED_NOTICES) + b'\n')
        doc = dict(sources=['src/recovered/asm/a.asm'], regions=rows,
                   revised=dict(sources=sorted(self.AT.REVISED_ALLOWLIST)) if revised else {})
        return doc, root

    def tearDown(self):
        if hasattr(self, 'tmp'):
            self.tmp.cleanup()

    def test_flagged_allowlisted_row_passes(self):
        doc, root = self.doc([dict(source='src/recovered/asm/a.asm', symbol='_A'),
                              dict(source=self.source, symbol=self.symbol, revised='binary-derived',
                                   archive_parent=self.parent)])
        self.AT.validate_manifest(doc, root)

    def test_refusals(self):
        bad = [
            [dict(source=self.source, symbol=self.symbol)],                                     # no flag
            [dict(source=self.source, symbol='_Draw_Voxel_Regular_ASM', revised='binary-derived',
                  archive_parent='x')],                                                         # not allowlisted
            [dict(source='src/recovered/asm/a.asm', symbol='_A', revised='binary-derived')],     # flag on archive
            [dict(source='src/recovered/asm/a.asm', symbol=self.symbol),
             dict(source=self.source, symbol=self.symbol, revised='binary-derived', archive_parent=self.parent)],
        ]
        for rows in bad:
            doc, root = self.doc(rows)
            with self.assertRaises(SystemExit):
                self.AT.validate_manifest(doc, root)

    def test_notices_required(self):
        doc, root = self.doc([])
        (root / self.source).write_bytes(b'; no notices\n')
        with self.assertRaises(SystemExit):
            self.AT.validate_manifest(doc, root)
