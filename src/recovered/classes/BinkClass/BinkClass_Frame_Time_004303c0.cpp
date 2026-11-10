// int BinkClass::Frame_Time() (0x004303C0): milliseconds per frame.
#include "shared/movie.hpp"

int BinkClass::Frame_Time()
{
    return 1000 / (Handle->FrameRate / Handle->FrameRateDiv);
}
