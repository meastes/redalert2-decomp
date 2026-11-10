// void RadarClass::Set_Map_Dimensions(Rect const *rect, bool arg2, int arg3, bool arg4) (0x00630300, slot 28)
// Passes the map dimensions on to the base class (TibSun RadarClass::Set_Map_Dimensions).
#include "shared/radar.hpp"
// @ref ?Set_Map_Dimensions@MapClass@@UAEXPBVRect@@_NH1@Z=005484e0

void RadarClass::Set_Map_Dimensions(Rect const *rect, bool arg2, int arg3, bool arg4)
{
    DisplayClass::Set_Map_Dimensions(rect, arg2, arg3, arg4);
}
