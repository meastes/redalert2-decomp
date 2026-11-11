import unittest
from pathlib import Path
from unittest.mock import patch

import seed_quaternion as quaternion


class QuaternionSeedTests(unittest.TestCase):
    def test_changed_export_never_invokes_native_verifier(self):
        with patch.object(quaternion, "verify_native") as verify:
            self.assertIsNone(quaternion.optional_quaternion_seed("different export", None, Path(".")))
            verify.assert_not_called()

    def test_native_refusal_preserves_original_seed_path(self):
        with patch.object(quaternion.hashlib, "sha256") as digest, patch.object(
                quaternion, "verify_native", side_effect=ValueError("different native body")):
            digest.return_value.hexdigest.return_value = quaternion.RAW_SHA256
            self.assertIsNone(quaternion.optional_quaternion_seed("frozen export", None, Path(".")))

    def test_verified_export_emits_typed_make_closest(self):
        with patch.object(quaternion.hashlib, "sha256") as digest, patch.object(quaternion, "verify_native"):
            digest.return_value.hexdigest.return_value = quaternion.RAW_SHA256
            source = quaternion.optional_quaternion_seed("frozen export", None, Path("."))
        self.assertIn("Quaternion &Quaternion::Make_Closest(Quaternion const &target)", source)
        self.assertEqual(source.count("Conjugate(target)"), 2)
