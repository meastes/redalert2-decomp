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
#include "_mixfile.h"
#include "globals.h"
#include "vector.h"

/***************************************************************************
**	This holds the theater specific mixfiles (the mixfile pointers TibSun
**	defines here are not defined yet).
*/
DynamicVectorClass<MFCD *> ExpandMix;
DynamicVectorClass<MixFileClass *> ScenarioMixFiles;
DynamicVectorClass<MFCD *> MapsMixLocal;
DynamicVectorClass<MFCD *> MoviesMixLocal;
