from pathlib import Path
import tempfile,unittest
from seed_shared_types import SharedTypes,code_view
from seed_shared_preprocessor import structural_views

class StructuralHeaders(unittest.TestCase):
 def index(self,text):
  temp=tempfile.TemporaryDirectory();self.addCleanup(temp.cleanup);root=Path(temp.name);(root/'fixture.hpp').write_text(text);return SharedTypes(root)
 def test_alternate_function_opening_keeps_record_fields(self):
  text='struct Cursor {\n#ifdef OP\n virtual operator bool() const {\n#else\n virtual bool Valid() const {\n#endif\n return true;\n }\n int Index;\n};\n';index=self.index(text);self.assertIn('Index',index.records['Cursor'].fields)
 def test_alternate_class_bases_remain_ambiguous(self):
  text='#ifdef DERIVED\nstruct Point : public Base {\n#else\nstruct Point {\n#endif\n int X;\n};\n';index=self.index(text);self.assertNotIn('Point',index.records);self.assertIn('Point',index.ambiguous)
 def test_larger_conditional_layout_is_not_selected(self):
  text='#ifdef DERIVED\nstruct Point : public Base {\n#else\nstruct Point {\n int X;\n#endif\n int Z;\n};\n';self.assertIn('Point',self.index(text).ambiguous)
 def test_equivalent_class_openings_merge(self):
  text='#ifdef A\nstruct Point {\n#else\nstruct Point {\n#endif\n int X;\n};\n';self.assertIn('X',self.index(text).records['Point'].fields)
 def test_nested_structural_choices(self):
  text='struct R {\n#ifdef A\n#ifdef B\n int F() {\n#else\n int G() {\n#endif\n#else\n int H() {\n#endif\n return 1; }\n int X;\n};\n';self.assertIn('X',self.index(text).records['R'].fields)
 def test_correlated_open_and_close_do_not_cross(self):
  text='#ifdef A\nstruct R {\n#endif\n#ifdef A\n int X; };\n#endif\n';views=structural_views(text,code_view);self.assertEqual(len(views),2);self.assertIn('R',self.index(text).ambiguous)
 def test_ifndef_is_negated_definedness(self):
  text='#ifdef A\nstruct R {\n#endif\n#ifndef A\nstruct R {\n#endif\n int X; };\n';self.assertIn('X',self.index(text).records['R'].fields)
 def test_elif_all_branches(self):
  text='#if defined(A)\nstruct R {\n#elif defined(B)\nstruct R {\n#else\nstruct R {\n#endif\n int X; };\n';self.assertIn('X',self.index(text).records['R'].fields)
 def test_defined_zero_differs_from_undefined(self):
  text='#if defined(A) && !A\nstruct R {\n int Y;\n#else\nstruct R {\n#endif\n int X; };\n';self.assertIn('R',self.index(text).ambiguous)
 def test_fixed_local_zero_flag(self):
  text='#define A 0\n#if defined(A) && !A\nstruct R {\n int Y;\n#else\nstruct R {\n#endif\n int X; };\n';self.assertEqual(set(self.index(text).records['R'].fields),{'X','Y'})
 def test_conditional_local_definition(self):
  text='#ifdef ALL\n#define A\n#endif\n#ifdef A\nstruct R {\n#else\nstruct R {\n#endif\n int X; };\n';self.assertIn('X',self.index(text).records['R'].fields)
 def test_undef_overrides_external_flag(self):
  text='#undef A\n#ifdef A\nstruct R {\n int Y;\n#else\nstruct R {\n#endif\n int X; };\n';self.assertEqual(set(self.index(text).records['R'].fields),{'X'})
 def test_header_guard_does_not_make_record_optional(self):
  text='#ifndef GUARD\n#define GUARD\n#ifdef A\nstruct R {\n#else\nstruct R {\n#endif\n int X; };\n#endif\n';self.assertIn('R',self.index(text).records)
 def test_offsets_and_newlines_are_preserved(self):
  text='#ifdef A\nstruct R {\n#else\nstruct R {\n#endif\n int X; };\n'
  for view in structural_views(text,code_view):self.assertEqual(len(view),len(text));self.assertEqual([i for i,c in enumerate(view) if c=='\n'],[i for i,c in enumerate(text) if c=='\n']);self.assertEqual(view.index('int X'),text.index('int X'))
 def test_comment_braces_do_not_change_structure(self):self.assertIn('R',self.index('/* } { */\nstruct R { int X; };').records)
 def test_string_braces_do_not_change_structure(self):self.assertIn('X',self.index('struct R { const char *F() { return "{"; } int X; };').records['R'].fields)
 def test_continued_directive_body_is_not_code(self):
  text='struct R {\n#define OPEN '+chr(92)+chr(10)+' { '+chr(92)+chr(10)+' {\n int X; };\n';self.assertIn('X',self.index(text).records['R'].fields)
 def test_method_uses_active_address_comment(self):
  text='struct R {\n#ifdef A\n virtual bool F() {\n#else\n virtual bool G() {\n#endif\n return true; }\n int H(); // 0x00402000\n};\n';record,method=self.index(text).method_at(0x402000);self.assertEqual((record.name,method.name),('R','H'))
 def test_variant_method_abi_is_not_chosen(self):
  text='struct R {\n#ifdef A\n int F(int x) { // 0x00402000\n#else\n int F(double x) { // 0x00402000\n#endif\n return 1; }\n int X; };\n';self.assertIsNone(self.index(text).method_at(0x402000))
 def test_unterminated_group_refuses(self):
  with self.assertRaisesRegex(ValueError,'Unterminated'):self.index('#ifdef A\nstruct R {};\n')
 def test_unmatched_endif_refuses(self):
  with self.assertRaisesRegex(ValueError,'Unmatched'):self.index('#endif\nstruct R {};\n')
 def test_duplicate_else_refuses(self):
  with self.assertRaisesRegex(ValueError,'Malformed'):structural_views('#if 1\n#else\n#else\n#endif\n',code_view)
 def test_elif_after_else_refuses(self):
  with self.assertRaisesRegex(ValueError,'Malformed'):structural_views('#if 1\n#else\n#elif A\n#endif\n',code_view)
 def test_unsupported_structural_expression_refuses(self):
  with self.assertRaisesRegex(ValueError,'Unsupported'):self.index('#if A + 1\nstruct R {\n#else\nstruct R {\n#endif\n};\n')
 def test_structural_value_macro_expansion_refuses(self):
  with self.assertRaisesRegex(ValueError,'value-macro'):self.index('#define A B\n#if A\nstruct R {\n#else\nstruct R {\n#endif\n};\n')
 def test_three_valid_defined_value_states_fit_three_state_budget(self):
  text='#if !defined(A)\nstruct R { int X;\n#elif !A\nstruct R { int X; int Y;\n#else\nstruct R { int X; int Y; int Z;\n#endif\n};\n';self.assertEqual(len(structural_views(text,code_view,max_variants=3)),3)
 def test_variant_limit_never_samples(self):
  text='#ifdef A\nstruct R {\n#else\nstruct R {\n#endif\n};\n'
  with self.assertRaisesRegex(ValueError,'Too many'):structural_views(text,code_view,max_variants=1)
 def test_false_constant_block_is_omitted(self):self.assertIn('R',self.index('#if 0\n {{{ broken\n#endif\nstruct R { int X; };').records)
 def test_ordinary_balanced_alternatives_retain_index_view(self):
  text='struct R {\n#ifdef A\n int X;\n#else\n int Y;\n#endif\n};\n';self.assertEqual(set(self.index(text).records['R'].fields),{'X','Y'})
 def test_type_only_include_fragment_does_not_abort_class_index(self):
  temp=tempfile.TemporaryDirectory();self.addCleanup(temp.cleanup);root=Path(temp.name);(root/'types.hpp').write_text('typedef int integer;\n#endif\n');(root/'record.hpp').write_text('struct R { int X; };\n');self.assertIn('R',SharedTypes(root).records)
 def test_whole_malformed_class_still_refuses(self):
  with self.assertRaisesRegex(ValueError,'Unbalanced'):self.index('struct R { int X;')
if __name__=='__main__':unittest.main()
