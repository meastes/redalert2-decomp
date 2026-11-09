// bool __stdcall DriveLocomotionClass::Is_Moving() (0x0049FC70, ILocomotion 4)
// A destination is set, or a head-to coordinate other than where the
// object stands (X and Y).
#include "shared/drive_locomotion.hpp"
// @ref _unknown_geometry_l=00852998

bool __stdcall DriveLocomotionClass::Is_Moving()
{
    if (unknown_34 != unknown_geometry_l) {
        return true;
    }
    if (unknown_40 != unknown_geometry_l) {
        if (unknown_40.X != Object_Coord(LinkedTo).X || unknown_40.Y != Object_Coord(LinkedTo).Y) {
            return true;
        }
    }
    return false;
}
