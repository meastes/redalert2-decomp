// bool TechnoClass::unknown_slot38() (0x006CD630, slot 38)
// Owner test and an armed weapon slot 230, unless slot 201.
#include "shared/techno.hpp"
// @ref ?Is_Player_Control@HouseClass@@QAE_NXZ=004f52b0

bool TechnoClass::unknown_slot38()
{
    if (House->Is_Player_Control()) {
        WeaponStruct *weapon = unknown_slot230();
        if (weapon && weapon->WeaponType && !unknown_slot201()) {
            return true;
        }
    }
    return false;
}
