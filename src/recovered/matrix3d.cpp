// matrix3d.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/matrix3d.json carries the addresses and the rows that
// stay in their own files.


#include "shared/matrix3d.hpp"


// ??0Matrix3D@@QAE@PBM@Z (0x0058FDE0)
// Matrix3D(float const *matrix): copies twelve floats, row by row.

Matrix3D::Matrix3D(float const *matrix)
{
    float *dest = &Row[0][0];
    for (int index = 0; index < 12; index++) {
        dest[index] = matrix[index];
    }
}


// ??0Matrix3D@@QAE@ABV0@@Z (0x0058FE10)
// Matrix3D(Matrix3D const &matrix): member-wise copy.

struct MatrixData { float Data[12]; };

Matrix3D::Matrix3D(Matrix3D const &matrix)
{
    *(MatrixData *)Row = *(MatrixData const *)matrix.Row;
}


// ??0Matrix3D@@QAE@MMMMMMMMMMMM@Z (0x0058FE30)
// Matrix3D from twelve floats, row major.

Matrix3D::Matrix3D(float m11, float m12, float m13, float m14,
                   float m21, float m22, float m23, float m24,
                   float m31, float m32, float m33, float m34)
{
    Row[0][0] = m11; Row[0][1] = m12; Row[0][2] = m13; Row[0][3] = m14;
    Row[1][0] = m21; Row[1][1] = m22; Row[1][2] = m23; Row[1][3] = m24;
    Row[2][0] = m31; Row[2][1] = m32; Row[2][2] = m33; Row[2][3] = m34;
}


// ??0Matrix3D@@QAE@ABUVector3@@000@Z (0x0058FE90)
// Matrix3D from three row vectors and a translation.

Matrix3D::Matrix3D(Vector3 const &x, Vector3 const &y, Vector3 const &z, Vector3 const &position)
{
    Row[0][0] = x.X; Row[0][1] = x.Y; Row[0][2] = x.Z; Row[0][3] = position.X;
    Row[1][0] = y.X; Row[1][1] = y.Y; Row[1][2] = y.Z; Row[1][3] = position.Y;
    Row[2][0] = z.X; Row[2][1] = z.Y; Row[2][2] = z.Z; Row[2][3] = position.Z;
}


// Matrix3D::Matrix3D(float unknown1, float unknown2) (0x0058FEF0)
// Identity rotated about Z by unknown1, about X by unknown2 and back about Z
// by -unknown1 (a tilt of unknown2 towards direction unknown1; meaning of the
// parameters is a hypothesis).

Matrix3D::Matrix3D(float unknown1, float unknown2)
{
    Make_Identity();
    Rotate_Z(unknown1);
    Rotate_X(unknown2);
    Rotate_Z(-unknown1);
}


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


// ?Make_Identity@Matrix3D@@QAEXXZ (0x00590060)
// Sets the identity transform.

void Matrix3D::Make_Identity()
{
    Row[0][0] = 1.0f; Row[0][1] = 0.0f; Row[0][2] = 0.0f; Row[0][3] = 0.0f;
    Row[1][0] = 0.0f; Row[1][1] = 1.0f; Row[1][2] = 0.0f; Row[1][3] = 0.0f;
    Row[2][0] = 0.0f; Row[2][1] = 0.0f; Row[2][2] = 1.0f; Row[2][3] = 0.0f;
}


// ?Translate@Matrix3D@@QAEXMMM@Z (0x00590090)
// Moves the origin by (x, y, z) in local coordinates.

void Matrix3D::Translate(float x, float y, float z)
{
    Row[0][3] += Row[0][0] * x + Row[0][1] * y + Row[0][2] * z;
    Row[1][3] += Row[1][0] * x + Row[1][1] * y + Row[1][2] * z;
    Row[2][3] += Row[2][0] * x + Row[2][1] * y + Row[2][2] * z;
}


// ?Translate@Matrix3D@@QAEXABUVector3@@@Z (0x005900F0)
// Moves the origin by offset in local coordinates: Translate_X, _Y and _Z (defined later, expanded inline).

void Matrix3D::Translate(Vector3 const &offset)
{
    Translate_X(offset.X);
    Translate_Y(offset.Y);
    Translate_Z(offset.Z);
}


// ?Translate_X@Matrix3D@@QAEXM@Z (0x00590180)
// Moves the origin along the local X axis.

void Matrix3D::Translate_X(float x)
{
    Row[0][3] += Row[0][0] * x;
    Row[1][3] += Row[1][0] * x;
    Row[2][3] += Row[2][0] * x;
}


// ?Translate_Y@Matrix3D@@QAEXM@Z (0x005901B0)
// Moves the origin along the local Y axis.

void Matrix3D::Translate_Y(float y)
{
    Row[0][3] += Row[0][1] * y;
    Row[1][3] += Row[1][1] * y;
    Row[2][3] += Row[2][1] * y;
}


// ?Translate_Z@Matrix3D@@QAEXM@Z (0x005901E0)
// Moves the origin along the local Z axis.

void Matrix3D::Translate_Z(float z)
{
    Row[0][3] += Row[0][2] * z;
    Row[1][3] += Row[1][2] * z;
    Row[2][3] += Row[2][2] * z;
}


// ?Scale@Matrix3D@@QAEXM@Z (0x00590210)
// Scales the three axes by scale.

void Matrix3D::Scale(float scale)
{
    Row[0][0] *= scale; Row[1][0] *= scale; Row[2][0] *= scale;
    Row[0][1] *= scale; Row[1][1] *= scale; Row[2][1] *= scale;
    Row[0][2] *= scale; Row[1][2] *= scale; Row[2][2] *= scale;
}


// ?Scale@Matrix3D@@QAEXMMM@Z (0x00590270)
// Scales the axes by x, y and z.

void Matrix3D::Scale(float x, float y, float z)
{
    Row[0][0] *= x; Row[1][0] *= x; Row[2][0] *= x;
    Row[0][1] *= y; Row[1][1] *= y; Row[2][1] *= y;
    Row[0][2] *= z; Row[1][2] *= z; Row[2][2] *= z;
}


// ?Scale_X@Matrix3D@@QAEXM@Z (0x005902D0)
// Scales the local X axis.

void Matrix3D::Scale_X(float x)
{
    Row[0][0] *= x; Row[1][0] *= x; Row[2][0] *= x;
}


// ?Scale_Y@Matrix3D@@QAEXM@Z (0x005902F0)
// Scales the local Y axis.

void Matrix3D::Scale_Y(float y)
{
    Row[0][1] *= y; Row[1][1] *= y; Row[2][1] *= y;
}


// ?Scale_Z@Matrix3D@@QAEXM@Z (0x00590320)
// Scales the local Z axis.

void Matrix3D::Scale_Z(float z)
{
    Row[0][2] *= z; Row[1][2] *= z; Row[2][2] *= z;
}


// ?Shear_X@Matrix3D@@QAEXMM@Z (0x00590350)
// Adds y times the Y axis and z times the Z axis to the X axis (name hypothesis).

void Matrix3D::Shear_X(float y, float z)
{
    Row[0][0] += Row[0][1] * y + Row[0][2] * z;
    Row[1][0] += Row[1][1] * y + Row[1][2] * z;
    Row[2][0] += Row[2][1] * y + Row[2][2] * z;
}


// ?Shear_Z@Matrix3D@@QAEXMM@Z (0x005903A0)
// Adds x times the X axis and y times the Y axis to the Z axis (name hypothesis).

void Matrix3D::Shear_Z(float x, float y)
{
    Row[0][2] += Row[0][0] * x + Row[0][1] * y;
    Row[1][2] += Row[1][0] * x + Row[1][1] * y;
    Row[2][2] += Row[2][0] * x + Row[2][1] * y;
}


// ?Shear_Y@Matrix3D@@QAEXMM@Z (0x005903F0)
// Adds x times the X axis and z times the Z axis to the Y axis (name hypothesis).

void Matrix3D::Shear_Y(float x, float z)
{
    Row[0][1] += Row[0][0] * x + Row[0][2] * z;
    Row[1][1] += Row[1][0] * x + Row[1][2] * z;
    Row[2][1] += Row[2][0] * x + Row[2][2] * z;
}


// ?Pre_Rotate_X@Matrix3D@@QAEXM@Z (0x00590440)
// Rotates about the world X axis by theta (rows 1 and 2 mixed).

void Matrix3D::Pre_Rotate_X(float theta)
{
    float s = Fast_Sin(theta);
    float c = Fast_Cos(theta);
    float ns = -s;
    float a0 = Row[1][0];
    float a1 = Row[1][1];
    float a2 = Row[1][2];
    float a3 = Row[1][3];
    float b0 = Row[2][0];
    float b1 = Row[2][1];
    float b2 = Row[2][2];
    float b3 = Row[2][3];
    Row[1][0] = c * a0 + ns * b0;
    Row[1][1] = c * a1 + ns * b1;
    Row[1][2] = c * a2 + ns * b2;
    Row[1][3] = c * a3 + ns * b3;
    Row[2][0] = s * a0 + c * b0;
    Row[2][1] = s * a1 + c * b1;
    Row[2][2] = s * a2 + c * b2;
    Row[2][3] = s * a3 + c * b3;
}


// Matrix3D pre-multiplication by a Y rotation; original00590550.
void Matrix3D::Pre_Rotate_Y(float angle)
{
    float sine = Fast_Sin(angle);
    float cosine = Fast_Cos(angle);
    float const &negative_sine = -sine;
    float first_x = Row[0][0];
    float first_y = Row[0][1];
    float first_z = Row[0][2];
    float first_translation = Row[0][3];
    float third_x = Row[2][0];
    float third_y = Row[2][1];
    float third_z = Row[2][2];
    float third_translation = Row[2][3];
    Row[0][0] = third_x * sine + first_x * cosine;
    Row[0][1] = third_y * sine + first_y * cosine;
    Row[0][2] = third_z * sine + first_z * cosine;
    Row[0][3] = third_translation * sine + first_translation * cosine;
    Row[2][0] = first_x * negative_sine + third_x * cosine;
    Row[2][1] = first_y * negative_sine + third_y * cosine;
    Row[2][2] = first_z * negative_sine + third_z * cosine;
    Row[2][3] = first_translation * negative_sine + third_translation * cosine;
}


// ?Pre_Rotate_Z@Matrix3D@@QAEXM@Z (0x00590650)
// Rotates about the world Z axis by theta (rows 0 and 1 mixed).

void Matrix3D::Pre_Rotate_Z(float theta)
{
    float s = Fast_Sin(theta);
    float c = Fast_Cos(theta);
    float ns = -s;
    float a0 = Row[0][0];
    float a1 = Row[0][1];
    float a2 = Row[0][2];
    float a3 = Row[0][3];
    float b0 = Row[1][0];
    float b1 = Row[1][1];
    float b2 = Row[1][2];
    float b3 = Row[1][3];
    Row[0][0] = c * a0 + ns * b0;
    Row[0][1] = c * a1 + ns * b1;
    Row[0][2] = c * a2 + ns * b2;
    Row[0][3] = c * a3 + ns * b3;
    Row[1][0] = s * a0 + c * b0;
    Row[1][1] = s * a1 + c * b1;
    Row[1][2] = s * a2 + c * b2;
    Row[1][3] = s * a3 + c * b3;
}


// ?Rotate_X@Matrix3D@@QAEXM@Z (0x00590760)
// Rotates about the local X axis by theta.

void Matrix3D::Rotate_X(float theta)
{
    float s = Fast_Sin(theta);
    float c = Fast_Cos(theta);
    float tmp1;
    float tmp2;
    tmp1 = Row[0][1];
    tmp2 = Row[0][2];
    Row[0][1] = c * tmp1 + s * tmp2;
    Row[0][2] = -s * tmp1 + c * tmp2;
    tmp1 = Row[1][1];
    tmp2 = Row[1][2];
    Row[1][1] = c * tmp1 + s * tmp2;
    Row[1][2] = -s * tmp1 + c * tmp2;
    tmp1 = Row[2][1];
    tmp2 = Row[2][2];
    Row[2][1] = c * tmp1 + s * tmp2;
    Row[2][2] = -s * tmp1 + c * tmp2;
}


// ?Rotate_X@Matrix3D@@QAEXMM@Z (0x00590800)
// Rotates about the local X axis (sine and cosine given).

void Matrix3D::Rotate_X(float s, float c)
{
    float tmp1;
    float tmp2;
    tmp1 = Row[0][1];
    tmp2 = Row[0][2];
    Row[0][1] = c * tmp1 + s * tmp2;
    Row[0][2] = -s * tmp1 + c * tmp2;
    tmp1 = Row[1][1];
    tmp2 = Row[1][2];
    Row[1][1] = c * tmp1 + s * tmp2;
    Row[1][2] = -s * tmp1 + c * tmp2;
    tmp1 = Row[2][1];
    tmp2 = Row[2][2];
    Row[2][1] = c * tmp1 + s * tmp2;
    Row[2][2] = -s * tmp1 + c * tmp2;
}


// ?Rotate_Y@Matrix3D@@QAEXM@Z (0x00590880)
// Rotates about the local Y axis by theta.

void Matrix3D::Rotate_Y(float theta)
{
    float s = Fast_Sin(theta);
    float c = Fast_Cos(theta);
    float tmp1;
    float tmp2;
    tmp1 = Row[0][0];
    tmp2 = Row[0][2];
    Row[0][0] = c * tmp1 - s * tmp2;
    Row[0][2] = s * tmp1 + c * tmp2;
    tmp1 = Row[1][0];
    tmp2 = Row[1][2];
    Row[1][0] = c * tmp1 - s * tmp2;
    Row[1][2] = s * tmp1 + c * tmp2;
    tmp1 = Row[2][0];
    tmp2 = Row[2][2];
    Row[2][0] = c * tmp1 - s * tmp2;
    Row[2][2] = s * tmp1 + c * tmp2;
}


// ?Rotate_Y@Matrix3D@@QAEXMM@Z (0x00590920)
// Rotates about the local Y axis (sine and cosine given).

void Matrix3D::Rotate_Y(float s, float c)
{
    float tmp1;
    float tmp2;
    tmp1 = Row[0][0];
    tmp2 = Row[0][2];
    Row[0][0] = c * tmp1 - s * tmp2;
    Row[0][2] = s * tmp1 + c * tmp2;
    tmp1 = Row[1][0];
    tmp2 = Row[1][2];
    Row[1][0] = c * tmp1 - s * tmp2;
    Row[1][2] = s * tmp1 + c * tmp2;
    tmp1 = Row[2][0];
    tmp2 = Row[2][2];
    Row[2][0] = c * tmp1 - s * tmp2;
    Row[2][2] = s * tmp1 + c * tmp2;
}


// ?Rotate_Z@Matrix3D@@QAEXM@Z (0x005909A0)
// Rotates about the local Z axis by theta.

void Matrix3D::Rotate_Z(float theta)
{
    float c = Fast_Cos(theta);
    float s = Fast_Sin(theta);
    float tmp1;
    float tmp2;
    tmp1 = Row[0][0];
    tmp2 = Row[0][1];
    Row[0][0] = c * tmp1 + s * tmp2;
    Row[0][1] = -s * tmp1 + c * tmp2;
    tmp1 = Row[1][0];
    tmp2 = Row[1][1];
    Row[1][0] = c * tmp1 + s * tmp2;
    Row[1][1] = -s * tmp1 + c * tmp2;
    tmp1 = Row[2][0];
    tmp2 = Row[2][1];
    Row[2][0] = c * tmp1 + s * tmp2;
    Row[2][1] = -s * tmp1 + c * tmp2;
}


// ?Rotate_Z@Matrix3D@@QAEXMM@Z (0x00590A40)
// Rotates about the local Z axis (sine and cosine given).

void Matrix3D::Rotate_Z(float s, float c)
{
    float tmp1;
    float tmp2;
    tmp1 = Row[0][0];
    tmp2 = Row[0][1];
    Row[0][0] = c * tmp1 + s * tmp2;
    Row[0][1] = -s * tmp1 + c * tmp2;
    tmp1 = Row[1][0];
    tmp2 = Row[1][1];
    Row[1][0] = c * tmp1 + s * tmp2;
    Row[1][1] = -s * tmp1 + c * tmp2;
    tmp1 = Row[2][0];
    tmp2 = Row[2][1];
    Row[2][0] = c * tmp1 + s * tmp2;
    Row[2][1] = -s * tmp1 + c * tmp2;
}


// ?Get_X_Translation@Matrix3D@@QBEMXZ (0x00590AC0)
// The X coordinate of the transformed origin.

float Matrix3D::Get_X_Translation() const
{
    Vector3 v = *this * Vector3(0.0f, 0.0f, 0.0f);
    return v.X;
}


// ?Get_Y_Translation@Matrix3D@@QBEMXZ (0x00590B10)
// The Y coordinate of the transformed origin.

float Matrix3D::Get_Y_Translation() const
{
    Vector3 v = *this * Vector3(0.0f, 0.0f, 0.0f);
    return v.Y;
}


// ?Get_Z_Translation@Matrix3D@@QBEMXZ (0x00590B60)
// The Z coordinate of the transformed origin.

float Matrix3D::Get_Z_Translation() const
{
    Vector3 v = *this * Vector3(0.0f, 0.0f, 0.0f);
    return v.Z;
}


// ?Get_X_Rotation@Matrix3D@@QBEMXZ (0x00590BB0)
// The rotation about the X axis, from where the transform takes a unit axis.

float Matrix3D::Get_X_Rotation() const
{
    Vector3 v = *this * Vector3(0.0f, 1.0f, 0.0f);
    return Fast_Atan2(v.Z, v.Y);
}


// ?Get_Y_Rotation@Matrix3D@@QBEMXZ (0x00590C10)
// The rotation about the Y axis, from where the transform takes a unit axis.

float Matrix3D::Get_Y_Rotation() const
{
    Vector3 v = *this * Vector3(0.0f, 0.0f, 1.0f);
    return Fast_Atan2(v.X, v.Z);
}


// ?Get_Z_Rotation@Matrix3D@@QBEMXZ (0x00590C70)
// The rotation about the Z axis, from where the transform takes a unit axis.

float Matrix3D::Get_Z_Rotation() const
{
    Vector3 v = *this * Vector3(1.0f, 0.0f, 0.0f);
    return Fast_Atan2(v.Y, v.X);
}


// ?Rotate_Vector@Matrix3D@@QBE?AUVector3@@ABU2@@Z (0x00590CD0)
// Applies the rotation part to vector.

Vector3 Matrix3D::Rotate_Vector(Vector3 const &vector) const
{
    return Vector3(Row[0][0] * vector.X + Row[0][1] * vector.Y + Row[0][2] * vector.Z,
                   Row[1][0] * vector.X + Row[1][1] * vector.Y + Row[1][2] * vector.Z,
                   Row[2][0] * vector.X + Row[2][1] * vector.Y + Row[2][2] * vector.Z);
}


// void Matrix3D::Look_At(Vector3 const &position, Vector3 const &target, float roll) (0x00590D50)
// A transform at position facing target: pitch and yaw from the direction,
// then roll about the view axis (Westwood's Look_At; the axis conventions
// follow the original calls).

double __cdecl unknown_sqrt_4ba410(double value);

void Matrix3D::Look_At(Vector3 const &position, Vector3 const &target, float roll)
{
    float dx = target.X - position.X;
    float dy = target.Y - position.Y;
    float dz = target.Z - position.Z;
    float len1 = (float)unknown_sqrt_4ba410(dy * dy + dx * dx + dz * dz);
    float len2 = (float)unknown_sqrt_4ba410(dx * dx + dz * dz);
    float sinp;
    float cosp;
    float siny;
    float cosy;
    if (len1 != 0.0f) {
        sinp = dy / len1;
        cosp = len2 / len1;
    } else {
        sinp = 0.0f;
        cosp = 1.0f;
    }
    if (len2 != 0.0f) {
        siny = dx / len2;
        cosy = dz / len2;
    } else {
        siny = 0.0f;
        cosy = 1.0f;
    }
    Make_Identity();
    Translate(position);
    Rotate_Y(siny, cosy);
    Rotate_X(-sinp, cosp);
    Rotate_Z(roll);
}


// void Matrix3D::Obj_Look_At(Vector3 const &position, Vector3 const &target, float roll) (0x00590F10)
// A transform at position whose Z axis points at target (yaw and pitch from the direction, then roll),
// built from the forward, up and right vectors (Westwood's wwmath Obj_Look_At). The up vector is
// assigned member by member and the length is held as a double (both fix the original's load order).

void Matrix3D::Obj_Look_At(Vector3 const &position, Vector3 const &target, float roll)
{
    Vector3 dir(target.X - position.X, target.Y - position.Y, target.Z - position.Z);
    float yaw;
    if (dir.Y == 0.0f && dir.X == 0.0f) {
        yaw = 0.0f;
    } else {
        yaw = Fast_Atan2(dir.Y, dir.X);
    }
    float pitch = Fast_Atan2(-dir.Z, Fast_Sqrt(dir.X * dir.X + (dir.Y * dir.Y)));
    float cy = Fast_Cos(yaw);
    float sy = Fast_Sin(yaw);
    float cp = Fast_Cos(pitch);
    float sp = Fast_Sin(pitch);
    float cr = Fast_Cos(roll);
    float sr = Fast_Sin(roll);
    Vector3 up;
    up.X = sr * sy + cr * sp * cy;
    up.Y = cr * sp * sy - sr * cy;
    up.Z = cr * cp;
    double len = Fast_Sqrt(dir.X * dir.X + dir.Y * dir.Y + dir.Z * dir.Z);
    Vector3 forward;
    if (len != 0.0) {
        forward = Vector3(dir.X / len, dir.Y / len, dir.Z / len);
    } else {
        forward = dir;
    }
    Vector3 right(forward.Y * up.Z - forward.Z * up.Y, forward.Z * up.X - forward.X * up.Z, up.Y * forward.X - up.X * forward.Y);
    Row[0][0] = right.X;
    Row[0][1] = up.X;
    Row[0][2] = forward.X;
    Row[0][3] = position.X;
    Row[1][0] = right.Y;
    Row[1][1] = up.Y;
    Row[1][2] = forward.Y;
    Row[1][3] = position.Y;
    Row[2][0] = right.Z;
    Row[2][1] = up.Z;
    Row[2][2] = forward.Z;
    Row[2][3] = position.Z;
}


// Matrix3D operator*(Matrix3D const &, Matrix3D const &) (0x00591180): the product of two transforms
// (Westwood's wwmath matrix product). Left's 3x3 and all of right are read into float locals first; the
// result is a local returned through the copy constructor, which this unit defines first (0x0058FE10) and
// expands here.

Matrix3D __fastcall operator*(Matrix3D const &left, Matrix3D const &right)
{
    Matrix3D result;
    float a00 = left.Row[0][0];
    float a01 = left.Row[0][1];
    float a02 = left.Row[0][2];
    float a10 = left.Row[1][0];
    float a11 = left.Row[1][1];
    float a12 = left.Row[1][2];
    float a20 = left.Row[2][0];
    float a21 = left.Row[2][1];
    float a22 = left.Row[2][2];
    float b00 = right.Row[0][0];
    float b01 = right.Row[0][1];
    float b02 = right.Row[0][2];
    float b03 = right.Row[0][3];
    float b10 = right.Row[1][0];
    float b11 = right.Row[1][1];
    float b12 = right.Row[1][2];
    float b13 = right.Row[1][3];
    float b20 = right.Row[2][0];
    float b21 = right.Row[2][1];
    float b22 = right.Row[2][2];
    float b23 = right.Row[2][3];
    result.Row[0][0] = a00 * b00 + a01 * b10 + a02 * b20;
    result.Row[1][0] = a10 * b00 + a11 * b10 + a12 * b20;
    result.Row[2][0] = a20 * b00 + a21 * b10 + a22 * b20;
    result.Row[0][1] = a00 * b01 + a01 * b11 + a02 * b21;
    result.Row[1][1] = a10 * b01 + a11 * b11 + a12 * b21;
    result.Row[2][1] = a20 * b01 + a21 * b11 + a22 * b21;
    result.Row[0][2] = a00 * b02 + a01 * b12 + a02 * b22;
    result.Row[1][2] = a10 * b02 + a11 * b12 + a12 * b22;
    result.Row[2][2] = a20 * b02 + a21 * b12 + a22 * b22;
    result.Row[0][3] = a00 * b03 + a01 * b13 + a02 * b23 + left.Row[0][3];
    result.Row[1][3] = a10 * b03 + a11 * b13 + a12 * b23 + left.Row[1][3];
    result.Row[2][3] = a20 * b03 + a21 * b13 + a22 * b23 + left.Row[2][3];
    return result;
}


// ??D@YI?AUVector3@@ABVMatrix3D@@ABU0@@Z (0x00591380)
// Transforms a point: the rotation rows applied to vector, plus the translation column.
// The rotation elements are read into one-use float locals first; that fixes the x87
// operand order and the scheduling of the return-pointer copy (mov eax, ecx).

Vector3 __fastcall operator*(Matrix3D const &matrix, Vector3 const &vector)
{
    float m00 = matrix.Row[0][0], m01 = matrix.Row[0][1], m02 = matrix.Row[0][2];
    float m10 = matrix.Row[1][0], m11 = matrix.Row[1][1], m12 = matrix.Row[1][2];
    float m20 = matrix.Row[2][0], m21 = matrix.Row[2][1], m22 = matrix.Row[2][2];
    return Vector3(m00 * vector.X + m01 * vector.Y + m02 * vector.Z + matrix.Row[0][3],
                   (m10 * vector.X) + m11 * vector.Y + m12 * vector.Z + matrix.Row[1][3],
                   (m20 * vector.X) + m21 * vector.Y + m22 * vector.Z + matrix.Row[2][3]);
}


// 0x00591400: returns a copy of a Matrix3D (hidden return pointer in ecx, source
// in edx). Nothing in the executable calls or references it; written as a
// __fastcall free function by value like Matrix3D_Rigid_Inverse_00591420 (the
// ABI is a hypothesis). The copy constructor 0x0058FE10 defined earlier in
// matrix3d.cpp is expanded inline (rep movsd).

Matrix3D __fastcall Matrix3D_Copy_00591400(Matrix3D const &matrix)
{
    return matrix;
}


Matrix3D __fastcall Matrix3D_Rigid_Inverse_00591420(Matrix3D const &matrix)
{
    Matrix3D inverse;
    inverse.Row[0][0] = matrix.Row[0][0]; inverse.Row[0][1] = matrix.Row[1][0]; inverse.Row[0][2] = matrix.Row[2][0];
    inverse.Row[1][0] = matrix.Row[0][1]; inverse.Row[1][1] = matrix.Row[1][1]; inverse.Row[1][2] = matrix.Row[2][1];
    inverse.Row[2][0] = matrix.Row[0][2]; inverse.Row[2][1] = matrix.Row[1][2]; inverse.Row[2][2] = matrix.Row[2][2];
    inverse.Row[0][3] = -(inverse.Row[0][0] * matrix.Row[0][3] + inverse.Row[0][1] * matrix.Row[1][3] + inverse.Row[0][2] * matrix.Row[2][3]);
    inverse.Row[1][3] = -(inverse.Row[1][0] * matrix.Row[0][3] + inverse.Row[1][1] * matrix.Row[1][3] + inverse.Row[1][2] * matrix.Row[2][3]);
    inverse.Row[2][3] = -(inverse.Row[2][0] * matrix.Row[0][3] + inverse.Row[2][1] * matrix.Row[1][3] + inverse.Row[2][2] * matrix.Row[2][3]);
    return inverse;
}
