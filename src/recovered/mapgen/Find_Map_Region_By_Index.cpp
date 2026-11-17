#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/map_region.hpp"
extern DynamicVectorClass<MapRegionClass *> MapRegionClass_MapRegions;
// @ref ?MapRegionClass_MapRegions@@3V?$DynamicVectorClass@PAVMapRegionClass@@@@A=00a6fd28
MapRegionClass *__fastcall Find_Map_Region_By_Index_00575ce0(int index)
{
    for(int i=0;i<MapRegionClass_MapRegions.Count();i++)
        if(MapRegionClass_MapRegions[i]->Index==index)
            return MapRegionClass_MapRegions[i];
    return 0;
}
