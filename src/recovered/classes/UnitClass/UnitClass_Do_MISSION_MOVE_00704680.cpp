// int UnitClass::Do_MISSION_MOVE() (0x00704680, slot 133)
// Clears +0x5BA; with +0x5C8 set queues mission 5 and answers 1; otherwise
// starts the +0x2A8 change over the type's DeployTime when it is idle and
// continues with the foot behaviour.
#include "shared/unit.hpp"
// @ref ?Is_Count_Down_Complete@DelayTimerClass@@QBE_NXZ=00496b50
// @ref ?Count_Down@DelayTimerClass@@QAEXN@Z=00496bc0
// @ref ?Do_MISSION_MOVE@FootClass@@UAEHXZ=004c38d0

int UnitClass::Do_MISSION_MOVE()
{
    unknown_5ba = false;
    if (unknown_5c8) {
        QueueMission(5, false);
        return 1;
    }
    if (!unknown_2a8.Is_Count_Down_Complete()) {
        unknown_2a8.Count_Down(Type->DeployTime);
    }
    return FootClass::Do_MISSION_MOVE();
}
