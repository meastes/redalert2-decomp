import unittest
from unittest.mock import patch

import seed_bullet_ai as bullet


class LateBulletCoordinateTests(unittest.TestCase):
    def test_refuses_missing_result_buffer(self):
        text = ("void __fastcall FUN_0045f710(int *param_1)\n{\n"
                "        BulletClass *bullet = reinterpret_cast<BulletClass *>(param_1);\n")
        with self.assertRaisesRegex(bullet.CFGRefusal, "Frozen late coordinate seed shape changed"):
            bullet.repair_late_coordinates(text)

    def test_late_refusal_preserves_prior_coordinate_repair(self):
        original = "known seed"
        earlier = "prior coordinate repair"
        with patch.object(bullet.hashlib, "sha256") as digest, patch.object(
                bullet, "verify_native", return_value=(None, {}, None)), patch.object(
                bullet, "verify_coordinate_calls", return_value={}), patch.object(
                bullet, "verify_late_coordinates", side_effect=bullet.CFGRefusal("unknown receiver")), patch.object(
                bullet, "repair_speed_and_rotation", return_value=original), patch.object(
                bullet, "repair_signed_coordinates", return_value=original), patch.object(
                bullet, "repair_height_receivers", return_value=original), patch.object(
                bullet, "repair_coordinate_returns", return_value=earlier):
            digest.return_value.hexdigest.return_value = bullet.SEED_SHA256
            self.assertTrue(bullet.optional_bullet_ai(original, bullet.ENTRY, None, ".", {}).endswith(earlier))
