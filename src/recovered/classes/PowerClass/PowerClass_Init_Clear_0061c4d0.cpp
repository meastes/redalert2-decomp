// void PowerClass::Init_Clear() (0x0061C4D0, slot 7)
// Clears the radar, then restarts the power bar's timers and counters.
#include "shared/power.hpp"
// @ref ?Init_Clear@RadarClass@@UAEXXZ=0062f260
// @ref ??BSystemTimerClass@@QBEJXZ=0069a430

void PowerClass::Init_Clear()
{
    RadarClass::Init_Clear();
    RecordedDrain = -1;
    RecordedPower = -1;
    FlashTimer = CDTimerClass<SystemTimerClass>();
    FlashCount = 0;
    UpdateTimer = CDTimerClass<SystemTimerClass>();
    GreenPipCount = 0;
    YellowPipCount = 0;
    RedPipCount = 0;
    HasChanged = false;
}
