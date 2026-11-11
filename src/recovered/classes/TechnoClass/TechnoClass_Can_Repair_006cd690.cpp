// bool TechnoClass::Can_Repair() (0x006CD690, slot 36)
// Kind 6 with type flag +0xAD8 and less than full strength.
#include "shared/techno.hpp"

bool TechnoClass::Can_Repair()
{
    if (Kind_Of() != 6) {
        return false;
    }
    return Techno_Type_Class()->Repairable && Strength != Class_Of()->MaxStrength;
}
