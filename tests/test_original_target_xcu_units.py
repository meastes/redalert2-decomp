"""xcu_units.py: the .CRT$XCU unit-evidence tiers (docs/CRT_XCU_TABLE.md). Synthetic table, no image."""
from pathlib import Path
import sys
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools/original_target'))
import xcu_units

CALL = ('push 0x10', 'call 0x500000', 'ret ')
PLAIN = ('mov dword ptr [D], 0', 'ret ')


def fact(entry, shape=PLAIN):
    return dict(entry=entry, members=[entry], dtors=[], targets=set(), shape=shape)


class XcuUnitsTest(unittest.TestCase):
    def classify(self):
        # units in .text order; each owned initialiser is also a module-map anchor
        code_owner = {0x100: 'a', 0x120: 'a',
                      0x300: 'b', 0x310: 'b', 0x340: 'b',
                      0x500: 'c', 0x520: 'c',                 # c starts with ordinary code
                      0x600: 'd', 0x610: 'd', 0x700: 'e', 0x710: 'e',
                      0x800: 'f', 0x810: 'f',                 # f ends with an initialiser
                      0x900: 'g', 0x910: 'g', 0xA00: 'h'}
        unit_of = {0x660: 'e.cpp', 0x670: 'd.cpp', 0x960: 'x.cpp'}
        facts = [fact(0x100), fact(0x130), fact(0x300), fact(0x320), fact(0x400), fact(0x520),
                 fact(0x600, CALL), fact(0x650, CALL), fact(0x660), fact(0x670), fact(0x700),
                 fact(0x810), fact(0x850), fact(0x900), fact(0x950), fact(0xA00)]
        for i, f in enumerate(facts):
            f['index'] = i
        extremes = xcu_units.classify_entries(facts, code_owner, {}, unit_of, {})
        return facts, extremes

    def test_entries(self):
        facts, extremes = self.classify()
        by = {f['entry']: f for f in facts}
        self.assertEqual(by[0x100]['cls'], 'owned')
        self.assertEqual((by[0x130]['cls'], by[0x130]['pick']), ('two', 'b'))
        self.assertEqual((by[0x320]['cls'], by[0x320]['pick']), ('single:table', 'b'))
        self.assertTrue(by[0x650]['excluded'])
        self.assertFalse(by[0x660]['excluded'])
        self.assertIsNone(by[0x850]['pick'])                     # f ends with an initialiser
        self.assertEqual(by[0x950]['cls'], 'open')                # x's code lies between g and h
        self.assertIn('f', extremes['ends_init'])
        self.assertEqual(extremes['starts_other'], {'c', 'f'})

    def test_files(self):
        facts, extremes = self.classify()
        sources = {'%x.cpp' % e: [dict(entry=e)] for e in (0x130, 0x320, 0x400, 0x650, 0x660, 0x670, 0x850, 0x950)}
        sources['local.cpp'] = [dict(entry=0x999)]
        files = {r['file']: r for r in xcu_units.classify_files(facts, sources, extremes, local_destructors={0x999})}
        tiers = {k: (r['tier'], r['unit']) for k, r in files.items()}
        self.assertEqual(tiers, {
            '130.cpp': ('xcu-prefix', 'b'),
            '320.cpp': ('single', 'b'),
            '400.cpp': ('stay', None),              # prefix rule refused: c does not start with its initialisers
            '650.cpp': ('xcu-exclusion', 'e'),      # d already owns this shape
            '660.cpp': ('xcu-prefix+map', 'e'),
            '670.cpp': ('stay', None),              # the unit map names the other candidate
            '850.cpp': ('stay', None),              # no pick
            '950.cpp': ('group', None),
            'local.cpp': ('stay', None)})
        self.assertIn('function-local static', files['local.cpp']['reason'])
        self.assertEqual(set(xcu_units.FLAG_TIERS) | {'single', 'stay', 'group'}, set(xcu_units.TIERS))


class LiftGroupsTest(unittest.TestCase):
    def test_one_group_one_tier(self):
        records = [dict(file='a', tier='xcu-prefix', unit='u', reason='r'),
                   dict(file='b', tier='xcu-exclusion', unit='u', reason='r'),
                   dict(file='c', tier='xcu-prefix+map', unit='u', reason='r'),
                   dict(file='d', tier='xcu-prefix', unit='v', reason='r'),       # another group
                   dict(file='e', tier='stay', unit=None, reason='r')]
        out = {r['file']: r['tier'] for r in xcu_units.lift_groups(
            records, {'a': 'p1', 'b': 'p1', 'c': 'p1', 'e': 'p1', 'd': 'p2'})}
        self.assertEqual(out, {'a': 'xcu-exclusion', 'b': 'xcu-exclusion', 'c': 'xcu-exclusion',
                               'd': 'xcu-prefix', 'e': 'stay'})


class XcuJoinTest(unittest.TestCase):
    """refile_unit.py join-xcu placement, the unit_evidence flag (canonical) and its STATUS lines."""

    def test_place_xcu(self):
        import refile_unit
        module = ('// unit.cpp: banner\n//\n#include "shared/a.hpp"\n\n'
                  '// Fn (0x00401100)\nvoid Fn() {}\n\n// Gn (0x00401300)\nvoid Gn() {}\n')
        chunks = [(0x401000, '#include "shared/s.hpp"'), (0x401010, '// emitted'),
                  (0x401200, 'int joined;'), (0x401400, '// tail')]
        where = {0x401000: ('prelude', 5), 0x401010: ('prelude', 6), 0x401200: ('address', 9), 0x401400: ('append', 99)}
        text, spans = refile_unit.place_xcu(module, [0x401100, 0x401300], chunks, where)
        lines = text.split('\n')
        self.assertEqual(lines[:4], ['// unit.cpp: banner', '//', '', '#include "shared/s.hpp"'])
        self.assertLess(text.index('// emitted'), text.index('#include "shared/a.hpp"'))
        self.assertLess(text.index('void Fn()'), text.index('int joined;'))
        self.assertLess(text.index('int joined;'), text.index('// Gn (0x00401300)'))
        self.assertTrue(text.rstrip().endswith('// tail'))
        for entry, (first, last) in spans.items():
            self.assertIn(dict(chunks)[entry].split('\n')[0], lines[first - 1])
        # the unit text alone comes back when nothing is placed
        self.assertEqual(refile_unit.place_xcu(module, [0x401100, 0x401300], [], {})[0], module)

    def test_unit_evidence(self):
        import canonical
        row = dict(entry=0x401000, body_bytes=10, unit_evidence='xcu-prefix')
        self.assertEqual(canonical.validate_unit_evidence(row, [row, dict(entry=0x401100)]), 'xcu-prefix')
        self.assertIsNone(canonical.validate_unit_evidence(dict(entry=1), []))
        with self.assertRaises(ValueError):
            canonical.validate_unit_evidence(dict(row, unit_evidence='guess'), [row, row])
        with self.assertRaises(ValueError):
            canonical.validate_unit_evidence(row, [row])        # not in a module source
        counts = canonical.unit_evidence_counts(dict(regions=[row, dict(entry=2, body_bytes=5)]))
        self.assertEqual(counts['xcu-prefix'], (1, 10))
        self.assertEqual(counts['xcu-exclusion'], (0, 0))

    def test_status_lines(self):
        import status
        lines = status.unit_evidence_lines(dict(regions=[dict(entry=1, body_bytes=7, unit_evidence='xcu-exclusion')]))
        self.assertEqual(len(lines), 4)
        self.assertIn('xcu single', lines[0])
        self.assertIn('unit_evidence: xcu-exclusion', lines[1])
        self.assertIn('| 1 rows, 7 bytes |', lines[1])
        self.assertIn('| 0 rows, 0 bytes |', lines[3])


if __name__ == '__main__':
    unittest.main()
