/*
**	Command & Conquer Red Alert 2
**	MEGAWEALTH.H: the Megawealth multiplayer game mode (RTTI .?AVMegawealth@@, vtable 0x007A6AA4).
**
**	Lane R9: declared from the vtable (45 slots, slot 29 its own OnGameStart, the rest
**	MultiplayerGameMode's) and the constructor at 0x005AA720, which passes its six arguments to
**	MultiplayerGameMode's constructor unchanged.
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
