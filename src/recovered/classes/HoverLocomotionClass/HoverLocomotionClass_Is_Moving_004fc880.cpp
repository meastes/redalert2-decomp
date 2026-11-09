// bool __stdcall HoverLocomotionClass::Is_Moving() (0x004FC880, ILocomotion 4)
// A destination or a head-to coordinate is set.
#include "shared/hover_locomotion.hpp"
// @ref _unknown_geometry_l=00a41080

bool __stdcall HoverLocomotionClass::Is_Moving()
{
    return unknown_18 != unknown_geometry_l || unknown_24 != unknown_geometry_l;
}
