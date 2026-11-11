// void SaveVersionInfo::Get_Play_Time(FILETIME &time) const (0x006594B0)
#include "shared/savegame_info.hpp"

void SaveVersionInfo::Get_Play_Time(FILETIME &time) const
{
    time = PlayTime;
}
