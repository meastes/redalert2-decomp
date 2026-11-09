// bool __stdcall WalkLocomotionClass::Is_Moving_Now() (0x0071DEB0, ILocomotion 32)
// Moving, with speed, toward a head-to coordinate.
#include "shared/walk_locomotion.hpp"
// @ref __real@8@00000000000000000000=0079b610
// @ref _unknown_geometry_l=00af9060

bool __stdcall WalkLocomotionClass::Is_Moving_Now()
{
    if (Is_Moving() && LinkedTo->unknown_468 > 0.0 && unknown_28 != unknown_geometry_l) {
        return true;
    }
    return false;
}
