// Behaviour row the long skirmish runs reach (Scout1103, native M10). NOT
// EXACT; never original code or bytes.
#include "shared/radar.hpp"

// 0x006331A0 RadarClass (seed decompile): the radar is up: +0x14AC (the mode)
// is 2 and +0x14A8 is 1.
bool RadarClass::Unknown_006331A0()
{
    return unknown_14ac == 2 && unknown_14a8 == 1;
}
