from collections import defaultdict
import unittest

import capstone

import seed_candidate as seed
from seed_guarded_cfg import CFGRefusal
from seed_height_coordinate import first_coordinate, render, source_binding


PARAMS = ["int *coord", "int flags"]
BODY = """{
    short low;
    short high;
    float old_x;
    undefined8 old_y;
    int cell;
    int saved;
    int result;
    int use;
    low = (short)(*coord + (*coord >> 0x1f & 0xffU) >> 8);
    high = (short)(coord[1] + (coord[1] >> 0x1f & 0xffU) >> 8);
    old_x = (float)(low * 0x100 + 0x80);
    old_y = (double)(ulonglong)(high * 0x100 + 0x80);
    saved = cell;
    result = FUN_00004000();
    use = FUN_00004000();
    old_y = (double)use;
    result = (int)old_y;
}
"""


def first_call_graph(z=0, y_offset=0x30, x_offset=0x30):
    # The push makes the X displacement one word larger. Y is stored before it.
    code = bytes.fromhex("0fbf4c2416 0fbf442414 c1e108 c1e008 81c180000000 "
                         "8d54242c 0580000000 894c24")
    code += bytes([y_offset])
    code += bytes.fromhex("52 b900200000 894424") + bytes([x_offset])
    code += bytes.fromhex("c7442438") + bytes([z,0,0,0])
    call = 0x1000 + len(code)
    code += bytes.fromhex("e800000000")
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True
    nodes = {ins.address:ins for ins in decoder.disasm(code,0x1000)}
    edges = {at:[at+ins.size] for at,ins in nodes.items()}
    pred = defaultdict(set)
    for at, dest in edges.items():
        pred[dest[0]].add(at)
    return {"nodes":nodes,"edges":edges,"pred":pred}, call


class HeightCoordinate(unittest.TestCase):
    def test_source_recovers_distinct_inputs(self):
        binding = source_binding(BODY, PARAMS, seed)
        changed = render(BODY,binding,{"receiver":0x2000})
        self.assertIn("FUN_00004000(&DAT_00002000, &height_coord)",changed)
        self.assertIn("FUN_00004000(&DAT_00002000, coord)",changed)
        self.assertLess(changed.index("height_coord.Y"),changed.index("height_coord.X"))
        self.assertIn("old_y = (double)use;",changed)

    def test_input_reassignment_refuses(self):
        with self.assertRaises(CFGRefusal):
            source_binding(BODY.replace("use = FUN_", "coord = 0; use = FUN_"), PARAMS, seed)

    def test_carrier_read_before_overwrite_refuses(self):
        with self.assertRaises(CFGRefusal):
            source_binding(BODY.replace("use = FUN_", "result = (int)old_y; use = FUN_"), PARAMS, seed)

    def test_old_carrier_used_to_overwrite_itself_refuses(self):
        with self.assertRaises(CFGRefusal):
            source_binding(BODY.replace("(double)use","(double)old_y"), PARAMS, seed)

    def test_y_x_z_stores_account_for_argument_push(self):
        graph, call = first_call_graph()
        self.assertEqual(first_coordinate(graph,call),
                         (0x2000,[(0x30,("center",0x16)),
                                  (0x2c,("center",0x14)),(0x34,("imm",0))]))

    def test_changed_z_or_overlapping_store_refuses(self):
        for case in ({"z":1},{"x_offset":0x34},{"y_offset":0x2c}):
            with self.assertRaises(CFGRefusal):
                first_coordinate(*first_call_graph(**case))
