// UnknownTimed::unknown_496b30 (0x00496B30): the reverse change has finished (DelayTimerClass::Is_Count_Up_Complete).
#include "shared/techno.hpp"

bool UnknownTimed::unknown_496b30() const
{
    if (!IsActive && IsReverse == true) {
        return true;
    }
    return false;
}
