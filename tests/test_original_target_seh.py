"""Structured exception handling frames: fs:[__except_list], the linked
_except_handler3 and the per-compile `$T<n>` scope table (synthetic inputs)."""
from pathlib import Path
import struct
import sys
import tempfile
from types import SimpleNamespace
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools/original_target'))
import capstone  # noqa: E402
from canonical import CanonicalAudit, sha  # noqa: E402
from coff import Coff  # noqa: E402
from compile_candidates import compare_candidate, OPTIONS  # noqa: E402
import diff_candidate as diff  # noqa: E402
from verify_manifest import (Audit, LIBRARY_MANIFEST, bound_values, except_list_site,  # noqa: E402
                             receipt_key, TARGET_SHA256, TARGET_BYTES)

BASE = 0x400000
ENTRY = 0x401000
TABLE = 0x402000                # original scope table (read-only data)
HANDLER = 0x403008              # __except_handler3 inside its library member
MEMBER = dict(entry='00403000', bytes=0x20, member='exsup3.obj', symbols=['__except_handler3'])
CODE = 0x60000020
RDATA = 0x40000040

# push ebp; mov ebp,esp; push -1; push $T; push __except_handler3;
# mov eax,fs:[__except_list]; push eax; mov fs:[__except_list],esp;
# filter (+0x1d): mov eax,1; ret; handler (+0x23): mov ecx,[ebp-0x10];
# mov fs:[__except_list],ecx; mov esp,ebp; pop ebp; ret
FUNCTION = bytes.fromhex('558bec6aff' '6800000000' '6800000000' '64a100000000' '50' '64892500000000'
                         'b801000000c3' '8b4df0' '64890d00000000' '8be55dc3')
FILTER, HANDLER_LABEL = 0x1d, 0x23
# (offset, symbol) of each code relocation, all DIR32.
CODE_RELOCS = [(6, '$T7'), (11, '__except_handler3'), (17, '__except_list'), (25, '__except_list'),
               (0x29, '__except_list')]


def original_code():
    code = bytearray(FUNCTION)
    struct.pack_into('<I', code, 6, TABLE)
    struct.pack_into('<I', code, 11, HANDLER)
    return bytes(code)


def original_table(filter_va=ENTRY + FILTER, handler_va=ENTRY + HANDLER_LABEL):
    return struct.pack('<iII', -1, filter_va, handler_va)


def coff_object(table=b'\xff\xff\xff\xff' + bytes(8), table_relocs=((4, '$L1'), (8, '$L2')),
                table_flags=0x40401040, extra_table=False, labels=(FILTER, HANDLER_LABEL)):
    """A VC6-shaped object: a /Gy code COMDAT for _fixture and the associative
    .rdata COMDAT holding its scope table `$T7`."""
    names = ['_fixture', '$T7', '$L1', '$L2', '__except_handler3', '__except_list'] + (['$T8'] if extra_table else [])
    # name -> (value, section, type, storage)
    layout = {'_fixture': (0, 1, 0x20, 2), '$T7': (0, 2, 0, 3), '$L1': (labels[0], 1, 0, 6),
              '$L2': (labels[1], 1, 0, 6), '__except_handler3': (0, 0, 0x20, 2), '__except_list': (0, 0, 0, 2),
              '$T8': (12, 2, 0, 3)}
    strings = bytearray(4); symbols = []
    for name in names:
        value, section, kind, storage = layout[name]
        encoded = name.encode()
        if len(encoded) <= 8:
            field = encoded.ljust(8, b'\0')
        else:
            field = struct.pack('<II', 0, len(strings)); strings += encoded + b'\0'
        symbols.append(struct.pack('<8sIhHBB', field, value, section, kind, storage, 0))
    index = {n: i for i, n in enumerate(names)}
    sections = [('.text', FUNCTION, 0x60501020, CODE_RELOCS), ('.rdata', table, table_flags, list(table_relocs))]
    offset = 20 + 40 * len(sections); headers = []; data = bytearray()
    for name, blob, flags, relocs in sections:
        raw = offset + len(data); data += blob
        at = offset + len(data)
        for position, symbol in relocs:
            data += struct.pack('<IIH', position, index[symbol], 6)
        headers.append(struct.pack('<8sIIIIIIHHI', name.encode(), 0, 0, len(blob), raw, at if relocs else 0,
                                   0, len(relocs), 0, flags))
    struct.pack_into('<I', strings, 0, len(strings))
    return (struct.pack('<HHIIIHH', 0x14c, len(sections), 0, offset + len(data), len(symbols), 0, 0) +
            b''.join(headers) + data + b''.join(symbols) + bytes(strings))


class Image:
    """Original code at ENTRY, the scope table at TABLE, the library member."""
    def __init__(self, table=None):
        self.memory = bytearray(0x4000)
        self.put(ENTRY, original_code())
        self.put(TABLE, original_table() if table is None else table)

    def put(self, va, data):
        self.memory[va - BASE:va - BASE + len(data)] = data

    def original(self, va, count):
        if va < BASE or va - BASE + count > len(self.memory):
            raise ValueError('outside')
        return bytes(self.memory[va - BASE:va - BASE + count])

    @staticmethod
    def section(rva):
        if 0x1000 <= rva < 0x2000 or 0x3000 <= rva < 0x4000:
            return SimpleNamespace(Characteristics=CODE)
        if 0x2000 <= rva < 0x3000:
            return SimpleNamespace(Characteristics=RDATA)
        return None


def references(**extra):
    return dict({'$T': TABLE, '__except_handler3': HANDLER}, **extra)


def bindings(table_name='$T7', table=TABLE):
    """The receipt; `table_name` is the scope table's spelling as recorded at
    acceptance (another compile of the unit numbers it differently)."""
    out = []
    for offset, name in CODE_RELOCS:
        target = {'$T7': table, '__except_handler3': HANDLER}.get(name, 0)
        out.append(dict(offset=offset, kind=6, symbol=table_name if name == '$T7' else name, target=target,
                        addend=0, resolved=target))
    return out


class BindingRuleTests(unittest.TestCase):
    def test_scope_table_binds_only_through_the_stated_reference(self):
        self.assertEqual(bound_values({'$T': TABLE}, '$T271'), {TABLE})
        self.assertEqual(bound_values({'$T271': TABLE}, '$T271'), set())
        self.assertEqual(bound_values({}, '$T5'), set())
        self.assertEqual(bound_values({'$T': TABLE, '_x': 1}, '_x'), {1})

    def test_receipt_key_folds_only_scope_table_spellings(self):
        receipt = [dict(symbol='$T40372', target=TABLE), dict(symbol='$TPoint', target=1),
                   dict(symbol='_x$T5', target=2), dict(symbol='__except_handler3', target=HANDLER)]
        self.assertEqual([b['symbol'] for b in receipt_key(receipt)], ['$T', '$TPoint', '_x$T5', '__except_handler3'])
        self.assertEqual(receipt_key(bindings('$T98228')), receipt_key(bindings('$T40372')))
        self.assertNotEqual(receipt_key(bindings('$T98228', TABLE + 12)), receipt_key(bindings('$T40372')))
        self.assertEqual(receipt[0]['symbol'], '$T40372')     # the recorded receipt itself is not rewritten

    def test_except_list_is_absolute_zero_and_a_rebinding_is_ambiguous(self):
        self.assertEqual(bound_values({}, '__except_list'), {0})
        self.assertEqual(bound_values({'__except_list': 0}, '__except_list'), {0})
        self.assertEqual(len(bound_values({'__except_list': 0x401000}, '__except_list')), 2)

    def test_except_list_site_is_an_fs_zero_operand(self):
        decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32); decoder.detail = True
        site = lambda code: except_list_site(next(decoder.disasm(bytes.fromhex(code), 0)))
        self.assertTrue(site('64a100000000'))          # mov eax, fs:[0]
        self.assertTrue(site('64892500000000'))        # mov fs:[0], esp
        self.assertFalse(site('a100000000'))           # mov eax, ds:[0]
        self.assertFalse(site('64a104000000'))         # mov eax, fs:[4]
        self.assertFalse(site('648b4000'))             # mov eax, fs:[eax]

    def test_scratch_lookup_and_compiler_local_spelling(self):
        self.assertEqual(diff.lookup('$T12', {'$T': TABLE}), TABLE)
        self.assertIsNone(diff.lookup('$T12', {'$T12': TABLE}))
        self.assertEqual(diff.lookup('__except_list', {}), 0)
        row = dict(symbol='?Win@@YGHXZ', entry=ENTRY, relocations=[dict(symbol='$T12', target=TABLE)])
        self.assertNotIn('$T12', diff.canonical_references({'regions': [row]}))


class ScopeTableTests(unittest.TestCase):
    def audit(self, image=None, library=(MEMBER,)):
        image = image or Image()
        audit = Audit.__new__(Audit)
        audit.base = BASE
        audit.original = image.original
        audit.pe = SimpleNamespace(get_section_by_rva=image.section)
        audit.document = lambda path, expected=None: {'regions': list(library)} if path == LIBRARY_MANIFEST else {}
        return audit

    def check(self, obj, refs=None, image=None):
        coff = Coff(obj)
        audit = self.audit(image)
        candidates = {'f': dict(symbol_prefix='_fixture', entry=ENTRY, references=references() if refs is None else refs)}
        return audit, audit.original_duplicate(coff, 2, coff.sections[1], candidates)

    def test_table_with_owned_labels_equals_the_original(self):
        audit, accepted = self.check(coff_object())
        self.assertTrue(accepted)
        self.assertEqual(audit.data_original_copies, {'$T': TABLE})

    def test_refusals(self):
        cases = {
            'unbound': dict(obj=coff_object(), refs={}),
            'wrong address': dict(obj=coff_object(), refs=references(**{'$T': TABLE + 4})),
            'code address': dict(obj=coff_object(), refs=references(**{'$T': ENTRY})),
            'label moved': dict(obj=coff_object(labels=(FILTER + 1, HANDLER_LABEL))),
            'label outside function': dict(obj=coff_object(labels=(FILTER, len(FUNCTION)))),
            'pointer in level field': dict(obj=coff_object(table_relocs=((0, '$L1'), (8, '$L2')))),
            'partial record': dict(obj=coff_object(table=b'\xff' * 8, table_relocs=((4, '$L1'),))),
            'writable section': dict(obj=coff_object(table_flags=0xc0401040)),
            'two tables': dict(obj=coff_object(table=b'\xff\xff\xff\xff' + bytes(20), extra_table=True)),
            'original differs': dict(obj=coff_object(), image=Image(original_table(handler_va=ENTRY))),
        }
        for name, case in cases.items():
            with self.subTest(name):
                self.assertFalse(self.check(case['obj'], case.get('refs'), case.get('image'))[1])

    def test_library_member_binding(self):
        audit = self.audit()
        self.assertTrue(audit.library_member_defines('__except_handler3', HANDLER))
        self.assertFalse(audit.library_member_defines('__except_handler3', 0x403020))
        self.assertFalse(audit.library_member_defines('__except_handler2', HANDLER))


class CandidateComparisonTests(unittest.TestCase):
    def test_frame_relocations_resolve_and_compare(self):
        with tempfile.TemporaryDirectory() as temp:
            obj = Path(temp) / 'candidate.obj'; obj.write_bytes(coff_object())
            audit = SimpleNamespace(original=lambda va, size: original_code()[:size])
            candidate = dict(method='fixture', entry=ENTRY, symbol_prefix='_fixture', source='private/f.cpp',
                             source_sha256='f', references=references(), region_bytes=len(FUNCTION))
            result = compare_candidate(audit, Coff(obj.read_bytes()), candidate, obj, OPTIONS[0], 0)
            self.assertEqual(result['status'], 'extent-exact')
            self.assertEqual(result['relocations'], bindings())
            candidate['references'] = {'__except_handler3': HANDLER}
            refused = compare_candidate(audit, Coff(obj.read_bytes()), candidate, obj, OPTIONS[0], 0)
            self.assertEqual(refused['status'], 'comparison-refused')


class StrictAuditTests(unittest.TestCase):
    """The inherited strict verifier over a canonical row (as in a full build)."""
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory(); self.addCleanup(self.tmp.cleanup); self.root = Path(self.tmp.name)
        (self.root / 'src/recovered').mkdir(parents=True)
        self.source = self.root / 'src/recovered/00401000.cpp'; self.source.write_text('int fixture() {}\n')

    def row(self, refs=None, relocations=None):
        body = original_code()
        return dict(entry=ENTRY, method='401000', symbol='_fixture', symbol_prefix='_fixture',
                    source='src/recovered/00401000.cpp', source_sha256=sha(self.source.read_bytes()),
                    population='p', source_order=0, options=['/O2', '/Ob2', '/G5', '/Gy'],
                    body_bytes=len(body), body_sha256=sha(body), padding_bytes=0, padding_sha256=sha(b''),
                    full_extent_bytes=len(body), full_extent_sha256=sha(body),
                    references=references() if refs is None else refs,
                    relocations=bindings() if relocations is None else relocations)

    def verify(self, row, obj, image=None, library=(MEMBER,)):
        image = image or Image()
        audit = CanonicalAudit.__new__(CanonicalAudit)
        audit.repository = self.root; audit.base = BASE; audit.catalog = {'regions': [row]}
        audit.blobs = {}; audit.documents = {}; audit.coffs = {}; audit.validated_objects = set(); audit.groups = {}
        audit.decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32); audit.decoder.detail = True
        audit.original = image.original
        audit.pe = SimpleNamespace(get_section_by_rva=image.section)
        document = audit.document
        audit.document = lambda path, expected=None: ({'regions': list(library)} if path == LIBRARY_MANIFEST
                                                      else document(path, expected))
        body = original_code()
        bound = dict(entry='%08x' % ENTRY, ranges=[dict(start='%08x' % ENTRY, end_inclusive='%08x' % (ENTRY + len(body) - 1),
                                                         bytes_hex=body.hex())])
        audit.bounds = {'p': ({ENTRY: bound}, {})}
        path = self.root / 'fresh.obj'; path.write_bytes(obj)
        manifest = audit.verify_fresh(self.root, [([row], path)])
        return audit, manifest

    def test_seh_frame_passes_and_records_the_scope_table(self):
        audit, manifest = self.verify(self.row(), coff_object())
        self.assertEqual(manifest['matched_source_regions'], 1)
        self.assertEqual(manifest['accepted'][0]['relocations'], bindings())

    def test_renumbered_scope_table_passes(self):
        # Accepted when the unit numbered its table $T40372; a header change
        # elsewhere in the unit now makes the fresh compile emit $T7.
        audit, manifest = self.verify(self.row(relocations=bindings('$T40372')), coff_object())
        self.assertEqual(manifest['matched_source_regions'], 1)
        self.assertEqual(audit.data_original_copies, {'$T': TABLE})

    def test_renumbered_scope_table_bound_elsewhere_refuses(self):
        # The receipt records a different target: still refused.
        with self.assertRaisesRegex(ValueError, 'Relocation receipt differs'):
            self.verify(self.row(relocations=bindings('$T40372', TABLE + 12)), coff_object())
        # The stated reference moves with it: the table no longer equals the
        # original bytes there.
        image = Image(); image.put(TABLE + 12, original_table(filter_va=ENTRY + 1))
        with self.assertRaisesRegex(ValueError, 'Unsupported allocatable data section'):
            self.verify(self.row(refs=references(**{'$T': TABLE + 12}), relocations=bindings('$T40372', TABLE + 12)),
                        coff_object(), image=image)

    def test_renumbered_scope_table_with_different_bytes_refuses(self):
        with self.assertRaisesRegex(ValueError, 'Unsupported allocatable data section'):
            self.verify(self.row(relocations=bindings('$T40372')), coff_object(),
                        image=Image(original_table(handler_va=ENTRY + FILTER)))

    def test_renumbered_receipt_keeps_one_scope_table_per_object(self):
        with self.assertRaisesRegex(ValueError, 'Unsupported allocatable data section'):
            self.verify(self.row(relocations=bindings('$T40372')), coff_object(extra_table=True))

    def test_other_receipt_names_are_still_compared(self):
        receipt = bindings()
        receipt[1] = dict(receipt[1], symbol='__except_handler2')
        with self.assertRaisesRegex(ValueError, 'Relocation receipt differs'):
            self.verify(self.row(relocations=receipt), coff_object())

    def test_handler_outside_its_library_member_refuses(self):
        other = dict(MEMBER, symbols=['__except_handler2'])
        with self.assertRaisesRegex(ValueError, 'SEH helper __except_handler3'):
            self.verify(self.row(), coff_object(), library=(other,))

    def test_unbound_scope_table_refuses(self):
        refs = references(); del refs['$T']
        with self.assertRaisesRegex(ValueError, 'Unsupported allocatable data section'):
            self.verify(self.row(refs=refs), coff_object())

    def test_scope_table_differing_from_the_original_refuses(self):
        with self.assertRaisesRegex(ValueError, 'Unsupported allocatable data section'):
            self.verify(self.row(), coff_object(), image=Image(original_table(filter_va=ENTRY + 1)))

    def test_except_list_off_the_fs_segment_refuses(self):
        # The same relocation on a plain ds:[0] operand (no fs prefix).
        global FUNCTION
        saved = FUNCTION
        FUNCTION = FUNCTION[:15] + b'\x90' + FUNCTION[16:]       # 90 a1 00000000: nop; mov eax,[0]
        try:
            with self.assertRaisesRegex(ValueError, r'fs:\[__except_list\]'):
                self.verify(self.row(), coff_object())
        finally:
            FUNCTION = saved


if __name__ == '__main__':
    unittest.main()
