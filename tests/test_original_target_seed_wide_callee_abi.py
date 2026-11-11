"""Authored native fixtures for branch-sensitive member ABI guards."""
import hashlib
import struct
import unittest
import capstone
import seed_candidate as seed
from seed_callee_abi import AbiRefusal
from seed_wide_callee_abi import inspect_wide_call,wide_signature,raw_source_arguments,sret_copy,receiver_offsets,graph

class Code:
    def __init__(self,base):self.base=base;self.data=bytearray();self.labels={};self.fixups=[]
    def emit(self,raw):self.data.extend(raw)
    def label(self,name):self.labels[name]=self.base+len(self.data)
    def jump(self,opcode,label):self.emit(opcode);self.fixups.append((len(self.data),label));self.emit(b'\0'*4)
    def call(self,target):self.emit(b'\xe8'+struct.pack('<i',target-(self.base+len(self.data)+5)))
    def finish(self):
        for offset,label in self.fixups:self.data[offset:offset+4]=struct.pack('<i',self.labels[label]-(self.base+offset+4))
        return bytes(self.data)

class Fixture:
    def __init__(self,second_offset=0x98,overwrite_saved=False,wide_store=True,word_push=False,nested=True,helper_store=8,helper_pop=4):
        code=Code(0x2000)
        code.emit(b'\x56\x8b\xf1\x8d\xbe'+struct.pack('<i',0x98)+b'\x85\xc0');code.jump(b'\x0f\x84','second')
        code.label('first');code.emit(b'\x8b\xbe\xec\x00\x00\x00') # EDI overwritten on this branch only.
        self.arguments(code,wide_store,word_push)
        code.emit(b'\x8d\x54\x24\x48\x52\x8d\x8e'+struct.pack('<i',0x98));code.jump(b'\xe9','call')
        code.label('second');self.arguments(code,wide_store,word_push)
        if nested:
            code.emit(b'\x8d\x44\x24\x54\x50\x8b\xce');code.call(0x3000);code.emit(b'\x50')
        else:code.emit(b'\x8d\x44\x24\x54\x50')
        if overwrite_saved:code.emit(b'\x66\xbf\x01\x00')
        if second_offset==0x98:code.emit(b'\x8b\xcf')
        else:code.emit(b'\x8d\x8e'+struct.pack('<i',second_offset))
        code.label('call');self.call=code.base+len(code.data);code.call(0x1000);code.emit(b'\x5e\xc3')
        self.code=code;caller=code.finish()
        helper=(b'\x8b\x44\x24\x04\x83\xc1\x20\x56\x8b\xd0'
                b'\x8b\x31\x89\x32\x8b\x71\x04\x89\x72\x04\x5e'
                b'\x8b\x49\x08\x89\x4a'+bytes([helper_store])+b'\xc2'+struct.pack('<H',helper_pop))
        self.data={0x1000:b'\xc2\x28\x00',0x2000:caller,0x3000:helper}
        self.decoder=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);self.decoder.detail=True
        self.rows={entry:dict(body_sha256=hashlib.sha256(raw).hexdigest()) for entry,raw in self.data.items()}
    def arguments(self,code,wide,word):
        code.emit(b'\x83\xec\x08'+(b'\xdd\x1c\x24' if wide else b'\xd9\x1c\x24'))
        code.emit(b'\x8d\x54\x24\x24\x52')
        for i in range(6):code.emit(b'\x66\x6a\x00' if word and i==0 else b'\x6a\x00')
    def original(self,address,length):
        for base,raw in self.data.items():
            if base<=address and address+length<=base+len(raw):return raw[address-base:address-base+length]
        raise ValueError('outside fixture')
    def function(self,entry):
        raw=self.data[entry];return None,dict(contiguous=True,ranges=[dict(start='%08x'%entry,end_inclusive='%08x'%(entry+len(raw)-1),bytes_hex=raw.hex())])
    def prove(self):return inspect_wide_call(self,self,0x2000,0x1000,self.rows,8)

class NativeWideTests(unittest.TestCase):
    def test_both_receiver_paths_and_nested_copy(self):
        p=Fixture().prove();self.assertEqual(p['receiver_offset'],0x98);self.assertEqual(len(p['paths']),2);self.assertEqual(p['paths'][1]['nested_copies'][0]['result_bytes'],12)
    def test_without_nested_copy(self):self.assertEqual(len(Fixture(nested=False).prove()['paths']),2)
    def test_conflicting_receiver_offsets(self):
        with self.assertRaises(AbiRefusal):Fixture(second_offset=0x9c).prove()
    def test_partial_receiver_register_write(self):
        with self.assertRaises(AbiRefusal):Fixture(overwrite_saved=True).prove()
    def test_single_precision_store_refused(self):
        with self.assertRaises(AbiRefusal):Fixture(wide_store=False).prove()
    def test_word_push_refused(self):
        with self.assertRaises(AbiRefusal):Fixture(word_push=True).prove()
    def test_nested_wrong_cleanup(self):
        with self.assertRaises(AbiRefusal):Fixture(helper_pop=8).prove()
    def test_nested_overlapping_output(self):
        with self.assertRaises(AbiRefusal):Fixture(helper_store=4).prove()
    def test_nested_noncontiguous_output(self):
        with self.assertRaises(AbiRefusal):Fixture(helper_store=12).prove()
    def test_unaccepted_nested_helper(self):
        f=Fixture();del f.rows[0x3000]
        with self.assertRaises(AbiRefusal):f.prove()
    def test_changed_nested_helper_body(self):
        f=Fixture();f.rows[0x3000]['body_sha256']='0'*64
        with self.assertRaises(AbiRefusal):f.prove()
    def test_wrong_final_cleanup(self):
        f=Fixture();f.data[0x1000]=b'\xc2\x20\x00';f.rows[0x1000]['body_sha256']=hashlib.sha256(f.data[0x1000]).hexdigest()
        with self.assertRaises(AbiRefusal):f.prove()
    def test_wrong_source_width_count(self):
        f=Fixture()
        with self.assertRaises(AbiRefusal):inspect_wide_call(f,f,0x2000,0x1000,f.rows,7)
    def test_unknown_receiver_before_capture(self):
        f=Fixture();raw=f.data[0x2000];f.data[0x2000]=raw.replace(b'\x8b\xf1',b'\x8b\xf0',1)
        with self.assertRaises(AbiRefusal):f.prove()
    def test_memory_write_over_outgoing_double(self):
        f=Fixture();raw=f.data[0x2000];f.data[0x2000]=raw.replace(b'\xdd\x1c\x24',b'\x89\x04\x24',1)
        with self.assertRaises(AbiRefusal):f.prove()
    def test_x87_store_access_metadata_not_trusted(self):
        f=Fixture();n,_,_,_=graph(f,f,0x2000);stores=[i for i in n.values() if i.mnemonic=='fstp'];self.assertEqual(len(stores),2);self.assertEqual(len(f.prove()['paths']),2)

class SignatureTests(unittest.TestCase):
    source='void Bounce::Init(Coord const &coord, double elasticity, double gravity, double max_velocity, Vector const &velocity, double rotation) { value = rotation; }'
    symbol='?Init@Bounce@@QAEXABUCoord@@NNNABUVector@@N@Z'
    old=['int this_','int *coord']+['undefined4 word%d'%i for i in range(6)]+['undefined4 *velocity']
    args=['coord']+['word%d'%i for i in range(6)]+['velocity','rotation']
    body='{\n  int *coord;\n'+''.join('  undefined4 word%d;\n'%i for i in range(6))+'  float *velocity;\n  double rotation;\n}'
    def test_exact_raw_prefix_boundaries(self):self.assertEqual(wide_signature(self.source,self.symbol,self.old,seed),(8,[0,7]))
    def test_source_types_preserved(self):raw_source_arguments(self.body,[],self.args,self.old,seed)
    def test_missing_final_double(self):
        with self.assertRaises(AbiRefusal):wide_signature(self.source.replace(', double rotation',''),self.symbol,self.old,seed)
    def test_float_instead_of_double(self):
        with self.assertRaises(AbiRefusal):wide_signature(self.source.replace('double rotation','float rotation'),self.symbol,self.old,seed)
    def test_no_double_word_coalescing(self):
        old=list(self.old);old[2]='double word0'
        with self.assertRaises(AbiRefusal):wide_signature(self.source,self.symbol,old,seed)
    def test_reference_not_raw_pointer(self):
        old=list(self.old);old[1]='int coord'
        with self.assertRaises(AbiRefusal):wide_signature(self.source,self.symbol,old,seed)
    def test_record_by_value_refused(self):
        with self.assertRaises(AbiRefusal):wide_signature(self.source.replace('Coord const &coord','Coord coord'),self.symbol,self.old,seed)
    def test_symbol_owner_disagreement(self):
        with self.assertRaises(AbiRefusal):wide_signature(self.source,self.symbol.replace('@Bounce@','@Other@'),self.old,seed)
    def test_numeric_float_conversion_refused(self):
        with self.assertRaises(AbiRefusal):raw_source_arguments(self.body.replace('double rotation','float rotation'),[],self.args,self.old,seed)
    def test_pointer_to_double_refused(self):
        with self.assertRaises(AbiRefusal):raw_source_arguments(self.body.replace('double rotation','int *rotation'),[],self.args,self.old,seed)
    def test_nonintegral_raw_word_refused(self):
        with self.assertRaises(AbiRefusal):raw_source_arguments(self.body.replace('undefined4 word0','float word0'),[],self.args,self.old,seed)
    def test_unknown_pointer_argument_refused(self):
        with self.assertRaises(AbiRefusal):raw_source_arguments(self.body.replace('int *coord','int coord'),[],self.args,self.old,seed)
    def test_side_effect_argument_refused(self):
        args=list(self.args);args[0]='get_coord()'
        with self.assertRaises(AbiRefusal):raw_source_arguments(self.body,[],args,self.old,seed)
    def test_duplicate_local_refused(self):
        with self.assertRaises(AbiRefusal):raw_source_arguments(self.body.replace('  int *coord;','  int *coord;\n  int *coord;'),[],self.args,self.old,seed)
    def test_comma_declaration_refused(self):
        with self.assertRaises(AbiRefusal):raw_source_arguments(self.body.replace('  int *coord;','  int *coord, *other;'),[],self.args,self.old,seed)

class ProviderTests(unittest.TestCase):
    def setUp(self):
        import tempfile
        from pathlib import Path
        from seed_callee_abi import _CONTEXTS
        self.tmp=tempfile.TemporaryDirectory();self.addCleanup(self.tmp.cleanup);self.root=Path(self.tmp.name);self.f=Fixture()
        self.path='src/recovered/classes/Bounce/Init_00001000.cpp';p=self.root/self.path;p.parent.mkdir(parents=True);p.write_text(SignatureTests.source)
        directory=self.root/'private/original-target-20261020/inventory/pass';(directory/'decompiled').mkdir(parents=True);(directory/'decompiled/00002000.c').write_text('frozen caller');self.f.passes=[directory]
        self.f.rows[0x1000].update(entry=0x1000,source=self.path,symbol=SignatureTests.symbol)
        _CONTEXTS[self.root.resolve()]=({entry:[row] for entry,row in self.f.rows.items()},self.f);self.addCleanup(_CONTEXTS.pop,self.root.resolve())
        self.body=SignatureTests.body[:-1]+'  FUN_00001000('+', '.join(SignatureTests.args)+');\n}'
        self.old=('void','__thiscall',SignatureTests.old)
    def view(self,**kwargs):
        from seed_wide_callee_abi import accepted_wide_member_view
        return accepted_wide_member_view(self.root,self.f,0x2000,0x1000,kwargs.get('body',self.body),['int this_'],kwargs.get('prototype',self.old),kwargs.get('this_name','this_'),kwargs.get('accepted',{0x1000:self.path}),seed)
    def test_guarded_provider_preserves_raw_prefix_and_adds_double(self):
        result=self.view();self.assertIsNotNone(result);self.assertEqual(result['prototype'][2][:-1],SignatureTests.old);self.assertEqual(result['prototype'][2][-1],'double seed_trailing_double');self.assertEqual(result['owner'],'((char*)this_ + 0x98)')
        body,decl=seed.declare_callee(self.body,'FUN_00001000',result['prototype'],result['owner']);self.assertIn('(double)(rotation)',body);self.assertIn('(int *)(coord)',body);self.assertIn('(undefined4 *)(velocity)',body)
    def test_orphan_source_declined(self):self.assertIsNone(self.view(accepted={}))
    def test_wrong_accepted_source_declined(self):self.assertIsNone(self.view(accepted={0x1000:'other.cpp'}))
    def test_missing_receiver_declined(self):self.assertIsNone(self.view(this_name=None))
    def test_changed_return_abi_declined(self):self.assertIsNone(self.view(prototype=('double','__thiscall',SignatureTests.old)))
    def test_changed_calling_convention_declined(self):self.assertIsNone(self.view(prototype=('void','__cdecl',SignatureTests.old)))
    def test_double_argument_not_float_conversion(self):self.assertIsNone(self.view(body=self.body.replace('double rotation','float rotation')))
    def test_unknown_final_argument_declined(self):self.assertIsNone(self.view(body=self.body.replace('double rotation;','int *rotation;')))
    def test_two_source_calls_declined(self):self.assertIsNone(self.view(body=self.body+self.body))
    def test_opcode_with_misleading_memory_access_declined(self):
        self.f.data[0x2000]=self.f.data[0x2000].replace(b'\xdd\x1c\x24',b'\xd9\x34\x24',1);self.assertIsNone(self.view())
    def test_nested_negative_output_write_declined(self):
        self.f.data[0x3000]=self.f.data[0x3000].replace(b'\x89\x4a\x08',b'\x89\x4a\xfc');self.f.rows[0x3000]['body_sha256']=hashlib.sha256(self.f.data[0x3000]).hexdigest();self.assertIsNone(self.view())
    def test_unverified_nested_out_pointer_declined(self):
        self.f.data[0x2000]=self.f.data[0x2000].replace(b'\x8d\x44\x24\x54',b'\x8d\x44\x24\x00',1);self.assertIsNone(self.view())

if __name__=='__main__':unittest.main()
