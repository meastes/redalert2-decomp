
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFTS (lane Y3): functions unmatched in the YR tree, the text TibSun's
// (TibSun archive code/drive.cpp; SPDX GPL-3.0-or-later, Copyright 2025 Electronic Arts Inc. and
// OpenTS contributors, EA's GPL section 7 terms; THIRD_PARTY.md); native changes are marked
// NATIVE. Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------

Matrix3DStruct STDMETHODCALLTYPE DriveLocomotionClass::Draw_Matrix(int *key)
{
	Matrix3DStruct m;

	if (!Is_Angled()) {
		Matrix3D m1, m2;
		m1.Make_Identity();
		m2.Make_Identity();

		float val = LinkedTo->TClass->VoxelCenterY;
		float val2 = LinkedTo->TClass->VoxelCenterX;

		float fcos = fastmath::cos(LinkedTo->AngleRotatedForwards);
		float fsin = fastmath::sin(LinkedTo->AngleRotatedForwards);

		float scos = fastmath::cos(LinkedTo->AngleRotatedSideways);
		float ssin = fastmath::sin(LinkedTo->AngleRotatedSideways);

		int num = fabs(ssin) * val + fabs(fsin) * val2;

		int x_shift = (int)(val2 - (float)(int)(fcos * val2));
		int x_offset = x_shift;
		int y_shift = (int)(val - (float)(int)(scos * val));
		int y_offset = y_shift;
		if (LinkedTo->AngleRotatedForwards < 0.0f) {
			x_offset = -x_shift;
		}
		if (LinkedTo->AngleRotatedSideways > 0.0f) {
			y_offset = -y_shift;
		}

		m1.Translate_Z(num);
		m2.Translate_X(x_offset);
		m2.Translate_Y(y_offset);
		m2.Rotate_X(LinkedTo->AngleRotatedSideways);
		m2.Rotate_Y(LinkedTo->AngleRotatedForwards);

		if (key) {
			*key = -1;
		}

		Matrix3D basemtx(BASECLASS::Draw_Matrix(key));
		Matrix3D slopemtx = Get_Slope_Matrix();
		m = (Matrix3DStruct &)(((m1 * slopemtx) * basemtx) * m2);
		return(m);
	}

	if (key && *key != -1) {
		int keybase = *key << 6;
		*key = keybase;
		*key = keybase + CurrentRamp;
	}

	Matrix3D basemtx(BASECLASS::Draw_Matrix(key));
	Matrix3D slopemtx = Get_Slope_Matrix();
	m = (Matrix3DStruct &)(slopemtx * basemtx);
	return(m);
}
