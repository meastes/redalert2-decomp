
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y3), appended to the voxel unit: FUN_00754CB0, YR 0x00754CB0
// (3,290 bytes, unmatched in ra2-decomp; TibSun Init_Voxel_Matrices). From the RA2 tree's twin
// Unknown_00718060 (src/recovered/voxel.cpp, 0x00718060), whose body gamemd.exe repeats call for
// call with the same constants; the RA2 tables map to this unit's by the addresses gamemd.exe
// writes: ScreenSlopeMatrices/Quaternions (RA2 0x00AF73A0/0x00AF77D8), SlopeMatrices/Quaternions
// (0x00AF8608/0x00AF6618), LightDirectionVectors/LightTiltVectors (0x00AF6768/0x00AF7930),
// IsometricViewMatrix, BounceMatrices. The slopes are this unit's CELL_SLOPE_ANGLE (0x00B43F08)
// and CELL_DIAG_SLOPE_ANGLE (0x00B44310), the view angles RAD_60 and RAD_45.
// Loop counters are distinct: the tree builds with VC6 for-scope.
// Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------
// BounceMatrices' entries (Matrix3D::Row is protected; the original writes the fields).
struct YRBounceMatrix : Matrix3D {
	static void Set_Entry(Matrix3D &matrix, int row, int column, float value) { ((YRBounceMatrix &)matrix).Row[row][column] = value; }
};

void __fastcall FUN_00754CB0(void)
{
	float slope = (float)CELL_SLOPE_ANGLE;
	float diag_slope = (float)CELL_DIAG_SLOPE_ANGLE;
	Vector3 diagonal1 = Matrix3D(Vector3(0.0f, 0.0f, 1.0f), 0.785375f) * Vector3(1.0f, 0.0f, 0.0f);
	Vector3 diagonal3 = Matrix3D(Vector3(0.0f, 0.0f, 1.0f), 2.3561251f) * Vector3(1.0f, 0.0f, 0.0f);
	Vector3 diagonal5 = Matrix3D(Vector3(0.0f, 0.0f, 1.0f), 3.9268751f) * Vector3(1.0f, 0.0f, 0.0f);
	Vector3 diagonal7 = Matrix3D(Vector3(0.0f, 0.0f, 1.0f), 5.4976249f) * Vector3(1.0f, 0.0f, 0.0f);
	Vector3 east(1.0f, 0.0f, 0.0f);
	Vector3 north(0.0f, 1.0f, 0.0f);
	Vector3 west(-1.0f, 0.0f, 0.0f);
	Vector3 south(0.0f, -1.0f, 0.0f);

	static const float straight_z[4] = { 3.1415f, 1.57075f, 0.0f, 4.7122502f };
	static const float diagonal_z[4] = { 3.9268751f, 2.3561251f, 0.785375f, 5.4976249f };
	Vector3 const *straight_axis[4] = { &west, &north, &east, &south };
	Vector3 const *diagonal_axis[4] = { &diagonal5, &diagonal3, &diagonal1, &diagonal7 };

	ScreenSlopeMatrices[0].Make_Identity();
	for (int i1 = 0; i1 < 4; i1++) {
		ScreenSlopeMatrices[1 + i1] = Matrix3D(diagonal_z[i1], diag_slope);
	}
	for (int i2 = 0; i2 < 4; i2++) {
		ScreenSlopeQuaternions[1 + i2] = Axis_To_Quat(*diagonal_axis[i2], diag_slope);
	}
	for (int g3 = 0; g3 < 3; g3++) {
		float angle = (g3 == 2) ? diag_slope : slope;
		for (int i3 = 0; i3 < 4; i3++) {
			ScreenSlopeMatrices[5 + g3 * 4 + i3] = Matrix3D(straight_z[i3], angle);
		}
		for (int j3 = 0; j3 < 4; j3++) {
			ScreenSlopeQuaternions[5 + g3 * 4 + j3] = Axis_To_Quat(*straight_axis[j3], angle);
		}
	}
	for (int i4 = 17; i4 < 21; i4++) {
		ScreenSlopeMatrices[i4].Make_Identity();
	}
	for (int i5 = 17; i5 < 21; i5++) {
		ScreenSlopeQuaternions[i5].Set(0.0f, 0.0f, 0.0f, 1.0f);
	}

	for (int facing = 0; facing < 256; facing++) {
		Matrix3D matrix;
		matrix.Make_Identity();
		matrix.Rotate_Z(facing * 0.024542969f);
		LightDirectionVectors[facing] = matrix * Vector3(1.0f, 0.0f, 0.0f);
		LightTiltVectors[facing] = Vector3::Cross_Product(LightDirectionVectors[facing], Vector3(0.0f, 0.0f, 1.0f));
		Matrix3D rotation(LightTiltVectors[facing], VoxelLightAngle);
		LightDirectionVectors[facing] = rotation * LightDirectionVectors[facing];
	}

	static const float ramp_z[4] = { 4.7122502f, 3.1415f, 1.57075f, 0.0f };
	Vector3 const *ramp_axis[4] = { &south, &west, &north, &east };
	SlopeMatrices[0].Make_Identity();
	for (int i7 = 0; i7 < 4; i7++) {
		SlopeMatrices[1 + i7] = Matrix3D(ramp_z[i7], diag_slope);
	}
	for (int g8 = 0; g8 < 3; g8++) {
		float angle = (g8 == 2) ? diag_slope : slope;
		for (int i8 = 0; i8 < 4; i8++) {
			SlopeMatrices[5 + g8 * 4 + i8] = Matrix3D(diagonal_z[i8], angle);
		}
	}
	SlopeQuaternions[0].Set(0.0f, 0.0f, 0.0f, 1.0f);
	for (int i9 = 0; i9 < 4; i9++) {
		SlopeQuaternions[1 + i9] = Axis_To_Quat(*ramp_axis[i9], diag_slope);
	}
	for (int g10 = 0; g10 < 3; g10++) {
		float angle = (g10 == 2) ? diag_slope : slope;
		for (int i10 = 0; i10 < 4; i10++) {
			SlopeQuaternions[5 + g10 * 4 + i10] = Axis_To_Quat(*diagonal_axis[i10], angle);
		}
	}
	for (int i11 = 17; i11 < 21; i11++) {
		SlopeMatrices[i11].Make_Identity();
	}
	for (int i12 = 17; i12 < 21; i12++) {
		SlopeQuaternions[i12].Set(0.0f, 0.0f, 0.0f, 1.0f);
	}

	IsometricViewMatrix.Make_Identity();
	IsometricViewMatrix.Rotate_X((float)-RAD_60);
	IsometricViewMatrix.Rotate_Z((float)-RAD_45);

	for (int index = 0; index < 4; index++) {
		BounceMatrices[index].Make_Identity();
	}
	YRBounceMatrix::Set_Entry(BounceMatrices[0], 1, 1, -1.0f);
	YRBounceMatrix::Set_Entry(BounceMatrices[1], 0, 0, 0.0f);
	YRBounceMatrix::Set_Entry(BounceMatrices[1], 0, 1, 1.0f);
	YRBounceMatrix::Set_Entry(BounceMatrices[1], 1, 0, 1.0f);
	YRBounceMatrix::Set_Entry(BounceMatrices[1], 1, 1, 0.0f);
	YRBounceMatrix::Set_Entry(BounceMatrices[2], 0, 0, -1.0f);
	YRBounceMatrix::Set_Entry(BounceMatrices[3], 0, 0, 0.0f);
	YRBounceMatrix::Set_Entry(BounceMatrices[3], 0, 1, -1.0f);
	YRBounceMatrix::Set_Entry(BounceMatrices[3], 1, 0, -1.0f);
	YRBounceMatrix::Set_Entry(BounceMatrices[3], 1, 1, 0.0f);
}
