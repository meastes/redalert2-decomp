// UnknownTimed::unknown_496b50 (0x00496B50): the forward change has finished (DelayTimerClass::Is_Count_Down_Complete).
#include "shared/techno.hpp"

bool UnknownTimed::unknown_496b50() const
{
    if (!IsActive && !IsReverse) {
        return true;
    }
    return false;
}
