// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).

// ?Scale@Quaternion@@QAEXM@Z (0x00622200)
// Multiplies the components by scale.
#include "shared/quaternion.hpp"

void Quaternion::Scale(float scale)
{
    X = (float)(scale * X);
    Y = (float)(scale * Y);
    Z = (float)(scale * Z);
    W = (float)(scale * W);
}
