// void PowerClass::Remove_Pip() (0x0061CB50)
// Steps one of the power bar's counters down toward the target 0x0061C700
// gives it: +0x1528 first, then +0x1530, then +0x152C.
#include "shared/power.hpp"
// @ref ?Desired_Levels@PowerClass@@QAEHAAH00@Z=0061c700

void PowerClass::Remove_Pip()
{
    int green;
    int red;
    int yellow;
    Desired_Levels(green, yellow, red);
    if (GreenPipCount > green) {
        GreenPipCount--;
        return;
    }
    if (RedPipCount > red) {
        RedPipCount--;
        return;
    }
    if (YellowPipCount > yellow) {
        YellowPipCount--;
    }
}
