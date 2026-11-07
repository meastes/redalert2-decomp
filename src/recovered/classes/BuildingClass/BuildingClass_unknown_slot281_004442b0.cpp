// int BuildingClass::unknown_slot281(ObjectClass *object) (0x004442B0, slot 281)
// The distance in leptons from the object's center to its dock coordinate
// here (slot 41), through TibSun TPoint3D::Distance_To (shared/coord.hpp).
#include "shared/building.hpp"
// @ref ?Fast_Sqrt@@YAMN@Z=004ba410
// @ref __ftol=0077f110

int BuildingClass::unknown_slot281(ObjectClass *object)
{
    return unknown_slot41((int)object).Distance_To(object->Center_Coord());
}
