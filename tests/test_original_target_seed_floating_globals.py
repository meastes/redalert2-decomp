import struct
import unittest

from seed_candidate import floating_operand_globals


class FloatingOperandGlobalTests(unittest.TestCase):
    address = 0x765400
    symbol = '_PTR_value_00765400'

    def operand(self, opcode, address=None):
        return bytes.fromhex(opcode) + struct.pack('<I', self.address if address is None else address)

    def infer(self, expression, code, params=('double value',)):
        row = {'ranges': [{'start': '00401000', 'bytes_hex': code.hex()}]}
        return floating_operand_globals(expression.replace('GLOBAL', self.symbol), row, params)

    def test_real_memory_width_for_both_operand_sizes(self):
        for opcode, kind in [('dc 0d', 'double'), ('d8 0d', 'float')]:
            with self.subTest(kind=kind):
                self.assertEqual(self.infer('return value * GLOBAL;', self.operand(opcode)),
                                 {self.symbol: kind})

    def test_chained_double_scales(self):
        body = 'return (double)count * GLOBAL * _DAT_00765410;'
        code = self.operand('dc 0d') + self.operand('dc 0d', self.address+16)
        self.assertEqual(self.infer(body, code, ('int count',)),
                         {self.symbol: 'double', '_DAT_00765410': 'double'})

    def test_subtract_and_ordered_comparison(self):
        self.assertEqual(self.infer('return value - GLOBAL;', self.operand('d8 25')),
                         {self.symbol: 'float'})
        self.assertEqual(self.infer('if (GLOBAL < value) {}', self.operand('dc 1d')),
                         {self.symbol: 'double'})

    def test_source_type_required(self):
        for params in [('int value',), ('char *value',), ()]:
            with self.subTest(params=params):
                self.assertEqual(self.infer('return value * GLOBAL;', self.operand('dc 0d'), params), {})

    def test_mixed_uses_and_aliases_refuse(self):
        for body in ['value * GLOBAL; use(&GLOBAL);', 'value * GLOBAL; use(DAT_00765400);',
                     'value * GLOBAL[0];', 'return *GLOBAL;', '/*value * GLOBAL*/',
                     'return (int)value * GLOBAL;', 'return ptr->value * GLOBAL;']:
            with self.subTest(body=body):
                self.assertEqual(self.infer(body, self.operand('dc 0d')), {})

    def test_missing_or_incompatible_native_access_refuses(self):
        for code in [b'', self.operand('dc 0d', self.address+16), self.operand('d8 25'),
                     self.operand('dc 88'), self.operand('64 dc 0d'),
                     self.operand('dc 0d') * 2, self.operand('dc 0d') + self.operand('b8'),
                     self.operand('dc 0d') + b'\x0f']:
            with self.subTest(code=code):
                self.assertEqual(self.infer('value * GLOBAL;', code), {})
