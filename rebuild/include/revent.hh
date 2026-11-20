/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/

#pragma once

/// Names derived from RadarEventNames
enum RadarEventType {
	RADAREVENT_NONE=-1,

	RADAREVENT_COMBAT,
	RADAREVENT_NONCOMBAT,
	RADAREVENT_DROPZONE,
	RADAREVENT_BASE_ATTACKED,
	RADAREVENT_HARVESTER_ATTACKED,
	RADAREVENT_ENEMY_SENSED,
	RADAREVENT_UNIT_PRODUCED,	// RA2: a factory finished a unit (HouseClass::Place_Object)
	RADAREVENT_UNIT_LOST,	// RA2: the eleven kinds from here on are the image's (RadarEventNames at 0x007EE5B8, RadarEventTypes at 0x007A8D58: 17 each); names as ra2-decomp's include/revent.hh
	RADAREVENT_UNIT_REPAIRED,
	RADAREVENT_BUILDING_INFILTRATED,
	RADAREVENT_BUILDING_CAPTURED,
	RADAREVENT_BEACON_PLACED,
	RADAREVENT_SUPERWEAPON_DETECTED,
	RADAREVENT_SUPERWEAPON_ACTIVATED,
	RADAREVENT_BRIDGE_REPAIRED,
	RADAREVENT_GARRISON_ABANDONED,
	RADAREVENT_ALLY_BASE_ATTACKED,

	RADAREVENT_COUNT,
	RADAREVENT_FIRST=0
};
