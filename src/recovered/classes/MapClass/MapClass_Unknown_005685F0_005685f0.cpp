// void MapClass::Unknown_005685F0() (0x005685F0)
// Runs CellClass 0x0047BC80 on every cell whose +0x140 bit 1 is clear.
#include "shared/map.hpp"
#include "shared/map_cell.hpp"
// @ref ?unknown_47bc80@CellClass@@QAEXXZ=0047bc80

void MapClass::Unknown_005685F0()
{
    Reset_Iterator();
    for (CellClass *cell = Iterate(); cell != 0; cell = Iterate()) {
        if (!cell->IsFogMapped) {
            cell->unknown_47bc80();
        }
    }
}
