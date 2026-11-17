#include "shared/map.hpp"
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#define VECTOR_IMPLEMENT_DYNAMIC_DELETE_OBJECT
#define VECTOR_IMPLEMENT_DYNAMIC_DELETE_INDEX
#include "shared/map_region.hpp"
#include "shared/map_cell.hpp"
#include "shared/unknown_map_generator.hpp"
#include "shared/map_view.hpp"
extern DynamicVectorClass<MapRegionClass *> MapRegionClass_MapRegions;

#include "shared/unknown_map_generator.hpp"
#include "shared/map_view.hpp"
bool MapGeneratorClass::Expand_All_High_Ground()
{
    bool success=true;
    Map.Set_Cursor_Shape(0);
    Map.Reset_Iterator();
    CellClass *cell=Map.Iterate();
    while (cell!=0) {
        if (!success) break;
        success=Map.Expand_High_Ground(cell,-1);
        cell=Map.Iterate();
    }
    return success;
}

#include "shared/map_region.hpp"
void MapRegionClass::Update_Cell_Marks_005707a0()
{
    int count = (int)((unsigned)MapCellStride * (unsigned)MapCellStride);
    for (int i = 0; i < count; ++i) RMGCellData[i].Marked = 0;
    Reassign_Border_Cells(8, -2);
    for (int j = 0; j < count; ++j) {
        UnknownMapGenerationCell &data=RMGCellData[j];
        if (data.RegionIndex == Index) RMGCellData[j].Marked = 1;
        else if (data.RegionIndex == -2) RMGCellData[j].RegionIndex = Index;
    }
}

