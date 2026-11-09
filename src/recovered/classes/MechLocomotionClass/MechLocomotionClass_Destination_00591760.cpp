// CoordStruct __stdcall MechLocomotionClass::Destination() (0x00591760, ILocomotion 5)
// The destination while moving, otherwise none.
#include "shared/mech_locomotion.hpp"
// @ref _unknown_geometry_l=00a70b18

CoordStruct __stdcall MechLocomotionClass::Destination()
{
    if (Is_Moving()) {
        return CoordStruct(unknown_18.X, unknown_18.Y, unknown_18.Z);
    }
    return CoordStruct(unknown_geometry_l.X, unknown_geometry_l.Y, unknown_geometry_l.Z);
}
