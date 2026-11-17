#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#define VECTOR_IMPLEMENT_DYNAMIC_ADD
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#include "shared/map_region.hpp"
void MapRegionClass::Collect_Neighbor_Regions_005709c0()
{
    NeighborRegionIndices=new DynamicVectorClass<int>;
    unsigned char *seen=new unsigned char[unknown_map_region_next_index_a70a28];
    int index;
    for(index=0;index<unknown_map_region_next_index_a70a28;index++)
        seen[index]=0;
    DynamicVectorClass<Cell> *boundary=Collect_Boundary_Cells_0056ed10();
    for(index=0;index<boundary->Count();index++) {
        Cell cell=(*boundary)[index];
        for(int direction=0;direction<8;direction++) {
            Cell next=cell+unknown_adjacent_851a70[direction&7];
            if(next.Y+next.X>MapRegionClass_MapStartDiagonal && next.X-next.Y<MapRegionClass_MapStartDiagonal &&
               next.Y-next.X<MapRegionClass_MapStartDiagonal && next.Y+next.X<=MapRegionClass_MapEndDiagonal) {
                if(RMGCellData) {
                    int regionIndex=RMGCellData[next.Y*MapCellStride+next.X].RegionIndex;
                    if(regionIndex>=0) seen[regionIndex]=1;
                }
            }
        }
    }
    for(index=0;index<unknown_map_region_next_index_a70a28;index++)
        if(seen[index] && index!=Index) NeighborRegionIndices->Add(index);
    CellCount=0;
    int totalCells=MapCellStride*MapCellStride;
    for(index=0;index<totalCells;index++) {
        if(RMGCellData[index].RegionIndex==Index &&
           RMGCellData[index].Position!=Cell(0,0)) CellCount++;
    }
    delete boundary;
    delete[] seen;
}
