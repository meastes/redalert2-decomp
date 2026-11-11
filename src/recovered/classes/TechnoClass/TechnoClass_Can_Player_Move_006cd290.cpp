// bool TechnoClass::Can_Player_Move() (0x006CD290, slot 39)
// Owner test, then slots 201/202, the type flag +0xB04 and slot 114.
#include "shared/techno.hpp"
// @ref ?Is_Player_Control@HouseClass@@QAE_NXZ=004f52b0

bool TechnoClass::Can_Player_Move()
{
    if (!House->Is_Player_Control()) {
        return false;
    }
    if (unknown_slot201()) {
        return false;
    }
    if (Techno_Type_Class()->Spawned) {
        return false;
    }
    if (unknown_slot202()) {
        return false;
    }
    return unknown_slot114() ? true : false;
}
