// int BuildingClass::Sort_Y() const (0x00445380, slot 45)
// The object value, raised by 32 for type flag +0x135F and lowered by 16
// for type flag +0x1351.
#include "shared/building.hpp"
// @ref ?Sort_Y@ObjectClass@@UBEHXZ=005d5450

int BuildingClass::Sort_Y() const
{
    int value = ObjectClass::Sort_Y();
    return value + ((Type->TurretAnimIsVoxel ? 32 : 0) - (Type->Gate ? 16 : 0));
}
