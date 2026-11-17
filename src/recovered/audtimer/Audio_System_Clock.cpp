// __int64 Audio_System_Clock() -- 0x00409310
// Purpose: the timeGetTime clock extended to 64 bits: bumps the reader
// sequence, then (until no other reader interfered) reads timeGetTime,
// counts a wrap of the low word into the high word and stores both.
// Returns the extended time.
#include "shared/audio.hpp"

__int64 Audio_System_Clock()
{
    int sequence = ++Audio_Clock_Sequence;
    unsigned long high = *Audio_Clock_High;
    unsigned long low = *Audio_Clock_Low;
    while (sequence != Audio_Clock_Sequence) {
        sequence = Audio_Clock_Sequence;
    }
    for (;;) {
        unsigned long now = timeGetTime();
        if (now < low) {
            high++;
        }
        *Audio_Clock_High = high;
        *Audio_Clock_Low = now;
        if (sequence == Audio_Clock_Sequence) {
            break;
        }
        sequence = Audio_Clock_Sequence;
    }
    return Audio_Clock_Time;
}
