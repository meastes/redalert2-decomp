// void UnknownBitmapWriter::Load_Image_0068ca20(char const *name) (0x0068CA20)
// Loads a .pcx file into the cache keeping its palette, each pixel replaced by
// its palette colour's red value (Load_Image_0068c600(name, 1, 1)).
#include "shared/pcx_cache.hpp"

void UnknownBitmapWriter::Load_Image_0068ca20(char const *name)
{
    Load_Image_0068c600(name, 1, 1);
}
