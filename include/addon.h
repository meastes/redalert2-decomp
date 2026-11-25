/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/

#pragma once

enum AddonType {
	ADDON_BASE_GAME,
	ADDON_FIRESTORM,
	ADDON_COUNT,

	ADDON_FIRST=0,
	ADDON_ANY=-1,
};


AddonType operator++(AddonType & val);
AddonType operator--(AddonType & val);
