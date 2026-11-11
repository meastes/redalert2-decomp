// int AircraftTypeClass::Fetch_Heap_ID() const (0x0041C330)
// The type's index in AircraftTypes.
#include "shared/aircraft_type.hpp"

int AircraftTypeClass::Fetch_Heap_ID() const
{
    return ArrayIndex;
}
