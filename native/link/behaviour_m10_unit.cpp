// Behaviour-written rows the skirmish harness reaches once units take move
// and deploy orders (Scout1103, native M10). NOT EXACT; never original code or bytes.
#include "shared/unit.hpp"

// 0x0070A240 UnitClass slot 201 (seed decompile): true when +0x5C0 holds a
// value (not -1), else TechnoClass's slot 201 (0x006D8E80, +0x3FC > 0).
bool UnitClass::unknown_slot201()
{
    return unknown_5c0 != -1 || TechnoClass::unknown_slot201();
}

