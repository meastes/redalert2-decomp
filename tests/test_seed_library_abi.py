from pathlib import Path
from types import SimpleNamespace
import capstone,hashlib,os,struct,sys,unittest
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools/original_target'))
if os.environ.get('RA2_SEED_TEST_TOOLS'):sys.path.append(os.environ['RA2_SEED_TEST_TOOLS'])
from seed_library_abi import normal_c_runtime_abi
from seed_guarded_cfg import CFGRefusal
ENTRY=0x1000
class RuntimeABI(unittest.TestCase):
 def fixture(self,body=bytes.fromhex('b801000000c3'),compiler=0xa1fe8,function=True,relocation=False):
  source=body;relocs=struct.pack('<IIH',1,0,20) if relocation else b'';records=[struct.pack('<8sIhHBB',b'_toy',0,1,0x20 if function else 0,2,0)]
  if compiler is not None:records.append(struct.pack('<8sIhHBB',b'@comp.id',compiler,-1,0,3,0))
  obj=struct.pack('<HHIIIHH',0x14c,1,0,60+len(source)+len(relocs),len(records),0,0)+struct.pack('<8sIIIIIIHHI',b'.text',0,0,len(source),60,60+len(source) if relocs else 0,0,int(bool(relocs)),0,0x60100020)+source+relocs+b''.join(records)+struct.pack('<I',4)
  header=b'toy.obj/'.ljust(16)+b'0'.ljust(12)+b'0'.ljust(6)+b'0'.ljust(6)+b'100644'.ljust(8)+str(len(obj)).encode().ljust(10)+b'`\n';data=b'!<arch>\n'+header+obj+(b'\n' if len(obj)%2 else b'');directory=Path(__file__).resolve().parents[1]/'build/test-library-abi'/self._testMethodName;directory.mkdir(parents=True,exist_ok=True);path=directory/'LIBCMT.LIB';path.write_bytes(data);digest=hashlib.sha256(data).hexdigest();native=bytearray(body)
  if relocation:struct.pack_into('<I',native,1,0x100)
  native=bytes(native);decoder=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);decoder.detail=True;inventory=SimpleNamespace(function=lambda entry:(None,dict(contiguous=True,ranges=[dict(start='%08x'%ENTRY,end_inclusive='%08x'%(ENTRY+len(native)-1),bytes_hex=native.hex())])));env=SimpleNamespace(audit=SimpleNamespace(decoder=decoder,original=lambda at,size:native[at-ENTRY:at-ENTRY+size]),inventory=inventory)
  row=dict(entry='%08x'%ENTRY,bytes=len(native),archive='LIBCMT.LIB',archive_role='VC6 SP3',archive_sha256=digest,member='toy.obj',member_index=0,section_index=0,section='.text',symbols=['_toy'],sha256=hashlib.sha256(native).hexdigest(),relocations=[dict(offset=1,type=20,resolved=0x100)] if relocation else [],assignment='by order among identical sections');ledger=dict(regions=[row],archives=[dict(name='LIBCMT.LIB',sha256=digest)]);return env,ledger,{'LIBCMT.LIB':path}
 def prove(self,**kwargs):return normal_c_runtime_abi(*self.prepare(**kwargs))
 def prepare(self,**kwargs):
  env,ledger,archives=self.fixture(**kwargs);return env,ENTRY,ledger,archives
 def test_compiler_function_identity_exports_only_abi(self):
  p=self.prove();self.assertTrue(p['fully_relocated_producer_exact']);self.assertTrue(p['nonvolatile_registers_preserved']);self.assertFalse(p['memory_effects_proved']);self.assertFalse(p['allocation_semantics_inferred']);self.assertTrue(p['name_assignment'])
 def test_exact_relocated_producer_body_is_checked(self):self.assertTrue(self.prove(body=bytes.fromhex('e800000000c3'),relocation=True)['fully_relocated_producer_exact'])
 def test_unknown_compiler_product_refuses(self):
  with self.assertRaisesRegex(CFGRefusal,'compiler object'):self.prove(compiler=0x610001)
 def test_no_compiler_metadata_does_not_assume_normal_c_source(self):
  with self.assertRaisesRegex(CFGRefusal,'compiler object'):self.prove(compiler=None)
 def test_symbol_must_be_coff_function(self):
  with self.assertRaisesRegex(CFGRefusal,'COFF function'):self.prove(function=False)
 def test_mutated_archive_refuses(self):
  env,ledger,archives=self.fixture();path=archives['LIBCMT.LIB'];path.write_bytes(path.read_bytes()+b'X')
  with self.assertRaisesRegex(CFGRefusal,'archive hash'):normal_c_runtime_abi(env,ENTRY,ledger,archives)
 def test_wrong_native_hash_refuses(self):
  env,ledger,archives=self.fixture();ledger['regions'][0]['sha256']='0'*64
  with self.assertRaisesRegex(CFGRefusal,'native body'):normal_c_runtime_abi(env,ENTRY,ledger,archives)
 def test_wrong_relocation_type_refuses(self):
  env,ledger,archives=self.fixture(body=bytes.fromhex('e800000000c3'),relocation=True);ledger['regions'][0]['relocations'][0]['type']=6
  with self.assertRaisesRegex(CFGRefusal,'set/type'):normal_c_runtime_abi(env,ENTRY,ledger,archives)
 def test_wrong_relocated_value_refuses_full_equality(self):
  env,ledger,archives=self.fixture(body=bytes.fromhex('e800000000c3'),relocation=True);ledger['regions'][0]['relocations'][0]['resolved']+=1
  with self.assertRaisesRegex(CFGRefusal,'producer bytes'):normal_c_runtime_abi(env,ENTRY,ledger,archives)
 def test_direct_nonvolatile_clobber_is_not_in_supported_wrapper_domain(self):
  with self.assertRaisesRegex(CFGRefusal,'nonvolatile'):self.prove(body=bytes.fromhex('bd01000000c3'))
 def test_stdcall_cleanup_not_treated_as_cdecl(self):
  with self.assertRaisesRegex(CFGRefusal,'cleanup zero'):self.prove(body=bytes.fromhex('c20400'))
 def test_dynamic_stack_abi_is_not_assumed_cdecl(self):
  with self.assertRaisesRegex(CFGRefusal,'ESP effect'):self.prove(body=bytes.fromhex('8be0c3'))

if __name__=='__main__':unittest.main()
