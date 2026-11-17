// void PaletteClass::Adjust(int ratio) (0x006030A0): every entry toward black by ratio/256.
#include "shared/convert.hpp"
#include "shared/rgb_globals.hpp"

void PaletteClass::Adjust(int ratio)
{
    for (int index = 0; index < 256; index++) {
        Palette[index].Adjust(ratio, BlackColor);
    }
}
