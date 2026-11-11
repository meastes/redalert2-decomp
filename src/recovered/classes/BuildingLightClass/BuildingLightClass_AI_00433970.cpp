// void BuildingLightClass::AI() (0x00433970, slot 23)
// Moves the light: behavior 3 drifts a quarter of the way toward the followed
// object while it is within the far range (else falls back to behavior 1),
// behavior 2 sweeps a full circle around the owner, behavior 1 swings back
// and forth within the rules' angle. A building owner in behavior 1 then
// springs its tag (event 0x23) when a non-allied object of kind 15 or 1 in
// the 3x3 cells around the light is within the light's radius. Lights whose
// owner is gone or inactive remove themselves (names are hypotheses).
// Statement shape adopted from the TibSun reconstruction's blight.cpp
// BuildingLightClass::AI (OpenTS; SPDX-License-Identifier: GPL-3.0-or-later;
// see THIRD_PARTY.md). RA2 differences, from the bytes: the cached Following,
// one Spring, case 3 after cases 1 and 2, and the follow test measuring
// Distance_YZX(Owner->Center_Coord()) into a local inside the target test (the
// owner's Center_Coord temporary in its own top frame slot 0xC4, the y, z, x
// sum as in the locomotors' Start_Of_Move). Needs RA2_BUILDING_LIGHT_DISTANCE,
// RA2_COORD_TPOINT3D, RA2_TPOINT3D_DIFFERENCE and RA2_CELL_OCCUPIER (defined
// at line 1 of the unit's first row 0x00432DD0).
#include "shared/building_light.hpp"
#include "shared/rules.hpp"
#include "shared/house.hpp"
#include "shared/map_cell_view.hpp"
#include "shared/matrix3d.hpp"
#include "shared/tag.hpp"
CoordStruct __fastcall Lerp(CoordStruct const &from, CoordStruct const &to, float t); // 0x007220C0 (Wave unit)
// @ref ?unknown_sqrt_4ba410@@YANN@Z=004ba410
// @ref ?Is_Ally@HouseClass@@QBE_NPBVTechnoClass@@@Z=004e5580
// @ref ?Lerp@@YI?AUCoordStruct@@ABU1@0M@Z=007220c0
// @ref ?Set_Behavior@BuildingLightClass@@QAEXH@Z=00434370
// @ref ?Make_Identity@Matrix3D@@QAEXXZ=00590060
// @ref ?Rotate_Z@Matrix3D@@QAEXM@Z=005909a0
// @ref ??D@YI?AUVector3@@ABVMatrix3D@@ABU0@@Z=00591380
// @ref ??0?$TPoint3D@H@@QAE@HHH@Z=0041b630
// @ref ?Spring@TagClass@@QAE_NHPAVObjectClass@@VCell@@_NH@Z=006b3df0
// @ref _unknown_geometry_k=0084ea08
// @ref _?_scale@?1??Detection_Radius@BuildingLightClass@@QBEHXZ@4NB=0079c7a8
// @ref __real@8@bffe8000000000000000=0079c9b0

void BuildingLightClass::AI()
{
    if (Owner == 0 || !Owner->unknown_7d) {
        Delete_Me();
        return;
    }

    CoordStruct coord;
    ObjectClass *following;
    switch (Behavior) {
    default:
        coord = Coord;
        break;

    case 1: {
        unknown_98 += unknown_b8;
        if (unknown_c0) {
            if (unknown_98 > Rules->SpotlightAngle / 2) {
                unknown_b8 -= Rules->SpotlightAcceleration;
                if (unknown_b8 < 0) {
                    unknown_b8 = 0;
                    unknown_c0 = false;
                }
            } else if (unknown_b8 < Rules->SpotlightSpeed) {
                unknown_b8 += Rules->SpotlightAcceleration;
            }
        } else {
            if (unknown_98 < Rules->SpotlightAngle / -2) {
                unknown_b8 += Rules->SpotlightAcceleration;
                if (unknown_b8 > 0.0) {
                    unknown_b8 = 0.0;
                    unknown_c0 = true;
                }
            } else if (-Rules->SpotlightSpeed < unknown_b8) {
                unknown_b8 -= Rules->SpotlightAcceleration;
            }
        }

        Matrix3D mtx;
        mtx.Make_Identity();
        mtx.Rotate_Z(unknown_98);

        Vector3 v(unknown_ac.X - unknown_a0.X, unknown_ac.Y - unknown_a0.Y, unknown_ac.Z - unknown_a0.Z);
        v = mtx * v;
        coord = CoordStruct(unknown_a0.X + v.X, unknown_a0.Y + v.Y, unknown_a0.Z + v.Z);
    }
    break;
    case 2: {
        CoordStruct owner_coord = Owner->Coord;
        unknown_98 += Rules->SpotlightSpeed * 4;

        if (unknown_98 > 6.283185307179586) {
            unknown_98 -= 6.283185307179586;
        }

        Matrix3D mtx;
        mtx.Make_Identity();
        mtx.Rotate_Z(unknown_98);

        Vector3 v(unknown_ac.X - owner_coord.X, unknown_ac.Y - owner_coord.Y, unknown_ac.Z - owner_coord.Z);
        v = mtx * v;
        coord = CoordStruct(owner_coord.X + v.X, owner_coord.Y + v.Y, owner_coord.Z + v.Z);
    }
    break;

    case 3:
        following = Following;
        if (following != 0 && following->unknown_7d) {
            int dist = following->Distance_YZX(Owner->Center_Coord());
            if (dist < Rules->unknown_6b0) {
                coord = Lerp(Position_Coord_004f3f40(), following->Position_Coord_004f3f40(), 0.25f);
                break;
            }
        }
        Set_Behavior(1);
        break;

    }

    Set_Coord(coord);

    BuildingClass *owner_building = (Owner->Kind_Of() == 6) ? (BuildingClass *)Owner : 0;

    if (Behavior == 1) {
        if (owner_building != 0 && owner_building->unknown_7d && owner_building->unknown_slot195() && owner_building->unknown_34 != 0) {
            bool found = false;
            Cell cell = unknown_slot104();
            int radius = Detection_Radius();
            int detection = radius + 30;
            HouseClass *house = Owner->House;

            for (int x = -1; x < 2; x++) {
                for (int y = -1; y < 2; y++) {
                    ObjectClass *occupier = Map[cell + Cell(x, y)].Cell_Occupier();
                    while (occupier != 0) {
                        TechnoClass *techno = (TechnoClass *)occupier;
                        if ((occupier->Kind_Of() == 0xf || occupier->Kind_Of() == 1) && !house->Is_Ally(techno)) {
                            int dist = occupier->Center_Coord().Distance_To(Position_Coord_004f3f40());
                            if (dist < detection) {
                                found = true;
                            }
                        }
                        occupier = occupier->unknown_30;
                    }
                }
            }

            if (found) {
                owner_building->unknown_34->Spring(0x23, owner_building, unknown_geometry_k);
            }
        }
    }
}
