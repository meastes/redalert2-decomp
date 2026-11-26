// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).

#include "always.h"
#include "rect.h"
#include "geometry.h"

const Rect SecondRectNone(0, 0, 0, 0); // RA2: a second zero rectangle after the header statics; ra2-decomp's statement from its _vanim.cpp (src/_vanim.cpp:42 at b20f510); its map2.cpp lists the initializer (_$E41) without the statement
