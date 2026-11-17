// Reset the inline description buffer to the localized default, original40+8.
#include "shared/map_seed.hpp"
extern "C" wchar_t *__cdecl wcscpy(wchar_t *,wchar_t const *);
void MapSeedClass::Set_Default_Description_00576fe0()
{
    wcscpy(DescriptionBuffer,StringTable::Load_String("TXT_RANDOM_MAP_DESCRIPTION",0,"D:\\Ra2\\MapGen.cpp",0xED1));
}
