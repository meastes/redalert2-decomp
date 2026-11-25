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

#pragma once

enum BAnimType {
	BANIM_NONE = -1,

	BANIM_UPGRADE_ONE,
	BANIM_UPGRADE_TWO,
	BANIM_UPGRADE_THREE,
	BANIM_ACTIVE_ONE,
	BANIM_ACTIVE_TWO,
	BANIM_ACTIVE_THREE,
	BANIM_ACTIVE_FOUR,
	BANIM_PRE_PRODUCTION,
	BANIM_PRODUCTION,
	BANIM_TURRET,
	BANIM_SPECIAL_ONE,
	BANIM_SPECIAL_TWO,
	BANIM_SPECIAL_THREE,
	BANIM_SUPER_ONE, // RA2: SuperAnim (BuildingTypeClass::Read_INI reads it into slot 13)
	BANIM_SUPER_TWO, // RA2: SuperAnimTwo, slot 14
	BANIM_SUPER_THREE, // RA2: SuperAnimThree, slot 15
	BANIM_SUPER_FOUR, // RA2: SuperAnimFour, slot 16
	BANIM_IDLE, // RA2: IdleAnim, slot 17

	BANIM_COUNT,
	BANIM_FIRST = 0,

	BANIM_ALL = -2,
};
