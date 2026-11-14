// MultiplayerManBattle constructor (0x005A74B0): the base MultiplayerGameMode ctor, then the vtable. Draft: Scout1076.
#include "shared/multiplayer_game_mode.hpp"

MultiplayerManBattle::MultiplayerManBattle(WideString const &name, WideString const &description, Wstring const &ini_name,
    Wstring const &unknown_4, bool unknown_5, int id) :
    MultiplayerGameMode(name, description, ini_name, unknown_4, unknown_5, id)
{
}
