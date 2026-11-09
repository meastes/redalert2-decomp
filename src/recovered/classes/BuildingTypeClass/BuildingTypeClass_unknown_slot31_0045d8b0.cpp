// CoordStruct BuildingTypeClass::unknown_slot31() const (0x0045D8B0)
// The foundation's extent in leptons, Height= levels high.
#include "shared/building_type.hpp"
// @ref ?unknown_foundation_width_7cfc70@@3QBHB=007cfc70
// @ref ?unknown_foundation_height_7cfcc8@@3QBHB=007cfcc8
// @ref _unknown_geometry_g=00850298

CoordStruct BuildingTypeClass::unknown_slot31() const
{
    return CoordStruct(unknown_foundation_width_7cfc70[Foundation] * 256,
                       unknown_foundation_height_7cfcc8[Foundation] * 256,
                       Height * unknown_geometry_g);
}
