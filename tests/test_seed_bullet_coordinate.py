import unittest
from unittest.mock import patch

import seed_bullet_ai as bullet

from seed_bullet_ai import CFGRefusal, repair_coordinate_returns


class BulletCoordinateTests(unittest.TestCase):
    def test_refuses_a_changed_buffer_lifetime(self):
        text = "local_108 local_108 local_108 local_108"
        with self.assertRaisesRegex(CFGRefusal, "Frozen coordinate seed shape changed"):
            repair_coordinate_returns(text)

    def test_refuses_source_without_the_expected_velocity_view(self):
        text = "namespace seed_0045f710 { }"
        with self.assertRaisesRegex(CFGRefusal, "Frozen coordinate seed shape changed"):
            repair_coordinate_returns(text)

    def test_virtual_refusal_preserves_the_previously_repaired_seed(self):
        original = "known original seed"
        with patch.object(bullet.hashlib, "sha256") as digest, patch.object(
                bullet, "verify_native", return_value=(None, {}, None)), patch.object(
                bullet, "verify_coordinate_calls", side_effect=CFGRefusal("missing table")), patch.object(
                bullet, "repair_speed_and_rotation", return_value=original), patch.object(
                bullet, "repair_signed_coordinates", return_value=original), patch.object(
                bullet, "repair_height_receivers", return_value="previous repair"):
            digest.return_value.hexdigest.return_value = bullet.SEED_SHA256
            self.assertTrue(bullet.optional_bullet_ai(
                original, bullet.ENTRY, None, ".", {}).endswith("previous repair"))
