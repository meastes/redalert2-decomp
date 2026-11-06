/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/

// Adapted from TibSun matrix3d.cpp/matrix3d.h axis-angle formulas.
#include "shared/matrix3d.hpp"
Matrix3D::Matrix3D(Vector3 const &axis, float angle)
{
 float c=Fast_Cos(angle);
 float s=Fast_Sin(angle);

	Row[0][0] = axis.X*axis.X + c*(1 - axis.X*axis.X);
	Row[0][1] = axis.X*axis.Y*(1 - c) - axis.Z*s;
	Row[0][2] = axis.Z*axis.X*(1 - c) + axis.Y*s;
	Row[0][3] = 0.0;

	Row[1][0] = axis.X*axis.Y*(1 - c) + axis.Z*s;
	Row[1][1] = axis.Y*axis.Y + c*(1 - axis.Y*axis.Y);
	Row[1][2] = axis.Y*axis.Z*(1 - c) - axis.X*s;
	Row[1][3] = 0.0;

	Row[2][0] = axis.Z*axis.X*(1 - c) - axis.Y*s;
	Row[2][1] = axis.Y*axis.Z*(1 - c) + axis.X*s;
	Row[2][2] = axis.Z*axis.Z + c*(1 - axis.Z*axis.Z);
	Row[2][3] = 0.0;
}
