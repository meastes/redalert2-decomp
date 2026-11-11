// int WeaponTypeClass::Get_Speed(int range) const (0x00733F70)
// Init_Max_Speed's value for another range.
#include "shared/weapon_type.hpp"
// @ref ?Rules@@3PAVRulesClass@@A=00839848

int WeaponTypeClass::Get_Speed(int range) const
{
    if (Projectile != 0 && Projectile->ROT == 0) {
        double speed = Projectile->Floater ? Unknown_0047FCD0() : Rules->unknown_1320;
        return Unknown_0047FB70(range, speed);
    }
    return unknown_a8;
}
