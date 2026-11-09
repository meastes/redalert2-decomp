// char const *SavegameInformation::Get_unknown_294() const (0x00659400)
// mischa85/RedAlert2 commit 5a79accc names this SaveVersionInfo::Get_Player_Name (src/savever.cpp),
// the char[64] at +0x294 (name hypothesis; recorded for the endgame renames).
#include "shared/savegame_info.hpp"

char const *SavegameInformation::Get_unknown_294() const
{
    return unknown_294;
}
