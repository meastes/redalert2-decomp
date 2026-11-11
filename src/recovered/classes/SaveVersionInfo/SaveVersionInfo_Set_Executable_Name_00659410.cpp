// void SaveVersionInfo::Set_Executable_Name(char const *text) (0x00659410)
#include "shared/savegame_info.hpp"

void SaveVersionInfo::Set_Executable_Name(char const *text)
{
    ExecutableName[0x103] = 0;
    strncpy(ExecutableName, text, 0x103);
}
