// MSTimerClass::operator long (0x005B6A20): the millisecond reading of the system clock.
// The same unit as the constructor 0x005B6A00 (timeBeginPeriod(1)) and destructor 0x005B6A10
// (timeEndPeriod(1)); TibSun mstimer.cpp has the class with these four members.
#include "shared/main_loop.hpp"
#include "shared/win32_api.hpp"

MSTimerClass::operator long() const
{
    return timeGetTime();
}
