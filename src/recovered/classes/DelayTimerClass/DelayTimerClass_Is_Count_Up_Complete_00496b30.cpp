// DelayTimerClass::Is_Count_Up_Complete (0x00496B30): the reverse change has finished (DelayTimerClass::Is_Count_Up_Complete).
#include "shared/techno.hpp"

bool DelayTimerClass::Is_Count_Up_Complete() const
{
    if (!IsActive && IsToCountUp == true) {
        return true;
    }
    return false;
}
