// void __stdcall MechLocomotionClass::Stop_Moving() (0x00591900, ILocomotion 18)
// Drops the destination; stops unless a head-to coordinate remains.
#include "shared/mech_locomotion.hpp"
// @ref _unknown_geometry_l=00a70b18

void __stdcall MechLocomotionClass::Stop_Moving()
{
    unknown_18 = unknown_geometry_l;
    if (unknown_24 == unknown_geometry_l) {
        unknown_30 = false;
    }
}
