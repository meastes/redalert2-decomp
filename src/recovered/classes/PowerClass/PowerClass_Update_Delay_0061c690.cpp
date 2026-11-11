// int PowerClass::Update_Delay() (0x0061C690)
// The three counters' total, capped at Desired_Power_Height, as a share of it
// times 5. No direct callers (inlined elsewhere).
#include "shared/power.hpp"
// @ref __real@8@4001a000000000000000=0079c278
// @ref __ftol=0077f110

int PowerClass::Update_Delay()
{
    int value = Desired_Power_Height();
    int total = RedPipCount + YellowPipCount + GreenPipCount;
    if (total > value) {
        total = value;
    }
    return (int)((double)total / value * 5.0);
}
// @ref ?Desired_Power_Height@PowerClass@@QAEHXZ=0061c5f0
