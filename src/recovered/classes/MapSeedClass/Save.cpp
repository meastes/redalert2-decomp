#include "shared/map_seed.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/ini_class.hpp"
#include "shared/debug_printf.hpp"
extern "C" wchar_t *__cdecl wcscpy(wchar_t *,wchar_t const *);
bool MapSeedClass::Save(char const *filename,wchar_t const *description)
{
    if(filename) {
        Debug_Printf("Saving random map: %s - %ls\n",filename,description);
        CCFileClass file(filename);
        INIClass ini;
        if(description) wcscpy(DescriptionBuffer,description);
        ini.Put_Unicode("RandomMap","Description",description);
        ini.Put_Int("RandomMap","Width",Width);
        ini.Put_Int("RandomMap","Height",Height);
        ini.Put_Int("RandomMap","NumPlayers",NumPlayers);
        ini.Put_Int("RandomMap","Seed",Seed);
        ini.Put_Int("RandomMap","MapType",MapType);
        ini.Put_Int("RandomMap","Theater",Theater);
        ini.Put_Int("RandomMap","Time",Time);
        ini.Put_Int("RandomMap","RegionSize",RegionSize);
        ini.Put_Int("RandomMap","Ruggedness",Ruggedness);
        ini.Put_Int("RandomMap","Accessibility",Accessibility);
        ini.Put_Int("RandomMap","WaterAmount",WaterAmount);
        ini.Put_Int("RandomMap","Tiberium",Tiberium);
        ini.Put_Int("RandomMap","TiberiumLayout",TiberiumLayout);
        ini.Put_Int("RandomMap","Vegetation",Vegetation);
        ini.Put_Int("RandomMap","UrbanPresence",UrbanPresence);
        ini.Put_Int("RandomMap","Resources",Resources);
        ini.Save(file);
        return true;
    }
    return false;
}
