/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/

#include "always.h"

#include "_rect.h"

#include "rect.h"

// RA2: RECT_NONE is defined in rect.h (each module including it holds a copy); this module's is its first initializer
Rect TacticalRect(0,0,640,400);
Rect VisibleRect(0,0,640,400);
// RA2: SidebarRect is not defined here (its definition would add an empty initializer the original's table lacks)
