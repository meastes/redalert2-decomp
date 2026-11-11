// CoordStruct TerrainTypeClass::Coord_Fixup(CoordStruct const &coord) const (0x006E5850)
// The coordinate, raised to the ground height when below it.
#include "shared/terrain_type.hpp"
#include "shared/map_view.hpp"
// @ref ?Get_Height_GL@MapClass@@QAEHABUCoordStruct@@@Z=0055a280

CoordStruct TerrainTypeClass::Coord_Fixup(CoordStruct const &coord) const
{
    CoordStruct result = coord;
    if (result.Z < Map.Get_Height_GL(result)) {
        result.Z = Map.Get_Height_GL(result);
    }
    return result;
}
