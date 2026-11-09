// Behaviour rows (Scout1121, native M14b; NOT EXACT, native/behaviour_rewrites.json).
#include "shared/building_class.hpp"

// 0x00446920 BuildingClass slot 126 (Mission_Attack; pass-6a decompile 00446920,
// simplified): without a target the building drops it and returns to guard
// (mission 5, commenced); with one it asks Can_Fire (slot 218) with the weapon
// slot 174 chooses: FIRE_OK fires (slot 221), FIRE_FACING turns toward it
// (slot 285, the facing at +0x2E0), FIRE_ILLEGAL/CANT/RANGE drop the target.
// The original's jump table (other fire results, the power check of weapons that
// need power, the charge states at +0xA8) is not carried.
int BuildingClass::unknown_slot126()
{
    if (unknown_228 == 0) {
        unknown_slot220(0);
        unknown_slot116(5, false);
        unknown_slot117();
        return 1;
    }
    int weapon = unknown_slot174(unknown_228);
    switch (unknown_slot218(unknown_228, weapon, true)) {
    case 0:                                     // FIRE_OK
        unknown_slot221(unknown_228, weapon);
        break;
    case 2: {                                   // FIRE_FACING
        UnknownFacingDir dir = unknown_slot285(unknown_228);
        unknown_2e0.Set_Desired(dir);
        break;
    }
    case 5: case 6: case 8:                     // FIRE_ILLEGAL, FIRE_CANT, FIRE_RANGE
        unknown_slot220(0);
        break;
    default:
        break;
    }
    return 1;
}
