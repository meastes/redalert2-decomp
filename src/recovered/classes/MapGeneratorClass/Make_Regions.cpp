#include "shared/unknown_map_generator.hpp"
#include "shared/map.hpp"
// These no-argument operations have no established incoming receiver.
void MapRegionClass_Make_Ice_Regions();
void unknown_00570810();
void MapGeneratorClass::Make_Regions()
{
    if (SeedData.MapType==4 || SeedData.MapType==3) {
        MapRegionClass_Make_Ice_Regions();
        unknown_00570810();
        Fixup_Cliff_Corners();
        Map.Build_All_Cliffs(0,-1);
        Vary_Cliff_Runs();
    }
}
