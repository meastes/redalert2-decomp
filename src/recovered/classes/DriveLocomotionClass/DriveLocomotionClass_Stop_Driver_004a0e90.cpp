// bool DriveLocomotionClass::Stop_Driver() (0x004A0E90)
// Clears the head-to coordinate (Westwood's DriveClass::Stop_Driver).
// Expanded where it is used; this out-of-line copy is never called.
#include "shared/drive_locomotion.hpp"
// @ref _unknown_geometry_l=00852998

bool DriveLocomotionClass::Stop_Driver()
{
    if (unknown_40 != unknown_geometry_l) {
        unknown_40 = unknown_geometry_l;
        unknown_63 = false;
        return true;
    }
    return false;
}
