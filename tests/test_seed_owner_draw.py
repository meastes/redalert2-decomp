import unittest
from pathlib import Path
from unittest.mock import patch
import seed_owner_draw as owner

class Providers:
    def accepted(self,target):return dict(symbol=owner.PROVIDERS[target][0])
    def cleanup(self,target):return owner.PROVIDERS[target][1]

class OwnerDrawSeedTests(unittest.TestCase):
    def test_changed_export_keeps_generic_path(self):
        with patch.object(owner,'verify_native') as verify:
            self.assertIsNone(owner.optional_owner_draw_seed('changed export',None,Path('.')))
            verify.assert_not_called()

    def test_native_refusal_keeps_original_path(self):
        with patch.object(owner.hashlib,'sha256') as digest,patch.object(owner,'verify_native',side_effect=ValueError('different provider')):
            digest.return_value.hexdigest.return_value=owner.RAW_SHA256
            self.assertIsNone(owner.optional_owner_draw_seed('frozen export',None,Path('.')))

    def test_changed_root_body_refuses_before_provider_reads(self):
        with self.assertRaisesRegex(ValueError,'body differs'):
            owner.verify_graph(dict(body_sha256='different'),Providers())

    def test_wrong_provider_symbol_is_refused(self):
        receiver=Providers()
        with patch.object(receiver,'accepted',return_value=dict(symbol='different')),self.assertRaisesRegex(ValueError,'symbol differs'):
            owner.verify_graph(dict(body_sha256=owner.BODY_SHA256),receiver)

    def test_wrong_native_cleanup_is_refused(self):
        receiver=Providers()
        with patch.object(receiver,'cleanup',return_value=256),self.assertRaisesRegex(ValueError,'cleanup differs'):
            owner.verify_graph(dict(body_sha256=owner.BODY_SHA256),receiver)

    def test_changed_canonical_provider_extent_refuses_before_decode(self):
        receiver=Providers()
        with patch.object(receiver,'accepted',return_value=dict(body_bytes=1,body_sha256='different')),self.assertRaisesRegex(ValueError,'extent differs'):
            owner.canonical_cleanup(receiver,next(iter(owner.CANONICAL_EXTENTS)))

    def test_verified_seed_keeps_typed_placement_and_production(self):
        with patch.object(owner,'canonical_cleanup',side_effect=lambda receiver,target:owner.CANONICAL_EXTENTS[target][2]):
            owner.verify_graph(dict(body_sha256=owner.BODY_SHA256),Providers())
        with patch.object(owner.hashlib,'sha256') as digest,patch.object(owner,'verify_native'):
            digest.return_value.hexdigest.return_value=owner.RAW_SHA256
            source=owner.optional_owner_draw_seed('frozen export',None,Path('.'))
        self.assertIn('static WolDictionary<UnknownPairKey, bool> ctrlmessages',source)
        self.assertIn('static WolArrayList<HWND> hwndarray',source)
        self.assertIn('CallWindowProcA(specific_proc, window, message, wparam, lparam)',source)
        self.assertNotIn('unique0x',source)

    def test_cpp_path_escapes_and_provisional_bindings_are_preserved(self):
        self.assertIn(r'D:\\Ra2\\ownrdraw.cpp',owner.CANDIDATE)
        self.assertNotIn('// @ref _$E85=',owner.CANDIDATE)
        self.assertNotIn('// @ref _$E86=',owner.CANDIDATE)
        self.assertNotIn('exact-match score 100',owner.CANDIDATE)
