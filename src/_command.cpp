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

#include "always.h"
#include "rect.h" // RA2: RECT_NONE, the first initializer of the unit
#include "_command.h"
#include "index.h"
#include "vector.h"

class CommandClass;

IndexClass<int, CommandClass const *> HotkeyCommands;
DynamicVectorClass<CommandClass const *> AllCommands;
