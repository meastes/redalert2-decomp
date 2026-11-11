#include "shared/techno.hpp"
void DelayTimerClass::Reverse()
{
    if (IsActive) {
        Timer.Reverse();
        IsToCountUp = !IsToCountUp;
    }
}
