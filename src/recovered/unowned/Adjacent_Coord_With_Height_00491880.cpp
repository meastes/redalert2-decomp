// CoordStruct __fastcall Adjacent_Coord_With_Height(CoordStruct const &coord, int facing) -- 0x00491880
// TibSun coord.cpp: the coordinate one cell toward a facing (the AdjacentPoint offsets at
// 0x00851AC0), its height moved by the ground-height difference between the two places.
#include "shared/coord.hpp"
#include "shared/map_view.hpp"
// @ref ?Map@@3VMouseClass@@A=008324e0
// @ref ?Get_Height_GL@MapClass@@QAEHABUCoordStruct@@@Z=0055a280

CoordStruct __fastcall Adjacent_Coord_With_Height(CoordStruct const &coord, int facing)
{
    CoordStruct newcoord = (coord + unknown_adjacent_offsets_00851ac0[facing]);
    newcoord.Z += Map.Get_Height_GL(newcoord) - Map.Get_Height_GL(coord);
    return newcoord;
}
