// MSTimerClass::MSTimerClass (0x005B6A00): raises the system timer resolution to 1 ms (TibSun mstimer.cpp).
#include "shared/main_loop.hpp"
#include "shared/ahandle.hpp"

MSTimerClass::MSTimerClass()
{
    timeBeginPeriod(1);
}
