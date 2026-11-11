// CoordStruct ObjectTypeClass::Pixel_Dimensions() const (0x005D5D10)
#include "shared/object_type.hpp"

CoordStruct ObjectTypeClass::Pixel_Dimensions() const
{
    CoordStruct coord;
    coord.X = 10;
    coord.Y = 10;
    coord.Z = 10;
    return coord;
}
