// @ref ?Map@@3VMouseClass@@A=008324e0
// @ref ?Iterate@MapClass@@QAEPAVCellClass@@XZ=0055a490
// @ref ?Reset_Iterator@MapClass@@QAEXXZ=0055a550
// @ref ?unknown_map_cells_a70a24@@=00a70a24
// @ref ?unknown_map_cell_side_84e804@@=0084e804
#include "shared/map_region.hpp"
#include "shared/cell.hpp"
#include "shared/map_view.hpp"
#include "shared/unknown_map_generator.hpp"
// Clears every map cell's spread mark back to zero (TibSun MapGeneratorClass::Clear_Cell_Data_Spreads,
// a member in RA2's TibSun-ordered run 0x581870-0x582A50; never called, ECX not read).
void MapGeneratorClass::Clear_Cell_Data_Spreads()
{
    Map.Reset_Iterator();
    CellClass *cell=Map.Iterate();
    while(cell) {
        Set_Cell_Data_Spread(cell->Fetch_CellID(),0);
        cell=Map.Iterate();
    }
}
