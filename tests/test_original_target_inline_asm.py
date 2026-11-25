"""Evidenced inline assembly: `__asm` only with the ledger flag and an eligible, archive-verified TibSun citation."""
from datetime import datetime, timezone
from pathlib import Path
import sys
import tempfile
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1]/'tools/original_target'))
from canonical import validate_inline_asm, inline_asm_blocks, module_inline_asm_blocks
from canonical import header_specialization_asm, header_asm_specializations, header_asm_rows, header_asm_citations
import status

ARCHIVE_CONQUER = '''void Main_Game(int argc, char * argv[])
{
\tint ret = Init_Game(argc, argv);

\t__asm {
\t\tmov ExceptionReturnStack, esp
\t\tmov ExceptionReturnFrame, ebp
\t\tlea eax, ret_addr
\t\tmov ExceptionReturnAddress, eax
\t}

ret_addr:
\treturn;
}
'''
CANDIDATE = '''// Main_Game 0x00481BF0
void Main_Game()
{
    // Stack/frame/resume label capture, the author's __asm (TibSun conquer.cpp:5).
    __asm {
        mov ExceptionReturnStack, esp   ; stack
        mov ExceptionReturnFrame,ebp
        lea eax, ret_addr
        mov ExceptionReturnAddress, eax
    }
ret_addr:
    return;
}
'''
EVIDENCED = dict(inline_asm='evidenced', inline_asm_citation='TibSun conquer.cpp:5')


class InlineAsmAuditTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.archive = Path(self.temp.name)
        (self.archive/'conquer.cpp').write_text(ARCHIVE_CONQUER)
        (self.archive/'display.cpp').write_text(ARCHIVE_CONQUER)

    def tearDown(self):
        self.temp.cleanup()

    def check(self, source, row, **options):
        return validate_inline_asm(source.encode(), row, self.archive, **options)

    def test_plain_source_needs_no_flag(self):
        self.assertEqual(self.check('int f() { return 0; }  // no __asm here\n', {}), 0)

    def test_commented_asm_is_not_a_block(self):
        self.assertEqual(inline_asm_blocks('//\t__asm nop\n/* _asm { int 3 } */ char *s = "__asm";\n'), [])

    def test_asm_without_flag_is_refused(self):
        with self.assertRaisesRegex(ValueError, 'without inline_asm: evidenced'):
            self.check(CANDIDATE, {})
        with self.assertRaisesRegex(ValueError, 'without inline_asm: evidenced'):
            self.check('void f() { _asm int 3 }\n', {})
        with self.assertRaisesRegex(ValueError, 'without inline_asm: evidenced'):
            self.check(CANDIDATE, dict(inline_asm='yes', inline_asm_citation='TibSun conquer.cpp:5'))

    def test_flag_and_eligible_citation_are_accepted(self):
        self.assertEqual(self.check(CANDIDATE, EVIDENCED, require_archive=True), 1)
        self.assertEqual(self.check(CANDIDATE, dict(EVIDENCED, inline_asm_citation=['TibSun conquer.cpp:5'])), 1)

    def test_non_eligible_citation_is_refused(self):
        source = CANDIDATE.replace('conquer.cpp', 'display.cpp')
        with self.assertRaisesRegex(ValueError, 'inline_asm_citation must be'):
            self.check(source, dict(EVIDENCED, inline_asm_citation='TibSun display.cpp:5'))
        with self.assertRaisesRegex(ValueError, 'inline_asm_citation must be'):
            self.check(CANDIDATE, dict(EVIDENCED, inline_asm_citation='conquer.cpp:5'))
        with self.assertRaisesRegex(ValueError, 'inline_asm_citation must be'):
            self.check(CANDIDATE, dict(inline_asm='evidenced'))

    def test_citation_must_be_quoted_and_match_the_archive_block(self):
        with self.assertRaisesRegex(ValueError, 'not quoted in the source'):
            self.check(CANDIDATE.replace('TibSun conquer.cpp:5', 'TibSun conquer'), EVIDENCED)
        with self.assertRaisesRegex(ValueError, 'No __asm block starts on the cited'):
            self.check(CANDIDATE.replace(':5', ':4'), dict(EVIDENCED, inline_asm_citation='TibSun conquer.cpp:4'))
        extra = CANDIDATE.replace('        mov ExceptionReturnAddress, eax\n',
                                  '        mov ExceptionReturnAddress, eax\n        xor eax, eax\n')
        with self.assertRaisesRegex(ValueError, 'differs from the cited TibSun archive block'):
            self.check(extra, EVIDENCED)
        with self.assertRaisesRegex(ValueError, 'One inline_asm_citation per __asm block'):
            self.check(CANDIDATE + 'void g() { __asm nop }\n', EVIDENCED)

    def test_missing_archive_refuses_acceptance(self):
        with self.assertRaisesRegex(ValueError, 'archive unavailable'):
            validate_inline_asm(CANDIDATE.encode(), EVIDENCED, self.archive/'absent', require_archive=True)
        self.assertEqual(validate_inline_asm(CANDIDATE.encode(), EVIDENCED, self.archive/'absent'), 1)

    def test_flag_without_asm_is_refused(self):
        with self.assertRaisesRegex(ValueError, 'without an __asm block'):
            self.check('int f() { return 0; }\n', EVIDENCED)


ARCHIVE_MPU = """unsigned int Get_CPU_Rate(unsigned int & high)
{
\treturn 0;
}

void RDTSC(void)
{
\t_asm
\t{
\t\tmov\tTSC_Low, eax
\t\tmov\tTSC_High, edx
\t}
}

int Get_CPU_Speed(void)
{
\t_asm\tmov\tstamp0, EAX
\treturn 0;
}
"""
# The mpu shape: one module file, one row without asm and two with (TibSun mpu.cpp:8, :17).
MODULE = """unsigned int __fastcall Get_CPU_Rate(unsigned int *high)
{
    *high = 0;
    return 0;
}

// TibSun mpu.cpp:8
void RDTSC(void)
{
\t_asm
\t{
\t\tmov\tTSC_Low, eax
\t\tmov\tTSC_High, edx
\t}
}

// TibSun mpu.cpp:17
int Get_CPU_Speed(void)
{
    unsigned int stamp0;
\t_asm\tmov\tstamp0, EAX
    if (stamp0) {
        return 1;
    }
    return 0;
}
"""
ROWS = [dict(entry=0x5abf20, symbol_prefix='?Get_CPU_Rate@@'),
        dict(entry=0x5abf60, symbol_prefix='?RDTSC@@', inline_asm='evidenced', inline_asm_citation='TibSun mpu.cpp:8'),
        dict(entry=0x5abf70, symbol_prefix='?Get_CPU_Speed@@', inline_asm='evidenced',
             inline_asm_citation='TibSun mpu.cpp:17')]


class ModuleInlineAsmTests(unittest.TestCase):
    """Per-function attribution of __asm blocks in a module source (review 2026-11-11)."""
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.archive = Path(self.temp.name)
        (self.archive/'mpu.cpp').write_text(ARCHIVE_MPU)

    def tearDown(self):
        self.temp.cleanup()

    def validate(self, text, rows):
        attributed = module_inline_asm_blocks(text, rows)
        return {row['entry']: validate_inline_asm(text.encode(), row, self.archive, require_archive=True,
                                                  blocks=attributed[row['entry']]) for row in rows}

    def test_mixed_rows_attribute_each_block_to_its_function(self):
        self.assertEqual(self.validate(MODULE, ROWS), {0x5abf20: 0, 0x5abf60: 1, 0x5abf70: 1})

    def test_flag_required_with_asm_and_refused_without(self):
        rows = [dict(r) for r in ROWS]
        for key in ('inline_asm', 'inline_asm_citation'):
            rows[1].pop(key)
        with self.assertRaisesRegex(ValueError, 'without inline_asm: evidenced'):
            self.validate(MODULE, rows)
        rows = [dict(r) for r in ROWS]
        rows[0].update(inline_asm='evidenced', inline_asm_citation='TibSun mpu.cpp:8')
        with self.assertRaisesRegex(ValueError, 'without an __asm block'):
            self.validate(MODULE, rows)

    def test_citations_stay_per_block(self):
        rows = [dict(r) for r in ROWS]
        rows[2]['inline_asm_citation'] = ['TibSun mpu.cpp:17', 'TibSun mpu.cpp:17']
        with self.assertRaisesRegex(ValueError, 'One inline_asm_citation per __asm block'):
            self.validate(MODULE, rows)

    def test_block_outside_a_function_refuses_the_file(self):
        text = MODULE + 'struct S {\n    int x;\n};\n'
        self.assertEqual(module_inline_asm_blocks(text, ROWS)[0x5abf60][0][1], ['mov TSC_Low, eax', 'mov TSC_High, edx'])
        outside = MODULE + 'struct T { int y; _asm nop };\n'
        with self.assertRaisesRegex(ValueError, 'outside a function body'):
            module_inline_asm_blocks(outside, ROWS)

    def test_block_in_an_inline_helper_refuses_the_file(self):
        helper = 'static inline void Stamp(void)\n{\n\t_asm\tmov\tstamp0, EAX\n}\n\n' + MODULE
        with self.assertRaisesRegex(ValueError, 'not one row'):
            module_inline_asm_blocks(helper, ROWS)

    def test_block_in_a_macro_refuses_the_file(self):
        macro = '#define STAMP(x) _asm mov x, eax\n' + MODULE
        with self.assertRaisesRegex(ValueError, 'in a macro'):
            module_inline_asm_blocks(macro, ROWS)
        conditional = '#if 1 /* _asm */\n#endif\n#ifdef X _asm nop\n#endif\n' + MODULE
        with self.assertRaisesRegex(ValueError, 'in a macro'):
            module_inline_asm_blocks(conditional, ROWS)

    def test_overloaded_name_is_uncertain(self):
        rows = ROWS + [dict(entry=0x5abfff, symbol_prefix='?Get_CPU_Speed@@YAHH@Z')]
        with self.assertRaisesRegex(ValueError, 'not one row'):
            module_inline_asm_blocks(MODULE, rows)

    def test_member_and_constructor_names(self):
        text = 'Pipe::Pipe(void) : x(0)\n{\n}\nint Pipe::Put(void const *s, int n)\n{\n\t_asm nop\n\treturn n;\n}\n'
        rows = [dict(entry=1, symbol_prefix='??0Pipe@@QAE@XZ'), dict(entry=2, symbol_prefix='?Put@Pipe@@UAEHPBXH@Z')]
        self.assertEqual([len(v) for v in module_inline_asm_blocks(text, rows).values()], [0, 1])

    def test_unchanged_per_function_file(self):
        # A one-row source keeps the whole-file check (canonical.validate_catalog only
        # attributes blocks when several rows share a file), macro included.
        text = '// TibSun mpu.cpp:8\n' + MODULE[MODULE.index('void RDTSC'):MODULE.index('// TibSun mpu.cpp:17')]
        self.assertEqual(validate_inline_asm(text.encode(), ROWS[1], self.archive, require_archive=True), 1)



# The archive's own asm-only macro (TibSun mpu.cpp:134 ASM_RDTSC), defined once and invoked
# by RDTSC (inside its __asm block) and by Get_CPU_Speed (as a statement).
ARCHIVE_MPU_MACRO = """#define ASM_RDTSC _asm _emit 0x0f _asm _emit 0x31

void RDTSC(void)
{
\t_asm
\t{
\t\tASM_RDTSC;
\t\tmov\tTSC_Low, eax
\t}
}

int Get_CPU_Speed(void)
{
\tASM_RDTSC;
\t_asm\tmov\tstamp0, EAX
\treturn 0;
}
"""
MACRO_MODULE = """// TibSun mpu.cpp:1
#define ASM_RDTSC _asm _emit 0x0f _asm _emit 0x31

unsigned int __fastcall Get_CPU_Rate(unsigned int *high)
{
    *high = 0;
    return 0;
}

// TibSun mpu.cpp:5, invoking the macro at TibSun mpu.cpp:7
void RDTSC(void)
{
\t_asm
\t{
\t\tASM_RDTSC;
\t\tmov\tTSC_Low, eax
\t}
}

// TibSun mpu.cpp:14 (the macro), TibSun mpu.cpp:15
int Get_CPU_Speed(void)
{
    unsigned int stamp0;
\tASM_RDTSC;
\t_asm\tmov\tstamp0, EAX
    return stamp0 != 0;
}
"""
MACRO_ROWS = [dict(entry=0x5abf20, symbol_prefix='?Get_CPU_Rate@@'),
              dict(entry=0x5abf60, symbol_prefix='?RDTSC@@', inline_asm='evidenced',
                   inline_asm_citation=['TibSun mpu.cpp:5', 'TibSun mpu.cpp:7', 'TibSun mpu.cpp:1', 'TibSun mpu.cpp:1']),
              dict(entry=0x5abf70, symbol_prefix='?Get_CPU_Speed@@', inline_asm='evidenced',
                   inline_asm_citation=['TibSun mpu.cpp:14', 'TibSun mpu.cpp:1', 'TibSun mpu.cpp:1', 'TibSun mpu.cpp:15'])]


class ModuleAsmMacroTests(unittest.TestCase):
    """Asm-only archive macros in module sources (amendment, reviewed 2026-11-11)."""
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.archive = Path(self.temp.name)
        (self.archive/'mpu.cpp').write_text(ARCHIVE_MPU_MACRO)

    def tearDown(self):
        self.temp.cleanup()

    def validate(self, text, rows=MACRO_ROWS, unit='mpu.cpp'):
        attributed = module_inline_asm_blocks(text, rows)
        return {row['entry']: validate_inline_asm(text.encode(), row, self.archive, require_archive=True,
                                                  blocks=attributed[row['entry']], unit=unit) for row in rows}

    def test_archive_macro_is_accepted(self):
        self.assertEqual(self.validate(MACRO_MODULE), {0x5abf20: 0, 0x5abf60: 4, 0x5abf70: 4})
        expansion = module_inline_asm_blocks(MACRO_MODULE, MACRO_ROWS)[0x5abf70]
        self.assertEqual([b[1] for b in expansion], [None, ['_emit 0x0f'], ['_emit 0x31'], ['mov stamp0, EAX']])

    def test_non_asm_token_is_refused(self):
        token = MACRO_MODULE.replace('#define ASM_RDTSC _asm', '#define ASM_RDTSC Stamp(); _asm')
        with self.assertRaisesRegex(ValueError, 'non-asm token'):
            self.validate(token)
        changed = MACRO_MODULE.replace('_emit 0x31\n', '_emit 0x90\n', 1)
        with self.assertRaisesRegex(ValueError, 'differs from the archive definition'):
            self.validate(changed)
        with self.assertRaisesRegex(ValueError, 'not defined on the cited'):
            rows = [dict(r) for r in MACRO_ROWS]
            rows[2]['inline_asm_citation'] = ['TibSun mpu.cpp:14', 'TibSun mpu.cpp:15', 'TibSun mpu.cpp:1',
                                              'TibSun mpu.cpp:15']
            self.validate(MACRO_MODULE, rows)

    def test_invocation_outside_a_function_is_refused(self):
        outside = MACRO_MODULE + 'struct S {\n    int x;\n    ASM_RDTSC\n};\n'
        with self.assertRaisesRegex(ValueError, 'outside a function body'):
            module_inline_asm_blocks(outside, MACRO_ROWS)
        helper = MACRO_MODULE + 'static inline void Stamp(void)\n{\n\tASM_RDTSC;\n}\n'
        with self.assertRaisesRegex(ValueError, 'not one row'):
            module_inline_asm_blocks(helper, MACRO_ROWS)

    def test_invocation_without_an_archive_counterpart_is_refused(self):
        rows = [dict(r) for r in MACRO_ROWS]
        rows[2]['inline_asm_citation'] = ['TibSun mpu.cpp:15', 'TibSun mpu.cpp:1', 'TibSun mpu.cpp:1',
                                          'TibSun mpu.cpp:15']
        with self.assertRaisesRegex(ValueError, 'no archive invocation'):
            self.validate(MACRO_MODULE, rows)
        rows[2]['inline_asm_citation'] = ['TibSun mpu.cpp:1', 'TibSun mpu.cpp:1', 'TibSun mpu.cpp:1',
                                          'TibSun mpu.cpp:15']
        with self.assertRaisesRegex(ValueError, 'no archive invocation'):
            self.validate(MACRO_MODULE, rows)
        extra = MACRO_MODULE.replace('    return stamp0 != 0;', '\tASM_RDTSC;\n    return stamp0 != 0;')
        rows[2]['inline_asm_citation'] = ['TibSun mpu.cpp:14', 'TibSun mpu.cpp:1', 'TibSun mpu.cpp:1',
                                          'TibSun mpu.cpp:15', 'TibSun mpu.cpp:14', 'TibSun mpu.cpp:1',
                                          'TibSun mpu.cpp:1']
        with self.assertRaisesRegex(ValueError, 'cited twice'):
            self.validate(extra, rows)

    def test_macro_outside_its_unit_is_refused(self):
        with self.assertRaisesRegex(ValueError, 'outside its own unit'):
            self.validate(MACRO_MODULE, unit='conquer.cpp')
        with self.assertRaisesRegex(ValueError, 'outside its own unit'):
            self.validate(MACRO_MODULE, unit=None)


# The xsurface shape (rebuild tree, review session and user 2026-11-20): the archive's own file-static
# helper surface_quick_fill holds the author's _asm (TibSun xsurface.cpp:8 here); Fill_Rect expands it.
ARCHIVE_XSURFACE = """/// Fills a run of longwords with a color value.
#pragma warning(disable: 4035)
bool XSurface::Fill_Rect(Rect const & fillrect, int color)
{
}
static void *surface_quick_fill(void *buf, int count, int color)
{
\t_asm {
\t\tpush edi /// Bug fixed in TS but not in ShapeSet
\t\tmov ecx, [count]
\t\tmov edi, [buf]
\t\tcmp ecx, 0
\t\tjle short $end
\t\tmov eax, [color]
\t\trep stosd
\t$end:
\t\tmov eax, edi
\t\tpop edi
\t}
}
"""
XSURFACE = """bool XSurface::Fill_Rect(Rect const & fillrect, int color)
{
\treturn(XSurface::Fill_Rect(fillrect, Get_Rect(), color));
}

/// Inline assembly as the author wrote it: TibSun xsurface.cpp:8
#pragma warning(disable: 4035)
static void *surface_quick_fill(void *buf, int count, int color)
{
\t_asm {
\t\tpush edi
\t\tmov ecx, [count]
\t\tmov edi, [buf]
\t\tcmp ecx, 0
\t\tjle short $end
\t\tmov eax, [color]
\t\trep stosd
\t$end:
\t\tmov eax, edi
\t\tpop edi
\t}
}
#pragma warning(default: 4035)

bool XSurface::Fill_Rect(Rect const & cliprect, Rect const & fillrect, int color)
{
\tvoid *tail = surface_quick_fill(buffer, width, color << 16 | color);
\treturn(tail != 0);
}

bool XSurface::Fill(int color)
{
\treturn(Fill_Rect(Get_Rect(), Get_Rect(), color));
}
"""
FILL_RECT2, FILL_RECT3, FILL = 0x774390, 0x7743c0, 0x774e20
HELPER_FLAG = dict(inline_asm='evidenced', inline_asm_citation=['TibSun xsurface.cpp:8'])
# The 2-argument Fill_Rect calls the 3-argument one qualified (a direct call /Ob2 may expand, wherever it is
# defined), so it reaches the helper too; Fill's unqualified call to the virtual member is a dispatch.
XSURFACE_ROWS = [dict(entry=FILL_RECT2, symbol_prefix='?Fill_Rect@XSurface@@UAE_NABV?$TRect@H@@H@Z', **HELPER_FLAG),
                 dict(entry=FILL_RECT3, symbol_prefix='?Fill_Rect@XSurface@@UAE_NABV?$TRect@H@@0H@Z', **HELPER_FLAG),
                 dict(entry=FILL, symbol_prefix='?Fill@XSurface@@UAE_NH@Z')]


class HelperInlineAsmTests(unittest.TestCase):
    """__asm in a file-static helper of the unit, the archive's own (rebuild tree; 2026-11-20)."""
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.archive = Path(self.temp.name)
        (self.archive/'xsurface.cpp').write_text(ARCHIVE_XSURFACE)

    def tearDown(self):
        self.temp.cleanup()

    def validate(self, text, rows=XSURFACE_ROWS, unit='xsurface.cpp', archive=None):
        attributed = module_inline_asm_blocks(text, rows, helpers=True)
        return {row['entry']: validate_inline_asm(text.encode(), row, archive or self.archive, blocks=attributed[row['entry']],
                                                  unit=unit) for row in rows}

    def test_fill_rect_shape_is_accepted_on_the_row_that_expands_the_helper(self):
        self.assertEqual(self.validate(XSURFACE), {FILL_RECT2: 1, FILL_RECT3: 1, FILL: 0})
        blocks = module_inline_asm_blocks(XSURFACE, XSURFACE_ROWS, helpers=True)[FILL_RECT3]
        self.assertEqual([b[3] for b in blocks], ['surface_quick_fill'])

    def test_main_tree_still_refuses_the_helper(self):
        with self.assertRaisesRegex(ValueError, 'not one row'):
            module_inline_asm_blocks(XSURFACE, XSURFACE_ROWS)

    def test_helper_not_in_the_archive_is_refused(self):
        (self.archive/'xsurface.cpp').write_text(ARCHIVE_XSURFACE.replace('surface_quick_fill', 'quick_fill'))
        with self.assertRaisesRegex(ValueError, "not in the archive's surface_quick_fill"):
            self.validate(XSURFACE)
        renamed = XSURFACE.replace('surface_quick_fill', 'quick_fill')
        (self.archive/'xsurface.cpp').write_text(ARCHIVE_XSURFACE)
        with self.assertRaisesRegex(ValueError, "not in the archive's quick_fill"):
            self.validate(renamed)

    def test_block_text_differing_from_the_archive_is_refused(self):
        for changed in (XSURFACE.replace('\t\trep stosd\n', '\t\trep stosd\n\t\tnop\n'),
                        XSURFACE.replace('jle short $end', 'jl short $end')):
            with self.assertRaisesRegex(ValueError, 'differs from the cited TibSun archive block'):
                self.validate(changed)

    def test_caller_row_without_the_flag_is_refused(self):
        rows = [dict(r) for r in XSURFACE_ROWS]
        for key in HELPER_FLAG:
            rows[1].pop(key)
        with self.assertRaisesRegex(ValueError, 'without inline_asm: evidenced'):
            self.validate(XSURFACE, rows)
        # The 2-argument Fill_Rect, defined before the function it calls, reaches the helper all the same.
        rows = [dict(r) for r in XSURFACE_ROWS]
        for key in HELPER_FLAG:
            rows[0].pop(key)
        with self.assertRaisesRegex(ValueError, 'without inline_asm: evidenced'):
            self.validate(XSURFACE, rows)
        # A second caller (qualified, defined after the helper's caller, so VC6 may expand it) needs it too.
        second = XSURFACE + '\nvoid XSurface::Clear(void)\n{\n\tXSurface::Fill_Rect(r, r, 0);\n}\n'
        rows = XSURFACE_ROWS + [dict(entry=0x774f00, symbol_prefix='?Clear@XSurface@@QAEXXZ')]
        with self.assertRaisesRegex(ValueError, 'without inline_asm: evidenced'):
            self.validate(second, rows)
        rows[-1].update(HELPER_FLAG)
        self.assertEqual(self.validate(second, rows)[0x774f00], 1)
        # Through a file-static C function: the row reaches the helper all the same.
        relay = XSURFACE.replace('\nbool XSurface::Fill_Rect(Rect const & cliprect',
                                 '\nstatic void *relay(void *b)\n{\n\treturn surface_quick_fill(b, 1, 0);\n}\n'
                                 '\nbool XSurface::Fill_Rect(Rect const & cliprect')
        relay = relay.replace('surface_quick_fill(buffer, width, color << 16 | color)', 'relay(buffer)')
        self.assertEqual(self.validate(relay), {FILL_RECT2: 1, FILL_RECT3: 1, FILL: 0})

    def test_row_reaching_through_a_later_defined_function_needs_the_flag(self):
        # Clear is defined before Paint and Paint after the helper's caller; /Ob2 may expand Paint into Clear.
        later = XSURFACE.replace('\nbool XSurface::Fill_Rect(Rect const & cliprect',
                                 '\nvoid XSurface::Clear(void)\n{\n\tPaint(0);\n}\n'
                                 '\nbool XSurface::Fill_Rect(Rect const & cliprect') + \
            '\nvoid XSurface::Paint(int color)\n{\n\tsurface_quick_fill(0, 1, color);\n}\n'
        rows = XSURFACE_ROWS + [dict(entry=0x774f00, symbol_prefix='?Clear@XSurface@@QAEXXZ'),
                                dict(entry=0x774f40, symbol_prefix='?Paint@XSurface@@QAEXH@Z', **HELPER_FLAG)]
        with self.assertRaisesRegex(ValueError, 'without inline_asm: evidenced'):
            self.validate(later, rows)
        rows[-2] = dict(rows[-2], **HELPER_FLAG)
        self.assertEqual(self.validate(later, rows)[0x774f00], 1)
        # An unqualified call to a virtual member is a dispatch: not followed.
        virtual = [dict(r, symbol_prefix=r['symbol_prefix'].replace('QAEXH@Z', 'UAEXH@Z')) for r in rows]
        virtual[-2] = {k: v for k, v in virtual[-2].items() if k not in HELPER_FLAG}
        self.assertEqual(self.validate(later, virtual)[0x774f00], 0)

    def test_asm_in_a_non_static_function_of_another_unit_is_refused(self):
        exported = XSURFACE.replace('static void *surface_quick_fill', 'void *surface_quick_fill')
        with self.assertRaisesRegex(ValueError, 'not one row'):
            self.validate(exported)
        with self.assertRaisesRegex(ValueError, 'not one row'):
            module_inline_asm_blocks(exported.replace('XSurface::', 'Other::'), [], helpers=True)
        with self.assertRaisesRegex(ValueError, 'cited outside its own unit'):
            self.validate(XSURFACE, unit='blit.cpp')

    def test_helper_no_row_reaches_is_refused(self):
        rows = [dict(entry=0x775000, symbol_prefix='?Other@XSurface@@QAEXXZ')]
        with self.assertRaisesRegex(ValueError, 'no row reaches the asm helper'):
            module_inline_asm_blocks(XSURFACE, rows, helpers=True)

    def test_helper_needs_the_archive(self):
        with self.assertRaisesRegex(ValueError, 'reached through a helper cannot be verified'):
            self.validate(XSURFACE, archive=self.archive/'absent')

    def test_helper_named_outside_a_body_is_refused(self):
        table = XSURFACE + '\nstatic void *(*filler)(void *, int, int) = surface_quick_fill;\n'
        with self.assertRaisesRegex(ValueError, 'outside a function body'):
            module_inline_asm_blocks(table, XSURFACE_ROWS, helpers=True)


# A header's explicit asm specialization (rebuild tree; review session's conditions 2026-11-21, lane O16).
ARCHIVE_BLITBLIT = """template<class T>
class BlitTrans : public Blitter {
	public:
		virtual void BlitForward(void * dest, void const * source, int length, int z_min = 0) const
		{
			T * dcast = (T *)dest;
			for (int index = 0; index < length; index++) { if (((T *)source)[index]) dcast[index] = ((T *)source)[index]; }
		}
};

#if defined(_MSC_VER)
#pragma optimize("", off)

template <>
inline void BlitTrans<unsigned char>::BlitForward(void * dest, void const * source, int len, int z_min) const
{
	__asm {
		mov	esi,[source]
		mov	edi,[dest]
		mov	ecx,[len]
	}
again:
	__asm {
		dec	ecx
		jz		fini
		jmp	again
	}
fini:;
}

#if 0
template <>
inline void BlitPlainXlat<unsigned short>::BlitForward(void * dest, void const * source, int len, int z_min) const
{
	__asm {
		mov	ecx,[len]
	}
}
#endif
#endif

#pragma optimize("", on)
"""
# The adopted header: the archive's text below a longer preamble (lines differ, citations name the archive's).
HEADER_BLITBLIT = '/*\n**\tAdopted from the TibSun archive.\n*/\n#pragma once\n\n' + ARCHIVE_BLITBLIT
BLIT_TRANS = '?BlitForward@?$BlitTrans@E@@UBEXPAXPBXHH@Z'
SPECIAL = 'BlitTrans<unsigned char>::BlitForward'


def archive_line(text, needle, nth=0):
    lines = [n for n, line in enumerate(text.split('\n'), 1) if needle in line]
    return lines[nth]


SPECIAL_FLAG = dict(inline_asm='evidenced',
                    inline_asm_citation=['TibSun blitblit.h:%d' % archive_line(ARCHIVE_BLITBLIT, '__asm {', 0),
                                         'TibSun blitblit.h:%d' % archive_line(ARCHIVE_BLITBLIT, '__asm {', 1)])
UNIT_SOURCE = '#include "blitblit.h"\n\nvoid Unit_Function(void)\n{\n}\n\nvoid Other_Function(void)\n{\n}\n'
UNIT_ROWS = [dict(entry=0x775900, symbol_prefix='?Unit_Function@@YAXXZ'),
             dict(entry=0x775980, symbol_prefix=BLIT_TRANS, **SPECIAL_FLAG)]


class HeaderSpecializationAsmTests(unittest.TestCase):
    """__asm in a header's explicit specialization at the archive's place (rebuild tree; lane O16)."""
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.archive = Path(self.temp.name)
        (self.archive/'blitblit.h').write_text(ARCHIVE_BLITBLIT)

    def tearDown(self):
        self.temp.cleanup()

    def validate(self, header=HEADER_BLITBLIT, rows=UNIT_ROWS, source=UNIT_SOURCE, unit='xsurface.cpp', archive=None,
                 headers=None):
        """validate_catalog's order: the unit's own blocks, then the blocks of the header specialization the
        row's symbol names."""
        specializations = header_asm_specializations(headers or {'blitblit.h': header.encode()})
        attributed = module_inline_asm_blocks(source, rows, helpers=True)
        return {row['entry']: validate_inline_asm(source.encode(), row, archive or self.archive,
                                                  blocks=attributed[row['entry']] + header_asm_rows(row, specializations),
                                                  unit=unit) for row in rows}

    def test_specialization_blocks_are_read_from_live_code_only(self):
        found = header_specialization_asm(HEADER_BLITBLIT)
        self.assertEqual(list(found), [SPECIAL])      # the #if 0 specialization and the primary template are not
        self.assertEqual([line for _, _, line in found[SPECIAL]],
                         [archive_line(HEADER_BLITBLIT, '__asm {', n) for n in (0, 1)])
        guarded = HEADER_BLITBLIT.replace('#if defined(_MSC_VER)', '#ifdef RA2_NON_MATCHING')
        self.assertEqual(header_specialization_asm(guarded), {})
        self.assertEqual(header_asm_citations(SPECIAL, ARCHIVE_BLITBLIT, 'blitblit.h'), SPECIAL_FLAG['inline_asm_citation'])

    def test_every_unit_that_emits_the_specialization_is_accepted(self):
        self.assertEqual(self.validate(), {0x775900: 0, 0x775980: 2})
        convert = [dict(entry=0x4861a0, symbol_prefix='?Other_Function@@YAXXZ'),
                   dict(entry=0x4861c0, symbol_prefix=BLIT_TRANS, **SPECIAL_FLAG)]
        self.assertEqual(self.validate(rows=convert, unit='convert.cpp'), {0x4861a0: 0, 0x4861c0: 2})
        # A per-function source (one row) takes the header's blocks the same way.
        self.assertEqual(validate_inline_asm(b'void f() {}\n', UNIT_ROWS[1], self.archive, blocks=header_asm_rows(
            UNIT_ROWS[1], header_asm_specializations({'blitblit.h': HEADER_BLITBLIT.encode()}))), 2)

    def test_row_emitting_the_specialization_without_the_flag_is_refused(self):
        rows = [UNIT_ROWS[0], dict(entry=0x775980, symbol_prefix=BLIT_TRANS)]
        with self.assertRaisesRegex(ValueError, 'without inline_asm: evidenced'):
            self.validate(rows=rows)

    def test_flag_on_any_other_row_is_refused(self):
        for symbol in ('?BlitForward@?$BlitPlain@E@@UBEXPAXPBXHH@Z',       # another class template
                       '?BlitForward@?$BlitTrans@G@@UBEXPAXPBXHH@Z',       # another specialization of it
                       '?BlitBackward@?$BlitTrans@E@@UBEXPAXPBXHH@Z',      # another member
                       '?Unit_Function@@YAXXZ'):
            rows = [dict(entry=0x775900, symbol_prefix='?Other_Function@@YAXXZ'),
                    dict(entry=0x775980, symbol_prefix=symbol, **SPECIAL_FLAG)]
            with self.subTest(symbol=symbol), self.assertRaisesRegex(ValueError, 'flag on a source without an __asm block'):
                self.validate(rows=rows)

    def test_specialization_the_archive_lacks_is_refused(self):
        (self.archive/'blitblit.h').write_text(ARCHIVE_BLITBLIT.replace('BlitTrans<unsigned char>', 'BlitTrans<unsigned short>'))
        with self.assertRaisesRegex(ValueError, 'has no explicit specialization of that name'):
            self.validate()

    def test_primary_template_body_is_refused(self):
        # The archive has the block only in the primary template: no evidence for the specialization.
        primary = ARCHIVE_BLITBLIT.replace(
            '\t\t\tT * dcast = (T *)dest;\n',
            '\t\t\t__asm {\n\t\t\t\tmov\tesi,[source]\n\t\t\t\tmov\tedi,[dest]\n\t\t\t\tmov\tecx,[len]\n\t\t\t}\n')
        primary = primary.replace('template <>\ninline void BlitTrans<unsigned char>::BlitForward',
                                  'inline void Plain_Forward')
        (self.archive/'blitblit.h').write_text(primary)
        rows = [UNIT_ROWS[0], dict(UNIT_ROWS[1], inline_asm_citation=[
            'TibSun blitblit.h:%d' % archive_line(primary, '__asm {', 0),
            'TibSun blitblit.h:%d' % archive_line(primary, '__asm {', 1)])]
        with self.assertRaisesRegex(ValueError, 'has no explicit specialization of that name'):
            self.validate(rows=rows)
        # A block in the header's primary template is attributed to no row: a flag there is refused.
        header = HEADER_BLITBLIT.replace('template <>\ninline void BlitTrans<unsigned char>::BlitForward',
                                         'template <class T>\ninline void BlitTrans<T>::BlitForward')
        self.assertEqual(header_specialization_asm(header), {})
        with self.assertRaisesRegex(ValueError, 'flag on a source without an __asm block'):
            self.validate(header=header)

    def test_block_text_differing_from_the_archive_is_refused(self):
        for changed in (HEADER_BLITBLIT.replace('\t\tjz\t\tfini\n', '\t\tjz\t\tfini\n\t\tnop\n'),
                        HEADER_BLITBLIT.replace('mov\tecx,[len]', 'mov\tecx,[z_min]')):
            with self.assertRaisesRegex(ValueError, 'differs from the cited TibSun archive block'):
                self.validate(header=changed)

    def test_citations_must_be_the_archive_specializations_blocks_in_order(self):
        cited = SPECIAL_FLAG['inline_asm_citation']
        rows = [UNIT_ROWS[0], dict(UNIT_ROWS[1], inline_asm_citation=cited[:1])]
        with self.assertRaisesRegex(ValueError, 'One inline_asm_citation per __asm block'):
            self.validate(rows=rows)
        rows = [UNIT_ROWS[0], dict(UNIT_ROWS[1], inline_asm_citation=cited[::-1])]
        with self.assertRaisesRegex(ValueError, 'differs from the cited TibSun archive block'):
            self.validate(rows=rows)
        # The same instructions in another archive specialization are not this specialization's block.
        twin = ARCHIVE_BLITBLIT.replace('#if 0\n', '#if 1\n').replace('\t\tmov\tecx,[len]\n\t}\n}\n#endif',
                                                                 '\t\tmov\tesi,[source]\n\t\tmov\tedi,[dest]\n'
                                                                 '\t\tmov\tecx,[len]\n\t}\n}\n#endif')
        (self.archive/'blitblit.h').write_text(twin)
        rows = [UNIT_ROWS[0], dict(UNIT_ROWS[1], inline_asm_citation=[
            'TibSun blitblit.h:%d' % archive_line(twin, '__asm {', 2), cited[1]])]
        with self.assertRaisesRegex(ValueError, "is not the archive specialization's block 1"):
            self.validate(rows=rows)
        # The header holding fewer blocks than the archive specialization is refused.
        short = HEADER_BLITBLIT.replace('again:\n\t__asm {\n\t\tdec\tecx\n\t\tjz\t\tfini\n\t\tjmp\tagain\n\t}\n', '')
        (self.archive/'blitblit.h').write_text(ARCHIVE_BLITBLIT)
        rows = [UNIT_ROWS[0], dict(UNIT_ROWS[1], inline_asm_citation=cited[:1])]
        with self.assertRaisesRegex(ValueError, 'the archive specialization has 2'):
            self.validate(header=short, rows=rows)

    def test_citation_outside_the_header_is_refused(self):
        rows = [UNIT_ROWS[0], dict(UNIT_ROWS[1], inline_asm_citation=['TibSun xsurface.cpp:15', 'TibSun xsurface.cpp:21'])]
        with self.assertRaisesRegex(ValueError, 'cited outside its header'):
            self.validate(rows=rows)

    def test_specialization_needs_the_archive(self):
        with self.assertRaisesRegex(ValueError, 'header specialization cannot be verified'):
            self.validate(archive=self.archive/'absent')

    def test_specialization_no_row_symbol_can_name_or_defined_twice_is_refused(self):
        foreign = HEADER_BLITBLIT.replace('BlitTrans<unsigned char>', 'BlitTrans<Palette>')
        with self.assertRaisesRegex(ValueError, 'arguments no row symbol is read for'):
            header_asm_specializations({'blitblit.h': foreign.encode()})
        with self.assertRaisesRegex(ValueError, 'defined in two headers'):
            header_asm_specializations({'blitblit.h': HEADER_BLITBLIT.encode(), 'rlerle.h': HEADER_BLITBLIT.encode()})
        with self.assertRaisesRegex(ValueError, 'defined more than once'):
            header_specialization_asm(HEADER_BLITBLIT.replace('#if 0\n', '#if 1\n').replace(
                'BlitPlainXlat<unsigned short>', 'BlitTrans<unsigned char>'))


if __name__ == '__main__':
    unittest.main()
