import unittest

from seed_bullet_ai import CFGRefusal, repair_height_receivers


SOURCE = """    double local_138;
    double local_130;
    double local_128;
    int local_100;
            uVar20 = (undefined4)(FUN_0077f110());
            a = FUN_0077f110(); b = FUN_0077f110();
            FUN_0041b630();
            FUN_004642a0();
            c = FUN_0077f110(); d = FUN_0077f110(); e = FUN_0077f110();
            FUN_00434820();
            FUN_0055a280();
            local_160 = (int)(iVar5);
            lookup((int *)(&local_16c));
            lookup((int *)(local_108));
            if ((int)local_164 < iVar5 && iVar5 <= local_100) first = true;
            else if (local_100 < iVar5) second = true;
            bVar2 = (bool)(false);
"""


class BulletHeightReceiverTests(unittest.TestCase):
    def test_binds_both_signed_comparisons_to_written_coordinates(self):
        repaired = repair_height_receivers(SOURCE)
        self.assertIn("next_position += frame_velocity;", repaired)
        self.assertIn("height_position.Set_00434820(next_x, next_y, next_z);", repaired)
        self.assertIn("height_position.Z < iVar5 && iVar5 <= previous_position.Z", repaired)
        self.assertNotIn("local_100", repaired)

    def test_refuses_a_later_use_of_the_reused_word(self):
        with self.assertRaises(CFGRefusal):
            repair_height_receivers(SOURCE + "consume(local_100);\n")

    def test_refuses_different_height_lookup_storage(self):
        with self.assertRaises(CFGRefusal):
            repair_height_receivers(SOURCE.replace("(local_108)", "(local_10c)"))
