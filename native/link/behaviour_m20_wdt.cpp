// Behaviour-written body for native M20 (Scout1161; see behaviour_m20.cpp). NOT EXACT.
#include "shared/wdt_state.hpp"

// 0x0072C390 WDTState::Is_Valid (wdt_state.hpp): its +0x44 is set. The unowned
// view (src/recovered/unowned/unknown_has_44_0072c390.cpp) pads 0x44 bytes.
bool WDTState::Is_Valid() const
{
    return unknown_44 != 0;
}

// 0x004071E0 AudioLevel::Get_Target (audio.hpp; pass-6a decompile): the target's
// whole part (16.16 fixed point, +0x08 >> 16). Reached by the theme's volume fade
// once the music plays (Scout1161 play sweep).
#include "shared/audio.hpp"
int __fastcall AudioLevel::Get_Target()
{
    return (int)(Target >> 16);
}

// 0x0049EC60 DisplayClass (display.hpp; pass-6a decompile): stores the object
// (+0x119C) and whether there is one (+0x1198). Reached by the sidebar's
// repair/sell buttons and their cancelling right click (Scout1161 play sweep).
#include "shared/display.hpp"
void DisplayClass::Unknown_0049EC60(ObjectClass *object)
{
    unknown_119c = object;
    unknown_1198 = object != 0;
}
