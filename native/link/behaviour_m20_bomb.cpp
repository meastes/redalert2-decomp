// Behaviour-written body for native M20 (Scout1161; see behaviour_m20.cpp). NOT EXACT.
#include "shared/object.hpp"
#include "shared/sound_play.hpp"

// 0x00435FB0 BombClass (object.hpp: disarms): lets go of the object it sits on
// (that object's bomb pointer +0x38 and flag +0x68 cleared), marks itself done
// (+0x58), clears the planting unit (+0x24), the target (+0x2C) and the house
// (+0x28), finishes the ticking sound (+0x3C, 0x00405FE0) and clears +0x54, in the
// order of the unowned view src/recovered/unowned/Unknown435FB0Owner_unknown_435fb0_00435fb0.cpp
// (char pads to +0x24 and +0x38 of the target).
void BombClass::Unknown_00435FB0()
{
    if (Target != 0) {
        Target->unknown_38 = 0;
        Target->unknown_68 = false;
    }
    unknown_58 = true;
    Owner = 0;
    Target = 0;
    unknown_28 = 0;
    unknown_3c.Unknown_00405FE0();
    unknown_54 = 0;
}

// ObjectClass's base bodies of three virtuals (pass-6a decompiles: one-line leaves,
// shared with other classes at the same address). Reached by the selection band and
// the sidebar/house checks for objects that do not override them (Scout1161 play
// sweep, '#5' with a band drag over the map: hundreds of traps per frame).
// 0x00423DA0 slot 48: false.
bool ObjectClass::unknown_slot48()
{
    return false;
}
// 0x00423DB0 slot 49 (a house argument): false.
bool ObjectClass::unknown_slot49(HouseClass *house)
{
    return false;
}
// 0x0041B2E0 slot 51 (a bool argument): no house (0).
HouseClass *ObjectClass::unknown_slot51(bool arg1)
{
    return 0;
}
