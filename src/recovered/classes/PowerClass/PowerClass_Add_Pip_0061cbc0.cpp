// void PowerClass::Add_Pip() (0x0061CBC0)
// Steps one of the power bar's counters up toward the target 0x0061C700
// gives it: +0x1530 first, then +0x1528, then +0x152C (the counterpart of
// 0x0061CB50; no direct callers).
#include "shared/power.hpp"
// @ref ?Desired_Levels@PowerClass@@QAEHAAH00@Z=0061c700

void PowerClass::Add_Pip()
{
    int green;
    int red;
    int yellow;
    Desired_Levels(green, yellow, red);
    if (RedPipCount < red) {
        RedPipCount++;
        return;
    }
    if (GreenPipCount < green) {
        GreenPipCount++;
        return;
    }
    if (YellowPipCount < yellow) {
        YellowPipCount++;
    }
}
