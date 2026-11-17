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
// flasher.cpp: the original translation unit (one function in RA2 1.006). FlasherClass::Process from
// the TibSun archive dbfbfbf code/flasher.cpp (twins: mischa85/RedAlert2 src/flasher.cpp, ra2-decomp
// src/flasher.cpp); text unchanged. The module map manifests/original_target/modules/flasher.json
// carries the address.

#include "shared/flasher.hpp"


/***********************************************************************************************
 * FlasherClass::Process -- Performs the logic processing for the flashing ability.            *
 *                                                                                             *
 *    The ability for an object to flash is controlled by this logic processing routine. It    *
 *    should be called once per game tick per unit.                                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Should the associated object be redrawn?                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/28/1994 JLB : Created.                                                                 *
 *   06/20/1994 JLB : Is now independent of object it represents.                              *
 *=============================================================================================*/
bool FlasherClass::Process(void)
{
	if (FlashCount) {
		FlashCount--;
		IsBlushing = false;

		if (FlashCount & 0x01) {
			IsBlushing = true;
		}
		return(true);
	}
	return(false);
}
