// void SaveVersionInfo::Get_Last_Time(FILETIME &time) const (0x006594F0)
#include "shared/savegame_info.hpp"

void SaveVersionInfo::Get_Last_Time(FILETIME &time) const
{
    time = LastSaveTime;
}
