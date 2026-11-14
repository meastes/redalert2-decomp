// void SubTitle::Set_RGB_Color(unsigned char red, unsigned char green, unsigned char blue) -- 0x0069B100
// Purpose: packs the caption colour as 0xRRGGBB (+0x08).
// Assumptions: name from the ra2-decomp YR twin (SubTitle::Set_RGB_Color, straw.cpp).
#include "shared/subtitle.hpp"

void SubTitle::Set_RGB_Color(unsigned char red, unsigned char green, unsigned char blue)
{
    Color = ((unsigned long)red << 16) | ((unsigned long)green << 8) | (unsigned long)blue;
}
