import unittest
from pathlib import Path
from unittest.mock import patch

import seed_unit_voxel as voxel


class UnitVoxelSeedTests(unittest.TestCase):
    def test_changed_export_keeps_the_original_path(self):
        with patch.object(voxel, "verify_native") as verify:
            self.assertIsNone(voxel.optional_voxel_seed("changed export", None, Path(".")))
            verify.assert_not_called()

    def test_native_refusal_keeps_the_original_path(self):
        with patch.object(voxel.hashlib, "sha256") as digest, patch.object(
                voxel, "verify_native", side_effect=ValueError("different matrix provider")):
            digest.return_value.hexdigest.return_value = voxel.RAW_SHA256
            self.assertIsNone(voxel.optional_voxel_seed("frozen export", None, Path(".")))

    def test_verified_seed_uses_typed_surface_and_matrix_records(self):
        with patch.object(voxel.hashlib, "sha256") as digest, patch.object(voxel, "verify_native"):
            digest.return_value.hexdigest.return_value = voxel.RAW_SHA256
            source = voxel.optional_voxel_seed("frozen export", None, Path("."))
        self.assertIn("BSurface *voxel_surface=unknown_surface_ad0604;", source)
        self.assertIn("cliprect=voxel_surface->Get_Rect();", source)
        self.assertIn("Matrix3D draw_result;", source)
        self.assertIn("Draw_Matrix(&draw_result,&key)", source)
