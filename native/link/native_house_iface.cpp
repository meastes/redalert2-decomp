// Native adapter (Scout1115, native M13; port/fixups.json): the player's IHouse
// Available_Money (HouseClass slot 0x004E2B30) for units that reach the interface
// through its VC6 offset (HouseClass +0x24). Not original code; same result.
#include "shared/house.hpp"

long Native_House_Available_Money(HouseClass *house)
{
    return house->Available_Money();
}
