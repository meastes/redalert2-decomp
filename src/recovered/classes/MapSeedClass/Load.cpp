#include "shared/map_seed.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/ini_class.hpp"
#include "shared/debug_printf.hpp"
extern "C" wchar_t *__cdecl wcscpy(wchar_t *,wchar_t const *);
bool MapSeedClass::Load(char const *filename)
{
    if(filename) {
        Debug_Printf("Loading random map: %s\n",filename);
        CCFileClass file(filename);
        INIClass ini;
        if(ini.Load(file,false)) {
            wchar_t *description=DescriptionBuffer;
            ini.Get_Unicode("RandomMap","Description",StringTable::Load_String("TXT_RANDOM_MAP_DESCRIPTION",0,"D:\\Ra2\\MapGen.cpp",0x149B),description,128);
            Width=ini.Get_Int("RandomMap","Width",Width);
            Height=ini.Get_Int("RandomMap","Height",Height);
            NumPlayers=ini.Get_Int("RandomMap","NumPlayers",NumPlayers);
            Seed=ini.Get_Int("RandomMap","Seed",Seed);
            MapType=ini.Get_Int("RandomMap","MapType",MapType);
            Theater=ini.Get_Int("RandomMap","Theater",Theater);
            Time=ini.Get_Int("RandomMap","Time",Time);
            RegionSize=ini.Get_Int("RandomMap","RegionSize",RegionSize);
            Ruggedness=ini.Get_Int("RandomMap","Ruggedness",Ruggedness);
            Accessibility=ini.Get_Int("RandomMap","Accessibility",Accessibility);
            WaterAmount=ini.Get_Int("RandomMap","WaterAmount",WaterAmount);
            Tiberium=ini.Get_Int("RandomMap","Tiberium",Tiberium);
            TiberiumLayout=ini.Get_Int("RandomMap","TiberiumLayout",TiberiumLayout);
            Vegetation=ini.Get_Int("RandomMap","Vegetation",Vegetation);
            UrbanPresence=ini.Get_Int("RandomMap","UrbanPresence",UrbanPresence);
            Resources=ini.Get_Int("RandomMap","Resources",Resources);
            return true;
        }
    }
    return false;
}
