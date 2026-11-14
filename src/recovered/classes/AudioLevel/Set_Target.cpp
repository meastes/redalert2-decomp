// void __fastcall AudioLevel::Set_Target(int value) -- 0x004070A0
// Purpose: makes the level ramp to value (16.16); a level at rest restarts
// its clock first.
// @ref ?Audio_Now@@YI_JXZ=004092d0
#include "shared/audio.hpp"

void __fastcall AudioLevel::Set_Target(int value)
{
    Flags &= ~AUDIO_LEVEL_PENDING;
    if (Target == Current) {
        LastTime = Audio_Now();
    }
    Target = value << 16;
}
