// Complete MapSeedClass constructor at00576F50, original140+4.
#include "shared/map_seed.hpp"
extern "C" wchar_t *__cdecl wcscpy(wchar_t *,wchar_t const *);
MapSeedClass::MapSeedClass()
    : Theater(0),MapType(1),Resources(1),Ruggedness(0),Time(1),WaterAmount(0),
      NumPlayers(2),Tiberium(0),TiberiumLayout(0),Vegetation(0),UrbanPresence(0),
      Width(0),Height(0),Accessibility(0),RegionSize(0),Seed(-1)
{
    Extension="SED";
    DescriptionBuffer[0]=0;
    Description=DescriptionBuffer;
    if(StringTable::Is_Loaded_006fa030())
        wcscpy(DescriptionBuffer,StringTable::Load_String("TXT_RANDOM_MAP_DESCRIPTION",0,"D:\\Ra2\\MapGen.cpp",0xED1));
}
