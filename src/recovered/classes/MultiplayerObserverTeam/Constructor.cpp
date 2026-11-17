// MultiplayerObserverTeam::MultiplayerObserverTeam() -- 0x005AA7B0 (MPObserver.cpp)
// The observers' team: a MultiplayerTeam named by the localized
// "MP:ObserverTeam" string (converted implicitly to a WideString), with
// team number -1. Class names from RTTI (vtables 0x007A6B5C, MultiplayerTeam
// 0x007A7314).
#include "shared/multiplayer_game_mode.hpp"
#include "shared/string_table.hpp"

MultiplayerObserverTeam::MultiplayerObserverTeam() :
    MultiplayerTeam(StringTable::Load_String("MP:ObserverTeam", 0, "D:\\Ra2\\MPObserver.cpp", 5), -1)
{
}
