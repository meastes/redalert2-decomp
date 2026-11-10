// int InfantryClass::Do_MISSION_GUARD() (0x00506470)
// Defers to FootClass's slot 129 unless 0x00507F70 handles it (not -1).
#include "shared/infantry.hpp"
// @ref ?Update_Deployed_Attack_00507f70@InfantryClass@@QAEHXZ=00507f70
// @ref ?Do_MISSION_GUARD@FootClass@@UAEHXZ=004c4660

int InfantryClass::Do_MISSION_GUARD()
{
    int result = Update_Deployed_Attack_00507f70();
    if (result == -1) {
        return FootClass::Do_MISSION_GUARD();
    }
    return result;
}
