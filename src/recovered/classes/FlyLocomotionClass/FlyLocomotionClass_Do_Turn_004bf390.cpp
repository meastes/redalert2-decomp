// void __stdcall FlyLocomotionClass::Do_Turn(UnknownFacingDir dir) (0x004BF390, ILocomotion 19)
// Sets the linked object's turret facing at once.
#include "shared/fly_locomotion.hpp"

void __stdcall FlyLocomotionClass::Do_Turn(UnknownFacingDir dir)
{
    LinkedTo->unknown_2f8.Set(UnknownFacingDir(dir));
}
