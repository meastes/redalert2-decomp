// void DisplayClass::Unknown_0049E5E0() (0x0049E5E0)
// Runs 0x004798F0 on every cell of the map.
#include "shared/display.hpp"
#include "shared/map_cell.hpp"
#include "shared/map_view.hpp"
// @ref ?Map@@3VMouseClass@@A=008324e0
// @ref ?Iterate@MapClass@@QAEPAVCellClass@@XZ=0055a490
// @ref ?Reset_Iterator@MapClass@@QAEXXZ=0055a550
// @ref ?Unknown_004798F0@CellClass@@QAEXXZ=004798f0

void DisplayClass::Unknown_0049E5E0()
{
    Map.Reset_Iterator();
    for (CellClass *cell = Map.Iterate(); cell; cell = Map.Iterate()) {
        cell->Unknown_004798F0();
    }
}
