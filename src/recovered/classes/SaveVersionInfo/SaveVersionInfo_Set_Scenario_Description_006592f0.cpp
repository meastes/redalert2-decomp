// void SaveVersionInfo::Set_Scenario_Description(wchar_t const *text) (0x006592F0)
#include "shared/savegame_info.hpp"

void SaveVersionInfo::Set_Scenario_Description(wchar_t const *text)
{
    ScenarioDescription[0x7F] = 0;
    wcsncpy(ScenarioDescription, text, 0x7F);
}
