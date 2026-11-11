// CoordStruct AircraftTypeClass::Lepton_Dimensions() const (0x0041BFA0)
// One cell square, twice the 0x0083C550 height.
#include "shared/aircraft_type.hpp"
// @ref _unknown_geometry_g=0083c550

CoordStruct AircraftTypeClass::Lepton_Dimensions() const
{
    return CoordStruct(256, 256, unknown_geometry_g * 2);
}
