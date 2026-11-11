// void ObjectClass::Set_Height_AGL(int height) (0x005D4860)
#include "shared/object_type.hpp"
#include "shared/map_view.hpp"
// @ref ?Map@@3VMouseClass@@A=008324e0
// @ref ?Get_Height_GL@MapClass@@QAEHABUCoordStruct@@@Z=0055a280

void ObjectClass::Set_Height_AGL(int height)
{
    if (unknown_79) {
        height += unknown_geometry_j;
    }
    if (unknown_74) {
        Mark(0);
        CoordStruct coord = Coord;
        Coord.Z = Map.Get_Height_GL(coord) + height;
        Mark(1);
    } else {
        CoordStruct coord = Coord;
        Coord.Z = Map.Get_Height_GL(coord) + height;
    }
}
