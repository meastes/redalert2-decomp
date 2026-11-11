"""Authored machine fixtures for native-proved DWORD carrier stores."""
from pathlib import Path
from types import SimpleNamespace
import struct
import sys
import unittest
import capstone
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools/original_target'))
import seed_candidate as seed
from seed_carrier_stores import source_groups,inspect_carrier_stores,CarrierRefusal
from seed_switches import inspect_index_switch,UnsupportedSwitch

ENTRY=0x401000
CALLEE=0x401100
class Machine:
    def __init__(self, middle=b'', word=False, callee=b'\xc3', alignment=None):
        self.base=0x400000
        self.decoder=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);self.decoder.detail=True
        chunks=[];at=15
        for _ in range(2):
            call=b'\xe8'+struct.pack('<i',CALLEE-(ENTRY+at+5))
            store=(b'\x66' if word else b'')+b'\x89\x44\x24\x00'
            chunk=call+middle+store+b'\x83\xc4\x08\xc3';chunks.append(chunk);at+=len(chunk)
        default=at;code_end=at+4;gap=(-code_end)%4;self.table=ENTRY+code_end+gap
        align=(b'\x8d\x49\x00' if gap==3 else b'\x90'*gap) if alignment is None else alignment
        assert len(align)==gap
        code=b'\x83\xec\x08\x83\xf8\x01\x77'+bytes([default-8])+b'\xff\x24\x85'+struct.pack('<I',self.table)
        self.data=bytearray(code+b''.join(chunks)+b'\x83\xc4\x08\xc3'+align+struct.pack('<II',ENTRY+15,ENTRY+15+len(chunks[0])))
        self.default=ENTRY+default;self.root_size=len(self.data);self.data+=b'\x90'*(0x100-len(self.data));self.data+=callee+b'\x90'*16;self.callee_size=len(callee)
        self.section=SimpleNamespace(Characteristics=0x60000020,VirtualAddress=0x1000,SizeOfRawData=len(self.data));self.pe=SimpleNamespace(get_section_by_rva=lambda r:self.section)
    def original(self,address,length):return bytes(self.data[address-ENTRY:address-ENTRY+length])
    def function(self,address):
        size=self.root_size if address==ENTRY else self.callee_size if address==CALLEE else None
        if size is None:return None,None
        return None,dict(contiguous=True,ranges=[dict(start='%08x'%address,end_inclusive='%08x'%(address+size-1),bytes_hex=self.original(address,size).hex())])

BODY="""{
 undefined8 slot;
 uint value;
 switch(n) {
 case 0:
  value = FUN_00401100();
  slot = (double)CONCAT44(slot._4_4_,value);
  break;
 case 1:
  value = FUN_00401100();
  slot = (double)CONCAT44(slot._4_4_,value);
  break;
 }
}"""

class CarrierSourceTests(unittest.TestCase):
    def test_only_exact_source_store_starts_are_authorized(self):
        groups=source_groups(BODY,['int n'],seed)
        self.assertEqual(set(groups),{0,1});self.assertEqual([len(x) for x in groups.values()],[1,1])
        starts={x['start'] for rows in groups.values() for x in rows}
        lowered=seed.floating_word_store_artifacts(BODY,['int n'],starts)
        self.assertEqual(lowered.count('*((uint *)&(slot)) = (uint)(value);'),2)
        self.assertEqual(seed.floating_word_store_artifacts(BODY,['int n']),BODY)
    def test_effectful_and_nonadjacent_values_refuse(self):
        for replacement in ('slot = (double)CONCAT44(slot._4_4_,value++);','slot = (double)CONCAT44(other._4_4_,value);', 'use(value);\n slot = (double)CONCAT44(slot._4_4_,value);'):
            with self.subTest(replacement=replacement):
                self.assertIsNone(source_groups(BODY.replace('slot = (double)CONCAT44(slot._4_4_,value);',replacement),['int n'],seed))
    def test_ambiguous_and_nonlocal_storage_refuses(self):
        for declaration in ('undefined8 slot,other;', 'volatile undefined8 slot;', 'undefined8 slot[2];', 'undefined8 slot;\n {int slot;}', 'double slot;'):
            self.assertIsNone(source_groups(BODY.replace('undefined8 slot;',declaration),['int n'],seed))
        self.assertIsNone(source_groups(BODY.replace(' undefined8 slot;\n',''),['int n','undefined8 slot'],seed))
    def test_wide_float_and_unknown_value_types_refuse(self):
        for kind in ('float','double','ulonglong','Unknown','volatile uint'):
            self.assertIsNone(source_groups(BODY.replace('uint value;',kind+' value;'),['int n'],seed))
    def test_source_control_flow_and_macro_scopes_refuse(self):
        for extra in ('if(n) use(n);','while(n) use(n);','goto somewhere;','#define value 1','struct Local {};'):
            self.assertIsNone(source_groups(BODY.replace(' case 0:', ' case 0:\n '+extra),['int n'],seed))
    def test_nested_or_duplicate_cases_refuse(self):
        self.assertIsNone(source_groups(BODY.replace('case 1:','case 0:'),['int n'],seed))
        self.assertIsNone(source_groups(BODY.replace('  break;',' switch(n){}\n break;',1),['int n'],seed))
    def test_comments_and_strings_never_authorize_a_store(self):
        text='{ undefined8 slot; uint value; char *p="'+BODY.replace('\n',' ')+'"; }'
        self.assertIsNone(source_groups(text,['int n'],seed))

class NativeCarrierTests(unittest.TestCase):
    def check(self,machine=None,body=BODY):
        machine=Machine() if machine is None else machine
        return inspect_carrier_stores(machine,ENTRY,machine,source_groups(body,['int n'],seed))
    def test_two_cases_match_the_same_proved_storage(self):
        facts=self.check();self.assertEqual(len(facts['stores']),2)
        self.assertEqual({x['offset'] for x in facts['stores']},{-8})
        self.assertEqual(facts['callee_cleanup'],{'00401100':0})
    def test_wrong_callee_identity_refuses(self):
        with self.assertRaisesRegex(CarrierRefusal,'call order'):
            self.check(body=BODY.replace('FUN_00401100','FUN_00401120'))
    def test_word_store_does_not_prove_a_dword(self):
        with self.assertRaises(CarrierRefusal):
            self.check(Machine(word=True))
    def test_eax_clobber_is_not_a_call_result(self):
        with self.assertRaises(CarrierRefusal):self.check(Machine(middle=b'\x31\xc0'))
    def test_upper_word_write_refuses_even_when_low_word_matches(self):
        with self.assertRaisesRegex(CarrierRefusal,'overwritten'):
            self.check(Machine(middle=b'\x89\x54\x24\x04'))
    def test_partial_eax_writes_are_not_a_call_result(self):
        for code in (b'\xb0\x01',b'\xb4\x01',b'\x66\xb8\x01\x00'):
            with self.subTest(code=code), self.assertRaises(CarrierRefusal):
                self.check(Machine(middle=code))
    def test_prior_low_word_store_is_not_hidden_by_the_dword(self):
        with self.assertRaisesRegex(CarrierRefusal,'overwritten'):
            self.check(Machine(middle=b'\x66\x89\x44\x24\x00'))
    def test_sixteen_bit_push_is_not_counted_as_four_bytes(self):
        with self.assertRaisesRegex(CarrierRefusal,'PUSH width'):
            self.check(Machine(middle=b'\x66\x50'))
    def test_partial_sp_write_cannot_preserve_the_frame(self):
        with self.assertRaisesRegex(CarrierRefusal,'ESP write'):
            self.check(Machine(middle=b'\x66\x83\xc4\x02'))
    def test_far_and_sixteen_bit_return_do_not_prove_near_call_cleanup(self):
        for code in (b'\xcb',b'\x66\xc3'):
            with self.subTest(code=code), self.assertRaisesRegex(CarrierRefusal,'far/16-bit'):
                self.check(Machine(callee=code))
    def test_unbalanced_callee_stack_refuses(self):
        with self.assertRaisesRegex(CarrierRefusal,'not restored'):
            self.check(Machine(callee=b'\x50\xc3'))
    def test_unknown_stack_alignment_in_callee_refuses(self):
        with self.assertRaisesRegex(CarrierRefusal,'ESP write'):
            self.check(Machine(callee=b'\x83\xe4\xf8\xc3'))
    def test_unknown_callee_extent_refuses(self):
        machine=Machine();original=machine.function
        machine.function=lambda address:(None,None) if address==CALLEE else original(address)
        with self.assertRaisesRegex(CarrierRefusal,'contiguous'):self.check(machine)
    def test_default_edge_cannot_bypass_the_case_call(self):
        machine=Machine();second=struct.unpack('<I',machine.original(machine.table+4,4))[0]
        machine.data[7]=(second+5)-(ENTRY+8)
        # Preserve reachability of the trailing default cleanup from case 1.
        default=machine.default-ENTRY
        machine.data[default-4:default]=b'\x75\x02\x90\x90'
        with self.assertRaisesRegex(CarrierRefusal,'incoming'):
            self.check(machine)
    def test_cross_case_fallthrough_cannot_change_the_frame(self):
        machine=Machine();second=struct.unpack('<I',machine.original(machine.table+4,4))[0]
        machine.data[second-ENTRY-1]=0x90
        with self.assertRaisesRegex(CarrierRefusal,'incoming'):
            self.check(machine)
    def test_case_reentry_to_the_guard_is_not_a_fresh_frame(self):
        machine=Machine();second=struct.unpack('<I',machine.original(machine.table+4,4))[0];at=second-ENTRY-4
        machine.data[at:at+4]=bytes([0x75,(3-(at+2))&0xff,0x90,0x90])
        with self.assertRaisesRegex(CarrierRefusal,'reentry'):
            self.check(machine)
    def test_source_case_set_must_equal_native_guard(self):
        machine=Machine();groups=source_groups(BODY,['int n'],seed);groups[2]=groups.pop(1)
        with self.assertRaisesRegex(CarrierRefusal,'case sets'):
            inspect_carrier_stores(machine,ENTRY,machine,groups)
    def test_source_carrier_identity_cannot_alias_two_names(self):
        body=BODY.replace('undefined8 slot;','undefined8 slot;\n undefined8 other;').replace('case 1:\n  value = FUN_00401100();\n  slot = (double)CONCAT44(slot._4_4_,value);','case 1:\n  value = FUN_00401100();\n  other = (double)CONCAT44(other._4_4_,value);')
        with self.assertRaisesRegex(CarrierRefusal,'overlap'):self.check(body=body)
    def test_inventory_bytes_must_match_original(self):
        machine=Machine();original=machine.function
        def wrong(address):
            directory,row=original(address)
            if row and address==CALLEE:row['ranges'][0]['bytes_hex']='90'
            return directory,row
        machine.function=wrong
        with self.assertRaisesRegex(CarrierRefusal,'bytes differ'):self.check(machine)

class AlignmentTests(unittest.TestCase):
    def test_observed_three_byte_identity_lea_is_known_alignment(self):
        machine=Machine();facts=inspect_index_switch(machine,ENTRY,ENTRY+8,machine.table)
        self.assertEqual(facts['internal_alignment_bytes'],3)
    def test_nonidentity_lea_is_not_alignment(self):
        for code in (b'\x8d\x49\x01',b'\x8d\x41\x00',b'\x83\xc1\x00'):
            machine=Machine(alignment=code)
            with self.assertRaisesRegex(UnsupportedSwitch,'alignment'):
                inspect_index_switch(machine,ENTRY,ENTRY+8,machine.table)

if __name__=='__main__':unittest.main()
