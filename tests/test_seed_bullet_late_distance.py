import unittest
from unittest.mock import patch

import seed_bullet_ai as bullet


class LateBulletDistanceTests(unittest.TestCase):
    def test_refuses_missing_double_floor_declaration(self):
        with self.assertRaisesRegex(bullet.CFGRefusal, "Frozen late distance seed shape changed"):
            bullet.repair_late_distance("const double minimum_limit = 128.0;")

    def test_distance_refusal_keeps_both_coordinate_repairs(self):
        original = "known seed"
        earlier = "both coordinate repairs"
        with patch.object(bullet.hashlib, "sha256") as digest, patch.object(
                bullet, "verify_native", return_value=(None, {}, None)), patch.object(
                bullet, "verify_coordinate_calls", return_value={}), patch.object(
                bullet, "verify_late_coordinates", return_value={}), patch.object(
                bullet, "verify_late_distance", side_effect=bullet.CFGRefusal("unknown conversion")), patch.object(
                bullet, "repair_speed_and_rotation", return_value=original), patch.object(
                bullet, "repair_signed_coordinates", return_value=original), patch.object(
                bullet, "repair_height_receivers", return_value=original), patch.object(
                bullet, "repair_coordinate_returns", return_value=original), patch.object(
                bullet, "repair_late_coordinates", return_value=earlier):
            digest.return_value.hexdigest.return_value = bullet.SEED_SHA256
            self.assertTrue(bullet.optional_bullet_ai(original, bullet.ENTRY, None, ".", {}).endswith(earlier))
