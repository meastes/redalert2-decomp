/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/

#include "always.h"
#include "geometry.h"
#include "rect.h"

#include "_voxel.h"
#include "globals.h"
#include "inline.h"
#include "matrix3d.h"
#include "objtype.h"
#include "quat.h"

#include "ramp.hh"

/// warning C4305: 'argument' : truncation from 'const double' to 'float'
#pragma warning(disable : 4305)

/*
 * These are the orientations a voxel takes on when it stands on each kind of ramp -- the
 * matrix for an object settled on one ramp, the quaternion for one still tilting onto the
 * next. The Screen pair holds the same slopes about axes turned a further 45 degrees,
 * apparently the angle between the map grid and the screen. Nothing reads it.
 */
Matrix3D ScreenSlopeMatrices[RAMP_COUNT];
Matrix3D SlopeMatrices[RAMP_COUNT];

Quaternion ScreenSlopeQuaternions[RAMP_COUNT];
Quaternion SlopeQuaternions[RAMP_COUNT];

Matrix3D IsometricViewMatrix;
Matrix3D BounceMatrices[4];

Vector3 LightTiltVectors[256];
Vector3 LightDirectionVectors[256];
float ExtraLightTiltAngles[6] = { 0.0, 0.08726389, 0.17452778, 0.26179168, 0.17452778, 0.08726389 }; /// 0, 5, 10, 15, 10, 5

int SomeNumbers[17] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 5, 6, 7, 8, 9, 10, 11, 12 };


/// <summary>
/// Fetches the matrix that orients a voxel to the view.
/// This routine hands out the fixed isometric camera transform that the voxel drawing
/// code combines with each object's own orientation.
/// </summary>
/// <returns>Returns with the isometric view matrix.</returns>
Matrix3D Get_Isometric_View_Matrix(void)
{
	return(IsometricViewMatrix);
}


/// <summary>
/// Sets the angle that voxel objects are lit from.
/// This routine turns the light source about the vertical axis and derives the light and
/// shadow vectors from it.
/// </summary>
/// <param name="angle">The angle of the light source.</param>
void Set_Voxel_Light_Angle(float angle)
{
	Matrix3D mtx(true);
	VoxelLightAngle = angle;
	mtx.Rotate_Y(angle);
	VoxelLightSource = mtx * Vector3(-0.70710599f, -0.70710599f, 0); // RA2
	VoxelShadowLightVector = Vector3(-6 * VoxelLightSource.X, 0, 0);
}


/// <summary>
/// Builds the fixed matrix tables that the voxel renderer works from.
/// This routine prepares the orientation of every ramp type, the lighting direction and
/// tilt vector of every facing, the isometric view transform, and the bounce matrices.
/// </summary>
/// <remarks>Call this routine before any voxel is drawn.</remarks>
#if 0
void __fastcall Init_Voxel_Matrices()
{
	float slope_h = (float)CELL_SLOPE_ANGLE;
	float slope_i = (float)CELL_DIAG_SLOPE_ANGLE;
	Vector3 diagonal1 = Matrix3D(Vector3(0.0f, 0.0f, 1.0f), 0.785375f) * Vector3(1.0f, 0.0f, 0.0f);
	Vector3 diagonal3 = Matrix3D(Vector3(0.0f, 0.0f, 1.0f), 2.3561251f) * Vector3(1.0f, 0.0f, 0.0f);
	Vector3 diagonal5 = Matrix3D(Vector3(0.0f, 0.0f, 1.0f), 3.9268751f) * Vector3(1.0f, 0.0f, 0.0f);
	Vector3 diagonal7 = Matrix3D(Vector3(0.0f, 0.0f, 1.0f), 5.4976249f) * Vector3(1.0f, 0.0f, 0.0f);
	Vector3 east(1.0f, 0.0f, 0.0f);
	Vector3 north(0.0f, 1.0f, 0.0f);
	Vector3 west(-1.0f, 0.0f, 0.0f);
	Vector3 south(0.0f, -1.0f, 0.0f);

	ScreenSlopeMatrices[0].Make_Identity();
	ScreenSlopeMatrices[1] = Matrix3D(3.9268751f, slope_i);
	ScreenSlopeMatrices[2] = Matrix3D(2.3561251f, slope_i);
	ScreenSlopeMatrices[3] = Matrix3D(0.785375f, slope_i);
	ScreenSlopeMatrices[4] = Matrix3D(5.4976249f, slope_i);
	ScreenSlopeQuaternions[1] = Axis_To_Quat(diagonal5, slope_i);
	ScreenSlopeQuaternions[2] = Axis_To_Quat(diagonal3, slope_i);
	ScreenSlopeQuaternions[3] = Axis_To_Quat(diagonal1, slope_i);
	ScreenSlopeQuaternions[4] = Axis_To_Quat(diagonal7, slope_i);
	ScreenSlopeMatrices[5] = Matrix3D(3.1415f, slope_h);
	ScreenSlopeMatrices[6] = Matrix3D(1.57075f, slope_h);
	ScreenSlopeMatrices[7] = Matrix3D(0.0f, slope_h);
	ScreenSlopeMatrices[8] = Matrix3D(4.7122502f, slope_h);
	ScreenSlopeQuaternions[5] = Axis_To_Quat(west, slope_h);
	ScreenSlopeQuaternions[6] = Axis_To_Quat(north, slope_h);
	ScreenSlopeQuaternions[7] = Axis_To_Quat(east, slope_h);
	ScreenSlopeQuaternions[8] = Axis_To_Quat(south, slope_h);
	ScreenSlopeMatrices[9] = Matrix3D(3.1415f, slope_h);
	ScreenSlopeMatrices[10] = Matrix3D(1.57075f, slope_h);
	ScreenSlopeMatrices[11] = Matrix3D(0.0f, slope_h);
	ScreenSlopeMatrices[12] = Matrix3D(4.7122502f, slope_h);
	ScreenSlopeQuaternions[9] = Axis_To_Quat(west, slope_h);
	ScreenSlopeQuaternions[10] = Axis_To_Quat(north, slope_h);
	ScreenSlopeQuaternions[11] = Axis_To_Quat(east, slope_h);
	ScreenSlopeQuaternions[12] = Axis_To_Quat(south, slope_h);
	ScreenSlopeMatrices[13] = Matrix3D(3.1415f, slope_i);
	ScreenSlopeMatrices[14] = Matrix3D(1.57075f, slope_i);
	ScreenSlopeMatrices[15] = Matrix3D(0.0f, slope_i);
	ScreenSlopeMatrices[16] = Matrix3D(4.7122502f, slope_i);
	ScreenSlopeQuaternions[13] = Axis_To_Quat(west, slope_i);
	ScreenSlopeQuaternions[14] = Axis_To_Quat(north, slope_i);
	ScreenSlopeQuaternions[15] = Axis_To_Quat(east, slope_i);
	ScreenSlopeQuaternions[16] = Axis_To_Quat(south, slope_i);
	ScreenSlopeMatrices[17].Make_Identity();
	ScreenSlopeMatrices[18].Make_Identity();
	ScreenSlopeMatrices[19].Make_Identity();
	ScreenSlopeMatrices[20].Make_Identity();
	ScreenSlopeQuaternions[17].Set(0.0f, 0.0f, 0.0f, 1.0f);
	ScreenSlopeQuaternions[18].Set(0.0f, 0.0f, 0.0f, 1.0f);
	ScreenSlopeQuaternions[19].Set(0.0f, 0.0f, 0.0f, 1.0f);
	ScreenSlopeQuaternions[20].Set(0.0f, 0.0f, 0.0f, 1.0f);

	for (int facing = 0; facing < 256; facing++) {
		Matrix3D matrix;
		matrix.Make_Identity();
		matrix.Rotate_Z(facing * 0.024542969f);
		LightDirectionVectors[facing] = matrix * Vector3(1.0f, 0.0f, 0.0f);
		LightTiltVectors[facing] = Vector3::Cross_Product(LightDirectionVectors[facing], Vector3(0.0f, 0.0f, 1.0f));
		Matrix3D rotation(LightTiltVectors[facing], VoxelLightAngle);
		LightDirectionVectors[facing] = rotation * LightDirectionVectors[facing];
	}

	SlopeMatrices[0].Make_Identity();
	SlopeMatrices[1] = Matrix3D(4.7122502f, slope_i);
	SlopeMatrices[2] = Matrix3D(3.1415f, slope_i);
	SlopeMatrices[3] = Matrix3D(1.57075f, slope_i);
	SlopeMatrices[4] = Matrix3D(0.0f, slope_i);
	SlopeMatrices[5] = Matrix3D(3.9268751f, slope_h);
	SlopeMatrices[6] = Matrix3D(2.3561251f, slope_h);
	SlopeMatrices[7] = Matrix3D(0.785375f, slope_h);
	SlopeMatrices[8] = Matrix3D(5.4976249f, slope_h);
	SlopeMatrices[9] = Matrix3D(3.9268751f, slope_h);
	SlopeMatrices[10] = Matrix3D(2.3561251f, slope_h);
	SlopeMatrices[11] = Matrix3D(0.785375f, slope_h);
	SlopeMatrices[12] = Matrix3D(5.4976249f, slope_h);
	SlopeMatrices[13] = Matrix3D(3.9268751f, slope_i);
	SlopeMatrices[14] = Matrix3D(2.3561251f, slope_i);
	SlopeMatrices[15] = Matrix3D(0.785375f, slope_i);
	SlopeMatrices[16] = Matrix3D(5.4976249f, slope_i);
	SlopeQuaternions[0].Set(0.0f, 0.0f, 0.0f, 1.0f);
	SlopeQuaternions[1] = Axis_To_Quat(south, slope_i);
	SlopeQuaternions[2] = Axis_To_Quat(west, slope_i);
	SlopeQuaternions[3] = Axis_To_Quat(north, slope_i);
	SlopeQuaternions[4] = Axis_To_Quat(east, slope_i);
	SlopeQuaternions[5] = Axis_To_Quat(diagonal5, slope_h);
	SlopeQuaternions[6] = Axis_To_Quat(diagonal3, slope_h);
	SlopeQuaternions[7] = Axis_To_Quat(diagonal1, slope_h);
	SlopeQuaternions[8] = Axis_To_Quat(diagonal7, slope_h);
	SlopeQuaternions[9] = Axis_To_Quat(diagonal5, slope_h);
	SlopeQuaternions[10] = Axis_To_Quat(diagonal3, slope_h);
	SlopeQuaternions[11] = Axis_To_Quat(diagonal1, slope_h);
	SlopeQuaternions[12] = Axis_To_Quat(diagonal7, slope_h);
	SlopeQuaternions[13] = Axis_To_Quat(diagonal5, slope_i);
	SlopeQuaternions[14] = Axis_To_Quat(diagonal3, slope_i);
	SlopeQuaternions[15] = Axis_To_Quat(diagonal1, slope_i);
	SlopeQuaternions[16] = Axis_To_Quat(diagonal7, slope_i);
	SlopeMatrices[17].Make_Identity();
	SlopeMatrices[18].Make_Identity();
	SlopeMatrices[19].Make_Identity();
	SlopeMatrices[20].Make_Identity();
	SlopeQuaternions[17].Set(0.0f, 0.0f, 0.0f, 1.0f);
	SlopeQuaternions[18].Set(0.0f, 0.0f, 0.0f, 1.0f);
	SlopeQuaternions[19].Set(0.0f, 0.0f, 0.0f, 1.0f);
	SlopeQuaternions[20].Set(0.0f, 0.0f, 0.0f, 1.0f);

	IsometricViewMatrix.Make_Identity();
	IsometricViewMatrix.Rotate_X((float)-RAD_60);
	IsometricViewMatrix.Rotate_Z((float)-RAD_45);

	for (int index = 0; index < 4; index++) {
		BounceMatrices[index].Make_Identity();
	}
	BounceMatrices[0][1].Y = -1.0f;
	BounceMatrices[1][0].X = 0.0f;
	BounceMatrices[1][0].Y = 1.0f;
	BounceMatrices[1][1].X = 1.0f;
	BounceMatrices[1][1].Y = 0.0f;
	BounceMatrices[2][0].X = -1.0f;
	BounceMatrices[3][0].X = 0.0f;
	BounceMatrices[3][0].Y = -1.0f;
	BounceMatrices[3][1].X = -1.0f;
	BounceMatrices[3][1].Y = 0.0f;
}
#endif



/// <summary>
/// Fetches the screen-aligned orientation matrix of the ramp specified.
/// </summary>
/// <param name="matrix">The matrix to fill in with the ramp orientation.</param>
void Get_Screen_Slope_Matrix(int ramp, Matrix3D & matrix)
{
	matrix = ScreenSlopeMatrices[ramp];
}


/// <summary>
/// Fetches the orientation matrix of the ramp specified.
/// </summary>
/// <returns>Returns with the orientation to draw an object resting on that ramp.</returns>
Matrix3D Get_Slope_Matrix(int ramp)
{
	return(SlopeMatrices[ramp]);
}


/// <summary>
/// Fetches a screen-aligned orientation part way between two ramps.
/// This routine is used to tilt an object smoothly as it travels from one ramp onto
/// another. When both ramps are alike there is nothing to blend and the ramp's own
/// orientation is handed back.
/// </summary>
/// <param name="oldramp">The ramp being left behind.</param>
/// <param name="newramp">The ramp being moved onto.</param>
/// <param name="matrix">The matrix to fill in with the blended orientation.</param>
/// <param name="time">How far the transition has progressed, from zero to one.</param>
void Get_Screen_Slope_Transition_Matrix(int oldramp, int newramp, Matrix3D & matrix, double time)
{
	if (oldramp == newramp) {
		Get_Screen_Slope_Matrix(newramp, matrix);
	} else {
		matrix = Build_Matrix3D(Slerp(ScreenSlopeQuaternions[oldramp], ScreenSlopeQuaternions[newramp], time));
	}
}


/// <summary>
/// Fetches an orientation part way between two ramps.
/// This routine is used by the drive locomotor to tilt a unit smoothly as it crosses
/// from one ramp onto another.
/// </summary>
/// <param name="oldramp">The ramp being left behind.</param>
/// <param name="newramp">The ramp being moved onto.</param>
/// <param name="time">How far the transition has progressed, from zero to one.</param>
/// <returns>Returns with the orientation to draw the object at.</returns>
Matrix3D Get_Slope_Transition_Matrix(int oldramp, int newramp, double time)
{
	if (oldramp != newramp) {
		return(Build_Matrix3D(Slerp(SlopeQuaternions[oldramp], SlopeQuaternions[newramp], time)));
	} else {
		return(SlopeMatrices[newramp]);
	}
}


/// <summary>
/// Fetches the voxel number that the index specified maps to.
/// </summary>
/// <returns>Returns with the voxel number the index maps to.</returns>
int Get_Some_Voxel_Number(int index)
{
	return(SomeNumbers[index]);
}


/// <summary>
/// Determines the voxel facing between two coordinates.
/// This routine converts the direction from one coordinate to the other into the facing
/// that the voxel artwork and the lighting tables are indexed by.
/// </summary>
/// <param name="coord1">The coordinate to measure the direction from.</param>
/// <param name="coord2">The coordinate to measure the direction to.</param>
/// <returns>Returns with the facing, expressed in 256ths of a circle.</returns>
unsigned char _Direction256(Coord const & coord1, Coord const & coord2)
{
	DirType dir = Direction(coord1, coord2);
	unsigned char facing = DIR_NE - dir.As_Dir256();
	return(facing);
}


/// <summary>
/// Tilts and offsets a matrix towards the lighting specified.
/// </summary>
/// <param name="matrix">The matrix to transform in place.</param>
/// <param name="axis">The lighting facing to tilt about.</param>
/// <param name="angle">Index of the extra tilt angle to apply.</param>
void Apply_Light_Transform(Matrix3D & matrix, int axis, int angle)
{
	Matrix3D mtx(LightTiltVectors[axis], ExtraLightTiltAngles[angle]);
	matrix = matrix * mtx;
	matrix.Translate(-3 * LightDirectionVectors[axis]);
	matrix.Translate(0, 0, 3);
}


/// <summary>
/// Fetches the lighting vector for the direction specified.
/// </summary>
/// <returns>Returns with the scaled light direction vector.</returns>
Vector3 Get_Light_Vector(Dir256 dir, int)
{
	Vector3& vec = LightDirectionVectors[dir];
	return(Vector3(vec.X * 1.5f, vec.Y * 1.5f, vec.Z * 1.5f));
}


/// <summary>
/// Clears the cached voxel indexes of every object type.
/// </summary>
void Clear_Voxel_Indexes(void)
{
	ObjectTypeClass::Clear_Voxel_Indexes();
}


/// <summary>
/// Fetches the mirror matrix for the facing specified.
/// This routine is used by the bounce logic to flip an object's artwork about, so that
/// opposing facings can share the same prepared matrix.
/// </summary>
/// <param name="facing">The facing the object is bouncing towards.</param>
/// <returns>Returns with the matrix to mirror the object's orientation by.</returns>
Matrix3D Get_Bounce_Matrix(FacingType facing)
{
	if (facing >= FACING_S) {
		facing = FacingType(facing - FACING_S);
	}
	return(BounceMatrices[facing]);
}
