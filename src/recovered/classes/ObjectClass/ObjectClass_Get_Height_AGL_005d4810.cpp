// int ObjectClass::Get_Height_AGL() const (0x005D4810)
#include "shared/object_type.hpp"
#include "shared/map_view.hpp"
// @ref ?Map@@3VMouseClass@@A=008324e0
// @ref ?Get_Height_GL@MapClass@@QAEHABUCoordStruct@@@Z=0055a280

int ObjectClass::Get_Height_AGL() const
{
    CoordStruct coord = Coord;
    int height = Coord.Z - Map.Get_Height_GL(coord);
    if (unknown_79) {
        height -= unknown_geometry_j;
    }
    return height;
}
