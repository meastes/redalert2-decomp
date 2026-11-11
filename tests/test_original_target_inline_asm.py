"""Evidenced inline assembly: `__asm` only with the ledger flag and an eligible, archive-verified TibSun citation."""
from datetime import datetime, timezone
from pathlib import Path
import sys
import tempfile
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1]/'tools/original_target'))
from canonical import validate_inline_asm, inline_asm_blocks, module_inline_asm_blocks
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


class InlineAsmStatusTests(unittest.TestCase):
    def test_evidenced_bytes_are_reported_apart_from_source_credit(self):
        catalog = dict(regions=[dict(body_bytes=100, padding_bytes=4, population='p1'),
                                dict(body_bytes=40, padding_bytes=0, population='p1', inline_asm='evidenced',
                                     inline_asm_citation='TibSun conquer.cpp:346')])
        start = datetime(2026, 11, 7, 12, tzinfo=timezone.utc)
        text = status.render(catalog, 1000, [dict(commit='a', time=start, subject='', regions=2, body=100, padding=4)])
        self.assertIn('| Matched C/C++ body bytes | 100 (10.00% of .text) |', text)
        self.assertIn('not C/C++ credit) | 40 (4.00% of .text) |', text)
        self.assertIn('| Original .text scaffold bytes | 856 |', text)
        self.assertEqual(status.coverage(catalog)['body'], 100)


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


if __name__ == '__main__':
    unittest.main()
