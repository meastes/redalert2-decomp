// void __fastcall AudioLevel::Set(int value) -- 0x00407080
// Purpose: makes the level jump to value (16.16) on its next update.
#include "shared/audio.hpp"

void __fastcall AudioLevel::Set(int value)
{
    Flags |= AUDIO_LEVEL_PENDING;
    Target = value << 16;
}
