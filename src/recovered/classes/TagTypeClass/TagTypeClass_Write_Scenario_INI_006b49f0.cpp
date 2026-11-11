// void __fastcall TagTypeClass::Write_Scenario_INI(CCINIClass &ini) (0x006B49F0)
// Clears the [Tags] entries and the section, then writes every tag type.
#include "shared/object.hpp"

void __fastcall TagTypeClass::Write_Scenario_INI(CCINIClass &ini)
{
    int count = ini.Entry_Count(unknown_tags_section_7ac770);
    for (int index = 0; index < count; index++) {
        char buffer[32];
        char const *entry = ini.Get_Entry(unknown_tags_section_7ac770, index);
        ini.Get_String(unknown_tags_section_7ac770, entry, unknown_string_83c55c, buffer, sizeof(buffer));
        ini.Clear(buffer, 0);
    }
    ini.Clear(unknown_tags_section_7ac770, 0);
    for (int i = 0; i < TagTypes.Count(); i++) {
        TagTypes[i]->Write_INI(ini);
    }
}
