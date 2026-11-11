// CoordStruct ObjectTypeClass::Lepton_Dimensions() const (0x005D5D30)
#include "shared/object_type.hpp"
// @ref ?unknown_a72fdc@@3HA=00a72fdc

CoordStruct ObjectTypeClass::Lepton_Dimensions() const
{
    CoordStruct coord;
    coord.X = 0x100;
    coord.Y = 0x100;
    coord.Z = unknown_a72fdc * 2;
    return coord;
}
