"""Native proof refusals use authored synthetic x86, never target bytes."""
import hashlib
import struct
import unittest
from types import SimpleNamespace
import capstone
import seed_candidate as seed
from seed_callee_abi import AbiRefusal, accepted_signature, native_member_call, source_arguments

class Fixture:
    def __init__(self, prefix=None, args=None, setup=b'\x8b\xce', callee=b'\xc2\x14\x00', suffix=b'\x5e\xc3'):
        self.callee = callee
        prefix = b'\x56\x8b\xf1\xeb\x00' if prefix is None else prefix
        args = b'\x6a\x01'*5 if args is None else args
        before = prefix+args+setup
        self.caller = before+b'\xe8'+struct.pack('<i', 0x1000-(0x2000+len(before)+5))+suffix
        self.decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32); self.decoder.detail=True
        self.data = {0x1000:self.callee, 0x2000:self.caller}
    def original(self, address, length):
        for base, raw in self.data.items():
            if base <= address and address+length <= base+len(raw):return raw[address-base:address-base+length]
        raise ValueError('read outside fixture')
    def function(self, entry):
        raw=self.data[entry]
        return None, dict(contiguous=True,ranges=[dict(start='%08x'%entry,end_inclusive='%08x'%(entry+len(raw)-1),bytes_hex=raw.hex())])
    def prove(self, **kwargs):
        return native_member_call(self,self,0x2000,0x1000,5,kwargs.get('digest',hashlib.sha256(self.callee).hexdigest()))

class NativeAbiTests(unittest.TestCase):
    def test_real_saved_receiver_and_five_dword_pushes(self):
        proof=Fixture().prove();self.assertEqual(len(proof['pushes']),5);self.assertEqual(len(proof['returns']),1)
    def test_wrong_cleanup(self):
        with self.assertRaises(AbiRefusal):Fixture(callee=b'\xc2\x10\x00').prove()
    def test_plain_return(self):
        with self.assertRaises(AbiRefusal):Fixture(callee=b'\xc3').prove()
    def test_operand_size_return(self):
        with self.assertRaises(AbiRefusal):Fixture(callee=b'\x66\xc2\x14\x00').prove()
    def test_native_body_hash(self):
        with self.assertRaises(AbiRefusal):Fixture().prove(digest='0'*64)
    def test_missing_push(self):
        with self.assertRaises(AbiRefusal):Fixture(args=b'\x6a\x01'*4).prove()
    def test_extra_push(self):
        with self.assertRaises(AbiRefusal):Fixture(args=b'\x6a\x01'*6).prove()
    def test_word_push(self):
        with self.assertRaises(AbiRefusal):Fixture(args=b'\x66\x6a\x01'+b'\x6a\x01'*4).prove()
    def test_receiver_not_saved(self):
        with self.assertRaises(AbiRefusal):Fixture(setup=b'\x8b\xc8').prove()
    def test_receiver_partial_write(self):
        with self.assertRaises(AbiRefusal):Fixture(args=b'\x66\xbe\x01\x00'+b'\x6a\x01'*5).prove()
    def test_ecx_changed_before_capture(self):
        with self.assertRaises(AbiRefusal):Fixture(prefix=b'\x31\xc9\x56\x8b\xf1\xeb\x00').prove()
    def test_call_before_capture(self):
        with self.assertRaises(AbiRefusal):Fixture(prefix=b'\xff\xd0\x56\x8b\xf1\xeb\x00').prove()
    def test_stack_adjust_in_argument_prefix(self):
        with self.assertRaises(AbiRefusal):Fixture(args=b'\x83\xec\x04'+b'\x6a\x01'*5).prove()
    def test_stack_store_overwrites_argument(self):
        with self.assertRaises(AbiRefusal):Fixture(args=b'\x6a\x01'*5+b'\xc7\x04\x24\x00\x00\x00\x00').prove()
    def test_indirect_callee_branch(self):
        with self.assertRaises(AbiRefusal):Fixture(callee=b'\xff\xe0').prove()
    def test_callee_branch_outside_extent(self):
        with self.assertRaises(AbiRefusal):Fixture(callee=b'\xeb\x05').prove()
    def test_inventory_bytes_not_original(self):
        f=Fixture();old=f.function
        def bad(entry):
            d,row=old(entry);row['ranges'][0]['bytes_hex']='90'*len(f.data[entry]);return d,row
        f.function=bad
        with self.assertRaises(AbiRefusal):f.prove()

class SignatureTests(unittest.TestCase):
    source='double Owner::Cost(Cell **from, Cell **to, bool bridge, int move, Foot *unused) { return 1.0; }'
    symbol='?Cost@Owner@@QAENPAPAVCell@@0_NHPAVFoot@@@Z'
    def parse(self, source=None, symbol=None):return accepted_signature(source or self.source,symbol or self.symbol,seed)
    def test_only_pointer_forward_declarations(self):
        params,forwards=self.parse();self.assertEqual(forwards,['Cell','Foot']);self.assertEqual(params[-1],'::Foot * unused')
    def test_used_trailing_argument(self):
        with self.assertRaises(AbiRefusal):self.parse(self.source.replace('return 1.0','return unused != 0'))
    def test_comment_does_not_count_as_use(self):
        self.parse(self.source.replace('return 1.0','/* unused */ return 1.0'))
    def test_mismatched_accepted_symbol(self):
        with self.assertRaises(AbiRefusal):self.parse(symbol='?Other@Owner@@QAENPAPAVCell@@0_NHPAVFoot@@@Z')
    def test_record_by_value(self):
        with self.assertRaises(AbiRefusal):self.parse(self.source.replace('int move','Cell move'))
    def test_wide_value(self):
        with self.assertRaises(AbiRefusal):self.parse(self.source.replace('int move','double move'))
    def test_reference_argument(self):
        with self.assertRaises(AbiRefusal):self.parse(self.source.replace('int move','int &move'))
    def test_ambiguous_definition(self):
        with self.assertRaises(AbiRefusal):self.parse(self.source+'\n'+self.source)
    def test_numeric_float_argument_refused(self):
        params,_=self.parse()
        with self.assertRaises(AbiRefusal):source_arguments('{\n  float distance;\n  int *a;\n  int *b;\n  int *foot;\n}',[],['a','b','1','distance','foot'],params,seed)
    def test_unchanged_positional_pointer_integer_arguments(self):
        params,_=self.parse();source_arguments('{\n  int *a;\n  int *b;\n  int move;\n  int *foot;\n}',[],['a','b','1','move','foot'],params,seed)

class ProviderTests(unittest.TestCase):
    def setUp(self):
        import tempfile
        from pathlib import Path
        import seed_callee_abi as abi
        self.tmp=tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.root=Path(self.tmp.name)
        self.fixture=Fixture()
        self.path='src/recovered/classes/Owner/Cost_00001000.cpp'
        path=self.root/self.path;path.parent.mkdir(parents=True);path.write_text(SignatureTests.source)
        directory=self.root/'private/original-target-20261020/inventory/pass';(directory/'decompiled').mkdir(parents=True);(directory/'decompiled/00002000.c').write_text('frozen caller')
        self.fixture.passes=[directory]
        self.row=dict(entry=0x1000,source=self.path,symbol=SignatureTests.symbol,body_sha256=hashlib.sha256(self.fixture.callee).hexdigest())
        abi._CONTEXTS[self.root.resolve()]=({0x1000:[self.row]},self.fixture)
        self.addCleanup(abi._CONTEXTS.pop,self.root.resolve())
        self.body="{\n  int *a;\n  int *b;\n  int move;\n  int *foot;\n  FUN_00001000(a, b, 1, move, foot);\n}"
        self.old=('float10','__thiscall',['int receiver','int *a','int *b','char bridge','float move'])
    def view(self, **kw):
        import seed_callee_abi as abi
        return abi.accepted_member_view(self.root,self.fixture,0x2000,0x1000,kw.get('body',self.body),['int receiver'],kw.get('old',self.old),kw.get('this_name','receiver'),kw.get('accepted',{0x1000:self.path}),seed)
    def test_provider_positive_keeps_every_argument(self):
        result=self.view();self.assertIsNotNone(result);self.assertEqual(len(result['prototype'][2]),6)
        body,decl=seed.declare_callee(self.body,'FUN_00001000',result['prototype'],'receiver')
        self.assertIn('((Callee_00001000*)(receiver))->FUN_00001000',body)
        for arg in ('a','b','move','foot'):self.assertIn(')('+arg+')',body)
    def test_orphan_source_refused(self):self.assertIsNone(self.view(accepted={}))
    def test_accepted_map_disagreement_refused(self):self.assertIsNone(self.view(accepted={0x1000:'other.cpp'}))
    def test_unknown_incoming_receiver_refused(self):self.assertIsNone(self.view(this_name=None))
    def test_convention_disagreement_refused(self):self.assertIsNone(self.view(old=('float10','__cdecl',self.old[2])))
    def test_return_register_change_refused(self):self.assertIsNone(self.view(old=('int','__thiscall',self.old[2])))
    def test_source_call_count_ambiguity_refused(self):self.assertIsNone(self.view(body=self.body.replace('FUN_00001000(a, b, 1, move, foot);','FUN_00001000(a, b, 1, move, foot);\n FUN_00001000(a, b, 1, move, foot);')))
    def test_wrong_native_cleanup_refused(self):
        self.fixture.data[0x1000]=b'\xc2\x10\x00';self.assertIsNone(self.view())
    def test_float_to_integer_refused(self):self.assertIsNone(self.view(body=self.body.replace('int move;','float move;')))
    def test_arbitrary_integer_to_bool_refused(self):self.assertIsNone(self.view(body=self.body.replace('a, b, 1,','a, b, move,')))
    def test_unknown_original_parameter_width_refused(self):
        old=list(self.old[2]);old[-1]='Unknown wide';self.assertIsNone(self.view(old=('float10','__thiscall',old)))
    def test_ambiguous_accepted_row_refused(self):
        import seed_callee_abi as abi
        abi._CONTEXTS[self.root.resolve()][0][0x1000].append(self.row);self.assertIsNone(self.view())

if __name__ == '__main__':unittest.main()
