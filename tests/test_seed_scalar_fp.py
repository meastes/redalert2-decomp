import unittest
import hashlib
import struct
from types import SimpleNamespace
import capstone
import seed_candidate as seed
import seed_scalar_fp as fp

class SourceFlowTests(unittest.TestCase):
    def body(self,prefix='',argument='x * x',destination='int result;',gap=''):
        return '{\n double x;\n '+destination.replace('; ',';\n ')+'\n '+prefix+'FUN_00002000('+argument+');'+gap+'\n result = FUN_00003000();\n }'
    def pair(self,body):return fp.source_pair(body,[],0x2000,0x3000,{},seed)
    def refused(self,body):
        with self.assertRaises(ValueError):self.pair(body)
    def test_sibling_fuses_preserving_expression(self):
        body=self.body();a,z,text=self.pair(body);self.assertEqual(text,'result = (longlong)FUN_00002000(x * x);');self.assertEqual(body[:a]+text+body[z:],'{\n double x;\n int result;\n result = (longlong)FUN_00002000(x * x);\n }')
    def test_byte_destination_retained(self):self.assertIn('(longlong)',self.pair(self.body(destination='byte result;'))[2])
    def test_unbraced_if_refused(self):self.refused(self.body(prefix='if (x) '))
    def test_unbraced_else_refused(self):self.refused(self.body(prefix='else '))
    def test_label_refused(self):self.refused(self.body(prefix='label: '))
    def test_while_refused(self):self.refused(self.body(prefix='while (x) '))
    def test_comment_refused(self):self.refused(self.body(gap='/* retain */'))
    def test_intervening_statement_refused(self):self.refused(self.body(gap='x = 3;'))
    def test_multiple_producers_refused(self):self.refused(self.body(prefix='FUN_00002000(x); '))
    def test_multiple_conversions_refused(self):self.refused(self.body(gap='result = FUN_00003000();'))
    def test_missing_input_refused(self):self.refused(self.body(argument=''))
    def test_split_input_refused(self):self.refused(self.body(argument='1, 2'))
    def test_integer_only_input_refused(self):self.refused(self.body(argument='1'))
    def test_wide_numeric_input_refused(self):self.refused(self.body(argument='(double)wide',destination='ulonglong wide; int result;'))
    def test_bool_destination_refused(self):self.refused(self.body(destination='bool result;'))
    def test_float_destination_refused(self):self.refused(self.body(destination='float result;'))
    def test_wide_destination_refused(self):self.refused(self.body(destination='longlong result;'))
    def test_pointer_destination_refused(self):self.refused(self.body(destination='int *result;'))
    def test_shadowed_destination_refused(self):self.refused(self.body(destination='int result; { float result; }'))
    def test_unknown_destination_refused(self):self.refused(self.body(destination=''))
    def test_call_argument_refused(self):self.refused(self.body(argument='other(x)'))
    def test_mutation_argument_refused(self):self.refused(self.body(argument='x++'))
    def test_volatile_load_refused(self):self.refused(self.body(argument='*(volatile double *)p',destination='int *p; int result;'))
    def test_known_pointer_numeric_reads(self):
        t={'p':'int *'};self.assertEqual(fp.expression_type('(double)p[1] * (double)*p',t,seed),'double')
    def test_typed_fp_load(self):self.assertEqual(fp.expression_type('*(double *)(p + 4)',{'p':'int'},seed),'double')
    def test_unknown_address_refused(self):self.assertIsNone(fp.expression_type('*(double *)(mystery + 4)',{},seed))
    def test_array_index_mutation_refused(self):self.assertIsNone(fp.expression_type('p[i++]',{'p':'double *','i':'int'},seed))
    def test_fp_division_type(self):self.assertEqual(fp.expression_type('a / (float)2',{'a':'double'},seed),'double')
    def test_fp_remainder_refused(self):self.assertIsNone(fp.expression_type('a % 2',{'a':'double'},seed))
    def test_unknown_scalar_signature_refused(self):
        with self.assertRaises(ValueError):fp.scalar_signature('double __cdecl F(double x) { return x; }','?F@@YANN@Z',seed)
    def test_signature_and_symbol(self):self.assertEqual(fp.scalar_signature('float __cdecl F(double x) { return (float)x; }','?F@@YAMN@Z',seed),'F')
    def test_symbol_arity_refused(self):
        with self.assertRaises(ValueError):fp.scalar_signature('float __cdecl F(double x) { return (float)x; }','?F@@YAMM@Z',seed)

class RepositoryGuardTests(unittest.TestCase):
    """Isolate source/catalog refusal paths; actual native proof has its own run."""
    def setUp(self):
        import tempfile
        from pathlib import Path
        from unittest.mock import patch
        self.temp=tempfile.TemporaryDirectory();self.addCleanup(self.temp.cleanup);self.root=Path(self.temp.name)
        source=self.root/'src/recovered/f.cpp';source.parent.mkdir(parents=True);source.write_text('float __cdecl F(double x) { return (float)x; }',encoding='utf-8')
        self.source=source;self.row=dict(entry=0x2000,source='src/recovered/f.cpp',symbol='?F@@YAMN@Z',body_sha256='native',source_sha256=hashlib.sha256(source.read_bytes()).hexdigest())
        directory=self.root/'private/original-target-20261020/inventory/fixture';(directory/'decompiled').mkdir(parents=True);(directory/'decompiled/00001000.c').write_text('fixture',encoding='utf-8')
        self.inventory=SimpleNamespace(passes=[directory],function=lambda entry:(directory,{'ranges':[]}))
        self.rows={0x2000:[self.row]};fp._CONTEXTS[self.root.resolve()]=(self.rows,None,{})
        self.addCleanup(fp._CONTEXTS.pop,self.root.resolve(),None)
        self.body='{\n double input;\n int result;\n FUN_00002000(input);\n result = FUN_00003000();\n}'
        self.accepted={0x2000:'src/recovered/f.cpp'};self.prototype=('float10','',['undefined4 a','undefined4 b'])
        self.native=patch.object(fp,'native_pair',return_value={}).start();self.addCleanup(patch.stopall)
        self.helper=patch.object(fp,'helper_contract',return_value='helper').start()
        self.refs=patch('diff_candidate.known_references',return_value={'__ftol':0x3000}).start()
        patch.object(seed,'floating_operand_globals',return_value={}).start()
    def run_guard(self):return fp.recover(self.root,self.inventory,0x1000,0x2000,self.body,[],self.prototype,self.accepted,seed)
    def test_accepted_source_path_and_hash_pass(self):self.assertEqual(self.run_guard()['prototype'],('float10','__cdecl',['double input']))
    def test_changed_source_hash_refused(self):self.source.write_text(self.source.read_text()+'\n',encoding='utf-8');self.assertIsNone(self.run_guard());self.native.assert_not_called()
    def test_ambiguous_catalog_refused(self):self.rows[0x2000].append(self.row);self.assertIsNone(self.run_guard())
    def test_source_map_disagreement_refused(self):self.accepted[0x2000]='src/recovered/other.cpp';self.assertIsNone(self.run_guard())
    def test_noncanonical_source_path_refused(self):self.row['source']='build/candidate.cpp';self.accepted[0x2000]=self.row['source'];self.assertIsNone(self.run_guard())
    def test_missing_symbol_binding_refused(self):self.refs.return_value={};self.assertIsNone(self.run_guard());self.native.assert_not_called()
    def test_unaccepted_callee_refused(self):self.accepted.clear();self.assertIsNone(self.run_guard())
    def test_missing_inventory_provenance_refused(self):self.inventory.passes=[];self.assertIsNone(self.run_guard())
    def test_bad_native_evidence_refused(self):self.native.side_effect=ValueError('unknown native setup');self.assertIsNone(self.run_guard());self.helper.assert_not_called()
    def test_bad_helper_provenance_refused(self):self.helper.side_effect=ValueError('helper differs');self.assertIsNone(self.run_guard())
    def test_old_pointer_width_refused(self):self.prototype=('float10','',['int *a','int b']);self.assertIsNone(self.run_guard())
    def test_non_cdecl_refused(self):self.prototype=('float10','__thiscall',['undefined4 a','undefined4 b']);self.assertIsNone(self.run_guard())

class DynamicImportTests(unittest.TestCase):
    def test_unregistered_importlib_module_can_seed_calls(self):
        import importlib.util
        from pathlib import Path
        name='unregistered_scalar_fp_seed_test'
        spec=importlib.util.spec_from_file_location(name,Path(fp.__file__).with_name('seed_candidate.py'))
        module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
        module.REPOSITORY=seed.REPOSITORY
        class Inventory:
            passes=[]
            def function(self,entry):return None,{'ranges':[]}
            def decompiled(self,entry):
                if entry==0x401000:return 'int __cdecl FUN_00401000(void)\n{\n return FUN_00402000();\n}\n'
                return 'int __cdecl FUN_00402000(void)\n{\n return 1;\n}\n'
        self.assertIn('FUN_00402000()',module.seed(0x401000,Inventory(),{}))

class NativeFlowTests(unittest.TestCase):
    def fixture(self,*,store=bytes.fromhex('dd1c24'),between=b'',allocation=8,cleanup=8,callee_ret=bytes.fromhex('c3'),duplicate=False,extra_prefix=b'',bypass=False):
        root,callee,helper=0x1000,0x2000,0x3000
        def call(at,target):return b'\xe8'+struct.pack('<i',target-at-5)
        body=(b'\x75\x00' if bypass else b'')+b'\x83\xec'+bytes([allocation])+b'\xdd\x00'+extra_prefix+store
        body+=call(root+len(body),callee);cleanup_offset=len(body);body+=b'\x83\xc4'+bytes([cleanup])+between;body+=call(root+len(body),helper)
        if bypass:body=body[:1]+bytes([cleanup_offset-2])+body[2:]
        if duplicate:body+=call(root+len(body),helper)
        body+=b'\xc3';cb=bytes.fromhex('dd442404')+callee_ret
        blobs={root:body,callee:cb};md=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);md.detail=True
        audit=SimpleNamespace(decoder=md,original=lambda entry,length:blobs[entry][:length])
        inv=SimpleNamespace(function=lambda entry:(None,dict(contiguous=True,ranges=[dict(start='%08x'%entry,end_inclusive='%08x'%(entry+len(blobs[entry])-1),bytes_hex=blobs[entry].hex())])))
        return audit,inv,root,callee,hashlib.sha256(cb).hexdigest(),helper
    def prove(self,**options):return fp.native_pair(*self.fixture(**options))
    def refuse(self,**options):
        with self.assertRaises(ValueError):self.prove(**options)
    def test_bypass_producer_refused(self):self.refuse(bypass=True)
    def test_double_argument_and_flow(self):self.assertEqual(self.prove()['allocation'],0x1000)
    def test_four_byte_store_refused(self):self.refuse(store=bytes.fromhex('d91c24'))
    def test_partial_argument_refused(self):self.refuse(store=bytes.fromhex('89542404'))
    def test_missing_argument_store_refused(self):self.refuse(store=b'')
    def test_multiple_argument_store_refused(self):self.refuse(store=bytes.fromhex('dd1c24dd1c24'))
    def test_wrong_allocation_refused(self):self.refuse(allocation=4)
    def test_wrong_cleanup_refused(self):self.refuse(cleanup=4)
    def test_intervening_fp_operation_refused(self):self.refuse(between=bytes.fromhex('d9e0'))
    def test_intervening_integer_operation_refused(self):self.refuse(between=bytes.fromhex('89c0'))
    def test_hidden_state_change_refused(self):self.refuse(extra_prefix=bytes.fromhex('db e2'))
    def test_unmodeled_stack_push_refused(self):self.refuse(extra_prefix=bytes.fromhex('50'))
    def test_aliasing_memory_write_refused(self):self.refuse(extra_prefix=bytes.fromhex('8901'))
    def test_native_callee_cleanup_refused(self):self.refuse(callee_ret=bytes.fromhex('c20800'))
    def test_multiple_native_conversions_refused(self):self.refuse(duplicate=True)
    def test_callee_hash_mismatch_refused(self):
        args=list(self.fixture());args[4]='0'*64
        with self.assertRaises(ValueError):fp.native_pair(*args)

if __name__=='__main__':unittest.main()
