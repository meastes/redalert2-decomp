// quat.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/quat.json carries the addresses and the rows that
// stay in their own files.


#define RA2_MATRIX3D_INLINE_COPY
#define VECTOR3_DECLARE_WWMATH
#include "shared/quaternion.hpp"
#define SLERP_EPSILON 1e-5


// ??0Quaternion@@QAE@MMMM@Z (0x00622160)
// Quaternion(x, y, z, w).

Quaternion::Quaternion(float x, float y, float z, float w)
{
    X = x;
    Y = y;
    Z = z;
    W = w;
}


// ?Set@Quaternion@@QAEXMMMM@Z (0x00622180)
// Sets the four components.

void Quaternion::Set(float x, float y, float z, float w)
{
    X = x;
    Y = y;
    Z = z;
    W = w;
}


// ?Normalize@Quaternion@@QAEXXZ (0x006221A0)
// Divides the components by the squared length (nothing for a zero quaternion).

void Quaternion::Normalize()
{
    float len2 = X * X + Y * Y + Z * Z + W * W;
    if (0.0f != len2) {
        X /= len2;
        Y /= len2;
        Z /= len2;
        W /= len2;
    }
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).

// ?Scale@Quaternion@@QAEXM@Z (0x00622200)
// Multiplies the components by scale.

void Quaternion::Scale(float scale)
{
    X = (float)(scale * X);
    Y = (float)(scale * Y);
    Z = (float)(scale * Z);
    W = (float)(scale * W);
}


// ??AQuaternion@@QAEAAMH@Z (0x00622230)
// Component by index.

float &Quaternion::operator[](int index)
{
    return (&X)[index];
}


// ??AQuaternion@@QBEMH@Z (0x00622240)
// Component by index (value).

float Quaternion::operator[](int index) const
{
    return (&X)[index];
}


// ??4Quaternion@@QAE?AV0@ABV0@@Z (0x00622250)
// Assignment, returning a copy.

Quaternion Quaternion::operator=(Quaternion const &quat)
{
    X = quat[0];
    Y = quat[1];
    Z = quat[2];
    W = quat[3];
    return *this;
}


// Not exact (RA2_NON_MATCHING): Quaternion::Make_Closest at 0x00622290, best score 53.96, 2026-11-13; differs: 125 instructions in the original, 153 in the draft (shape 77.70)
#ifdef RA2_NON_MATCHING
// Quaternion &Quaternion::Make_Closest(Quaternion const &target) (0x00622290)
// Use the nearer of the two representations of this rotation (TibSun quat.cpp Q::Make_Closest).
inline float Quaternion::Length2() const
{
    return X * X + Y * Y + Z * Z + W * W;
}

Quaternion &Quaternion::Make_Closest(Quaternion const &qto)
{
    Quaternion t = *this;
    Quaternion rel1 = t * Conjugate(qto);

    Quaternion negthis = Quaternion(-X, -Y, -Z, -W);
    Quaternion rel2 = negthis * Conjugate(qto);

    float dot1 = rel1.Length2();
    float dot2 = rel2.Length2();

    // if we are on opposite hemisphere from qto, negate ourselves
    if (dot2 < dot1) {
        X = -X;
        Y = -Y;
        Z = -Z;
        W = -W;
    }

    return(*this);
}
#endif

// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).

Quaternion __fastcall operator * (const Quaternion & a,const Quaternion & b)
{
	Quaternion t1;
	Quaternion t2;
	Quaternion t3;
	Quaternion q;

	t1 = a;
	t2 = b;

	t1.Scale(b.W);
	t2.Scale(a.W);

	t3[0] = a.Y * b.Z - a.Z * b.Y;
	t3[1] = a.Z * b.X - a.X * b.Z;
	t3[2] = a.X * b.Y - a.Y * b.X;

	q.X = t1.X + t2.X + t3.X;
	q[1] = t1[1] + t2[1] + t3.Y;
	q[2] = t1[2] + t2[2] + t3.Z;
	q.W = a.W * b.W - (a.X * b.X + (a.Y * b.Y + a[2] * b[2]));

	q.Normalize();
	return(q);
}


// ??K@YI?AVQuaternion@@ABV0@0@Z (0x00622570)
// a times the inverse of b.

Quaternion __fastcall operator/(Quaternion const &a, Quaternion const &b)
{
    return a * Inverse(b);
}


// ?Conjugate@@YI?AVQuaternion@@ABV1@@Z (0x006225F0)
// The conjugate (vector part negated).

Quaternion __fastcall Conjugate(Quaternion const &quat)
{
    return Quaternion(-quat.X, -quat.Y, -quat.Z, quat.W);
}


// ?Inverse@@YI?AVQuaternion@@ABV1@@Z (0x00622640)
// The inverse of a unit quaternion (its conjugate).

Quaternion __fastcall Inverse(Quaternion const &quat)
{
    return Quaternion(-quat[0], -quat[1], -quat[2], quat[3]);
}


// Quaternion rotation between two positions projected onto a virtual trackball.
inline Vector3 operator-(Vector3 const &a, Vector3 const &b)
{
    return Vector3(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
}

// The delta's length: the three doubles stay on the x87 stack in declaration order.
inline float Trackball_Length_00622690(Vector3 const &vector)
{
    double y = vector.Y;
    double z = vector.Z;
    double x = vector.X;
    return Fast_Sqrt(x * x + z * z + y * y);
}

static float project_to_sphere(float radius, float x, float y)
{
    const float sqrt_two = 1.41421356f;
    float distance = Fast_Sqrt(x * x + y * y);
    float z;
    if (distance < radius * (sqrt_two / 2.0f)) {
        z = Fast_Sqrt(radius * radius - distance * distance);
    } else {
        float tangent = radius / sqrt_two;
        z = tangent * tangent / distance;
    }
    return z;
}
Quaternion __fastcall Trackball(float x0, float y0, float x1, float y1, float radius)
{
    Vector3 axis;
    float angle;
    Vector3 first;
    Vector3 second;
    Vector3 delta;
    float distance;
    if (x0 == x1 && y0 == y1) return Quaternion(0.0f,0.0f,0.0f,1.0f);
    first.X = x0;
    first.Y = y0;
    first.Z = project_to_sphere(radius, x0, y0);
    second.X = x1;
    second.Y = y1;
    second.Z = project_to_sphere(radius, x1, y1);
    axis = Vector3::Cross_Product(second, first);
    delta = first - second;
    distance = Trackball_Length_00622690(delta) / (2.0 * radius);
    if (distance > 1.0f) distance = 1.0f;
    else if (distance < -1.0f) distance = -1.0f;
    // Existing accepted name for the original 004BA550 inverse-trig callee;
    // its asin/acos identity is still a separate hypothesis.
    angle = 2.0 * Fast_Asin_004ba550(distance);
    return Axis_To_Quat(axis, angle);
}


// Quaternion rotation about a normalized axis.
// Vector3::Length, one call deep: Trackball inlines this row.
inline float Quat_Vector_Length(Vector3 const &vector)
{
    return Fast_Sqrt(vector.X * vector.X + vector.Y * vector.Y + vector.Z * vector.Z);
}
inline Vector3 Normalize(Vector3 const &vector)
{
    float length = Quat_Vector_Length(vector);
    if (length != 0.0) {
        double divisor = length;
        return Vector3((float)(vector.X / divisor), (float)(vector.Y / divisor), (float)(vector.Z / divisor));
    }
    return vector;
}
Quaternion __fastcall Axis_To_Quat(Vector3 const &axis, float angle)
{
    Quaternion result;
    Vector3 normalized = axis;
    normalized = Normalize(normalized);
    result.X = normalized.X;
    result.Y = normalized.Y;
    result.Z = normalized.Z;
    result.Scale(Fast_Sin(angle / 2.0f));
    result.W = Fast_Cos(angle / 2.0f);
    return result;
}


// ?Slerp@@YI?AVQuaternion@@ABV1@0M@Z (0x00622AC0)
// Spherical interpolation from a to b by t.


Quaternion __fastcall Slerp(Quaternion const &a, Quaternion const &b, float t)
{
    Quaternion res;
    double alpha;
    double beta;
    double theta;
    double sin_t, cos_t;

    cos_t = a.X * b.X + a.Y * b.Y + a.Z * b.Z + a.W * b.W;

    if ((cos_t + 1.0) > SLERP_EPSILON) {
        if ((1.0 - cos_t) > SLERP_EPSILON) {
            theta = Fast_Acos_004ba580(cos_t);
            sin_t = Fast_Sin(theta);
            beta = Fast_Sin((1.0f - t) * theta) / sin_t;
            alpha = Fast_Sin(t * theta) / sin_t;
        } else {
            beta = 1.0f - t;
            alpha = t;
        }
        for (int i = 0; i < 4; i++) {
            res[i] = (beta * a[i]) + (alpha * b[i]);
        }
    } else {
        beta = Fast_Sin((1.0f - t) * (3.14159265358979 / 2));
        alpha = Fast_Sin(t * (3.14159265358979 / 2));
        for (int i = 0; i < 4; i++) {
            res[i] = (beta * a[i]) + (alpha * b[i]);
        }
    }
    return res;
}


// ?Build_Quaternion@@YI?AVQuaternion@@ABVMatrix3D@@@Z (0x00622C60)
// The rotation quaternion of a matrix's rotation part.

Quaternion __fastcall Build_Quaternion(Matrix3D const &mat)
{
    float const *m = (float const *)&mat;
    double s, tr;
    int i, j, k;
    Quaternion q;
    int tab1[3] = { (4 * 0) + 0, (4 * 1) + 1, (4 * 2) + 2 };
    int tab2[3][4] = {
        { (4 * 0) + 0, (4 * 0) + 1, (4 * 0) + 2, (4 * 0) + 3 },
        { (4 * 1) + 0, (4 * 1) + 1, (4 * 1) + 2, (4 * 1) + 3 },
        { (4 * 2) + 0, (4 * 2) + 1, (4 * 2) + 2, (4 * 2) + 3 },
    };

    tr = m[tab1[0]] + (m[tab1[1]] + m[tab1[2]]);
    if (tr == 0.0) {
        return q;
    }

    if (tr > 0.0) {
        s = Fast_Sqrt(tr + 1.0);
        q[3] = (s * 0.5);
        s = 0.5 / s;
        q[0] = (m[tab2[2][1]] - m[tab2[1][2]]) * s;
        q[1] = (m[tab2[0][2]] - m[tab2[2][0]]) * s;
        q[2] = (m[tab2[1][0]] - m[tab2[0][1]]) * s;
    } else {
        i = 0;
        if ((double)m[tab1[1]] > (double)m[tab1[i]]) i = 1;
        if ((double)m[tab1[2]] > (double)m[tab1[i]]) i = 2;
        switch (i) {
            case 1:
                j = 2;
                k = 0;
                break;
            case 2:
                j = 0;
                k = 1;
                break;
            default:
                j = 1;
                k = 2;
                break;
        }
        double di = m[tab1[i]];
        double dj = m[tab1[j]];
        double dk = m[tab1[k]];
        s = Fast_Sqrt(di - (dj + dk) + 1.0);
        q[i] = (s * 0.5);
        s = 0.5 / s;
        q[3] = (m[tab2[k][j]] - m[tab2[j][k]]) * s;
        q[j] = (m[tab2[j][i]] + m[tab2[i][j]]) * s;
        q[k] = (m[tab2[k][i]] + m[tab2[i][k]]) * s;
    }
    return q;
}


// Converts a quaternion rotation to an affine matrix with zero translation.

Matrix3D __fastcall Build_Matrix3D(Quaternion const &rotation)
{
    Matrix3D matrix;
    matrix.Row[0][0] = 1.0f - 2.0f * (rotation[1] * rotation[1] + rotation[2] * rotation[2]);
    matrix.Row[0][1] = 2.0f * (rotation[0] * rotation[1] - rotation[2] * rotation[3]);
    matrix.Row[0][2] = 2.0f * (rotation[2] * rotation[0] + rotation[1] * rotation[3]);
    matrix.Row[1][0] = 2.0f * (rotation[0] * rotation[1] + rotation[2] * rotation[3]);
    matrix.Row[1][1] = 1.0f - 2.0f * (rotation[2] * rotation[2] + rotation[0] * rotation[0]);
    matrix.Row[1][2] = 2.0f * (rotation[1] * rotation[2] - rotation[0] * rotation[3]);
    matrix.Row[2][0] = 2.0f * (rotation[2] * rotation[0] - rotation[1] * rotation[3]);
    matrix.Row[2][1] = 2.0f * (rotation[1] * rotation[2] + rotation[0] * rotation[3]);
    matrix.Row[2][2] = 1.0f - 2.0f * (rotation[1] * rotation[1] + rotation[0] * rotation[0]);
    matrix.Row[0][3] = matrix.Row[1][3] = matrix.Row[2][3] = 0.0f;
    return matrix;
}
