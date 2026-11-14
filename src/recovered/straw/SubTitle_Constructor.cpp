// SubTitle::SubTitle() -- 0x0069B0C0
// Purpose: a caption with the script's defaults: shown at time 0 for 1200 ticks (20 s), white,
// on line 15, centred, no text.
// Assumptions: name and member order from the ra2-decomp YR twin (SubTitle::SubTitle, straw.cpp).
#include "shared/subtitle.hpp"

SubTitle::SubTitle() :
    DisplayTime(0),
    Duration(20 * 60),
    Color(0x00FFFFFF),
    LinePosition(15),
    Alignment(2),
    Text(0)
{
}
