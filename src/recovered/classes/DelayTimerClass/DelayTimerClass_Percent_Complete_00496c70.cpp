#include "shared/techno.hpp"
double DelayTimerClass::Percent_Complete() const
{
    if (IsActive) {
        return Timer.Percentage();
    }
    return 1.0;
}
