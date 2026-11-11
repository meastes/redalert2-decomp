import unittest
from pathlib import Path
from unittest.mock import patch

import seed_missile as missile


class MissileSeedTests(unittest.TestCase):
    def test_changed_export_does_not_consult_native_inputs(self):
        with patch.object(missile, "verify_native") as verify:
            self.assertIsNone(missile.optional_missile_seed("changed export", None, Path(".")))
            verify.assert_not_called()

    def test_native_refusal_preserves_existing_path(self):
        with patch.object(missile.hashlib, "sha256") as digest, patch.object(
                missile, "verify_native", side_effect=ValueError("different launch slot")):
            digest.return_value.hexdigest.return_value = missile.RAW_SHA256
            self.assertIsNone(missile.optional_missile_seed("frozen export", None, Path(".")))

    def test_verified_seed_uses_distinct_velocity_and_call_arguments(self):
        with patch.object(missile.hashlib, "sha256") as digest, patch.object(missile, "verify_native"):
            digest.return_value.hexdigest.return_value = missile.RAW_SHA256
            source = missile.optional_missile_seed("frozen export", None, Path("."))
        self.assertIn("TVelocity3D<double> velocity(0.0, 0.0, 0.0);", source)
        self.assertIn("bullet->unknown_slot118(turret, velocity)", source)
        self.assertIn("Map[House->NukeDest]", source)
        self.assertIn("Map[House->EMPDest]", source)
