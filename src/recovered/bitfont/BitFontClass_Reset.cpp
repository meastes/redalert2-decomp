// void BitFontClass::Reset() -- 0x004321B0
// Purpose: the font's default state: no font data, no surface, no missing-glyph copy,
// colours 0x7FFF/0x3555, tab width 0x40, clipping on, one-pixel metrics, an empty clip box.
// The out-of-line copy of the member bitfont.cpp expands inline in its constructor
// (names.csv ?Reset@BitFont@@; no direct callers).
#include "shared/win32_api.hpp"
#include "shared/text_print.hpp"

void BitFontClass::Reset()
{
    Data = 0;
    Color = 0x7FFF;
    unknown_26 = 0x3555;
    unknown_28 = 0x40;
    unknown_40 = 1;
    Clipping = 1;
    Width = 1;
    Height = 1;
    unknown_20 = 0;
    Buffer = 0;
    unknown_08 = 0;
    SetRect((RECT *)&ClipBox, 0, 0, 0, 0);
    unknown_2c = 1;
}
