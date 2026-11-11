// CoordStruct UnitTypeClass::Coord_Fixup(CoordStruct const &coord) const (0x0070B320)
// The coordinate, raised to the ground height when below it.
#include "shared/unit_type.hpp"
#include "shared/map_view.hpp"
// @ref ?Map@@3VMouseClass@@A=008324e0
// @ref ?Get_Height_GL@MapClass@@QAEHABUCoordStruct@@@Z=0055a280

CoordStruct UnitTypeClass::Coord_Fixup(CoordStruct const &coord) const
{
    CoordStruct result = coord;
    if (result.Z < Map.Get_Height_GL(result)) {
        result.Z = Map.Get_Height_GL(result);
    }
    return result;
}
