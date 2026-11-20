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

/****************************************************************************
**	The mouse cursor can be in different states. These states are listed
**	below. Some of these represent animating mouse cursors. The mouse
**	is controlled by passing one of these values to the appropriate
**	MouseClass member function.
**
**	RA2: 79 cursors (MouseClass::MouseControl, 0x7E2AF8). The first 36 are Tiberian Sun's; from 36 on
**	the names come from the image (Mouse_Left_Up's action switch 0x49B0F0, Scroll_Coast's cursor table,
**	Draw_Waypoints); cursors with no name evidenced are left unnamed, so a name after such a gap
**	carries its number.
*/
enum MouseType {
	MOUSE_NORMAL,
	MOUSE_N,
	MOUSE_NE,
	MOUSE_E,
	MOUSE_SE,
	MOUSE_S,
	MOUSE_SW,
	MOUSE_W,
	MOUSE_NW,
	MOUSE_NO_N,
	MOUSE_NO_NE,
	MOUSE_NO_E,
	MOUSE_NO_SE,
	MOUSE_NO_S,
	MOUSE_NO_SW,
	MOUSE_NO_W,
	MOUSE_NO_NW,
	MOUSE_CAN_SELECT,
	MOUSE_CAN_MOVE,
	MOUSE_NO_MOVE,
	MOUSE_STAY_ATTACK,
	MOUSE_CAN_ATTACK,
	MOUSE_AREA_GUARD,
	MOUSE_TOTE,
	MOUSE_NO_TOTE,
	MOUSE_ENTER,
	MOUSE_NO_ENTER,
	MOUSE_DEPLOY,
	MOUSE_NO_DEPLOY,
	MOUSE_UNDEPLOY,
	MOUSE_SELL_BACK,
	MOUSE_SELL_UNIT,
	MOUSE_NO_SELL_BACK,
	MOUSE_GREPAIR,				// Engineer entering friendly building to heal it.
	MOUSE_REPAIR,
	MOUSE_NO_REPAIR,
	MOUSE_IVAN_BOMB = 38,
	MOUSE_PARA_DROP = 47,
	MOUSE_LIGHTNING_STORM = 50,
	MOUSE_DEMOLITIONS = 52,
	MOUSE_NUCLEAR_BOMB,
	MOUSE_IRON_CURTAIN = 57,
	MOUSE_CHRONOSPHERE,
	MOUSE_DISARM_BOMB,
	MOUSE_WAYPOINT,
	MOUSE_SCROLL_COASTING,
	MOUSE_SCROLL_COASTING_N,
	MOUSE_SCROLL_COASTING_NE,
	MOUSE_SCROLL_COASTING_E,
	MOUSE_SCROLL_COASTING_SE,
	MOUSE_SCROLL_COASTING_S,
	MOUSE_SCROLL_COASTING_SW,
	MOUSE_SCROLL_COASTING_W,
	MOUSE_SCROLL_COASTING_NW,
	MOUSE_PATROL_WAYPOINT,
	MOUSE_PLACE_BEACON = 78,

	MOUSE_COUNT = 79,
	MOUSE_FIRST=0
};
