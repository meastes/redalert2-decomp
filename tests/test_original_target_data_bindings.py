"""Data-only bindings (data_bindings.py): enclosed layout, bss elimination and content binding.
Synthetic objects and images only; no game payload."""
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


LITERAL = 0x40301040   # COMDAT initialised data, 4-byte aligned (a ??_C@ string literal)
TABLE = 0x1004          # where the synthetic image holds the pointer table


class ContentTest(unittest.TestCase):
    """data_bound_by: content (review session 2026-11-20): a pointer table bound by its relocated
    bytes, shaped like revent's RadarEventNames (each slot a pointer to a literal of the object)."""

    def setUp(self):
        import tree
        self.tree, self.saved = tree, tree.REBUILD
        tree.REBUILD = True
        self.addCleanup(setattr, tree, 'REBUILD', self.saved)
        # image at 0x1000: a 4-byte item, the table (0x1004: -> 0x100C, -> 0x1010), "AB", "CD", the next item.
        self.data = bytearray(0x200)
        self.data[0:4] = b'\x01\x02\x03\x04'
        self.data[4:12] = (0x100C).to_bytes(4, 'little') + (0x1010).to_bytes(4, 'little')
        self.data[12:15] = b'AB\0'
        self.data[16:19] = b'CD\0'
        self.data[20:24] = b'\x05\x06\x07\x08'

    def coff(self, table_flags=DATA, texts=(b'AB\0', b'CD\0'), relocated=(0, 4), extra_symbol=False, size=8):
        sections = [('.data', size, table_flags)] + [('.rdata', len(t), LITERAL) for t in texts]
        symbols = [('_names', 0, 1, 2)] + [('??_C@_lit%d' % i, 0, i + 2, 2) for i in range(len(texts))]
        if extra_symbol:
            symbols.append(('_other', 4, 1, 2))
        coff = FakeCoff(sections, symbols)
        for i, text in enumerate(texts):
            coff.sections[i + 1]['data'] = text
        coff.sections[0]['relocs'] = [(offset, 1 + n, 6) for n, offset in enumerate(relocated)]
        if not relocated:
            coff.sections[0]['data'] = bytes(self.data[4:4 + size])
        return coff

    def bind(self, coff, references=None):
        data = bytes(self.data)

        def find(pattern, literal=False):
            hits, at = [], data.find(pattern)
            while at >= 0:
                if not literal or at == 0 or data[at - 1] == 0:
                    hits.append(0x1000 + at)
                at = data.find(pattern, at + 1)
            return hits

        def pointed(low, high):
            values = {int.from_bytes(data[i:i + 4], 'little') for i in range(len(data) - 3)}
            return {v for v in values if low <= v < high}
        return db.bind(coff, {'_names': 'content'}, references or {}, image(0x1000, data),
                       below_ends={0x1004}, above_starts={0x1014}, find=find, pointed=pointed)

    def test_pointer_table_binds_with_its_literals(self):
        bound = self.bind(self.coff())
        self.assertEqual(bound['_names'][:2], (TABLE, 'content'))
        self.assertEqual(bound['??_C@_lit0'][0], 0x100C)
        self.assertEqual(bound['??_C@_lit1'][0], 0x1010)

    def test_main_tree_refuses(self):
        self.tree.REBUILD = False
        with self.assertRaisesRegex(db.BindingRefused, 'rebuild tree'):
            self.bind(self.coff())

    def test_altered_literal_refuses(self):
        with self.assertRaisesRegex(db.BindingRefused, 'not in the image'):
            self.bind(self.coff(texts=(b'AX\0', b'CD\0')))

    def test_literal_hit_must_begin_a_literal(self):
        self.data[11] = ord('x')     # "AB" now continues a longer string: no literal begins there
        with self.assertRaisesRegex(db.BindingRefused, 'not in the image'):
            self.bind(self.coff())

    def test_table_bytes_twice_refuse(self):
        self.data[0x100:0x108] = self.data[4:12]
        with self.assertRaisesRegex(db.BindingRefused, 'occur 2 times'):
            self.bind(self.coff())

    def test_pointer_into_the_range_refuses(self):
        self.data[0x180:0x184] = (TABLE + 4).to_bytes(4, 'little')
        with self.assertRaisesRegex(db.BindingRefused, 'points into'):
            self.bind(self.coff())

    def test_bss_symbol_refuses(self):
        with self.assertRaisesRegex(db.BindingRefused, 'not in an initialised data section'):
            self.bind(self.coff(table_flags=BSS))

    def test_relocation_free_object_refuses(self):
        with self.assertRaisesRegex(db.BindingRefused, 'no relocation'):
            self.bind(self.coff(texts=(), relocated=()))

    def test_slot_without_relocation_refuses(self):
        coff = self.coff(texts=(b'AB\0',), relocated=(0,))
        coff.sections[0]['data'] = bytes(4) + bytes(self.data[8:12])
        with self.assertRaisesRegex(db.BindingRefused, 'array of pointers'):
            self.bind(coff)

    def test_two_symbols_in_the_section_refuse(self):
        with self.assertRaisesRegex(db.BindingRefused, 'only symbol'):
            self.bind(self.coff(extra_symbol=True))

    def test_declared_content_kind_accepted(self):
        self.assertEqual(db.declared([dict(data_bound_by={'_a': 'content'}, data_bound_by_note='n')]), {'_a': 'content'})


if __name__ == '__main__':
    unittest.main()
