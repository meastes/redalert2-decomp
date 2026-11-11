// Behaviour rows (Scout1208, native M27; NOT EXACT, native/behaviour_rewrites.json).
#define VECTOR_IMPLEMENT_ALL
#include "shared/vector_class.hpp"
#include "shared/rect.hpp"
#include "shared/unknown_map_generator.hpp"
#include "shared/ccini_class.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/building_type_class.hpp"

extern "C" char *__cdecl strtok(char *string, char const *delimiters);

namespace {
// The ore patch lamp lists: the entry's comma separated building type IDs (Find_Or_Make;
// 0 is not added), the list left empty when the entry is missing (the original expands the
// TypeListReader shape inline twice, 0x0050E9A0 Get_String, strtok, 0x0045E0E0).
void Read_Building_List_m27(CCINIClass &ini, char const *entry, TypeList<BuildingTypeClass const *> &list)
{
    char buffer[128];
    if (ini.Get_String("General", entry, "", buffer, sizeof(buffer))) {
        for (char *token = strtok(buffer, ","); token != 0 && *token != '\0'; token = strtok(0, ",")) {
            BuildingTypeClass *type = BuildingTypeClass::Find_Or_Make(token);
            if (type != 0) {
                list.Add(type);
            }
        }
    }
}
}

// 0x00579960 MapGeneratorClass::Read_Settings (pass-6a decompile 00579960; strings from
// game.exe 0x007E1738-0x007E1884): RMG.INI's [General] lists and limits for the random
// map generator. The lists are cleared, then each is read with CCINIClass::Get_Integer_List
// (0x0046B1B0) over the cleared list, RMGMinimum/MaximumTiberium and MaxTrees with Get_Int
// (0x0050D7F0), and the two ore patch lamp lists as building type IDs. Reached by the skirmish
// "Random Map" button (MapSeedClass::Randomize_Parameters 0x005789F0). NOT EXACT: Get_Int's
// defaults are not visible in the decompile; the current values are kept when an entry is
// missing.
void MapGeneratorClass::Read_Settings_00579960()
{
    CCFileClass file("RMG.INI");
    CCINIClass *ini = new CCINIClass;
    ini->Load(file, false);

    RMGLevelLightSettings.Clear();
    RMGVegetationMinimums.Clear();
    RMGVegetationMaximums.Clear();
    TemperateOrePatchLamps.Clear();
    SnowOrePatchLamps.Clear();
    TemperateAmbientLight.Clear();
    SnowAmbientLight.Clear();

    RMGMinimumTiberium = ini->Get_Int("General", "RMGMinimumTiberium", RMGMinimumTiberium);
    RMGMaximumTiberium = ini->Get_Int("General", "RMGMaximumTiberium", RMGMaximumTiberium);

    RMGLevelLightSettings = ini->Get_Integer_List("General", "RMGLevelLightSettings", RMGLevelLightSettings);
    RMGVegetationMinimums = ini->Get_Integer_List("General", "RMGVegetationMinimums", RMGVegetationMinimums);
    RMGVegetationMaximums = ini->Get_Integer_List("General", "RMGVegetationMaximums", RMGVegetationMaximums);
    TemperateAmbientLight = ini->Get_Integer_List("General", "TemperateAmbientLight", TemperateAmbientLight);
    SnowAmbientLight = ini->Get_Integer_List("General", "SnowAmbientLight", SnowAmbientLight);
    TemperateAmbientRed.Clear();
    TemperateAmbientRed = ini->Get_Integer_List("General", "TemperateAmbientRed", TemperateAmbientRed);
    TemperateAmbientGreen.Clear();
    TemperateAmbientGreen = ini->Get_Integer_List("General", "TemperateAmbientGreen", TemperateAmbientGreen);
    TemperateAmbientBlue.Clear();
    TemperateAmbientBlue = ini->Get_Integer_List("General", "TemperateAmbientBlue", TemperateAmbientBlue);
    SnowAmbientRed.Clear();
    SnowAmbientRed = ini->Get_Integer_List("General", "SnowAmbientRed", SnowAmbientRed);
    SnowAmbientGreen.Clear();
    SnowAmbientGreen = ini->Get_Integer_List("General", "SnowAmbientGreen", SnowAmbientGreen);
    SnowAmbientBlue.Clear();
    SnowAmbientBlue = ini->Get_Integer_List("General", "SnowAmbientBlue", SnowAmbientBlue);

    MaxTrees = ini->Get_Int("General", "MaxTrees", MaxTrees);
    Read_Building_List_m27(*ini, "TemperateOrePatchLamps", TemperateOrePatchLamps);
    Read_Building_List_m27(*ini, "SnowOrePatchLamps", SnowOrePatchLamps);

    delete ini;
}

// The lamp lists' Add (no other unit instantiates DynamicVectorClass<BuildingTypeClass const *>;
// weak, an owner unit's definition wins).
template class DynamicVectorClass<BuildingTypeClass const *>;

// 0x00482100 (pass-6a decompile): the random map dialog's progress callback (Do_Random_Map_Dialog
// 0x00577490 stores it in rmg_callback_7e09c4): runs the per-frame service 0x00481FB0 and never
// asks the generator to stop.
void unknown_481fb0();
bool Default_Map_Generation_Callback_00482100()
{
    unknown_481fb0();
    return false;
}

// 0x007E09C4 (Scout1281): the generator's progress callback, two names in the tree:
// Do_Random_Map_Dialog 0x00577490 and Do_Random_Map 0x00577880 store it as rmg_callback_7e09c4,
// Generate_Random_Map 0x0057A0D0 calls it as unknown_map_generation_callback_7e09c4
// (native/data/address_aliases.tsv binds that name here). Both had been separate zero-filled
// link-closure holes, so the generator never called the dialog's callback. Initial value as in
// game.exe's .data: 0x00482100, Default_Map_Generation_Callback_00482100.
extern "C" bool (*ra2_rmg_callback_7e09c4)() __asm__("_rmg_callback_7e09c4");
bool (*ra2_rmg_callback_7e09c4)() = Default_Map_Generation_Callback_00482100;
