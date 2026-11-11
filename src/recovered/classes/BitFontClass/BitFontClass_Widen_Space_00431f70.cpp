// void BitFontClass::Widen_Space() (0x00431F70)
// One pixel taller, with the space one pixel wider.
#include "shared/text_print.hpp"

void BitFontClass::Widen_Space()
{
    if (Data) {
        Height++;
        (*Glyph_Data(' '))++;
    }
}
