// void SaveVersionInfo::Set_Last_Time(FILETIME const &time) (0x006594D0)
#include "shared/savegame_info.hpp"

void SaveVersionInfo::Set_Last_Time(FILETIME const &time)
{
    LastSaveTime = time;
}
