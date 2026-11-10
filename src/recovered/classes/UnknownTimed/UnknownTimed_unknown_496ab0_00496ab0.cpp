// UnknownTimed::unknown_496ab0 (0x00496AB0): a timed change is running forward (DelayTimerClass::Is_Count_Down_Active).
#include "shared/techno.hpp"

bool UnknownTimed::unknown_496ab0() const
{
    if (IsActive && !IsReverse) {
        return true;
    }
    return false;
}
