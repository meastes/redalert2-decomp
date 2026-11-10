// int UnitClass::Do_MISSION_PATROL() (0x007046E0, slot 145)
// Clears +0x5BA, starts the +0x2A8 change over the type's DeployTime when
// it is idle, then the foot behaviour.
#include "shared/unit.hpp"
// @ref ?unknown_496b50@UnknownTimed@@QBE_NXZ=00496b50
// @ref ?unknown_496bc0@UnknownTimed@@QAEXN@Z=00496bc0
// @ref ?Do_MISSION_PATROL@FootClass@@UAEHXZ=004c3950

int UnitClass::Do_MISSION_PATROL()
{
    unknown_5ba = false;
    if (!unknown_2a8.unknown_496b50()) {
        unknown_2a8.unknown_496bc0(Type->DeployTime);
    }
    return FootClass::Do_MISSION_PATROL();
}
