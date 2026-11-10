// RadarClass::Unknown_006331A0 (0x006331A0): the radar shows the player names (TibSun RadarClass::Is_Player_Names).
#include "shared/radar.hpp"

bool RadarClass::Unknown_006331A0()
{
    return unknown_14ac == 2 && unknown_14a8 == 1;
}
