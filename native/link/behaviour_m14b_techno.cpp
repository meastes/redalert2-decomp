// Behaviour rows (Scout1121, native M14b; NOT EXACT, native/behaviour_rewrites.json):
// TechnoClass leaves a menu-started skirmish's combat reaches (hunting units).
#include "shared/techno.hpp"
#include "shared/techno_type.hpp"

// 0x00453B50 (TechnoClass slot 156, +0x270; kept copy elsewhere): the shared
// leaf `return false` (pass-6a decompile 00453b50).
bool TechnoClass::unknown_slot156()
{
    return false;
}

// 0x0041B3F0 (TechnoClass slot 232, +0x3A0): whether the techno type has a
// turret (pass-6a decompile 0041b3f0: slot 32's type, byte +0xAAB =
// TechnoTypeClass::Turret).
bool TechnoClass::unknown_slot232()
{
    return unknown_slot32()->Turret;
}

// 0x00453B60 (TechnoClass slot 169, +0x2A4; kept copy elsewhere): the shared
// leaf `return true` with one argument (pass-6a decompile 00453b60).
bool TechnoClass::unknown_slot169(AbstractClass *target)
{
    return true;
}
