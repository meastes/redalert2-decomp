// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 OpenTS contributors
#pragma once

#include "multiplayer.h"

/*
**	RA2: the Man Battle game mode (MPModes.ini "ManBattle"). It plays as the base mode does;
**	the class adds nothing but its own constructor and destructor.
*/
class MultiplayerManBattle : public MultiplayerGameMode
{
	public:
		MultiplayerManBattle(WideWstring const & title, WideWstring const & tooltip, Wstring const & inifile, Wstring const & mapfilter, bool aiallowed, int modeindex);
		virtual ~MultiplayerManBattle(void);
};
