// int InfantryClass::Do_MISSION_GUARD_AREA() (0x00506490)
// Defers to FootClass's slot 130 unless 0x00507F70 handles it (not -1).
#include "shared/infantry.hpp"
// @ref ?Update_Deployed_Attack_00507f70@InfantryClass@@QAEHXZ=00507f70
// @ref ?Do_MISSION_GUARD_AREA@FootClass@@UAEHXZ=004c5900

int InfantryClass::Do_MISSION_GUARD_AREA()
{
    int result = Update_Deployed_Attack_00507f70();
    if (result == -1) {
        return FootClass::Do_MISSION_GUARD_AREA();
    }
    return result;
}
