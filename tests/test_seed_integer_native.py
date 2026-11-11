from pathlib import Path
from types import SimpleNamespace
import capstone,struct,sys,unittest
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools/original_target'))
from seed_guarded_cfg import guarded_graph,CFGRefusal
from seed_frame_equations import AffineFrames
from seed_integer_native import bind_native_integer,components,dominates
ENTRY=0x1000
class NativeLifetime(unittest.TestCase):
 def fixture(self,word=0xfffffffe,extra=b'',zero=0,cache_before=b'',cache_update=None,loop_init=False,bypass=False,source_increments=1,sign=1,threshold=1):
  code=bytearray();labels={};fix=[]
  def emit(raw):code.extend(bytes.fromhex(raw) if isinstance(raw,str) else raw)
  def mark(name):labels[name]=len(code)
  def jump(op,name):fix.append((len(code),name));emit(bytes([op,0]))
  emit('83ec08');emit(b'\xc7\x04\x24'+struct.pack('<I',zero));zero_at=len(code)-7
  if bypass:emit('85ff');jump(0x74,'loop')
  mark('init');emit(b'\xc7\x44\x24\x04'+struct.pack('<I',word));emit(cache_before);mark('loop')
  emit('8b04248b5424042bc283f8');emit(bytes([threshold]));jump(0x7e,'true');emit('b900000000');jump(0xeb,'join');mark('true');emit('b901000000');mark('join');emit('8b0424');emit(extra);emit('89442404' if cache_update is None else cache_update);emit('8b042440890424833c2403');jump(0x7c,'init' if loop_init else 'loop');emit('83c408c3')
  for at,name in fix:
   offset=labels[name]-at-2;assert -128<=offset<=127;code[at+1]=offset&255
  body=bytes(code);decoder=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);decoder.detail=True;audit=SimpleNamespace(decoder=decoder,original=lambda at,size:body[at-ENTRY:at-ENTRY+size]);inventory=SimpleNamespace(function=lambda entry:(None,dict(contiguous=True,ranges=[dict(start='%08x'%ENTRY,end_inclusive='%08x'%(ENTRY+len(body)-1),bytes_hex=body.hex())])));graph=guarded_graph(audit,inventory,ENTRY);frames=AffineFrames(graph,{})
  return graph,frames,dict(sign_bit=sign,comparison_limit=2,counter_increment_count=source_increments)
 def prove(self,**kwargs):return bind_native_integer(*self.fixture(**kwargs),normal_calls=set())
 def reject(self,**kwargs):
  with self.assertRaises(CFGRefusal):self.prove(**kwargs)
 def test_unique_word_counter_and_loop_roles_proved_emission_still_disabled(self):
  p=self.prove();self.assertEqual(p['signed_value'],-2);self.assertEqual(p['counter_offset_relative_cache'],-4);self.assertTrue(p['native_source_roles_verified']);self.assertFalse(p['source_emission_allowed'])
 def test_nan_payload_is_taken_from_native_bits_not_fixed_to_minus_two(self):self.assertEqual(self.prove(word=0xffc00001)['signed_value'],0xffc00001-(1<<32))
 def test_positive_nan_payload_matches_positive_source_artifact(self):self.assertEqual(self.prove(word=0x7fc00001,sign=0)['signed_value'],0x7fc00001)
 def test_wrong_nan_sign_refuses(self):self.reject(word=0x7fc00001)
 def test_real_float_one_is_not_nan_artifact(self):self.reject(word=0x3f800000)
 def test_cache_init_must_dominate_loop(self):self.reject(bypass=True)
 def test_cache_initializer_inside_loop_refuses(self):self.reject(loop_init=True)
 def test_counter_initializer_must_be_zero(self):self.reject(zero=1)
 def test_native_comparison_must_match_source_bound(self):self.reject(threshold=4)
 def test_source_counter_update_count_must_match(self):self.reject(source_increments=2)
 def test_cache_update_cannot_come_from_a_literal(self):self.reject(cache_update=bytes.fromhex('c744240400000000'))
 def test_stale_counter_load_cannot_cross_direct_counter_write(self):self.reject(extra=bytes.fromhex('c7042405000000'))
 def test_fp_cache_read_is_not_an_integer_lifetime(self):self.reject(extra=bytes.fromhex('d9442404ddd8'))
 def test_partial_cache_write_refuses(self):self.reject(extra=bytes.fromhex('66c74424040000'))
 def test_callee_return_is_not_the_loaded_counter_value(self):self.reject(extra=bytes.fromhex('e800100000'))
 def test_preloop_overwrite_of_nan_initializer_refuses(self):self.reject(cache_before=bytes.fromhex('c744240405000000'))
 def test_preloop_overwrite_of_zero_counter_refuses(self):self.reject(cache_before=bytes.fromhex('c7042405000000'))
 def test_component_boundary_and_initializer_dominance_are_independent_cfg_facts(self):
  g,f,b=self.fixture();p=bind_native_integer(g,f,b,set());membership,groups=components(g);self.assertEqual(groups[membership[p['subtraction']]],groups[membership[p['update']['store']]]);self.assertTrue(dominates(g,p['initializer'],p['subtraction']));self.assertNotIn(p['initializer'],groups[membership[p['subtraction']]])

if __name__=='__main__':unittest.main()
