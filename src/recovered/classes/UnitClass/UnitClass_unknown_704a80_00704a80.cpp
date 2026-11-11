// void UnitClass::unknown_704a80() (0x00704A80): restarts the timed change at
// +0x2A8 over the type's DeployTime. No call sites: AI 0x006FB100 and slot 95
// 0x006FC1F0 expand the same statement inline.
#include "shared/unit.hpp"
// @ref ?Count_Down@DelayTimerClass@@QAEXN@Z=00496bc0

void UnitClass::unknown_704a80()
{
    unknown_2a8.Count_Down(Type->DeployTime);
}
