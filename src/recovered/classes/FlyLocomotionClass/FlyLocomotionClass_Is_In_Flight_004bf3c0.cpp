// bool FlyLocomotionClass::Is_In_Flight() (0x004BF3C0)
// Not +0x51, and either not +0x50 or at least half the +0x38 height up
// (slot 108). Expanded where it is used; this out-of-line copy is never
// called.
#include "shared/fly_locomotion.hpp"

bool FlyLocomotionClass::Is_In_Flight()
{
    if (!unknown_51 && (!unknown_50 || LinkedTo->Get_Height_AGL() >= unknown_38 / 2)) {
        return true;
    }
    return false;
}
