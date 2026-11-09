// CoordStruct __stdcall JumpjetLocomotionClass::Destination() (0x0052F360, ILocomotion 5)
// The destination while moving, otherwise the null coordinate.
#include "shared/jumpjet_locomotion.hpp"
// @ref _unknown_geometry_l=00a6e3e0

CoordStruct __stdcall JumpjetLocomotionClass::Destination()
{
    if (Is_Moving()) {
        return CoordStruct(unknown_3c.X, unknown_3c.Y, unknown_3c.Z);
    }
    return CoordStruct(unknown_geometry_l.X, unknown_geometry_l.Y, unknown_geometry_l.Z);
}
