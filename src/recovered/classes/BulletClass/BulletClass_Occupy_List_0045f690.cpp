// Cell const *BulletClass::Occupy_List(int arg1) (0x0045F690)
// Always the "no cell" value (0x7FFF, 0x7FFF), a function-local static like
// ObjectClass::Occupy_List's.
#include "shared/bullet.hpp"

Cell const *BulletClass::Occupy_List(int arg1)
{
    static UnknownStaticCell const unknown_none(Cell(0x7FFF, 0x7FFF));
    return &unknown_none.Cell;
}
