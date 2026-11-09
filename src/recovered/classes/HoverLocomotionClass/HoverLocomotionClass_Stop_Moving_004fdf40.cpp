// void __stdcall HoverLocomotionClass::Stop_Moving() (0x004FDF40, ILocomotion 18)
// Drops a destination that differs from the head-to coordinate.
#include "shared/hover_locomotion.hpp"
// @ref _unknown_geometry_l=00a41080

void __stdcall HoverLocomotionClass::Stop_Moving()
{
    if (unknown_18 != unknown_24) {
        unknown_18 = unknown_geometry_l;
        LinkedTo->unknown_4d0 = -1;
    }
}
