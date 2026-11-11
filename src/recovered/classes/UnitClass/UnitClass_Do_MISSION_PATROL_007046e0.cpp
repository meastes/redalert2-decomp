// int UnitClass::Do_MISSION_PATROL() (0x007046E0, slot 145)
// Clears +0x5BA, starts the +0x2A8 change over the type's DeployTime when
// it is idle, then the foot behaviour.
#include "shared/unit.hpp"
// @ref ?Is_Count_Down_Complete@DelayTimerClass@@QBE_NXZ=00496b50
// @ref ?Count_Down@DelayTimerClass@@QAEXN@Z=00496bc0
// @ref ?Do_MISSION_PATROL@FootClass@@UAEHXZ=004c3950

int UnitClass::Do_MISSION_PATROL()
{
    unknown_5ba = false;
    if (!unknown_2a8.Is_Count_Down_Complete()) {
        unknown_2a8.Count_Down(Type->DeployTime);
    }
    return FootClass::Do_MISSION_PATROL();
}
