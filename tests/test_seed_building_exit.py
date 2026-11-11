import unittest
from pathlib import Path
from unittest.mock import patch
import seed_building_exit as building

class Providers:
    def accepted(self,target):return dict(symbol=building.PROVIDERS[target][0])
    def cleanup(self,target):return building.PROVIDERS[target][1]

class BuildingExitSeedTests(unittest.TestCase):
    def test_changed_export_keeps_generic_path(self):
        with patch.object(building,'verify_native') as verify:
            self.assertIsNone(building.optional_building_exit_seed('changed export',None,Path('.')))
            verify.assert_not_called()

    def test_native_refusal_keeps_original_path(self):
        with patch.object(building.hashlib,'sha256') as digest,patch.object(building,'verify_native',side_effect=ValueError('different provider')):
            digest.return_value.hexdigest.return_value=building.RAW_SHA256
            self.assertIsNone(building.optional_building_exit_seed('frozen export',None,Path('.')))

    def test_changed_root_body_refuses_before_provider_reads(self):
        with self.assertRaisesRegex(ValueError,'body differs'):
            building.verify_graph(dict(body_sha256='different'),Providers())

    def test_wrong_provider_symbol_is_refused(self):
        receiver=Providers()
        with patch.object(receiver,'accepted',return_value=dict(symbol='different')),self.assertRaisesRegex(ValueError,'symbol differs'):
            building.verify_graph(dict(body_sha256=building.BODY_SHA256),receiver)

    def test_wrong_native_cleanup_is_refused(self):
        receiver=Providers()
        with patch.object(receiver,'cleanup',return_value=256),self.assertRaisesRegex(ValueError,'cleanup differs'):
            building.verify_graph(dict(body_sha256=building.BODY_SHA256),receiver)

    def test_changed_canonical_provider_extent_refuses_before_decode(self):
        receiver=Providers()
        with patch.object(receiver,'accepted',return_value=dict(body_bytes=1,body_sha256='different')),self.assertRaisesRegex(ValueError,'extent differs'):
            building.canonical_cleanup(receiver,next(iter(building.CANONICAL_EXTENTS)))

    def test_verified_seed_keeps_typed_placement_and_production(self):
        with patch.object(building,'canonical_cleanup',side_effect=lambda receiver,target:building.CANONICAL_EXTENTS[target][2]):
            building.verify_graph(dict(body_sha256=building.BODY_SHA256),Providers())
        with patch.object(building.hashlib,'sha256') as digest,patch.object(building,'verify_native'):
            digest.return_value.hexdigest.return_value=building.RAW_SHA256
            source=building.optional_building_exit_seed('frozen export',None,Path('.'))
        self.assertIn('House_Base_Distance_Score',source)
        self.assertIn('Map.Nearby_Location(',source)
        self.assertIn('aircraft->Set_Mission(2)',source)
        self.assertNotIn('unique0x',source)
