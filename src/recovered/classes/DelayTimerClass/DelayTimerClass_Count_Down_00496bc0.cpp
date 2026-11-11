#include "shared/techno.hpp"
void DelayTimerClass::Count_Down(double time)
{
    if (IsActive || IsToCountUp != false) {
        time *= 900.0;
        IsActive = true;
        Duration = time;
        Timer = UnknownDurationTimer423F60((long)time);
        IsToCountUp = false;
    }
}
