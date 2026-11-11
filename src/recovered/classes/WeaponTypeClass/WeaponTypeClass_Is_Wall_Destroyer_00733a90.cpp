// bool WeaponTypeClass::Is_Wall_Destroyer() const (0x00733A90)
#include "shared/weapon_type.hpp"

bool WeaponTypeClass::Is_Wall_Destroyer() const
{
    if (Warhead && Warhead->Wall) {
        return true;
    }
    return false;
}
