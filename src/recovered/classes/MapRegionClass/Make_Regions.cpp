#include "shared/map_region.hpp"
bool MapRegionClass::Make_Regions_00570810()
{
    bool success=RandomMapGen.Expand_All_High_Ground();
    if(RMGCellData) {
        int remaining=(int)((unsigned)MapCellStride*(unsigned)MapCellStride);
        for(int index=0;remaining>0;--remaining,++index) {
            RMGCellData[index].RegionIndex=-1;
            RMGCellData[index].unknown_3c=-1;
        }
    }
    for(int reverse=(int)((unsigned)MapRegionClass_MapRegions.Count()-1u);reverse>=0;--reverse)delete MapRegionClass_MapRegions[reverse];
    unknown_map_region_next_index_a70a28=0;
    int remaining=(int)((unsigned)MapCellStride*(unsigned)MapCellStride);
    for(int index=0;remaining>0;--remaining,++index) {
        if(RMGCellData[index].RegionIndex==-1 && (unsigned char)RMGCellData[index].Position.Not_Equal_0058db50(Cell(0,0))) {
            MapRegionClass *region=Create_Region_From_Cell(&Map[RMGCellData[index].Position]);
            if(region)region->unknown_1a=false;
        }
    }
    if(success) {
        int index;
        for(index=0;index<MapRegionClass_MapRegions.Count();++index)MapRegionClass_MapRegions[index]->Collect_Neighbor_Regions_005709c0();
        for(index=0;index<MapRegionClass_MapRegions.Count();++index)MapRegionClass_MapRegions[index]->Make_Region_Connections_00571ed0();
        for(index=0;index<MapRegionClass_MapRegions.Count();++index) {
            delete MapRegionClass_MapRegions[index]->NeighborRegionIndices;
            MapRegionClass_MapRegions[index]->NeighborRegionIndices=0;
        }
    }
    return true;
}
