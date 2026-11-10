// void MapClass::Unknown_00568650() (0x00568650)
// Runs CellClass 0x0047BE00 on every cell.
#include "shared/map.hpp"
#include "shared/map_cell.hpp"
// @ref ?unknown_47be00@CellClass@@QAEXXZ=0047be00

void MapClass::Unknown_00568650()
{
    Reset_Iterator();
    for (CellClass *cell = Iterate(); cell != 0; cell = Iterate()) {
        cell->unknown_47be00();
    }
}
