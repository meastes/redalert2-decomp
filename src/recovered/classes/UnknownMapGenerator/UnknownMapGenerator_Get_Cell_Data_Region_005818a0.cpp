#include "shared/unknown_map_generator.hpp"
#include "shared/map_region.hpp"
// The region of a cell's generator record, or -1 without generator data (TibSun
// MapGeneratorClass::Get_Cell_Data_Region over CellData::Get_Region; ECX owner not read, RET4).
int UnknownMapGenerator::Get_Cell_Data_Region_005818a0(Cell const &cell)
{
    return UnknownMapGenerationCell::Get_Region(cell);
}
