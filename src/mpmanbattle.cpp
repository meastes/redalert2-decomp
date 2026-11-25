/*
**	Command & Conquer Red Alert 2
**	MPMANBATTLE.CPP: the Man Battle game mode.
*/

#include "always.h"
#include "mpmanbattle.h"


MultiplayerManBattle::MultiplayerManBattle(WideWstring const & title, WideWstring const & tooltip, Wstring const & inifile, Wstring const & mapfilter, bool aiallowed, int modeindex) :
	MultiplayerGameMode(title, tooltip, inifile, mapfilter, aiallowed, modeindex)
{
}


MultiplayerManBattle::~MultiplayerManBattle(void)
{
}
