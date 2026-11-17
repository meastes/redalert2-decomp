// void PaletteClass::Partial_Adjust(int ratio, PaletteClass const &palette, bool *lookup) (0x00603160)
// Moves each entry whose lookup flag is set toward the same entry of palette
// by ratio/256 (RGBClass::Adjust, 0x0063CF20). Westwood's palette.cpp shape.
#include "shared/convert.hpp"

void PaletteClass::Partial_Adjust(int ratio, PaletteClass const &palette, bool *lookup)
{
    for (int index = 0; index < 256; index++) {
        if (lookup[index]) {
            Palette[index].Adjust(ratio, palette[index]);
        }
    }
}
