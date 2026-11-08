// MSTimerClass::operator() (0x005B6A30): the same reading as operator long; unreferenced in the
// image (name and member order follow TibSun mstimer.cpp, a hypothesis).
#include "shared/main_loop.hpp"
#include "shared/win32_api.hpp"

long MSTimerClass::operator()() const
{
    return timeGetTime();
}
