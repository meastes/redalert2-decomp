"""Clean-up end point census (lane K1, 2026-11-14): token definitions, counterparts, the five measures."""
from pathlib import Path
import sys
import tempfile
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1]/'tools/original_target'))
import cleanup_census

CLEAN = '''// alpha.cpp: FUN_00401000 in a comment is not code; neither is "param_1" in a string.
#include "shared/alpha.hpp"
int AlphaClass::Count(int unknown_0x24) { return unknown_0x24 + Size; }
void Blit() { __asm { mov eax, ecx } }
'''
DIRTY = '''int FUN_00402000(int param_1)
{
    int iVar1 = param_1;
    unsigned int eax = CONCAT11(iVar1, 0);
    undefined4 local_10 = 0;
    return Leaf_402000::get(eax) + SUB41;
}
'''


def region(entry, source, symbol='?f@@YAXXZ', size=10):
    return dict(entry=entry, source=source, symbol=symbol, body_bytes=size)


class Tokens(unittest.TestCase):
    def test_categories(self):
        counts = cleanup_census.scan(DIRTY)
        self.assertEqual(counts['ghidra_symbol'], 1)            # FUN_00402000
        self.assertEqual(counts['ghidra_local'], 5)             # param_1 x2, iVar1 x2, local_10
        self.assertEqual(counts['register'], 2)                 # eax declared and used
        self.assertEqual(counts['ghidra_op'], 1)                # CONCAT11( but not a bare SUB41
        self.assertEqual(counts['ghidra_type'], 1)
        self.assertEqual(counts['placeholder'], 1)              # Leaf_402000

    def test_comments_strings_and_asm_are_not_code(self):
        counts = cleanup_census.scan(CLEAN)
        self.assertEqual(sum(counts[c] for c in cleanup_census.CATEGORIES), 0)
        self.assertEqual(counts['placeholder'], 2)              # unknown_0x24 twice

    def test_ordinary_names_are_not_tokens(self):
        for name in ('edit', 'editor', 'dx', 'uint', 'byte', 'in_range', 'in_focus', 'UNKNOWN', 'Unknowns'):
            self.assertIsNone(cleanup_census.classify(name, '('), name)
        self.assertEqual(cleanup_census.classify('in_EAX'), 'ghidra_local')
        self.assertEqual(cleanup_census.classify('ppuVar12'), 'ghidra_local')
        self.assertEqual(cleanup_census.classify('ecx_word'), 'register')
        self.assertEqual(cleanup_census.classify('UnknownSessionA3D29C'), 'placeholder')


class Counterparts(unittest.TestCase):
    def test_yes_no_unknown(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            self.assertEqual(cleanup_census.counterpart('alpha', cleanup_census.reference_stems(root)), 'unknown')
            (root / 'TibSun/code').mkdir(parents=True)
            (root / 'TibSun/code/ALPHA.CPP').write_text('')
            stems = cleanup_census.reference_stems(root)
            self.assertEqual(cleanup_census.counterpart('alpha', stems), 'yes')
            self.assertEqual(cleanup_census.counterpart('beta', stems), 'unknown')   # mischa85 absent
            (root / 'mischa85-RedAlert2/src/sub').mkdir(parents=True)
            (root / 'mischa85-RedAlert2/src/sub/gamma.c').write_text('')
            stems = cleanup_census.reference_stems(root)
            self.assertEqual(cleanup_census.counterpart('beta', stems), 'no')
            self.assertEqual(cleanup_census.counterpart('gamma', stems), 'yes')


class Ra2DecompCounterpart(unittest.TestCase):
    def test_yes_no_unknown_and_not_in_line_one(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            self.assertIsNone(cleanup_census.ra2decomp_stems(root))
            self.assertEqual(cleanup_census.ra2decomp_counterpart('alpha', None), 'unknown')
            (root / 'ra2-decomp/src/wpaudio').mkdir(parents=True)
            (root / 'ra2-decomp/src/wpaudio/Beta.CPP').write_text('')
            (root / 'ra2-decomp/src/alpha.h').write_text('')
            stems = cleanup_census.ra2decomp_stems(root)
            self.assertEqual(cleanup_census.ra2decomp_counterpart('beta', stems), 'yes')
            self.assertEqual(cleanup_census.ra2decomp_counterpart('alpha', stems), 'no')   # a header is not a unit
        inputs, read = Census.inputs(None)
        result = cleanup_census.census(inputs, read, dict(tibsun=set(), mischa85=set()), file_backed_non_code=1000,
                                       ra2decomp={'beta'})
        units = {u['unit']: u for u in result['units']}
        self.assertEqual((units['alpha']['ra2decomp_counterpart'], units['beta']['ra2decomp_counterpart']), ('no', 'yes'))
        self.assertEqual(result['counterpart_units']['total'], 0)                     # line 1 stays TibSun/mischa85
        header, beta = cleanup_census.unit_lines(result, True).splitlines()[:2]
        self.assertEqual(header.split('\t')[2], 'ra2-decomp counterpart')
        self.assertEqual(beta.split('\t')[:3], ['beta', 'no', 'yes'])
        default = cleanup_census.census(inputs, read, dict(tibsun=set(), mischa85=set()), file_backed_non_code=1000)
        self.assertEqual({u['ra2decomp_counterpart'] for u in default['units']}, {'unknown'})


class Census(unittest.TestCase):
    def inputs(self):
        unit_map = {'alpha': ['00401000', '00401100', '00401200'], 'beta': ['00402000'],
                    'libcmt/strlen': ['00781000']}
        modules = {'alpha': dict(module='alpha', source='src/recovered/alpha.cpp', route='A',
                                 residuals=[dict(kind='split', entry='00401200', bytes=10, cause='held by population-9')],
                                 data=[dict(address='007a0000', bytes=16, kind='string')], kept_elsewhere=[],
                                 data_ranges=[dict(bytes=20, proven_bytes=16)],
                                 data_unproven=[dict(symbol='_x', reason='no original address bound')])}
        regions = [region(0x401000, 'src/recovered/alpha.cpp'),
                   region(0x401100, 'src/recovered/header_statics/00401100/init.cpp', '_$E2', 12),
                   region(0x401200, 'src/recovered/classes/AlphaClass/Other.cpp', size=10),
                   region(0x402000, 'src/recovered/00402000.cpp', size=30),
                   region(0x403000, 'src/recovered/00403000.cpp', size=7),
                   region(0x781000, 'src/recovered/strlen.cpp')]
        drafts = [dict(entry='00401300', bytes=50, source='src/recovered/alpha.cpp')]
        texts = {'src/recovered/alpha.cpp': CLEAN, 'src/recovered/00402000.cpp': DIRTY,
                 'src/recovered/header_statics/00401100/init.cpp': 'void f() {}\n',
                 'src/recovered/classes/AlphaClass/Other.cpp': 'void g() {}\n',
                 'src/shared/alpha.hpp': 'class AlphaClass { int Size; };\n'}
        return (dict(unit_map=unit_map, modules=modules, declared={'alpha': dict(header='src/shared/alpha.hpp')},
                     regions=regions, drafts=drafts, generated=[dict(range=['00800000', '00800100'])]),
                texts.get)

    def test_measures(self):
        inputs, read = self.inputs()
        stems = dict(tibsun={'alpha', 'beta'}, mischa85=set())
        result = cleanup_census.census(inputs, read, stems, file_backed_non_code=1000)
        units = {u['unit']: u for u in result['units']}
        self.assertEqual(set(units), {'alpha', 'beta'})          # library units are not counted
        self.assertEqual(units['alpha']['files'], 4)             # module source, two outside files, header
        self.assertEqual(units['alpha']['tokens'], 0)
        self.assertEqual(units['alpha']['placeholders'], 2)
        self.assertFalse(units['alpha']['done'])                 # counterpart unit: placeholders count
        self.assertEqual(units['beta']['address_named_files'], 1)
        self.assertEqual(units['alpha']['address_named_files'], 1)   # header_statics/00401100/init.cpp: its folder
        self.assertEqual(result['counterpart_units'], dict(done=0, total=2))
        f = result['functions']
        self.assertEqual((f['outside_rows'], f['outside_bytes']), (3, 52))
        self.assertEqual((f['recorded_residual_rows'], f['leftover_rows']), (1, 1))
        self.assertEqual(f['leftovers']['shared_initialiser'], [1, 12])
        self.assertEqual((f['unassigned_rows'], f['unassigned_bytes']), (1, 7))
        d = result['data']
        self.assertEqual((d['proven'], d['typed_total'], d['unproven_items']), (16, 20, 1))
        self.assertEqual(d['untyped_from_original'], 1000 - 16 - 4 - 256)
        self.assertEqual(result['drafts'], dict(count=1, bytes=50))
        block = cleanup_census.block(result).splitlines()
        self.assertEqual(len(block), 6)
        self.assertIn('3 files, 1 directories', block[5])   # 00402000.cpp, 00403000.cpp, header_statics/00401100/init.cpp
        self.assertIn('0 / 2', block[0])
        self.assertIn('3 rows, 52 bytes / 1 rows, 12 bytes', block[2])
        work = cleanup_census.unit_lines(result).splitlines()
        self.assertEqual(work[1].split('\t')[0], 'beta')         # most tokens first

    def test_joined_row_without_unit_map_entry_belongs_to_its_unit(self):
        inputs, read = self.inputs()
        inputs['regions'].append(region(0x401400, 'src/recovered/alpha.cpp', '_$E7', 9))
        inputs['declared']['alpha']['joined'] = [dict(entry='00401400', bytes=9, unit_evidence='xcu single'),
                                                 dict(entry='00403000', bytes=7)]
        f = cleanup_census.census(inputs, read, dict(tibsun=set(), mischa85=set()), file_backed_non_code=1000)['functions']
        self.assertEqual((f['unassigned_rows'], f['unassigned_bytes']), (0, 0))     # both joined rows now owned
        self.assertEqual((f['outside_rows'], f['outside_bytes']), (4, 59))          # 0x403000 sits outside alpha.cpp

    def test_address_named_tree(self):
        tree = cleanup_census.address_named_tree([
            'src/recovered/header_statics/0040b280/a.cpp', 'src/recovered/header_statics/0040b280/b.cpp',
            'src/recovered/header_statics/bomb/c.cpp', 'src/recovered/0056d620.cpp', 'src/recovered/x_0056d620.cpp',
            'src/recovered/classes/DynamicVectorClass_DDSURFACEDESC_ptr/Add.cpp', 'src/recovered/facade.cpp',
            'src/shared/unknown_base_539000.hpp', 'src/recovered/0x00401000_dir/deep/f.cpp'])
        self.assertEqual(tree['directories'], ['src/recovered/0x00401000_dir', 'src/recovered/header_statics/0040b280'])
        self.assertEqual(tree['files'], ['src/recovered/0056d620.cpp', 'src/recovered/0x00401000_dir/deep/f.cpp',
                                         'src/recovered/header_statics/0040b280/a.cpp',
                                         'src/recovered/header_statics/0040b280/b.cpp',
                                         'src/recovered/x_0056d620.cpp', 'src/shared/unknown_base_539000.hpp'])
        inputs, read = self.inputs()
        inputs['tree'] = ['src/recovered/alpha.cpp', 'src/recovered/header_statics/00401100/init.cpp']
        result = cleanup_census.census(inputs, read, dict(tibsun=set(), mischa85=set()), file_backed_non_code=1000)
        self.assertEqual((result['address_named']['files'], result['address_named']['directories']), (1, 1))
        inputs['tree'].append('src/recovered/unidentified/seed_views/0040bc60.cpp')
        result = cleanup_census.census(inputs, read, dict(tibsun=set(), mischa85=set()), file_backed_non_code=1000)
        self.assertEqual(result['address_named']['unidentified'], 1)
        inputs['tree'].append('src/recovered/awaiting_join/member_views/0040d1d0.cpp')
        result = cleanup_census.census(inputs, read, dict(tibsun=set(), mischa85=set()), file_backed_non_code=1000)
        self.assertEqual(result['address_named']['awaiting_join'], 1)
        self.assertIn('3 files, 1 directories; 1 of the files in src/recovered/unidentified/, '
                      '1 in src/recovered/awaiting_join/, 1 elsewhere', cleanup_census.block(result).splitlines()[5])

    def test_clean_unit_without_counterpart_allows_placeholders(self):
        inputs, read = self.inputs()
        inputs['regions'][1]['source'] = 'src/recovered/header_statics/alpha/init.cpp'
        result = cleanup_census.census(inputs, read, dict(tibsun=set(), mischa85=set()), file_backed_non_code=1000)
        units = {u['unit']: u for u in result['units']}
        self.assertTrue(units['alpha']['done'])
        self.assertEqual(result['other_units'], dict(done=1, total=2))


HEADER = """#ifndef RA2_SHARED_ALPHA_HPP
#define RA2_SHARED_ALPHA_HPP
class Beta;
enum class Mode { A, B };
template<class T> class Box { T v; };
class AlphaClass : public Base<int> {
public:
    int Count(int);
    struct Inner { int x; };
};
#ifdef RA2_ALPHA_WIDE
int wide;
#endif
#if defined(RA2_NON_MATCHING) && !defined(RA2_OTHER)
#endif
// class Gamma { };
extern int alpha_count_00a10000;
#endif
"""
UNIT = """#include "shared/alpha.hpp"
extern int alpha_count_00a10000;
extern "C" int __cdecl abs(int value);
extern char const name_7d7eac[];
extern "C" {
int c_thing;
}
// @ref ?Count@AlphaClass@@QAEHH@Z=00401000
  // @ref _alpha_count_00a10000=00a10000
struct AlphaClass { int duplicate; };
int Helper_00401020(int unknown_0x24) { return unknown_0x24 + Free_0x00401040(); }
int AlphaClass::Count(int n) { return n; }
"""


class Structure(unittest.TestCase):
    def test_definitions_not_declarations(self):
        found = cleanup_census.class_definitions(cleanup_census.code_only(HEADER))
        self.assertEqual(found, [('class', 'Box'), ('class', 'AlphaClass'), ('struct', 'Inner')])

    def test_switches_skip_include_guards(self):
        self.assertEqual(cleanup_census.preprocessor_switches(HEADER),
                         {'RA2_ALPHA_WIDE', 'RA2_NON_MATCHING', 'RA2_OTHER'})

    def test_address_suffixed(self):
        found = cleanup_census.address_suffixed(UNIT)
        self.assertEqual(found, {'alpha_count_00a10000': 1, 'name_7d7eac': 1, 'Helper_00401020': 1,
                                 'Free_0x00401040': 1})
        self.assertEqual(cleanup_census.address_suffixed('int unknown_0x24; int x_123456;'), {})

    def test_externs(self):
        names = [n for n, _ in cleanup_census.extern_names(cleanup_census.code_only(UNIT))]
        self.assertEqual(names, ['alpha_count_00a10000', 'abs', 'name_7d7eac'])

    def test_symbol_parts(self):
        self.assertEqual(cleanup_census.symbol_parts('?Process@FlasherClass@@QAE_NXZ'),
                         ('Process', 'FlasherClass', True))
        self.assertEqual(cleanup_census.symbol_parts('?Advance_004bbf00@@YI_NAAUX@@@Z'),
                         ('Advance_004bbf00', None, False))
        self.assertEqual(cleanup_census.base_name('Process_004bbf00'), 'Process')

    def test_member_counterparts(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / 'mischa85-RedAlert2/config/ra2-1.006').mkdir(parents=True)
            (root / 'mischa85-RedAlert2/config/ra2-1.006/names.csv').write_text(
                'address,symbol,evidence\n0x004BBF00,?Process@FlasherClass@@QAE_NXZ,matched\n'
                '0x00401000,?Free@@YAXXZ,matched\n')
            (root / 'TibSun/code').mkdir(parents=True)
            (root / 'TibSun/code/alpha.cpp').write_text(
                'bool AlphaClass::Shake(void)\n{\n return true;\n}\nint Plain(int a) { return a; }\n')
            regions = [region(0x4bbf00, 'src/recovered/flasher/A.cpp', '?Advance_004bbf00@@YI_NAAUX@@@Z'),
                       region(0x401000, 'src/recovered/a/F.cpp', '?Free@@YAXXZ'),
                       region(0x401100, 'src/recovered/a/S.cpp', '?Shake_00401100@@YI_NXZ'),
                       region(0x401200, 'src/recovered/a/P.cpp', '?Plain@Unknown_401200@@QAEHH@Z'),
                       region(0x401300, 'src/recovered/alpha.cpp', '?Shake@AlphaClass@@QAE_NXZ')]
            rows = cleanup_census.member_counterparts(regions, {0x401100: 'alpha', 0x401200: 'alpha',
                                                                0x401300: 'alpha'}, root)
            self.assertEqual([(r['entry'], r['kind'], r['counterparts']) for r in rows],
                             [('004bbf00', 'free', {'mischa85': 'FlasherClass::Process'}),
                              ('00401100', 'free', {'tibsun': 'AlphaClass::Shake'})])

    def test_structure_block(self):
        tree = ['src/shared/alpha.hpp', 'src/recovered/alpha.cpp', 'src/recovered/beta/One.cpp',
                'src/recovered/beta/Two.cpp', 'src/recovered/beta/Empty.cpp']
        texts = {'src/shared/alpha.hpp': HEADER, 'src/recovered/alpha.cpp': UNIT,
                 'src/recovered/beta/One.cpp': 'extern int alpha_count_00a10000;\nint One() { return 1; }\n',
                 'src/recovered/beta/Two.cpp': 'int Two() { return 2; }\n', 'src/recovered/beta/Empty.cpp': ''}
        regions = [region(0x401000, 'src/recovered/alpha.cpp'), region(0x401010, 'src/recovered/beta/One.cpp'),
                   region(0x401020, 'src/recovered/beta/Two.cpp'), region(0x401030, 'src/recovered/beta/Two.cpp')]
        with tempfile.TemporaryDirectory() as tmp:
            result = cleanup_census.structure(tmp, tmp, tree=tree, regions=regions, unit_map={},
                                              modules={'alpha': {'source': 'src/recovered/alpha.cpp'}},
                                              read=texts.get)
        self.assertEqual(result['classes']['multiply_defined'], 1)
        self.assertEqual(result['classes']['top'][0]['name'], 'AlphaClass')
        self.assertEqual(result['switches']['count'], 3)
        f = result['files_outside']
        self.assertEqual((f['unit_files'], f['outside'], f['one_function'], f['several_functions'], f['no_row']),
                         (1, 3, 1, 1, 1))
        self.assertEqual(result['ref_lines'], dict(lines=2, files=1))
        e = result['externs']
        self.assertEqual((e['lines'], e['names'], e['files'], e['declared_in_a_header'], e['in_several_cpp']),
                         (4, 3, 2, 1, 1))
        lines = cleanup_census.structure_block(result).splitlines()
        self.assertEqual(len(lines), 7)
        self.assertTrue(lines[5].startswith('(f) "// @ref" comment lines: 2 in 1 files'))


if __name__ == '__main__':
    unittest.main()
