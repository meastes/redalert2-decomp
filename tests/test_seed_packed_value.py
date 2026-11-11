"""Authored source/native fixtures, not original body acceptance evidence."""
from collections import defaultdict
import copy
import unittest
import capstone
import seed_candidate as seed
from seed_packed_value import bind_packed_copy,native_packed_copy
from seed_guarded_cfg import CFGRefusal

PARAMS=['int owner','uint *output','short *input','char flag']
BODY="""{
 uint high;
 uint low;
 int temp;
 high = 2;
 low = 3;
 temp = flag;
 _flag = CONCAT22((short)high,(short)low);
 *output = _flag;
 return;
}
"""

def native_graph(raw):
    decoder=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);decoder.detail=True
    nodes={i.address:i for i in decoder.disasm(bytes.fromhex(raw),0x1000)};edges={};pred=defaultdict(set)
    for at,i in nodes.items():
        targets=[] if i.mnemonic=='ret' else [at+i.size]
        if i.mnemonic.startswith('j'):
            targets=[i.operands[0].imm]
            if i.mnemonic!='jmp':targets.append(at+i.size)
        edges[at]=targets
        for x in targets:pred[x].add(at)
    return dict(entry=0x1000,nodes=nodes,edges=edges,pred=pred)

class PackedSource(unittest.TestCase):
    def bind(self,body=BODY,params=PARAMS,conv='__thiscall'):return bind_packed_copy(body,params,conv,seed)
    def refused(self,body=BODY,params=PARAMS,conv='__thiscall'):
        with self.assertRaises(CFGRefusal):self.bind(body,params,conv)
    def test_complete_packed_value_preserves_expression_and_parameter(self):
        b=self.bind();self.assertEqual(b['expression'],'CONCAT22((short)high,(short)low)');self.assertEqual(b['parameter_frame'],12);self.assertEqual(b['output_frame'],4);self.assertEqual(b['narrow_bytes'],1);self.assertEqual(b['stack_bytes'],12)
    def test_parameter_names_do_not_encode_native_offsets(self):
        b=self.bind(BODY.replace('flag','edge'),[p.replace('flag','edge') for p in PARAMS]);self.assertEqual(b['parameter_frame'],12);self.assertEqual(b['alias'],'_edge')
    def test_formal_order_changes_expected_physical_slots(self):
        b=self.bind(params=['int owner','char flag','uint *output','short *input']);self.assertEqual((b['parameter_frame'],b['output_frame']),(4,8))
    def test_short_parameter_view_supported_without_widening(self):self.assertEqual(self.bind(params=PARAMS[:-1]+['short flag'])['narrow_bytes'],2)
    def test_ordinary_word_parameter_is_not_a_narrow_alias(self):self.refused(params=PARAMS[:-1]+['uint flag'])
    def test_unknown_narrow_name_refuses(self):self.refused(BODY.replace('_flag','_unknown'))
    def test_declared_alias_refuses(self):self.refused(BODY.replace(' uint high;',' uint _flag;\n uint high;'))
    def test_shadowed_source_param_refuses(self):self.refused(BODY.replace(' uint high;',' char flag;\n uint high;'))
    def test_old_parameter_read_after_copy_refuses(self):self.refused(BODY.replace(' return;',' temp = flag;\n return;'))
    def test_old_parameter_address_escape_refuses(self):self.refused(BODY.replace(' temp = flag;',' consume(&flag);'))
    def test_extra_alias_read_refuses(self):self.refused(BODY.replace(' return;',' temp = _flag;\n return;'))
    def test_alias_address_escape_refuses(self):self.refused(BODY.replace('*output = _flag;','consume(&_flag);'))
    def test_nonadjacent_output_refuses(self):self.refused(BODY.replace(' *output',' temp = 0;\n *output'))
    def test_qualified_formal_refuses(self):
        for kind in ('volatile char','const char','char &','char *'):
            with self.subTest(kind=kind):self.refused(params=PARAMS[:-1]+[kind+' flag'])
    def test_nonmember_or_fastcall_refuses(self):
        for conv in ('__cdecl','__fastcall','__stdcall',''):
            with self.subTest(conv=conv):self.refused(conv=conv)
    def test_uninitialized_half_refuses(self):self.refused(BODY.replace(' high = 2;\n',''))
    def test_conditional_only_half_definition_refuses(self):self.refused(BODY.replace(' high = 2;',' if (flag) {\n high = 2;\n }'))
    def test_jump_or_loop_cannot_bypass_a_source_definition(self):
        self.refused(BODY.replace(' high = 2;',' goto done;\n high = 2;').replace(' _flag =',' done:;\n _flag ='))
        self.refused(BODY.replace(' high = 2;',' while (flag) { high = 2; }'))
    def test_sibling_branch_definition_does_not_dominate(self):
        body=BODY.replace(' high = 2;',' if (flag == 1) {\n high = 2;\n }')
        body=body.replace(' _flag = CONCAT22((short)high,(short)low);\n *output = _flag;', ' if (flag == 2) {\n _flag = CONCAT22((short)high,(short)low);\n *output = _flag;\n }')
        self.refused(body)
    def test_pointer_float_or_unknown_halves_refuse(self):
        for text in ('uint *high;','double high;','Mystery high;'):
            with self.subTest(text=text):self.refused(BODY.replace('uint high;',text))
    def test_effectful_or_unbound_half_refuses(self):
        for text in ('callee()','high++','missing'):
            with self.subTest(text=text):self.refused(BODY.replace('(short)high',text))
    def test_wrong_concat_width_refuses(self):self.refused(BODY.replace('CONCAT22','CONCAT13'))
    def test_commented_span_is_not_discarded(self):self.refused(BODY.replace(' *output',' /*keep*/ *output'))
    def test_quoted_half_is_not_interpreted_as_an_integer(self):self.refused(BODY.replace('(short)high','"pointer"'))
    def test_unbraced_control_refuses(self):self.refused(BODY.replace(' _flag =',' if (temp)\n _flag ='))
    def test_used_producer_assignment_refuses(self):self.refused(BODY.replace(' _flag =',' temp = _flag ='))
    def test_output_pointer_must_point_to_a_word(self):self.refused(params=['int owner','short *output','short *input','char flag'])
    def test_output_must_be_a_formal_with_native_identity(self):self.refused(BODY.replace(' uint high;',' uint *sink;\n uint high;').replace('*output','*sink'))
    def test_wide_physical_formal_refuses(self):self.refused(params=['longlong owner']+PARAMS[1:])
    def test_preprocessor_or_type_alias_scope_refuses(self):self.refused(BODY.replace(' uint high;',' #define FLAG 1\n uint high;'));self.refused(BODY.replace(' uint high;',' typedef unsigned int Other;\n uint high;'))

class PackedNative(unittest.TestCase):
    def fixture(self,old='807c240c00',low='66894c240c',high='668954240e',pointer='8b442404',read='8b4c240c',output='8908',ret='c20c00',gap=''):
        return native_graph(old+low+high+pointer+read+gap+output+ret),dict(parameter_frame=12,output_frame=4,narrow_bytes=1,stack_bytes=12)
    def proof(self,**kwargs):return native_packed_copy(*self.fixture(**kwargs))
    def refuses(self,**kwargs):
        with self.assertRaises(CFGRefusal):self.proof(**kwargs)
    def test_authored_two_words_and_complete_output(self):
        p=self.proof();self.assertEqual({x['frame'] for x in p['word_stores']},{12,14});self.assertFalse(p['original_parameter_object_widened']);self.assertFalse(p['full_source_alias_correspondence_claimed'])
    def test_missing_narrow_input_read_refuses(self):self.refuses(old='90')
    def test_narrow_read_after_wide_definition_refuses(self):self.refuses(gap='807c240c00')
    def test_whole_word_store_instead_of_two_words_refuses(self):self.refuses(low='894c240c')
    def test_overlapping_or_gapped_words_refuse(self):
        for high in ('668954240d','6689542410'):
            with self.subTest(high=high):self.refuses(high=high)
    def test_partial_packet_read_refuses(self):self.refuses(read='668b4c240c')
    def test_wrong_packet_read_refuses(self):self.refuses(read='8b4c2410')
    def test_wrong_output_formal_refuses(self):self.refuses(pointer='8b442408')
    def test_narrow_output_store_refuses(self):self.refuses(output='668908')
    def test_implicit_push_overwrite_of_packet_refuses(self):self.refuses(pointer='8b442404 83c410 50 83ec0c')
    def test_output_pointer_arithmetic_refuses(self):self.refuses(gap='83c004')
    def test_output_data_partial_clobber_refuses(self):self.refuses(gap='6631c9')
    def test_additional_nonframe_write_refuses(self):self.refuses(gap='8913')
    def test_call_or_io_in_tail_refuses(self):
        for gap in ('e800000000','ee'):
            with self.subTest(gap=gap):self.refuses(gap=gap)
    def test_wrong_cleanup_refuses(self):self.refuses(ret='c20400')
    def test_alternate_ingress_to_packet_read_refuses(self):
        g,b=self.fixture();read=next(at for at,i in g['nodes'].items() if i.mnemonic=='mov' and i.op_str=='ecx, dword ptr [esp + 0xc]');g['pred'][read].add(g['entry'])
        with self.assertRaises(CFGRefusal):native_packed_copy(g,b)
    def test_register_copy_of_verified_packet_is_allowed(self):self.assertTrue(self.proof(gap='89cb 89d9')['normal_MS_i386_input_contract_qualified'])

if __name__=='__main__':unittest.main()
