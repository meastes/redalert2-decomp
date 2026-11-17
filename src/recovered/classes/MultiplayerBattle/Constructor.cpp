// MultiplayerBattle::MultiplayerBattle(...) -- 0x005A2320 (the MultiplayerBattle unit)
// Forwards its six arguments to the MultiplayerGameMode constructor
// (0x005B6CD0). This unit also emits (and the link kept) the
// MultiplayerGameMode inline virtuals 0x005A2380-0x005A24F0.
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/multiplayer_game_mode.hpp"

MultiplayerBattle::MultiplayerBattle(WideString const &name, WideString const &description, Wstring const &ini_name,
    Wstring const &unknown_4, bool unknown_5, int id) :
    MultiplayerGameMode(name, description, ini_name, unknown_4, unknown_5, id)
{
}
