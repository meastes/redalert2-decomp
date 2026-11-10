// bool RadarClass::RevealFogShroud(Cell const &cell, HouseClass *house, bool arg3) (0x0062FCA0)
// Passes the call on (the radar adds nothing).
#include "shared/radar.hpp"
// @ref ?RevealFogShroud@DisplayClass@@UAE_NABVCell@@PAVHouseClass@@_N@Z=00499fc0

bool RadarClass::RevealFogShroud(Cell const &cell, HouseClass *house, bool arg3)
{
    if (DisplayClass::RevealFogShroud(cell, house, arg3)) {
        return true;
    }
    return false;
}
