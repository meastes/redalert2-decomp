// Update the attack target; unload deployed vehicles denied a ground target.
#include "shared/building.hpp"
#include "shared/house.hpp"
#include "shared/weapon_type.hpp"
#include "shared/bullet_type.hpp"
// @ref ?unknown_slot220@TechnoClass@@UAEXPAVAbstractClass@@@Z=006c9af0
// @ref ?Is_Human_Player@HouseClass@@QAE_NXZ=004f52f0
void BuildingClass::unknown_slot220(AbstractClass *target)
{
    if (Mission == 19 || !unknown_slot195())
        target = 0;
    if (!target || !unknown_slot231(0)->WeaponType
        || unknown_slot231(0)->WeaponType->Projectile->AA
        || unknown_slot213(target)) {
        TechnoClass::unknown_slot220(target);
    } else {
        TechnoClass::unknown_slot220(0);
        if ((Type->TickTank || Type->Artillary)
            && !House->Is_Human_Player() && !unknown_slot201()) {
            unknown_slot116(19, false);
            Commence();
        }
    }
}
