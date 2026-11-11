// SaveVersionInfo::SaveVersionInfo() (0x00659240)
// The first function of the SaveVersionInfo unit (TibSun savever.cpp, a separate file from
// saveload.cpp: SaveLoad's Save_Game/Load_Game call its accessors out of line). The unit's smart
// pointers release through comip's _Release, expanded inline (shared/com_ptr.hpp).
#define COM_PTR_IMPLEMENT_RELEASE
#define COM_PTR_RELEASE_IN_DESTRUCTOR
#include "shared/savegame_info.hpp"

SaveVersionInfo::SaveVersionInfo()
{
    InternalVersion = 0;
    Version = 0;
    CampaignNumber = -1;
    ScenarioNumber = 0;
    GameType = 0;
    ScenarioDescription[0] = 0;
    PlayerHouse[0] = 0;
    UnknownString[0] = 0;
    PlayerName[0] = 0;
    ExecutableName[0] = 0;
    StartTime.dwLowDateTime = 0;
    StartTime.dwHighDateTime = 0;
    PlayTime.dwLowDateTime = 0;
    PlayTime.dwHighDateTime = 0;
    LastSaveTime.dwLowDateTime = 0;
    LastSaveTime.dwHighDateTime = 0;
}
