// void __stdcall WalkLocomotionClass::Do_Turn(UnknownFacingDir dir) (0x0071E170, ILocomotion 19)
// Faces the direction at once.
#include "shared/walk_locomotion.hpp"

void __stdcall WalkLocomotionClass::Do_Turn(UnknownFacingDir dir)
{
    LinkedTo->unknown_2e0.Set(UnknownFacingDir(dir));
}
