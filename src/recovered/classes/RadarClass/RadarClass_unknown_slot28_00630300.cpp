// void RadarClass::unknown_slot28(Rect const *rect, bool arg2, int arg3, bool arg4) (0x00630300, slot 28)
// Passes the map dimensions on to the base class (TibSun RadarClass::Set_Map_Dimensions).
#include "shared/radar.hpp"
// @ref ?unknown_slot28@MapClass@@UAEXPBVRect@@_NH1@Z=005484e0

void RadarClass::unknown_slot28(Rect const *rect, bool arg2, int arg3, bool arg4)
{
    DisplayClass::unknown_slot28(rect, arg2, arg3, arg4);
}
