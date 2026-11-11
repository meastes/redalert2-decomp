// int BuildingTypeClass::Fetch_Heap_ID() const (0x0045ED20)
// The type's index in BuildingTypes.
#include "shared/building_type.hpp"

int BuildingTypeClass::Fetch_Heap_ID() const
{
    return ArrayIndex;
}
