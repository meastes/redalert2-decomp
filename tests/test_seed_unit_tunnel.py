import unittest
from pathlib import Path
from unittest.mock import patch
import seed_unit_tunnel as tunnel

class Providers:
    def accepted(self,target):return dict(symbol=tunnel.PROVIDERS[target][0])
    def cleanup(self,target):return tunnel.PROVIDERS[target][1]

class UnitTunnelSeedTests(unittest.TestCase):
    def test_changed_export_does_not_use_typed_seed(self):
        with patch.object(tunnel,'verify_native') as verify:
            self.assertIsNone(tunnel.optional_tunnel_seed('changed export',None,Path('.')))
            verify.assert_not_called()

    def test_native_refusal_keeps_generic_storage_checks(self):
        with patch.object(tunnel.hashlib,'sha256') as digest, patch.object(tunnel,'verify_native',side_effect=ValueError('different tunnel provider')):
            digest.return_value.hexdigest.return_value=tunnel.RAW_SHA256
            self.assertIsNone(tunnel.optional_tunnel_seed('frozen export',None,Path('.')))

    def test_native_body_must_match_before_provider_acceptance(self):
        with self.assertRaisesRegex(ValueError,'body differs'):
            tunnel.verify_graph(dict(body_sha256='changed'),Providers())

    def test_wrong_provider_cleanup_is_refused(self):
        receiver=Providers()
        with patch.object(receiver,'cleanup',return_value=256),self.assertRaisesRegex(ValueError,'ABI differs'):
            tunnel.verify_graph(dict(body_sha256=tunnel.BODY_SHA256),receiver)

    def test_verified_seed_uses_typed_coordinate_and_fp_operations(self):
        tunnel.verify_graph(dict(body_sha256=tunnel.BODY_SHA256),Providers())
        with patch.object(tunnel.hashlib,'sha256') as digest,patch.object(tunnel,'verify_native'):
            digest.return_value.hexdigest.return_value=tunnel.RAW_SHA256
            source=tunnel.optional_tunnel_seed('frozen export',None,Path('.'))
        self.assertIn('Fast_Sin(radians)*distance',source)
        self.assertIn('Map.Get_Height_GL(exit)-enter_height',source)
        self.assertIn('Cell_Tube_0047a130(&Map[Position_Coord_004f3f40()])',source)
        self.assertNotIn('unique0x',source)
