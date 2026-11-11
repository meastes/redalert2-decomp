// void SaveVersionInfo::Set_Unknown_String(char const *text) (0x006593A0)
#include "shared/savegame_info.hpp"

void SaveVersionInfo::Set_Unknown_String(char const *text)
{
    UnknownString[0x103] = 0;
    strncpy(UnknownString, text, 0x103);
}
