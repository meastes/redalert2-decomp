#include "shared/map_generation_path.hpp"
#include "shared/tileset_index.hpp"
extern int IsometricTileTypeClass_DirtRoadSlopes;
// @ref ?IsometricTileTypeClass_DirtRoadCurve@@=00a6e014
// @ref ?IsometricTileTypeClass_DirtRoadSlopes@@=00a6dd04
extern DirtRoadTile unknown_generation_paths_a700a0[109];
// @ref ?unknown_generation_paths_a700a0@@=00a700a0
// The original109-record array/12-byte stride is evidenced by full initializer/cleanup.
// Its source name and this cdecl/void operation spelling remain hypotheses.
void Init_Dirt_Roads()
{
    int i;
    for(i=0;i<101;i++)
        unknown_generation_paths_a700a0[i].unknown_08=(int)((unsigned)IsometricTileTypeClass_DirtRoadCurve+(unsigned)i);
    for(i=0;i<8;i++)
        unknown_generation_paths_a700a0[101+i].unknown_08=(int)((unsigned)IsometricTileTypeClass_DirtRoadSlopes+(unsigned)i);
}
