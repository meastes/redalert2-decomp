// void *IsometricTileTypeClass::Get_Image_Data() const (0x005293D0)
// The image, loaded on first use when the type demand-loads it (+0x2C4).
// The unit implements the vector accessors (as its Load, 0x0052E1C0): the
// first source of the unit that includes vector_class.hpp defines them.
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/vector_class.hpp"
#include "shared/isometric_tile_type.hpp"
// @ref ?Load_Tile_Data@IsometricTileTypeClass@@QAEXXZ=0052b560

void *IsometricTileTypeClass::Get_Image_Data() const
{
    void *image = unknown_a4;
    if (image == 0 && unknown_2c4) {
        ((IsometricTileTypeClass *)this)->Load_Tile_Data();
        image = unknown_a4;
    }
    return image;
}
