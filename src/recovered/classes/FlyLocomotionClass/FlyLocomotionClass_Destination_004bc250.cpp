// CoordStruct __stdcall FlyLocomotionClass::Destination() (0x004BC250, ILocomotion 5)
#include "shared/fly_locomotion.hpp"
// @ref _unknown_geometry_l=00865df8

CoordStruct __stdcall FlyLocomotionClass::Destination()
{
    if (Is_Moving()) {
        return CoordStruct(unknown_1c.X, unknown_1c.Y, unknown_1c.Z);
    }
    return CoordStruct(unknown_geometry_l.X, unknown_geometry_l.Y, unknown_geometry_l.Z);
}
