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
// Function comment blocks from mischa85/RedAlert2 392e904 src/gametime.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// gametime.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/gametime.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unknown_time_stamp.hpp"
#include "shared/win32_api.hpp"


// The global GameTimeClass at 0x00866074; its dynamic
// initializer 0x004D0030 is the constructor 0x004D0040 expanded inline.

GameTimeClass Game_Time;


/***************************************************************************
 * GameTimeClass - Constructor function for GameTimeClass                  *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/22/1995  MG : Created.                                             *
 *=========================================================================*/
// (0x004D0040)
// Purpose: records the current system time (timeGetTime) at +0x00. No
// direct callers (the out-of-line copy of the constructor the global
// 0x00866074's initializer expands). Owner not identified.

GameTimeClass::GameTimeClass()
{
    Time = timeGetTime();
}


// int GameTimeClass::unknown_elapsed_004d0050() const -- 0x004D0050
// Purpose: milliseconds since the recorded time (+0x00), computed across a
// timeGetTime wrap (one less when the clock has wrapped). No direct callers.

int GameTimeClass::unknown_elapsed_004d0050() const
{
    unsigned long now = timeGetTime();
    if (now <= Time) {
        return now - Time - 1;
    }
    return now - Time;
}


/***************************************************************************
 * Get_Game_Time - returns the time in ms elapsed since game was started   *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      unsigned long - time in milliseconds since game was started        *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/22/1995  MG : Created.                                             *
 *=========================================================================*/
// (0x004D0070)
// Milliseconds since the startup time stamp 0x00866074 (its elapsed time expanded inline).
// Unreferenced; no inventory row.

int Get_Game_Time()
{
    return Game_Time.unknown_elapsed_004d0050();
}


/// <summary>
/// Fetches the elapsed game time in movie timer ticks.
/// This routine is the timer source handed to the VQA player, which paces its frames
/// and its audio against a sixty tick per second clock rather than milliseconds.
/// </summary>
/// <returns>Returns with the time elapsed since the game started, in sixtieths of a
/// second.</returns>
// (0x004D0090)
// Game ticks (1/60 s) elapsed since the startup time stamp 0x00866074.

unsigned long Get_Game_Time_50()
{
    return (unsigned long)Game_Time.unknown_elapsed_004d0050() * 3 / 50;
}
