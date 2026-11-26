// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).

#include "always.h"
#include "rect.h" // RA2: the header statics, in the order of the unit's initializer table
#include "geometry.h"

#include "_vanim.h"

#include "vanim.h"
#include "vector.h"

const Rect SecondRectNone(0, 0, 0, 0); // RA2: a second zero rectangle after the header statics; ra2-decomp's statement (src/_vanim.cpp:42 at b20f510)

DynamicVectorClass<VoxelAnimClass *> VoxelAnims;
