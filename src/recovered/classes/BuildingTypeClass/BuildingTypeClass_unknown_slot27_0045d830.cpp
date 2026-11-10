// CoordStruct BuildingTypeClass::unknown_slot27(CoordStruct const &coord) const (0x0045D830)
// The coordinate placed on the ground.
#include "shared/building_type.hpp"
#include "shared/map_view.hpp"
// @ref ?Map@@3VMouseClass@@A=008324e0
// @ref ?Get_Height_GL@MapClass@@QAEHABUCoordStruct@@@Z=0055a280

CoordStruct BuildingTypeClass::unknown_slot27(CoordStruct const &coord) const
{
    CoordStruct result = coord;
    result.Z = Map.Get_Height_GL(result);
    return result;
}
