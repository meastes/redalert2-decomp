// bool TechnoClass::unknown_slot192(HouseClass *house) (0x006D7C40, slot 192)
// Cell test 0x0047C2E0 for a house.
#include "shared/techno.hpp"
#include "shared/gscreen.hpp"
#include "shared/techno_map.hpp"
// @ref ?Is_Sensed@UnknownCellClass@@QAE_NH@Z=0047c2e0

bool TechnoClass::unknown_slot192(HouseClass *house)
{
    if (house) {
        return Map_Cell(Center_Coord())->Is_Sensed(house->ArrayIndex);
    }
    return false;
}
