"""Authored toy switch fixtures, never original-game code arrays."""
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch
import capstone,importlib.util,struct,sys,unittest
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools/original_target'))
from seed_guarded_cfg import guarded_graph,CFGRefusal
ENTRY=0x1000
class GuardedCFG(unittest.TestCase):
 def fixture(self,level=2,zero=b'\x33\xd2',safe=b'',limit=1,prefix=b'',mapping=0x2100,table=0x2000,load=None):
  compare=(b'\x83\xf9'+bytes([limit])) if limit<128 else b'\x81\xf9'+struct.pack('<I',limit)
  if level==1:compare=compare.replace(b'\xf9',b'\xf8')
  setup=zero+(b'\x8a\x91'+struct.pack('<I',mapping) if load is None else load) if level==2 else b''
  if level==3:setup=b'\x0f\xb6\x91'+struct.pack('<I',mapping)
  jump=b'\xff\x24'+(b'\x85' if level==1 else b'\x95')+struct.pack('<I',table)
  default=len(prefix)+len(compare)+len(safe)+2+len(setup)+len(jump)+2
  branch=b'\x77'+bytes([default-(len(prefix)+len(compare)+len(safe)+2)])
  body=prefix+compare+safe+branch+setup+jump+b'\xc3\xc3\xc3';self.body=body;self.data=bytearray(0x2000);self.data[:len(body)]=body;struct.pack_into('<II',self.data,table-ENTRY,ENTRY+default-2,ENTRY+default-1);self.data[mapping-ENTRY:mapping-ENTRY+2]=b'\x00\x01';self.table=table;self.mapping=mapping;self.setup_at=ENTRY+len(prefix)+len(compare)+len(safe)+2
  self.section=SimpleNamespace(VirtualAddress=ENTRY,SizeOfRawData=len(self.data),Misc_VirtualSize=len(self.data),Characteristics=0x60000020)
  decoder=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);decoder.detail=True
  audit=SimpleNamespace(base=0,decoder=decoder,pe=SimpleNamespace(get_section_by_rva=lambda address:self.section if ENTRY<=address<ENTRY+len(self.data) else None),original=lambda at,size:bytes(self.data[at-ENTRY:at-ENTRY+size]));inventory=SimpleNamespace(function=lambda entry:(None,dict(contiguous=True,ranges=[dict(start='%08x'%ENTRY,end_inclusive='%08x'%(ENTRY+len(body)-1),bytes_hex=body.hex())])))
  return audit,inventory
 def prove(self,**kwargs):return guarded_graph(*self.fixture(**kwargs),ENTRY)
 def reject(self,mutation=None,reason=None,**kwargs):
  audit,inventory=self.fixture(**kwargs)
  if mutation:mutation()
  with self.assertRaisesRegex(CFGRefusal,reason or '.'):guarded_graph(audit,inventory,ENTRY)
 def test_two_level_map_derived_from_original_guard(self):
  p=self.prove();self.assertEqual(p['switches'][0]['bound'],2);self.assertEqual(p['switches'][0]['level'],2);self.assertEqual([x['size'] for x in p['switches'][0]['literals']],[8,2]);self.assertFalse(p['body_acceptance_claimed'])
 def test_one_level_table_is_supported(self):self.assertEqual(self.prove(level=1)['switches'][0]['level'],1)
 def test_movzx_index_needs_no_xor(self):self.assertEqual(self.prove(level=3)['switches'][0]['level'],2)
 def test_nonflag_store_between_compare_and_branch_is_supported(self):self.assertEqual(self.prove(safe=bytes.fromhex('890424'))['switches'][0]['bound'],2)
 def test_changed_index_between_compare_and_branch_refuses(self):self.reject(safe=bytes.fromhex('8bc8'),reason='index/flags changed')
 def test_partial_index_write_between_compare_and_branch_refuses(self):self.reject(safe=bytes.fromhex('b100'),reason='index/flags changed')
 def test_changed_flags_between_compare_and_branch_refuse(self):self.reject(safe=bytes.fromhex('83c701'),reason='bound')
 def test_unknown_upper_index_bits_refuse(self):self.reject(zero=bytes.fromhex('9090'),reason='upper bits')
 def test_wrong_zero_register_refuses(self):self.reject(zero=bytes.fromhex('33c0'),reason='upper bits')
 def test_high_byte_is_not_low_index(self):self.reject(load=b'\x8a\xb1'+struct.pack('<I',0x2100),reason='low-byte')
 def test_unsigned_range_is_bounded(self):self.reject(limit=256,reason='bound')
 def test_full_byte_index_range_is_supported(self):self.assertEqual(self.prove(limit=255)['switches'][0]['bound'],256)
 def test_negative_unsigned_guard_bound_refuses(self):self.reject(limit=0xffffffff,reason='bound')
 def test_writable_index_table_refuses(self):self.reject(lambda:setattr(self.section,'Characteristics',0xe0000020),reason='Writable')
 def test_table_must_fit_initialized_data(self):self.reject(lambda:setattr(self.section,'SizeOfRawData',0x1004),reason='initialized')
 def test_mapped_target_outside_code_refuses(self):self.reject(lambda:struct.pack_into('<I',self.data,self.table-ENTRY,0x4000),reason='target escapes')
 def test_mapped_target_inside_instruction_refuses(self):self.reject(lambda:struct.pack_into('<I',self.data,self.table-ENTRY,ENTRY+1),reason='Overlapping')
 def test_expanded_target_cannot_bypass_guard(self):self.reject(lambda:struct.pack_into('<I',self.data,self.table-ENTRY,self.setup_at),reason='ingress/bypass')
 def test_direct_guard_bypass_refuses(self):self.reject(prefix=bytes.fromhex('85db7407'),reason='ingress/bypass')
 def test_pointer_table_alignment_refuses(self):self.reject(table=0x2002,reason='Unaligned')
 def test_overlapping_map_and_pointer_tables_refuse(self):self.reject(mapping=0x2000,reason='Overlapping switch')
 def test_original_inventory_byte_disagreement_refuses(self):self.reject(lambda:self.data.__setitem__(0,0x90),reason='bytes differ')
 def test_module_import_performs_no_path_reads(self):
  path=Path(__file__).resolve().parents[1]/'tools/original_target/seed_guarded_cfg.py';spec=importlib.util.spec_from_file_location('fresh_guarded_cfg_no_io',path);module=importlib.util.module_from_spec(spec)
  with patch.object(Path,'read_bytes',side_effect=AssertionError('import read')),patch.object(Path,'read_text',side_effect=AssertionError('import read')):spec.loader.exec_module(module)

if __name__=='__main__':unittest.main()
