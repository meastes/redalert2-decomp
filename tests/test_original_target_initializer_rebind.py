"""Synthetic `_$E<n>` re-binding contracts; fixtures contain no game payload."""
from pathlib import Path
import json
import struct
import sys
import unittest

REPOSITORY = next(parent for parent in Path(__file__).resolve().parents
                  if (parent / 'tools/original_target/initializer_rebind.py').is_file())
sys.path.insert(0, str(REPOSITORY / 'tools/original_target'))
import apply_rename
import initializer_rebind as rebind
from compile_candidates import compare_candidate


class FakeCoff:
    """The Coff interface the re-binding and compare_candidate read: one COMDAT
    section per function, relocations (offset, symbol index, kind)."""
    def __init__(self, functions):
        self.sections, self.symbols, names = [], {}, {}
        def index(name):
            if name not in names:
                names[name] = len(self.symbols)
                self.symbols[names[name]] = dict(name=name, value=0, section=0, type=0, storage=2, size=0)
            return names[name]
        for name, data, relocs in functions:
            self.sections.append(dict(name='.text', data=bytes(data), flags=0x60101020, size=len(data),
                                      relocs=[(o, index(target), kind) for o, target, kind in relocs]))
            symbol = self.symbols[index(name)]
            symbol.update(section=len(self.sections), type=0x20, storage=3, size=len(data))

    def function(self, name):
        found = [s for s in self.symbols.values() if s['name'] == name and s['section']]
        if len(found) != 1:
            raise ValueError('Function symbol ambiguous/missing: ' + name)
        section = self.sections[found[0]['section'] - 1]
        return found[0], section, 0, len(section['data'])


INIT_AT, DTOR_AT, OBJECT_AT, ATEXIT_AT, OTHER_AT = 0x401000, 0x401040, 0x500000, 0x402000, 0x401080


def initializer(object_name='_Object', destructor='_$E41'):
    """mov [Object],0; push offset destructor; call _atexit; pop ecx; ret"""
    data = bytearray(b'\xc7\x05' + bytes(4) + bytes(4) + b'\x68' + bytes(4) + b'\xe8' + bytes(4) + b'\x59\xc3')
    return data, [(2, object_name, 6), (11, destructor, 6), (16, '_atexit', 20)]


def destructor(object_name='_Object'):
    """mov ecx, offset Object; jmp ??1Object"""
    data = bytearray(b'\xb9' + bytes(4) + b'\xe9' + bytes(4))
    return data, [(1, object_name, 6), (6, '??1Thing@@QAE@XZ', 20)]


def other(object_name='_Other'):
    data = bytearray(b'\xc7\x05' + bytes(4) + b'\x01\x00\x00\x00\xc3')
    return data, [(2, object_name, 6)]


def link(data, relocs, address, table):
    """The original bytes: `data` with its relocations resolved through `table`."""
    out = bytearray(data)
    for offset, name, kind in relocs:
        value = table[name] + struct.unpack_from('<I', out, offset)[0]
        if kind == 20:
            value -= address + offset + 4
        struct.pack_into('<I', out, offset, value & 0xffffffff)
    return bytes(out)


TABLE = {'_Object': OBJECT_AT, '_Other': OBJECT_AT + 8, '_atexit': ATEXIT_AT, '??1Thing@@QAE@XZ': 0x403000,
         '_$E41': DTOR_AT}


def original_image(pieces):
    image = {}
    for address, (data, relocs) in pieces.items():
        for i, b in enumerate(link(data, relocs, address, dict(TABLE, **{'_$E41': DTOR_AT}))):
            image[address + i] = b
    def original(address, count):
        if any(address + i not in image for i in range(count)):
            raise ValueError('Original range outside image sections')
        return bytes(image[address + i] for i in range(count))
    return original


def receipt(relocs, address, table):
    out = []
    for offset, name, kind in relocs:
        out.append(dict(offset=offset, kind=kind, symbol=name, target=table[name], addend=0))
    return out


def catalog_rows():
    """Accepted rows as the catalog holds them: initializer _$E43 at INIT_AT pushing
    its destructor _$E41 at DTOR_AT, and _$E44 (another initializer) at OTHER_AT."""
    table = dict(TABLE)
    init, dtor, oth = initializer(), destructor(), other()
    rows = [dict(entry=INIT_AT, symbol='_$E43', symbol_prefix='_$E43', body_bytes=len(init[0]),
                 full_extent_bytes=len(init[0]), population='population-1', references=table,
                 relocations=receipt(init[1], INIT_AT, table), source='src/recovered/a.cpp', options=[]),
            dict(entry=DTOR_AT, symbol='_$E41', symbol_prefix='_$E41', body_bytes=len(dtor[0]),
                 full_extent_bytes=len(dtor[0]), population='population-1', references=table,
                 relocations=receipt(dtor[1], DTOR_AT, table), source='src/recovered/b.cpp', options=[]),
            dict(entry=OTHER_AT, symbol='_$E44', symbol_prefix='_$E44', body_bytes=len(oth[0]),
                 full_extent_bytes=len(oth[0]), population='population-1', references=table,
                 relocations=receipt(oth[1], OTHER_AT, table), source='src/recovered/c.cpp', options=[])]
    return rows, original_image({INIT_AT: init, DTOR_AT: dtor, OTHER_AT: oth})


def fresh_object(init_name, dtor_name, other_name, other_value=1, decoy=None):
    init = initializer(destructor=dtor_name)
    oth = other()
    if other_value != 1:
        oth[0][6] = other_value
    functions = [(dtor_name, *destructor()), (init_name, *init), (other_name, *oth)]
    if decoy:
        functions.append((decoy, *other('_Decoy')))
    return FakeCoff(functions)


class Audit:
    def __init__(self, original):
        self.original = original


class InitializerRebindTests(unittest.TestCase):
    def compare(self, rows, coff, original):
        results = []
        for row in rows:
            candidate = dict(row, method='%08x' % row['entry'], region_bytes=row['full_extent_bytes'],
                             source_sha256='')
            results.append(compare_candidate(Audit(original), coff, candidate, Path(__file__), [], 0))
        return results

    def test_unchanged_numbers_bind_to_themselves(self):
        rows, original = catalog_rows()
        coff = fresh_object('_$E43', '_$E41', '_$E44')
        self.assertEqual(rebind.group_mapping(coff, rows, rows[0]['references'], original), {})
        self.assertEqual([r['status'] for r in self.compare(rows, coff, original)], ['extent-exact'] * 3)

    def test_renumbered_initializers_rebind_and_verify_exactly(self):
        rows, original = catalog_rows()
        # Two more declarations before the unit's statics: every number moves by 2,
        # and the old _$E43 now names the destructor's old neighbour.
        coff = fresh_object('_$E45', '_$E43', '_$E46')
        before = self.compare(rows, coff, original)
        self.assertNotEqual([r['status'] for r in before], ['extent-exact'] * 3)
        mapping = rebind.group_mapping(coff, rows, rows[0]['references'], original)
        self.assertEqual(mapping, {'_$E41': '_$E43', '_$E43': '_$E45', '_$E44': '_$E46'})
        rebind.rebind_rows(rows, mapping)
        self.assertEqual([r['symbol_prefix'] for r in rows], ['_$E45', '_$E43', '_$E46'])
        self.assertIs(rows[0]['references'], rows[1]['references'])
        self.assertEqual(rows[0]['references']['_$E43'], DTOR_AT)
        self.assertNotIn('_$E41', rows[0]['references'])
        self.assertEqual(rows[0]['relocations'][1]['symbol'], '_$E43')
        self.assertEqual([r['status'] for r in self.compare(rows, coff, original)], ['extent-exact'] * 3)

    def test_destructor_follows_its_initializer_when_its_content_is_ambiguous(self):
        rows, original = catalog_rows()
        # The destructor alone has no row: it is bound only through the initializer's push.
        rows = [rows[0], rows[2]]
        coff = FakeCoff([('_$E50', *destructor()), ('_$E51', *destructor()),
                         ('_$E53', *initializer(destructor='_$E51')), ('_$E54', *other())])
        mapping = rebind.group_mapping(coff, rows, rows[0]['references'], original)
        self.assertEqual(mapping['_$E41'], '_$E51')
        self.assertEqual(mapping['_$E43'], '_$E53')

    def test_a_real_byte_difference_still_refuses_with_a_mismatch(self):
        rows, original = catalog_rows()
        coff = fresh_object('_$E45', '_$E43', '_$E46', other_value=2)
        mapping = rebind.group_mapping(coff, rows, rows[0]['references'], original)
        # Content cannot bind _$E44 (its bytes differ); position (the shift of the
        # nearest bound name before it) compares it with its source counterpart.
        self.assertEqual(mapping['_$E44'], '_$E46')
        rebind.rebind_rows(rows, mapping)
        results = self.compare(rows, coff, original)
        self.assertEqual([r['status'] for r in results], ['extent-exact', 'extent-exact', 'mismatch'])
        self.assertEqual(results[2]['differing_bytes'], 1)

    def test_a_lookalike_never_turns_a_difference_into_a_match(self):
        rows, original = catalog_rows()
        # The differing initializer's slot is taken by nothing equal to the original;
        # a decoy with other targets does not bind (its relocation names differ).
        coff = fresh_object('_$E45', '_$E43', '_$E46', other_value=2, decoy='_$E47')
        mapping = rebind.group_mapping(coff, rows, rows[0]['references'], original)
        rebind.rebind_rows(rows, mapping)
        self.assertEqual(self.compare(rows, coff, original)[2]['status'], 'mismatch')

    def test_no_anchor_leaves_names_unbound(self):
        rows, original = catalog_rows()
        rows = [rows[2]]
        coff = fresh_object('_$E45', '_$E43', '_$E46', other_value=2)
        self.assertEqual(rebind.group_mapping(coff, rows, {}, original), {})
        self.assertEqual(self.compare(rows, coff, original)[0]['status'], 'comparison-refused')

    def test_bare_reference_binds_by_content_only(self):
        rows, original = catalog_rows()
        table = {'_$E41': DTOR_AT, '_$E90': 0x7fff0000, **{k: v for k, v in TABLE.items() if not k.startswith('_$E')}}
        rows = [dict(rows[2], references=table)]
        coff = fresh_object('_$E45', '_$E43', '_$E46')
        mapping = rebind.group_mapping(coff, rows, table, original)
        self.assertEqual(mapping, {'_$E41': '_$E43', '_$E44': '_$E46'})     # _$E90: no readable content

    def test_padded_comdat_extent_binds_against_the_full_extent(self):
        rows, original = catalog_rows()
        row = rows[2]
        data, relocs = other()
        padded = bytes(data) + b'\x90' * 5
        image = dict((row['entry'] + i, b) for i, b in enumerate(link(data, relocs, row['entry'], TABLE) + b'\x90' * 5))
        def padded_original(address, count):
            if any(address + i not in image for i in range(count)):
                raise ValueError('outside')
            return bytes(image[address + i] for i in range(count))
        row = dict(row, padding_bytes=5, full_extent_bytes=len(padded))
        coff = FakeCoff([('_$E46', padded, relocs)])
        self.assertEqual(rebind.group_mapping(coff, [row], row['references'], padded_original), {'_$E44': '_$E46'})

    def test_guard_flags_and_stable_functions_follow(self):
        # A function with a local static: it tests guard flag _$S7 and registers
        # the local's atexit destructor _$E9; the destructor clears the flag.
        func = (bytearray(b'\xa0' + bytes(4) + b'\x68' + bytes(4) + b'\xc3'), [(1, '_$S7', 6), (6, '_$E9', 6)])
        dtor = (bytearray(b'\x80\x25' + bytes(4) + b'\xfe\xc3'), [(2, '_$S7', 6)])
        table = {'_$S7': 0x600000, '_$E9': 0x401100}
        image = {}
        for address, (data, relocs) in {0x401000: func, 0x401100: dtor}.items():
            for i, b in enumerate(link(data, relocs, address, table)):
                image[address + i] = b
        def original(address, count):
            if any(address + i not in image for i in range(count)):
                raise ValueError('outside')
            return bytes(image[address + i] for i in range(count))
        rows = [dict(entry=0x401000, symbol='?F@@YAXXZ', symbol_prefix='?F@@YAXXZ', body_bytes=len(func[0]),
                     full_extent_bytes=len(func[0]), references=table, relocations=receipt(func[1], 0, table),
                     source='src/recovered/f.cpp'),
                dict(entry=0x401100, symbol='_$E9', symbol_prefix='_$E9', body_bytes=len(dtor[0]),
                     full_extent_bytes=len(dtor[0]), references=table, relocations=receipt(dtor[1], 0, table),
                     source='src/recovered/f.cpp')]
        fresh_func = (func[0], [(1, '_$S8', 6), (6, '_$E10', 6)])
        fresh_dtor = (dtor[0], [(2, '_$S8', 6)])
        coff = FakeCoff([('?F@@YAXXZ', *fresh_func), ('_$E10', *fresh_dtor)])
        coff.symbols[len(coff.symbols)] = dict(name='_$S8', value=0, section=1, type=0, storage=3, size=0)
        mapping = rebind.group_mapping(coff, rows, table, original)
        self.assertEqual(mapping, {'_$S7': '_$S8', '_$E9': '_$E10'})
        rebind.rebind_rows(rows, mapping)
        self.assertEqual(rows[0]['references'], {'_$S8': 0x600000, '_$E10': 0x401100})
        self.assertEqual([r['status'] for r in self.compare(rows, coff, original)], ['extent-exact'] * 2)

    def test_scoped_guard_flags_pair_only_within_their_function(self):
        old = '_?$S50@?4??Motion_Capture@@YIXXZ@4EA'
        fresh = '_?$S51@?4??Motion_Capture@@YIXXZ@4EA'
        other_scope = '_?$S51@?4??Sync_Delay@@YIXXZ@4EA'
        receipt = [dict(offset=2, kind=6, symbol=old, addend=0)]
        self.assertEqual(rebind._receipt_pairs(receipt, [(2, 6, 0, fresh)]), {old: fresh})
        self.assertIsNone(rebind._receipt_pairs(receipt, [(2, 6, 0, other_scope)]))
        self.assertIsNone(rebind._receipt_pairs(receipt, [(2, 6, 0, '_$S51')]))
        self.assertIsNone(rebind._receipt_pairs(receipt, [(2, 6, 0, '_$E51')]))
        self.assertIsNone(rebind.counted_form('?Motion_Capture@@YIXXZ'))

    def test_identical_initializers_pair_off_in_order_between_neighbours(self):
        empty = (bytearray(b'\xc3' + b'\x90' * 15), [])
        table = dict(TABLE)
        pieces = {0x401000: other(), 0x401010: empty, 0x401020: empty, 0x401030: other('_Object')}
        original = original_image(pieces)
        names = ['_$E10', '_$E13', '_$E16', '_$E19']
        rows = [dict(entry=address, symbol=name, symbol_prefix=name, body_bytes=len(data), full_extent_bytes=len(data),
                     references=table, relocations=receipt(relocs, address, table), source='src/recovered/a.cpp')
                for name, (address, (data, relocs)) in zip(names, sorted(pieces.items()))]
        # Two declarations were added between _$E10 and _$E13.
        coff = FakeCoff([('_$E10', *other()), ('_$E15', *empty), ('_$E18', *empty), ('_$E21', *other('_Object'))])
        mapping = rebind.group_mapping(coff, rows, table, original)
        self.assertEqual(mapping, {'_$E13': '_$E15', '_$E16': '_$E18', '_$E19': '_$E21'})

    def test_conflicting_units_of_one_population_keep_their_names(self):
        self.assertIsNone(rebind.merge_population_mappings([{'_$E1': '_$E2'}, {'_$E1': '_$E3'}]))
        self.assertIsNone(rebind.merge_population_mappings([{'_$E1': '_$E3'}, {'_$E2': '_$E3'}]))
        self.assertEqual(rebind.merge_population_mappings([{'_$E1': '_$E2'}, {}]), {'_$E1': '_$E2'})

    def test_source_refs_follow_simultaneously(self):
        text = '// @ref _$E41=00401040\n// @ref _$E43=00401000\n// @ref _$E430=00401080\nint _$E41;\n'
        self.assertEqual(rebind.rebind_source_refs(text, {'_$E41': '_$E43', '_$E43': '_$E45'}),
                         '// @ref _$E43=00401040\n// @ref _$E45=00401000\n// @ref _$E430=00401080\nint _$E41;\n')

    def test_only_initializer_names_are_renamed(self):
        row = dict(symbol='?F@@YAXXZ', symbol_prefix='?F@@', references={'?F@@': 1, '_$E4': 2},
                   relocations=[dict(symbol='_$E4'), dict(symbol='?G@@YAXXZ')])
        rebind.rebind_rows([row], {'_$E4': '_$E40', '?F@@': '?X@@'})
        self.assertEqual(row['symbol_prefix'], '?F@@')
        self.assertEqual(row['references'], {'?F@@': 1, '_$E40': 2})
        self.assertEqual([r['symbol'] for r in row['relocations']], ['_$E40', '?G@@YAXXZ'])

    def test_header_user_drift_allows_only_the_rebinding(self):
        rows, _ = catalog_rows()
        rows[0]['relocations'].append(dict(offset=30, kind=6, symbol='$T40373', target=1, addend=0, resolved=1))
        before = json.dumps(rows[0], sort_keys=True)
        mapping = {'_$E41': '_$E43', '_$E43': '_$E45'}
        rebind.rebind_rows(rows, mapping)
        rows[0]['relocations'][-1]['symbol'] = '$T40380'      # the scope-table counter moved too
        self.assertFalse(apply_rename.header_user_drifted(before, mapping, rows[0], {}))
        self.assertTrue(apply_rename.header_user_drifted(before, None, rows[0], {}))
        rows[0]['relocations'][-1]['target'] = 2
        self.assertTrue(apply_rename.header_user_drifted(before, mapping, rows[0], {}))
        rows[0]['relocations'][-1]['target'] = 1
        rows[0]['body_bytes'] += 1
        self.assertTrue(apply_rename.header_user_drifted(before, mapping, rows[0], {}))

if __name__ == '__main__':
    unittest.main()
