// void SaveVersionInfo::Set_Play_Time(FILETIME const &time) (0x00659490)
#include "shared/savegame_info.hpp"

void SaveVersionInfo::Set_Play_Time(FILETIME const &time)
{
    PlayTime = time;
}
