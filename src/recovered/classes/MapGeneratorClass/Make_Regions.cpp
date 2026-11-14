#include "shared/unknown_map_generator.hpp"
#include "shared/map.hpp"
// These no-argument operations have no established incoming receiver.
void unknown_005704c0();
void unknown_00570810();
// @ref ?unknown_005704c0@@YAXXZ=005704c0
// @ref ?unknown_00570810@@YAXXZ=00570810
// @ref ?unknown_00582fd0@MapGeneratorClass@@=00582fd0
// @ref ?Fixup_Cliff_Corners@MapGeneratorClass@@=005831c0
// @ref ?Build_All_Cliffs@MapClass@@=0055b060
void MapGeneratorClass::Make_Regions()
{
    if (SeedData.MapType==4 || SeedData.MapType==3) {
        unknown_005704c0();
        unknown_00570810();
        Fixup_Cliff_Corners();
        Map.Build_All_Cliffs(0,-1);
        unknown_00582fd0();
    }
}
