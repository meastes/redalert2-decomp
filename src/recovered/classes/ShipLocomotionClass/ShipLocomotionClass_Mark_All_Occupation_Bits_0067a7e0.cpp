// void __stdcall ShipLocomotionClass::Mark_All_Occupation_Bits(int mark) (0x0067A7E0, ILocomotion 39)
// Marks the cells toward the head-to coordinate when one is set.
#include "shared/ship_locomotion.hpp"
// @ref ?unknown_676b20@ShipLocomotionClass@@QAEXABUCoordStruct@@H@Z=00676b20
// @ref _unknown_geometry_l=00ab91c8

void __stdcall ShipLocomotionClass::Mark_All_Occupation_Bits(int mark)
{
    if (unknown_40 != unknown_geometry_l) {
        unknown_676b20(unknown_40, mark);
    }
}
