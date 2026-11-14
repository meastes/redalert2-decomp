"""Non-exact drafts in source (user decision 2026-11-13): guarded blocks, markers, the manifest flag,
the strict-audit refusals (claims of acceptance, instruction arrays, inline asm, guard spellings) and the
STATUS lines."""
import json
from pathlib import Path
import sys
import tempfile
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1]/'tools/original_target'))
import non_exact
import status

MARK = '// Not exact (RA2_NON_MATCHING): Foo::Bar at 0x00401100, best score 61.50, 2026-11-13; differs: registers'
MODULE = '''// unit.cpp
#include "shared/foo.hpp"

// int Foo::Baz() (0x00401000)
int Foo::Baz() { return 1; }

%s
#ifdef RA2_NON_MATCHING
// int Foo::Bar() (0x00401100)
int Foo::Bar() { return Baz() + 1; }
#endif

// int Foo::Qux() (0x00401200)
int Foo::Qux() { return 2; }
'''


def region(entry, source, size=0x20):
    return dict(entry=entry, source=source, full_extent_bytes=size, body_bytes=size, padding_bytes=0,
                population='population-1', options=[], source_order=entry, symbol='?s%x' % entry)


def row(entry=0x401100, source='src/recovered/unit.cpp', score=61.5, size=0x40, anchor='00401000'):
    return dict(entry='%08X' % entry, name='Foo::Bar', bytes=size, source=source, anchor=anchor, symbol=None,
                references={}, non_exact=dict(score=score, date='2026-11-13', differs='registers'))


class Tree:
    def __init__(self, files, rows):
        self.dir = tempfile.TemporaryDirectory()
        self.root = Path(self.dir.name)
        for path, text in files.items():
            (self.root / path).parent.mkdir(parents=True, exist_ok=True)
            (self.root / path).write_text(text)
        (self.root / 'src/shared').mkdir(parents=True, exist_ok=True)
        (self.root / non_exact.MANIFEST).parent.mkdir(parents=True, exist_ok=True)
        (self.root / non_exact.MANIFEST).write_text(json.dumps(dict(about='', rows=rows)))
        self.catalog = dict(regions=[region(0x401000, 'src/recovered/unit.cpp'),
                                     region(0x401200, 'src/recovered/unit.cpp')])

    def problems(self):
        return non_exact.validate(self.root, self.catalog)


class GuardFormTests(unittest.TestCase):
    def test_blocks_and_nesting(self):
        text = '#ifdef RA2_NON_MATCHING\n#if X\nint a;\n#endif\n#endif\n#ifdef OTHER\n#endif\n'
        blocks, problems = non_exact.guarded_blocks(text)
        self.assertEqual(blocks, [(0, 4)])
        self.assertEqual(problems, [])

    def test_other_spellings_refuse(self):
        for text in ('#if defined(RA2_NON_MATCHING)\n#endif\n', '#ifndef RA2_NON_MATCHING\n#endif\n',
                     '#define RA2_NON_MATCHING\n', '#ifdef RA2_NON_MATCHING\n#else\n#endif\n',
                     '#ifdef RA2_NON_MATCHING\n#ifdef RA2_NON_MATCHING\n#endif\n#endif\n',
                     '#ifdef RA2_NON_MATCHING\nint a;\n'):
            self.assertTrue(non_exact.guarded_blocks(text)[1], text)

    def test_commented_directive_is_no_block(self):
        self.assertEqual(non_exact.guarded_blocks('/*\n#ifdef RA2_NON_MATCHING\n*/\n'), ([], []))

    def test_instruction_arrays_and_asm(self):
        for body in ('void f() { __asm { mov eax, 1 } }', 'void f() { _asm int 3 }', 'void f() { __emit 0x90 }',
                     'unsigned char code[] = {' + ', '.join(['0x90'] * 16) + '};',
                     'const char *code = "\\x55\\x8b\\xec\\x83\\xec\\x10\\x53\\x56";'):
            text = '#ifdef RA2_NON_MATCHING\n%s\n#endif\n' % body
            self.assertTrue(non_exact.block_problems(text, 0, 2), body)

    def test_plain_code_and_comments_pass(self):
        text = ('#ifdef RA2_NON_MATCHING\n// the original used __asm here\nint table[] = {1, 2, 3};\n'
                'int f() { return table[1] + 0x10; }\n#endif\n')
        self.assertEqual(non_exact.block_problems(text, 0, 4), [])


class ValidateTests(unittest.TestCase):
    def test_placed_draft_passes(self):
        tree = Tree({'src/recovered/unit.cpp': MODULE % MARK}, [row()])
        self.assertEqual(tree.problems(), [])

    def test_block_without_marker_refuses(self):
        tree = Tree({'src/recovered/unit.cpp': MODULE % '// a draft'}, [row()])
        self.assertTrue(any('without its marker' in p for p in tree.problems()))

    def test_marker_score_must_match_the_manifest(self):
        tree = Tree({'src/recovered/unit.cpp': MODULE % MARK}, [row(score=70.0)])
        self.assertTrue(any('score/date' in p for p in tree.problems()))

    def test_row_without_marker_refuses(self):
        tree = Tree({'src/recovered/unit.cpp': MODULE.replace('%s\n#ifdef RA2_NON_MATCHING', '#if 0')
                     .replace('#endif\n\n//', '#endif\n\n//', 1)}, [row()])
        self.assertTrue(any('0 marker' in p for p in tree.problems()))

    def test_guarded_function_claiming_acceptance_refuses(self):
        tree = Tree({'src/recovered/unit.cpp': MODULE % MARK}, [row()])
        tree.catalog['regions'].append(region(0x401100, 'src/recovered/unit.cpp'))
        self.assertTrue(any('claims acceptance' in p for p in tree.problems()))

    def test_overlapping_extent_refuses(self):
        tree = Tree({'src/recovered/unit.cpp': MODULE % MARK}, [row(size=0x180)])
        self.assertTrue(any('claims acceptance' in p for p in tree.problems()))

    def test_accepted_header_inside_a_guard_refuses(self):
        text = (MODULE % MARK).replace('// int Foo::Bar() (0x00401100)', '// int Foo::Qux() (0x00401200)\n'
                                       '// int Foo::Bar() (0x00401100)')
        text = text.replace('\n// int Foo::Qux() (0x00401200)\nint Foo::Qux()', '\nint Foo::Qux()')
        tree = Tree({'src/recovered/unit.cpp': text}, [row()])
        self.assertTrue(any('inside a RA2_NON_MATCHING block' in p for p in tree.problems()))

    def test_exact_score_refuses(self):
        tree = Tree({'src/recovered/unit.cpp': MODULE % MARK.replace('61.50', '100.00')}, [row(score=100.0)])
        self.assertTrue(any('exact draft' in p for p in tree.problems()))

    def test_asm_in_guarded_block_refuses(self):
        text = (MODULE % MARK).replace('return Baz() + 1;', '__asm { mov eax, 1 }')
        tree = Tree({'src/recovered/unit.cpp': text}, [row()])
        self.assertTrue(any('inline assembly' in p for p in tree.problems()))

    def test_guard_in_shared_header_checked(self):
        tree = Tree({'src/recovered/unit.cpp': MODULE % MARK,
                     'src/shared/foo.hpp': '#ifdef RA2_NON_MATCHING\nvoid g() { _emit 0x90 }\n#endif\n'}, [row()])
        self.assertTrue(any('foo.hpp' in p for p in tree.problems()))

    def test_shared_header_guard_holds_function_declarations_only(self):
        ok = ('class Foo {\n    int a;\n#ifdef RA2_NON_MATCHING\n    // a draft member\n'
              '    int Bar(struct Cell *cell, int const &n) const;\n    void Baz();\n#endif\n};\n')
        self.assertEqual(non_exact.header_block_problems(ok, 2, 6), [])
        for body in ('    int extra;', '    virtual void Bar();', '    void Bar() { }', '    typedef int T;',
                     '    enum { A };', '    int table[4];'):
            text = 'class Foo {\n#ifdef RA2_NON_MATCHING\n%s\n#endif\n};\n' % body
            self.assertTrue(non_exact.header_block_problems(text, 1, 3), body)

    def test_draft_only_file(self):
        own = 'src/recovered/own.cpp'
        text = MARK + '\n#ifdef RA2_NON_MATCHING\nint Foo::Bar() { return 3; }\n#endif\n'
        tree = Tree({'src/recovered/unit.cpp': MODULE.replace('%s\n', ''), own: text}, [row(source=own)])
        tree.files = None
        # The MODULE text without a marker still has its guarded block: drop it for this test.
        (tree.root / 'src/recovered/unit.cpp').write_text('// int Foo::Baz() (0x00401000)\nint Foo::Baz() { return 1; }\n')
        self.assertEqual(tree.problems(), [])
        self.assertEqual(non_exact.draft_only_sources(tree.root, tree.catalog), {own})
        (tree.root / own).write_text(text + 'int stray;\n')
        self.assertTrue(any('outside its' in p for p in tree.problems()))


class PlaceTests(unittest.TestCase):
    def test_insertion_in_address_order(self):
        text = (MODULE % MARK)
        at = non_exact.insertion_line(text, 0x401150, {0x401000, 0x401200})
        self.assertTrue(text.split('\n')[at].startswith('// int Foo::Qux()'))
        self.assertEqual(non_exact.insertion_line(text, 0x401300, {0x401000, 0x401200}), len(text.split('\n')) - 1)

    def test_clean_draft_moves_refs(self):
        body, refs = non_exact.clean_draft('#include "shared/foo.hpp"\n// @ref _x=0049A000\nint y;\n',
                                           '#include "shared/foo.hpp"\n')
        self.assertEqual(body, 'int y;')
        self.assertEqual(refs, {'_x': '0049A000'})

    def test_clean_draft_drops_unit_definitions_and_undefines_switches(self):
        draft = ('#define RA2_FOO_SWITCH\nint Foo::Baz()\n{\n    return 9;\n}\n\nint Foo::Bar()\n{\n    return Baz();\n}\n')
        body, _ = non_exact.clean_draft(draft, MODULE.replace('int Foo::Bar()', 'int Foo::Other()') % MARK)
        self.assertNotIn('return 9', body)
        self.assertIn('int Foo::Bar()', body)
        self.assertTrue(body.endswith('#undef RA2_FOO_SWITCH'))

    def test_prelude_block(self):
        text = non_exact.add_prelude(MODULE % MARK, ['#define RA2_FOO_SWITCH'])
        self.assertIn('#define RA2_FOO_SWITCH', text.split('#include')[0])
        tree = Tree({'src/recovered/unit.cpp': text}, [row()])
        self.assertEqual(tree.problems(), [])
        bad = text.replace('#define RA2_FOO_SWITCH', 'int stray;')
        (tree.root / 'src/recovered/unit.cpp').write_text(bad)
        self.assertTrue(any('prelude' in p for p in tree.problems()))

    def test_marker_round_trip(self):
        line = non_exact.marker_line('Foo::Bar', 0x401100, 61.5, '2026-11-13', 'registers')
        m = non_exact.MARKER.match(line)
        self.assertEqual((m.group('entry'), m.group('score'), m.group('differs')), ('00401100', '61.50', 'registers'))


class StatusTests(unittest.TestCase):
    def test_functions_with_source(self):
        treemap = [dict(entry=0x401000, bytes=10, status='exact'), dict(entry=0x401100, bytes=10, status='open'),
                   dict(entry=0x401200, bytes=10, status='open')]
        funcs = status.functions(treemap, set(), {}, {0x401100})
        self.assertEqual((funcs['done'], funcs['with_source'], funcs['total']), (1, 2, 3))

    def test_line(self):
        line = status.non_exact_line((2, 300, 55.5))
        self.assertIn('2 functions, 300 bytes, mean score 55.50', line)
        self.assertIn('no .text credit', line)


if __name__ == '__main__':
    unittest.main()
