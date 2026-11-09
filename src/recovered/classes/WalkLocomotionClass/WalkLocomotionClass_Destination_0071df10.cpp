// CoordStruct __stdcall WalkLocomotionClass::Destination() (0x0071DF10, ILocomotion 5)
#include "shared/walk_locomotion.hpp"
// @ref _unknown_geometry_l=00af9060

CoordStruct __stdcall WalkLocomotionClass::Destination()
{
    if (Is_Moving()) {
        return CoordStruct(unknown_1c.X, unknown_1c.Y, unknown_1c.Z);
    }
    return CoordStruct(unknown_geometry_l.X, unknown_geometry_l.Y, unknown_geometry_l.Z);
}
