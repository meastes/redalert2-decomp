"""treemap.py books the handwritten assembly procedures of assembly.json as reproduced (exact), not open."""
import json
from pathlib import Path
import sys
import tempfile
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools/original_target'))
import treemap


class FakeInventory:
    def all_functions(self):
        def row(entry, end):
            return dict(contiguous=True, ranges=[dict(end_inclusive='%x' % end)], qualified_name='f%x' % entry)
        return {0x401000: row(0x401000, 0x40104F), 0x77EFC0: row(0x77EFC0, 0x77F00F), 0x402000: row(0x402000, 0x40201F)}


class AssemblyRowsTest(unittest.TestCase):
    def setUp(self):
        self.saved = {name: getattr(treemap, name) for name in
                      ('accepted_regions', 'assembly_regions', 'Inventory', 'rtti_tables', 'original_source_spans',
                       'open_misses')}
        treemap.accepted_regions = lambda: [dict(entry=0x401000, body_bytes=0x50, symbol='?f@@YAXXZ',
                                                 source='src/recovered/x/f.cpp')]
        treemap.Inventory = FakeInventory
        treemap.rtti_tables = lambda: ({}, {})
        treemap.original_source_spans = lambda: []
        treemap.open_misses = lambda: {}

    def tearDown(self):
        for name, value in self.saved.items():
            setattr(treemap, name, value)

    def test_manifest_rows(self):
        with tempfile.TemporaryDirectory() as scratch:
            path = Path(scratch) / 'manifests/original_target'
            path.mkdir(parents=True)
            (path / 'assembly.json').write_text(json.dumps(dict(regions=[
                dict(entry='0077efc0', bytes=80, symbol='_Proc', source='src/recovered/asm/a.asm', section='.text'),
                dict(entry='007ab000', bytes=4, symbol='_Table', source='src/recovered/asm/a.asm', section='.data')])))
            rows = self.saved['assembly_regions'](scratch)
            self.assertEqual(rows, [dict(entry=0x77EFC0, body_bytes=80, symbol='_Proc', source='src/recovered/asm/a.asm')])
            self.assertEqual(self.saved['assembly_regions'](Path(scratch) / 'none'), [])

    def test_assembly_procedure_is_exact_and_others_stay_open(self):
        treemap.assembly_regions = lambda: [dict(entry=0x77EFC0, body_bytes=0x50, symbol='_Proc',
                                                 source='src/recovered/asm/a.asm'),
                                            dict(entry=0x401000, body_bytes=0x50, symbol='_Dup', source='')]
        functions, _ = treemap.collect()
        status = {f['entry']: (f['status'], f['bytes']) for f in functions}
        self.assertEqual(status[0x77EFC0], ('exact', 0x50))
        self.assertEqual(status[0x401000], ('exact', 0x50))
        self.assertEqual(status[0x402000], ('remaining', 0x20))
        self.assertEqual(len(functions), 3)       # a catalogued entry is not booked twice
        self.assertEqual(next(f for f in functions if f['entry'] == 0x77EFC0)['group'], 'asm')


if __name__ == '__main__':
    unittest.main()
