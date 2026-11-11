// bool BuildingTypeClass::Can_Always_Undeploy() const (0x0045EC40)
// TibSun builtype.cpp: IsLimpetMine || IsMobileWar; RA2 dropped both features and the query is false.
#include "shared/building_type_class.hpp"

bool BuildingTypeClass::Can_Always_Undeploy() const
{
    return false;
}
