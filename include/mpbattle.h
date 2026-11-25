/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 ra2-decomp contributors
 * Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
 *
 * Derived from Electronic Arts source code; EA's GPL Section 7 terms apply.
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/

#pragma once

#include "multiplayer.h"

/*
**	RA2: the battle game mode (movies.cpp defines it, where ra2-decomp's file has it). Red Alert 2's class overrides
**	only the destructor and IsSkirmishAllowed (its vtable at 0x007A66B4 points to the base class for every other slot);
**	Yuri's Revenge's (ra2-decomp) overrides most of its slots.
*/
class MultiplayerBattle : public MultiplayerGameMode
{
	public:
		MultiplayerBattle(WideWstring const & title, WideWstring const & tooltip, Wstring const & inifile, Wstring const & mapfilter, bool aiallowed, int modeindex);
		virtual ~MultiplayerBattle(void);
		virtual bool IsSkirmishAllowed(void);
};
