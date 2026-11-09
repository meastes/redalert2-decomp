// void __stdcall WalkLocomotionClass::Force_Immediate_Destination(CoordStruct coord) (0x0071E1A0, ILocomotion 30)
// Sets the head-to coordinate; with neither it nor a destination the walk
// ends (slot 308 when it was moving).
#include "shared/walk_locomotion.hpp"
// @ref _unknown_geometry_l=00af9060

void __stdcall WalkLocomotionClass::Force_Immediate_Destination(CoordStruct coord)
{
    bool moving = unknown_34;
    unknown_28 = CoordStruct(coord.X, coord.Y, coord.Z);
    if (unknown_28 == unknown_geometry_l && unknown_1c == unknown_geometry_l) {
        unknown_34 = false;
        if (moving) {
            LinkedTo->unknown_slot308();
        }
    }
}
