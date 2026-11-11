// DelayTimerClass::Is_Count_Down_Complete (0x00496B50): the forward change has finished (DelayTimerClass::Is_Count_Down_Complete).
#include "shared/techno.hpp"

bool DelayTimerClass::Is_Count_Down_Complete() const
{
    if (!IsActive && !IsToCountUp) {
        return true;
    }
    return false;
}
