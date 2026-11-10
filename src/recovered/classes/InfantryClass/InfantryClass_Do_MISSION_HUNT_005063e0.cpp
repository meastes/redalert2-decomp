// int InfantryClass::Do_MISSION_HUNT() (0x005063E0, slot 132)
// Against a building target (unless the house check 0x004F52F0 holds):
// Infiltrate types, or Occupier/Assaulter types the building accepts,
// target it and set mission 8 (returns 1); otherwise FootClass's.
#include "shared/infantry.hpp"
// @ref ?Unknown_004F52F0@HouseClass@@QAE_NXZ=004f52f0
// @ref ?Unknown_00452EB0@BuildingClass@@QAE_NPAVTechnoClass@@@Z=00452eb0
// @ref ?Do_MISSION_HUNT@FootClass@@UAEHXZ=004c48e0

int InfantryClass::Do_MISSION_HUNT()
{
    if (!House->Unknown_004F52F0() && unknown_228 != 0 && unknown_228->Kind_Of() == 6) {
        if (Type->Infiltrate
            || ((Type->Occupier || Type->Assaulter) && ((BuildingClass *)unknown_228)->Unknown_00452EB0(this))) {
            Assign_Destination(unknown_228, true);
            Set_Mission(8);
            return 1;
        }
    }
    return FootClass::Do_MISSION_HUNT();
}
