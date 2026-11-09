// void __stdcall WalkLocomotionClass::Stop_Moving() (0x0071E110, ILocomotion 18)
// Drops the destination; without a head-to coordinate the walk ends and
// the linked object is told (slot 308).
#include "shared/walk_locomotion.hpp"
// @ref _unknown_geometry_l=00af9060

void __stdcall WalkLocomotionClass::Stop_Moving()
{
    unknown_1c = unknown_geometry_l;
    if (unknown_28 == unknown_geometry_l) {
        unknown_34 = false;
        unknown_36 = false;
        LinkedTo->unknown_slot308();
    }
}
