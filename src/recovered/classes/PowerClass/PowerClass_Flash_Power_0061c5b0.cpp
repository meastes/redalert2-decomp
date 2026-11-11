// void PowerClass::Flash_Power() (0x0061C5B0)
// Sets the first counter to 10 and restarts the first timer at 3 ticks.
#include "shared/power.hpp"
// @ref ??BSystemTimerClass@@QBEJXZ=0069a430

void PowerClass::Flash_Power()
{
    FlashCount = 10;
    FlashTimer = 3;
}
