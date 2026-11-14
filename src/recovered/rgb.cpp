// rgb.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/rgb.json carries the addresses and the rows that
// stay in their own files.


#define RGB_DECLARE_BRIGHTEN
#define RGB_DECLARE_TO_HSV
#include "shared/hsv_class.hpp"
#include "shared/rgb_class.hpp"
#define MIN(a,b) (((b) < (a)) ? (b) : (a))
#define MAX(a,b) (((b) > (a)) ? (b) : (a))


RGBClass::RGBClass(RGBClass const &from, RGBClass const &to, float ratio)
{
    Red = (unsigned char)MAX(MIN(from.Red * (1.0f - ratio) + to.Red * ratio, 255.0f), 0.0f);
    Green = (unsigned char)MAX(MIN(from.Green * (1.0f - ratio) + to.Green * ratio, 255.0f), 0.0f);
    Blue = (unsigned char)MAX(MIN(from.Blue * (1.0f - ratio) + to.Blue * ratio, 255.0f), 0.0f);
}


// RGBClass &RGBClass::Set(RGBClass const &color, float factor) -- 0x0063CDF0 (TS rgb.cpp)
// This colour as color scaled by factor, each channel clamped to 0..255.

RGBClass &RGBClass::Set(RGBClass const &color, float factor)
{
    Red = (unsigned char)MAX(MIN(color.Red * factor, 255.0f), 0.0f);
    Green = (unsigned char)MAX(MIN(color.Green * factor, 255.0f), 0.0f);
    Blue = (unsigned char)MAX(MIN(color.Blue * factor, 255.0f), 0.0f);
    return *this;
}


// void RGBClass::Adjust(int ratio, RGBClass const &target) (0x0063CF20)
// Moves each channel toward target's by (ratio & 0xFF)/256 (Westwood's
// RGBClass::Adjust). Re-plan of the accepted
// RgbTriplet::BlendRgbTowardTarget_0063cf20 (population-32) under its class
// name; the channels are read and written in the original's order.

void RGBClass::Adjust(int ratio, RGBClass const &target)
{
    int weight = ratio & 0xFF;
    Red = (unsigned char)(Red + ((int)target.Red - (int)Red) * weight / 256);
    Green = (unsigned char)(Green + ((int)target.Green - (int)Green) * weight / 256);
    Blue = (unsigned char)(Blue + ((int)target.Blue - (int)Blue) * weight / 256);
}


// int RGBClass::Difference(RGBClass const &other) const (0x0063CFB0)
// The weighted distance 2|dRed| + 4|dGreen| + 3|dBlue|.

int RGBClass::Difference(RGBClass const &other) const
{
    int red = (int)Red - (int)other.Red;
    if (red < 0) red = -red;
    int green = (int)Green - (int)other.Green;
    if (green < 0) green = -green;
    int blue = (int)Blue - (int)other.Blue;
    if (blue < 0) blue = -blue;
    return blue * 3 + (red + green * 2) * 2;
}


// RGBClass::operator HSVClass() const -- 0x0063D000 (TS rgb.cpp)
// The colour as hue (0-255 over six sectors), saturation and value.

RGBClass::operator HSVClass() const
{
    int hue;
    int saturation;
    int value;

    int red = Red;
    int green = Green;
    int blue = Blue;

    hue = 0;

    value = (red > green) ? red : green;
    if (blue > value) value = blue;

    int white = (red < green) ? red : green;
    if (blue < white) white = blue;

    saturation = 0;
    if (value) {
        saturation = ((value - white) * 255) / value;
    }

    if (saturation != 0) {
        unsigned int tmp = value - white;
        unsigned int r1 = ((value - red) * 255) / tmp;
        unsigned int g1 = ((value - green) * 255) / tmp;
        unsigned int b1 = ((value - blue) * 255) / tmp;
        if (value == red) {
            if (white == green) {
                tmp = 5 * 256 + b1;
            } else {
                tmp = 1 * 256 - g1;
            }
        } else {
            if (value == green) {
                if (white == blue) {
                    tmp = 1 * 256 + r1;
                } else {
                    tmp = 3 * 256 - b1;
                }
            } else {
                if (white == red) {
                    tmp = 3 * 256 + g1;
                } else {
                    tmp = 5 * 256 - r1;
                }
            }
        }
        hue = tmp / 6;
    }
    HSVClass hsv((unsigned char)hue, (unsigned char)saturation, (unsigned char)value);
    return hsv;
}
