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

/**********************************************************************
**	The theaters of operation are as follows.
*/
enum TheaterType {
	THEATER_NONE=-1,

	THEATER_TEMPERATE,
	THEATER_SNOW,

	THEATER_COUNT=3,	// RA2: the table at 0x0079AB68 (Theater_From_Name walks it) holds temperate, snow and urban; in place, a new enumerator would unmatch vein.cpp
	THEATER_FIRST=0
};
