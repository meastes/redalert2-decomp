// mstimer.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/mstimer.json carries the addresses and the rows that
// stay in their own files.


#include "shared/main_loop.hpp"
#include "shared/ahandle.hpp"
#include "shared/win32_api.hpp"


// MSTimerClass::MSTimerClass (0x005B6A00): raises the system timer resolution to 1 ms (TibSun mstimer.cpp).

MSTimerClass::MSTimerClass()
{
    timeBeginPeriod(1);
}


// MSTimerClass::~MSTimerClass (0x005B6A10): restores the system timer resolution (TibSun mstimer.cpp).

MSTimerClass::~MSTimerClass()
{
    timeEndPeriod(1);
}


// MSTimerClass::operator long (0x005B6A20): the millisecond reading of the system clock.
// The same unit as the constructor 0x005B6A00 (timeBeginPeriod(1)) and destructor 0x005B6A10
// (timeEndPeriod(1)); TibSun mstimer.cpp has the class with these four members.

MSTimerClass::operator long() const
{
    return timeGetTime();
}


// MSTimerClass::operator() (0x005B6A30): the same reading as operator long; unreferenced in the
// image (name and member order follow TibSun mstimer.cpp, a hypothesis).

long MSTimerClass::operator()() const
{
    return timeGetTime();
}
