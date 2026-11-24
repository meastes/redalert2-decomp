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

// RA2: the rectangle class lives in trect.h; this header adds the per-module RECT_NONE, so a module
// that needs only the class (through stbuffer.h) holds no copy of it, as the original's emember, psystype
// and ptype hold none.
#pragma once

#include "trect.h"

// RA2: defined here (internal linkage), so every module including this header holds its own copy and initializer
const Rect RECT_NONE(0, 0, 0, 0);

void __fastcall Inflate_Rect(Rect & rect, int amount); // RA2: moves each edge by amount (txtlabel2.cpp)
