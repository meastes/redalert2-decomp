from types import SimpleNamespace
import unittest

from seed_rect_scan import render, source_binding
from seed_guarded_cfg import CFGRefusal


PARAMETERS = ["int receiver", "int unused_fastcall_register_1", "int unused_0", "int unused_1"]
SEED = SimpleNamespace(code_view=lambda body: body)
BODY = """{
    int *unaff_retaddr;
    undefined4 packet;
    undefined4 initial;
    x0 = *packet + 12;
    y0 = packet[1] + 20;
    initial._2_2_ = (short)y0;
    initial._0_2_ = x0 + -2;
    rows = packet[3] / 0xf + 0x11;
    index = 0;
    columns = packet[2] / 0x3c + 4;
    packet._0_2_ = x0;
    packet._2_2_ = initial._2_2_;
    do {
        packet._0_2_ = (short)step + (short)initial;
        packet._2_2_ = initial._2_2_ + step._2_2_;
        remaining = columns;
        do {
            check = FUN_00400000(&stack0xffffffb0);
            if (check) {
                cell = FUN_00401000(&stack0xffffffb0);
                if (((*unaff_retaddr <= x0) &&
                     (x0 < unaff_retaddr[2] + *unaff_retaddr)) &&
                    ((unaff_retaddr[1] <= y0 &&
                      (y0 < unaff_retaddr[3] + unaff_retaddr[1])))) {
                    draw(cell);
                }
            }
            remaining = remaining + -1;
        } while (remaining != 0);
    } while (++index < rows);
}"""


class RectScanTests(unittest.TestCase):
    def test_splits_rectangles_and_restores_both_scan_coordinates(self):
        repaired = render(BODY, source_binding(BODY, PARAMETERS, SEED))
        self.assertIn("x0 = scan_area->X + 12;", repaired)
        self.assertIn("scan_clip->Width + scan_clip->X", repaired)
        self.assertIn("Cell scan_cell = row_cell;", repaired)
        self.assertEqual(repaired.count("(&scan_cell.X)"), 2)
        self.assertIn("++scan_cell.X;\n            --scan_cell.Y;", repaired)

    def test_refuses_an_additional_read_of_reused_home(self):
        with self.assertRaises(CFGRefusal):
            source_binding(BODY.replace("draw(cell);", "draw(packet);"), PARAMETERS, SEED)

    def test_refuses_a_live_later_use_of_packed_base(self):
        with self.assertRaises(CFGRefusal):
            source_binding(BODY.replace("draw(cell);", "draw(initial);"), PARAMETERS, SEED)

    def test_refuses_call_with_different_scan_home(self):
        text = BODY.replace("FUN_00401000(&stack0xffffffb0)", "FUN_00401000(&stack0xffffffac)")
        with self.assertRaises(CFGRefusal):
            source_binding(text, PARAMETERS, SEED)

    def test_refuses_control_flow_that_can_bypass_the_scan_update(self):
        with self.assertRaises(CFGRefusal):
            source_binding(BODY.replace("draw(cell);", "continue;"), PARAMETERS, SEED)

    def test_refuses_already_bound_stack_parameter(self):
        with self.assertRaises(CFGRefusal):
            source_binding(BODY, PARAMETERS[:2] + ["int area", "int clip"], SEED)
