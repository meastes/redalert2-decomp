"""refile_unit.py (Route A) contracts on synthetic text and catalogs; no game bytes, no compiler."""
import json
import pathlib
import sys
import tempfile
import unittest

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parents[1] / 'tools' / 'original_target'))
import apply_rename
import module_check
import refile_unit

OPTIONS = ['/O2', '/Ob2', '/G5', '/Gy']


def row(entry, population, source, order=None, **extra):
    base = dict(entry=entry, population=population, source=source, source_order=order or entry,
                symbol='?F%x@@YAXXZ' % entry, symbol_prefix='?F%x@@' % entry, options=list(OPTIONS),
                body_bytes=16, references={}, relocations=[])
    base.update(extra)
    return base


A = ('// first\n#define RA2_SWITCH_A   // why\n#include "shared/a.hpp"\n#include "shared/b.hpp"\n'
     '// @ref ?G@@3HA=00800000\nint F1() { return 1; }\n')
B = ('// seed-generated: tools/original_target/regenerate_seeds.py may rewrite this file;\n'
     '// delete this line when editing it by hand.\n'
     '#include "shared/b.hpp"\n#define RA2_SWITCH_B\n#include "shared/c.hpp"\n'
     '#ifndef RA2_LOCAL\n#define RA2_LOCAL\n#include "shared/guarded.hpp"\n#endif\n'
     '#pragma auto_inline(off)\nint F2() { return 2; }\n#pragma auto_inline(on)\n')
C = '#define RA2_SWITCH_A\n#include "shared/a.hpp"\n#include "shared/d.hpp"\nint F3() { return 3; }\n'
FILES = [(0x1000, 'src/recovered/a_00001000.cpp', A), (0x2000, 'src/recovered/b_00002000.cpp', B),
         (0x3000, 'src/recovered/c_00003000.cpp', C)]


class SplitLinesTests(unittest.TestCase):
    def test_kinds_and_depth(self):
        kinds = [k for k, _ in refile_unit.split_lines(B)]
        self.assertEqual(kinds.count('marker'), 2)
        self.assertEqual(kinds.count('include'), 2)      # the guarded include stays where it is
        self.assertEqual(kinds.count('define'), 1)
        self.assertEqual([k for k, _ in refile_unit.split_lines(A)].count('ref'), 1)

    def test_continued_define_is_one_line(self):
        lines = refile_unit.split_lines('#define M(a) \\\n    ((a) + 1)\nint x;\n')
        self.assertEqual(lines[0], ('define', '#define M(a) \\\n    ((a) + 1)'))
        self.assertEqual(lines[1], ('other', 'int x;'))


class PourTests(unittest.TestCase):
    def test_one_block_in_original_order_without_repeats(self):
        text, info = refile_unit.pour(FILES)
        self.assertEqual(info['blocks'], [(0x1000, [
            '#define RA2_SWITCH_A   // why', '#include "shared/a.hpp"', '#include "shared/b.hpp"',
            '#define RA2_SWITCH_B', '#include "shared/c.hpp"', '#include "shared/d.hpp"'])])
        self.assertEqual(info['switches'], ['RA2_SWITCH_A', 'RA2_SWITCH_B', 'RA2_LOCAL'])
        self.assertNotIn('@ref', text)
        self.assertNotIn('seed-generated', text)
        # bodies in address order after the block; guarded lines and pragmas in place
        self.assertLess(text.index('shared/d.hpp'), text.index('int F1()'))
        self.assertLess(text.index('int F1()'), text.index('#ifndef RA2_LOCAL'))
        self.assertLess(text.index('#pragma auto_inline(off)'), text.index('int F2()'))
        self.assertLess(text.index('int F2()'), text.index('int F3()'))
        self.assertEqual(text.count('#include "shared/a.hpp"'), 1)

    def test_a_held_row_keeps_later_lines_behind_it(self):
        text, info = refile_unit.pour(FILES, held={0x2000})
        self.assertEqual([first for first, _ in info['blocks']], [0x1000, 0x3000])
        self.assertEqual(info['blocks'][1][1], ['#include "shared/d.hpp"'])
        self.assertLess(text.index('int F2()'), text.index('shared/d.hpp'))
        self.assertLess(text.index('shared/c.hpp'), text.index('int F1()'))

    def test_in_place_files_keep_their_lines(self):
        text, info = refile_unit.pour(FILES, held={0x1000}, in_place={0x1000})
        self.assertLess(text.index('// first'), text.index('#define RA2_SWITCH_A'))
        self.assertLess(text.index('int F1()'), text.index('#define RA2_SWITCH_B'))

    def test_inplace_form_only_drops_repeats(self):
        text, info = refile_unit.pour(FILES, form='inplace')
        self.assertEqual(info['blocks'], [])
        self.assertEqual(text.count('#include "shared/b.hpp"'), 1)
        self.assertEqual(text.count('#define RA2_SWITCH_A'), 1)
        self.assertLess(text.index('int F2()'), text.index('shared/d.hpp'))

    def test_verbatim_form_keeps_every_directive(self):
        text, _ = refile_unit.pour(FILES, form='verbatim')
        self.assertEqual(text.count('#include "shared/b.hpp"'), 2)
        self.assertEqual(text.count('#define RA2_SWITCH_A'), 2)

    def test_a_define_after_undef_is_kept(self):
        files = [(1, 'a', '#define X 1\nint a;\n'), (2, 'b', '#undef X\n#define X 1\nint b;\n')]
        text, _ = refile_unit.pour(files, form='inplace')
        self.assertEqual(text.count('#define X 1'), 2)

    def test_chunks_are_contiguous_in_the_module(self):
        text, info = refile_unit.pour(FILES)
        for chunk in info['chunks'].values():
            self.assertIn(chunk, text)


class AfterMissTests(unittest.TestCase):
    ENTRIES = [0x1000, 0x2000, 0x3000]

    def test_a_fresh_miss_is_held(self):
        self.assertEqual(refile_unit.after_miss('hoist', set(), set(), {0x2000}, self.ENTRIES),
                         ('hoist', {0x2000}, set(), set()))

    def test_a_held_row_that_still_differs_puts_the_files_up_to_it_in_place(self):
        self.assertEqual(refile_unit.after_miss('hoist', {0x2000}, set(), {0x2000}, self.ENTRIES),
                         ('hoist', {0x2000}, {0x1000, 0x2000}, set()))

    def test_stalled_holding_reaches_the_plainer_forms(self):
        # Every file already in place and the same rows still differ: nothing grows, so the
        # loop must leave the gathered form instead of repeating it (unit globals).
        everything = set(self.ENTRIES)
        form, held, in_place, flipped = refile_unit.after_miss('hoist', {0x3000}, everything, {0x3000}, self.ENTRIES)
        self.assertEqual((form, flipped), ('inplace', set()))
        form, held, in_place, flipped = refile_unit.after_miss(form, held, in_place, {0x3000}, self.ENTRIES)
        self.assertEqual((form, flipped), ('verbatim', set()))

    def test_rows_that_differ_in_the_canonical_text_are_flipped_and_the_rest_start_again(self):
        self.assertEqual(refile_unit.after_miss('verbatim', {0x3000}, set(self.ENTRIES), {0x3000}, self.ENTRIES),
                         ('hoist', set(), set(), {0x3000}))
        self.assertEqual(refile_unit.after_miss('verbatim', set(), set(), {0x3000}, self.ENTRIES, start='inplace')[0],
                         'inplace')

    def test_the_walk_always_ends(self):
        # Whatever keeps differing, the forms run out and the rows flip within a bounded number of steps.
        form, held, in_place, steps = 'hoist', set(), set(), 0
        while True:
            steps += 1
            form, held, in_place, flipped = refile_unit.after_miss(form, held, in_place, {0x1000, 0x3000}, self.ENTRIES)
            if flipped:
                break
            self.assertLess(steps, 8)
        self.assertEqual(flipped, {0x1000, 0x3000})

    def test_flipped_cause_carries_the_comparator_reason(self):
        cause = refile_unit.flipped_cause(dict(entry='00003000', status='comparison-refused',
                                               reason='Function symbol ambiguous/missing: ?Clear@X@@UAEXXZ'))
        self.assertIn('not exact in the unit\'s text in any form', cause)
        self.assertIn('Function symbol ambiguous/missing: ?Clear@X@@UAEXXZ', cause)
        self.assertIn('differs', refile_unit.flipped_cause(dict(entry='00003000')))


class RefusalReasonTests(unittest.TestCase):
    def test_compile_failure_gives_the_first_error_without_local_paths(self):
        tail = ('Traceback (most recent call last):\n  File "/Users/someone/repo/tools/x.py", line 3\n'
                'ValueError: Compile failed: /Users/someone/repo/.claude/worktrees/w/build/refile/u/attempt-01/compile.log\n')
        log = ('Z:\\Users\\someone\\repo\\src\\shared/bullet.hpp(280) : warning C4554: check operator precedence\n'
               'Z:\\Users\\someone\\repo\\src\\shared/ghidra_types.hpp(29) : error C2371: \'DWORD\' : redefinition\n'
               'accepted.cpp(12) : error C2065: \'Max\' : undeclared identifier\n')
        seen = []
        reason = refile_unit.refusal_reason(tail, lambda path: seen.append(path) or log)
        self.assertEqual(seen, ['/Users/someone/repo/.claude/worktrees/w/build/refile/u/attempt-01/compile.log'])
        self.assertEqual(reason, "compile failed: ghidra_types.hpp(29) : error C2371: 'DWORD' : redefinition")

    def test_no_path_survives_in_any_reason(self):
        for tail, log in (('ValueError: Compile failed: /Users/someone/repo/build/compile.log', ''),
                          ('ValueError: Region 00401000 differs in /Users/someone/repo/build/out/a.obj', None),
                          ('refused: C:\\Users\\someone\\repo\\build\\x.obj missing', None), ('', None)):
            reason = refile_unit.refusal_reason(tail, lambda path: log)
            self.assertNotIn('someone', reason)
            self.assertNotIn('/Users/', reason)
            self.assertNotIn('Users', reason)
            self.assertNotIn('worktrees', reason)
            self.assertTrue(reason)
        self.assertEqual(refile_unit.refusal_reason('ValueError: Compile failed: /a/b/compile.log', lambda path: ''),
                         'compile failed')
        self.assertEqual(refile_unit.refusal_reason('ValueError: Region 00401000 differs in /a/b/out/a.obj'),
                         'ValueError: Region 00401000 differs in a.obj')


class ScrubCauseTests(unittest.TestCase):
    WORKTREE = '/Users/someone/Code/repo/.claude/worktrees/agent-1/build/refile/u/attempt-01/out/a.obj'

    def test_absolute_paths_lose_their_folders_and_relative_paths_stay(self):
        self.assertEqual(refile_unit.scrub_cause('Unsupported section .CRT$XCU #2 in ' + self.WORKTREE),
                         'Unsupported section .CRT$XCU #2 in a.obj')
        self.assertEqual(refile_unit.scrub_cause('in Z:\\Users\\someone\\repo\\src\\shared/x.hpp(29) : error C2371'),
                         'in x.hpp(29) : error C2371')
        for kept in ('already in the module source src/recovered/unit.cpp',
                     'held by population-42 under another option set (/O1 /Ob2 /G5 /Gy)',
                     "frozen in another population's inventory pass (population-3817)",
                     'its file sets VECTOR_IMPLEMENT_* before shared/vector_class.hpp; and/or nothing'):
            self.assertEqual(refile_unit.scrub_cause(kept), kept)

    def test_every_recorded_residual_cause_is_scrubbed(self):
        row = dict(entry=0x1000, body_bytes=3, population='population-1', source='src/recovered/a.cpp')
        for cause in ('the unit does not compile (attempt 1: ValueError: Compile failed: %s)' % self.WORKTREE,
                      'not exact: ' + self.WORKTREE, 'C:\\Users\\someone\\x\\y.log refused'):
            recorded = refile_unit.residual_row(row, 'flipped', cause)['cause']
            self.assertNotIn('/Users/', recorded)
            self.assertNotIn('Users', recorded)
            self.assertNotIn('worktrees', recorded)
            self.assertNotIn('someone', recorded)
        miss = dict(entry='00001000', status='comparison-refused', reason='No symbol in ' + self.WORKTREE)
        self.assertNotIn('/Users/', refile_unit.residual_row(row, 'flipped', refile_unit.flipped_cause(miss))['cause'])

    def test_restate_rewrites_only_route_a_causes_that_hold_a_path(self):
        modules = {
            'u': dict(route='A', residuals=[dict(entry='00001000', cause='refused in ' + self.WORKTREE),
                                            dict(entry='00002000', cause='frozen in another population (population-2)')],
                      held_rows=[dict(entry='00003000', cause='differs when the lines of later files are gathered')]),
            'old': dict(residuals=[dict(entry='00004000', cause='note ' + self.WORKTREE)])}
        changed = refile_unit.restate_causes(modules)
        self.assertEqual([(u, e, new) for u, e, _, new in changed], [('u', '00001000', 'refused in a.obj')])
        self.assertEqual(modules['u']['residuals'][0]['cause'], 'refused in a.obj')
        self.assertEqual(refile_unit.restate_causes(modules), [])


class ManifestLocalPathTests(unittest.TestCase):
    def test_a_local_path_in_a_manifest_is_found(self):
        import status
        with tempfile.TemporaryDirectory() as folder:
            root = pathlib.Path(folder)
            (root / 'manifests/modules').mkdir(parents=True)
            (root / 'manifests/clean.json').write_text(json.dumps(dict(cause='src/recovered/a.cpp; /O2 /Ob2; and/or')))
            (root / 'manifests/modules/home.json').write_text(json.dumps(dict(cause='in /Users/someone/repo/build/a.obj')))
            (root / 'manifests/modules/tree.json').write_text(json.dumps(dict(cause='x .claude/worktrees/agent-1/y')))
            (root / 'manifests/modules/win.json').write_text(json.dumps(dict(cause='Z:\\Users\\someone\\a.hpp(2)')))
            self.assertEqual([name for name, _ in status.manifest_local_paths(root)],
                             ['manifests/modules/home.json', 'manifests/modules/tree.json', 'manifests/modules/win.json'])

    def test_the_tracked_manifests_carry_no_absolute_local_path(self):
        import status
        self.assertEqual(status.manifest_local_paths(), [])


class ConfigAndNewlineTests(unittest.TestCase):
    def test_config_default_honours_RA2_CONFIG(self):
        import catalog_io
        root = pathlib.Path('/repo')
        self.assertEqual(catalog_io.config_path(root, {}), root / 'private/original-target.local.json')
        self.assertEqual(catalog_io.config_path(root, {'RA2_CONFIG': ''}), root / 'private/original-target.local.json')
        self.assertEqual(catalog_io.config_path(root, {'RA2_CONFIG': 'C:/ra2/private/original-target.win.json'}),
                         pathlib.Path('C:/ra2/private/original-target.win.json'))

    def test_every_command_takes_its_config_default_from_RA2_CONFIG(self):
        import os
        from unittest import mock
        seen = {}

        def fake_run(unit, form, joins, attempts, config, name=None, keep=None):
            seen['config'] = config
        with mock.patch.dict(os.environ, {'RA2_CONFIG': '/somewhere/original-target.win.json'}), \
                mock.patch.object(refile_unit, 'run', fake_run), mock.patch.object(sys, 'argv', ['refile_unit.py', 'run', 'u']):
            refile_unit.main()
        self.assertEqual(seen['config'], pathlib.Path('/somewhere/original-target.win.json'))
        with mock.patch.dict(os.environ, {'RA2_CONFIG': '/somewhere/original-target.win.json'}), \
                mock.patch.object(refile_unit, 'run', fake_run), \
                mock.patch.object(sys, 'argv', ['refile_unit.py', 'run', 'u', '--config', '/explicit.json']):
            refile_unit.main()
        self.assertEqual(seen['config'], pathlib.Path('/explicit.json'))

    def test_generated_files_are_written_with_lf_on_every_platform(self):
        import catalog_io
        from unittest import mock
        with tempfile.TemporaryDirectory() as folder:
            path = pathlib.Path(folder) / 'm.cpp'
            catalog_io.write_text_lf(path, 'int a;\nint b;\n')
            self.assertEqual(path.read_bytes(), b'int a;\nint b;\n')
            # the line-end translation is switched off by the open call itself, whatever the platform
            with mock.patch('builtins.open', mock.mock_open()) as opened:
                catalog_io.write_text_lf(path, 'x\n')
            self.assertEqual(opened.call_args.kwargs.get('newline'), '\n')

    def test_the_draft_module_source_is_written_through_the_lf_writer(self):
        from unittest import mock
        written = []
        with tempfile.TemporaryDirectory() as folder, \
                mock.patch.object(refile_unit, 'REPOSITORY', pathlib.Path(folder)), \
                mock.patch.object(refile_unit.catalog_io, 'write_text_lf', lambda path, text: written.append(pathlib.Path(path).name)), \
                mock.patch.object(refile_unit.subprocess, 'run',
                                  lambda *a, **k: mock.Mock(returncode=1, stdout='', stderr='refused')):
            refile_unit.apply_attempt('u', 1, 'src/recovered/u.cpp', 'int a;\n', [0x10], [], 'config.json')
        self.assertEqual(written, ['u.cpp', 'spec.json', 'apply.log'])

    def test_no_platform_newline_write_is_left_in_the_route_a_tools(self):
        tools = pathlib.Path(__file__).resolve().parents[1] / 'tools' / 'original_target'
        for name in ('refile_unit.py', 'module_ledger.py'):
            self.assertNotIn('.write_text(', (tools / name).read_text(), name)
        native = pathlib.Path(__file__).resolve().parents[1] / 'native' / 'tools' / 'initterm_manifest.py'
        self.assertNotIn('.write_text(', native.read_text())


class PlanTests(unittest.TestCase):
    def setUp(self):
        self.texts = {'src/recovered/a.cpp': 'int a;', 'src/recovered/b.cpp': 'int b;',
                      'src/recovered/c.cpp': 'void f() { __asm { nop } }', 'src/recovered/d.cpp': 'int d;',
                      'src/recovered/e.cpp': 'int e;', 'src/recovered/f.cpp': 'int f;',
                      'src/recovered/g.cpp': 'int g;', 'src/recovered/m.cpp': 'int m;', 'src/recovered/o.cpp': 'int o;'}
        table = {'?G@@3HA': 0x800000}
        self.catalog = dict(regions=[
            row(0x10, 'population-1', 'src/recovered/a.cpp', references=table),
            row(0x20, 'population-1', 'src/recovered/b.cpp', references=table),
            row(0x30, 'population-1', 'src/recovered/c.cpp', references=table),
            row(0x40, 'population-2', 'src/recovered/d.cpp'),
            row(0x50, 'population-2', 'src/recovered/e.cpp', options=OPTIONS + ['/GR']),
            row(0x60, 'population-3', 'src/recovered/f.cpp',
                relocations=[dict(symbol='?G@@3HA', target=0x900000)]),
            row(0x70, 'population-4', 'src/recovered/g.cpp'),
            row(0x80, 'population-5', 'src/recovered/m.cpp'), row(0x90, 'population-5', 'src/recovered/m.cpp'),
            row(0x99, 'population-1', 'src/recovered/o.cpp', references=table)])
        self.entries = [0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80]

    def plan(self, **more):
        return self.plan_entries(self.entries, **more)

    def plan_entries(self, entries, **more):
        bounds = lambda pid, r: None if (pid, r['entry']) == ('population-1', 0x70) else 'same'
        return refile_unit.plan_unit(self.catalog, 'u', entries, self.texts.__getitem__, {}, bounds=bounds, **more)

    def test_members_candidates_and_residual_causes(self):
        plan = self.plan()
        self.assertEqual(plan['population'], 'population-1')
        self.assertEqual([r['entry'] for r in plan['members']], [0x10, 0x20])
        self.assertEqual([r['entry'] for r in plan['candidates']], [0x40])
        causes = {r['entry']: r['cause'] for r in plan['residuals']}
        self.assertIn('inline assembly', causes['00000030'])
        self.assertIn('another option set', causes['00000050'])
        self.assertIn('?G@@3HA is 00800000 in the unit and 00900000', causes['00000060'])
        self.assertIn("frozen in another population's inventory pass (population-4)", causes['00000070'])
        kinds = {r['entry']: r['kind'] for r in plan['residuals']}
        self.assertEqual(kinds, {'00000030': 'kept', '00000050': 'split', '00000060': 'split', '00000070': 'split',
                                 '00000080': 'kept'})
        held = next(r for r in plan['residuals'] if r['entry'] == '00000070')
        self.assertEqual((held['bytes'], held['population']), (16, 'population-4'))
        self.assertIn('already in the module source', causes['00000080'])
        self.assertEqual(plan['foreign'], 1)     # the other unit's row in the group stays in its file

    def test_two_option_sets_in_one_population_are_two_groups(self):
        gr = OPTIONS + ['/Gr']
        self.catalog['regions'].append(row(0x15, 'population-1', 'src/recovered/h.cpp', options=gr))
        self.catalog['regions'].append(dict(row(0x16, 'population-1', 'src/recovered/i.cpp', options=gr), body_bytes=1))
        self.texts.update({'src/recovered/h.cpp': 'int h;', 'src/recovered/i.cpp': 'int i;'})
        plan = self.plan_entries(self.entries + [0x15])
        self.assertEqual([r['entry'] for r in plan['members']], [0x10, 0x20])
        self.assertIn('a compile group of its own', {r['entry']: r['cause'] for r in plan['residuals']}['00000015'])
        self.assertEqual(plan['foreign'], 1)

    def test_separate_unit_population_has_no_unit_file(self):
        self.catalog['populations'] = [dict(id='population-1', separate_units=True)]
        plan = self.plan()
        self.assertEqual(plan['members'], [])
        self.assertIn('each source alone', {r['entry']: r['cause'] for r in plan['residuals']}['00000010'])

    def test_no_joins(self):
        plan = self.plan(joins=False)
        self.assertEqual(plan['candidates'], [])
        self.assertIn('joins not tried', {r['entry']: r['cause'] for r in plan['residuals']}['00000040'])

    def test_compiler_numbered_names_never_conflict_and_are_never_carried(self):
        joining = row(0x40, 'population-2', 'x', relocations=[dict(symbol='_$E12', target=5), dict(symbol='$S7', target=6),
                                                              dict(symbol='?New@@3HA', target=7)])
        self.assertIsNone(refile_unit.join_conflict(joining, {'_$E12': 99, '$S7': 98}, ''))
        self.assertEqual(refile_unit.join_references(joining, {'_$E12': 99}), {'?New@@3HA': 7})

    def test_ref_comment_conflict(self):
        joining = row(0x40, 'population-2', 'x')
        self.assertIn('?G@@3HA', refile_unit.join_conflict(joining, {'?G@@3HA': 1}, '// @ref ?G@@3HA=00000002\n'))


class JoinScreenTests(unittest.TestCase):
    """A join candidate that brings header state the unit lacks is split, not attempted."""

    def setUp(self):
        self.texts = {
            'src/recovered/a.cpp': '#define UNIT_SWITCH\n#include "shared/unit.hpp"\nint a;',
            'src/recovered/b.cpp': '#include "shared/unit.hpp"\nint b;',
            'src/shared/unit.hpp': '#include "shared/inner.hpp"\ntypedef unsigned long DWORD;\n',
            'src/shared/inner.hpp': 'class Inner;\n',
            'src/shared/ghidra_types.hpp': 'typedef unsigned int DWORD;\n',
            # joins: the unit's own lines, or a header the unit's headers already include
            'src/recovered/j1.cpp': '#include "shared/unit.hpp"\nint j1;',
            'src/recovered/j2.cpp': '#define UNIT_SWITCH\n#include "shared/inner.hpp"\nint j2;',
            # held back: the seed header, another header, another switch
            'src/recovered/s1.cpp': '#include "shared/ghidra_types.hpp"\nnamespace seed_00000050 { int s1; }',
            'src/recovered/s2.cpp': '#include "shared/other.hpp"\nint s2;',
            'src/recovered/s3.cpp': '#define OTHER_SWITCH\n#include "shared/unit.hpp"\nint s3;'}
        self.catalog = dict(regions=[
            row(0x10, 'population-1', 'src/recovered/a.cpp', body_bytes=400), row(0x20, 'population-1', 'src/recovered/b.cpp'),
            row(0x30, 'population-2', 'src/recovered/j1.cpp'), row(0x40, 'population-2', 'src/recovered/j2.cpp'),
            row(0x50, 'population-2', 'src/recovered/s1.cpp'), row(0x60, 'population-3', 'src/recovered/s2.cpp'),
            row(0x70, 'population-3', 'src/recovered/s3.cpp')])
        self.entries = [0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70]

    def test_candidates_with_foreign_header_state_are_split_before_any_compile(self):
        plan = refile_unit.plan_unit(self.catalog, 'u', self.entries, self.texts.__getitem__, {})
        self.assertEqual([r['entry'] for r in plan['members']], [0x10, 0x20])
        self.assertEqual([r['entry'] for r in plan['candidates']], [0x30, 0x40])
        held = {r['entry']: r for r in plan['residuals']}
        self.assertEqual(sorted(held), ['00000050', '00000060', '00000070'])
        self.assertTrue(all(r['kind'] == 'split' for r in held.values()))
        self.assertIn('needs header state the unit does not have: #include "shared/ghidra_types.hpp"', held['00000050']['cause'])
        self.assertIn('held by population-2', held['00000050']['cause'])
        self.assertIn('#include "shared/other.hpp"', held['00000060']['cause'])
        self.assertIn('#define OTHER_SWITCH', held['00000070']['cause'])

    def test_a_join_may_not_empty_a_population(self):
        # population-2 keeps s1 (screened out), so j1 and j2 may leave it; give j1 a population
        # of its own and it must stay: the join would leave that population without rows.
        self.catalog['regions'][2]['population'] = 'population-9'
        plan = refile_unit.plan_unit(self.catalog, 'u', self.entries, self.texts.__getitem__, {})
        self.assertEqual([r['entry'] for r in plan['candidates']], [0x40])
        held = {r['entry']: r for r in plan['residuals']}
        self.assertEqual(held['00000030']['kind'], 'split')
        self.assertEqual(held['00000030']['cause'], 'held by population-9; the join would leave that population without rows')

    def test_a_unit_that_holds_the_seed_header_lets_seed_rows_try(self):
        self.texts['src/recovered/b.cpp'] = '#include "shared/ghidra_types.hpp"\nint b;'
        # a row of another unit keeps population-2 alive when its three rows of this unit leave
        self.catalog['regions'].append(row(0x90, 'population-2', 'src/recovered/elsewhere.cpp'))
        plan = refile_unit.plan_unit(self.catalog, 'u', self.entries, self.texts.__getitem__, {})
        self.assertIn(0x50, [r['entry'] for r in plan['candidates']])

    def test_refused_joins_are_restated_as_split_only_where_the_screen_holds_them(self):
        self.texts['src/recovered/u.cpp'] = self.texts['src/recovered/a.cpp'] + '\n' + self.texts['src/recovered/b.cpp']
        refused = refile_unit.REFUSED_JOIN + ' (attempt 1: compile failed: ghidra_types.hpp(29) : error C2371)'
        modules = {'u': dict(route='A', source='src/recovered/u.cpp', residuals=[
            dict(kind='flipped', entry='00000050', population='population-2', source='src/recovered/s1.cpp', cause=refused),
            dict(kind='flipped', entry='00000030', population='population-2', source='src/recovered/j1.cpp', cause=refused),
            dict(kind='flipped', entry='00000060', population='population-3', source='src/recovered/s2.cpp',
                 cause='not exact in the unit\'s text in any form'),
            dict(kind='split', entry='00000070', population='population-3', source='src/recovered/s3.cpp', cause='frozen')])}
        changed = refile_unit.reclassify_refused_joins(modules, self.texts.__getitem__)
        self.assertEqual([(u, e) for u, e, _ in changed], [('u', '00000050')])
        rows = modules['u']['residuals']
        self.assertEqual([r['kind'] for r in rows], ['split', 'flipped', 'flipped', 'split'])
        self.assertEqual(rows[0]['cause'], 'held by population-2; needs header state the unit does not have: '
                                           '#include "shared/ghidra_types.hpp"')
        self.assertEqual(refile_unit.reclassify_refused_joins(modules, self.texts.__getitem__), [])


class JoinRowsTests(unittest.TestCase):
    """apply_rename `joins`: the unit's table has precedence, bounds must be equal."""
    def setUp(self):
        self.folder = tempfile.TemporaryDirectory()
        root = pathlib.Path(self.folder.name)
        def write(name, rows):
            (root / name).write_text('\n'.join(json.dumps(r) for r in rows) + '\n')
            return str(root / name)
        bound = dict(entry='00000040', ranges=[dict(start='00000040', length=16)])
        self.config = dict(populations={
            'population-1': dict(inventory=write('i1', [dict(entry='00000010'), bound]), gaps=write('g1', [dict(start='00000050', length=3)])),
            'population-2': dict(inventory=write('i2', [bound, dict(entry='00000060')]), gaps=write('g2', [dict(start='00000050', length=3)])),
            'population-3': dict(inventory=write('i3', [dict(bound, name='other')]), gaps=write('g3', []))})
        table = {'?G@@3HA': 0x800000}
        self.catalog = dict(regions=[
            row(0x10, 'population-1', 'a', references=table),
            row(0x40, 'population-2', 'd', references={'?H@@3HA': 0x810000, '_$E3': 0x40},
                relocations=[dict(symbol='?G@@3HA', target=0x800000), dict(symbol='?K@@3HA', target=0x820000)])])

    def tearDown(self):
        self.folder.cleanup()

    def test_join_moves_the_row_and_extends_the_table(self):
        rows = self.catalog['regions']
        left = apply_rename.join_rows(self.catalog, 'm', rows, {0x40}, self.config)
        self.assertEqual(left, {'population-2'})
        self.assertEqual(rows[1]['population'], 'population-1')
        self.assertIs(rows[0]['references'], rows[1]['references'])
        self.assertEqual(rows[0]['references'], {'?G@@3HA': 0x800000, '?H@@3HA': 0x810000, '?K@@3HA': 0x820000})

    def test_refuses_a_name_bound_elsewhere_by_the_unit(self):
        self.catalog['regions'][1]['relocations'][0]['target'] = 0x900000
        with self.assertRaises(SystemExit):
            apply_rename.join_rows(self.catalog, 'm', self.catalog['regions'], {0x40}, self.config)

    def test_refuses_another_frozen_bound(self):
        self.catalog['regions'][1]['population'] = 'population-3'
        with self.assertRaises(SystemExit):
            apply_rename.join_rows(self.catalog, 'm', self.catalog['regions'], {0x40}, self.config)

    def test_refuses_another_option_set(self):
        self.catalog['regions'][1]['options'] = OPTIONS + ['/GR']
        with self.assertRaises(SystemExit):
            apply_rename.join_rows(self.catalog, 'm', self.catalog['regions'], {0x40}, self.config)


class NativeRekeyTests(unittest.TestCase):
    def test_unique_text_is_only_re_keyed(self):
        module = 'x = 1;\n\n\ny = long_call(2);\n'
        chunks = {'a': 'x = 1;', 'b': 'y = long_call(2);'}
        out, problems = refile_unit.rekey_fixups([dict(source='b', old='long_call(2)', new='call(2)', why='w')],
                                                 {'a': 'm', 'b': 'm'}, module, chunks)
        self.assertEqual(problems, [])
        self.assertEqual(out, [dict(source='m', old='long_call(2)', new='call(2)', why='w')])

    def test_text_that_recurs_in_another_function_is_widened(self):
        module = 'void A()\n{\n    go(1);\n}\n\n\nvoid B()\n{\n    go(1);\n}\n'
        chunks = {'a': 'void A()\n{\n    go(1);\n}', 'b': 'void B()\n{\n    go(1);\n}'}
        out, problems = refile_unit.rekey_fixups([dict(source='b', old='go(1)', new='go(2)')], {'a': 'm', 'b': 'm'},
                                                 module, chunks)
        self.assertEqual(problems, [])
        self.assertEqual(len(out), 1)
        self.assertEqual(module.count(out[0]['old']), 1)
        fixed = module.replace(out[0]['old'], out[0]['new'])
        self.assertEqual(fixed, 'void A()\n{\n    go(1);\n}\n\n\nvoid B()\n{\n    go(2);\n}\n')

    def test_gathered_line_and_missing_text(self):
        module = '#include "shared/a.hpp"\n\n\nint a;\n'
        out, problems = refile_unit.rekey_fixups(
            [dict(source='a', old='#include "shared/a.hpp"', new='#include "port/a.hpp"'),
             dict(source='a', old='gone', new='x')], {'a': 'm'}, module, {'a': 'int a;'})
        self.assertEqual([f['source'] for f in out], ['m'])
        self.assertEqual(len(problems), 1)

    def test_later_fixups_are_placed_against_the_text_earlier_ones_leave(self):
        module = 'void A()\n{\n    first(1);\n    go(1);\n}\n\n\nvoid B()\n{\n    go(1);\n}\n'
        chunks = {'a': 'void A()\n{\n    first(1);\n    go(1);\n}', 'b': 'void B()\n{\n    go(1);\n}'}
        fixups = [dict(source='a', old='first(1)', new='first(2)'), dict(source='a', old='go(1)', new='go(2)')]
        out, problems = refile_unit.rekey_fixups(fixups, {'a': 'm', 'b': 'm'}, module, chunks)
        self.assertEqual(problems, [])
        text = module
        for fixup in out:
            self.assertIn(fixup['old'], text)       # native_units.py refuses a stale fixup
            text = text.replace(fixup['old'], fixup['new'])
        self.assertEqual(text, 'void A()\n{\n    first(2);\n    go(2);\n}\n\n\nvoid B()\n{\n    go(1);\n}\n')

    def test_several_occurrences_in_one_function(self):
        body = 'void B()\n{\n    a = (int)x;\n    b = (int)y;\n}'
        module = 'void A()\n{\n    a = (int)x;\n}\n\n\n' + body + '\n'
        out, problems = refile_unit.rekey_fixups([dict(source='b', old=' = (int)', new=' = (long)')], {'b': 'm'},
                                                 module, {'b': body})
        self.assertEqual(problems, [])
        text = module
        for fixup in out:
            self.assertEqual(text.count(fixup['old']), 1)
            text = text.replace(fixup['old'], fixup['new'])
        self.assertEqual(text, 'void A()\n{\n    a = (int)x;\n}\n\n\nvoid B()\n{\n    a = (long)x;\n    b = (long)y;\n}\n')

    def test_other_sources_are_untouched(self):
        fixup = dict(source='z', old='q', new='r')
        self.assertEqual(refile_unit.rekey_fixups([fixup], {'a': 'm'}, 'q', {}), ([fixup], []))


class UnplaceableFixupTests(unittest.TestCase):
    """Two functions of one unit carry a native fixup on the same declaration line (temporal)."""
    DECL = 'namespace seed_1 { void F(int building); }\n'
    A = '// Let_Go: its own line ahead of the declaration\n' + DECL + 'void Let_Go() { seed_1::F(1); }\n'
    B = DECL + 'void Release() { seed_1::F(2); }\n'
    C = 'void Other() { }\n'

    def fixups(self):
        return [dict(source='a', old=self.DECL, new=self.DECL + 'void Native_F(void *);\n'),
                dict(source='b', old=self.DECL, new=self.DECL + 'void Native_F(void *);\n'),
                dict(source='b', old='seed_1::F(2);', new='Native_F(0);'),
                dict(source='elsewhere', old='x', new='y')]

    def test_the_row_whose_fixup_cannot_be_made_unique_is_named_before_anything_is_applied(self):
        moved = {'a': 'm', 'b': 'm', 'c': 'm'}
        text, chunks = self.A + self.B + self.C, {'a': self.A, 'b': self.B, 'c': self.C}
        stuck = refile_unit.unplaceable_fixups(self.fixups(), moved, text, chunks)
        self.assertEqual(dict(stuck), {'b': refile_unit.NATIVE_NOT_UNIQUE})
        self.assertEqual(refile_unit.NATIVE_NOT_UNIQUE, 'native fixup cannot be made unique in the module')
        # the inputs are untouched: nothing was applied
        self.assertEqual(text, self.A + self.B + self.C)
        self.assertEqual(chunks, {'a': self.A, 'b': self.B, 'c': self.C})

    def test_poured_again_without_that_row_everything_places(self):
        moved = {'a': 'm', 'c': 'm'}
        self.assertEqual(dict(refile_unit.unplaceable_fixups(self.fixups(), moved, self.A + self.C,
                                                             {'a': self.A, 'c': self.C})), {})
        fixed, problems = refile_unit.rekey_fixups(self.fixups(), moved, self.A + self.C, {'a': self.A, 'c': self.C})
        self.assertEqual(problems, [])
        self.assertEqual([f['source'] for f in fixed], ['m', 'b', 'b', 'elsewhere'])

    def test_a_fixup_text_missing_from_its_part_is_reported_with_its_own_cause(self):
        stuck = refile_unit.unplaceable_fixups([dict(source='c', old='not there', new='x')], {'c': 'm'},
                                               self.A + self.B + self.C, {'c': self.C})
        self.assertIn('native fixup cannot be re-keyed into the module', stuck['c'])

    def test_units_without_native_fixups_are_never_held_back(self):
        self.assertEqual(dict(refile_unit.unplaceable_fixups([], {'a': 'm'}, self.A, {'a': self.A})), {})


class NativeTableRekeyTests(unittest.TestCase):
    def test_vtable_views_path_column(self):
        with tempfile.TemporaryDirectory() as folder:
            root = pathlib.Path(folder)
            (root / 'native/data').mkdir(parents=True)
            (root / 'src/recovered').mkdir(parents=True)
            (root / 'src/recovered/m.cpp').write_text('x')
            tsv = root / 'native/data/vtable_views.tsv'
            tsv.write_text('# c\npath\tclass\tverdict\nsrc/recovered/a.cpp\tV\tdead\nsrc/shared/h.hpp\tW\tdead\n')
            summary, problems = refile_unit.rekey_native({'src/recovered/a.cpp': 'src/recovered/m.cpp'}, root)
            self.assertEqual((summary['vtable_views'], problems), (1, []))
            self.assertEqual(tsv.read_text(), '# c\npath\tclass\tverdict\nsrc/recovered/m.cpp\tV\tdead\nsrc/shared/h.hpp\tW\tdead\n')


class SplitClassTests(unittest.TestCase):
    MODULE = '#define UNIT_SWITCH\n#include "shared/unit.hpp"\nint Unit_Function() { return 1; }\n'
    FILES = {
        'src/recovered/u.cpp': MODULE,
        'src/shared/unit.hpp': '#include "shared/inner.hpp"\n#include "beside.hpp"\nclass Unit;\n',
        'src/shared/inner.hpp': 'class Inner;\n',
        'src/shared/beside.hpp': 'class Beside;\n',
        'src/shared/ghidra_types.hpp': 'typedef unsigned char undefined1;\n',
        # a container member: its own implementation switch, one explicit instantiation
        'a.cpp': '#define VECTOR_IMPLEMENT_VECTOR_FETCH\n#include "shared/vector_class.hpp"\nclass T;\n'
                 'template class VectorClass<T *>;\n',
        # an explicit instantiation without a switch is still one member per file
        'b.cpp': '#include "shared/inner.hpp"\ntemplate class TypeList<int>;\n',
        # another unit's switch; another header; the seed header
        'c.cpp': '#define OTHER_SWITCH\n#include "shared/unit.hpp"\nint C() { return 2; }\n',
        'd.cpp': '#include "shared/other.hpp"\nint D() { return 3; }\n',
        'e.cpp': '#include "shared/ghidra_types.hpp"\nnamespace seed_00001000 {\nvoid FUN_00001000(void) {}\n}\n',
        # placeholder form: a type the file defines for itself; a seed namespace under the unit's own headers
        'f.cpp': '\nclass Leaf_1000 { public: bool get(); };\nbool Leaf_1000::get() { return false; }\n',
        'g.cpp': '#include "shared/inner.hpp"\nnamespace seed_00002000 {\nint FUN_00002000(void) { return 1; }\n}\n',
        # the unit's own lines and types only: nothing in the text decides
        'h.cpp': '#define UNIT_SWITCH\n#include "shared/beside.hpp"\nclass Unit;\nint H(Unit *u) { return u != 0; }\n',
    }

    def read(self, path):
        return self.FILES.get(path)

    def state(self):
        return refile_unit.module_state(self.MODULE, refile_unit.include_names(self.MODULE, self.read))

    def test_the_unit_state_holds_its_include_closure(self):
        self.assertEqual(refile_unit.include_names(self.MODULE, self.read),
                         ['shared/beside.hpp', 'shared/inner.hpp', 'shared/unit.hpp'])
        self.assertIn(('include', '"shared/inner.hpp"'), self.state())

    def test_each_class_is_read_from_the_row_text(self):
        state = self.state()
        got = {name: refile_unit.split_class(self.FILES[name], state) for name in self.FILES if len(name) == 5}
        self.assertEqual(got, {'a.cpp': 'template', 'b.cpp': 'template', 'c.cpp': 'header_state',
                               'd.cpp': 'header_state', 'e.cpp': 'header_state', 'f.cpp': 'stand_in',
                               'g.cpp': 'stand_in', 'h.cpp': 'unclassified'})

    def test_a_comment_only_file_is_emitted_by_another_source(self):
        text = '// Dynamic initializer (0x00001000) emitted by x.cpp; no source of its own.\n/* and a block */\n'
        self.assertEqual(refile_unit.split_class(text, self.state()), 'emitted')
        self.assertEqual(refile_unit.split_class(text, self.state(), 'own_population'), 'emitted')
        # an include-only file (header-static initialisers) has text of its own
        self.assertEqual(refile_unit.split_class('// statics\n#include "shared/beside.hpp"\n', self.state()),
                         'unclassified')

    def test_rows_the_text_does_not_decide_are_classed_by_their_holder(self):
        catalog = dict(populations=[dict(id='p-sep', separate_units=True), dict(id='p-own'), dict(id='p-mix'),
                                    dict(id='p-opt')],
                       regions=[dict(entry=0x1000, population='p-sep', options=['/O2']),
                                dict(entry=0x2000, population='p-opt', options=['/O1']),
                                dict(entry=0x2100, population='p-opt', options=['/O1']),
                                dict(entry=0x3000, population='p-own', options=['/O2']),
                                dict(entry=0x3100, population='p-own', options=['/O2']),
                                dict(entry=0x4000, population='p-mix', options=['/O2']),
                                dict(entry=0x4100, population='p-mix', options=['/O2']),
                                dict(entry=0x5000, population='p-mix', options=['/O2']),
                                dict(entry=0x6000, population='p-mix', options=['/O2'])])
        frozen = "frozen in another population's inventory pass (p-mix)"
        rows = [dict(kind='split', entry='00001000', population='p-sep', source='h.cpp', bytes=1),
                dict(kind='split', entry='00002000', population='p-opt', source='h.cpp', bytes=2),
                dict(kind='split', entry='00003000', population='p-own', source='h.cpp', bytes=3),
                dict(kind='split', entry='00003100', population='p-own', source='h.cpp', bytes=4),
                dict(kind='split', entry='00004000', population='p-mix', source='h.cpp', bytes=5, cause=frozen),
                dict(kind='split', entry='00004100', population='p-mix', source='h.cpp', bytes=6,
                     cause='held by p-mix (joins not tried in this run)'),
                dict(kind='split', entry='00005000', population='p-mix', source='a.cpp', bytes=7, cause=frozen)]
        entry = dict(source='src/recovered/u.cpp', options=['/O2'], residuals=rows)
        got = [(row['entry'], key) for _, row, key in refile_unit.classify_split_rows([entry], self.read, catalog)]
        self.assertEqual(got, [('00001000', 'separate_units'), ('00002000', 'option_set'),
                               ('00003000', 'own_population'), ('00003100', 'own_population'),
                               ('00004000', 'frozen_bound'), ('00004100', 'unclassified'), ('00005000', 'template')])
        # without the catalog only the text decides
        self.assertEqual(refile_unit.split_classes([entry], self.read)['unclassified'], [6, 21])

    def test_a_forward_declaration_is_not_a_stand_in(self):
        self.assertEqual(refile_unit.split_class('class Unit;\nstruct Other;\nint F();\n', self.state()), 'unclassified')

    def test_rows_and_bytes_per_class_and_only_split_rows(self):
        entry = dict(source='src/recovered/u.cpp', residuals=[
            dict(kind='split', source='a.cpp', bytes=13), dict(kind='split', source='b.cpp', bytes=2),
            dict(kind='split', source='c.cpp', bytes=40), dict(kind='split', source='f.cpp', bytes=3),
            dict(kind='split', source='h.cpp', bytes=5), dict(kind='split', source='gone.cpp', bytes=7),
            dict(kind='flipped', source='a.cpp', bytes=100), dict(kind='kept', source='c.cpp', bytes=100)])
        self.assertEqual(refile_unit.split_classes([entry, dict(source='src/recovered/none.cpp', residuals=[])], self.read),
                         dict(template=[2, 15], header_state=[1, 40], stand_in=[1, 3], emitted=[0, 0], separate_units=[0, 0],
                              option_set=[0, 0], own_population=[0, 0], frozen_bound=[0, 0], unclassified=[2, 12]))

    def test_status_line_carries_the_breakdown(self):
        import status
        entry = dict(module='u', route='A', code=[1], code_bytes=10, source='src/recovered/u.cpp', residuals=[
            dict(kind='split', source='a.cpp', bytes=13), dict(kind='split', source='e.cpp', bytes=1),
            dict(kind='split', source='f.cpp', bytes=3), dict(kind='flipped', source='a.cpp', bytes=9)])
        form = status.route_a_form([entry], self.read)
        self.assertEqual((form['route_a_split_rows'], form['route_a_split_bytes']), (3, 17))
        self.assertEqual(sum(form['route_a_split_%s_rows' % key] for key, _ in refile_unit.SPLIT_CLASSES), 3)
        self.assertEqual(sum(form['route_a_split_%s_bytes' % key] for key, _ in refile_unit.SPLIT_CLASSES), 17)
        line = status.route_a_line(form)
        self.assertIn('template members 1 rows, 13 bytes', line)
        self.assertIn('needs another unit\'s header state 1 rows, 1 bytes', line)
        self.assertIn('stand-ins 1 rows, 3 bytes', line)
        self.assertIn('not yet classified 0 rows, 0 bytes', line)
        # without a reader the line is as before
        self.assertNotIn('by cause', status.route_a_line(status.route_a_form([entry])))


class StatusLineTests(unittest.TestCase):
    def test_split_and_flipped_residuals_are_reported_separately(self):
        import status
        entries = [
            dict(module='plain', code=[1], code_bytes=10),
            dict(module='u', route='A', code=[1, 2], code_bytes=300, switches=['A', 'B'],
                 stand_in_references=[dict(site='1')],
                 residuals=[dict(kind='split', bytes=40), dict(kind='split', bytes=2), dict(kind='flipped', bytes=7),
                            dict(kind='kept', bytes=5)])]
        form = status.route_a_form(entries)
        self.assertEqual((form['route_a_units'], form['route_a_functions'], form['route_a_code_bytes']), (1, 2, 300))
        self.assertEqual((form['route_a_split_rows'], form['route_a_split_bytes']), (2, 42))
        self.assertEqual((form['route_a_flipped_rows'], form['route_a_flipped_bytes']), (1, 7))
        self.assertEqual((form['route_a_kept_rows'], form['route_a_kept_bytes']), (1, 5))
        line = status.route_a_line(form)
        self.assertIn('split 2 rows, 42 bytes', line)
        self.assertIn('flipped 1 rows, 7 bytes', line)
        self.assertIn('known ceiling', line)
        self.assertIn('1 stand-in references', line)


class PlaceJoinedTests(unittest.TestCase):
    MODULE = '#include "a.hpp"\n\n\nint f1() { return 1; }\n\n\nint f3() { return 3; }\n'

    def test_between_rows_in_address_order(self):
        out = refile_unit.place_joined(self.MODULE, [0x10, 0x30], ['int f1() { return 1; }', 'int f3() { return 3; }'],
                                       0x20, 'int f2() { return 2; }')
        self.assertLess(out.index('f1()'), out.index('f2()'))
        self.assertLess(out.index('f2()'), out.index('f3()'))

    def test_after_the_last_row(self):
        out = refile_unit.place_joined(self.MODULE, [0x10, 0x30], ['x', 'y'], 0x40, 'int f4() { return 4; }')
        self.assertTrue(out.endswith('int f3() { return 3; }\n\n\nint f4() { return 4; }\n'))

    def test_refuses_when_the_next_part_is_not_found(self):
        self.assertIsNone(refile_unit.place_joined(self.MODULE, [0x10, 0x30], ['int f1() { return 1; }', 'changed'],
                                                   0x20, 'int f2() { return 2; }'))


class UnitMapTests(unittest.TestCase):
    def test_round_trip_and_duplicate_refusal(self):
        with tempfile.TemporaryDirectory() as folder:
            target = pathlib.Path(folder) / 'unit_map.json'
            refile_unit.write_unit_map({'b': [dict(entry='0x00000020')], 'a': [dict(entry='0x00000030'), dict(entry='0x10')]},
                                       target)
            self.assertEqual(refile_unit.load_unit_map(target), {'a': [0x10, 0x30], 'b': [0x20]})
            with self.assertRaises(SystemExit):
                refile_unit.write_unit_map({'a': [dict(entry='0x10')], 'b': [dict(entry='0x10')]}, target)


class RouteAModuleCheckTests(unittest.TestCase):
    def catalog(self):
        return dict(regions=[
            dict(entry=0x10, source='src/recovered/u.cpp', symbol='?Real@@YAXXZ', relocations=[]),
            dict(entry=0x90, source='src/recovered/other.cpp', symbol='?O@@YAXXZ',
                 relocations=[dict(symbol='?StandIn@@YAXXZ', target=0x10)])])

    def test_route_a_counts_stand_in_references(self):
        problems, known = module_check.check(self.catalog(), {'u': dict(source='src/recovered/u.cpp', data=[], route='A')})
        self.assertEqual(problems, [])
        self.assertEqual(len(known), 1)
        self.assertIn('Route A keeps names', known[0])

    def test_other_modules_still_refuse(self):
        problems, _ = module_check.check(self.catalog(), {'u': dict(source='src/recovered/u.cpp', data=[])})
        self.assertEqual(len(problems), 1)


class TemplateFamilyTests(unittest.TestCase):
    """join-template --family: each family takes only members of its one header."""

    QI = ('// QueryInterface\n#include "shared/class_factory_query_interface.hpp"\n'
          '// @ref _IID_IClassFactory=007afcf0\n\nclass WaveClass;\n\ntemplate class TClassFactory<WaveClass>;\n')
    CTOR = ('#include "shared/class_factory.hpp"\nclass WaveClass;\n'
            'template <> TClassFactory<WaveClass>::TClassFactory() : references(0) {}\n')
    TL = ('// TypeList<int> copy constructor.\n// Layout and behaviour: shared/type_list.hpp.\n'
          '#define VECTOR_IMPLEMENT_VECTOR_COPY_CONSTRUCTOR\n#include "shared/type_list.hpp"\n\n'
          'template <>\nTypeList<int>::TypeList(TypeList<int> const &list)\n    : DynamicVectorClass<int>(list)\n{\n}\n')
    VEC = ('#define VECTOR_IMPLEMENT_VECTOR_CLEAR\n#include "shared/vector_class.hpp"\n'
           'template class VectorClass<int>;\n')

    def test_member_predicates_stay_within_their_header(self):
        F = refile_unit.FAMILIES
        self.assertTrue(F['class_factory']['member'](self.QI))
        self.assertTrue(F['class_factory']['member'](self.CTOR))
        self.assertFalse(F['class_factory']['member'](self.TL))
        self.assertTrue(F['type_list']['member'](self.TL))
        self.assertFalse(F['type_list']['member'](self.VEC))
        self.assertTrue(F['vector']['member'](self.VEC))
        self.assertFalse(F['vector']['member'](self.QI))

    def test_own_lines(self):
        F = refile_unit.FAMILIES
        holds = {('include', '"shared/class_factory.hpp"')}
        self.assertEqual(F['class_factory']['own'](self.QI, holds), [])
        self.assertEqual(len(F['class_factory']['own'](self.QI, set())), 1)
        self.assertEqual(F['type_list']['own'](self.TL, {('include', '"shared/type_list.hpp"')}), [])
        self.assertEqual(len(F['type_list']['own'](self.TL, set())), 1)     # the include, not the switch

    def test_member_comment_drops_the_layout_line(self):
        self.assertEqual(refile_unit.member_comment(self.TL), '// TypeList<int> copy constructor.')


if __name__ == '__main__':
    unittest.main()
