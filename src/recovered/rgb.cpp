/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/rgb.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.
// Parameter names from mischa85/RedAlert2 392e904 src/rgb.cpp where both define the function (names only).
// Local variable names from mischa85/RedAlert2 392e904 src/rgb.cpp where both functions declare the same local types (names only).

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


/// <summary>
/// Sets this color to a scaled version of another color.
/// This routine will multiply each color component of the source color by the value
/// specified, clamping the result to the legal color range.
/// </summary>
/// <param name="value">The scale to apply to each color component.</param>
/// <returns>Returns with a reference to this color object.</returns>
// (0x0063CDF0)
// This colour as color scaled by factor, each channel clamped to 0..255.

RGBClass &RGBClass::Set(RGBClass const &rgb, float value)
{
    Red = (unsigned char)MAX(MIN(rgb.Red * value, 255.0f), 0.0f);
    Green = (unsigned char)MAX(MIN(rgb.Green * value, 255.0f), 0.0f);
    Blue = (unsigned char)MAX(MIN(rgb.Blue * value, 255.0f), 0.0f);
    return *this;
}


/***********************************************************************************************
 * RGBClass::Adjust -- Adjust one RGB value toward another.                                    *
 *                                                                                             *
 *    This routine is used to modify an RGB value to proportionately match another RGB value   *
 *    according to the ratio parameter specified. Typical use of this routine is in palette    *
 *    fading from one palette to another or to black.                                          *
 *                                                                                             *
 * INPUT:   ratio    -- The ration of transformation. This value is in the form of 0 to 255,   *
 *                      with 0 being no change, and 255 being 100% transformed into the        *
 *                      destination color.                                                     *
 *                                                                                             *
 *          rgb      -- Reference to the destination RGB color to transform this color into.   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0063CF20)
// Moves each channel toward target's by (ratio & 0xFF)/256 (Westwood's
// RGBClass::Adjust). Re-plan of the accepted
// RgbTriplet::BlendRgbTowardTarget_0063cf20 (population-32) under its class
// name; the channels are read and written in the original's order.

void RGBClass::Adjust(int ratio, RGBClass const &rgb)
{
    int value = ratio & 0xFF;
    Red = (unsigned char)(Red + ((int)rgb.Red - (int)Red) * value / 256);
    Green = (unsigned char)(Green + ((int)rgb.Green - (int)Green) * value / 256);
    Blue = (unsigned char)(Blue + ((int)rgb.Blue - (int)Blue) * value / 256);
}


/***********************************************************************************************
 * RGBClass::Difference -- Determines the "distance" between two colors.                       *
 *                                                                                             *
 *    This routine is used to calculate a relative distance between two colors. The value is   *
 *    relative only to itself and thus is useful only for determining the magnitude of         *
 *    color difference rather than the nature of the color difference. Palette remapping       *
 *    code uses this routine to find closest matches for colors.                               *
 *                                                                                             *
 * INPUT:   rgb   -- Reference to the color to be compared to this color.                      *
 *                                                                                             *
 * OUTPUT:  Returns the difference between the two colors. The value returned is zero if the   *
 *          colors exactly match. The greater the positive value the greater the difference    *
 *          between the colors.                                                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0063CFB0)
// The weighted distance 2|dRed| + 4|dGreen| + 3|dBlue|.

int RGBClass::Difference(RGBClass const &rgb) const
{
    int red = (int)Red - (int)rgb.Red;
    if (red < 0) red = -red;
    int green = (int)Green - (int)rgb.Green;
    if (green < 0) green = -green;
    int blue = (int)Blue - (int)rgb.Blue;
    if (blue < 0) blue = -blue;
    return blue * 3 + (red + green * 2) * 2;
}


/***********************************************************************************************
 * RGBClass::operator HSVClass -- Conversion operator for RGB to HSV object.                   *
 *                                                                                             *
 *    This conversion operator will convert an RGBClass object into an HSVClass object.        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a reference (implicit) to the HSVClass object that most closely       *
 *          represents the RGBClass object.                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/20/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0063D000)
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
