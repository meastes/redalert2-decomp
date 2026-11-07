// 0056E970: assigns a map-generation grid cell to a region; does nothing without the grid
// (TibSun MapRegionClass::CellData::Set_Region). Physical contract: ECX -> signed16 X/Y,
// EDX -> raw32 stored at grid+38, RET0. Defined in the map generator unit so that its
// callers there may inline it (/Ob2); 33 ramp calls stay out of line.
#include "shared/map_region.hpp"
// @ref ?unknown_map_cell_side_84e804@@3HA=0084e804
// @ref ?unknown_map_cells_a70a24@@3PAUUnknownMapGenerationCell@@A=00a70a24
void __fastcall Set_Cell_Region_Index_0056e970(Cell const &position, int index)
{
    if (unknown_map_cells_a70a24) {
        UnknownMapGenerationCell &data = MapRegionClass::Get_Cell_Data(position);
        data.RegionIndex = index;
    }
}
