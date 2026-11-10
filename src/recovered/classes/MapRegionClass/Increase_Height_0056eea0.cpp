// void MapRegionClass::Increase_Height_0056eea0(char height) (0x0056EEA0, RET4): adds height to the map cell
// height of every cell owned by this region.
#include "shared/map_region.hpp"
#include "shared/cell.hpp"
#include "shared/map_view.hpp"

void MapRegionClass::Increase_Height_0056eea0(char height)
{
    Map.Reset_Iterator();
    CellClass *cell = Map.Iterate();
    while (cell != 0) {
        int region = UnknownMapGenerationCell::Get_Region(cell->Fetch_CellID());
        if (region == Index) {
            cell->unknown_11b += height;
        }
        cell = Map.Iterate();
    }
}
