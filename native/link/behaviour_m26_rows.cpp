// Behaviour rows (Scout1208, native M26; NOT EXACT, native/behaviour_rewrites.json).
// Small reachable-looking link closure rows, each from its pass-6a decompile.
#define RA2_HASH_STRING_UNIT
#define RA2_HSV_UNIT
#include "shared/hash_string.hpp"
#include "shared/hsv_class.hpp"
#include "shared/system_timer.hpp"
#include "shared/wide_string.hpp"
#include "shared/object.hpp"

// 0x004E1290 / 0x004E12A0: the free HashString comparisons jump to the members
// (0x004E11F0 / 0x004E1240; two-byte bodies).
bool __fastcall operator==(HashString const &a, HashString const &b)
{
    return a.operator==(b);
}
bool __fastcall operator!=(HashString const &a, HashString const &b)
{
    return a.operator!=(b);
}

// 0x004FF1E0: the free HSVClass comparison, the member's (0x004FF170).
bool __fastcall operator==(HSVClass const &a, HSVClass const &b)
{
    return a.operator==(b);
}

// 0x0069A430 as the callable clock (system_timer.hpp: the same body as operator long).
RA2_LONG SystemTimerClass::operator()() const
{
    return operator RA2_LONG();
}

// 0x00770400: the second copy of WideString::Get_Char (0x00770470): the character at
// index, 0 past the end.
unsigned short WideString::unknown_char_770400(unsigned index) const
{
    return Get_Char(index);
}

// 0x00435FF0: the bomb's type (+0x30).
int BombClass::Unknown_00435FF0() const
{
    return Type;
}
