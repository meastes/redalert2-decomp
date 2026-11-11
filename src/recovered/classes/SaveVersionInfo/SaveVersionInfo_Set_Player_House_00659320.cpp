// void SaveVersionInfo::Set_Player_House(wchar_t const *text) (0x00659320)
#include "shared/savegame_info.hpp"

void SaveVersionInfo::Set_Player_House(wchar_t const *text)
{
    PlayerHouse[0x3F] = 0;
    wcsncpy(PlayerHouse, text, 0x3F);
}
