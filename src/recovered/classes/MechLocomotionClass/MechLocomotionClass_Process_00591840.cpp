// bool __stdcall MechLocomotionClass::Process() (0x00591840, ILocomotion 16)
// One step (0x005919A0), then whether still moving.
#include "shared/mech_locomotion.hpp"
// @ref ?Movement_AI@MechLocomotionClass@@QAEX_N@Z=005919a0

bool __stdcall MechLocomotionClass::Process()
{
    Movement_AI(true);
    return Is_Moving();
}
