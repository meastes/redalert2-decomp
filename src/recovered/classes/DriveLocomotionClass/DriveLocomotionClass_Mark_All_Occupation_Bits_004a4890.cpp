// void __stdcall DriveLocomotionClass::Mark_All_Occupation_Bits(int mark) (0x004A4890, ILocomotion 39)
// Marks the cells toward the head-to coordinate when one is set.
#include "shared/drive_locomotion.hpp"
// @ref ?unknown_4a0bc0@DriveLocomotionClass@@QAEXABUCoordStruct@@H@Z=004a0bc0
// @ref _unknown_geometry_l=00852998

void __stdcall DriveLocomotionClass::Mark_All_Occupation_Bits(int mark)
{
    if (unknown_40 != unknown_geometry_l) {
        unknown_4a0bc0(unknown_40, mark);
    }
}
