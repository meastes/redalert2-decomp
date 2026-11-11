// void FootClass::Set_Coord(CoordStruct const &coord) (0x004CA160)
// ObjectClass's slot 103, bracketed by slot 71 (false, then true) while
// the object is on the map (+0x74).
#include "shared/foot.hpp"
// @ref ?Set_Coord@ObjectClass@@UAEXABUCoordStruct@@@Z=005d51c0

void FootClass::Set_Coord(CoordStruct const &coord)
{
    if (unknown_74) {
        Mark(0);
        ObjectClass::Set_Coord(coord);
        Mark(1);
    } else {
        ObjectClass::Set_Coord(coord);
    }
}
