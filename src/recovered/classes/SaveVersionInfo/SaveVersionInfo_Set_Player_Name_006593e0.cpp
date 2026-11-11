// void SaveVersionInfo::Set_Player_Name(char const *text) (0x006593E0)
#include "shared/savegame_info.hpp"

void SaveVersionInfo::Set_Player_Name(char const *text)
{
    PlayerName[0x3F] = 0;
    strncpy(PlayerName, text, 0x3F);
}
