// void __stdcall DropPodLocomotionClass::Move_To(CoordStruct to) (0x004A5FC0, ILocomotion 17)
// A pod without a destination lands at this one (on the ground): it starts
// DropPodHeight above it, DropPodHeight / tan(DropPodAngle) to the east, or
// as far west when that start is off the map (+0x1C). Placed there facing
// south, it gets an atmosphere entry animation.
#include "shared/droppod_locomotion.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/rules.hpp"
#include "shared/anim.hpp"
// @ref ?Map@@3VMouseClass@@A=008324e0
// @ref _unknown_geometry_l=00852a20

void __stdcall DropPodLocomotionClass::Move_To(CoordStruct to)
{
    if (unknown_20 == unknown_geometry_l) {
        unknown_20 = CoordStruct(to.X, to.Y, to.Z);
        unknown_20.Z = Map.Get_Height_GL(unknown_20);
        CoordStruct start = unknown_20;
        start.Z += Rules->DropPodHeight;
        start.X = (int)(start.X + Rules->DropPodHeight / unknown_math_4ba520(Rules->DropPodAngle));
        if (!Map.Unknown_0055A7F0(start)) {
            unknown_1c = true;
            start.X = (int)(unknown_20.X - Rules->DropPodHeight / unknown_math_4ba520(Rules->DropPodAngle));
        }
        LinkedTo->Set_Coord(start);
        if (LinkedTo->Unlimbo(&start, 0x80)) {
            LinkedTo->Set_Coord(start);
            LinkedTo->unknown_2e0.Set_Current(UnknownFacingDir(0x8000));
            new AnimClass(Rules->AtmosphereEntry, start, 0, 1, 0x600, 0);
        } else {
            LinkedTo->Unlimbo(&start, 0x80);
        }
    }
}
