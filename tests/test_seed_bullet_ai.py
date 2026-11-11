import hashlib
import unittest
from unittest.mock import patch

import seed_bullet_ai as bullet


class BulletSeedTests(unittest.TestCase):
    def test_acceptance_prevents_scratch_rewrite(self):
        with patch.object(bullet, "verify_native") as prove:
            self.assertEqual(bullet.optional_bullet_ai(
                "other", bullet.ENTRY, None, ".", {bullet.ENTRY: "source"}), "other")
            prove.assert_not_called()

    def test_seed_drift_prevents_native_probe(self):
        with patch.object(bullet, "verify_native") as prove:
            self.assertEqual(bullet.optional_bullet_ai(
                "changed", bullet.ENTRY, None, ".", {}), "changed")
            prove.assert_not_called()

    def test_native_refusal_returns_original_seed(self):
        text = "toy seed"
        digest = hashlib.sha256(text.encode()).hexdigest()
        with patch.object(bullet, "SEED_SHA256", digest), patch.object(
                bullet, "verify_native", side_effect=bullet.CFGRefusal("extent")):
            self.assertEqual(bullet.optional_bullet_ai(text, bullet.ENTRY, None, ".", {}), text)

    def test_other_entry_uses_normal_seed(self):
        with patch.object(bullet, "verify_native") as prove:
            self.assertEqual(bullet.optional_bullet_ai("other", 0x1000, None, ".", {}), "other")
            prove.assert_not_called()
