import unittest
import seed_candidate as seed
from seed_dword_partials import low24_store_hypotheses

class Low24Stores(unittest.TestCase):
    def body(self,slot='uint slot;',value='uint value;',statement='slot._0_3_ = (uint3)value;',prefix=''):
        return '{\n '+slot+'\n '+value+'\n '+prefix+'\n '+statement+'\n}'
    def rewrite(self,body,params=()):return low24_store_hypotheses(body,params,seed)
    def unchanged(self,body,params=()):self.assertEqual(self.rewrite(body,params),(body,[]))
    def test_normalizes_rhs_once_then_writes_exact_two_plus_one(self):
        text,notes=self.rewrite(self.body());self.assertIn('uint low24_value = (uint)(value) & 0x00ffffffU;',text);self.assertIn('*((ushort *)&(slot)) = (ushort)low24_value;',text);self.assertIn('((uchar *)&(slot))[2] = (uchar)(low24_value >> 16);',text);self.assertEqual(text.count('(uint)(value)'),1);self.assertNotIn('[3]',text);self.assertNotIn('._0_3_',text);self.assertIn('unproved',notes[0])
    def test_supported_known_dword_kinds(self):
        for kind in ('int','uint','undefined4','long','ulong','unsigned int','signed int','unsigned long','signed long'):
            with self.subTest(kind=kind):self.assertTrue(self.rewrite(self.body(slot=kind+' slot;'))[1])
    def test_rhs_parameter_is_known_word(self):self.assertTrue(self.rewrite(self.body(value=''),['uint value'])[1])
    def test_destination_parameter_remains_unsupported(self):self.unchanged(self.body(slot=''),['uint slot'])
    def test_rhs_same_slot_is_captured_before_stores(self):
        text,notes=self.rewrite(self.body(value='',statement='slot._0_3_ = (uint3)slot;'));self.assertIn('(uint)(slot) &',text);self.assertEqual(text.count('(uint)(slot)'),1)
    def test_unique_payload_name(self):
        text,notes=self.rewrite(self.body(prefix='uint low24_value;'));self.assertIn('uint low24_value_1 =',text)
    def test_multiple_stores_have_distinct_computed_payloads(self):
        text,notes=self.rewrite(self.body(statement='slot._0_3_ = (uint3)value;\n slot._0_3_ = (uint3)value;'));self.assertIn('uint low24_value =',text);self.assertIn('uint low24_value_1 =',text);self.assertIn('2 bounded24-bit',notes[0])
    def test_unrelated_source_and_trailing_comment_preserved(self):
        b=self.body(prefix='// keep this',statement='slot._0_3_ = (uint3)value; // keep tail');text,notes=self.rewrite(b);self.assertIn('// keep this',text);self.assertIn('// keep tail',text)
    def test_no_widened_store_or_uchar_array_added(self):
        text,notes=self.rewrite(self.body());self.assertNotIn('*((uint *)&',text);self.assertNotIn('uchar low24_value[',text);self.assertNotIn('memcpy',text)
    def test_unknown_destination_refuses(self):self.unchanged(self.body(slot=''))
    def test_unknown_rhs_refuses(self):self.unchanged(self.body(value=''))
    def test_narrow_wide_float_pointer_or_array_destination_refuses(self):
        for kind in ('char','short','undefined2','undefined8','longlong','double','float','int *'):
            with self.subTest(kind=kind):self.unchanged(self.body(slot=kind+' slot;'))
        self.unchanged(self.body(slot='uint slot[2];'))
    def test_narrow_wide_float_pointer_or_array_rhs_refuses(self):
        for kind in ('char','short','undefined8','longlong','double','float','int *'):
            with self.subTest(kind=kind):self.unchanged(self.body(value=kind+' value;'))
        self.unchanged(self.body(value='uint value[2];'))
    def test_qualified_objects_refuse(self):
        for kind in ('volatile uint','const uint','uint volatile','uint const'):
            with self.subTest(kind=kind):self.unchanged(self.body(slot=kind+' slot;'));self.unchanged(self.body(value=kind+' value;'))
    def test_shadowed_objects_refuse(self):self.unchanged(self.body(prefix='uint slot;'))
    def test_parameter_local_shadow_refuses(self):self.unchanged(self.body(),['uint value'])
    def test_comma_declaration_refuses(self):self.unchanged(self.body(slot='uint slot,other;'))
    def test_type_name_shadow_refuses(self):self.unchanged(self.body(prefix='uint uint3;'))
    def test_preprocessor_refuses(self):self.unchanged(self.body(prefix='#define SOMETHING 1'))
    def test_type_alias_declaration_refuses(self):self.unchanged(self.body(prefix='typedef unsigned int Other;'))
    def test_comment_within_selected_assignment_is_not_discarded(self):self.unchanged(self.body(statement='slot._0_3_ = (uint3)/*keep*/value;'))
    def test_used_assignment_result_refuses(self):self.unchanged(self.body(statement='value = (slot._0_3_ = (uint3)value);'))
    def test_unbraced_control_refuses(self):self.unchanged(self.body(statement='if (value)\n slot._0_3_ = (uint3)value;'))
    def test_label_ingress_refuses(self):self.unchanged(self.body(statement='label:\n slot._0_3_ = (uint3)value;'))
    def test_calls_mutations_or_compound_rhs_remain_unsupported(self):
        for expr in ('callee()','value++','++value','value+1','*value','value[0]'):
            with self.subTest(expr=expr):self.unchanged(self.body(statement='slot._0_3_ = (uint3)'+expr+';'))
    def test_other_offsets_widths_or_reads_remain_unsupported(self):
        for statement in ('slot._1_3_ = (uint3)value;','slot._0_6_ = value;','value = slot._0_3_;'):
            with self.subTest(statement=statement):self.unchanged(self.body(statement=statement))
    def test_existing_self_preserving_byte_forms_are_left_to_their_helper(self):
        for statement in ('slot._0_3_ = CONCAT12(1,(ushort)slot);','slot._0_3_ = (uint3)(ushort)slot;'):
            with self.subTest(statement=statement):self.unchanged(self.body(statement=statement))

if __name__=='__main__':unittest.main()
