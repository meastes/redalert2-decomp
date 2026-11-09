// void AudioTimeSpan::Reset() -- 0x00403820
// Purpose: clears the statistics (the peak window stays) and sets +0x40 to 5000.
#include "shared/audio.hpp"

void AudioTimeSpan::Reset()
{
    unknown_00 = 0;
    LateLimit = 0;
    Elapsed = 0;
    Peak = 0;
    LastTime = 0;
    TickCount = 0;
    unknown_50 = 0;
    unknown_40 = 5000;
    Maximum = 0;
    unknown_38 = 0;
    unknown_54 = 0;
    LateCount = 0;
}
