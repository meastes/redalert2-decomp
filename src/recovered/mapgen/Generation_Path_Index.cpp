#include "shared/map_generation_path.hpp"
#include "shared/tileset_index.hpp"
extern int IsometricTileTypeClass_DirtRoadSlopes;
// Original names and free fastcall/return-signedness source intent are hypotheses.
int __fastcall Generation_Path_Index_0058d9a0(int tile)
{
    if(tile>=IsometricTileTypeClass_DirtRoadCurve && tile<(int)((unsigned)IsometricTileTypeClass_DirtRoadCurve+101u))
        return tile-IsometricTileTypeClass_DirtRoadCurve;
    if(tile>=IsometricTileTypeClass_DirtRoadSlopes && tile<(int)((unsigned)IsometricTileTypeClass_DirtRoadSlopes+8u))
        return tile-IsometricTileTypeClass_DirtRoadSlopes+101;
    return 0;
}
