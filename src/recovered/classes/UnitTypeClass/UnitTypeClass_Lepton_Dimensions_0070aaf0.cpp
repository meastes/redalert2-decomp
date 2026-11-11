// CoordStruct UnitTypeClass::Lepton_Dimensions() const (0x0070AAF0)
// One cell square, twice the 0x00AD0600 height.
#include "shared/unit_type.hpp"
// @ref ?unknown_ad0600@@3HA=00ad0600

CoordStruct UnitTypeClass::Lepton_Dimensions() const
{
    return CoordStruct(256, 256, unknown_ad0600 * 2);
}
