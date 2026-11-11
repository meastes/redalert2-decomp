// void JumpjetLocomotionClass::Move_To(CoordStruct to) (0x0052F600)
#include "shared/jumpjet_locomotion.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/cell.hpp"
// @ref ?Nearby_Location@MapClass@@QAE?AVCell@@ABV2@HHH_NUPoint2D@@HHHH0H@Z=005502e0
// @ref _unknown_geometry_l=00a6e3e0

void __stdcall JumpjetLocomotionClass::Move_To(CoordStruct to)
{
    if (unknown_3c != unknown_geometry_l && unknown_4c != 0 && unknown_88) {
        LinkedTo->Clear_Occupy_Bit(unknown_3c);
        unknown_88 = false;
    }
    unknown_3c = CoordStruct(to.X, to.Y, to.Z);
    if (to != unknown_geometry_l) {
        Cell cell;
        if (LinkedTo->Kind_Of() == 1 && !((UnitClass *)LinkedTo)->Type->BalloonHover) {
            cell = Map.Nearby_Location(Coord_Cell(to), LinkedTo->Techno_Type_Class()->SpeedType, -1, 9, false, Point2D(1, 1), 0, 0, 0, 0, Cell(0, 0), 0);
        } else {
            cell = Map.Nearby_Location(Coord_Cell(to), LinkedTo->Techno_Type_Class()->SpeedType, -1, 9, (Map[CoordStruct(to)].unknown_140 >> 8) & 1, Point2D(1, 1), 0, 0, 0, 1, Cell(0, 0), 0);
        }
        CoordStruct coord = unknown_5310a0(CoordStruct(cell));
        if (coord != unknown_geometry_l) {
            if (LinkedTo->Kind_Of() != 1 || (!LinkedTo->Techno_Type_Class()->BalloonHover && LinkedTo->MissionQueue != 7 && LinkedTo->Get_Mission() != 7)) {
                unknown_3c = coord;
            }
            if (LinkedTo->MissionQueue != 7 && LinkedTo->Get_Mission() != 7) {
                LinkedTo->unknown_494 = &Map[unknown_3c];
            }
            unknown_48 = true;
            if (unknown_4c == 4) {
                unknown_4c = 1;
                unknown_78 = Height;
            }
        }
    } else {
        unknown_48 = false;
    }
}

