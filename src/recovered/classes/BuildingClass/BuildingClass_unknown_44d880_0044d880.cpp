// bool BuildingClass::unknown_44d880() (0x0044D880)
// True for a type that is not a gate, or for a gate in mission 24 whose
// +0x2A8 state reports done (0x00496B30).
#include "shared/building.hpp"
// @ref ?Is_Count_Up_Complete@DelayTimerClass@@QBE_NXZ=00496b30

bool BuildingClass::unknown_44d880()
{
    if (!Type->Gate) {
        return true;
    }
    if (Get_Mission() == 24 && unknown_2a8.Is_Count_Up_Complete()) {
        return true;
    }
    return false;
}
