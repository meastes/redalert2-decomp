// int InfantryTypeClass::Fetch_Heap_ID() const (0x0050AF80)
// The type's index in InfantryTypes.
#include "shared/infantry_type.hpp"

int InfantryTypeClass::Fetch_Heap_ID() const
{
    return ArrayIndex;
}
