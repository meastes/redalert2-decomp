// Behaviour rows first written beside WalkLocomotionClass's movement step
// (Scout1091, native M8; that step, 0x0071E220, is now the Scout1101 draft
// native/drafts/0071e220.cpp, Scout1103). NOT EXACT. Never original code or bytes.
#include "shared/techno.hpp"
#include "shared/techno_type.hpp"

// 0x006D8640 TechnoClass::Unknown_006D8640 (decompile: a __thiscall that
// reads only its argument): true when the weapon slot exists and names a
// weapon type (its first field).
bool TechnoClass::Unknown_006D8640(WeaponStruct *weapon)
{
    return weapon != 0 && weapon->WeaponType != 0;
}
