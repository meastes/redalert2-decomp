// bool BuildingTypeClass::Legal_Placement(Cell const &cell, HouseClass *house) const (0x0045D880)
// PlaceAnywhere types may always be placed.
#include "shared/building_type.hpp"
// @ref ?Legal_Placement@TechnoTypeClass@@UBE_NABVCell@@PAVHouseClass@@@Z=006de0e0

bool BuildingTypeClass::Legal_Placement(Cell const &cell, HouseClass *house) const
{
    if (PlaceAnywhere) {
        return true;
    }
    return TechnoTypeClass::Legal_Placement(cell, house);
}
