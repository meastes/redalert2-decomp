// MSTimerClass::~MSTimerClass (0x005B6A10): restores the system timer resolution (TibSun mstimer.cpp).
#include "shared/main_loop.hpp"
#include "shared/ahandle.hpp"

MSTimerClass::~MSTimerClass()
{
    timeEndPeriod(1);
}
