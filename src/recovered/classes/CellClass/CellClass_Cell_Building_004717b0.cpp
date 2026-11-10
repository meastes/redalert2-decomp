// ObjectClass *CellClass::Cell_Building() const (0x004717B0): the building
// in the cell (Find_Object kind 6).
#include "shared/cell.hpp"
// @ref ?unknown_flag_a40958@@3_NA=00a40958

ObjectClass *CellClass::Cell_Building() const
{
    return Find_Object(6);
}
