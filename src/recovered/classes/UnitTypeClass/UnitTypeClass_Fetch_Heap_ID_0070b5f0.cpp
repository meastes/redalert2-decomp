// int UnitTypeClass::Fetch_Heap_ID() const (0x0070B5F0)
// The type's index in UnitTypes.
#include "shared/unit_type.hpp"

int UnitTypeClass::Fetch_Heap_ID() const
{
    return ArrayIndex;
}
