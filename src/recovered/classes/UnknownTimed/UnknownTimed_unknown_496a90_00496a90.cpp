// UnknownTimed::unknown_496a90 (0x00496A90): a timed change is running in reverse (mischa85 names.csv:
// DelayTimerClass::Is_Count_Up_Active).
#include "shared/techno.hpp"

bool UnknownTimed::unknown_496a90() const
{
    if (IsActive && IsReverse) {
        return true;
    }
    return false;
}
