// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).

#include "shared/quaternion.hpp"
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


