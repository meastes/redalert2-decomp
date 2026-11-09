// CoordStruct __stdcall HoverLocomotionClass::Destination() (0x004FC900, ILocomotion 5)
#include "shared/hover_locomotion.hpp"
// @ref _unknown_geometry_l=00a41080

CoordStruct __stdcall HoverLocomotionClass::Destination()
{
    if (unknown_18 != unknown_geometry_l) {
        return CoordStruct(unknown_18.X, unknown_18.Y, unknown_18.Z);
    }
    return CoordStruct(unknown_geometry_l.X, unknown_geometry_l.Y, unknown_geometry_l.Z);
}
