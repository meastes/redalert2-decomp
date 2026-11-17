// void PaletteClass::Partial_Adjust(int ratio, char *lookup) (0x00603120): the flagged entries toward black.
#include "shared/convert.hpp"
#include "shared/rgb_globals.hpp"

void PaletteClass::Partial_Adjust(int ratio, char *lookup)
{
    for (int index = 0; index < 256; index++) {
        if (lookup[index]) {
            Palette[index].Adjust(ratio, BlackColor);
        }
    }
}
