// void __stdcall JumpjetLocomotionClass::Stop_Moving() (0x0052F960, ILocomotion 18)
// While moving (and 0x00A40774 clear): the target claim is released; the
// jumpjet then moves to the nearest free cell (0x005502E0 for its speed
// type and movement zone) at ground (bridge) height. With no free cell a
// living object is destroyed with the rules' C4 warhead and the target
// cleared.
#include "shared/jumpjet_locomotion.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/cell.hpp"
#include "shared/techno_type.hpp"
#include "shared/rules.hpp"
// @ref ?unknown_a40774@@3HA=00a40774
// @ref ?Nearby_Location@MapClass@@QAE?AVCell@@ABV2@HHH_NUPoint2D@@11110H@Z=005502e0
// @ref _unknown_geometry_l=00a6e3e0

void __stdcall JumpjetLocomotionClass::Stop_Moving()
{
    if (unknown_a40774 == 0 && unknown_48) {
        if (unknown_3c != unknown_geometry_l && unknown_4c != 0 && unknown_88) {
            LinkedTo->Clear_Occupy_Bit(unknown_3c);
            unknown_88 = false;
        }
        Cell here = Coord_Cell(Object_Coord(LinkedTo));
        Cell cell = Map.Nearby_Location(here, LinkedTo->Techno_Type_Class()->SpeedType, -1, LinkedTo->Techno_Type_Class()->MovementZone, false, Point2D(1, 1), 0, 0, 0, 1, Cell(0, 0), 0);
        if (cell != unknown_geometry_k) {
            CoordStruct coord = Cell_Coord(cell);
            coord.Z = Map.Get_Height_GL(coord);
            if (Map[cell].unknown_140 & 0x100) {
                coord.Z += unknown_geometry_j;
            }
            Move_To(CoordStruct(coord.X, coord.Y, coord.Z));
            return;
        }
        if (LinkedTo->Strength > 0) {
            LinkedTo->Take_Damage(&LinkedTo->Strength, 0, Rules->unknown_c74, 0, 1, 1);
            unknown_3c = unknown_geometry_l;
        }
    }
}
