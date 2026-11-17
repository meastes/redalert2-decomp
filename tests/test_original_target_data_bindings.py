"""Data-only bindings (data_bindings.py): enclosed layout and bss elimination. Synthetic
objects and images only; no game payload."""
import pathlib
import sys
import unittest

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parents[1] / 'tools' / 'original_target'))
import data_bindings as db

DATA = 0xc0300040      # initialised data, 4-byte aligned
BSS = 0xc0300080       # zero-filled, 4-byte aligned


class FakeCoff:
    def __init__(self, sections, symbols):
        self.sections = [dict(name=n, size=s, flags=f, data=bytes(s), relocs=[]) for n, s, f in sections]
        self.symbols = {i: dict(name=n, value=v, section=sec, storage=st)
                        for i, (n, v, sec, st) in enumerate(symbols)}


def image(base, data):
    def original(va, count):
        offset = va - base
        if offset < 0 or offset + count > len(data):
            raise ValueError('outside')
        return data[offset:offset + count]
    return original


class LayoutTest(unittest.TestCase):
    def setUp(self):
        # .data: a (16) b (16) table (8); only the table is bound, at 0x1020.
        self.coff = FakeCoff([('.data', 40, DATA)],
                             [('_a', 0, 1, 3), ('_b', 16, 1, 3), ('_table', 32, 1, 3)])
        self.refs = {'_table': 0x1020}
        self.original = image(0x0f00, bytes(0x400))

    def test_enclosed_binds_each_symbol_by_offset(self):
        bound = db.bind(self.coff, {'_a': 'layout', '_b': 'layout'}, self.refs, self.original,
                        below_ends={0x0ffe}, above_starts={0x1028})
        self.assertEqual(bound['_a'][0], 0x1000)
        self.assertEqual(bound['_b'][0], 0x1010)

    def test_open_lower_edge_refuses(self):
        with self.assertRaisesRegex(db.BindingRefused, 'open lower edge'):
            db.bind(self.coff, {'_a': 'layout'}, self.refs, self.original, below_ends=set(), above_starts={0x1028})

    def test_lower_anchor_too_far_refuses(self):
        with self.assertRaisesRegex(db.BindingRefused, 'not zero fill'):
            db.bind(self.coff, {'_a': 'layout'}, self.refs, self.original, below_ends={0x0ff0}, above_starts={0x1028})

    def test_open_upper_edge_refuses(self):
        with self.assertRaisesRegex(db.BindingRefused, 'open upper edge'):
            db.bind(self.coff, {'_a': 'layout'}, self.refs, self.original, below_ends={0x1000}, above_starts={0x0f00})

    def test_nonzero_fill_refuses(self):
        data = bytearray(0x400); data[0x1029 - 0x0f00] = 1
        with self.assertRaisesRegex(db.BindingRefused, 'not zero fill'):
            db.bind(self.coff, {'_a': 'layout'}, self.refs, image(0x0f00, bytes(data)),
                    below_ends={0x1000}, above_starts={0x102c})

    def test_no_anchor_inside_refuses(self):
        with self.assertRaisesRegex(db.BindingRefused, 'no symbol of the section is bound'):
            db.bind(self.coff, {'_a': 'layout'}, {}, self.original, below_ends={0x1000}, above_starts={0x1028})

    def test_disagreeing_anchors_refuse(self):
        with self.assertRaisesRegex(db.BindingRefused, 'disagree'):
            db.bind(self.coff, {'_a': 'layout'}, {'_table': 0x1020, '_b': 0x2000}, self.original,
                    below_ends={0x1000}, above_starts={0x1028})


class EliminationTest(unittest.TestCase):
    def coff(self, order):
        offsets, symbols, at = {}, [], 0
        sizes = dict(_x=8, _y=4, _z=16)
        for name in order:
            symbols.append((name, at, 1, 3)); at += sizes[name]
        return FakeCoff([('.bss', at, BSS)], symbols)

    def test_only_hole_binds(self):
        # original: x at 0x2000 (8), y at 0x2008 (4), hole 0x200c-0x201c, next unit at 0x201c
        refs = {'_x': 0x2000, '_y': 0x2008, '_next': 0x201c}
        bound = db.bind(self.coff(['_x', '_y', '_z']), {'_z': 'elimination'}, refs, None, set(), set())
        self.assertEqual(bound['_z'][0], 0x200c)

    def test_hole_of_wrong_size_refuses(self):
        refs = {'_x': 0x2000, '_y': 0x2008, '_next': 0x2020}
        with self.assertRaisesRegex(db.BindingRefused, 'does not fit'):
            db.bind(self.coff(['_x', '_y', '_z']), {'_z': 'elimination'}, refs, None, set(), set())

    def test_two_holes_refuse(self):
        refs = {'_x': 0x2000, '_y': 0x2010, '_next': 0x2024}
        with self.assertRaisesRegex(db.BindingRefused, 'free holes'):
            db.bind(self.coff(['_x', '_y', '_z']), {'_z': 'elimination'}, refs, None, set(), set())

    def test_alignment_fill_is_not_a_hole(self):
        # y (4) then x (8, 8-aligned): 4 bytes fill between, hole after x
        refs = {'_y': 0x2000, '_x': 0x2008, '_next': 0x2020}
        bound = db.bind(self.coff(['_y', '_x', '_z']), {'_z': 'elimination'}, refs, None, set(), set())
        self.assertEqual(bound['_z'][0], 0x2010)

    def test_another_unbound_symbol_refuses(self):
        refs = {'_x': 0x2000, '_next': 0x201c}
        with self.assertRaisesRegex(db.BindingRefused, 'other unbound'):
            db.bind(self.coff(['_x', '_y', '_z']), {'_z': 'elimination'}, refs, None, set(), set())

    def test_object_layout_must_agree(self):
        # the hole is last in the span, but the object puts _z first
        refs = {'_x': 0x2000, '_y': 0x2008, '_next': 0x201c}
        with self.assertRaisesRegex(db.BindingRefused, 'does not put it'):
            db.bind(self.coff(['_z', '_x', '_y']), {'_z': 'elimination'}, refs, None, set(), set())

    def test_data_section_refuses(self):
        coff = FakeCoff([('.data', 8, DATA)], [('_z', 0, 1, 3)])
        with self.assertRaisesRegex(db.BindingRefused, 'zero-filled'):
            db.bind(coff, {'_z': 'elimination'}, {}, None, set(), set())


class DeclarationTest(unittest.TestCase):
    def test_note_required(self):
        with self.assertRaisesRegex(db.BindingRefused, 'note'):
            db.declared([dict(data_bound_by={'_a': 'layout'})])

    def test_kind_checked(self):
        with self.assertRaisesRegex(db.BindingRefused, 'layout'):
            db.declared([dict(data_bound_by={'_a': 'guess'}, data_bound_by_note='n')])

    def test_rows_agree(self):
        rows = [dict(data_bound_by={'_a': 'layout'}, data_bound_by_note='n'),
                dict(data_bound_by={'_a': 'elimination'}, data_bound_by_note='n')]
        with self.assertRaisesRegex(db.BindingRefused, 'differs'):
            db.declared(rows)


if __name__ == '__main__':
    unittest.main()
