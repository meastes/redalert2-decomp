"""Source-binding tests; native flags below are synthetic, never acceptance evidence."""
import copy
import unittest
import seed_candidate as seed
from seed_capture_source import bind_source,rewrite_capture,CaptureRefusal

PARAMS=['int *owner','int *other']
BODY="""{
    uint delta;
    undefined4 high;
    int *low;
    int *a;
    int *b;
    int *alias;
    int buffer;
    alias = other;
    a = (int *)(**(code **)(*alias + 0x48))();
    b = (int *)(**(code **)(*owner + 0x48))(&buffer);
    delta = *a - *b;
    FUN_004ba410((double)CONCAT44(high,low) * (double)CONCAT44(high,low));
}
"""

class CaptureSource(unittest.TestCase):
    def bind(self,body=BODY,params=PARAMS):return bind_source(body,params,seed)
    def refuses(self,body,pattern=None,params=PARAMS):
        with self.assertRaises(CaptureRefusal) if pattern is None else self.assertRaisesRegex(CaptureRefusal,pattern):self.bind(body,params)
    def proof(self,binding):
        return dict(source_body_sha256=binding['source_body_sha256'],signed_dword_capture_verified=True,same_eight_byte_reload_verified=True,source_roots_correspond=True,frozen_source_verified=True)
    def test_positive_binds_source_parameter_order(self):
        b=self.bind();self.assertEqual([g['parameter_index'] for g in b['source_getters']],[1,0]);self.assertEqual(b['delta'],'delta');self.assertEqual(b['scalar_callee'],0x4ba410)
    def test_binding_grants_no_emission_or_acceptance(self):
        b=self.bind();self.assertFalse(b['source_emission_allowed']);self.assertFalse(b['alias_closure_claimed']);self.assertFalse(b['body_acceptance_claimed'])
    def test_renamed_identifiers_have_same_roles(self):
        s=BODY.replace('delta','distance_word').replace('high','upper_word').replace('low','lower_word');self.assertEqual(self.bind(s)['delta'],'distance_word')
    def test_square_spans_are_complete_leaves(self):
        b=self.bind();self.assertEqual([BODY[a:z] for a,z in b['square_spans']],['(double)CONCAT44(high,low)']*2)
    def test_native_evidence_is_required(self):
        with self.assertRaisesRegex(CaptureRefusal,'Missing native'):rewrite_capture(BODY,self.bind(),{},seed)
    def test_each_native_qualification_is_required(self):
        b=self.bind()
        for key in ('signed_dword_capture_verified','same_eight_byte_reload_verified','source_roots_correspond','frozen_source_verified'):
            p=self.proof(b);p[key]=False
            with self.subTest(key=key),self.assertRaises(CaptureRefusal):rewrite_capture(BODY,b,p,seed)
    def test_stale_source_refuses(self):
        b=self.bind()
        with self.assertRaisesRegex(CaptureRefusal,'stale'):rewrite_capture(BODY+' ',b,self.proof(b),seed)
    def test_native_source_pin_must_match(self):
        b=self.bind();p=self.proof(b);p['source_body_sha256']='0'*64
        with self.assertRaisesRegex(CaptureRefusal,'different source'):rewrite_capture(BODY,b,p,seed)
    def test_synthetic_rewrite_preserves_getters_and_real_difference(self):
        b=self.bind();s,notes=rewrite_capture(BODY,b,self.proof(b),seed);self.assertIn('double captured_delta;',s);self.assertIn('delta = *a - *b;\n    captured_delta = (double)(int)delta;',s);self.assertIn('FUN_004ba410(captured_delta * captured_delta);',s);self.assertEqual(s.count('(**(code **)(*'),2);self.assertNotIn('undefined4 high;',s);self.assertIn('int *low;',s);self.assertIn('Scratch hypothesis',notes[0]);self.assertIn('unproved',notes[0])
    def test_real_extra_high_use_preserved(self):
        text=BODY.replace('    alias =','    high = 7;\n    alias =');b=self.bind(text);s,_=rewrite_capture(text,b,self.proof(b),seed);self.assertIn('undefined4 high;',s);self.assertIn('high = 7;',s)
    def test_new_local_avoids_identifier_collision(self):
        text=BODY.replace('    uint delta;','    uint delta;\n    double captured_delta;');b=self.bind(text);s,_=rewrite_capture(text,b,self.proof(b),seed);self.assertIn('double captured_delta_1;',s)
    def test_comments_outside_selected_expressions_are_preserved(self):
        text=BODY.replace('    alias =','    // preserve me\n    alias =');b=self.bind(text);s,_=rewrite_capture(text,b,self.proof(b),seed);self.assertIn('// preserve me',s)
    def test_different_packed_operands_refuse(self):self.refuses(BODY.replace('CONCAT44(high,low));','CONCAT44(high,delta));'))
    def test_third_packed_read_refuses(self):self.refuses(BODY.replace('    alias =','    FUN_00400000(CONCAT44(high,low));\n    alias ='))
    def test_nonsquare_refuses(self):self.refuses(BODY.replace(') * (double)',') + (double)'))
    def test_comment_inside_selected_square_refuses(self):self.refuses(BODY.replace(') * (double)',') /*retain*/ * (double)'))
    def test_comma_scalar_argument_refuses(self):self.refuses(BODY.replace('CONCAT44(high,low));','CONCAT44(high,low), 1);'))
    def test_nested_scalar_containers_refuse(self):self.refuses(BODY.replace('FUN_004ba410(', 'FUN_00400000(FUN_004ba410(').replace('CONCAT44(high,low));','CONCAT44(high,low)));'))
    def test_nonlocal_high_refuses(self):self.refuses(BODY.replace('    undefined4 high;\n',''),params=PARAMS+['undefined4 high'])
    def test_wide_high_refuses(self):self.refuses(BODY.replace('undefined4 high','undefined8 high'))
    def test_volatile_delta_refuses(self):self.refuses(BODY.replace('uint delta','volatile uint delta'))
    def test_wide_difference_refuses(self):self.refuses(BODY.replace('uint delta','longlong delta'))
    def test_nonword_pointee_refuses(self):self.refuses(BODY.replace('int *a;','short *a;'))
    def test_pointer_to_pointer_difference_refuses(self):self.refuses(BODY.replace('int *a;','int **a;'))
    def test_array_difference_refuses(self):self.refuses(BODY.replace('uint delta;','uint delta[2];'))
    def test_comma_declaration_refuses(self):self.refuses(BODY.replace('uint delta;','uint delta,extra;'))
    def test_shadowed_declaration_refuses(self):self.refuses(BODY.replace('uint delta;','uint delta;\n    uint delta;'))
    def test_intervening_getter_statement_refuses(self):self.refuses(BODY.replace('    b =','    low = 0;\n    b ='))
    def test_difference_must_follow_both_getters(self):self.refuses(BODY.replace('    delta = *a - *b;','    low = 0;\n    delta = *a - *b;'))
    def test_reversed_getter_pointer_order_refuses(self):self.refuses(BODY.replace('delta = *a - *b','delta = *b - *a'))
    def test_duplicate_difference_refuses(self):self.refuses(BODY.replace('    FUN_004ba410','    delta = *a - *b;\n    FUN_004ba410'))
    def test_direct_delta_reassignment_refuses(self):self.refuses(BODY.replace('    FUN_004ba410','    delta = 9;\n    FUN_004ba410'))
    def test_compound_delta_writes_refuse(self):
        for op in ('+=','-=','*=','/=','%=','&=','|=','^=','<<=','>>='):
            with self.subTest(op=op):self.refuses(BODY.replace('    FUN_004ba410','    delta '+op+' 1;\n    FUN_004ba410'))
    def test_delta_escape_refuses(self):self.refuses(BODY.replace('    FUN_004ba410','    FUN_00400000(&delta);\n    FUN_004ba410'))
    def test_delta_increment_refuses(self):
        for expr in ('delta++','++delta','delta--','--delta'):
            with self.subTest(expr=expr):self.refuses(BODY.replace('    FUN_004ba410','    '+expr+';\n    FUN_004ba410'))
    def test_branch_between_capture_and_use_refuses(self):self.refuses(BODY.replace('    FUN_004ba410','    if (delta) goto end;\n    FUN_004ba410').replace('\n}','\nend:;\n}'))
    def test_changed_scope_refuses(self):self.refuses(BODY.replace('    FUN_004ba410','    }\n    {\n    FUN_004ba410'))
    def test_alias_reassignment_refuses(self):self.refuses(BODY.replace('    a =','    alias = owner;\n    a ='))
    def test_compound_root_writes_refuse(self):
        for name in ('alias','owner','other'):
            with self.subTest(name=name):self.refuses(BODY.replace('    alias =','    '+name+' += 1;\n    alias ='))
    def test_parameter_reassignment_refuses(self):self.refuses(BODY.replace('    alias =','    owner = other;\n    alias ='))
    def test_parameter_address_escape_refuses(self):self.refuses(BODY.replace('    alias =','    FUN_00400000(&owner);\n    alias ='))
    def test_alias_cycle_refuses(self):self.refuses(BODY.replace('alias = other','alias = low').replace('    alias =','    low = alias;\n    alias ='))
    def test_unknown_root_refuses(self):self.refuses(BODY.replace('*alias +','*low +'))
    def test_complex_getter_argument_refuses(self):self.refuses(BODY.replace('(&buffer)','(buffer + 1)'))
    def test_unaligned_slot_refuses(self):self.refuses(BODY.replace('0x48','0x49'))
    def test_oversized_slot_refuses(self):self.refuses(BODY.replace('0x48','0x2000'))

if __name__=='__main__':unittest.main()
