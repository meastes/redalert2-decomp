// char const *SaveVersionInfo::Get_Player_Name() const (0x00659400)
// mischa85/RedAlert2 commit 5a79accc names this SaveVersionInfo::Get_Player_House (src/savever.cpp),
// the char[64] at +0x294 (name hypothesis; recorded for the endgame renames).
#include "shared/savegame_info.hpp"

char const *SaveVersionInfo::Get_Player_Name() const
{
    return PlayerName;
}
