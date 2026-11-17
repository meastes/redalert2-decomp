"""Authored synthetic i386 fixtures, not original game bytes or acceptance evidence."""
from collections import defaultdict
from pathlib import Path
from types import SimpleNamespace
import struct
import unittest
import capstone
from seed_guarded_cfg import X,CFGRefusal
from seed_frame_equations import AffineFrames
from seed_capture_receivers import Receivers
from seed_capture_native import capture_candidate,linear_next,hidden_result_buffer


def graph(raw,entry=0x1000):
    decoder=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);decoder.detail=True
    nodes={ins.address:ins for ins in decoder.disasm(bytes.fromhex(raw),entry)};edges={};pred=defaultdict(set)
    for at,ins in nodes.items():
        targets=[] if ins.mnemonic=='ret' else [at+ins.size]
        if ins.mnemonic.startswith('j'):
            targets=[ins.operands[0].imm]
            if ins.mnemonic!='jmp':targets.append(at+ins.size)
        edges[at]=targets
        for target in targets:pred[target].add(at)
    return dict(entry=entry,nodes=nodes,edges=edges,pred=pred)


def contract():return dict(roots={0:('input',0,'Owner')},offsets={4:('input',1,'Other')},stack_bytes=4)


def machine(g):return Receivers(Path('.'),{'regions':[]},None,None,g,contract())

class ReceiverFlow(unittest.TestCase):
    def state(self,raw):
        g=graph(raw);m=machine(g)
        for at,i in g['nodes'].items():
            if i.mnemonic=='call':m.known[at]=0
        states,frames,steps=m.flow();return g,m,states,frames
    def test_saved_this_and_stack_argument_have_distinct_origins(self):
        g,m,s,f=self.state('89ce 8b7c2404 c3');last=max(g['nodes']);self.assertEqual(s[last][X.X86_REG_ESI],('input',0,'Owner'));self.assertEqual(s[last][X.X86_REG_EDI],('input',1,'Other'))
    def test_partial_register_write_kills_receiver(self):
        g,m,s,f=self.state('89ce 6631f6 c3');self.assertIsNone(s[max(g['nodes'])][X.X86_REG_ESI])
    def test_call_kills_volatile_registers_and_preserves_saved_receiver(self):
        g,m,s,f=self.state('89ce 89c8 e800000000 c3');last=max(g['nodes']);self.assertIsNone(s[last][X.X86_REG_EAX]);self.assertIsNone(s[last][X.X86_REG_ECX]);self.assertEqual(s[last][X.X86_REG_ESI],('input',0,'Owner'))
    def test_preceding_argument_write_refuses_origin(self):
        g,m,s,f=self.state('89442404 8b7c2404 c3');self.assertIsNone(s[max(g['nodes'])][X.X86_REG_EDI])
    def test_preceding_partial_argument_write_refuses_origin(self):
        g,m,s,f=self.state('6689442406 8b7c2404 c3');self.assertIsNone(s[max(g['nodes'])][X.X86_REG_EDI])
    def test_preceding_argument_address_escape_refuses_origin(self):
        g,m,s,f=self.state('8d442404 8b7c2404 c3');self.assertIsNone(s[max(g['nodes'])][X.X86_REG_EDI])
    def test_argument_copy_reuse_after_saved_load_is_allowed(self):
        g,m,s,f=self.state('8b7c2404 89442404 c3');self.assertEqual(s[max(g['nodes'])][X.X86_REG_EDI],('input',1,'Other'))
    def test_nonoverlapping_frame_write_keeps_origin(self):
        g,m,s,f=self.state('89442408 8b7c2404 c3');self.assertEqual(s[max(g['nodes'])][X.X86_REG_EDI],('input',1,'Other'))
    def test_reload_after_argument_copy_reuse_is_unknown(self):
        g,m,s,f=self.state('8b7c2404 89442404 8b742404 c3');last=max(g['nodes']);self.assertEqual(s[last][X.X86_REG_EDI],('input',1,'Other'));self.assertIsNone(s[last][X.X86_REG_ESI])
    def test_matching_receiver_and_vptr_enables_family(self):
        g=graph('89ce 8b16 89f1 ff5248 c3');m=machine(g);seen=[]
        def family(root,slot):seen.append((root,slot));return dict(pop=0,record_result=None,slot=slot)
        m.family=family;m.solve();self.assertEqual(seen,[(('input',0,'Owner'),18)])
    def test_mismatched_receiver_and_vptr_refuses_family(self):
        g=graph('89ce 8b7c2404 8b16 89f9 ff5248 c3');m=machine(g);seen=[];m.family=lambda root,slot:seen.append((root,slot));m.solve();self.assertEqual(seen,[])
    def test_nonzero_vptr_load_is_unknown(self):
        g,m,s,f=self.state('89ce 8b5604 c3');self.assertIsNone(s[max(g['nodes'])][X.X86_REG_EDX])
    def test_record_getter_return_is_typed_only_with_family_fact(self):
        # The authored fixture supplies a hidden pointer push and matching pop.
        g=graph('89ce 8b16 89f1 50 ff5248 89c3 c3');m=machine(g);call=next(at for at,i in g['nodes'].items() if i.mnemonic=='call');m.known[call]=4;m.families[call]=dict(pop=4,record_result='Coord',slot=18);s,f,n=m.flow();self.assertEqual(s[max(g['nodes'])][X.X86_REG_EBX],('return',call,('input',0,'Owner'),18,'Coord'))
    def test_scalar_call_return_is_not_a_record_pointer(self):
        g,m,s,f=self.state('e800000000 89c3 c3');self.assertIsNone(s[max(g['nodes'])][X.X86_REG_EBX])

class NativeCapture(unittest.TestCase):
    def fixture(self,store='894c2420',integer='db442420',capture='dd5c2410',between='',reload='dd442410',square='dc4c2410',after='dd1c24'):
        prefix=store+integer+capture+between+reload+square+after
        raw=bytes.fromhex(prefix);call=0x1000+len(raw);raw+=b'\xe8'+struct.pack('<i',0x2000-call-5)+b'\xc3';g=graph(raw.hex());frames=AffineFrames(g,{call:0})
        value=('difference',0x950,('word',('return',0x900,('input',0,'Owner'),18,'Coord'),0),('word',('return',0x910,('input',1,'Other'),18,'Coord'),0))
        incoming={0x1000:{X.X86_REG_ECX:value}};binding=dict(source_getters=[dict(parameter_index=0,slot=72),dict(parameter_index=1,slot=72)],scalar_callee=0x2000)
        m=SimpleNamespace(families={},accepted=lambda entry:dict(source='synthetic.cpp',source_sha256='synthetic-source',body_sha256='synthetic-body'))
        return g,frames,incoming,m,binding,0x1000+len(bytes.fromhex(store))
    def witness(self,**kwargs):return capture_candidate(*self.fixture(**kwargs))
    def refuses(self,**kwargs):
        with self.assertRaises(CFGRefusal):self.witness(**kwargs)
    def test_authored_complete_capture_witness(self):
        p=self.witness();self.assertEqual(p['capture_displacement'],16);self.assertEqual(p['reload_displacement'],16);self.assertEqual(p['getter_calls'],[0x900,0x910])
    def test_qword_integer_load_refuses(self):self.refuses(integer='df6c2420')
    def test_partial_integer_spill_refuses(self):self.refuses(store='66894c2420')
    def test_nonframe_integer_spill_refuses(self):self.refuses(store='8909')
    def test_different_integer_spill_refuses(self):self.refuses(store='894c2424')
    def test_float_capture_refuses(self):self.refuses(capture='d95c2410')
    def test_changed_x87_value_refuses(self):self.refuses(capture='d9e0 dd5c2410')
    def test_different_reload_bytes_refuse(self):self.refuses(reload='dd442418')
    def test_different_multiply_bytes_refuse(self):self.refuses(square='dc4c2414')
    def test_explicit_capture_overwrite_refuses(self):self.refuses(between='89442410')
    def test_partial_capture_overwrite_refuses(self):self.refuses(between='6689442416')
    def test_unrelated_frame_write_is_allowed(self):self.assertEqual(self.witness(between='89442420')['capture_displacement'],16)
    def test_nonframe_alias_write_refuses(self):self.refuses(between='8901')
    def test_unknown_intervening_call_refuses(self):self.refuses(between='e800000000')
    def test_wrong_scalar_consumer_refuses(self):
        args=list(self.fixture());args[4]['scalar_callee']=0x2001
        with self.assertRaises(CFGRefusal):capture_candidate(*args)
    def test_wrong_source_root_refuses(self):
        args=list(self.fixture());args[4]['source_getters'][0]['parameter_index']=1
        with self.assertRaises(CFGRefusal):capture_candidate(*args)
    def test_wrong_source_slot_refuses(self):
        args=list(self.fixture());args[4]['source_getters'][0]['slot']=76
        with self.assertRaises(CFGRefusal):capture_candidate(*args)
    def test_unknown_integer_value_refuses(self):
        args=list(self.fixture());args[2][0x1000][X.X86_REG_ECX]=None
        with self.assertRaises(CFGRefusal):capture_candidate(*args)
    def test_alternate_reload_ingress_refuses(self):
        args=list(self.fixture());g=args[0];reload=next(at for at,i in g['nodes'].items() if i.mnemonic=='fld');g['pred'][reload].add(g['entry'])
        with self.assertRaises(CFGRefusal):capture_candidate(*args)
    def test_same_bytes_with_different_esp_displacement(self):
        self.assertEqual(self.witness(between='83ec08',reload='dd442418',square='dc4c2418',after='83c408 dd1c24')['reload_displacement'],24)
    def test_same_spelling_after_esp_change_is_different_bytes(self):self.refuses(between='83ec08',after='83c408 dd1c24')


class HiddenBuffer(unittest.TestCase):
    def fixture(self,disp=48):
        g=graph('8d4424%02x 50 dd5c2410 ff5248 c3'%disp);call=next(at for at,i in g['nodes'].items() if i.mnemonic=='call');capture=next(at for at,i in g['nodes'].items() if i.mnemonic=='fstp');frames=AffineFrames(g,{call:4});return g,frames,call,capture,g['nodes'][capture].operands[0]
    def test_authored_output_buffer_above_capture(self):self.assertEqual(hidden_result_buffer(*self.fixture())['relative_to_capture'],36)
    def test_overlapping_output_buffer_refuses(self):
        with self.assertRaises(CFGRefusal):hidden_result_buffer(*self.fixture(16))
    def test_output_buffer_below_capture_refuses_without_extent_guess(self):
        with self.assertRaises(CFGRefusal):hidden_result_buffer(*self.fixture(4))
    def test_unknown_output_register_definition_refuses(self):
        args=list(self.fixture());g=args[0];i=g['nodes'][g['entry']];d=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);d.detail=True;g['nodes'][g['entry']]=next(d.disasm(bytes.fromhex('8b442430'),g['entry']))
        with self.assertRaises(CFGRefusal):hidden_result_buffer(*args)
    def test_alternate_buffer_setup_ingress_refuses(self):
        args=list(self.fixture());g=args[0];push=next(at for at,i in g['nodes'].items() if i.mnemonic=='push');g['pred'][push].add(args[2])
        with self.assertRaises(CFGRefusal):hidden_result_buffer(*args)

if __name__=='__main__':unittest.main()
