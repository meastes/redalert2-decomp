from pathlib import Path
import sys,unittest,struct,hashlib
from types import SimpleNamespace
import capstone
import seed_candidate as seed
from seed_scalar_pairs import source_pairs,native_pairs
from seed_callee_abi import AbiRefusal
class SourcePairs(unittest.TestCase):
 def body(self,middle='',first='FUN_00001000(x);\n result = FUN_00002000();',second=''):
  return '{\n double x;\n int result;\n int other;\n '+first+'\n '+middle+'\n '+second+'\n}\n'
 def parse(self,body):return source_pairs(body,[],0x1000,0x2000,{},seed)
 def test_one_pair(self):
  rows=self.parse(self.body());self.assertEqual(len(rows),1);self.assertIn('result = (longlong)FUN_00001000(x);',rows[0][2])
 def test_unrelated_later_conversion_preserved(self):
  body=self.body(middle='other = FUN_00002000();');rows=self.parse(body);first,last,replacement=rows[0];text=body[:first]+replacement+body[last:];self.assertIn('other = FUN_00002000();',text)
 def test_unrelated_earlier_conversion_preserved(self):self.assertEqual(len(self.parse(self.body(first='other = FUN_00002000();\n FUN_00001000(x);\n result = FUN_00002000();'))),1)
 def test_multiple_local_pairs(self):self.assertEqual(len(self.parse(self.body(second='FUN_00001000(x + 1.0);\n other = FUN_00002000();'))),2)
 def test_missing_adjacent_consumer_refuses(self):
  with self.assertRaises(AbiRefusal):self.parse(self.body(first='FUN_00001000(x);\n other = 1;\n result = FUN_00002000();'))
 def test_nonempty_consumer_argument_refuses(self):
  with self.assertRaises(AbiRefusal):self.parse(self.body(first='FUN_00001000(x);\n result = FUN_00002000(x);'))
 def test_assigned_producer_refuses(self):
  with self.assertRaises(AbiRefusal):self.parse(self.body(first='x = FUN_00001000(x);\n result = FUN_00002000();'))
 def test_split_dword_source_call_refuses(self):
  with self.assertRaises(AbiRefusal):self.parse(self.body(first='FUN_00001000(1,2);\n result = FUN_00002000();'))
 def test_unknown_input_refuses(self):
  with self.assertRaises(AbiRefusal):self.parse(self.body(first='FUN_00001000(missing);\n result = FUN_00002000();'))
 def test_integer_only_input_refuses(self):
  with self.assertRaises(AbiRefusal):self.parse(self.body(first='FUN_00001000(result);\n result = FUN_00002000();'))
 def test_nested_call_input_refuses(self):
  with self.assertRaises(AbiRefusal):self.parse(self.body(first='FUN_00001000(FUN_00003000());\n result = FUN_00002000();'))
 def test_mutating_input_refuses(self):
  with self.assertRaises(AbiRefusal):self.parse(self.body(first='FUN_00001000(x++);\n result = FUN_00002000();'))
 def test_comment_in_selected_span_refuses(self):
  with self.assertRaises(AbiRefusal):self.parse(self.body(first='FUN_00001000(x); /* keep */\n result = FUN_00002000();'))
 def test_one_unpaired_producer_refuses_all(self):
  with self.assertRaises(AbiRefusal):self.parse(self.body(second='FUN_00001000(x);'))
 def test_unknown_destination_refuses(self):
  with self.assertRaises(AbiRefusal):self.parse(self.body(first='FUN_00001000(x);\n missing = FUN_00002000();'))
 def test_shadowed_destination_refuses(self):
  with self.assertRaises(AbiRefusal):self.parse(self.body(middle='{ int result; }'))
 def test_float_destination_refuses(self):
  with self.assertRaises(AbiRefusal):self.parse(self.body().replace('int result;','float result;'))
 def test_source_expression_retained_once(self):
  rows=self.parse(self.body(first='FUN_00001000(x * x + 2.0);\n result = FUN_00002000();'));self.assertEqual(rows[0][2].count('x * x + 2.0'),1)
class NativePairs(unittest.TestCase):
 def proof(self,pairs=1,extra=True,store=None,cleanup=True,intervening=False,count=None):
  entry=0x3000;callee=0x1000;helper=0x2000;raw=bytearray()
  def call(target):raw.extend(b'\xe8'+struct.pack('<i',target-(entry+len(raw)+5)))
  for n in range(pairs):
   raw.extend(bytes.fromhex('83ec08'));raw.extend(bytes.fromhex(store or 'dd1c24'));call(callee)
   if cleanup:raw.extend(bytes.fromhex('83c408'))
   if intervening:raw.extend(b'\x90')
   call(helper)
  if extra:call(helper)
  raw.extend(b'\xc3');data={entry:bytes(raw),callee:bytes.fromhex('dd442404c3')};decoder=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);decoder.detail=True
  def original(at,size):
   for start,body in data.items():
    if start<=at and at+size<=start+len(body):return body[at-start:at-start+size]
   raise ValueError('Outside fixture')
  def function(at):
   body=data[at];return None,dict(contiguous=True,ranges=[dict(start='%08x'%at,end_inclusive='%08x'%(at+len(body)-1),bytes_hex=body.hex())])
  return native_pairs(SimpleNamespace(original=original,decoder=decoder),SimpleNamespace(function=function),entry,callee,hashlib.sha256(data[callee]).hexdigest(),helper,pairs if count is None else count)
 def test_unrelated_helper_calls_do_not_confuse_pair(self):self.assertEqual(len(self.proof(extra=True)),1)
 def test_multiple_local_native_pairs(self):self.assertEqual(len(self.proof(pairs=2)),2)
 def test_missing_cleanup_refuses(self):
  with self.assertRaises(AbiRefusal):self.proof(cleanup=False)
 def test_intervening_instruction_refuses(self):
  with self.assertRaises(AbiRefusal):self.proof(intervening=True)
 def test_partial_dword_store_refuses(self):
  with self.assertRaises(AbiRefusal):self.proof(store='d91c24')
 def test_qword_at_wrong_offset_refuses(self):
  with self.assertRaises(AbiRefusal):self.proof(store='dd5c2404')
 def test_source_native_count_mismatch_refuses(self):
  with self.assertRaises(AbiRefusal):self.proof(count=2)
if __name__=='__main__':unittest.main()
