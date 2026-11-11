// bool __stdcall FlyLocomotionClass::Is_Ion_Sensitive() (0x004BF520, ILocomotion 25)
// Everything but hunter-seekers.
#include "shared/fly_locomotion.hpp"

bool __stdcall FlyLocomotionClass::Is_Ion_Sensitive()
{
    return !LinkedTo->Techno_Type_Class()->HunterSeeker;
}
