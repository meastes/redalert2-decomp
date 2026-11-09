// void CellClass::Set_CellID(Cell const &cell) (0x0047A450)
#include "shared/cell_class.hpp"

void CellClass::Set_CellID(Cell const &cell)
{
    MapCoords = cell;
}
