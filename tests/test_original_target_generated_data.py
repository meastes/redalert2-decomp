"""Generated data modules: the fastmath generator's arithmetic and the whole-block proof.
Synthetic objects; the expected values below are mathematical facts, not game data."""
import pathlib
import sys
import unittest
from fractions import Fraction

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parents[1] / 'tools' / 'original_target'))
import generate_fastmath as gf
import generated_data as gd


class GeneratorTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.tables = gf.tables()

    def test_counts(self):
        self.assertEqual([len(self.tables[n]) for n, _, _, _ in gf.SYMBOLS], [10244, 4100, 4100, 4098, 16386])

    def test_known_values(self):
        sin = self.tables['sin_tab']
        self.assertEqual(sin[0], 0)
        self.assertEqual(sin[2048], 0x3F800000)          # sin(pi/2) rounds to 1 in extended precision
        self.assertNotEqual(sin[4096], 0)                 # x87 sin(M_PI) is not zero
        self.assertEqual(self.tables['asin_tab'][2048], 0)
        self.assertEqual(self.tables['asin_tab'][0], 0xBFC90FDA)   # -pi/2 chopped
        self.assertEqual(self.tables['sqrt_tab'][0], 0)  # sqrt(1.0) = 1.0
        self.assertEqual(self.tables['atan_tab'][4097], gf.ATAN_STEP_BITS)
        self.assertEqual(sin[10241:], [0, 0, 0])

    def test_chop_is_toward_zero(self):
        third = gf.fixed(Fraction(1, 3))
        self.assertEqual(gf.chop_float_fixed(third), 0x3EAAAAAA)       # nearest would be ...AB
        self.assertEqual(gf.chop_float_fixed(-third), 0xBEAAAAAA)
        self.assertEqual(gf.chop_float_exact(Fraction(1, 3)), 0x3EAAAAAA)
        self.assertEqual(gf.chop_sqrt_exact(Fraction(2)), 0x3FB504F3)

    def test_text_is_deterministic(self):
        self.assertEqual(gf.module_text(), gf.module_text())


class FakeCoff:
    def __init__(self, data, symbols, flags=0xc0300040, relocs=()):
        self.sections = [dict(name='.data', size=len(data), data=data, flags=flags, relocs=list(relocs))]
        self.symbols = {i: dict(name=n, value=v, section=1, storage=2) for i, (n, v) in enumerate(symbols)}


class ProveTest(unittest.TestCase):
    module = dict(name='m', range=['00001000', '00001008'])
    image = bytes(range(8))

    def original(self, va, count):
        return self.image[va - 0x1000:va - 0x1000 + count]

    def test_whole_block(self):
        coff = FakeCoff(self.image, [('a', 0), ('b', 4)])
        self.assertEqual(gd.prove(self.module, coff, {'a': 0x1000, 'b': 0x1004}, self.original)[0], 0x1000)

    def test_unbound_symbol_refuses(self):
        with self.assertRaises(SystemExit):
            gd.prove(self.module, FakeCoff(self.image, [('a', 0), ('b', 4)]), {'a': 0x1000}, self.original)

    def test_disagreeing_bases_refuse(self):
        with self.assertRaises(SystemExit):
            gd.prove(self.module, FakeCoff(self.image, [('a', 0), ('b', 4)]), {'a': 0x1000, 'b': 0x1008}, self.original)

    def test_partial_block_refuses(self):
        with self.assertRaises(SystemExit):
            gd.prove(self.module, FakeCoff(self.image[:4], [('a', 0)]), {'a': 0x1000}, self.original)

    def test_differing_bytes_refuse(self):
        with self.assertRaises(SystemExit):
            gd.prove(self.module, FakeCoff(bytes(8), [('a', 0)]), {'a': 0x1000}, self.original)

    def test_relocations_refuse(self):
        with self.assertRaises(SystemExit):
            gd.prove(self.module, FakeCoff(self.image, [('a', 0)], relocs=[(0, 0, 6)]), {'a': 0x1000}, self.original)


if __name__ == '__main__':
    unittest.main()
