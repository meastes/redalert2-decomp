// void __fastcall AudioLevel::Set_Rate(__int64 period, int step) -- 0x004070D0
// Purpose: the level moves step (whole units) per period in milliseconds:
// the 16.16 step per millisecond, and no more than one period per update.
#include "shared/audio.hpp"

void __fastcall AudioLevel::Set_Rate(__int64 period, int step)
{
    Step = (step << 16) / (unsigned int)period;
    MaxElapsed = period;
}
