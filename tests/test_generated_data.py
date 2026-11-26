"""Tests for the generated data modules (tools/original_target/generated_data.py, generate_fastmath.py; review-session
ruling 2026-11-26, lane O104): a generator computes every entry from its formula and carries no table value; every
constant it reads from the image is flagged in the manifest; the proof needs every name bound.

Run: python -m unittest discover -s tests"""
import json
import re
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'tools' / 'original_target'))
import generated_data as G     # noqa: E402


class ManifestTest(unittest.TestCase):
    def test_manifest_loads_and_names_seven_points(self):
        doc = G.load()
        module = next(m for m in doc['modules'] if m['name'] == 'fastmath_tables')
        self.assertEqual(len(module['symbols']) + len(module['inner_points']), 7)
        self.assertEqual(module['range'], ['00801dc4', '00827e04'])
        self.assertEqual([c['name'] for c in module['flagged_constants']], ['ATAN_STEP_BITS'])


class GeneratorTest(unittest.TestCase):
    def test_flagged_constants_match_the_manifest(self):
        import generate_fastmath as F
        module = next(m for m in G.load()['modules'] if m['name'] == 'fastmath_tables')
        self.assertEqual(sorted(F.FLAGGED_CONSTANTS), sorted(c['name'] for c in module['flagged_constants']))
        self.assertEqual(F.ATAN_STEP_BITS, int(module['flagged_constants'][0]['value'], 16))

    def test_generator_carries_no_table_values(self):
        # No wide literal but the flagged constant and the float format's masks, the block's start (--check) and
        # two cited code addresses (values, differences or exceptions would need some).
        text = (ROOT / 'tools/original_target/generate_fastmath.py').read_text()
        literals = {m.lower() for m in re.findall(r'0x[0-9A-Fa-f]{6,}', text)}
        self.assertEqual(literals, {'0x3cc7fe84', '0x7fffff', '0x80000000', '0x801dc4', '0x004bab30', '0x004ba6f0'})
        self.assertNotRegex(text, r'\b0[0-9A-Fa-f]{7}h\b')        # no MASM-style dword literal either

    def test_table_sizes_and_derived_pi(self):
        import generate_fastmath as F
        from fractions import Fraction
        # M_PI's bits (0x400921FB54442D18) and the x87's documented 66-bit pi, both derived from the computed pi.
        self.assertEqual(F.PI_DOUBLE, Fraction(0x1921FB54442D18, 1 << 51))
        self.assertEqual(F.PI_X87, Fraction(0xC90FDAA22168C234C >> 2, 1 << 64))
        total = sum(count + zeros + (1 if label == 'atan_tab' else 0) for label, _, count, zeros in F.SYMBOLS)
        self.assertEqual(4 * total, 0x827E04 - 0x801DC4)


class ProveTest(unittest.TestCase):
    class FakeCoff:
        def __init__(self, data, names):
            self.sections = [dict(name='.data', size=len(data), flags=0xC0000040, relocs=[], data=data)]
            self.symbols = {i: dict(name=n, section=1, storage=2, value=v) for i, (n, v) in enumerate(names)}

    def test_inner_point_must_be_bound(self):
        module = dict(name='m', range=['00001000', '00001008'], symbols=['_a'],
                      inner_points=[dict(name='a[1]', symbol='_a', offset=4)])
        coff = self.FakeCoff(b'\1' * 8, [('_a', 0)])
        original = lambda start, n: b'\1' * n
        with self.assertRaises(SystemExit):
            G.prove(module, coff, {'_a': 0x1000}, original, {('_a', 4): False})
        self.assertEqual(G.prove(module, coff, {'_a': 0x1000}, original, {('_a', 4): True}), (0x1000, b'\1' * 8))

    def test_public_symbols_must_equal_the_manifest(self):
        module = dict(name='m', range=['00001000', '00001008'], symbols=['_a', '_b'], inner_points=[])
        coff = self.FakeCoff(b'\1' * 8, [('_a', 0)])
        with self.assertRaises(SystemExit):
            G.prove(module, coff, {'_a': 0x1000}, lambda s, n: b'\1' * n, {})


if __name__ == '__main__':
    unittest.main()
