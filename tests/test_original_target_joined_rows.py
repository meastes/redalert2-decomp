"""Rows of several frozen populations in one translation unit (docs/DECISIONS.md "Route A split
rows", option (ii)): the strict audit's shape for a joined row. Synthetic bytes only, no compiler."""
import json
from pathlib import Path
import struct
import sys
import tempfile
import unittest
from types import SimpleNamespace

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools/original_target'))
import capstone
import apply_rename
import refile_unit
from canonical import CanonicalAudit, bound_population, sha, validate_catalog
from verify_manifest import TARGET_BYTES, TARGET_SHA256

OPTIONS = ['/O2', '/Ob2', '/G5', '/Gy']
HOME, JOINED = 0x401000, 0x401010       # the unit's own row; the row frozen in another population


def two_function_object(first, second):
    """A COFF object with two COMDAT-like .text sections: _home and _joined."""
    sections = [('.text', first, 0x60100020), ('.text', second, 0x60100020)]
    symbols = [struct.pack('<8sIhHBB', b'_home', 0, 1, 0x20, 2, 0), struct.pack('<8sIhHBB', b'_joined', 0, 2, 0x20, 2, 0)]
    offset = 20 + 40 * len(sections)
    headers, data = [], bytearray()
    for name, blob, flags in sections:
        raw = offset + len(data)
        data.extend(blob)
        headers.append(struct.pack('<8sIIIIIIHHI', name.encode(), 0, 0, len(blob), raw, 0, 0, 0, 0, flags))
    return (struct.pack('<HHIIIHH', 0x14c, len(sections), 0, offset + len(data), len(symbols), 0, 0) +
            b''.join(headers) + data + b''.join(symbols) + struct.pack('<I', 4))


def frozen(entry, body):
    return dict(entry='%08x' % entry, ranges=[dict(start='%08x' % entry, end_inclusive='%08x' % (entry + len(body) - 1),
                                                   bytes_hex=body.hex())])


class JoinedRowAuditTests(unittest.TestCase):
    """A unit (population `unit`) compiles its own row and a row frozen in population `other`."""
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory(); self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name)
        (self.root / 'src/recovered').mkdir(parents=True)
        self.source = self.root / 'src/recovered/unit.cpp'
        self.source.write_text('void home() {}\nint joined() { return 0; }\n')
        self.home_body, self.joined_body = b'\xc3', b'\x33\xc0\xc3'
        # The original image: home, 15 NOPs, joined, one NOP of frozen gap padding.
        self.image = self.home_body + b'\x90' * 15 + self.joined_body + b'\x90'
        self.gap = dict(start='%08x' % (JOINED + 3), length=1)

    def row(self, entry, symbol, body, padding=b'', **extra):
        row = dict(entry=entry, method='%x' % entry, symbol=symbol, symbol_prefix=symbol,
                   source='src/recovered/unit.cpp', source_sha256=sha(self.source.read_bytes()),
                   population='unit', source_order=entry, options=list(OPTIONS),
                   body_bytes=len(body), body_sha256=sha(body), padding_bytes=len(padding), padding_sha256=sha(padding),
                   full_extent_bytes=len(body) + len(padding), full_extent_sha256=sha(body + padding),
                   references={}, relocations=[])
        row.update(extra)
        return row

    def rows(self):
        return [self.row(HOME, '_home', self.home_body),
                self.row(JOINED, '_joined', self.joined_body, b'\x90', bound_population='other')]

    def catalog(self, rows):
        return dict(schema='ra2-canonical-source-v1', target=dict(size=TARGET_BYTES, sha256=TARGET_SHA256),
                    populations=[dict(id='unit'), dict(id='other')], regions=rows)

    def audit(self, rows, bounds):
        audit = CanonicalAudit.__new__(CanonicalAudit)
        audit.repository = self.root; audit.base = 0x400000; audit.catalog = self.catalog(rows)
        audit.blobs = {}; audit.documents = {}; audit.coffs = {}; audit.validated_objects = set(); audit.groups = {}
        audit.decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32); audit.decoder.detail = True
        audit.original = lambda address, size: self.image[address - HOME:address - HOME + size]
        audit.pe = SimpleNamespace(get_section_by_rva=lambda rva: object())
        audit.bounds = bounds
        return audit

    def verify(self, rows, bounds, joined_bytes=None):
        obj = self.root / 'fresh.obj'
        obj.write_bytes(two_function_object(self.home_body, (joined_bytes or self.joined_body) + b'\x90'))
        return self.audit(rows, bounds).verify_fresh(self.root, [(rows, obj)], jobs=1)

    def own_bounds(self):
        # The unit's pass freezes only its own row; the other pass freezes the joined row and its gap.
        return {'unit': ({HOME: frozen(HOME, self.home_body)}, {}),
                'other': ({JOINED: frozen(JOINED, self.joined_body)}, {JOINED + 3: self.gap})}

    def test_bound_population_is_the_rows_own_unless_it_joined(self):
        home, joined = self.rows()
        self.assertEqual((bound_population(home), bound_population(joined)), ('unit', 'other'))

    def test_a_row_of_another_population_is_accepted_when_exact(self):
        manifest = self.verify(self.rows(), self.own_bounds())
        self.assertEqual(manifest['matched_source_regions'], 2)
        joined = next(r for r in manifest['accepted'] if r['entry'] == JOINED)
        # the bound and the padding provenance come from the row's own frozen pass
        self.assertEqual(joined['original_bound'], frozen(JOINED, self.joined_body))
        self.assertEqual(joined['padding_provenance'], self.gap)
        self.assertEqual((joined['population'], joined['bound_population']), ('unit', 'other'))

    def test_a_joined_row_that_differs_in_the_unit_refuses(self):
        with self.assertRaisesRegex(ValueError, 'Complete compiler extent differs'):
            self.verify(self.rows(), self.own_bounds(), joined_bytes=b'\x31\xc0\xc3')

    def test_refused_when_its_own_bound_is_missing(self):
        bounds = self.own_bounds()
        bounds['other'] = ({}, {})
        with self.assertRaisesRegex(ValueError, 'no frozen bound in its own population other'):
            self.verify(self.rows(), bounds)

    def test_the_units_pass_never_stands_in_for_a_missing_own_bound(self):
        # The unit's pass happens to hold a row at the joined entry; the row's own pass does not.
        bounds = self.own_bounds()
        bounds['unit'][0][JOINED] = frozen(JOINED, self.joined_body)
        bounds['unit'][1][JOINED + 3] = self.gap
        bounds['other'] = ({}, {})
        with self.assertRaisesRegex(ValueError, 'no frozen bound in its own population other'):
            self.verify(self.rows(), bounds)

    def test_the_units_pass_does_not_override_the_own_bound(self):
        # The unit's pass disagrees about the joined entry (a longer body): the own bound decides.
        bounds = self.own_bounds()
        bounds['unit'][0][JOINED] = frozen(JOINED, self.joined_body + b'\x90')
        self.assertEqual(self.verify(self.rows(), bounds)['matched_source_regions'], 2)
        # ... and a row recorded with the unit's longer extent is not what its own pass froze.
        rows = [self.rows()[0], self.row(JOINED, '_joined', self.joined_body + b'\x90', bound_population='other')]
        with self.assertRaises(ValueError):
            self.verify(rows, bounds)

    def test_padding_must_come_from_the_own_gap_inventory(self):
        bounds = self.own_bounds()
        bounds['unit'][1][JOINED + 3] = self.gap        # only the unit's pass has the gap row
        bounds['other'] = (bounds['other'][0], {})
        # A missing gap row in the row's own pass is a listed refusal, never a KeyError.
        with self.assertRaisesRegex(ValueError, 'Padding with no gap row in the frozen pass'):
            self.verify(self.rows(), bounds)

    def test_unknown_frozen_population_refuses(self):
        rows = self.rows(); rows[1]['bound_population'] = 'gone'
        with self.assertRaisesRegex(ValueError, 'unknown frozen population'):
            self.verify(rows, self.own_bounds())

    def test_catalog_refuses_two_units_claiming_one_region(self):
        home, joined = self.rows()
        validate_catalog(self.root, self.catalog([home, joined]), modules=False)
        # The population that froze the row still lists it, in its own file.
        (self.root / 'src/recovered/00401010.cpp').write_text('int joined() { return 0; }\n')
        stay = dict(joined, population='other', source='src/recovered/00401010.cpp', source_order=99,
                    source_sha256=sha((self.root / 'src/recovered/00401010.cpp').read_bytes()))
        del stay['bound_population']
        with self.assertRaisesRegex(ValueError, 'Region claimed by two units: 00401010'):
            validate_catalog(self.root, self.catalog([home, joined, stay]), modules=False)
        # Two units both joining it.
        (self.root / 'src/recovered/second.cpp').write_text('int joined() { return 0; }\n')
        second = dict(joined, population='third', source='src/recovered/second.cpp', source_order=98,
                      source_sha256=sha((self.root / 'src/recovered/second.cpp').read_bytes()))
        catalog = self.catalog([home, joined, second]); catalog['populations'].append(dict(id='third'))
        with self.assertRaisesRegex(ValueError, 'Region claimed by two units: 00401010'):
            validate_catalog(self.root, catalog, modules=False)

    def test_catalog_refuses_a_malformed_provenance_key(self):
        home, joined = self.rows()
        for value in ('unit', 'absent'):
            with self.subTest(value=value), self.assertRaisesRegex(ValueError, 'without its own frozen population'):
                validate_catalog(self.root, self.catalog([home, dict(joined, bound_population=value)]), modules=False)

    def test_catalog_refuses_a_joined_row_in_a_closed_compiler_output_population(self):
        for pid in ('unit', 'other'):
            catalog = self.catalog(self.rows())
            next(p for p in catalog['populations'] if p['id'] == pid)['compiler_output_policy'] = dict(path='x', sha256='y')
            with self.subTest(pid=pid), self.assertRaisesRegex(ValueError, 'closed compiler-output population'):
                validate_catalog(self.root, catalog, modules=False)


class JoinRowsOwnBoundTests(unittest.TestCase):
    """apply_rename `joins` with `joins_bound: own`."""
    def setUp(self):
        self.folder = tempfile.TemporaryDirectory(); self.addCleanup(self.folder.cleanup)
        root = Path(self.folder.name)

        def write(name, rows):
            (root / name).write_text('\n'.join(json.dumps(r) for r in rows) + '\n')
            return str(root / name)
        bound = dict(entry='00000040', ranges=[dict(start='00000040', length=16)])
        gap = dict(start='00000050', length=3)
        self.config = dict(populations={
            'population-1': dict(inventory=write('i1', [dict(entry='00000010')]), gaps=write('g1', [])),
            'population-2': dict(inventory=write('i2', [bound]), gaps=write('g2', [gap])),
            'population-3': dict(inventory=write('i3', [dict(entry='00000010'), bound]), gaps=write('g3', [gap])),
            'population-4': dict(inventory=write('i4', []), gaps=write('g4', []))})

        def row(entry, population, **extra):
            return dict(dict(entry=entry, population=population, source='s%x' % entry, options=list(OPTIONS),
                             body_bytes=16, references={}, relocations=[], source_order=entry), **extra)
        self.row = row
        self.catalog = dict(populations=[dict(id='population-%d' % n) for n in (1, 2, 3, 4)],
                            regions=[row(0x10, 'population-1'), row(0x40, 'population-2')])

    def join(self, **more):
        return apply_rename.join_rows(self.catalog, 'm', self.catalog['regions'], {0x40}, self.config, **more)

    def test_a_joined_row_keeps_the_module_behind_its_population_s_earlier_sources(self):
        self.catalog['regions'][1]['source_order'] = 0x01      # ahead of the module's row
        self.catalog['regions'].append(self.row(0x80, 'population-4', source_order=0x90))
        apply_rename.join_rows(self.catalog, 'm', self.catalog['regions'][:2], {0x40}, self.config, own_bound=True)
        self.assertEqual(self.catalog['regions'][1]['source_order'], 0x91)

    def test_an_own_bound_join_brings_only_its_relocation_names(self):
        self.catalog['regions'][1].update(references={'?Other@@3HA': 0x900},
                                          relocations=[dict(symbol='?Used@@3HA', target=0x800)])
        self.join(own_bound=True)
        table = self.catalog['regions'][0]['references']
        self.assertEqual(table, {'?Used@@3HA': 0x800})

    def test_default_still_needs_an_equal_bound_in_the_units_pass(self):
        with self.assertRaises(SystemExit):
            self.join()

    def test_own_bound_keeps_the_frozen_population_as_provenance(self):
        self.assertEqual(self.join(own_bound=True), {'population-2'})
        joined = self.catalog['regions'][1]
        self.assertEqual((joined['population'], joined['bound_population']), ('population-1', 'population-2'))
        self.assertIs(joined['references'], self.catalog['regions'][0]['references'])

    def test_an_equal_bound_in_the_units_pass_needs_no_provenance_key(self):
        self.catalog['regions'][0]['population'] = 'population-3'
        self.assertEqual(self.join(own_bound=True), {'population-2'})
        self.assertNotIn('bound_population', self.catalog['regions'][1])

    def test_a_row_without_a_bound_in_its_own_pass_refuses(self):
        self.catalog['regions'][1]['population'] = 'population-4'
        with self.assertRaisesRegex(SystemExit, 'holds no frozen bound'):
            self.join(own_bound=True)

    def test_a_joined_row_moving_again_keeps_its_first_provenance(self):
        self.join(own_bound=True)
        self.catalog['regions'].append(self.row(0x80, 'population-4'))
        rows = self.catalog['regions'][1:]
        apply_rename.join_rows(self.catalog, 'm2', rows, {0x40}, self.config, own_bound=True)
        self.assertEqual((rows[0]['population'], rows[0]['bound_population']), ('population-4', 'population-2'))
        # ... and back in the population that froze it, the key goes.
        self.catalog['regions'].append(self.row(0x90, 'population-2'))
        rows = [self.catalog['regions'][1], self.catalog['regions'][3]]
        apply_rename.join_rows(self.catalog, 'm3', rows, {0x40}, self.config, own_bound=True)
        self.assertNotIn('bound_population', rows[0])

    def test_other_option_sets_and_closed_populations_refuse(self):
        self.catalog['populations'][1]['compiler_output_policy'] = dict(path='x', sha256='y')
        with self.assertRaisesRegex(SystemExit, 'closed compiler-output'):
            self.join(own_bound=True)
        del self.catalog['populations'][1]['compiler_output_policy']
        self.catalog['regions'][1]['options'] = OPTIONS + ['/GR']
        with self.assertRaisesRegex(SystemExit, 'another option set'):
            self.join(own_bound=True)

    def test_adopt_options_takes_the_units_set_and_adopt_gr_alone_does_not(self):
        o1 = ['/O1', '/Ob2', '/G5', '/Gy']
        self.catalog['regions'][1]['options'] = list(o1)
        with self.assertRaisesRegex(SystemExit, 'another option set'):
            self.join(own_bound=True, adopt_gr=True)
        self.assertEqual(self.join(own_bound=True, adopt_options=True), {'population-2'})
        self.assertEqual(self.catalog['regions'][1]['options'], OPTIONS)


class UnitCounterRenumberTests(unittest.TestCase):
    """join-own: a joining row's `_$E<n>`/`_$S<n>` names take numbers the unit does not use."""
    def test_names_move_past_the_unit_and_donors_stay_apart(self):
        members = [dict(symbol='_$E7', symbol_prefix='_$E7', relocations=[dict(symbol='_$E9', target=1)])]
        candidates = {
            0x10: dict(population='population-2', symbol='_$E4', symbol_prefix='_$E4',
                       relocations=[dict(symbol='_$E2', target=0x20), dict(symbol='_atexit', target=5)]),
            0x20: dict(population='population-2', symbol='_$E2', symbol_prefix='_$E2', relocations=[]),
            0x30: dict(population='population-3', symbol='_$E4', symbol_prefix='_$E4', relocations=[])}
        chunks = {0x10: '// @ref _$E2=00000020\nWstring w;', 0x20: '// destructor', 0x30: ''}
        renumber = refile_unit.renumber_counted(candidates, chunks, members, {'_$S11': 3}, 'x // @ref _$E12=1')
        self.assertEqual(renumber[0x10], {'_$E2': '_$E100013', '_$E4': '_$E100014'})
        self.assertIs(renumber[0x10], renumber[0x20])            # one block per donor population
        self.assertEqual(renumber[0x30], {'_$E4': '_$E100015'})
        row = refile_unit.renumbered_row(candidates[0x10], renumber[0x10])
        self.assertEqual((row['symbol'], [r['symbol'] for r in row['relocations']]), ('_$E100014', ['_$E100013', '_atexit']))
        self.assertEqual(candidates[0x10]['symbol'], '_$E4')     # the catalog row is untouched
        text = refile_unit.renumbered_text(chunks[0x10], renumber[0x10])
        self.assertEqual(text, '// @ref _$E100013=00000020\nWstring w;')
        self.assertEqual(refile_unit.counted_references(row, text), {'_$E100013': 0x20})

    def test_join_rows_applies_the_renumbering(self):
        catalog = dict(populations=[dict(id='population-1'), dict(id='population-2')], regions=[
            dict(entry=0x10, population='population-1', source='m', options=list(OPTIONS), references={},
                 relocations=[], source_order=1, symbol='_$E1', symbol_prefix='_$E1'),
            dict(entry=0x40, population='population-2', source='s', options=list(OPTIONS), references={'_$E2': 0x60},
                 relocations=[dict(symbol='_$E2', target=0x60)], source_order=2, symbol='_$E1',
                 symbol_prefix='_$E1')])
        config = dict(populations={})
        saved = apply_rename.frozen_bounds
        self.addCleanup(setattr, apply_rename, 'frozen_bounds', saved)
        apply_rename.frozen_bounds = lambda config: (lambda pid, row: ('own', None) if pid == 'population-2' else None)
        apply_rename.join_rows(catalog, 'm', catalog['regions'], {0x40}, config, own_bound=True,
                               renumber={0x40: {'_$E1': '_$E5', '_$E2': '_$E6'}})
        row = catalog['regions'][1]
        self.assertEqual((row['symbol'], row['symbol_prefix'], row['relocations'][0]['symbol']), ('_$E5', '_$E5', '_$E6'))
        # the placeholder it relocates to is bound in the unit's table (the re-binding needs its address)
        self.assertEqual(catalog['regions'][0]['references'].get('_$E6'), 0x60)


class AdoptOptionsTests(unittest.TestCase):
    """refile_unit.py join-own --adopt-options (docs/DECISIONS.md "Stand-ins frozen under another
    optimisation option than their unit")."""
    O1 = ['/O1', '/Ob2', '/G5', '/Gy']

    def test_notes_name_the_frozen_set(self):
        self.assertIsNone(refile_unit.options_note(OPTIONS, OPTIONS))
        self.assertEqual(refile_unit.options_note(OPTIONS, OPTIONS + ['/GR']),
                         "joined under the unit's /GR (frozen without it)")
        self.assertEqual(refile_unit.options_note(self.O1, OPTIONS + ['/GR']),
                         "joined under the unit's options (frozen under /O1 /Ob2 /G5 /Gy)")
        # a row frozen with /GR in a unit without it is another option set, not the /GR case
        self.assertEqual(refile_unit.options_note(OPTIONS + ['/GR'], OPTIONS),
                         "joined under the unit's options (frozen under /O2 /Ob2 /G5 /Gy /GR)")

    def test_adopt_options_reaches_stand_ins_and_the_units_own_second_population(self):
        gr = OPTIONS + ['/GR']
        join = refile_unit.joins_under_options
        for key in ('stand_in', 'own_population'):
            with self.subTest(key=key):
                self.assertTrue(join(self.O1, gr, key, adopt_options=True))
                self.assertFalse(join(self.O1, gr, key, adopt_gr=True))      # /GR alone is not enough
                self.assertFalse(join(self.O1, gr, key))
        # other classes keep the /GR rule only
        for key in ('header_state', 'template', 'option_set', 'emitted', 'frozen_bound'):
            with self.subTest(key=key):
                self.assertFalse(join(self.O1, gr, key, adopt_options=True))
                self.assertTrue(join(OPTIONS, gr, key, adopt_gr=True))
        self.assertTrue(join(OPTIONS, OPTIONS, 'header_state'))

    def test_other_options_excludes_the_same_set_and_the_units_gr(self):
        self.assertFalse(refile_unit.other_options(OPTIONS, OPTIONS))
        self.assertFalse(refile_unit.other_options(OPTIONS, OPTIONS + ['/GR']))
        self.assertTrue(refile_unit.other_options(self.O1, OPTIONS))
        self.assertTrue(refile_unit.other_options(OPTIONS + ['/GR'], OPTIONS))

    def test_a_row_stays_out_unless_unit_rows_differ_under_its_set(self):
        candidates = {0x10: dict(options=list(self.O1)), 0x20: dict(options=OPTIONS + ['/Gr']),
                      0x30: dict(options=list(OPTIONS))}
        chunks = {e: 'text' for e in candidates}
        refused = {}
        evidence = {tuple(self.O1): (3, 9), tuple(OPTIONS + ['/Gr']): (0, 9)}
        refile_unit.refuse_unevidenced(candidates, chunks, refused, evidence)
        self.assertEqual(sorted(candidates), [0x10, 0x30])
        self.assertEqual(sorted(chunks), [0x10, 0x30])
        self.assertRegex(refused[0x20], 'not evidenced against /O2 /Ob2 /G5 /Gy /Gr')

    def test_evidence_compiles_the_unit_unchanged_under_each_set(self):
        calls = []

        def compile_(config, catalog, pid, module, text, folder, options=None):
            calls.append((text, tuple(options)))
            return (None if options == ['/O2', '/Ob1', '/G5', '/Gy'] else 'obj'), '', 0
        saved = refile_unit.screen_compile, refile_unit.screen_rows
        self.addCleanup(lambda: setattr(refile_unit, 'screen_compile', saved[0]) or
                        setattr(refile_unit, 'screen_rows', saved[1]))
        refile_unit.screen_compile = compile_
        refile_unit.screen_rows = lambda audit, obj, rows, table, bounds: {rows[0]['entry']: 'differs'}
        members = [dict(entry=1), dict(entry=2)]
        out = refile_unit.option_evidence({}, {}, None, 'u', 'm', 'p', members, lambda path: 'UNIT TEXT', {},
                                          {tuple(self.O1), ('/O2', '/Ob1', '/G5', '/Gy')}, Path('/nonexistent'))
        self.assertEqual(out, {tuple(self.O1): (1, 2), ('/O2', '/Ob1', '/G5', '/Gy'): (2, 2)})
        self.assertEqual({text for text, _ in calls}, {'UNIT TEXT'})


class ContainerJoinTests(unittest.TestCase):
    MODULE = ('// unit\n#include "shared/vector_class.hpp"\n\n'
              '// F1 (0x00000010)\nint F1() { return 1; }\n\n'
              '// a row without an address comment\nint G() { return 0; }\n\n'
              '// F4 (0x00000040)\n// second comment line\nint F4() { return 4; }\n')
    MEMBER = ('// DynamicVectorClass<AClass *>::Clear (0x00000020).\n'
              '// @ref ?X@@3HA=00800000\n'
              '#define VECTOR_IMPLEMENT_DYNAMIC_CLEAR\n'
              '#include "shared/vector_class.hpp"\n\nclass AClass;\n\n'
              'template class DynamicVectorClass<AClass *>;\n')

    def state(self):
        return refile_unit.module_state(self.MODULE)

    def test_member_files_are_recognised(self):
        self.assertTrue(refile_unit.container_member_file(self.MEMBER))
        self.assertFalse(refile_unit.container_member_file('int F() { return 0; }\n'))
        self.assertFalse(refile_unit.container_member_file(
            '#include "shared/type_list.hpp"\ntemplate class TypeList<AClass *>;\n'))

    def test_former_member_switches_are_not_own_lines(self):
        self.assertEqual(refile_unit.container_own_lines(self.MEMBER, self.state()), [])
        block = '#ifndef VECTOR_IMPLEMENT_ALL\n#define VECTOR_IMPLEMENT_ALL\n#endif\n'
        self.assertEqual(refile_unit.container_own_lines(block + self.MEMBER, self.state()), [])

    def test_declaration_variants_and_other_header_state_are_own_lines(self):
        variant = self.MEMBER.replace('DYNAMIC_CLEAR', 'DYNAMIC_INSERT_AFTER_INDEX')
        self.assertEqual(refile_unit.container_own_lines(variant, self.state()),
                         ['#define VECTOR_IMPLEMENT_DYNAMIC_INSERT_AFTER_INDEX'])
        self.assertEqual(refile_unit.container_own_lines('#include "shared/other.hpp"\n' + self.MEMBER, self.state()),
                         ['#include "shared/other.hpp"'])

    def test_a_joined_member_keeps_only_its_description(self):
        self.assertEqual(refile_unit.member_comment(self.MEMBER), '// DynamicVectorClass<AClass *>::Clear (0x00000020).')
        with_layout = self.MEMBER.replace('\n', '\n// Layout and behaviour: shared/vector_class.hpp.\n', 1)
        self.assertEqual(refile_unit.member_comment(with_layout), '// DynamicVectorClass<AClass *>::Clear (0x00000020).')

    def test_members_at_one_place_form_one_compact_run(self):
        text, spans = refile_unit.place_in_address_order(
            self.MODULE, [0x10, 0x40], [(0x20, '// M20'), (0x30, '// M30'), (0x50, '// M50')], '// RUN')
        self.assertIn('// RUN\n// M20\n// M30\n', text)
        self.assertEqual(text.count('// RUN'), 2)
        self.assertEqual(spans[0x30][0], spans[0x20][0] + 1)

    def test_rows_go_in_address_order_before_the_next_named_unit_row(self):
        text, spans = refile_unit.place_in_address_order(
            self.MODULE, [0x10, 0x30, 0x40], [(0x50, 'int F5() { return 5; }'), (0x20, 'int F2() { return 2; }'),
                                              (0x08, 'int F0() { return 0; }')])
        order = [text.index(n) for n in ('F0', 'F1', 'int G', 'F2', 'F4 (', 'F5')]
        self.assertEqual(order, sorted(order))
        # G has no address comment: F2 sits behind it, before F4's comment block.
        self.assertLess(text.index('int G'), text.index('F2'))
        self.assertLess(text.index('F2'), text.index('// F4 (0x00000040)'))
        lines = text.split('\n')
        self.assertEqual(lines[spans[0x20][0] - 1], 'int F2() { return 2; }')

    def test_compile_errors_are_attributed_to_the_joining_row(self):
        spans = {0x20: (10, 12), 0x30: (16, 20)}
        log = ('accepted.cpp(111) : error C2065: \'x\' : undeclared identifier\n'
               'accepted.cpp(117) : warning C4244: conversion\n'
               'accepted.cpp(50) : error C2143: syntax error\n')
        inside, outside = refile_unit.screen_errors(log, spans, offset=100)
        self.assertEqual(list(inside), [0x20])
        self.assertEqual(len(outside), 1)

    def test_a_missing_gap_row_in_the_own_pass_is_a_listed_refusal(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            (root / 'inv.jsonl').write_text(json.dumps(dict(entry='00000020')) + '\n')
            (root / 'gaps.jsonl').write_text(json.dumps(dict(start='00000030', length=4)) + '\n')
            config = dict(populations=dict(p2=dict(inventory=str(root / 'inv.jsonl'), gaps=str(root / 'gaps.jsonl'))))
            row = dict(entry=0x20, population='p2', body_bytes=8, padding_bytes=8)
            self.assertIn('no gap row', refile_unit.own_frozen_bound(config, row, {}))
            self.assertIsNone(refile_unit.own_frozen_bound(config, dict(row, body_bytes=0x10), {}))
            self.assertIn('no inventory row', refile_unit.own_frozen_bound(config, dict(row, entry=0x24), {}))



class DeclarationsOnlyFlagTests(unittest.TestCase):
    SOURCE = b'// unit\n#define VECTOR_DECLARATIONS_ONLY\n#include "shared/vector_class.hpp"\n'
    MEMBER = [dict(symbol='??_7?$VectorClass@W4DiskID@@@@6B@', target=0x7d0000)]
    NOTE = dict(declarations_only=True, declarations_only_note='the generic members stay undefined here')

    def test_only_rows_referencing_a_container_member_carry_the_flag(self):
        from canonical import validate_declarations_only
        with self.assertRaisesRegex(ValueError, 'needs declarations_only'):
            validate_declarations_only(dict(entry=0x10, relocations=self.MEMBER), self.SOURCE)
        self.assertTrue(validate_declarations_only(dict(entry=0x10, relocations=self.MEMBER, **self.NOTE), self.SOURCE))
        # a row of the same source that names no container member is not a judgment call
        other = [dict(symbol='?Draw@@YAXXZ', target=0x401000)]
        self.assertFalse(validate_declarations_only(dict(entry=0x20, relocations=other), self.SOURCE))
        with self.assertRaisesRegex(ValueError, 'does not reference'):
            validate_declarations_only(dict(entry=0x20, relocations=other, **self.NOTE), self.SOURCE)

    def test_no_flag_without_the_switch(self):
        from canonical import validate_declarations_only
        self.assertFalse(validate_declarations_only(dict(entry=0x10, relocations=self.MEMBER), b'int F() { return 0; }\n'))
        with self.assertRaisesRegex(ValueError, 'does not reference'):
            validate_declarations_only(dict(entry=0x10, relocations=self.MEMBER, **self.NOTE), b'int F();\n')

if __name__ == '__main__':
    unittest.main()
