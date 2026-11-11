#include "shared/techno.hpp"
void DelayTimerClass::Count_Up(double time)
{
    if (IsActive || IsToCountUp != true) {
        time *= 900.0;
        IsActive = true;
        Duration = time;
        Timer = UnknownDurationTimer423F60((long)time);
        IsToCountUp = true;
    }
}
