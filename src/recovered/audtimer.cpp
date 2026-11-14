// audtimer.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/audtimer.json carries the addresses and the rows that
// stay in their own files.


#include "shared/audio.hpp"


// void Audio_Timer_Init() -- 0x00409280
// Purpose: picks the millisecond clock: with a performance counter, stores
// its ticks per millisecond and selects the counter clock; otherwise the
// timeGetTime clock.

void Audio_Timer_Init()
{
    LARGE_INTEGER frequency;
    if (QueryPerformanceFrequency(&frequency)) {
        Audio_Ticks_Per_Ms = frequency.QuadPart / 1000;
        Audio_Clock = Audio_Counter_Clock;
    } else {
        Audio_Clock = Audio_System_Clock;
    }
}


// __int64 __fastcall Audio_Now() -- 0x004092D0
// Purpose: the current time in milliseconds from the chosen clock, or 0
// before Audio_Timer_Init.

__int64 __fastcall Audio_Now()
{
    if (Audio_Clock != 0) {
        return Audio_Clock();
    }
    return 0;
}


// __int64 Audio_Counter_Clock() -- 0x004092E0
// Purpose: the performance counter in milliseconds.

__int64 Audio_Counter_Clock()
{
    LARGE_INTEGER count;
    QueryPerformanceCounter(&count);
    return count.QuadPart / Audio_Ticks_Per_Ms;
}
