// DelayTimerClass::Is_Count_Up_Active (0x00496A90): a timed change is running in reverse (mischa85 names.csv:
// DelayTimerClass::Is_Count_Up_Active).
#include "shared/techno.hpp"

bool DelayTimerClass::Is_Count_Up_Active() const
{
    if (IsActive && IsToCountUp) {
        return true;
    }
    return false;
}
