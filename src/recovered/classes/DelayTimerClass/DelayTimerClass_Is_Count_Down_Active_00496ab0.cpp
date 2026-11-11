// DelayTimerClass::Is_Count_Down_Active (0x00496AB0): a timed change is running forward (DelayTimerClass::Is_Count_Down_Active).
#include "shared/techno.hpp"

bool DelayTimerClass::Is_Count_Down_Active() const
{
    if (IsActive && !IsToCountUp) {
        return true;
    }
    return false;
}
