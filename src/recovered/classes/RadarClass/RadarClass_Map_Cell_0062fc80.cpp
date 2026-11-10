// bool RadarClass::Map_Cell(Cell const &cell, HouseClass *house) (0x0062FC80)
// Passes the call on (the radar adds nothing).
#include "shared/radar.hpp"
// @ref ?Map_Cell@DisplayClass@@UAE_NABVCell@@PAVHouseClass@@@Z=00499bb0

bool RadarClass::Map_Cell(Cell const &cell, HouseClass *house)
{
    if (DisplayClass::Map_Cell(cell, house)) {
        return true;
    }
    return false;
}
