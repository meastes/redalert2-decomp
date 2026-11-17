"""Type-aware member renames (member_rename.py), their apply_rename spec form and the native
re-key generated from a spec (native_rekey.py). Synthetic sources; no game payload."""
from pathlib import Path
import os
import sys
import tempfile
import unittest

REPOSITORY = next(parent for parent in Path(__file__).resolve().parents
                  if (parent / 'tools/original_target/member_rename.py').is_file())
sys.path.insert(0, str(REPOSITORY / 'tools/original_target'))
import member_rename
import native_rekey

HEADER = '''#ifndef A_HPP
#define A_HPP
class Base { public: int Count; void Get_Cell(); virtual void Draw(); };
class Derived : public Base { public: void Use(); };
class Other { public: void Get_Cell(); int Count; };
class Hider : public Base { public: void Get_Cell(); };
class Painter : public Base { public: virtual void Draw(); };
#endif
'''
UNIT = '''#include "shared/a.hpp"
// Base::Get_Cell (0x00401000) reads Count; a plain Get_Cell in a comment stays.
void Base::Get_Cell() { Count = 1; }
void Derived::Use() { Get_Cell(); this->Count = 2; }
void Other::Get_Cell() { Count = 3; }
void Hider::Get_Cell() { Base::Get_Cell(); }
void Free(Derived *d, Other *o, Hider *h) { d->Get_Cell(); o->Get_Cell(); h->Get_Cell(); d->Count = o->Count; }
'''


def have_libclang():
    return Path(member_rename.LIBCLANG).exists()


@unittest.skipUnless(have_libclang(), 'libclang (Command Line Tools) not installed')
class MemberRenameTests(unittest.TestCase):
    def setUp(self):
        self.folder = tempfile.TemporaryDirectory()
        self.root = Path(self.folder.name)
        (self.root / 'src/shared').mkdir(parents=True)
        (self.root / 'src/recovered').mkdir(parents=True)
        (self.root / 'src/shared/a.hpp').write_text(HEADER)
        (self.root / 'src/recovered/a.cpp').write_text(UNIT)
        self.saved, self.cwd = member_rename.REPOSITORY, os.getcwd()
        member_rename.REPOSITORY = self.root

    def tearDown(self):
        member_rename.REPOSITORY = self.saved
        os.chdir(self.cwd)
        self.folder.cleanup()

    def lines(self, result, path):
        text = (self.root / path).read_text()
        return [text.count('\n', 0, o) + 1 for o in result['edits'].get(path, [])]

    def test_renames_the_class_member_through_derived_types_only(self):
        result = member_rename.plan('Base', 'Get_Cell', 'Get_Position')
        # header: Base's declaration only (not Other's or Hider's)
        self.assertEqual(self.lines(result, 'src/shared/a.hpp'), [3])
        # unit: the X::name comment, the definition, Derived::Use, Hider's explicit Base::Get_Cell, d->Get_Cell
        self.assertEqual(self.lines(result, 'src/recovered/a.cpp'), [2, 3, 4, 6, 7])
        text = member_rename.apply_edits(UNIT, result['edits']['src/recovered/a.cpp'], 'Get_Cell', 'Get_Position')
        self.assertIn('o->Get_Cell(); h->Get_Cell()', text)
        self.assertIn('a plain Get_Cell in a comment stays', text)
        self.assertIn('void Other::Get_Cell()', text)
        self.assertEqual(result['manglings'], {'?Get_Cell@Base@@QAEXXZ': '?Get_Position@Base@@QAEXXZ'})

    def test_fields_rename_and_have_no_decorated_name(self):
        result = member_rename.plan('Base', 'Count', 'Number')
        self.assertEqual(self.lines(result, 'src/recovered/a.cpp'), [3, 4, 7])
        self.assertEqual(result['manglings'], {})

    def test_a_token_no_parse_explains_refuses(self):
        (self.root / 'src/recovered/b.cpp').write_text(
            '#include "shared/a.hpp"\n#ifdef NEVER\nvoid f(Base *b) { b->Get_Cell(); }\n#endif\n')
        with self.assertRaises(SystemExit) as caught:
            member_rename.plan('Base', 'Get_Cell', 'Get_Position')
        self.assertIn('no parse explains', str(caught.exception))
        self.assertIn('b.cpp:3', str(caught.exception))

    def test_uses_in_a_non_exact_draft_are_renamed_by_type(self):
        (self.root / 'src/recovered/d.cpp').write_text(
            '#include "shared/a.hpp"\n#ifdef RA2_NON_MATCHING\nvoid h(Derived *d, Other *o) { d->Get_Cell(); o->Get_Cell(); }\n#endif\n')
        result = member_rename.plan('Base', 'Get_Cell', 'Get_Position')
        text = (self.root / 'src/recovered/d.cpp').read_text()
        self.assertEqual([text.count('\n', 0, o) + 1 for o in result['edits']['src/recovered/d.cpp']], [3])
        edited = member_rename.apply_edits(text, result['edits']['src/recovered/d.cpp'], 'Get_Cell', 'Get_Position')
        self.assertIn('d->Get_Position(); o->Get_Cell();', edited)

    def test_a_use_inside_a_macro_refuses(self):
        (self.root / 'src/recovered/c.cpp').write_text(
            '#include "shared/a.hpp"\n#define CALL(p) (p)->Get_Cell()\nvoid g(Base *b) { CALL(b); }\n')
        with self.assertRaises(SystemExit):
            member_rename.plan('Base', 'Get_Cell', 'Get_Position')

    def test_a_use_in_a_macro_argument_is_renamed(self):
        (self.root / 'src/recovered/c.cpp').write_text(
            '#include "shared/a.hpp"\n#define TWICE(x) ((x) + (x))\nint g(Base *b, Other *o) { return TWICE(b->Count) + TWICE(o->Count); }\n')
        result = member_rename.plan('Base', 'Count', 'Number')
        text = (self.root / 'src/recovered/c.cpp').read_text()
        edited = member_rename.apply_edits(text, result['edits']['src/recovered/c.cpp'], 'Count', 'Number')
        self.assertIn('TWICE(b->Number) + TWICE(o->Count)', edited)

    def test_offsets_in_a_crlf_header_are_bytes(self):
        (self.root / 'src/shared/a.hpp').write_bytes(HEADER.replace('\n', '\r\n').encode())
        result = member_rename.plan('Base', 'Count', 'Number')
        data = (self.root / 'src/shared/a.hpp').read_bytes().decode('latin-1')
        self.assertEqual([data[o:o + 5] for o in result['edits']['src/shared/a.hpp']], ['Count'])

    def test_a_header_no_source_includes_does_not_refuse(self):
        (self.root / 'src/shared/orphan.hpp').write_text('struct Orphan { int Count; };\n')
        result = member_rename.plan('Base', 'Count', 'Number')
        self.assertNotIn('src/shared/orphan.hpp', result['edits'])

    def test_a_primed_batch_plans_as_single_renames_do(self):
        single = [member_rename.plan('Base', 'Get_Cell', 'Get_Position'), member_rename.plan('Base', 'Count', 'Number')]
        member_rename.prime(['Get_Cell', 'Count'], ['Get_Position', 'Number'])
        try:
            primed = [member_rename.plan('Base', 'Get_Cell', 'Get_Position'), member_rename.plan('Base', 'Count', 'Number')]
            with self.assertRaises(SystemExit):
                member_rename.plan('Base', 'Count', 'Get_Cell')   # a clash the primed parse must still see
        finally:
            member_rename.prime([])
        self.assertEqual([r['edits'] for r in single], [r['edits'] for r in primed])
        self.assertEqual([r['manglings'] for r in single], [r['manglings'] for r in primed])

    def test_padding_in_a_stand_in_view_keeps_its_name(self):
        (self.root / 'src/shared/v.hpp').write_text(
            '#ifdef VIEW\nclass Base { public: char pad_00[4]; char Count[4]; };\n#else\n#include "shared/a.hpp"\n#endif\n')
        (self.root / 'src/recovered/v.cpp').write_text(
            '#define VIEW\n#include "shared/v.hpp"\nint n = sizeof(((Base *)0)->Count);\n')
        result = member_rename.plan('Base', 'Count', 'Number')
        self.assertNotIn('src/shared/v.hpp', result['edits'])
        self.assertNotIn('src/recovered/v.cpp', result['edits'])
        self.assertEqual(self.lines(result, 'src/recovered/a.cpp'), [3, 4, 7])

    def test_a_one_byte_span_in_a_stand_in_keeps_its_name(self):
        (self.root / 'src/shared/r.hpp').write_text('#ifndef R_HPP\n#define R_HPP\nclass Rec { public: char unknown_1[1]; int k; };\n#endif\n')
        for n in ('r1', 'r2', 'r3'):
            (self.root / ('src/recovered/%s.cpp' % n)).write_text('#include "shared/r.hpp"\nint %s(Rec *r) { return r->unknown_1[0]; }\n' % n)
        (self.root / 'src/shared/s.hpp').write_text('#ifndef R_HPP\nclass Rec { public: char unknown_1[1]; };\n#endif\n')
        (self.root / 'src/recovered/s.cpp').write_text('#include "shared/s.hpp"\nint s(Rec *r) { return sizeof(r->unknown_1); }\n')
        result = member_rename.plan('Rec', 'unknown_1', 'IsPowered')
        self.assertIn('src/shared/r.hpp', result['edits'])
        self.assertEqual(len(result['edits']['src/recovered/r1.cpp']), 1)
        self.assertNotIn('src/shared/s.hpp', result['edits'])
        self.assertNotIn('src/recovered/s.cpp', result['edits'])

    def test_a_new_name_the_hierarchy_already_has_refuses(self):
        with self.assertRaises(SystemExit) as caught:
            member_rename.plan('Base', 'Get_Cell', 'Use')
        self.assertIn('already a member of Derived', str(caught.exception))
        # a sibling's member of that name does not clash
        member_rename.plan('Derived', 'Use', 'Draw_It')

    def test_hiding_a_base_member_only_when_allowed(self):
        with self.assertRaises(SystemExit):
            member_rename.plan('Derived', 'Use', 'Count')
        result = member_rename.plan('Derived', 'Use', 'Count', allow_hiding=True)
        self.assertEqual(self.lines(result, 'src/shared/a.hpp'), [4])
        # never a virtual base member (clang refuses a static or plain member hiding it)
        with self.assertRaises(SystemExit):
            member_rename.plan('Derived', 'Use', 'Draw', allow_hiding=True)

    def test_an_overridden_virtual_refuses(self):
        with self.assertRaises(SystemExit) as caught:
            member_rename.plan('Base', 'Draw', 'Render')
        self.assertIn('overridden in Painter', str(caught.exception))

    def test_a_family_rename_takes_the_base_and_every_override(self):
        result = member_rename.plan('Base', 'Draw', 'Render', family=True)
        self.assertEqual(result['owners'], ['Base', 'Painter'])
        self.assertEqual(self.lines(result, 'src/shared/a.hpp'), [3, 7])
        with self.assertRaises(SystemExit):
            member_rename.plan('Painter', 'Draw', 'Render', family=True)   # an override: rename the base family
        with self.assertRaises(SystemExit):
            member_rename.plan('Base', 'Get_Cell', 'Render', family=True)  # not virtual


class NativeRekeyTests(unittest.TestCase):
    def apply(self, spec, text):
        rules, review = native_rekey.rules(spec)
        for _, pattern, replacement in rules:
            text = pattern.sub(replacement, text)
        return text, review

    def test_identifier_of_decorated_names(self):
        self.assertEqual(native_rekey.identifier('?Get_Cell@TagClass@@QBE?AVCell@@XZ'), ('Get_Cell', ['TagClass']))
        self.assertEqual(native_rekey.identifier('?unknown_8523e0@@3HA'), ('unknown_8523e0', []))
        self.assertEqual(native_rekey.identifier('_unknown_dragging_b25f2c'), ('unknown_dragging_b25f2c', []))
        self.assertIsNone(native_rekey.identifier('??0TagClass@@QAE@XZ'))

    def test_free_names_every_spelling(self):
        spec = {'renames': {'?unknown_center_window_737080@@YAXPAUHWND__@@0@Z':
                            '?Center_Window_Within_Window@@YAXPAUHWND__@@0@Z',
                            '?unknown_8523e0@@3HA': '?VideoModeWidth@@3HA'}}
        text = ('0x00737080\t?unknown_center_window_737080@@YAXPAUHWND__@@0@Z\n'
                'extern "C" void x() __asm__("__Z28unknown_center_window_737080P6HWND__S0_");\n'
                '__asm__("_unknown_8523e0"); extern int unknown_8523e0; int my_unknown_8523e0x;\n'
                '__ZN8TagClass28unknown_center_window_737080Ev\n')
        out, _ = self.apply(spec, text)
        self.assertIn('?Center_Window_Within_Window@@YAXPAUHWND__@@0@Z', out)
        self.assertIn('__Z27Center_Window_Within_WindowP6HWND__S0_', out)
        self.assertIn('"_VideoModeWidth"', out)
        self.assertIn('extern int VideoModeWidth;', out)
        self.assertIn('my_unknown_8523e0x', out)
        # a member of the same spelling in another scope is not the free function
        self.assertIn('__ZN8TagClass28unknown_center_window_737080Ev', out)

    def test_member_renames_scoped_spellings_and_review_member_access(self):
        spec = {'member_renames': [{'class': 'TagClass', 'old': 'Get_Cell', 'new': 'Get_Position'}]}
        out, _ = self.apply(spec, '__ZNK8TagClass8Get_CellEv TagClass::Get_Cell() ?Get_Cell@TagClass@@QBE?AVCell@@XZ '
                                  'OtherClass::Get_Cell ?Get_Cell@OtherClass@@')
        self.assertEqual(out, '__ZNK8TagClass12Get_PositionEv TagClass::Get_Position() '
                              '?Get_Position@TagClass@@QBE?AVCell@@XZ OtherClass::Get_Cell ?Get_Cell@OtherClass@@')

    def test_offsetof_spellings_follow_a_member_rename(self):
        spec = {'member_renames': [{'class': 'DisplayClass', 'old': 'unknown_11a0', 'new': 'PendingObjectPtr'}]}
        out, _ = self.apply(spec, 'offsetof(DisplayClass, unknown_11a0) offsetof(MapView, unknown_11a0)')
        self.assertEqual(out, 'offsetof(DisplayClass, PendingObjectPtr) offsetof(MapView, unknown_11a0)')

    def test_a_merge_drops_self_aliases_and_repeated_rows(self):
        text = '# header\nalias\ttarget\n_Movies\t_Movies\t0x00A71050\tdata\n_A\t_B\t1\n_A\t_B\t1\n'
        self.assertEqual(native_rekey.merged('native/data/bss_aliases.tsv', text),
                         '# header\nalias\ttarget\n_A\t_B\t1\n')
        cpp = '__asm__(".globl _Movies\\n.set _Movies, _Movies\\n");  // same\nint x;\n'
        self.assertEqual(native_rekey.merged('native/data/bss_globals.cpp', cpp), 'int x;\n')

    def test_signature_only_rename_is_left_for_review(self):
        _, review = self.apply({'renames': {'?F@@YAXH@Z': '?F@@YAXI@Z'}}, '')
        self.assertTrue(any('only the signature' in r for r in review))


class SpecFormTests(unittest.TestCase):
    """apply_rename's spec forms: member_renames -> catalog renames; function_renames ->
    overload_renames plus a whole-word source rule."""
    def catalog(self):
        return dict(regions=[
            dict(entry=0x6B3D80, symbol='?Get_Cell@TagClass@@QBE?AVCell@@XZ', symbol_prefix='?Get_Cell@TagClass@@',
                 references={'?Get_Cell@TagClass@@': 0x6B3D80}, relocations=[]),
            dict(entry=0x737060, symbol='?unknown_center_on_owner_737060@@YIXPAUHWND__@@@Z',
                 symbol_prefix='?unknown_center_on_owner_737060@@', references={}, relocations=[]),
            dict(entry=0x737080, symbol='?unknown_center_window_737080@@YIXPAUHWND__@@0@Z',
                 symbol_prefix='?unknown_center_window_737080@@', references={}, relocations=[])])

    def test_member_renames_take_every_decorated_spelling(self):
        import apply_rename
        saved = member_rename.plan
        member_rename.plan = lambda *a, **k: dict(edits={'src/shared/tag.hpp': [10]}, overload=False,
                                                   manglings={'?Get_Cell@TagClass@@QBE?AVCell@@XZ': '?Get_Position@TagClass@@QBE?AVCell@@XZ'})
        try:
            renames, overloads = {}, {}
            edits = apply_rename.member_plans({'member_renames': [{'class': 'TagClass', 'old': 'Get_Cell', 'new': 'Get_Position'}]},
                                              self.catalog(), renames, overloads)
        finally:
            member_rename.plan = saved
        self.assertEqual(edits, {'src/shared/tag.hpp': [(10, 'Get_Cell', 'Get_Position')]})
        self.assertEqual(renames['?Get_Cell@TagClass@@'], '?Get_Position@TagClass@@')
        self.assertEqual(renames['?Get_Cell@TagClass@@QBE?AVCell@@XZ'], '?Get_Position@TagClass@@QBE?AVCell@@XZ')
        self.assertEqual(overloads, {})

    def test_function_renames_let_two_functions_take_one_name(self):
        import apply_rename
        renames, overloads = {}, {}
        spec = {'function_renames': [
            {'old': 'unknown_center_window_737080', 'new': 'Center_Window_Within_Window', 'at': '00737080'},
            {'old': 'unknown_center_on_owner_737060', 'new': 'Center_Window_Within_Window', 'at': '00737060'}]}
        rules = apply_rename.function_rules(spec, self.catalog(), renames, overloads)
        self.assertEqual(overloads['?unknown_center_on_owner_737060@@YIXPAUHWND__@@@Z'],
                         ('?Center_Window_Within_Window@@YIXPAUHWND__@@@Z', 0x737060))
        self.assertEqual(overloads['?unknown_center_window_737080@@YIXPAUHWND__@@0@Z'],
                         ('?Center_Window_Within_Window@@YIXPAUHWND__@@0@Z', 0x737080))
        text = 'void unknown_center_on_owner_737060(HWND w) { unknown_center_window_737080(w, x); }\n' \
               '// @ref ?unknown_center_window_737080@@YIXPAUHWND__@@0@Z=00737080\n'
        for pattern, replacement in rules:
            text = pattern.sub(replacement, text)
        self.assertIn('void Center_Window_Within_Window(HWND w) { Center_Window_Within_Window(w, x); }', text)
        self.assertIn('@ref ?unknown_center_window_737080@@', text)     # the decorated name follows overload_renames

    def test_global_renames_take_every_decorated_spelling_and_the_identifier(self):
        import apply_rename
        catalog = dict(regions=[dict(entry=0x401000, symbol='?f@@YAXXZ', symbol_prefix='?f@@',
                                     references={'?unknown_8523e0@@3HA': 0x8523E0, '_unknown_8523e4': 0x8523E4},
                                     relocations=[])])
        renames = {}
        saved = apply_rename.SYMBOLS
        apply_rename.SYMBOLS = Path(tempfile.mkdtemp()) / 'symbols.json'
        apply_rename.SYMBOLS.write_text('{"symbols": []}')
        try:
            rules = apply_rename.global_rules({'global_renames': [{'old': 'unknown_8523e0', 'new': 'VideoModeWidth'},
                                                                  {'old': 'unknown_8523e4', 'new': 'VideoModeHeight'}]},
                                              catalog, renames, {})
        finally:
            apply_rename.SYMBOLS = saved
        self.assertEqual(renames, {'?unknown_8523e0@@3HA': '?VideoModeWidth@@3HA', '_unknown_8523e4': '_VideoModeHeight'})
        text = 'extern int unknown_8523e0; x = unknown_8523e4 + my_unknown_8523e0;'
        for pattern, replacement in rules:
            text = pattern.sub(replacement, text)
        self.assertEqual(text, 'extern int VideoModeWidth; x = VideoModeHeight + my_unknown_8523e0;')

    def test_bare_prefix_keys_follow_both_forms(self):
        import apply_rename
        catalog = dict(regions=[dict(entry=0x5C7570, symbol='?Unknown_005c7570@@YIXPAUHWND__@@@Z',
                                     symbol_prefix='?Unknown_005c7570', references={'?Unknown_005c7570': 0x5C7570},
                                     relocations=[])])
        saved = apply_rename.SYMBOLS
        apply_rename.SYMBOLS = Path(tempfile.mkdtemp()) / 'symbols.json'
        apply_rename.SYMBOLS.write_text('{"symbols": []}')
        try:
            renames = {}
            apply_rename.global_rules({'global_renames': [{'old': 'Unknown_005c7570', 'new': 'Refresh_Dialog'}]},
                                      catalog, renames, {})
            self.assertEqual(renames, {'?Unknown_005c7570@@YIXPAUHWND__@@@Z': '?Refresh_Dialog@@YIXPAUHWND__@@@Z',
                                       '?Unknown_005c7570': '?Refresh_Dialog'})
            # as an overload: the bare prefix bound at the row's address becomes the new full name
            overloads = {}
            apply_rename.function_rules({'function_renames': [{'old': 'Unknown_005c7570', 'new': 'Refresh_Dialog',
                                                               'at': '005c7570'}]}, catalog, {}, overloads)
            self.assertEqual(apply_rename.overload_name(overloads, '?Unknown_005c7570', 0x5C7570),
                             '?Refresh_Dialog@@YIXPAUHWND__@@@Z')
        finally:
            apply_rename.SYMBOLS = saved

    def test_an_existing_bare_prefix_of_the_new_name_becomes_the_other_overloads_full_name(self):
        import apply_rename
        catalog = dict(regions=[
            dict(entry=0x737060, symbol='?unknown_center_on_owner_737060@@YIXPAUHWND__@@@Z',
                 symbol_prefix='?unknown_center_on_owner_737060@@', references={}, relocations=[]),
            dict(entry=0x737080, symbol='?Center_Window_Within_Window@@YIXPAUHWND__@@0@Z',
                 symbol_prefix='?Center_Window_Within_Window@@',
                 references={'?Center_Window_Within_Window@@': 0x737080}, relocations=[])])
        scoped, overloads = {}, {}
        apply_rename.function_rules({'function_renames': [{'old': 'unknown_center_on_owner_737060',
                                                           'new': 'Center_Window_Within_Window', 'at': '00737060'}]},
                                    catalog, {}, overloads, scoped)
        self.assertEqual(scoped, {'?Center_Window_Within_Window@@': ('?Center_Window_Within_Window@@YIXPAUHWND__@@0@Z',
                                                                     {0x737080})})

    def test_function_renames_refuse_a_member_or_a_missing_row(self):
        import apply_rename
        with self.assertRaises(SystemExit):
            apply_rename.function_rules({'function_renames': [{'old': 'Get_Cell', 'new': 'X', 'at': '006b3d80'}]},
                                        self.catalog(), {}, {})
        with self.assertRaises(SystemExit):
            apply_rename.function_rules({'function_renames': [{'old': 'f', 'new': 'g', 'at': '00400000'}]},
                                        self.catalog(), {}, {})


class CompilerRekeyTests(unittest.TestCase):
    """native_rekey.compiler_rekey: the native compiler types member uses in native-only code."""
    def test_renames_each_reported_use_until_the_build_passes(self):
        folder = tempfile.TemporaryDirectory()
        saved = native_rekey.REPOSITORY
        native_rekey.REPOSITORY = Path(folder.name)
        try:
            draft = Path(folder.name) / 'native/drafts/0045f710.cpp'
            draft.parent.mkdir(parents=True)
            draft.write_text('void f(FootClass *f) {\n    if (f->unknown_7d && other->unknown_7d) g(offsetof(X, unknown_7d));\n}\n')
            outputs = iter([
                (1, "%s:2:12: error: no member named 'unknown_7d' in 'FootClass'\n" % draft),
                (0, '')])
            made = native_rekey.compiler_rekey(
                {'member_renames': [{'class': 'ObjectClass', 'old': 'unknown_7d', 'new': 'IsActive'}]},
                None, build=lambda: next(outputs))
            self.assertEqual(made, [('native/drafts/0045f710.cpp', 2, 'unknown_7d', 'FootClass')])
            self.assertIn('f->IsActive && other->unknown_7d', draft.read_text())
        finally:
            native_rekey.REPOSITORY = saved
            folder.cleanup()

    def test_an_error_the_renames_do_not_explain_refuses(self):
        with self.assertRaises(SystemExit):
            native_rekey.compiler_rekey({'member_renames': [{'class': 'A', 'old': 'x', 'new': 'y'}]}, None,
                                        build=lambda: (1, "/r/native/a.cpp:1:1: error: expected ';'\n"))

    def test_a_use_in_a_generated_copy_of_src_refuses(self):
        with self.assertRaises(SystemExit):
            native_rekey.compiler_rekey({'member_renames': [{'class': 'A', 'old': 'x', 'new': 'y'}]}, None,
                                        build=lambda: (1, "/r/build/native-play/gen/overlay/shared/a.hpp:3:5: "
                                                          "error: use of undeclared identifier 'x'\n"))


class FixupsFollowTests(unittest.TestCase):
    def test_a_fixup_takes_only_the_renames_its_source_took(self):
        import json
        folder = tempfile.TemporaryDirectory()
        saved = native_rekey.REPOSITORY
        native_rekey.REPOSITORY = Path(folder.name)
        try:
            root = Path(folder.name)
            (root / 'src/shared').mkdir(parents=True)
            (root / 'native/port').mkdir(parents=True)
            # the source after the batch: the class's field renamed, a view's padding of that spelling not
            (root / 'src/shared/house_screen.hpp').write_text(
                '    bool WarFactoryInfiltrated;   // +0x207\n    char unknown_208[0x230 - 0x208];\n')
            fixups = {'fixups': [{'source': 'src/shared/house_screen.hpp',
                                  'old': '    bool unknown_207;   // +0x207\n    char unknown_208[0x230 - 0x208];\n',
                                  'new': '    bool unknown_207;   // arm64\n    char unknown_208[0x2f0 - 0x2c8];\n'}]}
            (root / 'native/port/fixups.json').write_text(json.dumps(fixups))
            spec = {'member_renames': [{'class': 'HouseClass', 'old': 'unknown_207', 'new': 'WarFactoryInfiltrated'},
                                       {'class': 'TechnoClass', 'old': 'unknown_208', 'new': 'TemporalImUsing'}]}
            self.assertEqual(native_rekey.fixups_follow(spec), ['src/shared/house_screen.hpp'])
            entry = json.loads((root / 'native/port/fixups.json').read_text())['fixups'][0]
            self.assertIn('bool WarFactoryInfiltrated;', entry['old'])
            self.assertIn('char unknown_208[0x230', entry['old'])
            self.assertIn('bool WarFactoryInfiltrated;   // arm64', entry['new'])
            self.assertIn('char unknown_208[0x2f0', entry['new'])
        finally:
            native_rekey.REPOSITORY = saved
            folder.cleanup()


if __name__ == '__main__':
    unittest.main()
