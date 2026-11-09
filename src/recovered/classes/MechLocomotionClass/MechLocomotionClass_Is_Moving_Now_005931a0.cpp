// bool __stdcall MechLocomotionClass::Is_Moving_Now() (0x005931A0, ILocomotion 32)
// Turning, or moving toward a head-to coordinate with some speed.
#include "shared/mech_locomotion.hpp"
// @ref ?Is_Rotating@FacingClass@@QBE_NXZ=004b8c90
// @ref _unknown_geometry_l=00a70b18

bool __stdcall MechLocomotionClass::Is_Moving_Now()
{
    if (LinkedTo->unknown_2e0.Is_Rotating()) {
        return true;
    }
    if (Is_Moving() && unknown_24 != unknown_geometry_l && LinkedTo->unknown_slot303() > 0) {
        return true;
    }
    return false;
}
