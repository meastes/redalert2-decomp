#include "shared/radar.hpp"
#include "shared/map_region.hpp"
extern DynamicVectorClass<MapRegionClass *> MapRegionClass_MapRegions;
// @ref ?MapRegionClass_MapRegions@@=00a6fd28
// @ref ?unknown_map_region_next_index_a70a28@@=00a70a28
void __fastcall Clear_Map_Regions_0056da00()
{
    unknown_map_region_next_index_a70a28=0;
    for (int index=(int)((unsigned)MapRegionClass_MapRegions.Count()-1u);index>=0;--index) {
        delete MapRegionClass_MapRegions[index];
    }
}
