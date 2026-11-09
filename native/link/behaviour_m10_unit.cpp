// Behaviour-written rows the skirmish harness reaches once units take move
// and deploy orders (Scout1103, native M10). NOT EXACT; never original code or bytes.
#include "shared/unit.hpp"

// 0x0070A240 UnitClass slot 201 (seed decompile): true when +0x5C0 holds a
// value (not -1), else TechnoClass's slot 201 (0x006D8E80, +0x3FC > 0).
bool UnitClass::unknown_slot201()
{
    return unknown_5c0 != -1 || TechnoClass::unknown_slot201();
}

// UnknownTimed state tests (seed decompiles; +0x18 IsActive, +0x19 IsReverse).
// 0x00496A90: active and reversing.
bool UnknownTimed::unknown_496a90() const
{
    return IsActive && IsReverse;
}

// 0x00496AB0: active, not reversing.
bool UnknownTimed::unknown_496ab0() const
{
    return IsActive && !IsReverse;
}

// 0x00496B30: not active and +0x19 exactly 1 (a finished reverse). The
// header's comment ("+0x18 set, or +0x19 is 1") does not match the decompile.
bool UnknownTimed::unknown_496b30() const
{
    return !IsActive && *(unsigned char const *)&IsReverse == 1;
}
