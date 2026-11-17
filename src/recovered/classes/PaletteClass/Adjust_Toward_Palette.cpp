// void PaletteClass::Adjust(int ratio, PaletteClass const &palette) (0x006030D0): every entry toward palette's.
#include "shared/convert.hpp"

void PaletteClass::Adjust(int ratio, PaletteClass const &palette)
{
    for (int index = 0; index < 256; index++) {
        Palette[index].Adjust(ratio, palette[index]);
    }
}
