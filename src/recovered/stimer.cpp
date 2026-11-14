// stimer.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/stimer.json carries the addresses and the rows that
// stay in their own files.


#include "shared/system_timer.hpp"
#include "shared/win32_api.hpp"


// long SystemTimerClass::operator long() const -- 0x0069A430
// Purpose: the system clock in 16 ms units: timeGetTime() >> 4.

SystemTimerClass::operator long() const
{
    return timeGetTime() >> 4;
}


// long UnknownTimeSource_0069a440::operator long() const -- 0x0069A440
// Purpose: the same clock as SystemTimerClass (timeGetTime() >> 4, 16 ms
// units); no callers, owner not identified.

UnknownTimeSource_0069a440::operator long() const
{
    return timeGetTime() >> 4;
}
