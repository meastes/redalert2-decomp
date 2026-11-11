// Cell const *ObjectClass::Occupy_List(int arg1) (0x005D4470)
#include "shared/object_type.hpp"
// @ref _?$S40@?1??Occupy_List@ObjectClass@@UAEPBVCell@@H@Z@4EA=00a72f0c
// (the guard's number follows the unit's static-initializer count: 37 in the
// full Object.cpp unit, 1 when compiled alone)
// @ref _?$S1@?1??Occupy_List@ObjectClass@@UAEPBVCell@@H@Z@4EA=00a72f0c
// @ref _?unknown_none@?1??Occupy_List@ObjectClass@@UAEPBVCell@@H@Z@4UUnknownStaticCell@@B=00a72f18

Cell const *ObjectClass::Occupy_List(int arg1)
{
    static UnknownStaticCell const unknown_none(Cell(0x7FFF, 0x7FFF));
    if (Class_Of() == 0) {
        return &unknown_none.Cell;
    }
    return Class_Of()->Occupy_List(arg1);
}
