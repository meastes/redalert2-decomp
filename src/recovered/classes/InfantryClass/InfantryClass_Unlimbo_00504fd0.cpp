// bool InfantryClass::Unlimbo(CoordStruct const *coord, int dir) (0x00504FD0, slot 53)
// Puts the infantry on the map: on the ground it moves to a free spot of
// the cell (alternate spots when 0x00A40774 is set or 0x0055A660 refuses the
// cell) and fails when there is none. After FootClass's slot 53, types
// without Sight clear +0x373, the spot below bridge height is occupied
// (slot 58) and +0x5D0 becomes 2.
#include "shared/infantry.hpp"
#include "shared/map_view.hpp"
#include "shared/techno_map.hpp"
// @ref ?Map@@3VMouseClass@@A=008324e0
// @ref ?Get_Height_GL@MapClass@@QAEHABUCoordStruct@@@Z=0055a280
// @ref ?Unknown_004764B0@UnknownCellClass@@QAE?AUCoordStruct@@ABU2@H_N@Z=004764b0
// @ref ?unknown_a40774@@3HA=00a40774
// @ref ?Unlimbo@FootClass@@UAE_NPBUCoordStruct@@H@Z=004c5e40
// @ref _unknown_geometry_l=00a410f8
// @ref _unknown_geometry_j=00a41120

bool InfantryClass::Unlimbo(CoordStruct const *coord, int dir)
{
    int ground = Map.Get_Height_GL(*coord);
    CoordStruct where = *coord;
    if (where.Z == ground) {
        int alternate = unknown_a40774 != 0 || !Map.Unknown_0055A660(Coord_Cell(*coord), true);
        where = Map_Cell(*coord)->Unknown_004764B0(*coord, alternate, false);
        if (where.X == unknown_geometry_l.X && where.Y == unknown_geometry_l.Y
            && where.Z == unknown_geometry_l.Z) {
            return false;
        }
        where.Z = ground;
    }
    if (FootClass::Unlimbo(&where, dir)) {
        if (Type->Sight == 0) {
            unknown_373 = false;
        }
        if (where.Z <= unknown_geometry_j + ground) {
            Set_Occupy_Bit(where);
        }
        unknown_5d0 = 2;
        return true;
    }
    return false;
}
