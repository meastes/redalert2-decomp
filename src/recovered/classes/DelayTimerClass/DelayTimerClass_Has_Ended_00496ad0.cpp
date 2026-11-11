#include "shared/techno.hpp"
bool DelayTimerClass::Has_Ended() const
{
    if (IsActive && Timer.TotalTime) {
        if ((double)(Timer.TotalTime - Timer.Value()) / Timer.TotalTime != 1.0) return false;
    }
    return true;
}
