/*
**	Command & Conquer Red Alert 2
**	MPOBSERVER.CPP: the Megawealth game mode and the observer team of the multiplayer setup.
**
**	The executable names this file in the observer team's string lookup (D:\Ra2\MPObserver.cpp).
**	Megawealth plays like a free-for-all game and reveals the map when the game starts; the
**	observer team holds the players who watch.
**
**	Lane R9 (rebuild), the function bodies written without adopted text and without the old
**	tree's or any other project's body for them: from the executable's instructions. The
**	observer team's declaration (mpsiege.h) was already in the tree. See
**	rebuild/manifest/provenance.csv, class c.
*/

#include "always.h"
#include "rect.h" // RA2: the header statics, in the order of the unit's initializer table
#include "geometry.h"
#include "megawealth.h"
#include "mpsiege.h"
#include "map.h"
#include "_map.h"
#include "stringtable.h"
#include "wstring.h"
#undef LoadString


Megawealth::Megawealth(WideWstring const & title, WideWstring const & tooltip, Wstring const & inifile, Wstring const & mapfilter, bool aiallowed, int modeindex) : // RA2: 0x005AA720
	MultiplayerGameMode(title, tooltip, inifile, mapfilter, aiallowed, modeindex)
{
}


Megawealth::~Megawealth(void) // RA2: 0x005AA760
{
}


/// <summary>
/// RA2: the whole map is open from the first frame.
/// </summary>
bool Megawealth::OnGameStart(void) // RA2: 0x005AA770
{
	Map.Reveal_The_Map();
	return(true);
}


MultiplayerObserverTeam::MultiplayerObserverTeam(void) : // RA2: 0x005AA7B0
	MultiplayerTeam(StringTable::LoadString("MP:ObserverTeam", NULL, "D:\\Ra2\\MPObserver.cpp", 5), -1)
{
}


bool MultiplayerObserverTeam::IsTeamIncluded(int index) // RA2: 0x005AA800
{
	return(MultiplayerTeam::IsTeamIncluded(index));
}
