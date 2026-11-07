#include "shared/unknown_map_generator.hpp"
#include "shared/map_region.hpp"
// Sets the region of a cell's generator record (TibSun MapGeneratorClass::Set_Cell_Data_Region
// over CellData::Set_Region 0x56E970; ECX owner not read, RET8).
void UnknownMapGenerator::Set_Cell_Data_Region_00581870(Cell const &cell,int region_id)
{
    Set_Cell_Region_Index_0056e970(cell,region_id);
}
