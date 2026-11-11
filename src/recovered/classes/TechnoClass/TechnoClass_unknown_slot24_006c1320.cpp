// bool TechnoClass::unknown_slot24() (0x006C1320, slot 24)
// A chain of state tests ending in the owner test 0x004F52B0.
#include "shared/techno.hpp"
// @ref ?Is_Player_Control@HouseClass@@QAE_NXZ=004f52b0

bool TechnoClass::unknown_slot24()
{
    if (Strength > 0 && !unknown_76 && unknown_32d && Kind_Of() != 6 && unknown_373 &&
            Techno_Type_Class()->unknown_200) {
        return House->Is_Player_Control() ? true : false;
    }
    return false;
}
