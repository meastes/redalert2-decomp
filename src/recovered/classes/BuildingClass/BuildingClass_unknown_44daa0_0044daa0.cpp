// bool BuildingClass::unknown_44daa0(ObjectClass *object) (0x0044DAA0)
// Whether the distance from the building's centre to the object's is at
// most the type's PsychicDetectionRadius in cells (256 leptons each).
#include "shared/building.hpp"
// @ref ?Fast_Sqrt@@YAMN@Z=004ba410
// @ref __ftol=0077f110

bool BuildingClass::unknown_44daa0(ObjectClass *object)
{
    CoordStruct here = Center_Coord();
    CoordStruct there = object->Center_Coord();
    return here.Distance_To(there) <= Type->PsychicDetectionRadius << 8;
}
