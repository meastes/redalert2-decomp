#include "shared/unknown_map_generator.hpp"
#include "shared/map_region.hpp"
#include "shared/cell.hpp"
#include "shared/map_view.hpp"
// @ref ?Map@@3VMouseClass@@A=008324e0
// @ref ?Iterate@MapClass@@QAEPAVCellClass@@XZ=0055a490
// @ref ?Reset_Iterator@MapClass@@QAEXXZ=0055a550
// @ref ?unknown_map_cells_a70a24@@=00a70a24
// @ref ?unknown_map_cell_side_84e804@@=0084e804
void MapGeneratorClass::Clear_Region_Cells(int source_index,int target_index,int level,int tile)
{
    Map.Reset_Iterator();
    for(CellClass *cell=Map.Iterate();cell;cell=Map.Iterate()) {
        Cell position=cell->MapCoords;
        UnknownMapGenerationCell &target=unknown_map_cells_a70a24[position.Y*unknown_map_cell_side_84e804+position.X];
        if(target.RegionIndex==source_index) {
            target.RegionIndex=target_index;
            target.unknown_4b=0;
            cell->IsoTileIndex=tile;
            cell->unknown_11a=0;
            cell->unknown_11b=(char)(level==-1 ? CellHeight:level);
        }
    }
}
