"""Module ledger and shift-build contracts on synthetic objects; no game bytes, no compiler."""
import pathlib
import struct
import sys
import unittest

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parents[1] / 'tools' / 'original_target'))
from coff import Coff
import accept_sources
from canonical import validate_split_declaration
import lane_tools
import module_check
import module_ledger
import shift_build


def coff_fixture(sections, symbols, relocations=()):
    """sections: [(name, payload, flags)]; symbols: [(name, value, section, storage)];
    relocations: [(section index 1-based, offset, symbol index, kind)]. Long names go to the string table."""
    strings = bytearray(b'\0\0\0\0')
    def name_field(name):
        if len(name) <= 8:
            return name.encode().ljust(8, b'\0')
        field = struct.pack('<II', 0, len(strings)); strings.extend(name.encode() + b'\0')
        return field
    headers, payload = [], bytearray()
    offset = 20 + 40 * len(sections)
    section_fields = [(('/%d' % len(strings)).encode().ljust(8, b'\0') if len(n) > 8 else n.encode().ljust(8, b'\0'))
                      for n, _, _ in sections]
    for n, _, _ in sections:
        if len(n) > 8:
            strings.extend(n.encode() + b'\0')
    relocs_by_section = {}
    for section, off, symbol, kind in relocations:
        relocs_by_section.setdefault(section, []).append(struct.pack('<IIH', off, symbol, kind))
    bodies = []
    for index, (n, data, flags) in enumerate(sections, 1):
        raw_at = offset + len(payload); payload.extend(data)
        blob = b''.join(relocs_by_section.get(index, []))
        reloc_at = offset + len(payload) if blob else 0; payload.extend(blob)
        bodies.append(struct.pack('<8sIIIIIIHHI', section_fields[index - 1], 0, 0, len(data), raw_at, reloc_at, 0,
                                  len(relocs_by_section.get(index, [])), 0, flags))
    table = bytearray()
    for name, value, section, storage in symbols:
        table.extend(struct.pack('<8sIhHBB', name_field(name), value, section, 0, storage, 0))
    symbols_at = offset + len(payload)
    struct.pack_into('<I', strings, 0, len(strings))
    header = struct.pack('<HHIIIHH', 0x14c, len(sections), 0, symbols_at, len(symbols), 0, 0)
    return header + b''.join(bodies) + bytes(payload) + bytes(table) + bytes(strings)


DATA, RDATA, CODE = 0xc0300040, 0x40300040, 0x60500020


class ModuleLedgerTests(unittest.TestCase):
    def original_image(self, layout):
        image = {}
        for address, data in layout:
            for i, b in enumerate(data):
                image[address + i] = b
        def original(va, count):
            if any(va + i not in image for i in range(count)):
                raise ValueError('outside the image')
            return bytes(image[va + i] for i in range(count))
        return original

    def test_data_sections_prove_at_their_bound_addresses_with_relocations(self):
        table = b'\x01\x00\x00\x00' + b'\x00\x00\x00\x00'          # {1, &"abc"} once relocated
        obj = coff_fixture([('.data', table, DATA), ('.data', b'abc\0', DATA), ('.text', b'\xc3', CODE)],
                           [('_Table', 0, 1, 3), ('??_C@_03ABC@abc?$AA@', 0, 2, 3), ('?F@@YAXXZ', 0, 3, 2)],
                           relocations=[(1, 4, 1, shift_build.DIR32)])
        coff = Coff(obj)
        original = self.original_image([(0x7f0000, b'\x01\x00\x00\x00\x08\x00\x7f\x00'), (0x7f0008, b'abc\0')])
        bindings = {'_Table': 0x7f0000, '??_C@_03ABC@abc?$AA@': 0x7f0008}
        copies, failures = module_ledger.prove_data(coff, bindings, original, {'?F@@YAXXZ': 0x401000})
        self.assertEqual(failures, [])
        self.assertEqual([(c['address'], c['bytes'], c['kind']) for c in copies],
                         [(0x7f0000, 8, 'table'), (0x7f0008, 4, 'string')])
        ranges, outside = module_ledger.coverage(copies, [(0x7f0000, 0x7f000c)], original)
        self.assertTrue(ranges[0]['complete']); self.assertEqual(ranges[0]['proven_bytes'], 12); self.assertEqual(outside, [])

    def test_differing_bytes_and_unbound_symbols_refuse(self):
        obj = coff_fixture([('.data', b'\x02\x00\x00\x00', DATA)], [('_Table', 0, 1, 3)])
        original = self.original_image([(0x7f0000, b'\x01\x00\x00\x00')])
        copies, failures = module_ledger.prove_data(Coff(obj), {'_Table': 0x7f0000}, original, {})
        self.assertEqual((copies, failures[0]['reason']), ([], 'bytes differ from the original'))
        copies, failures = module_ledger.prove_data(Coff(obj), {}, original, {})
        self.assertEqual(failures[0]['reason'], 'no original address bound')

    def test_coverage_refuses_gaps_that_are_not_zero_alignment_fill(self):
        # Two 5-byte copies with three zero bytes between them (alignment), then eight 0xff bytes.
        original = self.original_image([(0x7f0000, b'\x01' * 5 + b'\0' * 3 + b'\x02' * 5 + b'\0' * 3 + b'\xff' * 8)])
        copies = [dict(address=0x7f0000, bytes=5, names=['_a'], kind='table'),
                  dict(address=0x7f0008, bytes=5, names=['_b'], kind='table')]
        ranges, _ = module_ledger.coverage(copies, [(0x7f0000, 0x7f0010)], original)
        self.assertTrue(ranges[0]['complete']); self.assertEqual(ranges[0]['alignment_bytes'], 6)
        ranges, _ = module_ledger.coverage(copies, [(0x7f0000, 0x7f0018)], original)
        self.assertFalse(ranges[0]['complete']); self.assertEqual(ranges[0]['gaps'][0]['bytes'], 11)
        ranges, outside = module_ledger.coverage(copies, [(0x7f0000, 0x7f0005)], original)
        self.assertTrue(ranges[0]['complete']); self.assertEqual([c['address'] for c in outside], [0x7f0008])
        # A gap of four or more bytes is never alignment fill, zero or not.
        copies[1]['address'] = 0x7f000c
        original = self.original_image([(0x7f0000, b'\x01' * 5 + b'\0' * 7 + b'\x02' * 5)])
        ranges, _ = module_ledger.coverage(copies, [(0x7f0000, 0x7f0011)], original)
        self.assertFalse(ranges[0]['complete'])
        # Unless the next copy's section is 8-aligned (a type descriptor after an odd-length one).
        copies[1]['align'] = 8
        ranges, _ = module_ledger.coverage(copies, [(0x7f0000, 0x7f0011)], original)
        self.assertTrue(ranges[0]['complete'])
        self.assertEqual(module_ledger.section_alignment(0x00400040), 8)
        self.assertEqual(module_ledger.section_alignment(0x40), 1)


class ShiftBuildTests(unittest.TestCase):
    def test_section_renames_keep_contents_and_use_the_string_table(self):
        obj = coff_fixture([('.text', b'\xc3', CODE), ('.data', b'ab', DATA), ('.rdata$r', b'cd', RDATA),
                            ('.debug$F', b'ef', 0x42101048)],
                           [('?F@@YAXXZ', 0, 1, 2), ('_t', 0, 2, 3), ('??_R0X@@8', 0, 3, 2)])
        renamed = Coff(shift_build.rename_sections(obj, 'version'))
        self.assertEqual([s['name'] for s in renamed.sections], ['.text$zversion', '.data$zversion', '.rdata$zversion', '.debug$F'])
        self.assertEqual([s['data'] for s in renamed.sections], [b'\xc3', b'ab', b'cd', b'ef'])
        self.assertEqual(sorted(s['name'] for s in renamed.symbols.values()), ['??_R0X@@8', '?F@@YAXXZ', '_t'])
        with self.assertRaisesRegex(ValueError, 'String table'):
            shift_build.rename_sections(obj + b'\0', 'version')

    def test_container_carries_only_the_planned_relocations_against_undefined_symbols(self):
        parts = [('.text$00401000', b'\xe8\0\0\0\0\xc3'), ('.text$00401010', b'\xc3')]
        container = shift_build.write_container(parts, {0: [(1, '?Version_Number@VersionClass@@QAEIXZ', shift_build.REL32)]})
        parsed = Coff(container)
        self.assertEqual([(s['name'], s['data']) for s in parsed.sections], parts)
        self.assertEqual([(o, parsed.symbols[i]['name'], k) for o, i, k in parsed.sections[0]['relocs']],
                         [(1, '?Version_Number@VersionClass@@QAEIXZ', 20)])
        self.assertEqual(parsed.sections[1]['relocs'], [])
        self.assertTrue(all(s['section'] == 0 and s['storage'] == 2 for s in parsed.symbols.values()))
        self.assertEqual([(s['name'], s['data']) for s in Coff(shift_build.write_container(parts, {})).sections], parts)

    def test_definitions_name_the_scaffold_section_and_offset(self):
        lines = shift_build.definition_lines({'_sprintf': 0x7820f4, '__imp__X@4': 0x79a198},
                                             [('.text', 0x401000, 0x79a000), ('.rdata', 0x79a000, 0x7c9000)])
        self.assertEqual(lines, ['.globl "_sprintf"', '.set "_sprintf", .text + %d' % (0x7820f4 - 0x401000),
                                 '.globl "__imp__X@4"', '.set "__imp__X@4", .rdata + %d' % 0x198])
        with self.assertRaisesRegex(ValueError, 'outside the image'):
            shift_build.definition_lines({'_x': 0x10}, [('.text', 0x401000, 0x79a000)])
        with self.assertRaisesRegex(ValueError, 'spelling'):
            shift_build.definition_lines({'a"b': 0x401000}, [('.text', 0x401000, 0x79a000)])

    def test_run_patch_copies_the_data_directories_and_renames_the_resources(self):
        def pe32(directories, section_name):
            dos = bytearray(64); struct.pack_into('<I', dos, 0x3c, 64)
            coff = struct.pack('<IHHIIIHH', 0x4550, 0x14c, 1, 0, 0, 0, 224, 0x102)
            optional = bytearray(224); struct.pack_into('<H', optional, 0, 0x10b)
            optional[96:96 + 128] = directories
            section = struct.pack('<8sIIIIIIHHI', section_name, 0x1000, 0x1000, 0x200, 0x200, 0, 0, 0, 0, 0x40000040)
            return bytes(dos) + coff + bytes(optional) + section
        original = pe32(bytes(range(128)), b'.rsrc')
        linked = pe32(bytes(128), b'.rawres')
        patched = shift_build.patch_for_run(linked, original)
        self.assertEqual(patched[64 + 24 + 96:64 + 24 + 96 + 128], bytes(range(128)))
        self.assertEqual(patched[64 + 24 + 224:64 + 24 + 224 + 8], b'.rsrc\0\0\0')
        self.assertEqual(len(patched), len(linked))


MODULE_HEADER = """
class VersionClass {
public:
    VersionClass(void);
    virtual ~VersionClass(void) {}
    unsigned int Version_Number(void);
    unsigned short Major_Version(void);
    char *Version_Name(void);
    int Version_Protocol(int version);
    unsigned int Clip_Version(unsigned int minver, unsigned int maxver);
    unsigned int Min_Version(void);
private:
    void Read_Text_String(void);
    enum VersionEnum { MAJOR_VERSION = 0x0001, MINOR_VERSION = 0x0006 };
    unsigned int Version;
    unsigned short MajorVer;
    unsigned short MinorVer;
    char VersionName[30];
    char VersionText[16];
    unsigned int MinClipVer;
    unsigned int MaxClipVer;
    unsigned VersionInit : 1;
    unsigned MajorInit : 1;
    unsigned MinorInit : 1;
    unsigned TextInit : 1;
};
extern VersionClass VerNum;
"""
OTHER_HEADER = """
class VersionClass {
public:
    VersionClass();                 // 0x00712B20
    virtual ~VersionClass() {}
    unsigned int Version_Number(); // used by host initialization
    unsigned int Clip_Version(unsigned int minver, unsigned int maxver);
    int Version_Protocol(int version);
    unsigned int Min_Version();
    char *Version_Name();

    unsigned long Version;                  // +0x04
    unsigned short MajorVer;                // +0x08
    unsigned short MinorVer;                // +0x0A
    char VersionName[30];                   // +0x0C
    char VersionText[16];                   // +0x2A
    unsigned long MinClipVer;               // +0x3C
    unsigned long MaxClipVer;               // +0x40
    unsigned long Flags;                    // +0x44 bits 0-3
};
"""


class ModuleCheckTests(unittest.TestCase):
    def read(self, texts):
        return lambda path: texts[path]

    def test_split_declarations_agree_and_report_known_differences(self):
        texts = {'a.hpp': MODULE_HEADER, 'b.hpp': OTHER_HEADER}
        problems, known = module_check.compare_declarations('VersionClass', ['a.hpp', 'b.hpp'], self.read(texts))
        self.assertEqual(problems, [])
        self.assertTrue(any('unsigned long against unsigned int' in k for k in known), known)
        self.assertTrue(any('bit-fields' in k for k in known), known)
        self.assertTrue(any('Read_Text_String' in k for k in known), known)
        members, functions, virtuals = module_check.parse_class(MODULE_HEADER, 'VersionClass')
        self.assertEqual(module_check.layout(members, True)[1], 0x48)
        self.assertEqual(virtuals, ['~VersionClass'])

    def test_a_changed_signature_access_or_layout_refuses(self):
        changed = OTHER_HEADER.replace('unsigned int Min_Version();', 'unsigned long Min_Version();')
        problems, _ = module_check.compare_declarations('VersionClass', ['a.hpp', 'b.hpp'], self.read({'a.hpp': MODULE_HEADER, 'b.hpp': changed}))
        self.assertTrue(any('Min_Version ret' in p for p in problems), problems)
        changed = OTHER_HEADER.replace('public:\n    VersionClass();', 'private:\n    VersionClass();')
        problems, _ = module_check.compare_declarations('VersionClass', ['a.hpp', 'b.hpp'], self.read({'a.hpp': MODULE_HEADER, 'b.hpp': changed}))
        self.assertTrue(any('access' in p for p in problems), problems)
        changed = OTHER_HEADER.replace('char VersionText[16];', 'char VersionText[12];')
        problems, _ = module_check.compare_declarations('VersionClass', ['a.hpp', 'b.hpp'], self.read({'a.hpp': MODULE_HEADER, 'b.hpp': changed}))
        self.assertTrue(any('object size' in p or 'layout' in p or 'offsets' in p for p in problems), problems)
        changed = OTHER_HEADER.replace('virtual ~VersionClass() {}', '~VersionClass() {}')
        problems, _ = module_check.compare_declarations('VersionClass', ['a.hpp', 'b.hpp'], self.read({'a.hpp': MODULE_HEADER, 'b.hpp': changed}))
        self.assertTrue(any('virtual' in p for p in problems), problems)
        with self.assertRaisesRegex(module_check.Refusal, 'unsupported'):
            module_check.parse_class('class X { SomeType member; };', 'X')

    def test_references_into_the_module_must_use_its_symbols(self):
        catalog = dict(regions=[
            dict(entry=0x712b50, source='src/recovered/version.cpp', symbol='?Version_Number@VersionClass@@QAEIXZ', relocations=[]),
            dict(entry=0x491b70, source='src/recovered/a.cpp', symbol='?A@@YAXXZ',
                 relocations=[dict(target=0x712b50, symbol='?Version_Number@VersionClass@@QAEIXZ', kind=20)]),
            dict(entry=0x5bd070, source='src/recovered/b.cpp', symbol='?B@@YAXXZ',
                 relocations=[dict(target=0x712b50, symbol='?Unknown_Version_ID_00712b50@@YIIPAVVersionClass@@@Z', kind=20),
                              dict(target=0x7f9700, symbol='_VersionProtocol', kind=6)])])
        item = dict(source='src/recovered/version.cpp', data=[['007f9700', '007f9978']])
        problems = module_check.check_symbols(catalog, item, dict(data=[dict(symbol='_VersionProtocol')]))
        self.assertEqual(len(problems), 1); self.assertIn('005bd070', problems[0]); self.assertIn('Unknown_Version_ID', problems[0])
        problems = module_check.check_symbols(catalog, item, dict(data=[dict(symbol='_Other')]))
        self.assertEqual(len(problems), 2)


class SplitDeclarationFlagTests(unittest.TestCase):
    def test_flag_needs_a_note_naming_both_headers(self):
        self.assertFalse(validate_split_declaration({}))
        self.assertTrue(validate_split_declaration(dict(split_declaration=True,
                        split_declaration_note='lobby.hpp keeps a second declaration beside version.hpp')))
        for row in (dict(split_declaration_note='x.hpp y.hpp'), dict(split_declaration=False),
                    dict(split_declaration=True, split_declaration_note=''),
                    dict(split_declaration=True, split_declaration_note='only one header lobby.hpp')):
            with self.subTest(row=row), self.assertRaises(ValueError):
                validate_split_declaration(row)

    def test_plans_refuse_module_sources_and_carry_the_flag(self):
        catalog = dict(regions=[dict(entry=0x712b20, source='src/recovered/version.cpp', population='p-1'),
                                dict(entry=0x712b50, source='src/recovered/version.cpp', population='p-1'),
                                dict(entry=0x401000, source='src/recovered/a.cpp', population='p-2')])
        with self.assertRaisesRegex(SystemExit, 'module source'):
            accept_sources.refuse_module_sources(dict(sources=[dict(entry='00712b50', source='src/recovered/x.cpp')]), catalog)
        with self.assertRaisesRegex(SystemExit, 'module source'):
            accept_sources.refuse_module_sources(dict(sources=[dict(entry='00402000', source='src/recovered/version.cpp')]), catalog)
        accept_sources.refuse_module_sources(dict(sources=[dict(entry='00401000', source='src/recovered/a.cpp')]), catalog)
        plan = dict(sources=[dict(entry='00401000', source='src/recovered/a.cpp')])
        accept_sources.carry_count_calibration(plan, {0x401000: dict(split_declaration=True, split_declaration_note='a.hpp b.hpp')})
        self.assertEqual(plan['sources'][0]['split_declaration_note'], 'a.hpp b.hpp')
        with self.assertRaisesRegex(SystemExit, 'module source'):
            lane_tools.build_plan([dict(entry=0x712b20, source='src/recovered/version.cpp', population='p-1', source_order=0,
                                        symbol_prefix='?A@@', options=['/O2', '/Ob2', '/G5', '/Gy']),
                                   dict(entry=0x712b50, source='src/recovered/version.cpp', population='p-1', source_order=1,
                                        symbol_prefix='?B@@', options=['/O2', '/Ob2', '/G5', '/Gy'])],
                                  0x712b20, 'n', [], '/tmp', copy=lambda a, b: None)


if __name__ == '__main__':
    unittest.main()
