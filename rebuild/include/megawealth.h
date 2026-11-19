/*
**	Command & Conquer Red Alert 2
**	MEGAWEALTH.H: the Megawealth multiplayer game mode (RTTI .?AVMegawealth@@, vtable 0x007A6AA4).
**
**	Slot 29 (OnGameStart) is its own; the other 44 slots are MultiplayerGameMode's. The
**	constructor (0x005AA720) passes its six arguments to MultiplayerGameMode's unchanged.
*/
#pragma once

#include "multiplayer.h"

class Megawealth : public MultiplayerGameMode
{
	public:
		Megawealth(WideWstring const & title, WideWstring const & tooltip, Wstring const & inifile, Wstring const & mapfilter, bool aiallowed, int modeindex);
		virtual ~Megawealth(void);
		virtual bool OnGameStart(void) override;
};
