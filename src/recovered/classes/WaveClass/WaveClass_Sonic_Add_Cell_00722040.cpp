// void WaveClass::Sonic_Add_Cell(Cell const &cell) (0x00722040)
// Adds the cell to the wave's cells unless it is already in the list.
#include "shared/wave.hpp"
#include "shared/cell.hpp"

void WaveClass::Sonic_Add_Cell(Cell const &cell)
{
    CellClass *where = &Map[cell];
    if (Cells.ID(where) == -1) {
        Cells.Add(where);
    }
}
