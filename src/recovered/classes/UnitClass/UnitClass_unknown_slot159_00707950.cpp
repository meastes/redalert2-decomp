// bool UnitClass::unknown_slot159() (0x00707950, slot 159)
// True when the state at +0x2A8 is in none of its three active phases.
#include "shared/unit.hpp"
// @ref ?Is_Count_Up_Active@DelayTimerClass@@QBE_NXZ=00496a90
// @ref ?Is_Count_Down_Active@DelayTimerClass@@QBE_NXZ=00496ab0
// @ref ?Is_Count_Up_Complete@DelayTimerClass@@QBE_NXZ=00496b30

bool UnitClass::unknown_slot159()
{
    if (unknown_2a8.Is_Count_Up_Active() || unknown_2a8.Is_Count_Down_Active() || unknown_2a8.Is_Count_Up_Complete()) {
        return false;
    }
    return true;
}
