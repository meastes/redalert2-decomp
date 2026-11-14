// lightcalc.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/lightcalc.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unknown_globals.hpp"


// Normalize tile tint and scale light intensity, exact-target 005383C0.
// RA2 adds an upper clamp to all three channels before normalization.
void __fastcall Adjust_Tile_RGB(int &intensity, int &light, int &red, int &green, int &blue)
{
    red = red > 0 ? red : 0;
    green = green > 0 ? green : 0;
    blue = blue > 0 ? blue : 0;
    red = red >= 2000 ? 2000 : red;
    green = green >= 2000 ? 2000 : green;
    blue = blue >= 2000 ? 2000 : blue;
    intensity = 65536;
    if (red != 1000 || green != red || blue != red) {
        if (green <= red && red >= blue) {
            double scale = (double)red * 65536.0;
            scale /= 1000.0;
            intensity = (int)scale;
            if (intensity < 66) {
                intensity = 65536;
                red = 1000; green = 1000; blue = 1000;
                light = 0;
            } else {
                red = 1000;
                green = (int)((double)green * 65536.0 / scale);
                blue = (int)((double)blue * 65536.0 / scale);
                light = (light * intensity) >> 16;
            }
        } else if (green >= red && green >= blue) {
            double scale = (double)green * 65536.0;
            scale /= 1000.0;
            intensity = (int)scale;
            if (intensity < 66) {
                intensity = 65536;
                red = 1000; green = 1000; blue = 1000;
                light = 0;
            } else {
                green = 1000;
                red = (int)((double)red * 65536.0 / (double)intensity);
                blue = (int)((double)blue * 65536.0 / (double)intensity);
                light = (light * intensity) >> 16;
            }
        } else {
            double scale = (double)blue * 65536.0;
            scale /= 1000.0;
            intensity = (int)scale;
            if (intensity < 66) {
                intensity = 65536;
                red = 1000; green = 1000; blue = 1000;
                light = 0;
            } else {
                blue = 1000;
                red = (int)((double)red * 65536.0 / (double)intensity);
                green = (int)((double)green * 65536.0 / (double)intensity);
                light = (light * intensity) >> 16;
            }
        }
    }
    light = light >= 2000 ? 2000 : light;
}


class Leaf_5383a0 { public: void get(); };
void Leaf_5383a0::get() { unsigned int unused_this_address = (unsigned int)this; unsigned int zero_value; unsigned int unused_value; zero_value = 0; *(unsigned int*)(0xa6e888) = zero_value; *(unsigned int*)(0xa6e88c) = zero_value; *(unsigned int*)(0xa6e890) = zero_value; *(unsigned int*)(0xa6e894) = zero_value; }


// void __fastcall Unknown_Quantize_Color_005385a0(int *red, int *green, int *blue) (0x005385A0)
// Clamps three colour components to 0..1000 and drops their low bits by the detail level
// (0x00A40B2C: 0 keeps multiples of 128, 1 of 64, 2 of 32, anything else keeps them).

void __fastcall Unknown_Quantize_Color_005385a0(int *red, int *green, int *blue)
{
    int value = *red;
    if (value >= 1000) {
        value = 1000;
    }
    *red = value;
    value = *green;
    if (value >= 1000) {
        value = 1000;
    }
    *green = value;
    value = *blue;
    if (value >= 1000) {
        value = 1000;
    }
    *blue = value;
    *red = (*red <= 0 ? 0 : *red);
    *green = (*green <= 0 ? 0 : *green);
    *blue = (*blue <= 0 ? 0 : *blue);
    switch (unknown_a40b2c) {
    case 0:
        *red &= ~127;
        *green &= ~127;
        *blue &= ~127;
        break;
    case 1:
        *red &= ~63;
        *green &= ~63;
        *blue &= ~63;
        break;
    case 2:
        *red &= ~31;
        *green &= ~31;
        *blue &= ~31;
        break;
    }
}
