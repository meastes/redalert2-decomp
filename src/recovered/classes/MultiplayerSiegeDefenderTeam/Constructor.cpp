// MultiplayerSiegeDefenderTeam::MultiplayerSiegeDefenderTeam() -- 0x005ABDC0 (MPSiegeTeam.cpp)
// The siege defenders: team number 1, named by the localized
// "MP:DefenderTeam" string. Class names from RTTI (vtables 0x007A6C54
// defender, 0x007A6C64 attacker, 0x007A7314 MultiplayerTeam).
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/multiplayer_game_mode.hpp"
#include "shared/string_table.hpp"

MultiplayerSiegeDefenderTeam::MultiplayerSiegeDefenderTeam() :
    MultiplayerTeam(StringTable::Load_String("MP:DefenderTeam", 0, "D:\\Ra2\\MPSiegeTeam.cpp", 6), 1)
{
}
