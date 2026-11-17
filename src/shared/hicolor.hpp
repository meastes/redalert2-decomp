// High-colour pixel building for the DirectDraw display (dsurface.cpp).
//
// The screen's pixel format is kept as a right/left shift pair per channel
// (0x00852FB8-0x00852FCC, set up with the primary surface): a channel value
// 0-255 becomes ((value >> right) << left). Which shift belongs to which
// channel follows from the 3-byte palette colours (Red, Green, Blue) the
// callers pass; the original names are not known.
#ifndef RA2_SHARED_HICOLOR_HPP
#define RA2_SHARED_HICOLOR_HPP

#include "shared/rgb_class.hpp"

class PaletteClass;

extern int DSurface_RedRight;    // red left
extern int DSurface_RedLeft;    // red right
extern int DSurface_BlueRight;    // blue left
extern int DSurface_BlueLeft;    // blue right
extern int DSurface_GreenRight;    // green left
extern int DSurface_GreenLeft;    // green right

// Set when the display can stretch blits (0x00852FD6; DSurface::Create_Primary).
extern bool AllowStretchBlits_852fd6;      // 0x00852FD6

// Packs 0-255 channel values into a screen pixel (always expanded inline).
inline int Build_Hicolor_Pixel(int red, int green, int blue)
{
    return ((red >> DSurface_RedLeft) << DSurface_RedRight)
         | ((green >> DSurface_GreenLeft) << DSurface_GreenRight)
         | ((blue >> DSurface_BlueLeft) << DSurface_BlueRight);
}

// A screen pixel back to 8-bit channels, the inverse of Build_Hicolor_Pixel
// (always expanded inline; FootClass slot 243, HouseClass::Read_INI).
inline RGBClass Unpack_Hicolor_Pixel(unsigned short pixel)
{
    RGBClass color;
    color.Red = (pixel >> DSurface_RedRight) << DSurface_RedLeft;
    color.Green = (pixel >> DSurface_GreenRight) << DSurface_GreenLeft;
    color.Blue = (pixel >> DSurface_BlueRight) << DSurface_BlueLeft;
    return color;
}

// TibSun DSurface::Deconstruct_Hicolor_Pixel: a screen pixel back to 8-bit
// channels through the three-channel constructor.
inline RGBClass Deconstruct_Hicolor_Pixel(unsigned short color)
{
    return RGBClass(color >> DSurface_RedRight << DSurface_RedLeft,
                    color >> DSurface_GreenRight << DSurface_GreenLeft,
                    color >> DSurface_BlueRight << DSurface_BlueLeft);
}

// Fills count consecutive 256-entry pixel tables with palette scaled from
// black (first table) to double brightness (last table) (0x004ABA00).
void __fastcall Build_Brightness_Tables(unsigned short *table, int count, PaletteClass const &palette);

// Adds the deltas to a palette colour; the overflow of saturated channels is
// shared out over the others (halved when only one channel saturates), then
// the result is packed into a screen pixel (0x004AE8D0).
int __fastcall Brighten_Hicolor_Pixel(RGBClass const &color, int red_add, int green_add, int blue_add);

// Screen pixel masks keeping the bits that survive a shift right by one
// (0x00852FD0) and by two (0x00852FD2).
extern unsigned short unknown_half_mask_852fd0;
extern unsigned short unknown_quarter_mask_852fd2;

// Mixes two screen pixels by percent (0x004AF550).
unsigned short __fastcall Blend_Hicolor_Pixel(unsigned short color, unsigned short background, int percent);

// Accessors of the shifts (0x004ABB30-0x004ABB80) and of 0x007D6290
// (0x004ABB90); names after Westwood's DSurface::Get_Red_Left etc.
extern int unknown_7d6290;
int Get_Red_Left();         // 0x004ABB30
int Get_Red_Right();        // 0x004ABB40
int Get_Green_Left();       // 0x004ABB50
int Get_Green_Right();      // 0x004ABB60
int Get_Blue_Left();        // 0x004ABB70
int Get_Blue_Right();       // 0x004ABB80
int __fastcall Get_Unknown_7d6290();   // 0x004ABB90

#endif
